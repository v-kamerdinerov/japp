// Copyright (C) 1999-2000 Id Software, Inc.
//

#include "g_local.h"

qboolean G_SpawnString(const char *key, const char *defaultString, char **out) {
    int i;

    if (!level.spawning && !level.manualSpawning) {
        *out = (char *)defaultString;
        //		trap->Error( ERR_DROP, "G_SpawnString() called while not spawning" );
    }

    for (i = 0; i < level.numSpawnVars; i++) {
        if (!Q_stricmp(key, level.spawnVars[i][0])) {
            *out = level.spawnVars[i][1];
            return qtrue;
        }
    }

    *out = (char *)defaultString;
    return qfalse;
}

qboolean G_SpawnFloat(const char *key, const char *defaultString, float *out) {
    char *s;
    qboolean present;

    present = G_SpawnString(key, defaultString, &s);
    *out = atof(s);
    return present;
}

qboolean G_SpawnInt(const char *key, const char *defaultString, int *out) {
    char *s;
    qboolean present;

    present = G_SpawnString(key, defaultString, &s);
    *out = atoi(s);
    return present;
}

qboolean G_SpawnVector(const char *key, const char *defaultString, vector3 *out) {
    char *s;
    qboolean present;

    present = G_SpawnString(key, defaultString, &s);
    sscanf(s, "%f %f %f", &out->x, &out->y, &out->z);
    return present;
}

qboolean G_SpawnBoolean(const char *key, const char *defaultString, qboolean *out) {
    char *s;
    qboolean present;

    present = G_SpawnString(key, defaultString, &s);

    if (!Q_stricmp(s, "qtrue") || !Q_stricmp(s, "true") || !Q_stricmp(s, "yes") || !Q_stricmp(s, "1"))
        *out = qtrue;
    else if (!Q_stricmp(s, "qfalse") || !Q_stricmp(s, "false") || !Q_stricmp(s, "no") || !Q_stricmp(s, "0"))
        *out = qfalse;
    else
        *out = qfalse;

    return present;
}

BG_field_t fields[] = {
    {"alliedteam", FOFS(alliedTeam), F_INT},                   // for misc_turrets
    {"angerscript", FOFS(behaviorSet[BSET_ANGER]), F_LSTRING}, // name of script to run
    {"angle", FOFS(s.angles), F_ANGLEHACK},
    {"angles", FOFS(s.angles), F_VECTOR},
    {"attackscript", FOFS(behaviorSet[BSET_ATTACK]), F_LSTRING},   // name of script to run
    {"awakescript", FOFS(behaviorSet[BSET_AWAKE]), F_LSTRING},     // name of script to run
    {"blockedscript", FOFS(behaviorSet[BSET_BLOCKED]), F_LSTRING}, // name of script to run
    {"bounceCount", FOFS(bounceCount), F_INT},
    {"chunksize", FOFS(mass), F_FLOAT}, // for func_breakables
    {"classname", FOFS(classname), F_LSTRING},
    {"closetarget", FOFS(closetarget), F_LSTRING}, // for doors
    {"count", FOFS(count), F_INT},
    {"deathscript", FOFS(behaviorSet[BSET_DEATH]), F_LSTRING}, // name of script to run
    {"delay", FOFS(delay), F_INT},
    {"delayscript", FOFS(behaviorSet[BSET_DELAYED]), F_LSTRING}, // name of script to run
    {"delayscripttime", FOFS(delayScriptTime), F_INT},           // name of script to run
    {"dmg", FOFS(damage), F_INT},
    {"ffdeathscript", FOFS(behaviorSet[BSET_FFDEATH]), F_LSTRING}, // name of script to run
    {"ffirescript", FOFS(behaviorSet[BSET_FFIRE]), F_LSTRING},     // name of script to run
    {"fleescript", FOFS(behaviorSet[BSET_FLEE]), F_LSTRING},       // name of script to run
    {"fly_sound_debounce_time", FOFS(fly_sound_debounce_time), F_INT},
    {"fullname", FOFS(fullName), F_LSTRING},
    {"goaltarget", FOFS(goaltarget), F_LSTRING}, // for siege
    {"healingclass", FOFS(healingclass), F_LSTRING},
    {"healingrate", FOFS(healingrate), F_INT},
    {"healingsound", FOFS(healingsound), F_LSTRING},
    {"health", FOFS(health), F_INT},
    {"idealclass", FOFS(idealclass), F_LSTRING},                       // for siege spawnpoints
    {"linear", FOFS(alt_fire), F_INT},                                 // for movers to use linear movement
    {"lostenemyscript", FOFS(behaviorSet[BSET_LOSTENEMY]), F_LSTRING}, // name of script to run
    {"message", FOFS(message), F_LSTRING},
    {"mindtrickscript", FOFS(behaviorSet[BSET_MINDTRICK]), F_LSTRING}, // name of script to run
    {"model", FOFS(model), F_LSTRING},
    {"model2", FOFS(model2), F_LSTRING},
    {"npc_target", FOFS(NPC_target), F_LSTRING},
    {"npc_target2", FOFS(target2), F_LSTRING}, // NPC_spawner only
    {"npc_target4", FOFS(target4), F_LSTRING}, // NPC_spawner only
    {"npc_targetname", FOFS(NPC_targetname), F_LSTRING},
    {"npc_type", FOFS(NPC_type), F_LSTRING},
    {"numchunks", FOFS(radius), F_FLOAT},        // for func_breakables
    {"opentarget", FOFS(opentarget), F_LSTRING}, // for doors
    {"origin", FOFS(s.origin), F_VECTOR},
    {"ownername", FOFS(ownername), F_LSTRING},
    {"painscript", FOFS(behaviorSet[BSET_PAIN]), F_LSTRING}, // name of script to run
    {"paintarget", FOFS(paintarget), F_LSTRING},             // for doors
    {"parm1", 0, F_PARM1},
    {"parm10", 0, F_PARM10},
    {"parm11", 0, F_PARM11},
    {"parm12", 0, F_PARM12},
    {"parm13", 0, F_PARM13},
    {"parm14", 0, F_PARM14},
    {"parm15", 0, F_PARM15},
    {"parm16", 0, F_PARM16},
    {"parm2", 0, F_PARM2},
    {"parm3", 0, F_PARM3},
    {"parm4", 0, F_PARM4},
    {"parm5", 0, F_PARM5},
    {"parm6", 0, F_PARM6},
    {"parm7", 0, F_PARM7},
    {"parm8", 0, F_PARM8},
    {"parm9", 0, F_PARM9},
    {"radius", FOFS(radius), F_FLOAT},
    {"random", FOFS(random), F_FLOAT},
    {"roffname", FOFS(roffname), F_LSTRING},
    {"rofftarget", FOFS(rofftarget), F_LSTRING},
    {"script_targetname", FOFS(script_targetname), F_LSTRING}, // scripts look for this when "affecting"
    {"soundset", FOFS(soundSet), F_LSTRING},
    {"spawnflags", FOFS(spawnflags), F_INT},
    {"spawnscript", FOFS(behaviorSet[BSET_SPAWN]), F_LSTRING}, // name of script to run
    {"speed", FOFS(speed), F_FLOAT},
    {"splashDamage", FOFS(splashDamage), F_INT},
    {"splashRadius", FOFS(splashRadius), F_INT},
    {"target", FOFS(target), F_LSTRING},
    {"target2", FOFS(target2), F_LSTRING},
    {"target3", FOFS(target3), F_LSTRING},
    {"target4", FOFS(target4), F_LSTRING},
    {"target5", FOFS(target5), F_LSTRING},
    {"target6", FOFS(target6), F_LSTRING},
    {"targetname", FOFS(targetname), F_LSTRING},
    {"targetshadername", FOFS(targetShaderName), F_LSTRING},
    {"targetshadernewname", FOFS(targetShaderNewName), F_LSTRING},
    {"team", FOFS(team), F_LSTRING},
    {"teamnodmg", FOFS(teamnodmg), F_INT},
    {"teamowner", FOFS(s.teamowner), F_INT},
    {"teamuser", FOFS(alliedTeam), F_INT},
    {"usescript", FOFS(behaviorSet[BSET_USE]), F_LSTRING},         // name of script to run
    {"victoryscript", FOFS(behaviorSet[BSET_VICTORY]), F_LSTRING}, // name of script to run
    {"wait", FOFS(wait), F_FLOAT},
};

typedef struct spawn_s {
    const char *name;
    void (*spawn)(gentity_t *ent);
} spawn_t;

void SP_info_player_start(gentity_t *ent);
void SP_info_player_duel(gentity_t *ent);
void SP_info_player_duel1(gentity_t *ent);
void SP_info_player_duel2(gentity_t *ent);
void SP_info_player_deathmatch(gentity_t *ent);
void SP_info_player_siegeteam1(gentity_t *ent);
void SP_info_player_siegeteam2(gentity_t *ent);
void SP_info_player_intermission(gentity_t *ent);
void SP_info_player_intermission_red(gentity_t *ent);
void SP_info_player_intermission_blue(gentity_t *ent);
void SP_info_jedimaster_start(gentity_t *ent);
void SP_info_player_start_red(gentity_t *ent);
void SP_info_player_start_blue(gentity_t *ent);

void SP_info_siege_objective(gentity_t *ent);
void SP_info_siege_radaricon(gentity_t *ent);
void SP_info_siege_decomplete(gentity_t *ent);
void SP_target_siege_end(gentity_t *ent);
void SP_misc_siege_item(gentity_t *ent);

void SP_func_plat(gentity_t *ent);
void SP_func_static(gentity_t *ent);
void SP_func_rotating(gentity_t *ent);
void SP_func_bobbing(gentity_t *ent);
void SP_func_pendulum(gentity_t *ent);
void SP_func_button(gentity_t *ent);
void SP_func_door(gentity_t *ent);
void SP_func_train(gentity_t *ent);
void SP_func_timer(gentity_t *self);
void SP_func_breakable(gentity_t *ent);
void SP_func_glass(gentity_t *ent);
void SP_func_usable(gentity_t *ent);
void SP_func_wall(gentity_t *ent);

void SP_trigger_lightningstrike(gentity_t *ent);

void SP_trigger_always(gentity_t *ent);
void SP_trigger_multiple(gentity_t *ent);
void SP_trigger_once(gentity_t *ent);
void SP_trigger_push(gentity_t *ent);
void SP_trigger_teleport(gentity_t *ent);
void SP_trigger_hurt(gentity_t *ent);
void SP_trigger_space(gentity_t *self);
void SP_trigger_shipboundary(gentity_t *self);
void SP_trigger_hyperspace(gentity_t *self);
void SP_trigger_asteroid_field(gentity_t *self);

void SP_target_remove_powerups(gentity_t *ent);
void SP_target_give(gentity_t *ent);
void SP_target_delay(gentity_t *ent);
void SP_target_speaker(gentity_t *ent);
void SP_target_print(gentity_t *ent);
void SP_target_laser(gentity_t *self);
void SP_target_character(gentity_t *ent);
void SP_target_score(gentity_t *ent);
void SP_target_teleporter(gentity_t *ent);
void SP_target_relay(gentity_t *ent);
void SP_target_kill(gentity_t *ent);
void SP_target_position(gentity_t *ent);
void SP_target_location(gentity_t *ent);
void SP_target_counter(gentity_t *self);
void SP_target_random(gentity_t *self);
void SP_target_scriptrunner(gentity_t *self);
void SP_target_interest(gentity_t *self);
void SP_target_activate(gentity_t *self);
void SP_target_deactivate(gentity_t *self);
void SP_target_level_change(gentity_t *self);
void SP_target_play_music(gentity_t *self);
void SP_target_push(gentity_t *ent);

void SP_light(gentity_t *self);
void SP_info_null(gentity_t *self);
void SP_info_notnull(gentity_t *self);
void SP_info_camp(gentity_t *self);
void SP_path_corner(gentity_t *self);

void SP_misc_teleporter_dest(gentity_t *self);
void SP_misc_model(gentity_t *ent);
void SP_misc_model_static(gentity_t *ent);
void SP_misc_G2model(gentity_t *ent);
void SP_misc_portal_camera(gentity_t *ent);
void SP_misc_portal_surface(gentity_t *ent);
void SP_misc_weather_zone(gentity_t *ent);

void SP_misc_bsp(gentity_t *ent);
void SP_terrain(gentity_t *ent);
void SP_misc_skyportal_orient(gentity_t *ent);
void SP_misc_skyportal(gentity_t *ent);

void SP_misc_ammo_floor_unit(gentity_t *ent);
void SP_misc_shield_floor_unit(gentity_t *ent);
void SP_misc_model_shield_power_converter(gentity_t *ent);
void SP_misc_model_ammo_power_converter(gentity_t *ent);
void SP_misc_model_health_power_converter(gentity_t *ent);

void SP_fx_runner(gentity_t *ent);

void SP_target_screenshake(gentity_t *ent);
void SP_target_escapetrig(gentity_t *ent);

void SP_misc_maglock(gentity_t *self);

void SP_misc_faller(gentity_t *ent);

void SP_misc_holocron(gentity_t *ent);

void SP_reference_tag(gentity_t *ent);

void SP_misc_weapon_shooter(gentity_t *self);

void SP_NPC_spawner(gentity_t *self);

void SP_NPC_Vehicle(gentity_t *self);

void SP_NPC_Kyle(gentity_t *self);
void SP_NPC_Lando(gentity_t *self);
void SP_NPC_Jan(gentity_t *self);
void SP_NPC_Luke(gentity_t *self);
void SP_NPC_MonMothma(gentity_t *self);
void SP_NPC_Tavion(gentity_t *self);
void SP_NPC_Tavion_New(gentity_t *self);
void SP_NPC_Alora(gentity_t *self);
void SP_NPC_Reelo(gentity_t *self);
void SP_NPC_Galak(gentity_t *self);
void SP_NPC_Desann(gentity_t *self);
void SP_NPC_Bartender(gentity_t *self);
void SP_NPC_MorganKatarn(gentity_t *self);
void SP_NPC_Jedi(gentity_t *self);
void SP_NPC_Prisoner(gentity_t *self);
void SP_NPC_Rebel(gentity_t *self);
void SP_NPC_Human_Merc(gentity_t *self);
void SP_NPC_Stormtrooper(gentity_t *self);
void SP_NPC_StormtrooperOfficer(gentity_t *self);
void SP_NPC_Snowtrooper(gentity_t *self);
void SP_NPC_Tie_Pilot(gentity_t *self);
void SP_NPC_Ugnaught(gentity_t *self);
void SP_NPC_Jawa(gentity_t *self);
void SP_NPC_Gran(gentity_t *self);
void SP_NPC_Rodian(gentity_t *self);
void SP_NPC_Weequay(gentity_t *self);
void SP_NPC_Trandoshan(gentity_t *self);
void SP_NPC_Tusken(gentity_t *self);
void SP_NPC_Noghri(gentity_t *self);
void SP_NPC_SwampTrooper(gentity_t *self);
void SP_NPC_Imperial(gentity_t *self);
void SP_NPC_ImpWorker(gentity_t *self);
void SP_NPC_BespinCop(gentity_t *self);
void SP_NPC_Reborn(gentity_t *self);
void SP_NPC_ShadowTrooper(gentity_t *self);
void SP_NPC_Monster_Murjj(gentity_t *self);
void SP_NPC_Monster_Swamp(gentity_t *self);
void SP_NPC_Monster_Howler(gentity_t *self);
void SP_NPC_Monster_Claw(gentity_t *self);
void SP_NPC_Monster_Glider(gentity_t *self);
void SP_NPC_Monster_Flier2(gentity_t *self);
void SP_NPC_Monster_Lizard(gentity_t *self);
void SP_NPC_Monster_Fish(gentity_t *self);
void SP_NPC_Monster_Wampa(gentity_t *self);
void SP_NPC_Monster_Rancor(gentity_t *self);
void SP_NPC_MineMonster(gentity_t *self);
void SP_NPC_Droid_Interrogator(gentity_t *self);
void SP_NPC_Droid_Probe(gentity_t *self);
void SP_NPC_Droid_Mark1(gentity_t *self);
void SP_NPC_Droid_Mark2(gentity_t *self);
void SP_NPC_Droid_ATST(gentity_t *self);
void SP_NPC_Droid_Seeker(gentity_t *self);
void SP_NPC_Droid_Remote(gentity_t *self);
void SP_NPC_Droid_Sentry(gentity_t *self);
void SP_NPC_Droid_Gonk(gentity_t *self);
void SP_NPC_Droid_Mouse(gentity_t *self);
void SP_NPC_Droid_R2D2(gentity_t *self);
void SP_NPC_Droid_R5D2(gentity_t *self);
void SP_NPC_Droid_Protocol(gentity_t *self);

void SP_NPC_Reborn_New(gentity_t *self);
void SP_NPC_Cultist(gentity_t *self);
void SP_NPC_Cultist_Saber(gentity_t *self);
void SP_NPC_Cultist_Saber_Powers(gentity_t *self);
void SP_NPC_Cultist_Destroyer(gentity_t *self);
void SP_NPC_Cultist_Commando(gentity_t *self);

void SP_NPC_Rosh_Penin(gentity_t *self);

void SP_waypoint(gentity_t *ent);
void SP_waypoint_small(gentity_t *ent);
void SP_waypoint_navgoal(gentity_t *ent);
void SP_waypoint_navgoal_8(gentity_t *ent);
void SP_waypoint_navgoal_4(gentity_t *ent);
void SP_waypoint_navgoal_2(gentity_t *ent);
void SP_waypoint_navgoal_1(gentity_t *ent);

void SP_CreateWind(gentity_t *ent);
void SP_CreateSpaceDust(gentity_t *ent);
void SP_CreateSnow(gentity_t *ent);
void SP_CreateRain(gentity_t *ent);

void SP_point_combat(gentity_t *self);

void SP_shooter_blaster(gentity_t *ent);

void SP_team_CTF_redplayer(gentity_t *ent);
void SP_team_CTF_blueplayer(gentity_t *ent);

void SP_team_CTF_redspawn(gentity_t *ent);
void SP_team_CTF_bluespawn(gentity_t *ent);

void SP_misc_turret(gentity_t *ent);
void SP_misc_turretG2(gentity_t *base);

void SP_item_botroam(gentity_t *ent) {}

void SP_gametype_item(gentity_t *ent) {
    const gitem_t *item = NULL;
    char *value;
    int team = -1;

    G_SpawnString("teamfilter", "", &value);

    G_SetOrigin(ent, &ent->s.origin);

    // If a team filter is set then override any team settings for the spawns
    if (level.mTeamFilter[0]) {
        if (Q_stricmp(level.mTeamFilter, "red") == 0) {
            team = TEAM_RED;
        } else if (Q_stricmp(level.mTeamFilter, "blue") == 0) {
            team = TEAM_BLUE;
        }
    }

    if (ent->targetname && ent->targetname[0]) {
        if (team != -1) {
            if (strstr(ent->targetname, "flag")) {
                if (team == TEAM_RED) {
                    item = BG_FindItem("team_CTF_redflag");
                } else { // blue
                    item = BG_FindItem("team_CTF_blueflag");
                }
            }
        } else if (strstr(ent->targetname, "red_flag")) {
            item = BG_FindItem("team_CTF_redflag");
        } else if (strstr(ent->targetname, "blue_flag")) {
            item = BG_FindItem("team_CTF_blueflag");
        } else {
            item = NULL;
        }

        if (item) {
            ent->targetname = NULL;
            ent->classname = (char *)item->classname;
            G_SpawnItem(ent, item);
        }
    }
}

void SP_emplaced_gun(gentity_t *ent);

// Raz: PORTALS LOLWOT

void MatrixMultiply3f(vector3 m[3], vector3 *vec, vector3 *result) {
    result->x = m[0].x * vec->x + m[0].y * vec->y + m[0].z * vec->z;
    result->y = m[1].x * vec->x + m[1].y * vec->y + m[1].z * vec->z;
    result->z = m[2].x * vec->x + m[2].y * vec->y + m[2].z * vec->z;
}

void MatrixIdentity(vector3 m[3]) {
    VectorSet(&m[0], 1, 0, 0);
    VectorSet(&m[1], 0, 1, 0);
    VectorSet(&m[2], 0, 0, 1);
}

void jp_portal_touch(gentity_t *self, gentity_t *other, trace_t *trace) {
    // Remove the touch function for a while to avoid flooding
    self->genericValue1 = level.time;
    self->touch = NULL;

    if (self->s.health != -1) {
        vector3 tpPos;
        vector3 sourceVelocity, destVelocity;
        vector3 sourceNormal, destNormal;

        // Fetch normals
        VectorCopy(&self->s.boneAngles1, &sourceNormal);
        VectorCopy(&g_entities[self->s.health].s.boneAngles1, &destNormal);

        // Set position to teleport to
        VectorCopy(&g_entities[self->s.health].s.origin, &tpPos);
        VectorMA(&tpPos, 32.0f, &destNormal, &tpPos);

        // BEGIN position
        if (other->client) {
            qboolean isNPC = (other->s.eType == ET_NPC);
            uint32_t eFlags = other->client->ps.eFlags ^ EF_TELEPORT_BIT;
            VectorCopy(&tpPos, &other->client->ps.origin);
            other->client->ps.origin.z += 1.0f;
            SetClientViewAngle(other, &other->client->ps.viewangles);
            other->client->ps.eFlags ^= EF_TELEPORT_BIT;
            BG_PlayerStateToEntityState(&other->client->ps, &other->s, qtrue);
            if (isNPC)
                other->s.eType = ET_NPC;
            other->s.eFlags = eFlags;
            VectorCopy(&other->client->ps.origin, &other->r.currentOrigin);
        } else {
            VectorCopy(&tpPos, &other->s.pos.trBase);
        }
        // END position

        // BEGIN velocity
        if (other->client) { // Players and NPCs
            float reflectFactor;
            vector3 exitVector;
            vector3 velNormal;
            float length;
            vector3 debugLine;

            VectorNormalize2(&other->client->ps.velocity, &velNormal);
            reflectFactor = 2 * DotProduct(&sourceNormal, &velNormal);

            VectorCopy(&other->client->ps.velocity, &sourceVelocity);
            length = VectorLength(&sourceVelocity);

            exitVector.x = velNormal.x - reflectFactor * sourceNormal.x;
            exitVector.y = velNormal.y - reflectFactor * sourceNormal.y;
            exitVector.z = velNormal.z - reflectFactor * sourceNormal.z;

            VectorMA(&self->s.origin, -128.0f, &velNormal, &debugLine);
            G_TestLine(&debugLine, &self->s.origin, 0xF0, 7500);

            MatrixMultiply3f(self->portal_matrix, &exitVector, &destVelocity);

            VectorMA(&g_entities[self->s.health].s.origin, 128.0f, &destVelocity, &debugLine);
            G_TestLine(&g_entities[self->s.health].s.origin, &debugLine, 0x0F, 7500);

            VectorScale(&destVelocity, length, &destVelocity);
        } else { // Projectiles
            byteAlias_t ba;
            float velocity;
            ba.i = other->genericValue1; // stored during missile creation
            velocity = ba.f;
            //	VectorScale( destNormal, velocity, other->s.pos.trDelta );
            VectorMA(&other->s.pos.trBase, velocity, &destNormal, &other->s.pos.trDelta);
            other->s.pos.trTime = level.time;
            other->nextthink = level.time + other->genericValue2;
        }

        // VectorSet( destVelocity, destNormal.x*speed, destNormal.y*speed, destNormal.z*speed );
        if (other->client) { // Players and NPCs
            VectorCopy(&destVelocity, &other->client->ps.velocity);
            other->client->ps.pm_flags |= PMF_DUCKED;
            other->s.solid = 0;
        } else { // Projectiles
            VectorCopy(&destVelocity, &other->s.pos.trDelta);
            VectorCopy(&tpPos, &other->r.currentOrigin);
        }
        // END velocity

        g_entities[self->s.health].genericValue1 = level.time;
        g_entities[self->s.health].touch = NULL;
    }
    return;
}

void jp_portal_think(gentity_t *self) {
    //	G_PlayEffectID( G_EffectIndex( self->genericValue2 ? "env/portal2" : "env/portal1" ), self->s.origin, self->s.boneAngles1 );
    self->nextthink = level.time + self->count;

    // If it's been 100 milliseconds since the last touch, activate it again
    if (self->genericValue1 < level.time - 100)
        self->touch = jp_portal_touch;
    return;
}

void fx_runner_link(gentity_t *ent);
void SP_jp_portal(gentity_t *ent) {
    gentity_t *found = NULL, *from = NULL;

    // G_SpawnInt( "source", "1", &ent->userInt2 );
    ent->s.health = -1; // no linked portal

    while ((found = G_Find(from, FOFS(classname), "jp_portal"))) { // search for other portals, so we can remove our old 'self' or link with partners
        if (found == ent) {                                        // Found ourself
            from = ent;
            continue;
        }
        if (found->s.userInt2 == ent->s.userInt2) { // Found our old portal
            G_FreeEntity(found);
        } else { // Found a portal to link with
            vector3 mat[3], mat_inv[3];

            ent->s.health = found->s.number;
            found->s.health = ent->s.number;
            from = found;

            mat[0].x = found->s.boneAngles2.x;
            mat[1].x = found->s.boneAngles2.y;
            mat[2].x = found->s.boneAngles2.z;
            mat[0].y = found->s.boneAngles3.x;
            mat[1].y = found->s.boneAngles3.y;
            mat[2].y = found->s.boneAngles3.z;
            mat[0].z = found->s.boneAngles1.x;
            mat[1].z = found->s.boneAngles1.y;
            mat[2].z = found->s.boneAngles1.z;

            mat_inv[0].x = ent->s.boneAngles2.x;
            mat_inv[0].y = ent->s.boneAngles2.y;
            mat_inv[0].z = ent->s.boneAngles2.z;
            mat_inv[1].x = ent->s.boneAngles3.x;
            mat_inv[1].y = ent->s.boneAngles3.y;
            mat_inv[1].z = ent->s.boneAngles3.z;
            mat_inv[2].x = ent->s.boneAngles1.x;
            mat_inv[2].y = ent->s.boneAngles1.y;
            mat_inv[2].z = ent->s.boneAngles1.z;

            /*
            if ( VectorLength( mat_inv[0] ) == 0.0f || VectorLength( mat_inv[1] ) == 0.0f || VectorLength( mat_inv[2] ) == 0.0f )
            MatrixIdentity( mat_inv );

            if ( VectorLength( mat[0] ) == 0.0f || VectorLength( mat[1] ) == 0.0f || VectorLength( mat[2] ) == 0.0f )
            MatrixIdentity( mat );
            */

            MatrixMultiply(mat, mat_inv, ent->portal_matrix);

            mat[0].x = ent->s.boneAngles2.x;
            mat[1].x = ent->s.boneAngles2.y;
            mat[2].x = ent->s.boneAngles2.z;
            mat[0].y = ent->s.boneAngles3.x;
            mat[1].y = ent->s.boneAngles3.y;
            mat[2].y = ent->s.boneAngles3.z;
            mat[0].z = ent->s.boneAngles1.x;
            mat[1].z = ent->s.boneAngles1.y;
            mat[2].z = ent->s.boneAngles1.z;

            mat_inv[0].x = found->s.boneAngles2.x;
            mat_inv[0].y = found->s.boneAngles2.y;
            mat_inv[0].z = found->s.boneAngles2.z;
            mat_inv[1].x = found->s.boneAngles3.x;
            mat_inv[1].y = found->s.boneAngles3.y;
            mat_inv[1].z = found->s.boneAngles3.z;
            mat_inv[2].x = found->s.boneAngles1.x;
            mat_inv[2].y = found->s.boneAngles1.y;
            mat_inv[2].z = found->s.boneAngles1.z;

            MatrixMultiply(mat, mat_inv, found->portal_matrix);
        }
    }

    ent->s.eType = ET_SPECIAL;
    ent->s.userInt1 = 1;                                  // is a jp_portal
    ent->r.contents = CONTENTS_TRIGGER | CONTENTS_CORPSE; // actually solid, helps projectiles trigger us.
    ent->clipmask = MASK_SOLID;
    VectorSet(&ent->r.mins, -16.0f, -16.0f, 0.0f);
    VectorSet(&ent->r.maxs, 16.0f, 16.0f, 32.0f);

    G_SetOrigin(ent, &ent->s.origin);

    ent->think = jp_portal_think;
    ent->touch = jp_portal_touch;
    ent->nextthink = level.time + ent->count;

    trap->LinkEntity((sharedEntity_t *)ent);

    return;
}

spawn_t spawns[] = {
    {"emplaced_gun", SP_emplaced_gun},
    {"func_bobbing", SP_func_bobbing},
    {"func_breakable", SP_func_breakable},
    {"func_button", SP_func_button},
    {"func_door", SP_func_door},
    {"func_glass", SP_func_glass},
    {"func_group", SP_info_null},
    {"func_pendulum", SP_func_pendulum},
    {"func_plat", SP_func_plat},
    {"func_rotating", SP_func_rotating},
    {"func_static", SP_func_static},
    {"func_timer", SP_func_timer}, // rename trigger_timer?
    {"func_train", SP_func_train},
    {"func_usable", SP_func_usable},
    {"func_wall", SP_func_wall},
    {"fx_rain", SP_CreateRain},
    {"fx_runner", SP_fx_runner},
    {"fx_snow", SP_CreateSnow},
    {"fx_spacedust", SP_CreateSpaceDust},
    {"fx_wind", SP_CreateWind},
    {"gametype_item", SP_gametype_item},
    {"info_camp", SP_info_camp},
    {"info_jedimaster_start", SP_info_jedimaster_start},
    {"info_notnull", SP_info_notnull}, // use target_position instead
    {"info_null", SP_info_null},
    {"info_player_deathmatch", SP_info_player_deathmatch},
    {"info_player_duel", SP_info_player_duel},
    {"info_player_duel1", SP_info_player_duel1},
    {"info_player_duel2", SP_info_player_duel2},
    {"info_player_intermission", SP_info_player_intermission},
    {"info_player_intermission_blue", SP_info_player_intermission_blue},
    {"info_player_intermission_red", SP_info_player_intermission_red},
    {"info_player_siegeteam1", SP_info_player_siegeteam1},
    {"info_player_siegeteam2", SP_info_player_siegeteam2},
    {"info_player_start", SP_info_player_start},
    {"info_player_start_blue", SP_info_player_start_blue},
    {"info_player_start_red", SP_info_player_start_red},
    {"info_siege_decomplete", SP_info_siege_decomplete},
    {"info_siege_objective", SP_info_siege_objective},
    {"info_siege_radaricon", SP_info_siege_radaricon},
    {"item_botroam", SP_item_botroam},
    {"jp_portal", SP_jp_portal},
    {"light", SP_light},
    {"misc_ammo_floor_unit", SP_misc_ammo_floor_unit},
    {"misc_bsp", SP_misc_bsp},
    {"misc_faller", SP_misc_faller},
    {"misc_G2model", SP_misc_G2model},
    {"misc_holocron", SP_misc_holocron},
    {"misc_maglock", SP_misc_maglock},
    {"misc_model", SP_misc_model},
    {"misc_model_ammo_power_converter", SP_misc_model_ammo_power_converter},
    {"misc_model_health_power_converter", SP_misc_model_health_power_converter},
    {"misc_model_shield_power_converter", SP_misc_model_shield_power_converter},
    {"misc_model_static", SP_misc_model_static},
    {"misc_portal_camera", SP_misc_portal_camera},
    {"misc_portal_surface", SP_misc_portal_surface},
    {"misc_shield_floor_unit", SP_misc_shield_floor_unit},
    {"misc_siege_item", SP_misc_siege_item},
    {"misc_skyportal", SP_misc_skyportal},
    {"misc_skyportal_orient", SP_misc_skyportal_orient},
    {"misc_teleporter_dest", SP_misc_teleporter_dest},
    {"misc_turret", SP_misc_turret},
    {"misc_turretG2", SP_misc_turretG2},
    {"misc_weapon_shooter", SP_misc_weapon_shooter},
    {"misc_weather_zone", SP_misc_weather_zone},
    {"npc_alora", SP_NPC_Alora},
    {"npc_bartender", SP_NPC_Bartender},
    {"npc_bespincop", SP_NPC_BespinCop},
    {"npc_colombian_emplacedgunner", SP_NPC_ShadowTrooper},
    {"npc_colombian_rebel", SP_NPC_Reborn},
    {"npc_colombian_soldier", SP_NPC_Reborn},
    {"npc_cultist", SP_NPC_Cultist},
    {"npc_cultist_commando", SP_NPC_Cultist_Commando},
    {"npc_cultist_destroyer", SP_NPC_Cultist_Destroyer},
    {"npc_cultist_saber", SP_NPC_Cultist_Saber},
    {"npc_cultist_saber_powers", SP_NPC_Cultist_Saber_Powers},
    {"npc_desann", SP_NPC_Desann},
    {"npc_droid_atst", SP_NPC_Droid_ATST},
    {"npc_droid_gonk", SP_NPC_Droid_Gonk},
    {"npc_droid_interrogator", SP_NPC_Droid_Interrogator},
    {"npc_droid_mark1", SP_NPC_Droid_Mark1},
    {"npc_droid_mark2", SP_NPC_Droid_Mark2},
    {"npc_droid_mouse", SP_NPC_Droid_Mouse},
    {"npc_droid_probe", SP_NPC_Droid_Probe},
    {"npc_droid_protocol", SP_NPC_Droid_Protocol},
    {"npc_droid_r2d2", SP_NPC_Droid_R2D2},
    {"npc_droid_r5d2", SP_NPC_Droid_R5D2},
    {"npc_droid_remote", SP_NPC_Droid_Remote},
    {"npc_droid_seeker", SP_NPC_Droid_Seeker},
    {"npc_droid_sentry", SP_NPC_Droid_Sentry},
    {"npc_galak", SP_NPC_Galak},
    {"npc_gran", SP_NPC_Gran},
    {"npc_human_merc", SP_NPC_Human_Merc},
    {"npc_imperial", SP_NPC_Imperial},
    {"npc_impworker", SP_NPC_ImpWorker},
    {"npc_jan", SP_NPC_Jan},
    {"npc_jawa", SP_NPC_Jawa},
    {"npc_jedi", SP_NPC_Jedi},
    {"npc_kyle", SP_NPC_Kyle},
    {"npc_lando", SP_NPC_Lando},
    {"npc_luke", SP_NPC_Luke},
    {"npc_manuel_vergara_rmg", SP_NPC_Desann},
    {"npc_minemonster", SP_NPC_MineMonster},
    {"npc_monmothma", SP_NPC_MonMothma},
    {"npc_monster_claw", SP_NPC_Monster_Claw},
    {"npc_monster_fish", SP_NPC_Monster_Fish},
    {"npc_monster_flier2", SP_NPC_Monster_Flier2},
    {"npc_monster_glider", SP_NPC_Monster_Glider},
    {"npc_monster_howler", SP_NPC_Monster_Howler},
    {"npc_monster_lizard", SP_NPC_Monster_Lizard},
    {"npc_monster_murjj", SP_NPC_Monster_Murjj},
    {"npc_monster_rancor", SP_NPC_Monster_Rancor},
    {"npc_monster_swamp", SP_NPC_Monster_Swamp},
    {"npc_monster_wampa", SP_NPC_Monster_Wampa},
    {"npc_morgankatarn", SP_NPC_MorganKatarn},
    {"npc_noghri", SP_NPC_Noghri},
    {"npc_prisoner", SP_NPC_Prisoner},
    {"npc_rebel", SP_NPC_Rebel},
    {"npc_reborn", SP_NPC_Reborn},
    {"npc_reborn_new", SP_NPC_Reborn_New},
    {"npc_reelo", SP_NPC_Reelo},
    {"npc_rodian", SP_NPC_Rodian},
    {"npc_rosh_penin", SP_NPC_Rosh_Penin},
    {"npc_shadowtrooper", SP_NPC_ShadowTrooper},
    {"npc_snowtrooper", SP_NPC_Snowtrooper},
    {"npc_spawner", SP_NPC_spawner},
    {"npc_stormtrooper", SP_NPC_Stormtrooper},
    {"npc_stormtrooperofficer", SP_NPC_StormtrooperOfficer},
    {"npc_swamptrooper", SP_NPC_SwampTrooper},
    {"npc_tavion", SP_NPC_Tavion},
    {"npc_tavion_new", SP_NPC_Tavion_New},
    {"npc_tie_pilot", SP_NPC_Tie_Pilot},
    {"npc_trandoshan", SP_NPC_Trandoshan},
    {"npc_tusken", SP_NPC_Tusken},
    {"npc_ugnaught", SP_NPC_Ugnaught},
    {"npc_vehicle", SP_NPC_Vehicle},
    {"npc_weequay", SP_NPC_Weequay},
    {"path_corner", SP_path_corner},
    {"point_combat", SP_point_combat},
    {"ref_tag", SP_reference_tag},
    {"ref_tag_huge", SP_reference_tag},
    {"shooter_blaster", SP_shooter_blaster},
    {"target_activate", SP_target_activate},
    {"target_counter", SP_target_counter},
    {"target_deactivate", SP_target_deactivate},
    {"target_delay", SP_target_delay},
    {"target_escapetrig", SP_target_escapetrig},
    {"target_give", SP_target_give},
    {"target_interest", SP_target_interest},
    {"target_kill", SP_target_kill},
    {"target_laser", SP_target_laser},
    {"target_level_change", SP_target_level_change},
    {"target_location", SP_target_location},
    {"target_play_music", SP_target_play_music},
    {"target_position", SP_target_position},
    {"target_print", SP_target_print},
    {"target_push", SP_target_push},
    {"target_random", SP_target_random},
    {"target_relay", SP_target_relay},
    {"target_remove_powerups", SP_target_remove_powerups},
    {"target_score", SP_target_score},
    {"target_screenshake", SP_target_screenshake},
    {"target_scriptrunner", SP_target_scriptrunner},
    {"target_siege_end", SP_target_siege_end},
    {"target_speaker", SP_target_speaker},
    {"target_teleporter", SP_target_teleporter},
    {"team_CTF_blueplayer", SP_team_CTF_blueplayer},
    {"team_CTF_bluespawn", SP_team_CTF_bluespawn},
    {"team_CTF_redplayer", SP_team_CTF_redplayer},
    {"team_CTF_redspawn", SP_team_CTF_redspawn},
    {"terrain", SP_terrain},
    {"trigger_always", SP_trigger_always},
    {"trigger_asteroid_field", SP_trigger_asteroid_field},
    {"trigger_hurt", SP_trigger_hurt},
    {"trigger_hyperspace", SP_trigger_hyperspace},
    {"trigger_lightningstrike", SP_trigger_lightningstrike},
    {"trigger_multiple", SP_trigger_multiple},
    {"trigger_once", SP_trigger_once},
    {"trigger_push", SP_trigger_push},
    {"trigger_shipboundary", SP_trigger_shipboundary},
    {"trigger_space", SP_trigger_space},
    {"trigger_teleport", SP_trigger_teleport},
    {"waypoint", SP_waypoint},
    {"waypoint_navgoal", SP_waypoint_navgoal},
    {"waypoint_navgoal_1", SP_waypoint_navgoal_1},
    {"waypoint_navgoal_2", SP_waypoint_navgoal_2},
    {"waypoint_navgoal_4", SP_waypoint_navgoal_4},
    {"waypoint_navgoal_8", SP_waypoint_navgoal_8},
    {"waypoint_small", SP_waypoint_small},
};

static int spawncmp(const void *a, const void *b) { return Q_stricmp((const char *)a, ((spawn_t *)b)->name); }

// Finds the spawn function for the entity and calls it, returning qfalse if not found
qboolean G_CallSpawn(gentity_t *ent) {
    spawn_t *s;
    const gitem_t *item;
    char buf[MAX_STRING_CHARS] = {0};

    if (!ent->classname) {
        trap->Print("G_CallSpawn: NULL classname\n");
        return qfalse;
    }

    // swap this entity out for something else
    trap->Cvar_VariableStringBuffer(va("replace_%s", ent->classname), buf, sizeof(buf));
    if (buf[0])
        ent->classname = G_NewString(buf);

    // check item spawn functions
    // RAZTODO: cant reorder items because compat so....?
    for (item = bg_itemlist + 1; item->classname; item++) {
        if (!strcmp(item->classname, ent->classname)) {
            G_SpawnItem(ent, item);
            return qtrue;
        }
    }

    // check normal spawn functions
    s = (spawn_t *)bsearch(ent->classname, spawns, ARRAY_LEN(spawns), sizeof(spawn_t), spawncmp);
    if (s) { // found it
        if (VALIDSTRING(ent->healingsound))
            G_SoundIndex(ent->healingsound);

        s->spawn(ent);
        return qtrue;
    }

    trap->Print("%s doesn't have a spawn function\n", ent->classname);
    return qfalse;
}

// Builds a copy of the string, translating \n to real linefeeds so message texts can be multi-line
char *G_NewString(const char *string) {
    char *newb, *new_p;
    int i, len;

    len = strlen(string) + 1;

    // Raz: dyn alloc, not G_Alloc :O
    //	newb = (char *) G_Alloc( l );
    newb = (char *)malloc(len);

    new_p = newb;

    // turn \n into a real linefeed
    for (i = 0; i < len; i++) {
        if (string[i] == '\\' && i < len - 1) {
            if (string[i + 1] == 'n') {
                *new_p++ = '\n';
                i++;
            } else {
                *new_p++ = '\\';
            }
        } else {
            *new_p++ = string[i];
        }
    }

    return newb;
}

void BG_ParseField(const BG_field_t *l_fields, int numFields, const char *key, const char *value, byte *ent);
// Spawn an entity and fill in all of the level fields from level.spawnVars[], then call the class specfic spawn function
gentity_t *G_SpawnGEntityFromSpawnVars(qboolean inSubBSP) {
    int i;
    gentity_t *ent;
    char *s, *value;
    const char *gametypeName;
    static const char *gametypeNames[GT_MAX_GAME_TYPE] = {"ffa", "holocron", "jedimaster", "duel", "powerduel", "single", "team", "siege", "ctf", "cty"};

    // get the next free entity
    ent = G_Spawn();

    for (i = 0; i < level.numSpawnVars; i++) {
        BG_ParseField(fields, ARRAY_LEN(fields), level.spawnVars[i][0], level.spawnVars[i][1], (byte *)ent);
    }

    // check for "notsingle" flag
    if (level.gametype == GT_SINGLE_PLAYER) {
        G_SpawnInt("notsingle", "0", &i);
        if (i) {
            G_FreeEntity(ent);
            return NULL;
        }
    }
    // check for "notteam" flag (GT_FFA, GT_DUEL, GT_SINGLE_PLAYER)
    if (level.gametype >= GT_TEAM) {
        G_SpawnInt("notteam", "0", &i);
        if (i) {
            G_FreeEntity(ent);
            return NULL;
        }
    } else {
        G_SpawnInt("notfree", "0", &i);
        if (i) {
            G_FreeEntity(ent);
            return NULL;
        }
    }

    G_SpawnInt("notta", "0", &i);
    if (i) {
        G_FreeEntity(ent);
        return NULL;
    }

    if (G_SpawnString("gametype", NULL, &value)) {
        if (level.gametype >= GT_FFA && level.gametype < GT_MAX_GAME_TYPE) {
            gametypeName = gametypeNames[level.gametype];

            s = strstr(value, gametypeName);
            if (!s) {
                G_FreeEntity(ent);
                return NULL;
            }
        }
    }

    // move editor origin to pos
    VectorCopy(&ent->s.origin, &ent->s.pos.trBase);
    VectorCopy(&ent->s.origin, &ent->r.currentOrigin);

    // if we didn't get a classname, don't bother spawning anything
    if (!G_CallSpawn(ent)) {
        G_FreeEntity(ent);
        return NULL;
    }

    // Tag on the ICARUS scripting information only to valid recipients
    if (trap->ICARUS_ValidEnt((sharedEntity_t *)ent)) {
        trap->ICARUS_InitEnt((sharedEntity_t *)ent);

        if (ent->classname && ent->classname[0]) {
            if (Q_strncmp("NPC_", ent->classname, 4) != 0) { // Not an NPC_spawner (rww - probably don't even care for MP, but whatever)
                G_ActivateBehavior(ent, BSET_SPAWN);
            }
        }
    }

    if (level.manualSpawning) {
        ent->jpSpawned = qtrue;
    }
    return ent;
}

char *G_AddSpawnVarToken(const char *string) {
    int l;
    char *dest;

    l = strlen(string);
    if (level.numSpawnVarChars + l + 1 > MAX_SPAWN_VARS_CHARS) {
        trap->Error(ERR_DROP, "G_AddSpawnVarToken: MAX_SPAWN_CHARS");
    }

    dest = level.spawnVarChars + level.numSpawnVarChars;
    memcpy(dest, string, l + 1);

    level.numSpawnVarChars += l + 1;

    return dest;
}

void AddSpawnField(const char *field, char *value) {
    int i;

    for (i = 0; i < level.numSpawnVars; i++) {
        if (!Q_stricmp(level.spawnVars[i][0], field)) {
            level.spawnVars[i][1] = G_AddSpawnVarToken(value);
            return;
        }
    }

    level.spawnVars[level.numSpawnVars][0] = G_AddSpawnVarToken(field);
    level.spawnVars[level.numSpawnVars][1] = G_AddSpawnVarToken(value);
    level.numSpawnVars++;
}

#define NOVALUE "novalue"

static void HandleEntityAdjustment(void) {
    char *value;
    vector3 origin, newOrigin, angles;
    char temp[MAX_QPATH];
    float rotation;

    G_SpawnString("origin", NOVALUE, &value);
    if (Q_stricmp(value, NOVALUE) != 0) {
        sscanf(value, "%f %f %f", &origin.x, &origin.y, &origin.z);
    } else {
        origin.x = origin.y = origin.z = 0.0f;
    }

    rotation = DEG2RAD(level.mRotationAdjust);
    newOrigin.x = origin.x * cosf(rotation) - origin.y * sinf(rotation);
    newOrigin.y = origin.x * sinf(rotation) + origin.y * cosf(rotation);
    newOrigin.z = origin.z;
    VectorAdd(&newOrigin, &level.mOriginAdjust, &newOrigin);
    // damn VMs don't handle outputing a float that is compatible with sscanf in all cases
    Com_sprintf(temp, MAX_QPATH, "%0.0f %0.0f %0.0f", (double)newOrigin.x, (double)newOrigin.y, (double)newOrigin.z);
    AddSpawnField("origin", temp);

    G_SpawnString("angles", NOVALUE, &value);
    if (Q_stricmp(value, NOVALUE) != 0) {
        sscanf(value, "%f %f %f", &angles.x, &angles.y, &angles.z);

        angles.yaw = fmodf(angles.y + level.mRotationAdjust, 360.0f);
        // damn VMs don't handle outputing a float that is compatible with sscanf in all cases
        Com_sprintf(temp, MAX_QPATH, "%0.0f %0.0f %0.0f", (double)angles.x, (double)angles.y, (double)angles.z);
        AddSpawnField("angles", temp);
    } else {
        G_SpawnString("angle", NOVALUE, &value);
        if (Q_stricmp(value, NOVALUE) != 0) {
            sscanf(value, "%f", &angles.y);
        } else {
            angles.y = 0.0f;
        }
        angles.y = fmodf(angles.y + level.mRotationAdjust, 360.0f);
        Com_sprintf(temp, MAX_QPATH, "%0.0f", (double)angles.y);
        AddSpawnField("angle", temp);
    }

    // RJR experimental code for handling "direction" field of breakable brushes
    // though direction is rarely ever used.
    G_SpawnString("direction", NOVALUE, &value);
    if (Q_stricmp(value, NOVALUE) != 0) {
        sscanf(value, "%f %f %f", &angles.x, &angles.y, &angles.z);
    } else {
        angles.x = angles.y = angles.z = 0.0f;
    }
    angles.y = fmodf(angles.y + level.mRotationAdjust, 360.0f);
    Com_sprintf(temp, MAX_QPATH, "%0.0f %0.0f %0.0f", (double)angles.x, (double)angles.y, (double)angles.z);
    AddSpawnField("direction", temp);

    AddSpawnField("BSPInstanceID", level.mTargetAdjust);

    G_SpawnString("targetname", NOVALUE, &value);
    if (Q_stricmp(value, NOVALUE) != 0) {
        Com_sprintf(temp, MAX_QPATH, "%s%s", level.mTargetAdjust, value);
        AddSpawnField("targetname", temp);
    }

    G_SpawnString("target", NOVALUE, &value);
    if (Q_stricmp(value, NOVALUE) != 0) {
        Com_sprintf(temp, MAX_QPATH, "%s%s", level.mTargetAdjust, value);
        AddSpawnField("target", temp);
    }

    G_SpawnString("killtarget", NOVALUE, &value);
    if (Q_stricmp(value, NOVALUE) != 0) {
        Com_sprintf(temp, MAX_QPATH, "%s%s", level.mTargetAdjust, value);
        AddSpawnField("killtarget", temp);
    }

    G_SpawnString("brushparent", NOVALUE, &value);
    if (Q_stricmp(value, NOVALUE) != 0) {
        Com_sprintf(temp, MAX_QPATH, "%s%s", level.mTargetAdjust, value);
        AddSpawnField("brushparent", temp);
    }

    G_SpawnString("brushchild", NOVALUE, &value);
    if (Q_stricmp(value, NOVALUE) != 0) {
        Com_sprintf(temp, MAX_QPATH, "%s%s", level.mTargetAdjust, value);
        AddSpawnField("brushchild", temp);
    }

    G_SpawnString("enemy", NOVALUE, &value);
    if (Q_stricmp(value, NOVALUE) != 0) {
        Com_sprintf(temp, MAX_QPATH, "%s%s", level.mTargetAdjust, value);
        AddSpawnField("enemy", temp);
    }

    G_SpawnString("ICARUSname", NOVALUE, &value);
    if (Q_stricmp(value, NOVALUE) != 0) {
        Com_sprintf(temp, MAX_QPATH, "%s%s", level.mTargetAdjust, value);
        AddSpawnField("ICARUSname", temp);
    }
}

// Parses a brace bounded set of key / value pairs out of the level's entity strings into level.spawnVars[]
// This does not actually spawn an entity.
qboolean G_ParseSpawnVars(qboolean inSubBSP) {
    char keyname[MAX_TOKEN_CHARS];
    char com_token[MAX_TOKEN_CHARS];

    level.numSpawnVars = 0;
    level.numSpawnVarChars = 0;

    // parse the opening brace
    if (!trap->GetEntityToken(com_token, sizeof(com_token))) {
        // end of spawn string
        return qfalse;
    }
    if (com_token[0] != '{') {
        trap->Error(ERR_DROP, "G_ParseSpawnVars: found %s when expecting {", com_token);
    }

    // go through all the key / value pairs
    while (1) {
        // parse key
        if (!trap->GetEntityToken(keyname, sizeof(keyname))) {
            trap->Error(ERR_DROP, "G_ParseSpawnVars: EOF without closing brace");
        }

        if (keyname[0] == '}') {
            break;
        }

        // parse value
        if (!trap->GetEntityToken(com_token, sizeof(com_token))) {
            trap->Error(ERR_DROP, "G_ParseSpawnVars: EOF without closing brace");
        }

        if (com_token[0] == '}') {
            trap->Error(ERR_DROP, "G_ParseSpawnVars: closing brace without data");
        }
        if (level.numSpawnVars == MAX_SPAWN_VARS) {
            trap->Error(ERR_DROP, "G_ParseSpawnVars: MAX_SPAWN_VARS");
        }
        level.spawnVars[level.numSpawnVars][0] = G_AddSpawnVarToken(keyname);
        level.spawnVars[level.numSpawnVars][1] = G_AddSpawnVarToken(com_token);
        level.numSpawnVars++;
    }

    if (inSubBSP) {
        HandleEntityAdjustment();
    }

    return qtrue;
}

static const char *defaultStyles[32][3] = {
    {// 0 normal
     "z", "z", "z"},
    {// 1 FLICKER (first variety)
     "mmnmmommommnonmmonqnmmo", "mmnmmommommnonmmonqnmmo", "mmnmmommommnonmmonqnmmo"},
    {// 2 SLOW STRONG PULSE
     "abcdefghijklmnopqrstuvwxyzyxwvutsrqponmlkjihgfedcb", "abcdefghijklmnopqrstuvwxyzyxwvutsrqponmlkjihgfedcb",
     "abcdefghijklmnopqrstuvwxyzyxwvutsrqponmlkjihgfedcb"},
    {// 3 CANDLE (first variety)
     "mmmmmaaaaammmmmaaaaaabcdefgabcdefg", "mmmmmaaaaammmmmaaaaaabcdefgabcdefg", "mmmmmaaaaammmmmaaaaaabcdefgabcdefg"},
    {// 4 FAST STROBE
     "mamamamamama", "mamamamamama", "mamamamamama"},
    {// 5 GENTLE PULSE 1
     "jklmnopqrstuvwxyzyxwvutsrqponmlkj", "jklmnopqrstuvwxyzyxwvutsrqponmlkj", "jklmnopqrstuvwxyzyxwvutsrqponmlkj"},
    {// 6 FLICKER (second variety)
     "nmonqnmomnmomomno", "nmonqnmomnmomomno", "nmonqnmomnmomomno"},
    {// 7 CANDLE (second variety)
     "mmmaaaabcdefgmmmmaaaammmaamm", "mmmaaaabcdefgmmmmaaaammmaamm", "mmmaaaabcdefgmmmmaaaammmaamm"},
    {// 8 CANDLE (third variety)
     "mmmaaammmaaammmabcdefaaaammmmabcdefmmmaaaa", "mmmaaammmaaammmabcdefaaaammmmabcdefmmmaaaa", "mmmaaammmaaammmabcdefaaaammmmabcdefmmmaaaa"},
    {// 9 SLOW STROBE (fourth variety)
     "aaaaaaaazzzzzzzz", "aaaaaaaazzzzzzzz", "aaaaaaaazzzzzzzz"},
    {// 10 FLUORESCENT FLICKER
     "mmamammmmammamamaaamammma", "mmamammmmammamamaaamammma", "mmamammmmammamamaaamammma"},
    {// 11 SLOW PULSE NOT FADE TO BLACK
     "abcdefghijklmnopqrrqponmlkjihgfedcba", "abcdefghijklmnopqrrqponmlkjihgfedcba", "abcdefghijklmnopqrrqponmlkjihgfedcba"},
    {// 12 FAST PULSE FOR JEREMY
     "mkigegik", "mkigegik", "mkigegik"},
    {// 13 Test Blending
     "abcdefghijklmqrstuvwxyz", "zyxwvutsrqmlkjihgfedcba", "aammbbzzccllcckkffyyggp"},
    {// 14
     "", "", ""},
    {// 15
     "", "", ""},
    {// 16
     "", "", ""},
    {// 17
     "", "", ""},
    {// 18
     "", "", ""},
    {// 19
     "", "", ""},
    {// 20
     "", "", ""},
    {// 21
     "", "", ""},
    {// 22
     "", "", ""},
    {// 23
     "", "", ""},
    {// 24
     "", "", ""},
    {// 25
     "", "", ""},
    {// 26
     "", "", ""},
    {// 27
     "", "", ""},
    {// 28
     "", "", ""},
    {// 29
     "", "", ""},
    {// 30
     "", "", ""},
    {// 31
     "", "", ""}};

void *precachedKyle = 0;
void scriptrunner_run(gentity_t *self);

/*QUAKED worldspawn (0 0 0) ?

Every map should have exactly one worldspawn.
"music"		music wav file
"gravity"	800 is default gravity
"message"	Text to print during connection process

BSP Options
"gridsize"     size of lighting grid to "X Y Z". default="64 64 128"
"ambient"      scale of global light (from _color)
"fog"          shader name of the global fog texture - must include the full path, such as "textures/rj/fog1"
"distancecull" value for vis for the maximum viewing distance
"chopsize"     value for bsp on the maximum polygon / portal size
"ls_Xr"	override lightstyle X with this pattern for Red.
"ls_Xg"	green (valid patterns are "a-z")
"ls_Xb"	blue (a is OFF, z is ON)

"fogstart"		override fog start distance and force linear
"radarrange" for Siege/Vehicle radar - default range is 2500
*/
void EWebPrecache(void); // g_items.c
float g_cullDistance;
void SP_worldspawn(void) {
    char *text, temp[32];
    int i;
    int lengthRed, lengthBlue, lengthGreen;

    // I want to "cull" entities out of net sends to clients to reduce
    // net traffic on our larger open maps -rww
    G_SpawnFloat("distanceCull", "6000.0", &g_cullDistance);
    trap->SetServerCull(g_cullDistance);

    G_SpawnString("classname", "", &text);
    if (Q_stricmp(text, "worldspawn")) {
        trap->Error(ERR_DROP, "SP_worldspawn: The first entity isn't 'worldspawn'");
    }

    for (i = 0; i < level.numSpawnVars; i++) {
        if (Q_stricmp("spawnscript", level.spawnVars[i][0]) ==
            0) { // ONly let them set spawnscript, we don't want them setting an angle or something on the world.
            BG_ParseField(fields, ARRAY_LEN(fields), level.spawnVars[i][0], level.spawnVars[i][1], (byte *)&g_entities[ENTITYNUM_WORLD]);
        }
    }
    // The server will precache the standard model and animations, so that there is no hit
    // when the first client connnects.
    if (!BGPAFtextLoaded) {
        BG_ParseAnimationFile("models/players/_humanoid/animation.cfg", bgHumanoidAnimations, qtrue);
    }

    if (!precachedKyle) {
        int defSkin;

        trap->G2API_InitGhoul2Model(&precachedKyle, "models/players/kyle/model.glm", 0, 0, -20, 0, 0);

        if (precachedKyle) {
            defSkin = trap->R_RegisterSkin("models/players/kyle/model_default.skin");
            trap->G2API_SetSkin(precachedKyle, 0, defSkin, defSkin);
        }
    }

    if (!g2SaberInstance) {
        trap->G2API_InitGhoul2Model(&g2SaberInstance, "models/weapons2/saber/saber_w.glm", 0, 0, -20, 0, 0);

        if (g2SaberInstance) {
            // indicate we will be bolted to model 0 (ie the player) on bolt 0 (always the right hand) when we get copied
            trap->G2API_SetBoltInfo(g2SaberInstance, 0, 0);
            // now set up the gun bolt on it
            trap->G2API_AddBolt(g2SaberInstance, 0, "*blade1");
        }
    }

    if (level.gametype == GT_SIEGE) { // a tad bit of a hack, but..
        EWebPrecache();
    }

    // make some data visible to connecting client
    trap->SetConfigstring(CS_GAME_VERSION, GAME_VERSION);

    trap->SetConfigstring(CS_LEVEL_START_TIME, va("%i", level.startTime));

    G_SpawnString("music", "", &text);
    trap->SetConfigstring(CS_MUSIC, text);

    G_SpawnString("message", "", &text);
    trap->SetConfigstring(CS_MESSAGE, text); // map specific message

    trap->SetConfigstring(CS_MOTD, g_motd.string); // message of the day

    G_SpawnString("gravity", "800", &text);
    trap->Cvar_Set("g_gravity", text);

    G_SpawnString("enableBreath", "0", &text);
    trap->Cvar_Set("g_enableBreath", text);

    G_SpawnString("soundSet", "default", &text);
    trap->SetConfigstring(CS_GLOBAL_AMBIENT_SET, text);

    g_entities[ENTITYNUM_WORLD].s.number = ENTITYNUM_WORLD;
    g_entities[ENTITYNUM_WORLD].classname = "worldspawn";

    // see if we want a warmup time
    trap->SetConfigstring(CS_WARMUP, "");
    if (g_restarted.integer) {
        trap->Cvar_Set("g_restarted", "0");
        level.warmupTime = 0;
    }

    // Raz: Fix warmup
    else if (g_doWarmup.integer && level.gametype != GT_DUEL && level.gametype != GT_POWERDUEL && level.gametype != GT_SIEGE) { // Turn it on
        level.warmupTime = -1;
        trap->SetConfigstring(CS_WARMUP, va("%i", level.warmupTime));
        G_LogPrintf(level.log.console, "Warmup:\n");
    }

    trap->SetConfigstring(CS_LIGHT_STYLES + (LS_STYLES_START * 3) + 0, defaultStyles[0][0]);
    trap->SetConfigstring(CS_LIGHT_STYLES + (LS_STYLES_START * 3) + 1, defaultStyles[0][1]);
    trap->SetConfigstring(CS_LIGHT_STYLES + (LS_STYLES_START * 3) + 2, defaultStyles[0][2]);

    for (i = 1; i < LS_NUM_STYLES; i++) {
        Com_sprintf(temp, sizeof(temp), "ls_%dr", i);
        G_SpawnString(temp, defaultStyles[i][0], &text);
        lengthRed = strlen(text);
        trap->SetConfigstring(CS_LIGHT_STYLES + ((i + LS_STYLES_START) * 3) + 0, text);

        Com_sprintf(temp, sizeof(temp), "ls_%dg", i);
        G_SpawnString(temp, defaultStyles[i][1], &text);
        lengthGreen = strlen(text);
        trap->SetConfigstring(CS_LIGHT_STYLES + ((i + LS_STYLES_START) * 3) + 1, text);

        Com_sprintf(temp, sizeof(temp), "ls_%db", i);
        G_SpawnString(temp, defaultStyles[i][2], &text);
        lengthBlue = strlen(text);
        trap->SetConfigstring(CS_LIGHT_STYLES + ((i + LS_STYLES_START) * 3) + 2, text);

        if (lengthRed != lengthGreen || lengthGreen != lengthBlue) {
            Com_Error(ERR_DROP, "Style %d has inconsistent lengths: R %d, G %d, B %d", i, lengthRed, lengthGreen, lengthBlue);
        }
    }
}

// rww - Planning on having something here?
qboolean SP_bsp_worldspawn(void) { return qtrue; }

void G_PrecacheSoundsets(void) {
    gentity_t *ent = NULL;
    int i;
    int countedSets = 0;

    for (i = 0; i < MAX_GENTITIES; i++) {
        ent = &g_entities[i];

        if (ent->inuse && ent->soundSet && ent->soundSet[0]) {
            if (countedSets >= MAX_AMBIENT_SETS) {
                Com_Error(ERR_DROP, "MAX_AMBIENT_SETS was exceeded! (too many soundsets)\n");
            }

            ent->s.soundSetIndex = G_SoundSetIndex(ent->soundSet);
            countedSets++;
        }
    }
}

// Parses textual entity definitions out of an entstring and spawns gentities.
void G_SpawnEntitiesFromString(qboolean inSubBSP) {
    // allow calls to G_Spawn*()
    level.spawning = qtrue;
    level.numSpawnVars = 0;

    // the worldspawn is not an actual entity, but it still
    // has a "spawn" function to perform any global setup
    // needed by a level (setting configstrings or cvars, etc)
    if (!G_ParseSpawnVars(qfalse)) {
        trap->Error(ERR_DROP, "SpawnEntities: no entities");
    }

    if (!inSubBSP) {
        SP_worldspawn();
    } else {
        // Skip this guy if its worldspawn fails
        if (!SP_bsp_worldspawn()) {
            return;
        }
    }

    // parse ents
    while (G_ParseSpawnVars(inSubBSP)) {
        G_SpawnGEntityFromSpawnVars(inSubBSP);
    }

    if (g_entities[ENTITYNUM_WORLD].behaviorSet[BSET_SPAWN] &&
        g_entities[ENTITYNUM_WORLD].behaviorSet[BSET_SPAWN][0]) { // World has a spawn script, but we don't want the world in ICARUS and running scripts,
        // so make a scriptrunner and start it going.
        gentity_t *script_runner = G_Spawn();
        if (script_runner) {
            script_runner->behaviorSet[BSET_USE] = g_entities[ENTITYNUM_WORLD].behaviorSet[BSET_SPAWN];
            script_runner->count = 1;
            script_runner->think = scriptrunner_run;
            script_runner->nextthink = level.time + 100;

            if (script_runner->inuse) {
                trap->ICARUS_InitEnt((sharedEntity_t *)script_runner);
            }
        }
    }

    if (!inSubBSP) {
        level.spawning = qfalse; // any future calls to G_Spawn*() will be errors
    }

    G_PrecacheSoundsets();
}
