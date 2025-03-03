// Copyright (C) 1999-2000 Id Software, Inc.
//
// cg_effects.c -- these functions generate localentities, usually as a result
// of event processing

#include "cg_local.h"
#include "cg_media.h"

// Bullets shot underwater
void CG_BubbleTrail(vector3 *start, vector3 *end, float spacing) {
    vector3 move;
    vector3 vec;
    float len;
    int i;

#if 0
	if ( cg_noProjectileTrail.integer ) {
		return;
	}
#endif

    VectorCopy(start, &move);
    VectorSubtract(end, start, &vec);
    len = VectorNormalize(&vec);

    // advance a random amount first
    i = rand() % (int)spacing;
    VectorMA(&move, i, &vec, &move);

    VectorScale(&vec, spacing, &vec);

    for (; i < len; i += spacing) {
        localEntity_t *le;
        refEntity_t *re;

        le = CG_AllocLocalEntity();
        le->leFlags = LEF_PUFF_DONT_SCALE;
        le->leType = LE_MOVE_SCALE_FADE;
        le->startTime = cg.time;
        le->endTime = cg.time + 1000 + random() * 250;
        le->lifeRate = 1.0f / (le->endTime - le->startTime);

        re = &le->refEntity;
        re->shaderTime = cg.time / 1000.0f;

        re->reType = RT_SPRITE;
        re->rotation = 0;
        re->radius = 3;
        re->customShader = 0; // media.gfx.world.waterBubble;
        re->shaderRGBA[0] = 0xff;
        re->shaderRGBA[1] = 0xff;
        re->shaderRGBA[2] = 0xff;
        re->shaderRGBA[3] = 0xff;

        le->color[3] = 1.0f;

        le->pos.trType = TR_LINEAR;
        le->pos.trTime = cg.time;
        VectorCopy(&move, &le->pos.trBase);
        le->pos.trDelta.x = crandom() * 5;
        le->pos.trDelta.y = crandom() * 5;
        le->pos.trDelta.z = crandom() * 5 + 6;

        VectorAdd(&move, &vec, &move);
    }
}

// Adds a smoke puff or blood trail localEntity.
localEntity_t *CG_SmokePuff(const vector3 *p, const vector3 *vel, float radius, float r, float g, float b, float a, float duration, int startTime,
                            int fadeInTime, uint32_t leFlags, qhandle_t hShader) {
    static int seed = 0x92;
    localEntity_t *le;
    refEntity_t *re;
    //	int fadeInTime = startTime + duration / 2;

    le = CG_AllocLocalEntity();
    le->leFlags = leFlags;
    le->radius = radius;

    re = &le->refEntity;
    re->rotation = Q_random(&seed) * 360;
    re->radius = radius;
    re->shaderTime = startTime / 1000.0f;

    le->leType = LE_MOVE_SCALE_FADE;
    le->startTime = startTime;
    le->fadeInTime = fadeInTime;
    le->endTime = startTime + duration;
    if (fadeInTime > startTime) {
        le->lifeRate = 1.0f / (le->endTime - le->fadeInTime);
    } else {
        le->lifeRate = 1.0f / (le->endTime - le->startTime);
    }
    le->color[0] = r;
    le->color[1] = g;
    le->color[2] = b;
    le->color[3] = a;

    le->pos.trType = TR_LINEAR;
    le->pos.trTime = startTime;
    VectorCopy(vel, &le->pos.trDelta);
    VectorCopy(p, &le->pos.trBase);

    VectorCopy(p, &re->origin);
    re->customShader = hShader;

    re->shaderRGBA[0] = le->color[0] * 0xff;
    re->shaderRGBA[1] = le->color[1] * 0xff;
    re->shaderRGBA[2] = le->color[2] * 0xff;
    re->shaderRGBA[3] = 0xff;

    re->reType = RT_SPRITE;
    re->radius = le->radius;

    return le;
}

void CG_TestLine(vector3 *start, vector3 *end, int time, uint32_t color, int radius) {
    localEntity_t *le;
    refEntity_t *re;

    le = CG_AllocLocalEntity();
    le->leType = LE_LINE;
    le->startTime = cg.time;
    le->endTime = cg.time + time;
    le->lifeRate = 1.0f / (le->endTime - le->startTime);

    re = &le->refEntity;
    VectorCopy(start, &re->origin);
    VectorCopy(end, &re->oldorigin);
    re->shaderTime = cg.time / 1000.0f;

    re->reType = RT_LINE;
    re->radius = 0.5f * radius;
    re->customShader = media.gfx.world.whiteShader; // trap->R_RegisterShaderNoMip("textures/colombia/canvas_doublesided");

    re->shaderTexCoord.x = re->shaderTexCoord.y = 1.0f;

    re->shaderRGBA[2] = color & 0xff;
    color >>= 8;
    re->shaderRGBA[1] = color & 0xff;
    color >>= 8;
    re->shaderRGBA[0] = color & 0xff;
    re->shaderRGBA[3] = 0xff;

    le->color[3] = 1.0f;

    // re->renderfx |= RF_DEPTHHACK;
}

void CG_ThrowChunk(vector3 *origin, vector3 *velocity, qhandle_t hModel, int optionalSound, int startalpha) {
    localEntity_t *le;
    refEntity_t *re;

    le = CG_AllocLocalEntity();
    re = &le->refEntity;

    le->leType = LE_FRAGMENT;
    le->startTime = cg.time;
    le->endTime = le->startTime + 5000 + random() * 3000;

    VectorCopy(origin, &re->origin);
    AxisCopy(axisDefault, re->axis);
    re->hModel = hModel;

    le->pos.trType = TR_GRAVITY;
    le->angles.trType = TR_GRAVITY;
    VectorCopy(origin, &le->pos.trBase);
    VectorCopy(velocity, &le->pos.trDelta);
    VectorSet(&le->angles.trBase, 20, 20, 20);
    VectorCopy(velocity, &le->angles.trDelta);
    le->pos.trTime = cg.time;
    le->angles.trTime = cg.time;

    le->leFlags = LEF_TUMBLE;

    le->angles.trBase.yaw = 180;

    le->bounceFactor = 0.3f;
    le->bounceSound = optionalSound;

    le->forceAlpha = startalpha;
}

//----------------------------
//
// Breaking Glass Technology
//
//----------------------------

// Since we have shared verts when we tesselate the glass sheet, it helps to have a
//	random offset table set up up front.

static float offX[20][20], offZ[20][20];

#define FX_ALPHA_NONLINEAR 0x00000004
#define FX_APPLY_PHYSICS 0x02000000
#define FX_USE_ALPHA 0x08000000

static void CG_DoGlassQuad(vector3 p[4], vector2 uv[4], qboolean stick, int time, vector3 *dmgDir) {
    float bounce;
    vector3 rotDelta;
    vector3 vel, accel;
    vector3 rgb1;
    addpolyArgStruct_t apArgs;
    int i, i_2;

    VectorSet(&vel, crandom() * 12, crandom() * 12, -1);

    if (!stick) {
        // We aren't a motion delayed chunk, so let us move quickly
        VectorMA(&vel, 0.3f, dmgDir, &vel);
    }

    // Set up acceleration due to gravity, 800 is standard QuakeIII gravity, so let's use something close
    VectorSet(&accel, 0.0f, 0.0f, -(600.0f + random() * 100.0f));

    // We are using an additive shader, so let's set the RGB low so we look more like transparent glass
    //	VectorSet( rgb1, 0.1f, 0.1f, 0.1f );
    VectorSet(&rgb1, 1.0f, 1.0f, 1.0f);

    // Being glass, we don't want to bounce much
    bounce = random() * 0.2f + 0.15f;

    // Set up our random rotate, we only do PITCH and YAW, not ROLL.  This is something like degrees per second
    VectorSet(&rotDelta, crandom() * 40.0f, crandom() * 40.0f, 0.0f);

    // In an ideal world, this might actually work.
    /*
    CPoly *pol = FX_AddPoly(p, uv, 4,			// verts, ST, vertCount
    vel, accel,				// motion
    0.15f, 0.0f, 85.0f,		// alpha start, alpha end, alpha parm ( begin alpha fade when 85% of life is complete )
    rgb1, rgb1, 0.0f,		// rgb start, rgb end, rgb parm ( not used )
    rotDelta, bounce, time,	// rotation amount, bounce, and time to delay motion for ( zero if no delay );
    6000,					// life
    cgi_R_RegisterShader( "gfx/misc/test_crackle" ),
    FX_APPLY_PHYSICS | FX_ALPHA_NONLINEAR | FX_USE_ALPHA );

    if ( random() > 0.95f && pol )
    {
    pol->AddFlags( FX_IMPACT_RUNS_FX | FX_KILL_ON_IMPACT );
    pol->SetImpactFxID( theFxScheduler.RegisterEffect( "glass_impact" ));
    }
    */

    // rww - this is dirty.

    i = 0;
    i_2 = 0;

    while (i < 4) {
        while (i_2 < 3) {
            apArgs.p[i].raw[i_2] = p[i].raw[i_2];

            i_2++;
        }

        i_2 = 0;
        i++;
    }

    i = 0;
    i_2 = 0;

    while (i < 4) {
        while (i_2 < 2) {
            apArgs.ev[i].raw[i_2] = uv[i].raw[i_2];

            i_2++;
        }

        i_2 = 0;
        i++;
    }

    apArgs.numVerts = 4;
    VectorCopy(&vel, &apArgs.vel);
    VectorCopy(&accel, &apArgs.accel);

    apArgs.alpha1 = 0.15f;
    apArgs.alpha2 = 0.0f;
    apArgs.alphaParm = 85.0f;

    VectorCopy(&rgb1, &apArgs.rgb1);
    VectorCopy(&rgb1, &apArgs.rgb2);

    apArgs.rgbParm = 0.0f;

    VectorCopy(&rotDelta, &apArgs.rotationDelta);

    apArgs.bounce = bounce;
    apArgs.motionDelay = time;
    apArgs.killTime = 6000;
    apArgs.shader = media.gfx.world.glassShard;
    apArgs.flags = (FX_APPLY_PHYSICS | FX_ALPHA_NONLINEAR | FX_USE_ALPHA);

    trap->FX_AddPoly(&apArgs);
}

static void CG_CalcBiLerp(vector3 verts[4], vector3 subVerts[4], vector2 uv[4]) {
    vector3 temp;

    // Nasty crap
    VectorScale(&verts[0], 1.0f - uv[0].x, &subVerts[0]);
    VectorMA(&subVerts[0], uv[0].x, &verts[1], &subVerts[0]);
    VectorScale(&subVerts[0], 1.0f - uv[0].y, &temp);
    VectorScale(&verts[3], 1.0f - uv[0].x, &subVerts[0]);
    VectorMA(&subVerts[0], uv[0].x, &verts[2], &subVerts[0]);
    VectorMA(&temp, uv[0].y, &subVerts[0], &subVerts[0]);

    VectorScale(&verts[0], 1.0f - uv[1].x, &subVerts[1]);
    VectorMA(&subVerts[1], uv[1].x, &verts[1], &subVerts[1]);
    VectorScale(&subVerts[1], 1.0f - uv[1].y, &temp);
    VectorScale(&verts[3], 1.0f - uv[1].x, &subVerts[1]);
    VectorMA(&subVerts[1], uv[1].x, &verts[2], &subVerts[1]);
    VectorMA(&temp, uv[1].y, &subVerts[1], &subVerts[1]);

    VectorScale(&verts[0], 1.0f - uv[2].x, &subVerts[2]);
    VectorMA(&subVerts[2], uv[2].x, &verts[1], &subVerts[2]);
    VectorScale(&subVerts[2], 1.0f - uv[2].y, &temp);
    VectorScale(&verts[3], 1.0f - uv[2].x, &subVerts[2]);
    VectorMA(&subVerts[2], uv[2].x, &verts[2], &subVerts[2]);
    VectorMA(&temp, uv[2].y, &subVerts[2], &subVerts[2]);

    VectorScale(&verts[0], 1.0f - uv[3].x, &subVerts[3]);
    VectorMA(&subVerts[3], uv[3].x, &verts[1], &subVerts[3]);
    VectorScale(&subVerts[3], 1.0f - uv[3].y, &temp);
    VectorScale(&verts[3], 1.0f - uv[3].x, &subVerts[3]);
    VectorMA(&subVerts[3], uv[3].x, &verts[2], &subVerts[3]);
    VectorMA(&temp, uv[3].y, &subVerts[3], &subVerts[3]);
}
// bilinear
// f(p',q') = (1 - y) × {[(1 - x) × f(p,q)] + [x × f(p,q+1)]} + y × {[(1 - x) × f(p+1,q)] + [x × f(p+1,q+1)]}.

static void CG_CalcHeightWidth(vector3 verts[4], float *height, float *width) {
    vector3 dir1, dir2, cross;

    VectorSubtract(&verts[3], &verts[0], &dir1); // v
    VectorSubtract(&verts[1], &verts[0], &dir2); // p-a
    CrossProduct(&dir1, &dir2, &cross);
    *width = VectorNormalize(&cross) / VectorNormalize(&dir1); // v
    VectorSubtract(&verts[2], &verts[0], &dir2);               // p-a
    CrossProduct(&dir1, &dir2, &cross);
    *width += VectorNormalize(&cross) / VectorNormalize(&dir1); // v
    *width *= 0.5f;

    VectorSubtract(&verts[1], &verts[0], &dir1); // v
    VectorSubtract(&verts[2], &verts[0], &dir2); // p-a
    CrossProduct(&dir1, &dir2, &cross);
    *height = VectorNormalize(&cross) / VectorNormalize(&dir1); // v
    VectorSubtract(&verts[3], &verts[0], &dir2);                // p-a
    CrossProduct(&dir1, &dir2, &cross);
    *height += VectorNormalize(&cross) / VectorNormalize(&dir1); // v
    *height *= 0.5f;
}
// Consider a line in 3D with position vector "a" and direction vector "v" and
//  let "p" be the position vector of an arbitrary point in 3D
// dist = len( crossprod(p-a,v) ) / len(v);

void CG_InitGlass(void) {
    int i, t;

    // Build a table first, so that we can do a more unpredictable crack scheme
    //	do it once, up front to save a bit of time.
    for (i = 0; i < 20; i++) {
        for (t = 0; t < 20; t++) {
            offX[t][i] = crandom() * 0.03f;
            offZ[i][t] = crandom() * 0.03f;
        }
    }
}

void Vector2Set(vector2 *a, float b, float c) {
    a->x = b;
    a->y = c;
}

#define TIME_DECAY_SLOW 0.1f
#define TIME_DECAY_MED 0.04f
#define TIME_DECAY_FAST 0.009f

void CG_DoGlass(vector3 verts[4], vector3 *normal, vector3 *dmgPt, vector3 *dmgDir, float dmgRadius, int maxShards) {
    int i, t;
    int mxHeight, mxWidth;
    float height, width;
    float stepWidth, stepHeight;
    float timeDecay;
    float x, z;
    float xx, zz;
    float dif;
    int time = 0;
    int glassShards = 0;
    qboolean stick = qtrue;
    vector3 subVerts[4];
    vector2 biPoints[4];

    // To do a smarter tesselation, we should figure out the relative height and width of the brush face,
    //	then use this to pick a lod value from 1-3 in each axis.  This will give us 1-9 lod levels, which will
    //	hopefully be sufficient.
    CG_CalcHeightWidth(verts, &height, &width);

    trap->S_StartSound(dmgPt, -1, CHAN_AUTO, trap->S_RegisterSound("sound/effects/glassbreak1.wav"));

    // Pick "LOD" for height
    if (height < 100) {
        stepHeight = 0.2f;
        mxHeight = 5;
        timeDecay = TIME_DECAY_SLOW;
    } else if (height > 220) {
        stepHeight = 0.05f;
        mxHeight = 20;
        timeDecay = TIME_DECAY_FAST;
    } else {
        stepHeight = 0.1f;
        mxHeight = 10;
        timeDecay = TIME_DECAY_MED;
    }

    // Pick "LOD" for width
    /*
    if ( width < 100 )
    {
    stepWidth = 0.2f;
    mxWidth = 5;
    timeDecay = ( timeDecay + TIME_DECAY_SLOW ) * 0.5f;
    }
    else if ( width > 220 )
    {
    stepWidth = 0.05f;
    mxWidth = 20;
    timeDecay = ( timeDecay + TIME_DECAY_FAST ) * 0.5f;
    }
    else
    {
    stepWidth = 0.1f;
    mxWidth = 10;
    timeDecay = ( timeDecay + TIME_DECAY_MED ) * 0.5f;
    }
    */

    // Attempt to scale the glass directly to the size of the window

    stepWidth = (0.25f - (width * 0.0002f)); //(width*0.0005f));
    mxWidth = width * 0.2f;
    timeDecay = (timeDecay + TIME_DECAY_FAST) * 0.5f;

    if (stepWidth < 0.01f) {
        stepWidth = 0.01f;
    }
    if (mxWidth < 5) {
        mxWidth = 5;
    }

    for (z = 0.0f, i = 0; z < 1.0f; z += stepHeight, i++) {
        for (x = 0.0f, t = 0; x < 1.0f; x += stepWidth, t++) {
            // This is nasty..
            if (t > 0 && t < mxWidth) {
                xx = x - offX[i][t];
            } else {
                xx = x;
            }

            if (i > 0 && i < mxHeight) {
                zz = z - offZ[t][i];
            } else {
                zz = z;
            }

            Vector2Set(&biPoints[0], xx, zz);

            if (t + 1 > 0 && t + 1 < mxWidth) {
                xx = x - offX[i][t + 1];
            } else {
                xx = x;
            }

            if (i > 0 && i < mxHeight) {
                zz = z - offZ[t + 1][i];
            } else {
                zz = z;
            }

            Vector2Set(&biPoints[1], xx + stepWidth, zz);

            if (i + 1 > 0 && i + 1 < mxHeight && t + 1 > 0 && t + 1 < mxWidth) {
                xx = x - offX[i + 1][t + 1];
            } else {
                xx = x;
            }

            if (i + 1 > 0 && i + 1 < mxHeight) {
                zz = z - offZ[t + 1][i + 1];
            } else {
                zz = z;
            }

            Vector2Set(&biPoints[2], xx + stepWidth, zz + stepHeight);

            if (i + 1 > 0 && i + 1 < mxHeight && t > 0 && t < mxWidth) {
                xx = x - offX[i + 1][t];
            } else {
                xx = x;
            }

            if (i + 1 > 0 && i + 1 < mxHeight) {
                zz = z - offZ[t][i + 1];
            } else {
                zz = z;
            }

            Vector2Set(&biPoints[3], xx, zz + stepHeight);

            CG_CalcBiLerp(verts, subVerts, biPoints);

            dif = DistanceSquared(&subVerts[0], dmgPt) * timeDecay - random() * 32;

            // If we decrease dif, we are increasing the impact area, making it more likely to blow out large holes
            dif -= dmgRadius * dmgRadius;

            if (dif > 1) {
                stick = qtrue;
                time = dif + random() * 200;
            } else {
                stick = qfalse;
                time = 0;
            }

            CG_DoGlassQuad(subVerts, biPoints, stick, time, dmgDir);
            glassShards++;

            if (maxShards && glassShards >= maxShards) {
                return;
            }
        }
    }
}

// Break glass with fancy method
void CG_GlassShatter(int entnum, vector3 *dmgPt, vector3 *dmgDir, float dmgRadius, int maxShards) {
    vector3 verts[4], normal;

    if (cgs.inlineDrawModel[cg_entities[entnum].currentState.modelindex]) {
        trap->R_GetBModelVerts(cgs.inlineDrawModel[cg_entities[entnum].currentState.modelindex], verts, &normal);
        CG_DoGlass(verts, &normal, dmgPt, dmgDir, dmgRadius, maxShards);
    }
    // otherwise something awful has happened.
}

// Throws glass shards from within a given bounding box in the world
void CG_GlassShatter_Old(int entnum, vector3 *org, vector3 *mins, vector3 *maxs) {
    vector3 velocity, a, shardorg, dif, difx;
    float windowmass;
    float shardsthrow = 0;
    char chunkname[256];

    trap->S_StartSound(org, entnum, CHAN_BODY, trap->S_RegisterSound("sound/effects/glassbreak1.wav"));

    VectorSubtract(maxs, mins, &a);

    windowmass = VectorLength(&a); // should give us some idea of how big the chunk of glass is

    while (shardsthrow < windowmass) {
        velocity.x = crandom() * 150;
        velocity.y = crandom() * 150;
        velocity.z = 150 + crandom() * 75;

        Com_sprintf(chunkname, sizeof(chunkname), "models/chunks/glass/glchunks_%i.md3", Q_irand(1, 6));
        VectorCopy(org, &shardorg);

        dif.x = (maxs->x - mins->x) / 2;
        dif.y = (maxs->y - mins->y) / 2;
        dif.z = (maxs->z - mins->z) / 2;

        if (dif.x < 2)
            dif.x = 2;
        if (dif.y < 2)
            dif.y = 2;
        if (dif.z < 2)
            dif.z = 2;

        difx.x = Q_irand(1, (dif.x * 0.9f) * 2);
        difx.y = Q_irand(1, (dif.y * 0.9f) * 2);
        difx.z = Q_irand(1, (dif.z * 0.9f) * 2);

        if (difx.x > dif.x)
            shardorg.x += difx.x - (dif.x);
        else
            shardorg.x -= difx.x;
        if (difx.y > dif.y)
            shardorg.y += difx.y - (dif.y);
        else
            shardorg.y -= difx.y;
        if (difx.z > dif.z)
            shardorg.z += difx.z - (dif.z);
        else
            shardorg.z -= difx.z;

        // CG_TestLine(org, shardorg, 5000, 0xFF0000u, 3);

        CG_ThrowChunk(&shardorg, &velocity, trap->R_RegisterModel(chunkname), 0, 254);

        shardsthrow += 10;
    }
}

#define DEBRIS_SPECIALCASE_ROCK -1
#define DEBRIS_SPECIALCASE_CHUNKS -2
#define DEBRIS_SPECIALCASE_WOOD -3
#define DEBRIS_SPECIALCASE_GLASS -4

#define NUM_DEBRIS_MODELS_GLASS 8
#define NUM_DEBRIS_MODELS_WOOD 8
#define NUM_DEBRIS_MODELS_CHUNKS 3
#define NUM_DEBRIS_MODELS_ROCKS 4 // 12

int dbModels_Glass[NUM_DEBRIS_MODELS_GLASS];
int dbModels_Wood[NUM_DEBRIS_MODELS_WOOD];
int dbModels_Chunks[NUM_DEBRIS_MODELS_CHUNKS];
int dbModels_Rocks[NUM_DEBRIS_MODELS_ROCKS];

// Throws specified debris from within a given bounding box in the world
void CG_CreateDebris(int entnum, vector3 *org, vector3 *mins, vector3 *maxs, int debrissound, int debrismodel) {
    vector3 velocity, a, shardorg, dif, difx;
    float windowmass;
    float shardsthrow = 0;
    int omodel = debrismodel;

    if (omodel == DEBRIS_SPECIALCASE_GLASS && !dbModels_Glass[0]) { // glass no longer exists, using it for metal.
        dbModels_Glass[0] = trap->R_RegisterModel("models/chunks/metal/metal1_1.md3");
        dbModels_Glass[1] = trap->R_RegisterModel("models/chunks/metal/metal1_2.md3");
        dbModels_Glass[2] = trap->R_RegisterModel("models/chunks/metal/metal1_3.md3");
        dbModels_Glass[3] = trap->R_RegisterModel("models/chunks/metal/metal1_4.md3");
        dbModels_Glass[4] = trap->R_RegisterModel("models/chunks/metal/metal2_1.md3");
        dbModels_Glass[5] = trap->R_RegisterModel("models/chunks/metal/metal2_2.md3");
        dbModels_Glass[6] = trap->R_RegisterModel("models/chunks/metal/metal2_3.md3");
        dbModels_Glass[7] = trap->R_RegisterModel("models/chunks/metal/metal2_4.md3");
    }
    if (omodel == DEBRIS_SPECIALCASE_WOOD && !dbModels_Wood[0]) {
        dbModels_Wood[0] = trap->R_RegisterModel("models/chunks/crate/crate1_1.md3");
        dbModels_Wood[1] = trap->R_RegisterModel("models/chunks/crate/crate1_2.md3");
        dbModels_Wood[2] = trap->R_RegisterModel("models/chunks/crate/crate1_3.md3");
        dbModels_Wood[3] = trap->R_RegisterModel("models/chunks/crate/crate1_4.md3");
        dbModels_Wood[4] = trap->R_RegisterModel("models/chunks/crate/crate2_1.md3");
        dbModels_Wood[5] = trap->R_RegisterModel("models/chunks/crate/crate2_2.md3");
        dbModels_Wood[6] = trap->R_RegisterModel("models/chunks/crate/crate2_3.md3");
        dbModels_Wood[7] = trap->R_RegisterModel("models/chunks/crate/crate2_4.md3");
    }
    if (omodel == DEBRIS_SPECIALCASE_CHUNKS && !dbModels_Chunks[0]) {
        dbModels_Chunks[0] = trap->R_RegisterModel("models/chunks/generic/chunks_1.md3");
        dbModels_Chunks[1] = trap->R_RegisterModel("models/chunks/generic/chunks_2.md3");
    }
    if (omodel == DEBRIS_SPECIALCASE_ROCK && !dbModels_Rocks[0]) {
        dbModels_Rocks[0] = trap->R_RegisterModel("models/chunks/rock/rock1_1.md3");
        dbModels_Rocks[1] = trap->R_RegisterModel("models/chunks/rock/rock1_2.md3");
        dbModels_Rocks[2] = trap->R_RegisterModel("models/chunks/rock/rock1_3.md3");
        dbModels_Rocks[3] = trap->R_RegisterModel("models/chunks/rock/rock1_4.md3");
        /*
        dbModels_Rocks[4] = trap->R_RegisterModel("models/chunks/rock/rock2_1.md3");
        dbModels_Rocks[5] = trap->R_RegisterModel("models/chunks/rock/rock2_2.md3");
        dbModels_Rocks[6] = trap->R_RegisterModel("models/chunks/rock/rock2_3.md3");
        dbModels_Rocks[7] = trap->R_RegisterModel("models/chunks/rock/rock2_4.md3");
        dbModels_Rocks[8] = trap->R_RegisterModel("models/chunks/rock/rock3_1.md3");
        dbModels_Rocks[9] = trap->R_RegisterModel("models/chunks/rock/rock3_2.md3");
        dbModels_Rocks[10] = trap->R_RegisterModel("models/chunks/rock/rock3_3.md3");
        dbModels_Rocks[11] = trap->R_RegisterModel("models/chunks/rock/rock3_4.md3");
        */
    }

    VectorSubtract(maxs, mins, &a);

    windowmass = VectorLength(&a); // should give us some idea of how big the chunk of glass is

    while (shardsthrow < windowmass) {
        velocity.x = crandom() * 150;
        velocity.y = crandom() * 150;
        velocity.z = 150 + crandom() * 75;

        if (omodel == DEBRIS_SPECIALCASE_GLASS)
            debrismodel = dbModels_Glass[Q_irand(0, NUM_DEBRIS_MODELS_GLASS - 1)];
        else if (omodel == DEBRIS_SPECIALCASE_WOOD)
            debrismodel = dbModels_Wood[Q_irand(0, NUM_DEBRIS_MODELS_WOOD - 1)];
        else if (omodel == DEBRIS_SPECIALCASE_CHUNKS)
            debrismodel = dbModels_Chunks[Q_irand(0, NUM_DEBRIS_MODELS_CHUNKS - 1)];
        else if (omodel == DEBRIS_SPECIALCASE_ROCK)
            debrismodel = dbModels_Rocks[Q_irand(0, NUM_DEBRIS_MODELS_ROCKS - 1)];

        VectorCopy(org, &shardorg);

        dif.x = (maxs->x - mins->x) / 2;
        dif.y = (maxs->y - mins->y) / 2;
        dif.z = (maxs->z - mins->z) / 2;

        if (dif.x < 2)
            dif.x = 2;
        if (dif.y < 2)
            dif.y = 2;
        if (dif.z < 2)
            dif.z = 2;

        difx.x = Q_irand(1, (dif.x * 0.9f) * 2);
        difx.y = Q_irand(1, (dif.y * 0.9f) * 2);
        difx.z = Q_irand(1, (dif.z * 0.9f) * 2);

        if (difx.x > dif.x)
            shardorg.x += difx.x - (dif.x);
        else
            shardorg.x -= difx.x;
        if (difx.y > dif.y)
            shardorg.y += difx.y - (dif.y);
        else
            shardorg.y -= difx.y;
        if (difx.z > dif.z)
            shardorg.z += difx.z - (dif.z);
        else
            shardorg.z -= difx.z;

        // CG_TestLine(org, shardorg, 5000, 0xFF0000u, 3);

        CG_ThrowChunk(&shardorg, &velocity, debrismodel, debrissound, 0);

        shardsthrow += 10;
    }
}

// Used to find the player and shake the camera if close enough
// intensity ranges from 1 (minor tremble) to 16 (major quake)
void CG_ExplosionEffects(vector3 *origin, float intensity, int radius, int time) {
    // FIXME: When exactly is the vieworg calculated in relation to the rest of the frame?s

    vector3 dir;
    float dist, intensityScale;
    float realIntensity;
    refdef_t *refdef = CG_GetRefdef();

    VectorSubtract(&refdef->vieworg, origin, &dir);
    dist = VectorNormalize(&dir);

    // Use the dir to add kick to the explosion

    if (dist > radius)
        return;

    intensityScale = 1 - (dist / (float)radius);
    realIntensity = intensity * intensityScale;

    CGCam_Shake(realIntensity, time);
}

/*
-------------------------
CG_MiscModelExplosion

Adds an explosion to a misc model breakables
-------------------------
*/

void CG_MiscModelExplosion(vector3 *mins, vector3 *maxs, int size, material_t chunkType) {
    int ct = 13;
    float r;
    vector3 org, mid, dir;
    const char *effect = NULL, *effect2 = NULL;
    int eID1, eID2 = 0;
    int i;

    VectorAdd(mins, maxs, &mid);
    VectorScale(&mid, 0.5f, &mid);

    switch (chunkType) {
    default:
        break;
    case MAT_GLASS:
        effect = "chunks/glassbreak";
        ct = 5;
        break;
    case MAT_GLASS_METAL:
        effect = "chunks/glassbreak";
        effect2 = "chunks/metalexplode";
        ct = 5;
        break;
    case MAT_ELECTRICAL:
    case MAT_ELEC_METAL:
        effect = "chunks/sparkexplode";
        ct = 5;
        break;
    case MAT_METAL:
    case MAT_METAL2:
    case MAT_METAL3:
    case MAT_CRATE1:
    case MAT_CRATE2:
        effect = "chunks/metalexplode";
        ct = 2;
        break;
    case MAT_GRATE1:
        effect = "chunks/grateexplode";
        ct = 8;
        break;
    case MAT_ROPE:
        ct = 20;
        effect = "chunks/ropebreak";
        break;
    case MAT_WHITE_METAL: // not sure what this crap is really supposed to be..
    case MAT_DRK_STONE:
    case MAT_LT_STONE:
    case MAT_GREY_STONE:
    case MAT_SNOWY_ROCK:
        switch (size) {
        case 2:
            effect = "chunks/rockbreaklg";
            break;
        case 1:
        default:
            effect = "chunks/rockbreakmed";
            break;
        }
    }

    if (!effect) {
        return;
    }

    ct += 7 * size;

    // FIXME: real precache .. VERify that these need to be here...don't think they would because the effects should be registered in g_breakable
    // rww - No they don't.. indexed effects gameside get precached on load clientside, as server objects are setup before client asset load time.
    // However, we need to index them, so..
    eID1 = trap->FX_RegisterEffect(effect);

    if (effect2 && effect2[0]) {
        // FIXME: real precache
        eID2 = trap->FX_RegisterEffect(effect2);
    }

    // spawn chunk roughly in the bbox of the thing..
    for (i = 0; i < ct; i++) {
        int j;
        for (j = 0; j < 3; j++) {
            r = random() * 0.8f + 0.1f;
            org.raw[j] = (r * mins->raw[j] + (1 - r) * maxs->raw[j]);
        }

        // shoot effect away from center
        VectorSubtract(&org, &mid, &dir);
        VectorNormalize(&dir);

        if (effect2 && effect2[0] && (rand() & 1))
            trap->FX_PlayEffectID(eID2, &org, &dir, -1, -1, qfalse);
        else
            trap->FX_PlayEffectID(eID1, &org, &dir, -1, -1, qfalse);
    }
}

/*
-------------------------
CG_Chunks

Fun chunk spewer
-------------------------
*/

void CG_Chunks(int owner, vector3 *origin, const vector3 *normal, const vector3 *mins, const vector3 *maxs, float speed, int numChunks, material_t chunkType,
               int customChunk, float baseScale) {
    localEntity_t *le;
    refEntity_t *re;
    vector3 dir;
    int i, j, k;
    int chunkModel = 0;
    leBounceSoundType_e bounce = LEBS_NONE;
    float r, speedMod = 1.0f;
    qboolean chunk = qfalse;

    if (chunkType == MAT_NONE) {
        // Well, we should do nothing
        return;
    }

    // Set up our chunk sound info...breaking sounds are done here so they are done once on breaking..some return instantly because the chunks are done with
    // effects instead of models
    switch (chunkType) {
    default:
        break;
    case MAT_GLASS:
        trap->S_StartSound(NULL, owner, CHAN_BODY, media.sounds.environment.glassChunk);
        return;
    case MAT_GRATE1:
        trap->S_StartSound(NULL, owner, CHAN_BODY, media.sounds.environment.grate);
        return;
    case MAT_ELECTRICAL: // (sparks)
        trap->S_StartSound(NULL, owner, CHAN_BODY, trap->S_RegisterSound(va("sound/ambience/spark%d.wav", Q_irand(1, 6))));
        return;
    case MAT_DRK_STONE:
    case MAT_LT_STONE:
    case MAT_GREY_STONE:
    case MAT_WHITE_METAL: // not quite sure what this stuff is supposed to be...it's for Stu
    case MAT_SNOWY_ROCK:
        trap->S_StartSound(NULL, owner, CHAN_BODY, media.sounds.environment.rockBreak);
        bounce = LEBS_ROCK;
        speedMod = 0.5f; // rock blows up less
        break;
    case MAT_GLASS_METAL:
        trap->S_StartSound(NULL, owner, CHAN_BODY, media.sounds.environment.glassChunk); // FIXME: should probably have a custom sound
        bounce = LEBS_METAL;
        break;
    case MAT_CRATE1:
    case MAT_CRATE2:
        trap->S_StartSound(NULL, owner, CHAN_BODY, media.sounds.environment.crateBreak[Q_irand(0, 1)]);
        break;
    case MAT_METAL:
    case MAT_METAL2:
    case MAT_METAL3:
    case MAT_ELEC_METAL: // FIXME: maybe have its own sound?
        trap->S_StartSound(NULL, owner, CHAN_BODY, media.sounds.environment.chunk);
        bounce = LEBS_METAL;
        speedMod = 0.8f; // metal blows up a bit more
        break;
    case MAT_ROPE:
        //		trap->S_StartSound( NULL, owner, CHAN_BODY, cgi_S_RegisterSound( "" ));  FIXME:  needs a sound
        return;
    }

    if (baseScale <= 0.0f) {
        baseScale = 1.0f;
    }

    // Chunks
    for (i = 0; i < numChunks; i++) {
        if (customChunk > 0) {
            // Try to use a custom chunk.
            if (cgs.gameModels[customChunk]) {
                chunk = qtrue;
                chunkModel = cgs.gameModels[customChunk];
            }
        }

        if (!chunk) {
            // No custom chunk.  Pick a random chunk type at run-time so we don't get the same chunks
            switch (chunkType) {
            default:
                break;
            case MAT_METAL2: // bluegrey
                chunkModel = media.models.chunks[CHUNK_METAL2][Q_irand(0, 3)];
                break;
            case MAT_GREY_STONE: // gray
                chunkModel = media.models.chunks[CHUNK_ROCK1][Q_irand(0, 3)];
                break;
            case MAT_LT_STONE: // tan
                chunkModel = media.models.chunks[CHUNK_ROCK2][Q_irand(0, 3)];
                break;
            case MAT_DRK_STONE: // brown
                chunkModel = media.models.chunks[CHUNK_ROCK3][Q_irand(0, 3)];
                break;
            case MAT_SNOWY_ROCK: // gray & brown
                if (Q_irand(0, 1)) {
                    chunkModel = media.models.chunks[CHUNK_ROCK1][Q_irand(0, 3)];
                } else {
                    chunkModel = media.models.chunks[CHUNK_ROCK3][Q_irand(0, 3)];
                }
                break;
            case MAT_WHITE_METAL:
                chunkModel = media.models.chunks[CHUNK_WHITE_METAL][Q_irand(0, 3)];
                break;
            case MAT_CRATE1: // yellow multi-colored crate chunks
                chunkModel = media.models.chunks[CHUNK_CRATE1][Q_irand(0, 3)];
                break;
            case MAT_CRATE2: // red multi-colored crate chunks
                chunkModel = media.models.chunks[CHUNK_CRATE2][Q_irand(0, 3)];
                break;
            case MAT_ELEC_METAL:
            case MAT_GLASS_METAL:
            case MAT_METAL: // grey
                chunkModel = media.models.chunks[CHUNK_METAL1][Q_irand(0, 3)];
                break;
            case MAT_METAL3:
                if (rand() & 1) {
                    chunkModel = media.models.chunks[CHUNK_METAL1][Q_irand(0, 3)];
                } else {
                    chunkModel = media.models.chunks[CHUNK_METAL2][Q_irand(0, 3)];
                }
                break;
            }
        }

        // It wouldn't look good to throw a bunch of RGB axis models...so make sure we have something to work with.
        if (chunkModel) {
            le = CG_AllocLocalEntity();
            re = &le->refEntity;

            re->hModel = chunkModel;
            le->leType = LE_FRAGMENT;
            le->endTime = cg.time + 1300 + random() * 900;

            // spawn chunk roughly in the bbox of the thing...bias towards center in case thing blowing up doesn't complete fill its bbox.
            for (j = 0; j < 3; j++) {
                r = random() * 0.8f + 0.1f;
                re->origin.raw[j] = (r * mins->raw[j] + (1 - r) * maxs->raw[j]);
            }
            VectorCopy(&re->origin, &le->pos.trBase);

            // Move out from center of thing, otherwise you can end up things moving across the brush in an undesirable direction.  Visually looks wrong
            VectorSubtract(&re->origin, origin, &dir);
            VectorNormalize(&dir);
            VectorScale(&dir, flrand(speed * 0.5f, speed * 1.25f) * speedMod, &le->pos.trDelta);

            // Angular Velocity
            VectorSet(&le->angles.trBase, random() * 360, random() * 360, random() * 360);

            le->angles.trDelta.x = crandom();
            le->angles.trDelta.y = crandom();
            le->angles.trDelta.z = 0; // don't do roll

            VectorScale(&le->angles.trDelta, random() * 600.0f + 200.0f, &le->angles.trDelta);

            le->pos.trType = TR_GRAVITY;
            le->angles.trType = TR_LINEAR;
            le->pos.trTime = le->angles.trTime = cg.time;
            le->bounceFactor = 0.2f + random() * 0.2f;
            le->leFlags |= LEF_TUMBLE;
            // le->ownerGentNum = owner;
            le->leBounceSoundType = bounce;

            // Make sure that we have the desired start size set
            le->radius = flrand(baseScale * 0.75f, baseScale * 1.25f);
            re->nonNormalizedAxes = qtrue;
            AxisCopy(axisDefault, re->axis); // could do an angles to axis, but this is cheaper and works ok
            for (k = 0; k < 3; k++)
                re->modelScale.raw[k] = le->radius;
            ScaleModelAxis(re);
            /*
            for( k = 0; k < 3; k++ )
            {
            VectorScale( re->axis[k], le->radius, re->axis[k] );
            }
            */
        }
    }
}

void CG_ScorePlum(int client, vector3 *org, int score) {
    localEntity_t *le;
    refEntity_t *re;
    vector3 angles;
    static vector3 lastPos;

    // only visualize for the client that scored
    if (client != cg.predictedPlayerState.clientNum || cg_scorePlums.integer == 0) {
        return;
    }

    le = CG_AllocLocalEntity();
    le->leFlags = 0;
    le->leType = LE_SCOREPLUM;
    le->startTime = cg.time;
    le->endTime = cg.time + 4000;
    le->lifeRate = 1.0f / (le->endTime - le->startTime);

    le->color[0] = le->color[1] = le->color[2] = le->color[3] = 1.0f;
    le->radius = score;

    VectorCopy(org, &le->pos.trBase);
    if (org->z >= lastPos.z - 20 && org->z <= lastPos.z + 20) {
        le->pos.trBase.z -= 20;
    }

    // trap->Print( "Plum origin %i %i %i -- %i\n", (int)org[0], (int)org[1], (int)org[2], (int)Distance(org, lastPos));
    VectorCopy(org, &lastPos);

    re = &le->refEntity;

    re->reType = RT_SPRITE;
    re->radius = 16;

    VectorClear(&angles);
    AnglesToAxis(&angles, re->axis);
}

localEntity_t *CG_MakeExplosion(vector3 *origin, vector3 *dir, qhandle_t hModel, int numFrames, qhandle_t shader, int msec, qboolean isSprite, float scale,
                                uint32_t flags) {
    float ang = 0;
    localEntity_t *ex;
    int offset;
    vector3 tmpVec, newOrigin;

    if (msec <= 0) {
        trap->Error(ERR_DROP, "CG_MakeExplosion: msec = %i", msec);
    }

    // skew the time a bit so they aren't all in sync
    offset = rand() & 63;

    ex = CG_AllocLocalEntity();
    if (isSprite) {
        ex->leType = LE_SPRITE_EXPLOSION;
        ex->refEntity.rotation = rand() % 360;
        ex->radius = scale;
        VectorScale(dir, 16, &tmpVec);
        VectorAdd(&tmpVec, origin, &newOrigin);
    } else {
        ex->leType = LE_EXPLOSION;
        VectorCopy(origin, &newOrigin);

        // set axis with random rotate when necessary
        if (!dir) {
            AxisClear(ex->refEntity.axis);
        } else {
            if (!(flags & LEF_NO_RANDOM_ROTATE))
                ang = rand() % 360;
            VectorCopy(dir, &ex->refEntity.axis[0]);
            RotateAroundDirection(ex->refEntity.axis, ang);
        }
    }

    ex->startTime = cg.time - offset;
    ex->endTime = ex->startTime + msec;

    // bias the time so all shader effects start correctly
    ex->refEntity.shaderTime = ex->startTime / 1000.0f;

    ex->refEntity.hModel = hModel;
    ex->refEntity.customShader = shader;
    ex->lifeRate = (float)numFrames / msec;
    ex->leFlags = flags;

    // Scale the explosion
    if (scale != 1) {
        ex->refEntity.nonNormalizedAxes = qtrue;

        VectorScale(&ex->refEntity.axis[0], scale, &ex->refEntity.axis[0]);
        VectorScale(&ex->refEntity.axis[1], scale, &ex->refEntity.axis[1]);
        VectorScale(&ex->refEntity.axis[2], scale, &ex->refEntity.axis[2]);
    }
    // set origin
    VectorCopy(&newOrigin, &ex->refEntity.origin);
    VectorCopy(&newOrigin, &ex->refEntity.oldorigin);

    ex->color[0] = ex->color[1] = ex->color[2] = 1.0f;

    return ex;
}

/*
-------------------------
CG_SurfaceExplosion

Adds an explosion to a surface
-------------------------
*/

#define NUM_SPARKS 12
#define NUM_PUFFS 1
#define NUM_EXPLOSIONS 4

void CG_SurfaceExplosion(vector3 *origin, vector3 *normal, float radius, float shake_speed, qboolean smoke) {
    localEntity_t *le;
    // FXTrail			*particle;
    vector3 direction, new_org;
    vector3 velocity = {0, 0, 0};
    vector3 temp_org, temp_vel;
    int i;
    refdef_t *refdef = CG_GetRefdef();

    // Smoke
    // Move this out a little from the impact surface
    VectorMA(origin, 4, normal, &new_org);
    VectorSet(&velocity, 0.0f, 0.0f, 16.0f);

    for (i = 0; i < 4; i++) {
        VectorSet(&temp_org, new_org.x + (crandom() * 16.0f), new_org.y + (crandom() * 16.0f), new_org.z + (random() * 4.0f));
        VectorSet(&temp_vel, velocity.x + (crandom() * 8.0f), velocity.y + (crandom() * 8.0f), velocity.z + (crandom() * 8.0f));
    }

    // Core of the explosion

    // Orient the explosions to face the camera
    VectorSubtract(&refdef->vieworg, origin, &direction);
    VectorNormalize(&direction);

    // Tag the last one with a light
    le = CG_MakeExplosion(origin, &direction, media.models.explosion, 6, media.gfx.world.surfaceExplosion, 500, qfalse, radius * 0.02f + (random() * 0.3f), 0);
    le->light = 150;
    VectorSet(&le->lightColor, 0.9f, 0.8f, 0.5f);

    for (i = 0; i < NUM_EXPLOSIONS - 1; i++) {
        VectorSet(&new_org, (origin->x + (16 + (crandom() * 8)) * crandom()), (origin->y + (16 + (crandom() * 8)) * crandom()),
                  (origin->z + (16 + (crandom() * 8)) * crandom()));
        CG_MakeExplosion(&new_org, &direction, media.models.explosion, 6, media.gfx.world.surfaceExplosion, 300 + (rand() & 99), qfalse,
                         radius * 0.05f + (crandom() * 0.3f), 0);
    }

    // Shake the camera
    CG_ExplosionEffects(origin, shake_speed, 350, 750);

    // The level designers wanted to be able to turn the smoke spawners off.  The rationale is that they
    //	want to blow up catwalks and such that fall down...when that happens, it shouldn't really leave a mark
    //	and a smoke spewer at the explosion point...
    if (smoke) {
        VectorMA(origin, -8, normal, &temp_org);
        // Impact mark
        // FIXME: Replace mark
        // CG_ImpactMark( media.gfx.world.burnMarkShader, origin, normal, random()*360, 1,1,1,1, qfalse, 8, qfalse );
    }
}

// This is the spurt of blood when a character gets hit
void CG_Bleed(vector3 *origin, int entityNum) {
    localEntity_t *ex;

    ex = CG_AllocLocalEntity();
    ex->leType = LE_EXPLOSION;

    ex->startTime = cg.time;
    ex->endTime = ex->startTime + 500;

    VectorCopy(origin, &ex->refEntity.origin);
    ex->refEntity.reType = RT_SPRITE;
    ex->refEntity.rotation = rand() % 360;
    ex->refEntity.radius = 24;

    ex->refEntity.customShader = 0; // media.gfx.world.bloodExplosion;

    // don't show player's own blood in view
    if (entityNum == cg.snap->ps.clientNum) {
        ex->refEntity.renderfx |= RF_THIRD_PERSON;
    }
}

void CG_LaunchGib(vector3 *origin, vector3 *velocity, qhandle_t hModel) {
    localEntity_t *le;
    refEntity_t *re;

    le = CG_AllocLocalEntity();
    re = &le->refEntity;

    le->leType = LE_FRAGMENT;
    le->startTime = cg.time;
    le->endTime = le->startTime + 5000 + random() * 3000;

    VectorCopy(origin, &re->origin);
    AxisCopy(axisDefault, re->axis);
    re->hModel = hModel;

    le->pos.trType = TR_GRAVITY;
    VectorCopy(origin, &le->pos.trBase);
    VectorCopy(velocity, &le->pos.trDelta);
    le->pos.trTime = cg.time;

    le->bounceFactor = 0.6f;

    le->leBounceSoundType = LEBS_BLOOD;
    le->leMarkType = LEMT_BLOOD;
}
