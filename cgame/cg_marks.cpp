// Copyright (C) 1999-2000 Id Software, Inc.
//
// cg_marks.c -- wall marks

#include "cg_local.h"

// MARK POLYS

markPoly_t cg_activeMarkPolys; // double linked list
markPoly_t *cg_freeMarkPolys;  // single linked list
markPoly_t cg_markPolys[MAX_MARK_POLYS];
static int markTotal;

// This is called at startup and for tournament restarts
void CG_InitMarkPolys(void) {
    int i;

    memset(cg_markPolys, 0, sizeof(cg_markPolys));

    cg_activeMarkPolys.nextMark = &cg_activeMarkPolys;
    cg_activeMarkPolys.prevMark = &cg_activeMarkPolys;
    cg_freeMarkPolys = cg_markPolys;
    for (i = 0; i < MAX_MARK_POLYS - 1; i++) {
        cg_markPolys[i].nextMark = &cg_markPolys[i + 1];
    }
}

void CG_FreeMarkPoly(markPoly_t *le) {
    if (!le->prevMark) {
        trap->Error(ERR_DROP, "CG_FreeLocalEntity: not active");
    }

    // remove from the doubly linked active list
    le->prevMark->nextMark = le->nextMark;
    le->nextMark->prevMark = le->prevMark;

    // the free list is only singly linked
    le->nextMark = cg_freeMarkPolys;
    cg_freeMarkPolys = le;
}

// Will allways succeed, even if it requires freeing an old active mark
markPoly_t *CG_AllocMark(void) {
    markPoly_t *le;
    int time;

    if (!cg_freeMarkPolys) {
        // no free entities, so free the one at the end of the chain
        // remove the oldest active entity
        time = cg_activeMarkPolys.prevMark->time;
        while (cg_activeMarkPolys.prevMark && time == cg_activeMarkPolys.prevMark->time) {
            CG_FreeMarkPoly(cg_activeMarkPolys.prevMark);
        }
    }

    le = cg_freeMarkPolys;
    cg_freeMarkPolys = cg_freeMarkPolys->nextMark;

    memset(le, 0, sizeof(*le));

    // link into the active list
    le->nextMark = cg_activeMarkPolys.nextMark;
    le->prevMark = &cg_activeMarkPolys;
    cg_activeMarkPolys.nextMark->prevMark = le;
    cg_activeMarkPolys.nextMark = le;
    return le;
}

#define MAX_MARK_FRAGMENTS 128
#define MAX_MARK_POINTS 384

// origin should be a point within a unit of the plane
// dir should be the plane normal
// temporary marks will not be stored or randomly oriented, but immediately passed to the renderer.
void CG_ImpactMark(qhandle_t markShader, const vector3 *origin, const vector3 *dir, float orientation, float red, float green, float blue, float alpha,
                   qboolean alphaFade, float radius, qboolean temporary) {
    vector3 axis[3];
    float texCoordScale;
    vector3 originalPoints[4];
    byte colors[4];
    int i, j;
    int numFragments;
    markFragment_t markFragments[MAX_MARK_FRAGMENTS], *mf;
    vector3 markPoints[MAX_MARK_POINTS];
    vector3 projection;

    assert(markShader);

    if (!cg_marks.integer) {
        return;
    } else if (cg_marks.integer == 2) {
        trap->R_AddDecalToScene(markShader, origin, dir, orientation, red, green, blue, alpha, alphaFade, radius, temporary);
        return;
    }

    if (radius <= 0) {
        trap->Error(ERR_DROP, "CG_ImpactMark called with <= 0 radius");
    }

    // if ( markTotal >= MAX_MARK_POLYS ) {
    //	return;
    // }

    // create the texture axis
    VectorNormalize2(dir, &axis[0]);
    PerpendicularVector(&axis[1], &axis[0]);
    RotatePointAroundVector(&axis[2], &axis[0], &axis[1], orientation);
    CrossProduct(&axis[0], &axis[2], &axis[1]);

    texCoordScale = 0.5f * 1.0f / radius;

    // create the full polygon
    for (i = 0; i < 3; i++) {
        originalPoints[0].raw[i] = origin->raw[i] - radius * axis[1].raw[i] - radius * axis[2].raw[i];
        originalPoints[1].raw[i] = origin->raw[i] + radius * axis[1].raw[i] - radius * axis[2].raw[i];
        originalPoints[2].raw[i] = origin->raw[i] + radius * axis[1].raw[i] + radius * axis[2].raw[i];
        originalPoints[3].raw[i] = origin->raw[i] - radius * axis[1].raw[i] + radius * axis[2].raw[i];
    }

    // get the fragments
    VectorScale(dir, -20, &projection);
    numFragments = trap->R_MarkFragments(4, originalPoints, &projection, MAX_MARK_POINTS, &markPoints[0], MAX_MARK_FRAGMENTS, markFragments);

    colors[0] = red * 255;
    colors[1] = green * 255;
    colors[2] = blue * 255;
    colors[3] = alpha * 255;

    for (i = 0, mf = markFragments; i < numFragments; i++, mf++) {
        polyVert_t *v;
        polyVert_t verts[MAX_VERTS_ON_POLY];
        markPoly_t *mark;

        // we have an upper limit on the complexity of polygons
        // that we store persistantly
        if (mf->numPoints > MAX_VERTS_ON_POLY) {
            mf->numPoints = MAX_VERTS_ON_POLY;
        }
        for (j = 0, v = verts; j < mf->numPoints; j++, v++) {
            vector3 delta;

            VectorCopy(&markPoints[mf->firstPoint + j], &v->xyz);

            VectorSubtract(&v->xyz, origin, &delta);
            v->st[0] = 0.5f + DotProduct(&delta, &axis[1]) * texCoordScale;
            v->st[1] = 0.5f + DotProduct(&delta, &axis[2]) * texCoordScale;
            {
                int k;
                for (k = 0; k < 4; k++)
                    v->modulate[k] = colors[k];
            }
        }

        // if it is a temporary (shadow) mark, add it immediately and forget about it
        if (temporary) {
            trap->R_AddPolysToScene(markShader, mf->numPoints, verts, 1);
            continue;
        }

        // otherwise save it persistantly
        mark = CG_AllocMark();
        mark->time = cg.time;
        mark->alphaFade = alphaFade;
        mark->markShader = markShader;
        mark->poly.numVerts = mf->numPoints;
        mark->color[0] = red;
        mark->color[1] = green;
        mark->color[2] = blue;
        mark->color[3] = alpha;
        memcpy(mark->verts, verts, mf->numPoints * sizeof(verts[0]));
        markTotal++;
    }
}

#define MARK_TOTAL_TIME 10000
#define MARK_FADE_TIME 1000

void CG_AddMarks(void) {
    int j;
    markPoly_t *mp, *next;
    int t;
    int fade;

    if (!cg_marks.integer) {
        return;
    }

    mp = cg_activeMarkPolys.nextMark;
    for (; mp != &cg_activeMarkPolys; mp = next) {
        // grab next now, so if the local entity is freed we
        // still have it
        next = mp->nextMark;

        // see if it is time to completely remove it
        if (cg.time > mp->time + MARK_TOTAL_TIME) {
            CG_FreeMarkPoly(mp);
            continue;
        }

        // fade all marks out with time
        t = mp->time + MARK_TOTAL_TIME - cg.time;
        if (t < MARK_FADE_TIME) {
            fade = 255 * t / MARK_FADE_TIME;
            if (mp->alphaFade) {
                for (j = 0; j < mp->poly.numVerts; j++) {
                    mp->verts[j].modulate[3] = fade;
                }
            } else {
                float f = (float)t / MARK_FADE_TIME;
                for (j = 0; j < mp->poly.numVerts; j++) {
                    mp->verts[j].modulate[0] = mp->color[0] * f;
                    mp->verts[j].modulate[1] = mp->color[1] * f;
                    mp->verts[j].modulate[2] = mp->color[2] * f;
                }
            }
        } else {
            for (j = 0; j < mp->poly.numVerts; j++) {
                mp->verts[j].modulate[0] = mp->color[0];
                mp->verts[j].modulate[1] = mp->color[1];
                mp->verts[j].modulate[2] = mp->color[2];
            }
        }

        trap->R_AddPolysToScene(mp->markShader, mp->poly.numVerts, mp->verts, 1);
    }
}
