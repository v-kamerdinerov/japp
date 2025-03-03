#pragma once

#include "ai.h"

#define NPCAI_CHECK_WEAPON (0x00000001u)        //
#define NPCAI_BURST_WEAPON (0x00000002u)        //
#define NPCAI_MOVING (0x00000004u)              //
#define NPCAI_TOUCHED_GOAL (0x00000008u)        //
#define NPCAI_PUSHED (0x00000010u)              //
#define NPCAI_NO_COLL_AVOID (0x00000020u)       //
#define NPCAI_BLOCKED (0x00000040u)             //
#define NPCAI_UNUSED00000080 (0x00000080u)      //
#define NPCAI_OFF_PATH (0x00000100u)            //
#define NPCAI_IN_SQUADPOINT (0x00000200u)       //
#define NPCAI_STRAIGHT_TO_DESTPOS (0x00000400u) //
#define NPCAI_UNUSED00000800 (0x00000800u)      //
#define NPCAI_NO_SLOWDOWN (0x00001000u)         //
#define NPCAI_LOST (0x00002000u)                // Can't nav to his goal
#define NPCAI_SHIELDS (0x00004000u)             // Has shields, borg can adapt
#define NPCAI_GREET_ALLIES (0x00008000u)        // Say hi to nearby allies
#define NPCAI_FORM_TELE_NAV (0x00010000u)       // Tells formation people to use nav info to get to
#define NPCAI_ENROUTE_TO_HOMEWP (0x00020000u)   // Lets us know to run our lostenemyscript when we get to homeWp
#define NPCAI_MATCHPLAYERWEAPON (0x00040000u)   // Match the player's weapon except when it changes during cinematics
#define NPCAI_UNUSED00080000 (0x00080000u)      //
#define NPCAI_DIE_ON_IMPACT (0x00100000u)       // Next time you crashland, die!
#define NPCAI_CUSTOM_GRAVITY (0x00200000u)      // Don't use g_gravity, I fly!

// Script flags
#define SCF_CROUCHED (0x00000001u)         // Force ucmd.upmove to be -127
#define SCF_WALKING (0x00000002u)          // Force BUTTON_WALKING to be pressed
#define SCF_MORELIGHT (0x00000004u)        // NPC will have a minlight of 96
#define SCF_LEAN_RIGHT (0x00000008u)       // Force rightmove+BUTTON_USE
#define SCF_LEAN_LEFT (0x00000010u)        // Force leftmove+BUTTON_USE
#define SCF_RUNNING (0x00000020u)          // Takes off walking button, overrides SCF_WALKING
#define SCF_ALT_FIRE (0x00000040u)         // Force to use alt-fire when firing
#define SCF_NO_RESPONSE (0x00000080u)      // NPC will not do generic responses to being used
#define SCF_FFDEATH (0x00000100u)          // Just tells player_die to run the friendly fire deathscript
#define SCF_NO_COMBAT_TALK (0x00000200u)   // NPC will not use their generic combat chatter stuff
#define SCF_CHASE_ENEMIES (0x00000400u)    // NPC chase enemies - FIXME: right now this is synonymous with using combat points... should it be?
#define SCF_LOOK_FOR_ENEMIES (0x00000800u) // NPC be on the lookout for enemies
#define SCF_FACE_MOVE_DIR (0x00001000u)    // NPC face direction it's moving - FIXME: not really implemented right now
#define SCF_IGNORE_ALERTS (0x00002000u)    // NPC ignore alert events
#define SCF_DONT_FIRE (0x00004000u)        // NPC won't shoot
#define SCF_DONT_FLEE (0x00008000u)        // NPC never flees
#define SCF_FORCED_MARCH (0x00010000u)     // NPC that the player must aim at to make him walk
#define SCF_NO_GROUPS (0x00020000u)        // NPC cannot alert groups or be part of a group
#define SCF_FIRE_WEAPON (0x00040000u)      // NPC will fire his (her) weapon
#define SCF_NO_MIND_TRICK (0x00080000u)    // Not succeptible to mind tricks
#define SCF_USE_CP_NEAREST (0x00100000u)   // Will use combat point close to it, not next to player or try and flank player
#define SCF_NO_FORCE (0x00200000u)         // Not succeptible to force powers
#define SCF_NO_FALLTODEATH (0x00400000u)   // NPC will not scream and tumble and fall to hit death over large drops
#define SCF_NO_ACROBATICS (0x00800000u)    // Jedi won't jump, roll or cartwheel
#define SCF_USE_SUBTITLES (0x01000000u)    // Regardless of subtitle setting, this NPC will display subtitles when it speaks lines
#define SCF_NO_ALERT_TALK (0x02000000u)    // Will not say alert sounds, but still can be woken up by alerts

//#ifdef __DEBUG

// Debug flag definitions

#define AID_IDLE (0x00000000u)     // Nothing is happening
#define AID_ACQUIRED (0x00000001u) // A target has been found
#define AID_LOST (0x00000002u)     // Alert, but no target is in sight
#define AID_CONFUSED (0x00000004u) // Is unable to come up with a course of action
#define AID_LOSTPATH (0x00000008u) // Cannot make a valid movement due to lack of connections

//#endif //__DEBUG

// extern qboolean showWaypoints;

enum jumpState_e { JS_WAITING = 0, JS_FACING, JS_CROUCHING, JS_JUMPING, JS_LANDING };

enum lookMode_e { LM_ENT = 0, LM_INTEREST };

enum speechType_e {
    SPEECH_CHASE,
    SPEECH_CONFUSED,
    SPEECH_COVER,
    SPEECH_DETECTED,
    SPEECH_GIVEUP,
    SPEECH_LOOK,
    SPEECH_LOST,
    SPEECH_OUTFLANK,
    SPEECH_ESCAPING,
    SPEECH_SIGHT,
    SPEECH_SOUND,
    SPEECH_SUSPICIOUS,
    SPEECH_YELL,
    SPEECH_PUSHED
};

enum spot_e { SPOT_ORIGIN, SPOT_CHEST, SPOT_HEAD, SPOT_HEAD_LEAN, SPOT_WEAPON, SPOT_LEGS, SPOT_GROUND };

enum visibility_e { VIS_UNKNOWN, VIS_NOT, VIS_PVS, VIS_360, VIS_FOV, VIS_SHOOT };

typedef struct gNPCstats_e { // Stats, loaded in, and can be set by scripts
    // AI
    int aggression;      //			"
    int aim;             //			"
    float earshot;       //			"
    int evasion;         //			"
    int hfov;            // horizontal field of view
    int intelligence;    //			"
    int move;            //			"
    int reactions;       // 1-5, higher is better
    float shootDistance; // Maximum range- overrides range set for weapon if nonzero
    int vfov;            // vertical field of view
    float vigilance;     //			"
    float visrange;      //			"
    // Movement
    int runSpeed;
    int walkSpeed;
    float yawSpeed; // 1 - whatever, default is 50
    int health;
    int acceleration;
} gNPCstats_t;

// NOTE!!!  If you add any ptr fields into this structure could you please tell me so I can update the load/save code?
//	so far the only things I've got to cope with are a bunch of gentity_t*'s, but tell me if any more get added -slc
//

#define MAX_ENEMY_POS_LAG 2400
#define ENEMY_POS_LAG_INTERVAL 100
#define ENEMY_POS_LAG_STEPS (MAX_ENEMY_POS_LAG / ENEMY_POS_LAG_INTERVAL)
typedef struct gNPC_s {
    // FIXME: Put in playerInfo or something
    int timeOfDeath; // FIXME do we really need both of these
    gentity_t *touchedByPlayer;

    visibility_e enemyLastVisibility;

    int aimTime;
    float desiredYaw;
    float desiredPitch;
    float lockedDesiredYaw;
    float lockedDesiredPitch;
    gentity_t *aimingBeam; // debugging aid

    vector3 enemyLastSeenLocation;
    int enemyLastSeenTime;
    vector3 enemyLastHeardLocation;
    int enemyLastHeardTime;
    int lastAlertID; // unique ID

    uint32_t eFlags;
    uint32_t aiFlags;

    int currentAmmo; // this sucks, need to find a better way
    int shotTime;
    int burstCount;
    int burstMin;
    int burstMean;
    int burstMax;
    int burstSpacing;
    int attackHold;
    int attackHoldTime;
    vector3 shootAngles; // Angles to where bot is shooting - fixme: make he torso turn to reflect these

    // extra character info
    rank_e rank; // for pips

    // Behavior state info
    bState_e behaviorState;   // determines what actions he should be doing
    bState_e defaultBehavior; // State bot will default to if none other set
    bState_e tempBehavior;    // While valid, overrides other behavior

    qboolean ignorePain; // only play pain scripts when take pain

    int duckDebounceTime; // Keeps them ducked for a certain time
    int walkDebounceTime;
    int enemyCheckDebounceTime;
    int investigateDebounceTime;
    int investigateCount;
    vector3 investigateGoal;
    int investigateSoundDebounceTime;
    int greetingDebounceTime; // when we can greet someone next
    gentity_t *eventOwner;

    // bState-specific fields
    gentity_t *coverTarg;
    jumpState_e jumpState;
    float followDist;

    // goal, navigation & pathfinding
    gentity_t *tempGoal; // used for locational goals (player's last seen/heard position)
    gentity_t *goalEntity;
    gentity_t *lastGoalEntity;
    gentity_t *eventualGoal;
    gentity_t *captureGoal;  // Where we should try to capture
    gentity_t *defendEnt;    // Who we're trying to protect
    gentity_t *greetEnt;     // Who we're greeting
    int goalTime;            // FIXME: This is never actually used
    qboolean straightToGoal; // move straight at navgoals
    float distToGoal;
    int navTime;
    int blockingEntNum;
    int blockedSpeechDebounceTime;
    int lastSideStepSide;
    int sideStepHoldTime;
    int homeWp;
    AIGroupInfo_t *group;

    vector3 lastPathAngles; // So we know which way to face generally when we stop

    // stats
    gNPCstats_t stats;
    int aimErrorDebounceTime;
    float lastAimErrorYaw;
    float lastAimErrorPitch;
    vector3 aimOfs;
    int currentAim;
    int currentAggression;

    // scriptflags
    uint32_t scriptFlags; // in b_local.h

    // moveInfo
    int desiredSpeed;
    int currentSpeed;
    char last_forwardmove;
    char last_rightmove;
    vector3 lastClearOrigin;
    int consecutiveBlockedMoves;
    int blockedDebounceTime;
    int shoveCount;
    vector3 blockedDest;

    //
    int combatPoint;             // NPCs in bState BS_COMBAT_POINT will find their closest empty combat_point
    int lastFailedCombatPoint;   // NPCs in bState BS_COMBAT_POINT will find their closest empty combat_point
    speechType_e movementSpeech; // what to say when you first successfully move
    float movementSpeechChance;  // how likely you are to say it

    // Testing physics at 20fps
    int nextBStateThink;
    usercmd_t last_ucmd;

    //
    // JWEIER ADDITIONS START

    qboolean combatMove;
    int goalRadius;

    // FIXME: These may be redundant

    /*
    int			weaponTime;		//Time until refire is valid
    int			jumpTime;
    */
    int pauseTime; // Time to stand still
    int standTime;

    int localState; // Tracking information local to entity
    int squadState; // Tracking information for team level interaction

    // JWEIER ADDITIONS END
    //

    int confusionTime;  // Doesn't respond to alerts or pick up enemies (unless shot) until this time is up
    int charmedTime;    // charmed to enemy team
    int controlledTime; // controlled by player
    int surrenderTime;  // Hands up

    // Lagging enemy position - FIXME: seems awful wasteful...
    vector3 enemyLaggedPos[ENEMY_POS_LAG_STEPS];

    gentity_t *watchTarget; // for BS_CINEMATIC, keeps facing this ent

    int ffireCount; // sigh... you'd think I'd be able to find a way to do this without having to use 3 int fields, but...
    int ffireDebounce;
    int ffireFadeDebounce;
} gNPC_t;

void G_SquadPathsInit(void);
void NPC_InitGame(void);
void G_LoadBoltOns(void);
void Svcmd_NPC_f(void);
void NAV_DebugShowWaypoints(void);
void NAV_DebugShowBoxes(void);
void NAV_DebugShowSquadPaths(void);
