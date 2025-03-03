#pragma once

#include "qcommon/q_shared.h"
#include "game/g_local.h"

// Distance ratings
enum distance_e {
    DIST_MELEE,
    DIST_LONG,
};

// Attack types
enum attack_e {
    ATTACK_MELEE,
    ATTACK_RANGE,
};

enum squadState_e {
    SQUAD_IDLE,            // No target found, waiting
    SQUAD_STAND_AND_SHOOT, // Standing in position and shoot (no cover)
    SQUAD_RETREAT,         // Running away from combat
    SQUAD_COVER,           // Under protective cover
    SQUAD_TRANSITION,      // Moving between points, not firing
    SQUAD_POINT,           // On point, laying down suppressive fire
    SQUAD_SCOUT,           // Poking out to draw enemy
    NUM_SQUAD_STATES,
};

// sigh... had to move in here for groupInfo
enum rank_e { RANK_CIVILIAN, RANK_CREWMAN, RANK_ENSIGN, RANK_LT_JG, RANK_LT, RANK_LT_COMM, RANK_COMMANDER, RANK_CAPTAIN };

qboolean NPC_CheckPlayerTeamStealth(void);

// AI_GRENADIER
void NPC_BSGrenadier_Default(void);

// AI_SNIPER
void NPC_BSSniper_Default(void);

// AI_STORMTROOPER
void NPC_BSST_Investigate(void);
void NPC_BSST_Default(void);
void NPC_BSST_Sleep(void);

// AI_JEDI
void NPC_BSJedi_Investigate(void);
void NPC_BSJedi_Default(void);
void NPC_BSJedi_FollowLeader(void);

// AI_DROID
void NPC_BSDroid_Default(void);

// AI_ImperialProbe
void NPC_BSImperialProbe_Default(void);

// AI_atst
void NPC_BSATST_Default(void);

void NPC_BSInterrogator_Default(void);

// AI Mark 1
void NPC_BSMark1_Default(void);

// AI Mark 2
void NPC_BSMark2_Default(void);

void NPC_BSMineMonster_Default(void);
void NPC_BSHowler_Default(void);
void NPC_BSRancor_Default(void);

// Utilities
// Group AI
#define MAX_FRAME_GROUPS 32
// !!!!!!!!!! LOADSAVE-affecting structure !!!!!!!!!!
typedef struct AIGroupMember_s {
    int number;
    int waypoint;
    int pathCostToEnemy;
    int closestBuddy;
} AIGroupMember_t;

#define MAX_GROUP_MEMBERS 32
// !!!!!!!!!! LOADSAVE-affecting structure !!!!!!!!!!
typedef struct AIGroupInfo_s {
    int numGroup;
    qboolean processed;
    npcteam_e team;
    gentity_t *enemy;
    int enemyWP;
    int speechDebounceTime;
    int lastClearShotTime;
    int lastSeenEnemyTime;
    int morale;
    int moraleAdjust;
    int moraleDebounce;
    int memberValidateTime;
    int activeMemberNum;
    gentity_t *commander;
    vector3 enemyLastSeenPos;
    int numState[NUM_SQUAD_STATES];
    AIGroupMember_t member[MAX_GROUP_MEMBERS];
} AIGroupInfo_t;

int AI_GetGroupSize(vector3 *origin, int radius, team_e playerTeam, gentity_t *avoid);
int AI_GetGroupSize2(gentity_t *ent, int radius);

void AI_GetGroup(gentity_t *self);

qboolean AI_CheckEnemyCollision(gentity_t *ent, qboolean takeEnemy);
gentity_t *AI_DistributeAttack(gentity_t *attacker, gentity_t *enemy, team_e team, int threshold);
