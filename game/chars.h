// Copyright (C) 1999-2000 Id Software, Inc.
//
// Name:			chars.h
// Function:		bot characteristics
// Programmer:		Mr Elusive (MrElusive@idsoftware.com)
// Last update:		1999-09-08

// name
#define CHARACTERISTIC_NAME 0 // string
// gender of the bot
#define CHARACTERISTIC_GENDER 1 // string ("male", "female", "it")
// attack skill
//  >  0.0f && <  0.2f = don't move
//  >  0.3f && <  1.0f = aim at enemy during retreat
//  >  0.0f && <  0.4f = only move forward/backward
//  >= 0.4f && <  1.0f = circle strafing
//  >  0.7f && <  1.0f = random strafe direction change
#define CHARACTERISTIC_ATTACK_SKILL 2 // float [0, 1]
// weapon weight file
#define CHARACTERISTIC_WEAPONWEIGHTS 3 // string
// view angle difference to angle change factor
#define CHARACTERISTIC_VIEW_FACTOR 4 // float <0, 1]
// maximum view angle change
#define CHARACTERISTIC_VIEW_MAXCHANGE 5 // float [1, 360]
// reaction time in seconds
#define CHARACTERISTIC_REACTIONTIME 6 // float [0, 5]
// accuracy when aiming
#define CHARACTERISTIC_AIM_ACCURACY 7 // float [0, 1]
// weapon specific aim accuracy
#define CHARACTERISTIC_AIM_ACCURACY_MACHINEGUN 8       // float [0, 1]
#define CHARACTERISTIC_AIM_ACCURACY_SHOTGUN 9          // float [0, 1]
#define CHARACTERISTIC_AIM_ACCURACY_ROCKETLAUNCHER 10  // float [0, 1]
#define CHARACTERISTIC_AIM_ACCURACY_GRENADELAUNCHER 11 // float [0, 1]
#define CHARACTERISTIC_AIM_ACCURACY_LIGHTNING 12
#define CHARACTERISTIC_AIM_ACCURACY_PLASMAGUN 13 // float [0, 1]
#define CHARACTERISTIC_AIM_ACCURACY_RAILGUN 14
#define CHARACTERISTIC_AIM_ACCURACY_BFG10K 15 // float [0, 1]
// skill when aiming
//  >  0.0f && <  0.9f = aim is affected by enemy movement
//  >  0.4f && <= 0.8f = enemy linear leading
//  >  0.8f && <= 1.0f = enemy exact movement leading
//  >  0.5f && <= 1.0f = prediction shots when enemy is not visible
//  >  0.6f && <= 1.0f = splash damage by shooting nearby geometry
#define CHARACTERISTIC_AIM_SKILL 16 // float [0, 1]
// weapon specific aim skill
#define CHARACTERISTIC_AIM_SKILL_ROCKETLAUNCHER 17  // float [0, 1]
#define CHARACTERISTIC_AIM_SKILL_GRENADELAUNCHER 18 // float [0, 1]
#define CHARACTERISTIC_AIM_SKILL_PLASMAGUN 19       // float [0, 1]
#define CHARACTERISTIC_AIM_SKILL_BFG10K 20          // float [0, 1]

// chat
// file with chats
#define CHARACTERISTIC_CHAT_FILE 21 // string
// name of the chat character
#define CHARACTERISTIC_CHAT_NAME 22 // string
// characters per minute type speed
#define CHARACTERISTIC_CHAT_CPM 23 // integer [1, 4000]
// tendency to insult/praise
#define CHARACTERISTIC_CHAT_INSULT 24 // float [0, 1]
// tendency to chat misc
#define CHARACTERISTIC_CHAT_MISC 25 // float [0, 1]
// tendency to chat at start or end of level
#define CHARACTERISTIC_CHAT_STARTENDLEVEL 26 // float [0, 1]
// tendency to chat entering or exiting the game
#define CHARACTERISTIC_CHAT_ENTEREXITGAME 27 // float [0, 1]
// tendency to chat when killed someone
#define CHARACTERISTIC_CHAT_KILL 28 // float [0, 1]
// tendency to chat when died
#define CHARACTERISTIC_CHAT_DEATH 29 // float [0, 1]
// tendency to chat when enemy suicides
#define CHARACTERISTIC_CHAT_ENEMYSUICIDE 30 // float [0, 1]
// tendency to chat when hit while talking
#define CHARACTERISTIC_CHAT_HITTALKING 31 // float [0, 1]
// tendency to chat when bot was hit but didn't dye
#define CHARACTERISTIC_CHAT_HITNODEATH 32 // float [0, 1]
// tendency to chat when bot hit the enemy but enemy didn't dye
#define CHARACTERISTIC_CHAT_HITNOKILL 33 // float [0, 1]
// tendency to randomly chat
#define CHARACTERISTIC_CHAT_RANDOM 34 // float [0, 1]
// tendency to reply
#define CHARACTERISTIC_CHAT_REPLY 35 // float [0, 1]

// movement
// tendency to crouch
#define CHARACTERISTIC_CROUCHER 36 // float [0, 1]
// tendency to jump
#define CHARACTERISTIC_JUMPER 37 // float [0, 1]
// tendency to walk
#define CHARACTERISTIC_WALKER 48 // float [0, 1]
// tendency to jump using a weapon
#define CHARACTERISTIC_WEAPONJUMPING 38 // float [0, 1]
// tendency to use the grapple hook when available
#define CHARACTERISTIC_GRAPPLE_USER 39 // float [0, 1]	//use this!!

// goal
// item weight file
#define CHARACTERISTIC_ITEMWEIGHTS 40 // string
// the aggression of the bot
#define CHARACTERISTIC_AGGRESSION 41 // float [0, 1]
// the self preservation of the bot (rockets near walls etc.)
#define CHARACTERISTIC_SELFPRESERVATION 42 // float [0, 1]
// how likely the bot is to take revenge
#define CHARACTERISTIC_VENGEFULNESS 43 // float [0, 1]	//use this!!
// tendency to camp
#define CHARACTERISTIC_CAMPER 44 // float [0, 1]
// tendency to get easy frags
#define CHARACTERISTIC_EASY_FRAGGER 45 // float [0, 1]
// how alert the bot is (view distance)
#define CHARACTERISTIC_ALERTNESS 46 // float [0, 1]
// how much the bot fires it's weapon
#define CHARACTERISTIC_FIRETHROTTLE 47 // float [0, 1]
