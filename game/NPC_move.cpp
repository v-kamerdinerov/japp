//
// NPC_move.cpp
//
#include "b_local.h"
#include "g_nav.h"
#include "anims.h"

void G_Cylinder(vector3 *start, vector3 *end, float radius, vector3 *color);

int NAV_Steer(gentity_t *self, vector3 *dir, float distance);
int GetTime(int lastTime);

navInfo_t frameNavInfo;
qboolean FlyingCreature(gentity_t *ent);
qboolean PM_InKnockDown(playerState_t *ps);

qboolean NPC_ClearPathToGoal(vector3 *dir, gentity_t *goal) {
    trace_t trace;
    float radius, dist, tFrac;

    // FIXME: What does do about area portals?  THIS IS BROKEN
    // if ( gi.inPVS( NPC->r.currentOrigin, goal->r.currentOrigin ) == qfalse )
    //	return qfalse;

    // Look ahead and see if we're clear to move to our goal position
    if (NAV_CheckAhead(NPC, &goal->r.currentOrigin, &trace, (NPC->clipmask & ~CONTENTS_BODY) | CONTENTS_BOTCLIP)) {
        // VectorSubtract( goal->r.currentOrigin, NPC->r.currentOrigin, dir );
        return qtrue;
    }

    if (!FlyingCreature(NPC)) {
        // See if we're too far above
        if (fabsf(NPC->r.currentOrigin.z - goal->r.currentOrigin.z) > 48)
            return qfalse;
    }

    // This is a work around
    radius = (NPC->r.maxs.x > NPC->r.maxs.y) ? NPC->r.maxs.x : NPC->r.maxs.y;
    dist = Distance(&NPC->r.currentOrigin, &goal->r.currentOrigin);
    tFrac = 1.0f - (radius / dist);

    if (trace.fraction >= tFrac)
        return qtrue;

    // See if we're looking for a navgoal
    if (goal->flags & FL_NAVGOAL) {
        // Okay, didn't get all the way there, let's see if we got close enough:
        if (NAV_HitNavGoal(&trace.endpos, &NPC->r.mins, &NPC->r.maxs, &goal->r.currentOrigin, NPCInfo->goalRadius, FlyingCreature(NPC))) {
            // VectorSubtract(goal->r.currentOrigin, NPC->r.currentOrigin, dir);
            return qtrue;
        }
    }

    return qfalse;
}

qboolean NPC_CheckCombatMove(void) {
    // return NPCInfo->combatMove;
    if ((NPCInfo->goalEntity && NPC->enemy && NPCInfo->goalEntity == NPC->enemy) || (NPCInfo->combatMove)) {
        return qtrue;
    }

    if (NPCInfo->goalEntity && NPCInfo->watchTarget) {
        if (NPCInfo->goalEntity != NPCInfo->watchTarget) {
            return qtrue;
        }
    }

    return qfalse;
}

static void NPC_LadderMove(vector3 *dir) {
    // FIXME: this doesn't guarantee we're facing ladder
    // ALSO: Need to be able to get off at top
    // ALSO: Need to play an anim
    // ALSO: Need transitionary anims?

    if ((dir->z > 0) || (dir->z < 0 && NPC->client->ps.groundEntityNum == ENTITYNUM_NONE)) {
        // Set our movement direction
        ucmd.upmove = (dir->z > 0) ? 127 : -127;

        // Don't move around on XY
        ucmd.forwardmove = ucmd.rightmove = 0;
    }
}

qboolean NPC_GetMoveInformation(vector3 *dir, float *distance) {
    // NOTENOTE: Use path stacks!

    // Make sure we have somewhere to go
    if (NPCInfo->goalEntity == NULL)
        return qfalse;

    // Get our move info
    VectorSubtract(&NPCInfo->goalEntity->r.currentOrigin, &NPC->r.currentOrigin, dir);
    *distance = VectorNormalize(dir);

    VectorCopy(&NPCInfo->goalEntity->r.currentOrigin, &NPCInfo->blockedDest);

    return qtrue;
}

void NAV_GetLastMove(navInfo_t *info) { *info = frameNavInfo; }

qboolean NPC_GetMoveDirection(vector3 *out, float *distance) {
    vector3 angles;

    // Clear the struct
    memset(&frameNavInfo, 0, sizeof(frameNavInfo));

    // Get our movement, if any
    if (NPC_GetMoveInformation(&frameNavInfo.direction, &frameNavInfo.distance) == qfalse)
        return qfalse;

    // Setup the return value
    *distance = frameNavInfo.distance;

    // For starters
    VectorCopy(&frameNavInfo.direction, &frameNavInfo.pathDirection);

    // If on a ladder, move appropriately
    if (NPC->watertype & CONTENTS_LADDER) {
        NPC_LadderMove(&frameNavInfo.direction);
        return qtrue;
    }

    // Attempt a straight move to goal
    if (NPC_ClearPathToGoal(&frameNavInfo.direction, NPCInfo->goalEntity) == qfalse) {
        // See if we're just stuck
        if (NAV_MoveToGoal(NPC, &frameNavInfo) == WAYPOINT_NONE) {
            // Can't reach goal, just face
            vectoangles(&frameNavInfo.direction, &angles);
            NPCInfo->desiredYaw = AngleNormalize360(angles.yaw);
            VectorCopy(&frameNavInfo.direction, out);
            *distance = frameNavInfo.distance;
            return qfalse;
        }

        frameNavInfo.flags |= NIF_MACRO_NAV;
    }

    // Avoid any collisions on the way
    if (NAV_AvoidCollision(NPC, NPCInfo->goalEntity, &frameNavInfo) == qfalse) {
        // FIXME: Emit a warning, this is a worst case scenario
        // FIXME: if we have a clear path to our goal (exluding bodies), but then this
        //			check (against bodies only) fails, shouldn't we fall back
        //			to macro navigation?  Like so:
        if (!(frameNavInfo.flags & NIF_MACRO_NAV)) { // we had a clear path to goal and didn't try macro nav, but can't avoid collision so try macro nav here
            // See if we're just stuck
            if (NAV_MoveToGoal(NPC, &frameNavInfo) == WAYPOINT_NONE) {
                // Can't reach goal, just face
                vectoangles(&frameNavInfo.direction, &angles);
                NPCInfo->desiredYaw = AngleNormalize360(angles.yaw);
                VectorCopy(&frameNavInfo.direction, out);
                *distance = frameNavInfo.distance;
                return qfalse;
            }

            frameNavInfo.flags |= NIF_MACRO_NAV;
        }
    }

    // Setup the return values
    VectorCopy(&frameNavInfo.direction, out);
    *distance = frameNavInfo.distance;

    return qtrue;
}

int NAVNEW_MoveToGoal(gentity_t *self, navInfo_t *info);
qboolean NAVNEW_AvoidCollision(gentity_t *self, gentity_t *goal, navInfo_t *info, qboolean setBlockedInfo, int blockedMovesLimit);

qboolean NPC_GetMoveDirectionAltRoute(vector3 *out, float *distance, qboolean tryStraight) {
    vector3 angles;

    NPCInfo->aiFlags &= ~NPCAI_BLOCKED;

    // Clear the struct
    memset(&frameNavInfo, 0, sizeof(frameNavInfo));

    // Get our movement, if any
    if (NPC_GetMoveInformation(&frameNavInfo.direction, &frameNavInfo.distance) == qfalse)
        return qfalse;

    // Setup the return value
    *distance = frameNavInfo.distance;

    // For starters
    VectorCopy(&frameNavInfo.direction, &frameNavInfo.pathDirection);

    // If on a ladder, move appropriately
    if (NPC->watertype & CONTENTS_LADDER) {
        NPC_LadderMove(&frameNavInfo.direction);
        return qtrue;
    }

    // Attempt a straight move to goal
    if (!tryStraight || NPC_ClearPathToGoal(&frameNavInfo.direction, NPCInfo->goalEntity) == qfalse) { // blocked
        // Can't get straight to goal, use macro nav
        if (NAVNEW_MoveToGoal(NPC, &frameNavInfo) == WAYPOINT_NONE) {
            // Can't reach goal, just face
            vectoangles(&frameNavInfo.direction, &angles);
            NPCInfo->desiredYaw = AngleNormalize360(angles.yaw);
            VectorCopy(&frameNavInfo.direction, out);
            *distance = frameNavInfo.distance;
            return qfalse;
        }
        // else we are on our way
        frameNavInfo.flags |= NIF_MACRO_NAV;
    } else { // we have no architectural problems, see if there are ents inthe way and try to go around them
        // not blocked
        if (d_altRoutes.integer) { // try macro nav
            navInfo_t tempInfo;
            memcpy(&tempInfo, &frameNavInfo, sizeof(tempInfo));
            if (NAVNEW_AvoidCollision(NPC, NPCInfo->goalEntity, &tempInfo, qtrue, 5) == qfalse) { // revert to macro nav
                // Can't get straight to goal, dump tempInfo and use macro nav
                if (NAVNEW_MoveToGoal(NPC, &frameNavInfo) == WAYPOINT_NONE) {
                    // Can't reach goal, just face
                    vectoangles(&frameNavInfo.direction, &angles);
                    NPCInfo->desiredYaw = AngleNormalize360(angles.yaw);
                    VectorCopy(&frameNavInfo.direction, out);
                    *distance = frameNavInfo.distance;
                    return qfalse;
                }
                // else we are on our way
                frameNavInfo.flags |= NIF_MACRO_NAV;
            } else { // otherwise, either clear or can avoid
                memcpy(&frameNavInfo, &tempInfo, sizeof(frameNavInfo));
            }
        } else {                                                                                       // OR: just give up
            if (NAVNEW_AvoidCollision(NPC, NPCInfo->goalEntity, &frameNavInfo, qtrue, 30) == qfalse) { // give up
                return qfalse;
            }
        }
    }

    // Setup the return values
    VectorCopy(&frameNavInfo.direction, out);
    *distance = frameNavInfo.distance;

    return qtrue;
}

void G_UcmdMoveForDir(gentity_t *self, usercmd_t *cmd, vector3 *dir) {
    vector3 forward, right;
    float fDot, rDot;

    AngleVectors(&self->r.currentAngles, &forward, &right, NULL);

    dir->z = 0;
    VectorNormalize(dir);
    // NPCs cheat and store this directly because converting movement into a ucmd loses precision
    VectorCopy(dir, &self->client->ps.moveDir);

    fDot = DotProduct(&forward, dir) * 127.0f;
    rDot = DotProduct(&right, dir) * 127.0f;
    // Must clamp this because DotProduct is not guaranteed to return a number within -1 to 1, and that would be bad when we're shoving this into a signed byte
    if (fDot > 127.0f)
        fDot = 127.0f;
    if (fDot < -127.0f)
        fDot = -127.0f;
    if (rDot > 127.0f)
        rDot = 127.0f;
    if (rDot < -127.0f)
        rDot = -127.0f;
    cmd->forwardmove = floorf(fDot);
    cmd->rightmove = floorf(rDot);

    /*
    vector3	wishvel;
    for ( int i = 0 ; i < 3 ; i++ )
    {
    wishvel[i] = forward[i]*cmd->forwardmove + right[i]*cmd->rightmove;
    }
    VectorNormalize( wishvel );
    if ( !VectorCompare( wishvel, dir ) )
    {
    Com_Printf( "PRECISION LOSS: %s != %s\n", vtos(wishvel), vtos(dir) );
    }
    */
}

#if AI_TIMERS
extern int navTime;
#endif //	AI_TIMERS

// Now assumes goal is goalEntity, was no reason for it to be otherwise
qboolean NPC_MoveToGoal(qboolean tryStraight) {
    float distance;
    vector3 dir;

#if AI_TIMERS
    int startTime = GetTime(0);
#endif //	AI_TIMERS
       // If taking full body pain, don't move
    if (PM_InKnockDown(&NPC->client->ps) || ((NPC->s.legsAnim >= BOTH_PAIN1) && (NPC->s.legsAnim <= BOTH_PAIN18))) {
        return qtrue;
    }

    /*
    if( NPC->s.eFlags & EF_LOCKED_TO_WEAPON )
    {//If in an emplaced gun, never try to navigate!
    return qtrue;
    }
    */
    // rwwFIXMEFIXME: emplaced support

    // FIXME: if can't get to goal & goal is a target (enemy), try to find a waypoint that has line of sight to target, at least?
    // Get our movement direction
#if 1
    if (NPC_GetMoveDirectionAltRoute(&dir, &distance, tryStraight) == qfalse)
#else
    if (NPC_GetMoveDirection(dir, &distance) == qfalse)
#endif
        return qfalse;

    NPCInfo->distToGoal = distance;

    // Convert the move to angles
    vectoangles(&dir, &NPCInfo->lastPathAngles);
    if ((ucmd.buttons & BUTTON_WALKING)) {
        NPC->client->ps.speed = NPCInfo->stats.walkSpeed;
    } else {
        NPC->client->ps.speed = NPCInfo->stats.runSpeed;
    }

    // FIXME: still getting ping-ponging in certain cases... !!!  Nav/avoidance error?  WTF???!!!
    // If in combat move, then move directly towards our goal
    if (NPC_CheckCombatMove()) { // keep current facing
        G_UcmdMoveForDir(NPC, &ucmd, &dir);
    } else { // face our goal
        // FIXME: strafe instead of turn if change in dir is small and temporary
        NPCInfo->desiredPitch = 0.0f;
        NPCInfo->desiredYaw = AngleNormalize360(NPCInfo->lastPathAngles.yaw);

        // Pitch towards the goal and also update if flying or swimming
        if ((NPC->client->ps.eFlags2 & EF2_FLYING)) // moveType == MT_FLYSWIM )
        {
            NPCInfo->desiredPitch = AngleNormalize360(NPCInfo->lastPathAngles.pitch);

            if (dir.z) {
                float scale = (dir.z * distance);
                if (scale > 64)
                    scale = 64;
                else if (scale < -64)
                    scale = -64;
                NPC->client->ps.velocity.z = scale;
                // NPC->client->ps.velocity[2] = (dir[2] > 0) ? 64 : -64;
            }
        }

        // Set any final info
        ucmd.forwardmove = 127;
    }

#if AI_TIMERS
    navTime += GetTime(startTime);
#endif //	AI_TIMERS
    return qtrue;
}

// Now assumes goal is goalEntity, if want to use tempGoal, you set that before calling the func
qboolean NPC_SlideMoveToGoal(void) {
    float saveYaw = NPC->client->ps.viewangles.yaw;
    qboolean ret;

    NPCInfo->combatMove = qtrue;

    ret = NPC_MoveToGoal(qtrue);

    NPCInfo->desiredYaw = saveYaw;

    return ret;
}

void NPC_ApplyRoff(void) {
    BG_PlayerStateToEntityState(&NPC->client->ps, &NPC->s, qfalse);
    // VectorCopy ( NPC->r.currentOrigin, NPC->lastOrigin );
    // rwwFIXMEFIXME: Any significance to this?

    // use the precise origin for linking
    trap->LinkEntity((sharedEntity_t *)NPC);
}
