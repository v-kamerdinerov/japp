#pragma once

// Copyright (C) 1999-2000 Id Software, Inc.
//
// bg_public.h -- definitions shared by both the server game and client game modules

// because games can change separately from the main system version, we need a
// second version that must match between game and cgame

#include "game/bg_weapons.h"
#include "game/anims.h"
#include "game/bg_animTable.h"
#include "game/bg_vehicles.h"

// these two defs are shared now because we do clientside ent parsing
#define MAX_SPAWN_VARS 64
#define MAX_SPAWN_VARS_CHARS 4096

#define GAME_VERSION "basejka-1"

#define DEFAULT_SABER "Kyle"
#define DEFAULT_SABER_STAFF "dual_1"
#define DEFAULT_SABER_MODEL "models/weapons2/saber/saber_w.glm"
#define DEFAULT_MODEL "kyle"
#define DEFAULT_MODEL_FEMALE "jan"

#define DEFAULT_REDTEAM_NAME "Empire"
#define DEFAULT_BLUETEAM_NAME "Rebellion"

#define STEPSIZE 18

#define DEFAULT_FORCEPOWERS "7-1-000000000000000000"
#define DEFAULT_FORCEPOWERS_LEN (22) // numPowers + rank + side + separators
//"rank-side-heal.lev.speed.push.pull.tele.grip.lightning.rage.protect.absorb.teamheal.teamforce.drain.see"

#define DEFAULT_GRAVITY 800
#define GIB_HEALTH -40
#define ARMOR_PROTECTION 0.50f       // Shields only stop 50% of armor-piercing dmg
#define ARMOR_REDUCTION_FACTOR 0.50f // Certain damage doesn't take off armor as efficiently

#define JUMP_VELOCITY 225 // 270

#define MAX_ITEMS 256

#define RANK_TIED_FLAG 0x4000

#define ITEM_RADIUS 15 // item sizes are needed for client side pickup detection

#define SCORE_NOT_PRESENT -9999 // for the CS_SCORES[12] when only one player is present

#define VOTE_TIME 30000 // 30 seconds before vote times out
#define VOTE_EXECUTE_TIME 3000

#define DEFAULT_MINS_2 -24
#define DEFAULT_MAXS_2 40
#define CROUCH_MAXS_2 16
#define STANDARD_VIEWHEIGHT_OFFSET -4

#define MINS_Z -24
#define DEFAULT_VIEWHEIGHT (DEFAULT_MAXS_2 + STANDARD_VIEWHEIGHT_OFFSET) // 26
#define CROUCH_VIEWHEIGHT (CROUCH_MAXS_2 + STANDARD_VIEWHEIGHT_OFFSET)   // 12
#define DEAD_VIEWHEIGHT -16

// Raz: MAX_CLIENT_SCORE_SEND
#define MAX_CLIENT_SCORE_SEND 20

//
// config strings are a general means of communicating variable length strings
// from the server to all connected clients.
//

// CS_SERVERINFO and CS_SYSTEMINFO are defined in q_shared.h
#define CS_MUSIC 2
#define CS_MESSAGE 3 // from the map worldspawn's message field
#define CS_MOTD 4    // g_motd string for server message of the day
#define CS_WARMUP 5  // server time when the match will be restarted
#define CS_SCORES1 6
#define CS_SCORES2 7
#define CS_VOTE_TIME 8
#define CS_VOTE_STRING 9
#define CS_VOTE_YES 10
#define CS_VOTE_NO 11

// Raz: Unused now
#define CS_TEAMVOTE_TIME 12
#define CS_TEAMVOTE_STRING 14
#define CS_TEAMVOTE_YES 16
#define CS_TEAMVOTE_NO 18

#define CS_GAME_VERSION 20
#define CS_LEVEL_START_TIME 21 // so the timer only shows the current level
#define CS_INTERMISSION 22     // when 1, fraglimit/timelimit has been hit and intermission will start in a second or two
#define CS_FLAGSTATUS 23       // string indicating flag status in CTF
#define CS_SHADERSTATE 24
#define CS_BOTINFO 25
#define CS_LEGACY_FIXES 26
#define CS_ITEMS 27 // string of 0's and 1's that tell which items are present

#define CS_CLIENT_JEDIMASTER 28  // current jedi master
#define CS_CLIENT_DUELWINNER 29  // current duel round winner - needed for printing at top of scoreboard
#define CS_CLIENT_DUELISTS 30    // client numbers for both current duelists. Needed for a number of client-side things.
#define CS_CLIENT_DUELHEALTHS 31 // nmckenzie: DUEL_HEALTH.  Hopefully adding this cs is safe and good?
#define CS_GLOBAL_AMBIENT_SET 32

#define CS_AMBIENT_SET 37

#define CS_SIEGE_STATE (CS_AMBIENT_SET + MAX_AMBIENT_SETS)
#define CS_SIEGE_OBJECTIVES (CS_SIEGE_STATE + 1)
#define CS_SIEGE_TIMEOVERRIDE (CS_SIEGE_OBJECTIVES + 1)
#define CS_SIEGE_WINTEAM (CS_SIEGE_TIMEOVERRIDE + 1)
#define CS_SIEGE_ICONS (CS_SIEGE_WINTEAM + 1)

#define CS_MODELS (CS_SIEGE_ICONS + 1)
#define CS_SKYBOXORG (CS_MODELS + MAX_MODELS) // rww - skybox info
#define CS_SOUNDS (CS_SKYBOXORG + 1)
#define CS_ICONS (CS_SOUNDS + MAX_SOUNDS)
#define CS_PLAYERS (CS_ICONS + MAX_ICONS)
/*
Ghoul2 Insert Start
*/
#define CS_G2BONES (CS_PLAYERS + MAX_CLIENTS)
// rww - used to be CS_CHARSKINS, but I have eliminated the need for that.
/*
Ghoul2 Insert End
*/
#define CS_LOCATIONS (CS_G2BONES + MAX_G2BONES)
#define CS_PARTICLES (CS_LOCATIONS + MAX_LOCATIONS)
#define CS_EFFECTS (CS_PARTICLES + MAX_LOCATIONS)
#define CS_LIGHT_STYLES (CS_EFFECTS + MAX_FX)

// rwwRMG - added:
#define CS_TERRAINS (CS_LIGHT_STYLES + (MAX_LIGHT_STYLES * 3))
#define CS_BSP_MODELS (CS_TERRAINS + MAX_TERRAINS)

#define CS_MAX (CS_BSP_MODELS + MAX_SUB_BSP)

#if (CS_MAX) > MAX_CONFIGSTRINGS
#error overflow: (CS_MAX) > MAX_CONFIGSTRINGS
#endif

enum legacyFixes_e {
    LEGACYFIX_SABERMOVEDATA = 0,
    /*
    m    m                        ""#      "             m                    m
    #    #  mmm   m   m             #    mmm     mmm   mm#mm   mmm   m mm     #
    #mmmm# #"  #  "m m"             #      #    #   "    #    #"  #  #"  #    #
    #    # #""""   #m#              #      #     """m    #    #""""  #   #    "
    #    # "#mm"   "#      #        "mm  mm#mm  "mmm"    "mm  "#mm"  #   #    #
                   m"     "
                  ""
    Forks of OpenJK should NOT add to or modify the legacy fixes values
    Removal, replacement or adding of new flags might lead to incompatibilities
    Forks should define their own configstring or serverinfo cvar instead of modifying this
    */
};

enum g2ModelParts_t {
    G2_MODELPART_HEAD = 10,
    G2_MODELPART_WAIST,
    G2_MODELPART_LARM,
    G2_MODELPART_RARM,
    G2_MODELPART_RHAND,
    G2_MODELPART_LLEG,
    G2_MODELPART_RLEG
};

#define G2_MODEL_PART 50

#define BG_NUM_TOGGLEABLE_SURFACES 31

#define MAX_CUSTOM_SIEGE_SOUNDS 30

extern const char *bg_customSiegeSoundNames[MAX_CUSTOM_SIEGE_SOUNDS];

extern const char *bgToggleableSurfaces[BG_NUM_TOGGLEABLE_SURFACES];
extern const int bgToggleableSurfaceDebris[BG_NUM_TOGGLEABLE_SURFACES];

enum forceHandAnims_e {
    HANDEXTEND_NONE = 0,
    HANDEXTEND_FORCEPUSH,
    HANDEXTEND_FORCEPULL,
    HANDEXTEND_FORCE_HOLD,
    HANDEXTEND_SABERPULL,
    HANDEXTEND_CHOKE, // use handextend priorities to choke someone being gripped
    HANDEXTEND_WEAPONREADY,
    HANDEXTEND_DODGE,
    HANDEXTEND_KNOCKDOWN,
    HANDEXTEND_DUELCHALLENGE,
    HANDEXTEND_TAUNT,

    HANDEXTEND_PRETHROW,
    HANDEXTEND_POSTTHROW,
    HANDEXTEND_PRETHROWN,
    HANDEXTEND_POSTTHROWN,

    HANDEXTEND_DRAGGING,

    HANDEXTEND_JEDITAUNT,
};

enum brokenLimb_e { BROKENLIMB_NONE = 0, BROKENLIMB_LARM, BROKENLIMB_RARM, NUM_BROKENLIMBS };

// for supplier class items
#define TOSS_DEBOUNCE_TIME 5000

enum gametype_e {
    GT_FFA = 0,       // free for all
    GT_HOLOCRON,      // holocron ffa
    GT_JEDIMASTER,    // jedi master
    GT_DUEL,          // one on one tournament
    GT_POWERDUEL,     // two on one tournament
    GT_SINGLE_PLAYER, // single player ffa
    //-- team games go after this --
    GT_TEAM,  // team deathmatch
    GT_SIEGE, // siege
    GT_CTF,   // capture the flag
    GT_CTY,   // capture the ysalimiri
    GT_MAX_GAME_TYPE
};

// gametype bits
#define GTB_NONE (0x0000u)          // invalid
#define GTB_FFA (0x0001u)           // free for all
#define GTB_HOLOCRON (0x0002u)      // holocron ffa
#define GTB_JEDIMASTER (0x0004u)    // jedi master
#define GTB_DUEL (0x0008u)          // one on one tournament
#define GTB_POWERDUEL (0x0010u)     // two on one tournament
#define GTB_SINGLE_PLAYER (0x0020u) // single player ffa
#define GTB_NOTTEAM (0x003Fu)       // **SPECIAL: All of the above gametypes, i.e. not team-based
#define GTB_TEAM (0x0040u)          // team deathmatch
#define GTB_SIEGE (0x0080u)         // siege
#define GTB_NOTFLAG (0x00FFu)       // **SPECIAL: All of the above gametypes, i.e. not flag-related
#define GTB_CTF (0x0100u)           // capture the flag
#define GTB_CTY (0x0200u)           // capture the ysalimiri
#define GTB_ALL (0x03FFu)           // all

enum gender_e { GENDER_MALE, GENDER_FEMALE, GENDER_NEUTER };

extern const int forcePowerSorted[NUM_FORCE_POWERS];

enum saberLock_e { SABERLOCK_TOP, SABERLOCK_SIDE, SABERLOCK_LOCK, SABERLOCK_BREAK, SABERLOCK_SUPERBREAK, SABERLOCK_WIN, SABERLOCK_LOSE };

enum directions_e { DIR_RIGHT = 0, DIR_LEFT, DIR_FRONT, DIR_BACK, NUM_DIRECTIONS };

// The pmove code takes a player_state_t and a usercmd_t and generates a new player_state_t and some other output data.
//	Used for local prediction on the client game and true movement on the server game.

#pragma pack(push, 1)
typedef struct animation_s {
    unsigned short firstFrame;
    unsigned short numFrames;
    short frameLerp; // msec between frames
    // initialLerp is abs(frameLerp)
    signed char loopFrames; // 0 to numFrames
} animation_t;
#pragma pack(pop)

extern qboolean BGPAFtextLoaded;
extern animation_t bgHumanoidAnimations[MAX_TOTALANIMATIONS];

#define MAX_ANIM_FILES 16
#define MAX_ANIM_EVENTS 300

enum footstepType_e { FOOTSTEP_R, FOOTSTEP_L, FOOTSTEP_HEAVY_R, FOOTSTEP_HEAVY_L, NUM_FOOTSTEP_TYPES };

extern stringID_table_t animEventTypeTable[MAX_ANIM_EVENTS + 1];
extern stringID_table_t footstepTypeTable[NUM_FOOTSTEP_TYPES + 1];

// size of Anim eventData array...
#define MAX_RANDOM_ANIM_SOUNDS 4
#define AED_ARRAY_SIZE (MAX_RANDOM_ANIM_SOUNDS + 3)
// indices for AEV_SOUND data
#define AED_SOUNDINDEX_START 0
#define AED_SOUNDINDEX_END (MAX_RANDOM_ANIM_SOUNDS - 1)
#define AED_SOUND_NUMRANDOMSNDS (MAX_RANDOM_ANIM_SOUNDS)
#define AED_SOUND_PROBABILITY (MAX_RANDOM_ANIM_SOUNDS + 1)
// indices for AEV_SOUNDCHAN data
#define AED_SOUNDCHANNEL (MAX_RANDOM_ANIM_SOUNDS + 2)
// indices for AEV_FOOTSTEP data
#define AED_FOOTSTEP_TYPE 0
#define AED_FOOTSTEP_PROBABILITY 1
// indices for AEV_EFFECT data
#define AED_EFFECTINDEX 0
#define AED_BOLTINDEX 1
#define AED_EFFECT_PROBABILITY 2
#define AED_MODELINDEX 3
// indices for AEV_FIRE data
#define AED_FIRE_ALT 0
#define AED_FIRE_PROBABILITY 1
// indices for AEV_MOVE data
#define AED_MOVE_FWD 0
#define AED_MOVE_RT 1
#define AED_MOVE_UP 2
// indices for AEV_SABER_SWING data
#define AED_SABER_SWING_SABERNUM 0
#define AED_SABER_SWING_TYPE 1
#define AED_SABER_SWING_PROBABILITY 2
// indices for AEV_SABER_SPIN data
#define AED_SABER_SPIN_SABERNUM 0
#define AED_SABER_SPIN_TYPE 1 // 0 = saberspinoff, 1 = saberspin, 2-4 = saberspin1-saberspin3
#define AED_SABER_SPIN_PROBABILITY 2

enum animEventType_e { // NOTENOTE:  Be sure to update animEventTypeTable and ParseAnimationEvtBlock(...) if you change this enum list!
    AEV_NONE,
    AEV_SOUND,       //# animID AEV_SOUND framenum soundpath randomlow randomhi chancetoplay
    AEV_FOOTSTEP,    //# animID AEV_FOOTSTEP framenum footstepType chancetoplay
    AEV_EFFECT,      //# animID AEV_EFFECT framenum effectpath boltName chancetoplay
    AEV_FIRE,        //# animID AEV_FIRE framenum altfire chancetofire
    AEV_MOVE,        //# animID AEV_MOVE framenum forwardpush rightpush uppush
    AEV_SOUNDCHAN,   //# animID AEV_SOUNDCHAN framenum CHANNEL soundpath randomlow randomhi chancetoplay
    AEV_SABER_SWING, //# animID AEV_SABER_SWING framenum CHANNEL randomlow randomhi chancetoplay
    AEV_SABER_SPIN,  //# animID AEV_SABER_SPIN framenum CHANNEL chancetoplay
    AEV_NUM_AEV
};

typedef struct animevent_s {
    animEventType_e eventType;
    unsigned short keyFrame;                // Frame to play event on
    signed short eventData[AED_ARRAY_SIZE]; // Unique IDs, can be soundIndex of sound file to play OR effect index or footstep type, etc.
    char *stringData; // we allow storage of one string, temporarily (in case we have to look up an index later, then make sure to set stringData to NULL so we
                      // only do the look-up once)
} animevent_t;

typedef struct {
    char filename[MAX_QPATH];
    animation_t *anims;
    //	animsounds_t	torsoAnimSnds[MAX_ANIM_SOUNDS];
    //	animsounds_t	legsAnimSnds[MAX_ANIM_SOUNDS];
    //	qboolean		soundsCached;
} bgLoadedAnim_t;

typedef struct {
    char filename[MAX_QPATH];
    animevent_t torsoAnimEvents[MAX_ANIM_EVENTS];
    animevent_t legsAnimEvents[MAX_ANIM_EVENTS];
    qboolean eventsParsed;
} bgLoadedEvents_t;

extern bgLoadedAnim_t bgAllAnims[MAX_ANIM_FILES];

// In SP this is shared in with the anim stuff, and humanoid anim sets can be loaded
// multiple times just for the sake of sounds being different. We probably wouldn't
// care normally but since we're working in VMs we have to do everything possible to
// cut memory cost.
// On the bright side this also means we're cutting a rather large size out of
// required game-side memory.
#ifndef PROJECT_GAME
extern bgLoadedEvents_t bgAllEvents[MAX_ANIM_FILES];
extern int bgNumAnimEvents;
#endif

enum pmtype_e {
    PM_NORMAL,        // can accelerate and turn
    PM_JETPACK,       // special jetpack movement
    PM_FLOAT,         // float with no gravity in general direction of velocity (intended for gripping)
    PM_NOCLIP,        // noclip movement
    PM_SPECTATOR,     // still run into walls
    PM_DEAD,          // no acceleration or turning, but free falling
    PM_FREEZE,        // stuck in place with no control
    PM_INTERMISSION,  // no movement or status bar
    PM_SPINTERMISSION // no movement or status bar
};

enum weaponstate_e {
    WEAPON_READY,
    WEAPON_RAISING,
    WEAPON_DROPPING,
    WEAPON_FIRING,
    WEAPON_CHARGING,
    WEAPON_CHARGING_ALT,
    WEAPON_IDLE, // lowered		// NOTENOTE Added with saber
};

enum forceMasteries_e {
    FORCE_MASTERY_UNINITIATED,
    FORCE_MASTERY_INITIATE,
    FORCE_MASTERY_PADAWAN,
    FORCE_MASTERY_JEDI,
    FORCE_MASTERY_JEDI_GUARDIAN,
    FORCE_MASTERY_JEDI_ADEPT,
    FORCE_MASTERY_JEDI_KNIGHT,
    FORCE_MASTERY_JEDI_MASTER,
    NUM_FORCE_MASTERY_LEVELS
};

extern const char *forceMasteryLevels[NUM_FORCE_MASTERY_LEVELS];
extern const int forceMasteryPoints[NUM_FORCE_MASTERY_LEVELS];

extern int bgForcePowerCost[NUM_FORCE_POWERS][NUM_FORCE_POWER_LEVELS];

// pmove->pm_flags
#define PMF_DUCKED (0x00000001u)
#define PMF_JUMP_HELD (0x00000002u)
#define PMF_ROLLING (0x00000004u)
#define PMF_BACKWARDS_JUMP (0x00000008u) // go into backwards land
#define PMF_BACKWARDS_RUN (0x00000010u)  // coast down to backwards run
#define PMF_TIME_LAND (0x00000020u)      // pm_time is time before rejump
#define PMF_TIME_KNOCKBACK (0x00000040u) // pm_time is an air-accelerate only time
#define PMF_FIX_MINS (0x00000080u)       // mins have been brought up, keep tracing down to fix them
#define PMF_TIME_WATERJUMP (0x00000100u) // pm_time is waterjump
#define PMF_RESPAWNED (0x00000200u)      // clear after attack and jump buttons come up
#define PMF_USE_ITEM_HELD (0x00000400u)
#define PMF_UPDATE_ANIM (0x00000800u)   // The server updated the animation, the pmove should set the ghoul2 anim to match.
#define PMF_FOLLOW (0x00001000u)        // spectate following another player
#define PMF_SCOREBOARD (0x00002000u)    // spectate as a scoreboard
#define PMF_STUCK_TO_WALL (0x00004000u) // grabbing a wall
#define PMF_GRAPPLE_PULL (0x00008000u)

#define PMF_ALL_TIMES (PMF_TIME_WATERJUMP | PMF_TIME_LAND | PMF_TIME_KNOCKBACK)

#define MAXTOUCH 32

typedef struct bgEntity_s {
    entityState_t s;
    playerState_t *playerState;
    struct Vehicle_s *m_pVehicle; // vehicle data
    void *ghoul2;                 // g2 instance
    int localAnimIndex;           // index locally (game/cgame) to anim data for this skel
    vector3 modelScale;           // needed for g2 collision

    // Data type(s) must directly correspond to the head of the gentity and centity structures
} bgEntity_t;

typedef struct pmove_s {
    // state (in / out)
    playerState_t *ps;

    // rww - shared ghoul2 stuff (not actually the same data, but hey)
    void *ghoul2;
    int g2Bolts_LFoot;
    int g2Bolts_RFoot;
    vector3 modelScale;

    // hacky bool so we know if we're dealing with a nonhumanoid (which is probably a rockettrooper)
    qboolean nonHumanoid;

    // command (in)
    usercmd_t cmd;
    uint32_t tracemask;   // collide against these types of surfaces
    qboolean noFootsteps; // if the game is setup for no footsteps by the server
    qboolean gauntletHit; // true if a gauntlet attack would actually hit something

    int framecount;

    // results (out)
    int numtouch;
    int touchents[MAXTOUCH];

    int useEvent;

    vector3 mins, maxs; // bounding box size

    int watertype;
    int waterlevel;

    int gametype;

    int debugMelee;
    int stepSlideFix;
    int noSpecMove;

    animation_t *animations;

    float xyspeed;

    // for fixed msec Pmove
    int pmove_fixed;
    int pmove_float;
    int pmove_msec;

    // callbacks to test the world
    // these will be different functions during game and cgame
    void (*trace)(trace_t *results, const vector3 *start, const vector3 *mins, const vector3 *maxs, const vector3 *end, int passEntityNum, int contentMask);
    uint32_t (*pointcontents)(const vector3 *point, int passEntityNum);

    int checkDuelLoss;

    // rww - bg entitystate access method
    bgEntity_t *baseEnt; // base address of the entity array (g_entities or cg_entities)
    int entSize;         // size of the struct (gentity_t or centity_t) so things can be dynamic

    int overbounce;
    float gripSpeedScale;

#ifdef _DEBUG
    int debugLevel; // if set, diagnostic output will be printed
#endif
} pmove_t;

extern pmove_t *pm;

#define SETANIM_TORSO 1
#define SETANIM_LEGS 2
#define SETANIM_BOTH SETANIM_TORSO | SETANIM_LEGS // 3

#define SETANIM_FLAG_NORMAL 0   // Only set if timer is 0
#define SETANIM_FLAG_OVERRIDE 1 // Override previous
#define SETANIM_FLAG_HOLD 2     // Set the new timer
#define SETANIM_FLAG_RESTART 4  // Allow restarting the anim if playing the same one (weapon fires)
#define SETANIM_FLAG_HOLDLESS 8 // Set the new timer
// new flag to be able to restart and override without overriding the same animation.
// this is useful for situations where you want to have the animation timer pace
// a repeating animation
#define SETANIM_FLAG_PACE 16 // acts like a SETANIM_FLAG_RESTART but only restarts if the animation is over.

// player_state->stats[] indexes
// NOTE: may not have more than 16
enum statIndex_e {
    STAT_HEALTH,
    STAT_HOLDABLE_ITEM,
    STAT_HOLDABLE_ITEMS,
    STAT_PERSISTANT_POWERUP,
    // MAKE SURE STAT_WEAPONS REMAINS 4!!!!
    // There is a hardcoded reference in msg.cpp to send it in 32 bits -rww
    STAT_WEAPONS = 4, // 16 bit fields
    STAT_ARMOR,
    STAT_DEAD_YAW,      // look this direction when dead (FIXME: get rid of?)
    STAT_CLIENTS_READY, // bit mask of clients wishing to exit the intermission (FIXME: configstring?)
    STAT_MAX_HEALTH,    // health / armor limit, changable by handicap
    STAT_JUMPTIME
};

// player_state->persistant[] indexes
// these fields are the only part of player_state that isn't
// cleared on respawn
// NOTE: may not have more than 16
enum persEnum_e {
    PERS_SCORE,          // !!! MUST NOT CHANGE, SERVER AND GAME BOTH REFERENCE !!!
    PERS_HITS,           // total points damage inflicted so damage beeps can sound on change
    PERS_RANK,           // player rank or team rank
    PERS_TEAM,           // player team
    PERS_SPAWN_COUNT,    // incremented every respawn
    PERS_PLAYEREVENTS,   // 16 bits that can be flipped for events
    PERS_ATTACKER,       // clientnum of last damage inflicter
    PERS_ATTACKEE_ARMOR, // health/armor of last person we attacked
    PERS_KILLED,         // count of the number of times you died
    // player awards tracking
    PERS_IMPRESSIVE_COUNT,    // two railgun hits in a row
    PERS_EXCELLENT_COUNT,     // two successive kills in a short amount of time
    PERS_DEFEND_COUNT,        // defend awards
    PERS_ASSIST_COUNT,        // assist awards
    PERS_GAUNTLET_FRAG_COUNT, // kills with the guantlet
    PERS_CAPTURES             // captures
};

// entityState_t->eFlags
#define EF_G2ANIMATING (0x00000001u)     // perform g2 bone anims based on torsoAnim and legsAnim, works for ET_GENERAL -rww
#define EF_DEAD (0x00000002u)            // don't draw a foe marker over players with EF_DEAD
#define EF_RADAROBJECT (0x00000004u)     // display on team radar
#define EF_TELEPORT_BIT (0x00000008u)    // toggled every time the origin abruptly changes
#define EF_SHADER_ANIM (0x00000010u)     // Animating shader (by s.frame)
#define EF_PLAYER_EVENT (0x00000020u)    //
#define EF_RAG (0x00000040u)             // ragdoll him even if he's alive
#define EF_PERMANENT (0x00000080u)       // rww - I am claiming this. (for permanent entities)
#define EF_NODRAW (0x00000100u)          // may have an event, but no model (unspawned items)
#define EF_FIRING (0x00000200u)          // for lightning gun
#define EF_ALT_FIRING (0x00000400u)      // for alt-fires, mostly for lightning guns though
#define EF_JETPACK_ACTIVE (0x00000800u)  // jetpack is activated
#define EF_NOT_USED_1 (0x00001000u)      //
#define EF_TALK (0x00002000u)            // draw a talk balloon
#define EF_CONNECTION (0x00004000u)      // draw a connection trouble sprite
#define EF_ALT_DIM (0x00008000u)         // Player is in the alternate dimension
#define EF_GRAPPLE_SWING (0x00010000u)   // swinging on grapple hook
#define EF_NOT_USED_2 (0x00020000u)      //
#define EF_NOT_USED_3 (0x00040000u)      //
#define EF_BODYPUSH (0x00080000u)        // rww - claiming this for fullbody push effect
#define EF_DOUBLE_AMMO (0x00100000u)     // Hacky way to get around ammo max
#define EF_SEEKERDRONE (0x00200000u)     // show seeker drone floating around head
#define EF_MISSILE_STICK (0x00400000u)   // missiles that stick to the wall.
#define EF_ITEMPLACEHOLDER (0x00800000u) // item effect
#define EF_SOUNDTRACKER (0x01000000u)    // sound position needs to be updated in relation to another entity
#define EF_DROPPEDWEAPON (0x02000000u)   // it's a dropped weapon
#define EF_DISINTEGRATION (0x04000000u)  // being disintegrated by the disruptor
#define EF_INVULNERABLE (0x08000000u)    // just spawned in or whatever, so is protected
#define EF_CLIENTSMOOTH (0x10000000u)    // standard lerporigin smooth override on client
#define EF_JETPACK (0x20000000u)         // rww - wearing a jetpack
#define EF_JETPACK_FLAMING (0x40000000u) // rww - jetpack fire effect
#define EF_NOT_USED_4 (0x80000000u)      //

// These new EF2_??? flags were added for NPCs, they really should not be used often.
// NOTE: we only allow 10 of these!
#define EF2_HELD_BY_MONSTER (0x0001u)  // Being held by something, like a Rancor or a Wampa
#define EF2_USE_ALT_ANIM (0x0002u)     // For certain special runs/stands for creatures like the Rancor and Wampa whose runs/stands are conditional
#define EF2_ALERTED (0x0004u)          // For certain special anims, for Rancor: means you've had an enemy, so use the more alert stand
#define EF2_GENERIC_NPC_FLAG (0x0008u) // So far, used for Rancor...
#define EF2_FLYING (0x0010u)           // Flying FIXME: only used on NPCs doesn't *really* have to be passed over, does it?
#define EF2_HYPERSPACE                                                                                                                                         \
    (0x0020u) // Used to both start the hyperspace effect on the predicted client and to let the vehicle know it can now jump into hyperspace (after turning to
              // face the proper angle)
#define EF2_BRACKET_ENTITY (0x0040u) // Draw as bracketed
#define EF2_SHIP_DEATH (0x0080u)     // "died in ship" mode
#define EF2_NOT_USED_1 (0x0100u)     //
#define EF2_GRAPPLE_OUT (0x0200u)    // grapple hook is out

enum effectTypes_e {
    EFFECT_NONE = 0,
    EFFECT_SMOKE,
    EFFECT_EXPLOSION,
    EFFECT_EXPLOSION_PAS,
    EFFECT_SPARK_EXPLOSION,
    EFFECT_EXPLOSION_TRIPMINE,
    EFFECT_EXPLOSION_DETPACK,
    EFFECT_EXPLOSION_FLECHETTE,
    EFFECT_STUNHIT,
    EFFECT_EXPLOSION_DEMP2ALT,
    EFFECT_EXPLOSION_TURRET,
    EFFECT_SPARKS,
    EFFECT_WATER_SPLASH,
    EFFECT_ACID_SPLASH,
    EFFECT_LAVA_SPLASH,
    EFFECT_LANDING_MUD,
    EFFECT_LANDING_SAND,
    EFFECT_LANDING_DIRT,
    EFFECT_LANDING_SNOW,
    EFFECT_LANDING_GRAVEL,
    EFFECT_MAX
};

// NOTE: may not have more than 16
enum powerup_e {
    PW_NONE,

    PW_QUAD,
    PW_BATTLESUIT,
    PW_PULL,
    // PW_INVIS, //rww - removed
    // PW_REGEN, //rww - removed
    // PW_FLIGHT, //rww - removed

    PW_REDFLAG,
    PW_BLUEFLAG,
    PW_NEUTRALFLAG,

    PW_SHIELDHIT,

    // PW_SCOUT, //rww - removed
    // PW_GUARD, //rww - removed
    // PW_DOUBLER, //rww - removed
    // PW_AMMOREGEN, //rww - removed
    PW_SPEEDBURST,
    PW_DISINT_4,
    PW_SPEED,
    PW_CLOAKED,
    PW_FORCE_ENLIGHTENED_LIGHT,
    PW_FORCE_ENLIGHTENED_DARK,
    PW_FORCE_BOON,
    PW_YSALAMIRI,

    PW_NUM_POWERUPS

};

enum holdable_e {
    HI_NONE = 0,
    HI_SEEKER,
    HI_SHIELD,
    HI_MEDPAC,
    HI_MEDPAC_BIG,
    HI_BINOCULARS,
    HI_SENTRY_GUN,
    HI_JETPACK,
    HI_HEALTHDISP,
    HI_AMMODISP,
    HI_EWEB,
    HI_CLOAK,
    HI_NUM_HOLDABLE
};

enum ctfMsg_e {
    CTFMESSAGE_FRAGGED_FLAG_CARRIER,
    CTFMESSAGE_FLAG_RETURNED,
    CTFMESSAGE_PLAYER_RETURNED_FLAG,
    CTFMESSAGE_PLAYER_CAPTURED_FLAG,
    CTFMESSAGE_PLAYER_GOT_FLAG
};

// reward sounds (stored in ps->persistant[PERS_PLAYEREVENTS])
#define PLAYEREVENT_DENIEDREWARD (0x0001u)
#define PLAYEREVENT_GAUNTLETREWARD (0x0002u)
#define PLAYEREVENT_HOLYSHIT (0x0004u)

// entityState_t->event values
// entity events are for effects that take place reletive
// to an existing entities origin.  Very network efficient.

// two bits at the top of the entityState->event field
// will be incremented with each change in the event so
// that an identical event started twice in a row can
// be distinguished.  And off the value with ~EV_EVENT_BITS
// to retrieve the actual event number
#define EV_EVENT_BIT1 (0x00000100u)
#define EV_EVENT_BIT2 (0x00000200u)
#define EV_EVENT_BITS (EV_EVENT_BIT1 | EV_EVENT_BIT2)

#define EVENT_VALID_MSEC 300

enum pdSounds_e { PDSOUND_NONE, PDSOUND_PROTECTHIT, PDSOUND_PROTECT, PDSOUND_ABSORBHIT, PDSOUND_ABSORB, PDSOUND_FORCEJUMP, PDSOUND_FORCEGRIP };

enum entity_event_e {
    EV_NONE,

    EV_CLIENTJOIN,

    EV_FOOTSTEP,
    EV_FOOTSTEP_METAL,
    EV_FOOTSPLASH,
    EV_FOOTWADE,
    EV_SWIM,

    EV_STEP_4,
    EV_STEP_8,
    EV_STEP_12,
    EV_STEP_16,

    EV_FALL,

    EV_JUMP_PAD, // boing sound at origin, jump sound on player

    EV_GHOUL2_MARK, // create a projectile impact mark on something with a client-side g2 instance.

    EV_GLOBAL_DUEL,
    EV_PRIVATE_DUEL,

    EV_JUMP,
    EV_ROLL,
    EV_WATER_TOUCH, // foot touches
    EV_WATER_LEAVE, // foot leaves
    EV_WATER_UNDER, // head touches
    EV_WATER_CLEAR, // head leaves

    EV_ITEM_PICKUP,        // normal item pickups are predictable
    EV_GLOBAL_ITEM_PICKUP, // powerup / team sounds are broadcast to everyone

    EV_VEH_FIRE,

    EV_NOAMMO,
    EV_CHANGE_WEAPON,
    EV_FIRE_WEAPON,
    EV_ALT_FIRE,
    EV_SABER_ATTACK,
    EV_SABER_HIT,
    EV_SABER_BLOCK,
    EV_SABER_CLASHFLARE,
    EV_SABER_UNHOLSTER,
    EV_BECOME_JEDIMASTER,
    EV_DISRUPTOR_MAIN_SHOT,
    EV_DISRUPTOR_SNIPER_SHOT,
    EV_DISRUPTOR_SNIPER_MISS,
    EV_DISRUPTOR_HIT,
    EV_DISRUPTOR_ZOOMSOUND,

    EV_PREDEFSOUND,

    EV_TEAM_POWER,

    EV_SCREENSHAKE,

    EV_LOCALTIMER,

    EV_USE, // +Use key

    EV_USE_ITEM0,
    EV_USE_ITEM1,
    EV_USE_ITEM2,
    EV_USE_ITEM3,
    EV_USE_ITEM4,
    EV_USE_ITEM5,
    EV_USE_ITEM6,
    EV_USE_ITEM7,
    EV_USE_ITEM8,
    EV_USE_ITEM9,
    EV_USE_ITEM10,
    EV_USE_ITEM11,
    EV_USE_ITEM12,
    EV_USE_ITEM13,
    EV_USE_ITEM14,
    EV_USE_ITEM15,

    EV_ITEMUSEFAIL,

    EV_ITEM_RESPAWN,
    EV_ITEM_POP,
    EV_PLAYER_TELEPORT_IN,
    EV_PLAYER_TELEPORT_OUT,

    EV_GRENADE_BOUNCE, // eventParm will be the soundindex
    EV_MISSILE_STICK,  // eventParm will be the soundindex

    EV_PLAY_EFFECT,
    EV_PLAY_EFFECT_ID,
    EV_PLAY_PORTAL_EFFECT_ID,

    EV_PLAYDOORSOUND,
    EV_PLAYDOORLOOPSOUND,
    EV_BMODEL_SOUND,

    EV_MUTE_SOUND,
    EV_VOICECMD_SOUND,
    EV_GENERAL_SOUND,
    EV_GLOBAL_SOUND, // no attenuation
    EV_GLOBAL_TEAM_SOUND,
    EV_ENTITY_SOUND,

    EV_PLAY_ROFF,

    EV_GLASS_SHATTER,
    EV_DEBRIS,
    EV_MISC_MODEL_EXP,

    EV_CONC_ALT_IMPACT,

    EV_MISSILE_HIT,
    EV_MISSILE_MISS,
    EV_MISSILE_MISS_METAL,
    EV_BULLET, // otherEntity is the shooter

    EV_PAIN,
    EV_DEATH1,
    EV_DEATH2,
    EV_DEATH3,
    EV_OBITUARY,

    EV_POWERUP_QUAD,
    EV_POWERUP_BATTLESUIT,
    // EV_POWERUP_REGEN,

    EV_FORCE_DRAINED,

    EV_GIB_PLAYER, // gib a previously living player
    EV_SCOREPLUM,  // score plum

    EV_CTFMESSAGE,

    EV_BODYFADE,

    EV_SIEGE_ROUNDOVER,
    EV_SIEGE_OBJECTIVECOMPLETE,

    EV_DESTROY_GHOUL2_INSTANCE,

    EV_DESTROY_WEAPON_MODEL,

    EV_GIVE_NEW_RANK,
    EV_SET_FREE_SABER,
    EV_SET_FORCE_DISABLE,

    EV_WEAPON_CHARGE,
    EV_WEAPON_CHARGE_ALT,

    EV_SHIELD_HIT,

    EV_DEBUG_LINE,
    EV_TESTLINE,
    EV_STOPLOOPINGSOUND,
    EV_STARTLOOPINGSOUND,
    EV_TAUNT,

    // rww - Begin NPC sound events
    EV_ANGER1, // Say when acquire an enemy when didn't have one before
    EV_ANGER2,
    EV_ANGER3,

    EV_VICTORY1, // Say when killed an enemy
    EV_VICTORY2,
    EV_VICTORY3,

    EV_CONFUSE1, // Say when confused
    EV_CONFUSE2,
    EV_CONFUSE3,

    EV_PUSHED1, // Say when pushed
    EV_PUSHED2,
    EV_PUSHED3,

    EV_CHOKE1, // Say when choking
    EV_CHOKE2,
    EV_CHOKE3,

    EV_FFWARN, // ffire founds
    EV_FFTURN,
    // extra sounds for ST
    EV_CHASE1,
    EV_CHASE2,
    EV_CHASE3,
    EV_COVER1,
    EV_COVER2,
    EV_COVER3,
    EV_COVER4,
    EV_COVER5,
    EV_DETECTED1,
    EV_DETECTED2,
    EV_DETECTED3,
    EV_DETECTED4,
    EV_DETECTED5,
    EV_LOST1,
    EV_OUTFLANK1,
    EV_OUTFLANK2,
    EV_ESCAPING1,
    EV_ESCAPING2,
    EV_ESCAPING3,
    EV_GIVEUP1,
    EV_GIVEUP2,
    EV_GIVEUP3,
    EV_GIVEUP4,
    EV_LOOK1,
    EV_LOOK2,
    EV_SIGHT1,
    EV_SIGHT2,
    EV_SIGHT3,
    EV_SOUND1,
    EV_SOUND2,
    EV_SOUND3,
    EV_SUSPICIOUS1,
    EV_SUSPICIOUS2,
    EV_SUSPICIOUS3,
    EV_SUSPICIOUS4,
    EV_SUSPICIOUS5,
    // extra sounds for Jedi
    EV_COMBAT1,
    EV_COMBAT2,
    EV_COMBAT3,
    EV_JDETECTED1,
    EV_JDETECTED2,
    EV_JDETECTED3,
    EV_TAUNT1,
    EV_TAUNT2,
    EV_TAUNT3,
    EV_JCHASE1,
    EV_JCHASE2,
    EV_JCHASE3,
    EV_JLOST1,
    EV_JLOST2,
    EV_JLOST3,
    EV_DEFLECT1,
    EV_DEFLECT2,
    EV_DEFLECT3,
    EV_GLOAT1,
    EV_GLOAT2,
    EV_GLOAT3,
    EV_PUSHFAIL,

    EV_SIEGESPEC,

    EV_NUM_ENTITY_EVENTS
}; // There is a maximum of 256 events (8 bits transmission, 2 high bits for uniqueness)

enum global_team_sound_e {
    GTS_RED_CAPTURE,
    GTS_BLUE_CAPTURE,
    GTS_RED_RETURN,
    GTS_BLUE_RETURN,
    GTS_RED_TAKEN,
    GTS_BLUE_TAKEN,
    GTS_REDTEAM_SCORED,
    GTS_BLUETEAM_SCORED,
    GTS_REDTEAM_TOOK_LEAD,
    GTS_BLUETEAM_TOOK_LEAD,
    GTS_TEAMS_ARE_TIED
};

enum team_e {
    TEAM_FREE,
    TEAM_RED,
    TEAM_BLUE,
    TEAM_SPECTATOR,

    TEAM_NUM_TEAMS
};

enum duelTeam_e {
    DUELTEAM_FREE,
    DUELTEAM_LONE,
    DUELTEAM_DOUBLE,

    DUELTEAM_SINGLE, // for regular duel matches (not power duel)
};

// Time between location updates
#define TEAM_LOCATION_UPDATE_TIME 1000

// How many players on the overlay
#define TEAM_MAXOVERLAY 32

// team task
enum teamtask_e { TEAMTASK_NONE, TEAMTASK_OFFENSE, TEAMTASK_DEFENSE, TEAMTASK_PATROL, TEAMTASK_FOLLOW, TEAMTASK_RETRIEVE, TEAMTASK_ESCORT, TEAMTASK_CAMP };

// means of death
enum meansOfDeath_e {
    MOD_UNKNOWN,
    MOD_STUN_BATON,
    MOD_MELEE,
    MOD_SABER,
    MOD_BRYAR_PISTOL,
    MOD_BRYAR_PISTOL_ALT,
    MOD_BLASTER,
    MOD_TURBLAST,
    MOD_DISRUPTOR,
    MOD_DISRUPTOR_SPLASH,
    MOD_DISRUPTOR_SNIPER,
    MOD_BOWCASTER,
    MOD_REPEATER,
    MOD_REPEATER_ALT,
    MOD_REPEATER_ALT_SPLASH,
    MOD_DEMP2,
    MOD_DEMP2_ALT,
    MOD_FLECHETTE,
    MOD_FLECHETTE_ALT_SPLASH,
    MOD_ROCKET,
    MOD_ROCKET_SPLASH,
    MOD_ROCKET_HOMING,
    MOD_ROCKET_HOMING_SPLASH,
    MOD_THERMAL,
    MOD_THERMAL_SPLASH,
    MOD_TRIP_MINE_SPLASH,
    MOD_TIMED_MINE_SPLASH,
    MOD_DET_PACK_SPLASH,
    MOD_VEHICLE,
    MOD_CONC,
    MOD_CONC_ALT,
    MOD_FORCE_DARK,
    MOD_SENTRY,
    MOD_WATER,
    MOD_SLIME,
    MOD_LAVA,
    MOD_CRUSH,
    MOD_TELEFRAG,
    MOD_FALLING,
    MOD_SUICIDE,
    MOD_TARGET_LASER,
    MOD_TRIGGER_HURT,
    MOD_TEAM_CHANGE,
    // AURELIO: when/if you put this back in, remember to make a case for it in all the other places where
    // mod's are checked. Also, it probably isn't the most elegant solution for what you want - just add
    // a frag back to the player after you call the player_die (and keep a local of his pre-death score to
    // make sure he actually lost points, there may be cases where you don't lose points on changing teams
    // or suiciding, and so you would actually be giving him a point) -Rich
    //  I put it back in for now, if it becomes a problem we'll work around it later (it shouldn't though)...
    MOD_MAX
};

// gitem_t->type
enum itemType_e {
    IT_BAD,
    IT_WEAPON,  // EFX: rotate + upscale + minlight
    IT_AMMO,    // EFX: rotate
    IT_ARMOR,   // EFX: rotate + minlight
    IT_HEALTH,  // EFX: static external sphere + rotating internal
    IT_POWERUP, // instant on, timer based
    // EFX: rotate + external ring that rotates
    IT_HOLDABLE, // single use, holdable item
    // EFX: rotate + bob
    IT_PERSISTANT_POWERUP,
    IT_TEAM
};

#define MAX_ITEM_MODELS 3

typedef struct gitem_s {
    const char *classname; // spawning name
    const char *pickup_sound;
    const char *icon;
    int quantity;      // for ammo how much, or duration of powerup
    itemType_e giType; // IT_* flags
    int giTag;
    const char *view_model, *world_model[MAX_ITEM_MODELS];
    const char *precaches, *sounds;
    const char *description;
} gitem_t;

// included in both the game dll and the client

extern const gitem_t bg_itemlist[];
extern const size_t bg_numItems;

#define ITEM_INDEX(x) ((x)-bg_itemlist)

#define SABER_BLOCK_DUR 150 // number of milliseconds a block animation should take.

// dmflags->integer flags
#define DF_UNUSED0001 (0x0001u)
#define DF_UNUSED0002 (0x0002u)
#define DF_UNUSED0004 (0x0004u)
#define DF_NO_FALLING (0x0008u)
#define DF_FIXED_FOV (0x0010u)
#define DF_NO_FOOTSTEPS (0x0020u)
#define DF_NO_DROWN (0x0040u)
#define DF_FIXED_YAW (0x0080u)         // RAZTODO: Er, what do..
#define DF_NO_FIXED_ANIMS (0x0100u)    // RAZTODO: Whut?
#define DF_NO_REALISTIC_HOOK (0x0200u) // RAZTODO: Whut?

// rwwRMG - added in CONTENTS_TERRAIN
//  content masks
#define MASK_ALL (0xFFFFFFFFu)
#define MASK_SOLID (CONTENTS_SOLID | CONTENTS_TERRAIN)
#define MASK_PLAYERSOLID (CONTENTS_SOLID | CONTENTS_PLAYERCLIP | CONTENTS_BODY | CONTENTS_TERRAIN)
#define MASK_NPCSOLID (CONTENTS_SOLID | CONTENTS_MONSTERCLIP | CONTENTS_BODY | CONTENTS_TERRAIN)
#define MASK_DEADSOLID (CONTENTS_SOLID | CONTENTS_PLAYERCLIP | CONTENTS_TERRAIN)
#define MASK_WATER (CONTENTS_WATER | CONTENTS_LAVA | CONTENTS_SLIME)
#define MASK_OPAQUE (CONTENTS_SOLID | CONTENTS_SLIME | CONTENTS_LAVA | CONTENTS_TERRAIN)
#define MASK_SHOT (CONTENTS_SOLID | CONTENTS_BODY | CONTENTS_CORPSE | CONTENTS_TERRAIN)

// ET_FX States (stored in modelindex2)

#define FX_STATE_OFF 0
#define FX_STATE_ONE_SHOT 1
#define FX_STATE_ONE_SHOT_LIMIT 10
#define FX_STATE_CONTINUOUS 20

//
// entityState_t->eType
//
enum entityType_e {
    ET_GENERAL,
    ET_PLAYER,
    ET_ITEM,
    ET_MISSILE,
    ET_SPECIAL,  // rww - force fields
    ET_HOLOCRON, // rww - holocron icon displays
    ET_MOVER,
    ET_BEAM,
    ET_PORTAL,
    ET_SPEAKER,
    ET_PUSH_TRIGGER,
    ET_TELEPORT_TRIGGER,
    ET_INVISIBLE,
    ET_NPC, // ghoul2 player-like entity
    ET_TEAM,
    ET_BODY,
    ET_TERRAIN,
    ET_FX,

    ET_EVENTS, // any of the EV_* events can be added freestanding
    // by setting eType to ET_EVENTS + eventNum
    // this avoids having to set eFlags and eventNum
    ET_MAX
};
extern const stringID_table_t eTypes[ET_MAX];

//
// fields are needed for spawning from the entity string
//
// I moved these from g_spawn.c because the entity parsing stuff is semi-shared now -rww

#if defined(PROJECT_GAME) || defined(PROJECT_CGAME)

enum fieldtype_t {
    F_INT,
    F_FLOAT,
    F_LSTRING, // string on disk, pointer in memory, TAG_LEVEL
    F_VECTOR,
    F_ANGLEHACK,
    F_ENTITY, // index on disk, pointer in memory
    F_ITEM,   // index on disk, pointer in memory
    F_CLIENT, // index on disk, pointer in memory
    F_PARM1,  // Special case for parms
    F_PARM2,  // Special case for parms
    F_PARM3,  // Special case for parms
    F_PARM4,  // Special case for parms
    F_PARM5,  // Special case for parms
    F_PARM6,  // Special case for parms
    F_PARM7,  // Special case for parms
    F_PARM8,  // Special case for parms
    F_PARM9,  // Special case for parms
    F_PARM10, // Special case for parms
    F_PARM11, // Special case for parms
    F_PARM12, // Special case for parms
    F_PARM13, // Special case for parms
    F_PARM14, // Special case for parms
    F_PARM15, // Special case for parms
    F_PARM16, // Special case for parms
    F_IGNORE
};

typedef struct BG_field_s {
    const char *name;
    size_t ofs;
    fieldtype_t type;
} BG_field_t;

#endif

enum tauntTypes_e { TAUNT_TAUNT = 0, TAUNT_BOW, TAUNT_MEDITATE, TAUNT_FLOURISH, TAUNT_GLOAT };

// Okay, here lies the much-dreaded Pat-created FSM movement chart...  Heretic II strikes again!
// Why am I inflicting this on you?  Well, it's better than hardcoded states.
// Ideally this will be replaced with an external file or more sophisticated move-picker
// once the game gets out of prototype stage.

// rww - Moved all this to bg_public so that we can access the saberMoveData stuff on the cgame
// which is currently used for determining if a saber trail should be rendered in a given frame
#ifdef LS_NONE
#undef LS_NONE
#endif

enum saberMoveName_e {
    // totally invalid
    LS_INVALID = -1,
    // Invalid, or saber not armed
    LS_NONE = 0,

    // General movements with saber
    LS_READY,
    LS_DRAW,
    LS_PUTAWAY,

    // Attacks
    LS_A_TL2BR, // 4
    LS_A_L2R,
    LS_A_BL2TR,
    LS_A_BR2TL,
    LS_A_R2L,
    LS_A_TR2BL,
    LS_A_T2B,
    LS_A_BACKSTAB,
    LS_A_BACK,
    LS_A_BACK_CR,
    LS_ROLL_STAB,
    LS_A_LUNGE,
    LS_A_JUMP_T__B_,
    LS_A_FLIP_STAB,
    LS_A_FLIP_SLASH,
    LS_JUMPATTACK_DUAL,
    LS_JUMPATTACK_ARIAL_LEFT,
    LS_JUMPATTACK_ARIAL_RIGHT,
    LS_JUMPATTACK_CART_LEFT,
    LS_JUMPATTACK_CART_RIGHT,
    LS_JUMPATTACK_STAFF_LEFT,
    LS_JUMPATTACK_STAFF_RIGHT,
    LS_BUTTERFLY_LEFT,
    LS_BUTTERFLY_RIGHT,
    LS_A_BACKFLIP_ATK,
    LS_SPINATTACK_DUAL,
    LS_SPINATTACK,
    LS_LEAP_ATTACK,
    LS_SWOOP_ATTACK_RIGHT,
    LS_SWOOP_ATTACK_LEFT,
    LS_TAUNTAUN_ATTACK_RIGHT,
    LS_TAUNTAUN_ATTACK_LEFT,
    LS_KICK_F,
    LS_KICK_B,
    LS_KICK_R,
    LS_KICK_L,
    LS_KICK_S,
    LS_KICK_BF,
    LS_KICK_RL,
    LS_KICK_F_AIR,
    LS_KICK_B_AIR,
    LS_KICK_R_AIR,
    LS_KICK_L_AIR,
    LS_STABDOWN,
    LS_STABDOWN_STAFF,
    LS_STABDOWN_DUAL,
    LS_DUAL_SPIN_PROTECT,
    LS_STAFF_SOULCAL,
    LS_A1_SPECIAL,
    LS_A2_SPECIAL,
    LS_A3_SPECIAL,
    LS_UPSIDE_DOWN_ATTACK,
    LS_PULL_ATTACK_STAB,
    LS_PULL_ATTACK_SWING,
    LS_SPINATTACK_ALORA,
    LS_DUAL_FB,
    LS_DUAL_LR,
    LS_HILT_BASH,

    // starts
    LS_S_TL2BR, // 26
    LS_S_L2R,
    LS_S_BL2TR, //# Start of attack chaining to SLASH LR2UL
    LS_S_BR2TL, //# Start of attack chaining to SLASH LR2UL
    LS_S_R2L,
    LS_S_TR2BL,
    LS_S_T2B,

    // returns
    LS_R_TL2BR, // 33
    LS_R_L2R,
    LS_R_BL2TR,
    LS_R_BR2TL,
    LS_R_R2L,
    LS_R_TR2BL,
    LS_R_T2B,

    // transitions
    LS_T1_BR__R, // 40
    LS_T1_BR_TR,
    LS_T1_BR_T_,
    LS_T1_BR_TL,
    LS_T1_BR__L,
    LS_T1_BR_BL,
    LS_T1__R_BR, // 46
    LS_T1__R_TR,
    LS_T1__R_T_,
    LS_T1__R_TL,
    LS_T1__R__L,
    LS_T1__R_BL,
    LS_T1_TR_BR, // 52
    LS_T1_TR__R,
    LS_T1_TR_T_,
    LS_T1_TR_TL,
    LS_T1_TR__L,
    LS_T1_TR_BL,
    LS_T1_T__BR, // 58
    LS_T1_T___R,
    LS_T1_T__TR,
    LS_T1_T__TL,
    LS_T1_T___L,
    LS_T1_T__BL,
    LS_T1_TL_BR, // 64
    LS_T1_TL__R,
    LS_T1_TL_TR,
    LS_T1_TL_T_,
    LS_T1_TL__L,
    LS_T1_TL_BL,
    LS_T1__L_BR, // 70
    LS_T1__L__R,
    LS_T1__L_TR,
    LS_T1__L_T_,
    LS_T1__L_TL,
    LS_T1__L_BL,
    LS_T1_BL_BR, // 76
    LS_T1_BL__R,
    LS_T1_BL_TR,
    LS_T1_BL_T_,
    LS_T1_BL_TL,
    LS_T1_BL__L,

    // Bounces
    LS_B1_BR,
    LS_B1__R,
    LS_B1_TR,
    LS_B1_T_,
    LS_B1_TL,
    LS_B1__L,
    LS_B1_BL,

    // Deflected attacks
    LS_D1_BR,
    LS_D1__R,
    LS_D1_TR,
    LS_D1_T_,
    LS_D1_TL,
    LS_D1__L,
    LS_D1_BL,
    LS_D1_B_,

    // Reflected attacks
    LS_V1_BR,
    LS_V1__R,
    LS_V1_TR,
    LS_V1_T_,
    LS_V1_TL,
    LS_V1__L,
    LS_V1_BL,
    LS_V1_B_,

    // Broken parries
    LS_H1_T_, //
    LS_H1_TR,
    LS_H1_TL,
    LS_H1_BR,
    LS_H1_B_,
    LS_H1_BL,

    // Knockaways
    LS_K1_T_, //
    LS_K1_TR,
    LS_K1_TL,
    LS_K1_BR,
    LS_K1_BL,

    // Parries
    LS_PARRY_UP, //
    LS_PARRY_UR,
    LS_PARRY_UL,
    LS_PARRY_LR,
    LS_PARRY_LL,

    // Projectile Reflections
    LS_REFLECT_UP, //
    LS_REFLECT_UR,
    LS_REFLECT_UL,
    LS_REFLECT_LR,
    LS_REFLECT_LL,

    LS_JAPLUS_SPINATK,
    LS_JAPLUS_STFNEWANIM_BACKKICKFWD,
    LS_JAPLUS_STFNEWANIM_SPINKICKFWD,
    LS_JAPLUS_NEWANIM_JUMPBACKKICKSPIN,
    LS_JAPLUS_NEWANIM_JUMPBACKKICKFLIP,
    LS_JAPLUS_NEWANIM_FLIPSABERSTAB,

    LS_MOVE_MAX //
};

enum saberQuadrant_e { Q_BR, Q_R, Q_TR, Q_T, Q_TL, Q_L, Q_BL, Q_B, Q_NUM_QUADS };

typedef struct saberMoveData_s {
    const char *name;
    int animToUse;
    int startQuad;
    int endQuad;
    uint32_t animSetFlags;
    int blendTime;
    int blocking;
    saberMoveName_e chain_idle;   // What move to call if the attack button is not pressed at the end of this anim
    saberMoveName_e chain_attack; // What move to call if the attack button (and nothing else) is pressed
    qboolean trailLength;
} saberMoveData_t;
extern saberMoveData_t saberMoveData[LS_MOVE_MAX];

enum saberType_e {
    SABER_NONE = 0,
    SABER_SINGLE,
    SABER_STAFF,
    SABER_DAGGER,
    SABER_BROAD,
    SABER_PRONG,
    SABER_ARC,
    SABER_SAI,
    SABER_CLAW,
    SABER_LANCE,
    SABER_STAR,
    SABER_TRIDENT,
    SABER_SITH_SWORD,
    NUM_SABERS
};

typedef struct saberTrail_s {
    // Actual trail stuff
    int inAction; // controls whether should we even consider starting one
    int duration; // how long each trail seg stays in existence
    int lastTime; // time a saber segement was last stored
    vector3 base, dualbase;
    vector3 tip, dualtip;

    // Marks stuff
    qboolean haveOldPos[2];
    vector3 oldPos[2];
    vector3 oldNormal[2]; // store this in case we don't have a connect-the-dots situation
                          //	..then we'll need the normal to project a mark blob onto the impact point
} saberTrail_t;

typedef struct bladeInfo_s {
    qboolean active;
    saber_colors_t color;
    float radius;
    float length, lengthMax, lengthOld;
    float desiredLength;
    vector3 muzzlePoint, muzzlePointOld;
    vector3 muzzleDir, muzzleDirOld;
    saberTrail_t trail;
    int hitWallDebounceTime;
    int storageTime;
    int extendDebounce;
} bladeInfo_t;
#define MAX_BLADES 8

enum saber_styles_e { SS_NONE = 0, SS_FAST, SS_MEDIUM, SS_STRONG, SS_DESANN, SS_TAVION, SS_DUAL, SS_STAFF, SS_NUM_SABER_STYLES };

// SABER FLAGS
#define SFL_NOT_LOCKABLE (0x00000001u)           // can't get into a saberlock
#define SFL_NOT_THROWABLE (0x00000002u)          // can't be thrown - FIXME: maybe make this a max level of force saber throw that can be used with this saber?
#define SFL_NOT_DISARMABLE (0x00000004u)         // can't be dropped
#define SFL_NOT_ACTIVE_BLOCKING (0x00000008u)    // don't to try to block incoming shots with this saber
#define SFL_TWO_HANDED (0x00000010u)             // uses both hands
#define SFL_SINGLE_BLADE_THROWABLE (0x00000020u) // can throw this saber if only the first blade is on
#define SFL_RETURN_DAMAGE (0x00000040u)          // when returning from a saber throw, it keeps spinning and doing damage
#define SFL_ON_IN_WATER (0x00000080u)            // weapon stays active even in water
#define SFL_BOUNCE_ON_WALLS (0x00000100u)        // the saber will bounce back when it hits solid architecture (good for real-sword type mods)
#define SFL_BOLT_TO_WRIST (0x00000200u)          // saber model is bolted to wrist, not in hand... useful for things like claws & shields, etc.
#define SFL_NO_PULL_ATTACK (0x00000400u)         // cannot do pull+attack move (move not available in MP anyway)
#define SFL_NO_BACK_ATTACK (0x00000800u)         // cannot do back-stab moves
#define SFL_NO_STABDOWN (0x00001000u)            // cannot do stabdown move (when enemy is on ground)
#define SFL_NO_WALL_RUNS (0x00002000u)           // cannot side-run or forward-run on walls
#define SFL_NO_WALL_FLIPS (0x00004000u)          // cannot do backflip off wall or side-flips off walls
#define SFL_NO_WALL_GRAB (0x00008000u)           // cannot grab wall & jump off
#define SFL_NO_ROLLS (0x00010000u)               // cannot roll
#define SFL_NO_FLIPS (0x00020000u)               // cannot do flips
#define SFL_NO_CARTWHEELS (0x00040000u)          // cannot do cartwheels
#define SFL_NO_KICKS (0x00080000u)               // cannot do kicks (can't do kicks anyway if using a throwable saber/sword)
#define SFL_NO_MIRROR_ATTACKS (0x00100000u)      // cannot do the simultaneous attack left/right moves (only available in Dual Lightsaber Combat Style)
#define SFL_NO_ROLL_STAB (0x00200000u)           // cannot do roll-stab move at end of roll

#define SFL2_NO_WALL_MARKS (0x00000001u)    // stops the saber from drawing marks on the world (good for real-sword type mods)
#define SFL2_NO_DLIGHT (0x00000002u)        // stops the saber from drawing a dynamic light (good for real-sword type mods)
#define SFL2_NO_BLADE (0x00000004u)         // stops the saber from drawing a blade (good for real-sword type mods)
#define SFL2_NO_CLASH_FLARE (0x00000008u)   // the saber will not do the big, white clash flare with other sabers
#define SFL2_NO_DISMEMBERMENT (0x00000010u) // the saber never does dismemberment (good for pointed/blunt melee weapons)
#define SFL2_NO_IDLE_EFFECT                                                                                                                                    \
    (0x00000020u)                       // the saber will not do damage or any effects when it is idle (not in an attack anim).  (good for real-sword type mods)
#define SFL2_ALWAYS_BLOCK (0x00000040u) // the blades will always be blocking (good for things like shields that should always block)
#define SFL2_NO_MANUAL_DEACTIVATE (0x00000080u) // the blades cannot manually be toggled on and off
#define SFL2_TRANSITION_DAMAGE (0x00000100u)    // the blade does damage in start, transition and return anims (like strong style does)
#define SFL2_NO_WALL_MARKS2 (0x00000200u)       // stops the saber from drawing marks on the world (good for real-sword type mods)
#define SFL2_NO_DLIGHT2 (0x00000400u)           // stops the saber from drawing a dynamic light (good for real-sword type mods)
#define SFL2_NO_BLADE2 (0x00000800u)            // stops the saber from drawing a blade (good for real-sword type mods)
#define SFL2_NO_CLASH_FLARE2 (0x00001000u)      // the saber will not do the big, white clash flare with other sabers
#define SFL2_NO_DISMEMBERMENT2 (0x00002000u)    // the saber never does dismemberment (good for pointed/blunt melee weapons)
#define SFL2_NO_IDLE_EFFECT2                                                                                                                                   \
    (0x00004000u) // the saber will not do damage or any effects when it is idle (not in an attack anim).  (good for real-sword type mods)
#define SFL2_ALWAYS_BLOCK2 (0x00008000u)         // the blades will always be blocking (good for things like shields that should always block)
#define SFL2_NO_MANUAL_DEACTIVATE2 (0x00010000u) // the blades cannot manually be toggled on and off
#define SFL2_TRANSITION_DAMAGE2 (0x00020000u)    // the blade does damage in start, transition and return anims (like strong style does)

#define SABER_NAME_LENGTH (64)
typedef struct saberInfo_s {
    char name[SABER_NAME_LENGTH];     // entry in sabers.cfg, if any
    char fullName[SABER_NAME_LENGTH]; // the "Proper Name" of the saber, shown in UI
    saberType_e type;                 // none, single or staff
    char model[MAX_QPATH];            // hilt model
    qhandle_t skin;                   // registered skin id
    int soundOn;                      // game soundindex for turning on sound
    int soundLoop;                    // game soundindex for hum/loop sound
    int soundOff;                     // game soundindex for turning off sound
    int numBlades;
    bladeInfo_t blade[MAX_BLADES]; // blade info - like length, trail, origin, dir, etc.
    int stylesLearned;             // styles you get when you get this saber, if any
    int stylesForbidden;           // styles you cannot use with this saber, if any
    int maxChain;                  // how many moves can be chained in a row with this weapon (-1 is infinite, 0 is use default behavior)
    int forceRestrictions; // force powers that cannot be used while this saber is on (bitfield) - FIXME: maybe make this a limit on the max level, per force
                           // power, that can be used with this type?
    int lockBonus;         // in saberlocks, this type of saber pushes harder or weaker
    int parryBonus;        // added to strength of parry with this saber
    int breakParryBonus, breakParryBonus2; // added to strength when hit a parry
    int disarmBonus, disarmBonus2;         // added to disarm chance when win saberlock or have a good parry (knockaway)
    saber_styles_e singleBladeStyle;       // makes it so that you use a different style if you only have the first blade active

    // these values are global to the saber, like all of the ones above
    uint32_t saberFlags, saberFlags2; // from SFL(2)_ list above

    // done in cgame (client-side code)
    qhandle_t spinSound;     // none - if set, plays this sound as it spins when thrown
    qhandle_t swingSound[3]; // none - if set, plays one of these 3 sounds when swung during an attack - NOTE: must provide all 3!!!

    // done in game (server-side code)
    float moveSpeedScale; // 1.0f - you move faster/slower when using this saber
    float animSpeedScale; // 1.0f - plays normal attack animations faster/slower

    // done in both cgame and game (BG code)
    int kataMove;         // LS_INVALID - if set, player will execute this move when they press both attack buttons at the same time
    int lungeAtkMove;     // LS_INVALID - if set, player will execute this move when they crouch+fwd+attack
    int jumpAtkUpMove;    // LS_INVALID - if set, player will execute this move when they jump+attack
    int jumpAtkFwdMove;   // LS_INVALID - if set, player will execute this move when they jump+fwd+attack
    int jumpAtkBackMove;  // LS_INVALID - if set, player will execute this move when they jump+back+attack
    int jumpAtkRightMove; // LS_INVALID - if set, player will execute this move when they jump+rightattack
    int jumpAtkLeftMove;  // LS_INVALID - if set, player will execute this move when they jump+left+attack
    int readyAnim;        // -1 - anim to use when standing idle
    int drawAnim;         // -1 - anim to use when drawing weapon
    int putawayAnim;      // -1 - anim to use when putting weapon away
    int tauntAnim;        // -1 - anim to use when hit "taunt"
    int bowAnim;          // -1 - anim to use when hit "bow"
    int meditateAnim;     // -1 - anim to use when hit "meditate"
    int flourishAnim;     // -1 - anim to use when hit "flourish"
    int gloatAnim;        // -1 - anim to use when hit "gloat"

    //***NOTE: you can only have a maximum of 2 "styles" of blades, so this next value, "bladeStyle2Start" is the number of the first blade to use these value
    // on... all blades before this use the normal values above, all blades at and after this number use the secondary values below***
    int bladeStyle2Start; // 0 - if set, blades from this number and higher use the following values (otherwise, they use the normal values already set)

    //***The following can be different for the extra blades - not setting them individually defaults them to the value for the whole saber (and first blade)***

    // done in cgame (client-side code)
    int trailStyle, trailStyle2;       // 0 - default (0) is normal, 1 is a motion blur and 2 is no trail at all (good for real-sword type mods)
    int g2MarksShader, g2MarksShader2; // none - if set, the game will use this shader for marks on enemies instead of the default "gfx/damage/saberglowmark"
    int g2WeaponMarkShader, g2WeaponMarkShader2; // none - if set, the game will ry to project this shader onto the weapon when it damages a person (good for a
                                                 // blood splatter on the weapon)
    qhandle_t hitSound[3], hit2Sound[3];         // none - if set, plays one of these 3 sounds when saber hits a person - NOTE: must provide all 3!!!
    qhandle_t blockSound[3],
        block2Sound[3]; // none - if set, plays one of these 3 sounds when saber/sword hits another saber/sword - NOTE: must provide all 3!!!
    qhandle_t bounceSound[3], bounce2Sound[3]; // none - if set, plays one of these 3 sounds when saber/sword hits a wall and bounces off (must set bounceOnWall
                                               // to 1 to use these sounds) - NOTE: must provide all 3!!!
    int blockEffect, blockEffect2; // none - if set, plays this effect when the saber/sword hits another saber/sword (instead of "saber/saber_block.efx")
    int hitPersonEffect, hitPersonEffect2; // none - if set, plays this effect when the saber/sword hits a person (instead of "saber/blood_sparks_mp.efx")
    int hitOtherEffect,
        hitOtherEffect2;           // none - if set, plays this effect when the saber/sword hits something else damagable (instead of "saber/saber_cut.efx")
    int bladeEffect, bladeEffect2; // none - if set, plays this effect at the blade tag

    // done in game (server-side code)
    float knockbackScale, knockbackScale2;   // 0 - if non-zero, uses damage done to calculate an appropriate amount of knockback
    float damageScale, damageScale2;         // 1 - scale up or down the damage done by the saber
    float splashRadius, splashRadius2;       // 0 - radius of splashDamage
    int splashDamage, splashDamage2;         // 0 - amount of splashDamage, 100% at a distance of 0, 0% at a distance = splashRadius
    float splashKnockback, splashKnockback2; // 0 - amount of splashKnockback, 100% at a distance of 0, 0% at a distance = splashRadius
} saberInfo_t;
#define MAX_SABERS 2

extern const int WeaponReadyAnim[WP_NUM_WEAPONS];
extern const int WeaponReadyLegsAnim[WP_NUM_WEAPONS];
extern const int WeaponAttackAnim[WP_NUM_WEAPONS];

extern const int forcePowerDarkLight[NUM_FORCE_POWERS];

extern bgEntity_t *pm_entSelf;
extern bgEntity_t *pm_entVeh;

#define ARENAS_PER_TIER 4
#define MAX_ARENAS 1024
#define MAX_ARENAS_TEXT 8192

#define MAX_BOTS 1024
#define MAX_BOTS_TEXT 8192

#define HYPERSPACE_TIME 4000 // For hyperspace triggers
#define HYPERSPACE_TELEPORT_FRAC 0.75f
#define HYPERSPACE_SPEED 10000.0f // was 30000
#define HYPERSPACE_TURN_RATE 45.0f

extern const char *gametypeStringShort[GT_MAX_GAME_TYPE];

void *BG_Alloc(int size);
void *BG_AllocUnaligned(int size);
void BG_AddPredictableEventToPlayerstate(int newEvent, int eventParm, playerState_t *ps);
int BG_AnimLength(int index, animNumber_e anim);
void BG_AttachToRancor(void *ghoul2, float rancYaw, vector3 *rancOrigin, int time, qhandle_t *modelList, vector3 *modelScale, qboolean inMouth,
                       vector3 *out_origin, vector3 *out_angles, vector3 out_axis[3]);
void BG_BLADE_ActivateTrail(bladeInfo_t *blade, float duration);
void BG_BLADE_DeactivateTrail(bladeInfo_t *blade, float duration);
int BG_BrokenParryForAttack(int move);
int BG_BrokenParryForParry(int move);
qboolean BG_CanItemBeGrabbed(int gametype, const entityState_t *ent, const playerState_t *ps);
qboolean BG_CanUseFPNow(int gametype, playerState_t *ps, int time, forcePowers_t power);
void BG_ClearAnimsets(void);
void BG_ClearRocketLock(playerState_t *ps);
qboolean BG_DirectFlippingAnim(int anim);
void BG_EvaluateTrajectory(const trajectory_t *tr, int atTime, vector3 *result);
void BG_EvaluateTrajectoryDelta(const trajectory_t *tr, int atTime, vector3 *result);
const gitem_t *BG_FindItem(const char *classname);
const gitem_t *BG_FindItemForAmmo(ammo_e ammo);
const gitem_t *BG_FindItemForHoldable(holdable_e hi);
const gitem_t *BG_FindItemForPowerup(powerup_e pw);
const gitem_t *BG_FindItemForWeapon(weapon_e wp);
void BG_FixSaberMoveData(void);
qboolean BG_FlippingAnim(int anim);
void BG_ForcePowerDrain(playerState_t *ps, forcePowers_t forcePower, int overrideAmt);
void BG_G2ATSTAngles(void *ghoul2, int time, vector3 *cent_lerpAngles);
void BG_G2PlayerAngles(void *ghoul2, int motionBolt, entityState_t *cent, int time, vector3 *cent_lerpOrigin, vector3 *cent_lerpAngles, vector3 legs[3],
                       vector3 *legsAngles, qboolean *tYawing, qboolean *tPitching, qboolean *lYawing, float *tYawAngle, float *tPitchAngle, float *lYawAngle,
                       int frametime, vector3 *turAngles, vector3 *modelScale, int ciLegs, int ciTorso, int *corrTime, vector3 *lookAngles,
                       vector3 *lastHeadAngles, int lookTime, entityState_t *emplaced, int *crazySmoothFactor);
int BG_GetGametypeForString(const char *gametype);
const char *BG_GetGametypeString(int gametype);
int BG_GetItemIndexByTag(int tag, itemType_e type);
float BG_GetLegsAnimPoint(playerState_t *ps, int AnimIndex);
uint32_t BG_GetMapTypeBits(const char *type);
team_e BG_GetOpposingTeam(team_e team);
float BG_GetTorsoAnimPoint(playerState_t *ps, int AnimIndex);
void BG_GiveMeVectorFromMatrix(mdxaBone_t *boltMatrix, uint32_t flags, vector3 *vec);
qboolean BG_HasAnimation(int animIndex, int animation);
qboolean BG_HasYsalamiri(int gametype, playerState_t *ps);
void BG_IK_MoveArm(void *ghoul2, int lHandBolt, int time, entityState_t *ent, int basePose, vector3 *desiredPos, qboolean *ikInProgress, vector3 *origin,
                   vector3 *angles, vector3 *scale, int blendTime, qboolean forceHalt);
qboolean BG_InBackFlip(int anim);
qboolean BG_InDeathAnim(int anim);
int BG_InGrappleMove(int anim);
void BG_InitAnimsets(void);
qboolean BG_InKataAnim(int anim);
qboolean BG_InKnockDown(int anim);
qboolean BG_InLedgeMove(int anim);
qboolean BG_InReboundHold(int anim);
qboolean BG_InReboundJump(int anim);
qboolean BG_InReboundRelease(int anim);
qboolean BG_InRoll(playerState_t *ps, int anim);
qboolean BG_InSaberLock(int anim);
qboolean BG_InSaberLockOld(int anim);
qboolean BG_InSaberStandAnim(int anim);
qboolean BG_InSpecialJump(int anim);
qboolean BG_IsItemSelectable(playerState_t *ps, int item);
qboolean BG_KickingAnim(int anim);
qboolean BG_KickMove(int move);
int BG_KnockawayForParry(int move);
qboolean BG_KnockDownable(playerState_t *ps);
qboolean BG_LegalizedForcePowers(char *powerOut, size_t powerOutSize, int maxRank, qboolean freeSaber, int teamForce, int gametype, int fpDisabled);
saberInfo_t *BG_MySaber(int clientNum, int saberNum);
qboolean BG_OutOfMemory(void);
int BG_ParseAnimationFile(const char *filename, animation_t *animSet, qboolean isHumanoid);
void BG_PlayerStateToEntityState(playerState_t *ps, entityState_t *s, qboolean snap);
void BG_PlayerStateToEntityStateExtraPolate(playerState_t *ps, entityState_t *s, int time, qboolean snap);
qboolean BG_PlayerTouchesItem(playerState_t *ps, entityState_t *item, int atTime);
int BG_PickAnim(int animIndex, int minAnim, int maxAnim);
qboolean BG_SaberInAttack(int move);
qboolean BG_SaberInIdle(int move);
qboolean BG_SaberInKata(int saberMove);
qboolean BG_SaberInSpecial(int move);
qboolean BG_SaberInSpecialAttack(int anim);
qboolean BG_SaberInTransitionAny(int move);
void BG_SaberStartTransAnim(int clientNum, int saberAnimLevel, int weapon, int anim, float *animSpeed, int broken);
qboolean BG_SabersOff(playerState_t *ps);
void BG_SI_Activate(saberInfo_t *saber);
void BG_SI_ActivateTrail(saberInfo_t *saber, float duration);
qboolean BG_SI_Active(saberInfo_t *saber);
void BG_SI_BladeActivate(saberInfo_t *saber, int iBlade, qboolean bActive);
void BG_SI_Deactivate(saberInfo_t *saber);
void BG_SI_DeactivateTrail(saberInfo_t *saber, float duration);
float BG_SI_Length(saberInfo_t *saber);
float BG_SI_LengthMax(saberInfo_t *saber);
void BG_SI_SetDesiredLength(saberInfo_t *saber, float len, int bladeNum);
void BG_SI_SetLength(saberInfo_t *saber, float length);
void BG_SI_SetLengthGradual(saberInfo_t *saber, int time);
qboolean BG_SpinningSaberAnim(int anim);
char *BG_StringAlloc(const char *source);
void *BG_TempAlloc(int size);
void BG_TempFree(int size);
void BG_TouchJumpPad(playerState_t *ps, entityState_t *jumppad);
qboolean In_LedgeIdle(int anim);
bgEntity_t *PM_BGEntForNum(int num);
void PM_UpdateViewAngles(playerState_t *ps, const usercmd_t *cmd);
void Pmove(pmove_t *pmove);

// returns false if any usable projectile/ammo-consuming weapons are enabled
//	this ignores melee, saber, stun baton etc
bool BG_HasSetSaberOnly(
#if defined(PROJECT_GAME)
    void
#elif defined(PROJECT_CGAME) || defined(PROJECT_UI)
    const char *info
#endif
);

#ifndef PROJECT_GAME
int BG_ParseAnimationEvtFile(const char *as_filename, int animFileIndex, int eventFileIndex);
#endif
