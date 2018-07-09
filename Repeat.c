/**
 * Copyright 2018 Pushkar Piggott
 *
 * Repeat.c
 */

#include "Repeat.h"
#include "Piece.h"
#include "Topics.h"
#include "Display.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

///////////////////////////////////////////////////////////////////////////////
TSet** repeat = 0;

///////////////////////////////////////////////////////////////////////////////
void findRepeat(int displayRepeat) {
   int height = spHeight;
   pcSetHeight();
   initDisplay(-1);
   repeat = (TSet**)malloc(pieceCount * sizeof(TSet*));
   repeat[0] = 0;
   int maxSps = 4;
   TPlace* sps = SP_NEW(maxSps);
   int pc;
   for (pc = eFirstPiece; pc < pieceCount; ++pc) {
      int paths;
      for (paths = 0; pieces[pc][paths]; ++paths) {}
      repeat[pc] = (TSet*)malloc(paths * sizeof(TSet));
      int distinct = 0;
      int path;
      for (path = 0; pieces[pc][path]; ++path) {
         repeat[pc][path] = 0;
         int d = distinct;
         EOrientation or;
         // test only 001 plane 
         for (or = 0; or < OR_001_COUNT; ++or) {
            TPosition pos[eDimensions] = {spHeight / 2, spHeight / 2, spHeight / 2};
            while (distinct >= maxSps) {
               maxSps *= 2;
               sps = SP_EXTEND(sps, maxSps);
            }
            spClear(sps + SPS(distinct));
            SP_SET(sps + SPS(distinct), pc, pos);
            pcWalk(pc, pieces[pc][path], or, pos, sps + spXYZ * distinct);
            int eq;
            for (eq = distinct - 1; eq >= 0; --eq) {
               if (spEqual(sps + SPS(distinct), 
                           sps + SPS(eq))) {
                  break;
               }
            }
            if (displayRepeat) {
               if (eq == -1) {
                  printf("%c:%s:%s(%d)[%d]%s", GLYPH(pc), pieces[pc][path], orToString(or), or, distinct, EOL);
               } else {
                  printf("(%c:%s:%s(%d) == [%d])%s", GLYPH(pc), pieces[pc][path], orToString(or), or, eq, EOL);
               }
               displayWidePlane(spHeight / 2, sps + SPS(distinct));
            }
            if (eq < 0) {
               ++distinct;
            } else {
               // duplicate 001 plane results in other planes
               repeat[pc][path] = SET_WITH(SET_WITH(SET_WITH(
                                           repeat[pc][path], 
                                           OR_001_OFFSET + or),
                                           OR_110_OFFSET + or),
                                           OR_1T0_OFFSET + or);
            }
         }
         if (displayRepeat) {
            displayWidePlane(spHeight / 2, 0);
         }
         if (displayRepeat && repeat[pc][path]) {
            char* r = (repeat[pc][path] == SET_ALL_OF(eOrientations))
                    ? strdup("all (removed)")
                    : setToString(repeat[pc][path] & OR_001_SET, orToString);
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
