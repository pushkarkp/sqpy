/**
 * Copyright 2018 Pushkar Piggott
 *
 * Repeat.c
 */

#include "Repeat.h"
#include "Piece.h"
#include "SquarePyramid.h"
#include "Topics.h"
#include "Display.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

///////////////////////////////////////////////////////////////////////////////
TSet** repeat = 0;
TSet*** repeatRot = 0;
TSet*** repeatRef = 0;

#define PLANE (spHeight - 1)

///////////////////////////////////////////////////////////////////////////////
TSet findEqualSym(int or, int distinct, TPlace* sps, FEqualSym eqSym, FSetElementToString tostr) {
   TSet sor = 0; 
   int eq;
   for (eq = 0; eq < distinct; ++eq) {
      TSet s = eqSym(sps + SPS(eq), sps + SPS(distinct), 0);
      sor |= s;
      if (s != 0 && IS_TOPIC(eTopicRepeat)) {
         displayWidePlane(PLANE, 0);
         char* symstr = setToString(s, tostr, "");
         printf("(%s[%d] =%s= [%d])%s", orToString(or), distinct, symstr, eq, EOL);
         free(symstr);
         displayWidePlane(PLANE, sps + SPS(distinct));
         displayWidePlane(PLANE, sps + SPS(eq));
         displayWidePlane(PLANE, 0);
      }
   }
   return sor;
}

///////////////////////////////////////////////////////////////////////////////
void setOrientation(TSet* psor, int size, int or, TSet sosym) {
   if (sosym) { 
      int s;
      for (s = 0; s < size; ++s) {
         if (SET_HAS(sosym, s)) {
            psor[s] = SET_WITH(psor[s], or);
         }
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
void findRepeat() {
   int height = spHeight;
   pcSetHeight();
   initDisplay(-1);
   repeat = (TSet**)malloc(pieceCount * sizeof(TSet*));
   repeat[0] = 0;
   repeatRot = (TSet***)malloc(pieceCount * sizeof(TSet**));
   repeatRef = (TSet***)malloc(pieceCount * sizeof(TSet**));
   int maxSps = 4;
   TPlace* sps = SP_NEW(maxSps);
   int pc;
   for (pc = eFirstPiece; pc < pieceCount; ++pc) {
      int paths;
      for (paths = 0; pieces[pc][paths]; ++paths) {}
      repeat[pc] = (TSet*)malloc(paths * sizeof(TSet));
      repeatRot[pc] = (TSet**)malloc(paths * sizeof(TSet*));
      repeatRef[pc] = (TSet**)malloc(paths * sizeof(TSet*));
      int distinct = 0;
      int path;
      for (path = 0; pieces[pc][path]; ++path) {
         repeat[pc][path] = 0;
         repeatRot[pc][path] = (TSet*)malloc(eRotations * sizeof(TSet));
         memset(repeatRot[pc][path], 0, eRotations);
         repeatRef[pc][path] = (TSet*)malloc(eReflectionPlanes * sizeof(TSet));
         memset(repeatRef[pc][path], 0, eReflectionPlanes);
         int d = distinct;
         EOrientation or;
         // test only z plane 
         for (or = 0; or < OR_Z_COUNT; ++or) {
            TPosition pos[eDimensions] = {spHeight / 2, spHeight / 2, PLANE};
            while (distinct >= maxSps) {
               maxSps *= 2;
               sps = SP_EXTEND(sps, maxSps);
            }
            spClear(sps + SPS(distinct));
            SP_SET(sps + SPS(distinct), pc, pos);
            pcWalk(pc, 0, pieces[pc][path], or, pos, sps + spXYZ2 * distinct);
            int eq;
            for (eq = distinct - 1; eq >= 0; --eq) {
               if (spEqual(sps + SPS(distinct), 
                           sps + SPS(eq))) {
                  break;
               }
            }
            if (IS_TOPIC(eTopicRepeat)) {
               if (eq >= 0) {
                  printf("(%c:%s:%s == [%d])%s", GLYPH(pc), pieces[pc][path], orToString(or), eq, EOL);
               } else {
                  printf("%c:%s:%s[%d]%s", GLYPH(pc), pieces[pc][path], orToString(or), distinct, EOL);
               }
               displayWidePlane(PLANE, sps + SPS(distinct));
            }
            if (eq >= 0) {
               repeat[pc][path] = SET_WITH(repeat[pc][path], or);
            } else {
               setOrientation(repeatRot[pc][path], eRotations, or,
                              findEqualSym(or, distinct, sps, spEqualRotate, rotationToString));
               setOrientation(repeatRef[pc][path], eReflectionPlanes, or,
                              findEqualSym(or, distinct, sps, spEqualReflect, reflectionPlaneToString));
               ++distinct;
            }
         }
         if (IS_TOPIC(eTopicRepeat)) {
            displayWidePlane(PLANE, 0);
         }
         if (IS_TOPIC(eTopicRepeat) && repeat[pc][path]) {
            char* r = (repeat[pc][path] == SET_ALL_OF(eOrientations))
                    ? strdup("all (removed)")
                    : setToString(repeat[pc][path] & OR_Z_SET, orToString, "");
            printf("%c:%s repeats: %s%s%s", GLYPH(pc), pieces[pc][path], r, EOL, EOL);
            free(r);
         }
         if (distinct == d) {
            pcRemovePath(pc, path);
         }
      }
   }
   free(sps);
   spSetHeight(height);
   initDisplay(-1);
}

///////////////////////////////////////////////////////////////////////////////
TSetOfOrientations skipOr(const TSet* psos, TSet s, int size) {
//printf("skipOr(s 0x%x, size %d, *psos 0x%x)%s", s, size, *psos, EOL);
   if (!s) {
      return 0;
   }
   TSetOfOrientations soor = 0;
   int i;
   for (i = 0; i < size; ++i) {
      if (SET_HAS(s, i)) {
         soor |= psos[i];
      }
   }
   return soor;
}

///////////////////////////////////////////////////////////////////////////////
TSetOfOrientations spaceRotateSkip(TSetOfRotations sorn) {
   if (!SET_HAS(sorn, e180)) {
      return 0;
   }
   if (!SET_HAS(sorn, e90)
    || !SET_HAS(sorn, e270)) {
      // skip y half of both vertical planes
      return OR_BY_SET | OR_DY_SET;
   }
   // skip all but half of one vertical plane
   return OR_BY_SET | OR_DX_SET | OR_DY_SET;
}

///////////////////////////////////////////////////////////////////////////////
TSetOfOrientations spaceReflectSkip(TSetOfReflectionPlanes sorp) {
   if (SET_HAS(sorp, eXReflection)
    && SET_HAS(sorp, eYReflection)) {
      // skip all but half of one vertical plane
      return OR_BY_SET | OR_DX_SET | OR_DY_SET;
   }
   TSetOfOrientations soor = 0;
   if (SET_HAS(sorp, eXReflection)) {
      // skip half of both vertical planes
      return OR_BY_SET | OR_DX_SET;
   }
   if (SET_HAS(sorp, eYReflection)) {
      // skip y half of both vertical planes
      return OR_BY_SET | OR_DY_SET;
   }
   if (SET_HAS(sorp, eBReflection)) {
      // skip y half of bisected vertical plane
      soor |= OR_DY_SET;
   }
   if (SET_HAS(sorp, eDReflection)) {
      // skip y half of bisected vertical plane
      soor |= OR_BY_SET;
   }
   return soor;
}

///////////////////////////////////////////////////////////////////////////////
TSetOfOrientations pathRotateSkip(int pc, int path, TSetOfRotations sorn) {
   return skipOr(repeatRot[pc][path], sorn, eRotations);
}

///////////////////////////////////////////////////////////////////////////////
TSetOfOrientations pathReflectSkip(int pc, int path, TSetOfReflectionPlanes sorp) {
/*   char* str = setToString(sorp, reflectionPlaneToString, "");
   printf("%c[%d] %s", GLYPH(pc), path, str);
   free(str);
   int i;
   for (int i = 0; i < eReflectionPlanes; ++i) {
      char* str = setToString(repeatRef[pc][path][i], orToString, "");
      printf(" | %c 0x%x %s", reflectionPlaneToChar(i), repeatRef[pc][path][i], str);
      free(str);
   }
   printf(EOL);
*/   return skipOr(repeatRef[pc][path], sorp, eReflectionPlanes);
}
