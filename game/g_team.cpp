// Copyright (C) 1999-2000 Id Software, Inc.
//

#include "g_local.h"
#include "bg_saga.h"

typedef struct teamgame_s {
    float last_flag_capture;
    int last_capture_team;
    flagStatus_t redStatus;  // CTF
    flagStatus_t blueStatus; // CTF
    flagStatus_t flagStatus; // One Flag CTF
    int redTakenTime;
    int blueTakenTime;
    int bestCapTime;
    char bestCapName[MAX_NETNAME];
} teamgame_t;

teamgame_t teamgame;

void Team_SetFlagStatus(int team, flagStatus_t status);

void Team_InitGame(void) {
    memset(&teamgame, 0, sizeof teamgame);

    switch (level.gametype) {
    case GT_CTF:
    case GT_CTY:
        teamgame.redStatus = (flagStatus_t)-1; // Invalid to force update
        Team_SetFlagStatus(TEAM_RED, FLAG_ATBASE);
        teamgame.blueStatus = (flagStatus_t)-1;
        Team_SetFlagStatus(TEAM_BLUE, FLAG_ATBASE);
        teamgame.bestCapTime = -1;
        break;
    default:
        break;
    }
}

int OtherTeam(int team) {
    if (team == TEAM_RED)
        return TEAM_BLUE;
    else if (team == TEAM_BLUE)
        return TEAM_RED;
    return team;
}

const char *TeamName(int team) {
    if (team == TEAM_RED)
        return "RED";
    else if (team == TEAM_BLUE)
        return "BLUE";
    else if (team == TEAM_SPECTATOR)
        return "SPECTATOR";
    return "FREE";
}

const char *OtherTeamName(int team) {
    if (team == TEAM_RED)
        return "BLUE";
    else if (team == TEAM_BLUE)
        return "RED";
    else if (team == TEAM_SPECTATOR)
        return "SPECTATOR";
    return "FREE";
}

const char *TeamColorString(int team) {
    if (team == TEAM_RED)
        return S_COLOR_RED;
    else if (team == TEAM_BLUE)
        return S_COLOR_BLUE;
    else if (team == TEAM_SPECTATOR)
        return S_COLOR_YELLOW;
    return S_COLOR_WHITE;
}

// plIndex used to print pl->client->pers.netname
// teamIndex used to print team name
void PrintCTFMessage(int plIndex, int teamIndex, int ctfMessage) {
    gentity_t *te;

    if (plIndex == -1) {
        plIndex = MAX_CLIENTS + 1;
    }
    if (teamIndex == -1) {
        teamIndex = 50;
    }

    te = G_TempEntity(&vec3_origin, EV_CTFMESSAGE);
    te->r.svFlags |= SVF_BROADCAST;
    te->s.eventParm = ctfMessage;
    te->s.trickedEntIndex[0] = plIndex;
    if (ctfMessage == CTFMESSAGE_PLAYER_CAPTURED_FLAG) {
        if (teamIndex == TEAM_RED) {
            te->s.trickedEntIndex[1] = TEAM_BLUE;
        } else {
            te->s.trickedEntIndex[1] = TEAM_RED;
        }
    } else {
        te->s.trickedEntIndex[1] = teamIndex;
    }
}

// used for gametype > GT_TEAM
// for gametype GT_TEAM the level.teamScores is updated in AddScore in g_combat.c
void AddTeamScore(vector3 *origin, int team, int score) {
    gentity_t *te;

    te = G_TempEntity(origin, EV_GLOBAL_TEAM_SOUND);
    te->r.svFlags |= SVF_BROADCAST;

    if (team == TEAM_RED) {
        if (level.teamScores[TEAM_RED] + score == level.teamScores[TEAM_BLUE]) {
            // teams are tied sound
            te->s.eventParm = GTS_TEAMS_ARE_TIED;
        } else if (level.teamScores[TEAM_RED] <= level.teamScores[TEAM_BLUE] && level.teamScores[TEAM_RED] + score > level.teamScores[TEAM_BLUE]) {
            // red took the lead sound
            te->s.eventParm = GTS_REDTEAM_TOOK_LEAD;
        } else {
            // red scored sound
            te->s.eventParm = GTS_REDTEAM_SCORED;
        }
    } else {
        if (level.teamScores[TEAM_BLUE] + score == level.teamScores[TEAM_RED]) {
            // teams are tied sound
            te->s.eventParm = GTS_TEAMS_ARE_TIED;
        } else if (level.teamScores[TEAM_BLUE] <= level.teamScores[TEAM_RED] && level.teamScores[TEAM_BLUE] + score > level.teamScores[TEAM_RED]) {
            // blue took the lead sound
            te->s.eventParm = GTS_BLUETEAM_TOOK_LEAD;
        } else {
            // blue scored sound
            te->s.eventParm = GTS_BLUETEAM_SCORED;
        }
    }
    level.teamScores[team] += score;
}

qboolean OnSameTeam(gentity_t *ent1, gentity_t *ent2) {
    if (!ent1->client || !ent2->client) {
        return qfalse;
    }

    if (level.gametype == GT_POWERDUEL) {
        if (ent1->client->sess.duelTeam == ent2->client->sess.duelTeam) {
            return qtrue;
        }

        return qfalse;
    }

    if (level.gametype == GT_SINGLE_PLAYER) {
        qboolean ent1IsBot = qfalse;
        qboolean ent2IsBot = qfalse;

        if (ent1->r.svFlags & SVF_BOT) {
            ent1IsBot = qtrue;
        }
        if (ent2->r.svFlags & SVF_BOT) {
            ent2IsBot = qtrue;
        }

        if ((ent1IsBot && ent2IsBot) || (!ent1IsBot && !ent2IsBot)) {
            return qtrue;
        }
        return qfalse;
    }

    if (level.gametype < GT_TEAM) {
        return qfalse;
    }

    if (ent1->s.eType == ET_NPC && ent1->s.NPC_class == CLASS_VEHICLE && ent1->client && ent1->client->sess.sessionTeam != TEAM_FREE && ent2->client &&
        ent1->client->sess.sessionTeam == ent2->client->sess.sessionTeam) {
        return qtrue;
    }
    if (ent2->s.eType == ET_NPC && ent2->s.NPC_class == CLASS_VEHICLE && ent2->client && ent2->client->sess.sessionTeam != TEAM_FREE && ent1->client &&
        ent2->client->sess.sessionTeam == ent1->client->sess.sessionTeam) {
        return qtrue;
    }

    if (ent1->client->sess.sessionTeam == TEAM_FREE && ent2->client->sess.sessionTeam == TEAM_FREE && ent1->s.eType == ET_NPC &&
        ent2->s.eType == ET_NPC) { // NPCs don't do normal team rules
        return qfalse;
    }

    if (ent1->s.eType == ET_NPC && ent2->s.eType == ET_PLAYER) {
        return qfalse;
    } else if (ent1->s.eType == ET_PLAYER && ent2->s.eType == ET_NPC) {
        return qfalse;
    }

    if (ent1->client->sess.sessionTeam == ent2->client->sess.sessionTeam) {
        return qtrue;
    }

    return qfalse;
}

static char ctfFlagStatusRemap[] = {'0', '1', '*', '*', '2'};

void Team_SetFlagStatus(int team, flagStatus_t status) {
    qboolean modified = qfalse;

    switch (team) {
    case TEAM_RED: // CTF
        if (teamgame.redStatus != status) {
            teamgame.redStatus = status;
            modified = qtrue;
        }
        break;

    case TEAM_BLUE: // CTF
        if (teamgame.blueStatus != status) {
            teamgame.blueStatus = status;
            modified = qtrue;
        }
        break;

    case TEAM_FREE: // One Flag CTF
        if (teamgame.flagStatus != status) {
            teamgame.flagStatus = status;
            modified = qtrue;
        }
        break;

    default:
        break;
    }

    if (modified) {
        char st[4];

        if (level.gametype == GT_CTF || level.gametype == GT_CTY) {
            st[0] = ctfFlagStatusRemap[teamgame.redStatus];
            st[1] = ctfFlagStatusRemap[teamgame.blueStatus];
            st[2] = 0;
        }

        trap->SetConfigstring(CS_FLAGSTATUS, st);
    }
}

void Team_CheckDroppedItem(gentity_t *dropped) {
    if (dropped->item->giTag == PW_REDFLAG) {
        Team_SetFlagStatus(TEAM_RED, FLAG_DROPPED);
    } else if (dropped->item->giTag == PW_BLUEFLAG) {
        Team_SetFlagStatus(TEAM_BLUE, FLAG_DROPPED);
    } else if (dropped->item->giTag == PW_NEUTRALFLAG) {
        Team_SetFlagStatus(TEAM_FREE, FLAG_DROPPED);
    }
}

void Team_ForceGesture(int team) {
    int i;
    gentity_t *ent;

    for (i = 0; i < MAX_CLIENTS; i++) {
        ent = &g_entities[i];
        if (!ent->inuse)
            continue;
        if (!ent->client)
            continue;
        if (ent->client->sess.sessionTeam != team)
            continue;
        //
        ent->flags |= FL_FORCE_GESTURE;
    }
}

// Calculate the bonuses for flag defense, flag carrier defense, etc.
// Note that bonuses are not cumulative. You get one, they are in importance order.
void Team_FragBonuses(gentity_t *targ, gentity_t *inflictor, gentity_t *attacker) {
    int i, team, flag_pw, enemy_flag_pw, otherteam;
    gentity_t *ent, *flag, *carrier = NULL;
    const char *c;
    vector3 v1, v2;

    // no bonus for fragging yourself or team mates
    if (!targ->client || !attacker->client || targ == attacker || OnSameTeam(targ, attacker))
        return;

    team = targ->client->sess.sessionTeam;
    otherteam = OtherTeam(targ->client->sess.sessionTeam);
    if (otherteam < 0)
        return; // whoever died isn't on a team

    // same team, if the flag at base, check to he has the enemy flag
    if (team == TEAM_RED) {
        flag_pw = PW_REDFLAG;
        enemy_flag_pw = PW_BLUEFLAG;
    } else {
        flag_pw = PW_BLUEFLAG;
        enemy_flag_pw = PW_REDFLAG;
    }

    // did the attacker frag the flag carrier?
    if (targ->client->ps.powerups[enemy_flag_pw]) {
        attacker->client->pers.teamState.lastfraggedcarrier = level.time;
        AddScore(attacker, &targ->r.currentOrigin, CTF_FRAG_CARRIER_BONUS);
        attacker->client->pers.teamState.fragcarrier++;
        // PrintMsg(NULL, "%s" S_COLOR_WHITE " fragged %s's flag carrier!\n",
        //	attacker->client->pers.netname, TeamName(team));
        PrintCTFMessage(attacker->s.number, team, CTFMESSAGE_FRAGGED_FLAG_CARRIER);

        // the target had the flag, clear the hurt carrier
        // field on the other team
        for (i = 0; i < sv_maxclients.integer; i++) {
            ent = g_entities + i;
            if (ent->inuse && ent->client->sess.sessionTeam == otherteam)
                ent->client->pers.teamState.lasthurtcarrier = 0;
        }
        return;
    }

    if (targ->client->pers.teamState.lasthurtcarrier && level.time - targ->client->pers.teamState.lasthurtcarrier < CTF_CARRIER_DANGER_PROTECT_TIMEOUT &&
        !attacker->client->ps.powerups[flag_pw]) {
        // attacker is on the same team as the flag carrier and
        // fragged a guy who hurt our flag carrier
        AddScore(attacker, &targ->r.currentOrigin, CTF_CARRIER_DANGER_PROTECT_BONUS);

        attacker->client->pers.teamState.carrierdefense++;
        targ->client->pers.teamState.lasthurtcarrier = 0;

        attacker->client->ps.persistant[PERS_DEFEND_COUNT]++;
        attacker->client->rewardTime = level.time + REWARD_SPRITE_TIME;

        return;
    }

    if (targ->client->pers.teamState.lasthurtcarrier && level.time - targ->client->pers.teamState.lasthurtcarrier < CTF_CARRIER_DANGER_PROTECT_TIMEOUT) {
        AddScore(attacker, &targ->r.currentOrigin, CTF_CARRIER_DANGER_PROTECT_BONUS);

        attacker->client->pers.teamState.carrierdefense++;
        targ->client->pers.teamState.lasthurtcarrier = 0;

        attacker->client->ps.persistant[PERS_DEFEND_COUNT]++;
        attacker->client->rewardTime = level.time + REWARD_SPRITE_TIME;

        return;
    }

    // flag and flag carrier area defense bonuses

    // we have to find the flag and carrier entities

    // find the flag
    switch (attacker->client->sess.sessionTeam) {
    case TEAM_RED:
        c = "team_CTF_redflag";
        break;
    case TEAM_BLUE:
        c = "team_CTF_blueflag";
        break;
    default:
        return;
    }
    // find attacker's team's flag carrier
    for (i = 0; i < sv_maxclients.integer; i++) {
        carrier = g_entities + i;
        if (carrier->inuse && carrier->client->ps.powerups[flag_pw])
            break;
        carrier = NULL;
    }
    flag = NULL;
    while ((flag = G_Find(flag, FOFS(classname), c)) != NULL) {
        if (!(flag->flags & FL_DROPPED_ITEM))
            break;
    }

    if (!flag)
        return; // can't find attacker's flag

    // ok we have the attackers flag and a pointer to the carrier

    // check to see if we are defending the base's flag
    VectorSubtract(&targ->r.currentOrigin, &flag->r.currentOrigin, &v1);
    VectorSubtract(&attacker->r.currentOrigin, &flag->r.currentOrigin, &v2);

    if (((VectorLength(&v1) < CTF_TARGET_PROTECT_RADIUS && trap->InPVS(&flag->r.currentOrigin, &targ->r.currentOrigin)) ||
         (VectorLength(&v2) < CTF_TARGET_PROTECT_RADIUS && trap->InPVS(&flag->r.currentOrigin, &attacker->r.currentOrigin))) &&
        attacker->client->sess.sessionTeam != targ->client->sess.sessionTeam) {

        // we defended the base flag
        AddScore(attacker, &targ->r.currentOrigin, CTF_FLAG_DEFENSE_BONUS);
        attacker->client->pers.teamState.basedefense++;

        attacker->client->ps.persistant[PERS_DEFEND_COUNT]++;
        attacker->client->rewardTime = level.time + REWARD_SPRITE_TIME;

        return;
    }

    if (carrier && carrier != attacker) {
        VectorSubtract(&targ->r.currentOrigin, &carrier->r.currentOrigin, &v1);
        VectorSubtract(&attacker->r.currentOrigin, &carrier->r.currentOrigin, &v1);

        if (((VectorLength(&v1) < CTF_ATTACKER_PROTECT_RADIUS && trap->InPVS(&carrier->r.currentOrigin, &targ->r.currentOrigin)) ||
             (VectorLength(&v2) < CTF_ATTACKER_PROTECT_RADIUS && trap->InPVS(&carrier->r.currentOrigin, &attacker->r.currentOrigin))) &&
            attacker->client->sess.sessionTeam != targ->client->sess.sessionTeam) {
            AddScore(attacker, &targ->r.currentOrigin, CTF_CARRIER_PROTECT_BONUS);
            attacker->client->pers.teamState.carrierdefense++;

            attacker->client->ps.persistant[PERS_DEFEND_COUNT]++;
            attacker->client->rewardTime = level.time + REWARD_SPRITE_TIME;

            return;
        }
    }
}

// Check to see if attacker hurt the flag carrier.
// Needed when handing out bonuses for assistance to flag carrier defense.
void Team_CheckHurtCarrier(gentity_t *targ, gentity_t *attacker) {
    int flag_pw;

    if (!targ->client || !attacker->client)
        return;

    if (targ->client->sess.sessionTeam == TEAM_RED)
        flag_pw = PW_BLUEFLAG;
    else
        flag_pw = PW_REDFLAG;

    // flags
    if (targ->client->ps.powerups[flag_pw] && targ->client->sess.sessionTeam != attacker->client->sess.sessionTeam)
        attacker->client->pers.teamState.lasthurtcarrier = level.time;
}

gentity_t *Team_ResetFlag(int team) {
    const char *c;
    gentity_t *ent, *rent = NULL;

    switch (team) {
    case TEAM_RED:
        c = "team_CTF_redflag";
        break;
    case TEAM_BLUE:
        c = "team_CTF_blueflag";
        break;
    case TEAM_FREE:
        c = "team_CTF_neutralflag";
        break;
    default:
        return NULL;
    }

    ent = NULL;
    while ((ent = G_Find(ent, FOFS(classname), c)) != NULL) {
        if (ent->flags & FL_DROPPED_ITEM)
            G_FreeEntity(ent);
        else {
            rent = ent;
            RespawnItem(ent);
        }
    }

    Team_SetFlagStatus(team, FLAG_ATBASE);

    return rent;
}

void Team_ResetFlags(void) {
    if (level.gametype == GT_CTF || level.gametype == GT_CTY) {
        Team_ResetFlag(TEAM_RED);
        Team_ResetFlag(TEAM_BLUE);
    }
}

void Team_ReturnFlagSound(gentity_t *ent, int team) {
    gentity_t *te;

    if (ent == NULL) {
        trap->Print("Warning:  NULL passed to Team_ReturnFlagSound\n");
        return;
    }

    te = G_TempEntity(&ent->s.pos.trBase, EV_GLOBAL_TEAM_SOUND);
    if (team == TEAM_BLUE) {
        te->s.eventParm = GTS_RED_RETURN;
    } else {
        te->s.eventParm = GTS_BLUE_RETURN;
    }
    te->r.svFlags |= SVF_BROADCAST;
}

void Team_TakeFlagSound(gentity_t *ent, int team) {
    gentity_t *te;

    if (ent == NULL) {
        trap->Print("Warning:  NULL passed to Team_TakeFlagSound\n");
        return;
    }

    // only play sound when the flag was at the base
    // or not picked up the last 10 seconds
    switch (team) {
    case TEAM_RED:
        if (teamgame.blueStatus != FLAG_ATBASE) {
            if (teamgame.blueTakenTime > level.time - 10000)
                return;
        }
        teamgame.blueTakenTime = level.time;
        break;

    case TEAM_BLUE: // CTF
        if (teamgame.redStatus != FLAG_ATBASE) {
            if (teamgame.redTakenTime > level.time - 10000)
                return;
        }
        teamgame.redTakenTime = level.time;
        break;

    default:
        break;
    }

    te = G_TempEntity(&ent->s.pos.trBase, EV_GLOBAL_TEAM_SOUND);
    if (team == TEAM_BLUE) {
        te->s.eventParm = GTS_RED_TAKEN;
    } else {
        te->s.eventParm = GTS_BLUE_TAKEN;
    }
    te->r.svFlags |= SVF_BROADCAST;
}

void Team_CaptureFlagSound(gentity_t *ent, int team) {
    gentity_t *te;

    if (ent == NULL) {
        trap->Print("Warning:  NULL passed to Team_CaptureFlagSound\n");
        return;
    }

    te = G_TempEntity(&ent->s.pos.trBase, EV_GLOBAL_TEAM_SOUND);
    if (team == TEAM_BLUE) {
        te->s.eventParm = GTS_BLUE_CAPTURE;
    } else {
        te->s.eventParm = GTS_RED_CAPTURE;
    }
    te->r.svFlags |= SVF_BROADCAST;
}

void Team_ReturnFlag(int team) {
    Team_ReturnFlagSound(Team_ResetFlag(team), team);
    if (team == TEAM_FREE) {
        // PrintMsg(NULL, "The flag has returned!\n" );
    } else { // flag should always have team in normal CTF
        // PrintMsg(NULL, "The %s flag has returned!\n", TeamName(team));
        PrintCTFMessage(-1, team, CTFMESSAGE_FLAG_RETURNED);
    }
}

void Team_FreeEntity(gentity_t *ent) {
    if (ent->item->giTag == PW_REDFLAG) {
        Team_ReturnFlag(TEAM_RED);
    } else if (ent->item->giTag == PW_BLUEFLAG) {
        Team_ReturnFlag(TEAM_BLUE);
    } else if (ent->item->giTag == PW_NEUTRALFLAG) {
        Team_ReturnFlag(TEAM_FREE);
    }
}

// Automatically set in Launch_Item if the item is one of the flags
// Flags are unique in that if they are dropped, the base flag must be respawned when they time out
void Team_DroppedFlagThink(gentity_t *ent) {
    int team = TEAM_FREE;

    if (ent->item->giTag == PW_REDFLAG) {
        team = TEAM_RED;
    } else if (ent->item->giTag == PW_BLUEFLAG) {
        team = TEAM_BLUE;
    } else if (ent->item->giTag == PW_NEUTRALFLAG) {
        team = TEAM_FREE;
    }

    Team_ReturnFlagSound(Team_ResetFlag(team), team);
    // Reset Flag will delete this entity
}

// This is to account for situations when there are more players standing
// on flag stand and then flag gets returned. This leaded to bit random flag
// grabs/captures, improved version takes distance to the center of flag stand
// into consideration (closer player will get/capture the flag).
static vector3 minFlagRange = {50, 36, 36};
static vector3 maxFlagRange = {44, 36, 36};

int Team_TouchEnemyFlag(gentity_t *ent, gentity_t *other, int team);
int Team_TouchOurFlag(gentity_t *ent, gentity_t *other, int team) {
    int i, num, j, enemyTeam;
    gentity_t *player = NULL, *enemy = NULL;
    gclient_t *cl = other->client;
    int enemy_flag;
    vector3 mins, maxs;
    int touch[MAX_GENTITIES];
    float enemyDist, dist;
    int capTime;

    if (cl->sess.sessionTeam == TEAM_RED) {
        enemy_flag = PW_BLUEFLAG;
    } else {
        enemy_flag = PW_REDFLAG;
    }

    if (ent->flags & FL_DROPPED_ITEM) {
        // hey, its not home.  return it by teleporting it back
        // PrintMsg( NULL, "%s" S_COLOR_WHITE " returned the %s flag!\n",
        //	cl->pers.netname, TeamName(team));
        PrintCTFMessage(other->s.number, team, CTFMESSAGE_PLAYER_RETURNED_FLAG);

        AddScore(other, &ent->r.currentOrigin, CTF_RECOVERY_BONUS);
        other->client->pers.teamState.flagrecovery++;
        other->client->pers.teamState.lastreturnedflag = level.time;
        // ResetFlag will remove this entity!  We must return zero
        Team_ReturnFlagSound(Team_ResetFlag(team), team);
        return 0;
    }

    // the flag is at home base.  if the player has the enemy
    // flag, he's just won!
    if (!cl->ps.powerups[enemy_flag])
        return 0; // We don't have the flag

    // Raz: captures after timelimit hit could cause game ending with tied score
    if (level.intermissionQueued)
        return 0;

    // Raz: check for enemy closer to grab the flag
    VectorSubtract(&ent->s.pos.trBase, &minFlagRange, &mins);
    VectorAdd(&ent->s.pos.trBase, &maxFlagRange, &maxs);

    num = trap->EntitiesInBox(&mins, &maxs, touch, MAX_GENTITIES);

    dist = Distance(&ent->s.pos.trBase, &other->client->ps.origin);

    if (other->client->sess.sessionTeam == TEAM_RED) {
        enemyTeam = TEAM_BLUE;
    } else {
        enemyTeam = TEAM_RED;
    }

    for (j = 0; j < num; j++) {
        enemy = (g_entities + touch[j]);

        if (!enemy || !enemy->inuse || !enemy->client) {
            continue;
        }

        // check if its alive
        if (enemy->health < 1)
            continue; // dead people can't pickup

        // ignore specs
        if (enemy->client->sess.sessionTeam == TEAM_SPECTATOR)
            continue;

        // check if this is enemy
        if ((enemy->client->sess.sessionTeam != TEAM_RED && enemy->client->sess.sessionTeam != TEAM_BLUE) || enemy->client->sess.sessionTeam != enemyTeam) {
            continue;
        }

        // check if enemy is closer to our flag than us
        enemyDist = Distance(&ent->s.pos.trBase, &enemy->client->ps.origin);
        if (enemyDist < dist) {
            // possible recursion is hidden in this, but
            // infinite recursion wont happen, because we cant
            // have a < b and b < a at the same time
            return Team_TouchEnemyFlag(ent, enemy, team);
        }
    }

    if (japp_speedCaps.integer) { // Raz: speed caps
        capTime = trap->Milliseconds() - cl->pers.teamState.flagsince;
        if (teamgame.bestCapTime == -1) {
            trap->SendServerCommand(-1, va("print \"%s " S_COLOR_GREEN "captured the %s flag in " S_COLOR_CYAN "%.3f " S_COLOR_GREEN
                                           "seconds, setting the record\n\"",
                                           cl->pers.netname, TeamName(OtherTeam(team)), (double)(capTime / 1000.0f)));
            teamgame.bestCapTime = capTime;
            Q_strncpyz(teamgame.bestCapName, cl->pers.netname, sizeof(teamgame.bestCapName));
        } else if (capTime < teamgame.bestCapTime) {
            if (!Q_stricmp(teamgame.bestCapName, cl->pers.netname))
                trap->SendServerCommand(-1, va("print \"%s " S_COLOR_GREEN "captured the %s flag in " S_COLOR_CYAN "%.3f " S_COLOR_GREEN
                                               "seconds, breaking their record of " S_COLOR_CYAN "%.3f " S_COLOR_GREEN "seconds (" S_COLOR_YELLOW
                                               "+%.3f" S_COLOR_GREEN ")\n\"",
                                               cl->pers.netname, TeamName(OtherTeam(team)), (double)(capTime / 1000.0f),
                                               (double)(teamgame.bestCapTime / 1000.0f), (double)((teamgame.bestCapTime - capTime) / 1000.0f)));
            else {
                trap->SendServerCommand(-1, va("print \"%s " S_COLOR_GREEN "captured the %s flag in " S_COLOR_CYAN "%.3f " S_COLOR_GREEN
                                               "seconds, breaking %s" S_COLOR_GREEN "'s record of " S_COLOR_CYAN "%.3f " S_COLOR_GREEN
                                               "seconds (" S_COLOR_YELLOW "+%.3f" S_COLOR_GREEN ")\n\"",
                                               cl->pers.netname, TeamName(OtherTeam(team)), (double)(capTime / 1000.0f), teamgame.bestCapName,
                                               (double)(teamgame.bestCapTime / 1000.0f), (double)((teamgame.bestCapTime - capTime) / 1000.0f)));
            }
            teamgame.bestCapTime = capTime;
            Q_strncpyz(teamgame.bestCapName, cl->pers.netname, sizeof(teamgame.bestCapName));
        } else {
            if (!Q_stricmp(teamgame.bestCapName, cl->pers.netname)) {
                trap->SendServerCommand(-1, va("print \"%s " S_COLOR_GREEN "captured the %s flag in " S_COLOR_CYAN "%.3f " S_COLOR_GREEN
                                               "seconds, failing to break their record of " S_COLOR_CYAN "%.3f " S_COLOR_GREEN "seconds (" S_COLOR_RED
                                               "%.3f" S_COLOR_GREEN ")\n\"",
                                               cl->pers.netname, TeamName(OtherTeam(team)), (double)(capTime / 1000.0f),
                                               (double)(teamgame.bestCapTime / 1000.0f), (double)((teamgame.bestCapTime - capTime) / 1000.0f)));
            } else {
                trap->SendServerCommand(-1, va("print \"%s " S_COLOR_GREEN "captured the %s flag in " S_COLOR_CYAN "%.3f " S_COLOR_GREEN
                                               "seconds, failing to break %s" S_COLOR_GREEN "'s record of " S_COLOR_CYAN "%.3f " S_COLOR_GREEN
                                               "seconds (" S_COLOR_RED "%.3f" S_COLOR_GREEN ")\n\"",
                                               cl->pers.netname, TeamName(OtherTeam(team)), (double)(capTime / 1000.0f), teamgame.bestCapName,
                                               (double)(teamgame.bestCapTime / 1000.0f), (double)((teamgame.bestCapTime - capTime) / 1000.0f)));
            }
        }
    } else {
        // PrintMsg( NULL, "%s" S_COLOR_WHITE " captured the %s flag!\n", cl->pers.netname, TeamName(OtherTeam(team)));
        PrintCTFMessage(other->s.number, team, CTFMESSAGE_PLAYER_CAPTURED_FLAG);
    }

    cl->ps.powerups[enemy_flag] = 0;

    teamgame.last_flag_capture = level.time;
    teamgame.last_capture_team = team;

    // Increase the team's score
    AddTeamScore(&ent->s.pos.trBase, other->client->sess.sessionTeam, 1);
    //	Team_ForceGesture(other->client->sess.sessionTeam);
    // rww - don't really want to do this now. Mainly because performing a gesture disables your upper torso animations until it's done and you can't fire

    other->client->pers.teamState.captures++;
    other->client->rewardTime = level.time + REWARD_SPRITE_TIME;
    other->client->ps.persistant[PERS_CAPTURES]++;

    // other gets another 10 frag bonus
    AddScore(other, &ent->r.currentOrigin, CTF_CAPTURE_BONUS);

    Team_CaptureFlagSound(ent, team);

    // Ok, let's do the player loop, hand out the bonuses
    for (i = 0; i < sv_maxclients.integer; i++) {
        player = &g_entities[i];
        if (!player->inuse)
            continue;

        if (player->client->sess.sessionTeam != cl->sess.sessionTeam) {
            player->client->pers.teamState.lasthurtcarrier = -5;
        } else if (player->client->sess.sessionTeam == cl->sess.sessionTeam) {
            if (player != other)
                AddScore(player, &ent->r.currentOrigin, CTF_TEAM_BONUS);
            // award extra points for capture assists
            if (player->client->pers.teamState.lastreturnedflag + CTF_RETURN_FLAG_ASSIST_TIMEOUT > level.time) {
                AddScore(player, &ent->r.currentOrigin, CTF_RETURN_FLAG_ASSIST_BONUS);
                other->client->pers.teamState.assists++;

                player->client->ps.persistant[PERS_ASSIST_COUNT]++;
                player->client->rewardTime = level.time + REWARD_SPRITE_TIME;
            }
            if (player->client->pers.teamState.lastfraggedcarrier + CTF_FRAG_CARRIER_ASSIST_TIMEOUT > level.time) {
                AddScore(player, &ent->r.currentOrigin, CTF_FRAG_CARRIER_ASSIST_BONUS);
                other->client->pers.teamState.assists++;
                player->client->ps.persistant[PERS_ASSIST_COUNT]++;
                player->client->rewardTime = level.time + REWARD_SPRITE_TIME;
            }
        }
    }
    Team_ResetFlags();

    CalculateRanks();

    return 0; // Do not respawn this automatically
}

int Team_TouchEnemyFlag(gentity_t *ent, gentity_t *other, int team) {
    gclient_t *cl = other->client;
    vector3 mins, maxs;
    int num, j, ourFlag;
    int touch[MAX_GENTITIES];
    gentity_t *enemy;
    float enemyDist, dist;

    if (!!(ent->genericValue2 & 32) && other - g_entities == (ent->genericValue2 & 31)) { // was dropped by us
        if ((ent->genericValue2 >> 6) > (level.time & 0x3FFFFFF) - 1000) {
            return 0;
        }
    }

    VectorSubtract(&ent->s.pos.trBase, &minFlagRange, &mins);
    VectorAdd(&ent->s.pos.trBase, &maxFlagRange, &maxs);

    num = trap->EntitiesInBox(&mins, &maxs, touch, MAX_GENTITIES);

    dist = Distance(&ent->s.pos.trBase, &other->client->ps.origin);

    if (other->client->sess.sessionTeam == TEAM_RED) {
        ourFlag = PW_REDFLAG;
    } else {
        ourFlag = PW_BLUEFLAG;
    }

    for (j = 0; j < num; ++j) {
        enemy = (g_entities + touch[j]);

        if (!enemy || !enemy->inuse || !enemy->client) {
            continue;
        }

        // ignore specs
        if (enemy->client->sess.sessionTeam == TEAM_SPECTATOR)
            continue;

        // check if its alive
        if (enemy->health < 1)
            continue; // dead people can't pick up items

        // lets check if he has our flag
        if (!enemy->client->ps.powerups[ourFlag])
            continue;

        // check if enemy is closer to our flag than us
        enemyDist = Distance(&ent->s.pos.trBase, &enemy->client->ps.origin);
        if (enemyDist < dist) {
            // possible recursion is hidden in this, but
            // infinite recursion wont happen, because we cant
            // have a < b and b < a at the same time
            return Team_TouchOurFlag(ent, enemy, team);
        }
    }

    // PrintMsg (NULL, "%s" S_COLOR_WHITE " got the %s flag!\n",
    //	other->client->pers.netname, TeamName(team));
    PrintCTFMessage(other->s.number, team, CTFMESSAGE_PLAYER_GOT_FLAG);

    if (team == TEAM_RED)
        cl->ps.powerups[PW_REDFLAG] = INT_MAX; // flags never expire
    else
        cl->ps.powerups[PW_BLUEFLAG] = INT_MAX; // flags never expire

    Team_SetFlagStatus(team, FLAG_TAKEN);

    AddScore(other, &ent->r.currentOrigin, CTF_FLAG_BONUS);
    cl->pers.teamState.flagsince = trap->Milliseconds() - ent->genericValue1; // Raz: speed caps
    Team_TakeFlagSound(ent, team);

    return -1; // Do not respawn this automatically, but do delete it if it was FL_DROPPED
}

int Pickup_Team(gentity_t *ent, gentity_t *other) {
    int team;
    gclient_t *cl = other->client;

    // figure out what team this flag is
    if (strcmp(ent->classname, "team_CTF_redflag") == 0) {
        team = TEAM_RED;
    } else if (strcmp(ent->classname, "team_CTF_blueflag") == 0) {
        team = TEAM_BLUE;
    } else if (strcmp(ent->classname, "team_CTF_neutralflag") == 0) {
        team = TEAM_FREE;
    } else {
        //		PrintMsg ( other, "Don't know what team the flag is on.\n");
        return 0;
    }
    // GT_CTF
    if (team == cl->sess.sessionTeam) {
        return Team_TouchOurFlag(ent, other, team);
    }
    return Team_TouchEnemyFlag(ent, other, team);
}

// Report a location for the player. Uses placed nearby target_location entities
gentity_t *Team_GetLocation(gentity_t *ent) {
    gentity_t *eloc, *best;
    float bestlen, len;
    vector3 origin;

    best = NULL;
    bestlen = 3 * 8192.0f * 8192.0f;

    VectorCopy(&ent->r.currentOrigin, &origin);

    for (eloc = level.locationHead; eloc; eloc = eloc->nextTrain) {
        len = (origin.x - eloc->r.currentOrigin.x) * (origin.x - eloc->r.currentOrigin.x) +
              (origin.y - eloc->r.currentOrigin.y) * (origin.y - eloc->r.currentOrigin.y) +
              (origin.z - eloc->r.currentOrigin.z) * (origin.z - eloc->r.currentOrigin.z);

        if (len > bestlen) {
            continue;
        }

        if (!trap->InPVS(&origin, &eloc->r.currentOrigin)) {
            continue;
        }

        bestlen = len;
        best = eloc;
    }

    return best;
}

// Report a location for the player. Uses placed nearby target_location entities
qboolean Team_GetLocationMsg(gentity_t *ent, char *loc, int loclen) {
    gentity_t *best;

    best = Team_GetLocation(ent);

    if (!best)
        return qfalse;

    if (best->count) {
        if (best->count < 0)
            best->count = 0;
        if (best->count > 7)
            best->count = 7;
        Com_sprintf(loc, loclen, "%c%c%s" S_COLOR_WHITE, Q_COLOR_ESCAPE, best->count + '0', best->message);
    } else
        Com_sprintf(loc, loclen, "%s", best->message);

    return qtrue;
}

#define MAX_TEAM_SPAWN_POINTS 32

// go to a random point that doesn't telefrag
gentity_t *SelectRandomTeamSpawnPoint(int teamstate, team_e team, int siegeClass) {
    gentity_t *spot, *spots[MAX_TEAM_SPAWN_POINTS];
    int count, selection;
    const char *classname;
    qboolean mustBeEnabled = qfalse;

    if (level.gametype == GT_SIEGE) {
        if (team == SIEGETEAM_TEAM1)
            classname = "info_player_siegeteam1";
        else
            classname = "info_player_siegeteam2";

        mustBeEnabled = qtrue; // siege spawn points need to be "enabled" to be used (because multiple spawnpoint sets can be placed at once)
    } else {
        if (teamstate == TEAM_BEGIN) {
            if (team == TEAM_RED)
                classname = "team_CTF_redplayer";
            else if (team == TEAM_BLUE)
                classname = "team_CTF_blueplayer";
            else
                return NULL;
        } else {
            if (team == TEAM_RED)
                classname = "team_CTF_redspawn";
            else if (team == TEAM_BLUE)
                classname = "team_CTF_bluespawn";
            else
                return NULL;
        }
    }
    count = 0;

    spot = NULL;

    while ((spot = G_Find(spot, FOFS(classname), classname)) != NULL) {
        if (SpotWouldTelefrag(spot)) {
            continue;
        }

        if (mustBeEnabled && !spot->genericValue1) { // siege point that's not enabled, can't use it
            continue;
        }

        spots[count] = spot;
        if (++count == MAX_TEAM_SPAWN_POINTS)
            break;
    }

    if (!count) { // no spots that won't telefrag
        return G_Find(NULL, FOFS(classname), classname);
    }

    if (level.gametype == GT_SIEGE && siegeClass >= 0 &&
        bgSiegeClasses[siegeClass].name[0]) { // out of the spots found, see if any have an idealclass to match our class name
        gentity_t *classSpots[MAX_TEAM_SPAWN_POINTS];
        int classCount = 0;
        int i = 0;

        while (i < count) {
            if (spots[i] && spots[i]->idealclass && spots[i]->idealclass[0] &&
                !Q_stricmp(spots[i]->idealclass, bgSiegeClasses[siegeClass].name)) { // this spot's idealclass matches the class name
                classSpots[classCount] = spots[i];
                classCount++;
            }
            i++;
        }

        if (classCount > 0) { // found at least one
            selection = rand() % classCount;
            return classSpots[selection];
        }
    }

    selection = rand() % count;
    return spots[selection];
}

gentity_t *SelectCTFSpawnPoint(team_e team, int teamstate, vector3 *origin, vector3 *angles) {
    gentity_t *spot;

    spot = SelectRandomTeamSpawnPoint(teamstate, team, -1);

    if (!spot) {
        return SelectSpawnPoint(&vec3_origin, origin, angles, team);
    }

    VectorCopy(&spot->s.origin, origin);
    origin->z += 9;
    VectorCopy(&spot->s.angles, angles);

    return spot;
}

gentity_t *SelectSiegeSpawnPoint(int siegeClass, team_e team, int teamstate, vector3 *origin, vector3 *angles) {
    gentity_t *spot;

    spot = SelectRandomTeamSpawnPoint(teamstate, team, siegeClass);

    if (!spot) {
        return SelectSpawnPoint(&vec3_origin, origin, angles, team);
    }

    VectorCopy(&spot->s.origin, origin);
    origin->z += 9;
    VectorCopy(&spot->s.angles, angles);

    return spot;
}

static int SortClients(const void *a, const void *b) { return *(int *)a - *(int *)b; }

// Format: clientNum location health armor weapon powerups
void TeamplayInfoMessageFixed(gentity_t *ent) {
    char entry[1024];
    char string[8192];
    int stringlength;
    int i, j;
    gentity_t *player;
    int cnt;
    int h, a;
    int clients[TEAM_MAXOVERLAY];
    int team;

    if (!ent->client->pers.teamInfo)
        return;

    // send team info to spectator for team of followed client
    if (ent->client->sess.sessionTeam == TEAM_SPECTATOR) {
        if (ent->client->sess.spectatorState != SPECTATOR_FOLLOW || ent->client->sess.spectatorClient < 0) {
            return;
        }
        team = g_entities[ent->client->sess.spectatorClient].client->sess.sessionTeam;
    } else {
        team = ent->client->sess.sessionTeam;
    }

    if (team != TEAM_RED && team != TEAM_BLUE) {
        return;
    }

    // figure out what client should be on the display
    // we are limited to 8, but we want to use the top eight players
    // but in client order (so they don't keep changing position on the overlay)
    for (i = 0, cnt = 0; i < sv_maxclients.integer && cnt < TEAM_MAXOVERLAY; i++) {
        player = g_entities + level.sortedClients[i];
        if (player->inuse && player->client->sess.sessionTeam == team) {
            clients[cnt++] = level.sortedClients[i];
        }
    }

    // We have the top eight players, sort them by clientNum
    qsort(clients, cnt, sizeof(clients[0]), SortClients);

    // send the latest information on all clients
    string[0] = 0;
    stringlength = 0;

    for (i = 0, cnt = 0; i < sv_maxclients.integer && cnt < TEAM_MAXOVERLAY; i++) {
        player = g_entities + i;
        if (player->inuse && player->client->sess.sessionTeam == team) {

            h = player->client->ps.stats[STAT_HEALTH];
            a = player->client->ps.stats[STAT_ARMOR];
            if (h < 0)
                h = 0;
            if (a < 0)
                a = 0;

            Com_sprintf(entry, sizeof(entry), " %i %i %i %i %i %i",
                        //	level.sortedClients[i], player->client->pers.teamState.location, h, a,
                        i, player->client->pers.teamState.location, h, a, player->client->ps.weapon, player->s.powerups);
            j = strlen(entry);
            if (stringlength + j >= sizeof(string))
                break;
            strcpy(string + stringlength, entry);
            stringlength += j;
            cnt++;
        }
    }

    trap->SendServerCommand(ent - g_entities, va("tinfo %i %s", cnt, string));
}

void TeamplayInfoMessageOriginal(gentity_t *ent) {
    char entry[1024];
    char string[8192];
    int stringlength;
    int i, j;
    gentity_t *player;
    int cnt;
    int h, a;
    int clients[TEAM_MAXOVERLAY];

    if (!ent->client->pers.teamInfo)
        return;

    // figure out what client should be on the display
    // we are limited to 8, but we want to use the top eight players
    // but in client order (so they don't keep changing position on the overlay)
    for (i = 0, cnt = 0; i < sv_maxclients.integer && cnt < TEAM_MAXOVERLAY; i++) {
        player = g_entities + level.sortedClients[i];
        if (player->inuse && player->client->sess.sessionTeam == ent->client->sess.sessionTeam) {
            clients[cnt++] = level.sortedClients[i];
        }
    }

    // We have the top eight players, sort them by clientNum
    qsort(clients, cnt, sizeof(clients[0]), SortClients);

    // send the latest information on all clients
    string[0] = 0;
    stringlength = 0;

    for (i = 0, cnt = 0; i < sv_maxclients.integer && cnt < TEAM_MAXOVERLAY; i++) {
        player = g_entities + i;
        if (player->inuse && player->client->sess.sessionTeam == ent->client->sess.sessionTeam) {

            h = player->client->ps.stats[STAT_HEALTH];
            a = player->client->ps.stats[STAT_ARMOR];
            if (h < 0)
                h = 0;
            if (a < 0)
                a = 0;

            Com_sprintf(entry, sizeof(entry), " %i %i %i %i %i %i",
                        //				level.sortedClients[i], player->client->pers.teamState.location, h, a,
                        i, player->client->pers.teamState.location, h, a, player->client->ps.weapon, player->s.powerups);
            j = strlen(entry);
            if (stringlength + j >= sizeof(string))
                break;
            strcpy(string + stringlength, entry);
            stringlength += j;
            cnt++;
        }
    }

    trap->SendServerCommand(ent - g_entities, va("tinfo %i %s", cnt, string));
}

void CheckTeamStatus(void) {
    int i;
    gentity_t *loc, *ent;

    // OSP: pause
    if (level.pause.state != PAUSE_NONE)
        return;

    if (level.time - level.lastTeamLocationTime > TEAM_LOCATION_UPDATE_TIME) {

        level.lastTeamLocationTime = level.time;

        for (i = 0; i < sv_maxclients.integer; i++) {
            ent = g_entities + i;

            if (!ent->client) {
                continue;
            }

            if (ent->client->pers.connected != CON_CONNECTED) {
                continue;
            }

            if (ent->inuse && (ent->client->sess.sessionTeam == TEAM_RED || ent->client->sess.sessionTeam == TEAM_BLUE)) {
                loc = Team_GetLocation(ent);
                if (loc)
                    ent->client->pers.teamState.location = loc->health;
                else
                    ent->client->pers.teamState.location = 0;
            }
        }

        for (i = 0; i < sv_maxclients.integer; i++) {
            ent = g_entities + i;

            if (ent->client->pers.connected != CON_CONNECTED) {
                continue;
            }

            if ( ent->inuse /*&& (ent->client->sess.sessionTeam == TEAM_RED ||	ent->client->sess.sessionTeam == TEAM_BLUE)) {
				TeamplayInfoMessageOriginal( ent );
				}
				else if ( ent->inuse */ )
                TeamplayInfoMessageFixed(ent);
        }
    }
}

/*QUAKED team_CTF_redplayer (1 0 0) (-16 -16 -16) (16 16 32)
Only in CTF games.  Red players spawn here at game start.
*/
void SP_team_CTF_redplayer(gentity_t *ent) {}

/*QUAKED team_CTF_blueplayer (0 0 1) (-16 -16 -16) (16 16 32)
Only in CTF games.  Blue players spawn here at game start.
*/
void SP_team_CTF_blueplayer(gentity_t *ent) {}

/*QUAKED team_CTF_redspawn (1 0 0) (-16 -16 -24) (16 16 32)
potential spawning position for red team in CTF games.
Targets will be fired when someone spawns in on them.
*/
void SP_team_CTF_redspawn(gentity_t *ent) {}

/*QUAKED team_CTF_bluespawn (0 0 1) (-16 -16 -24) (16 16 32)
potential spawning position for blue team in CTF games.
Targets will be fired when someone spawns in on them.
*/
void SP_team_CTF_bluespawn(gentity_t *ent) {}
