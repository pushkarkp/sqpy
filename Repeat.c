/**
 * Copyright 2018 Pushkar Piggott
 *
 * Repeat.c
 */

#include "Repeat.h"
#include "Piece.h"
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
   const int level = spHeight + 1;
   const int skip = (spHeight - 1) / 2;
   const int startr = skip * level;
   const int endr = (skip + 1) * level + 1;
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
         EOrientation or;
         for (or = 0; or < OR_ON_PLANE; ++or) {
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
                  printf("%c %s %s: %d%s", GLYPH(pc), pieces[pc][path], orToString(or), distinct, EOL);
               } else {
                  printf("(%c %s %s == %d)%s", GLYPH(pc), pieces[pc][path], orToString(or), eq, EOL);
               }
               displayWideRowRange(eCube, startr, endr, sps + SPS(distinct));
            }
            if (eq < 0) {
               ++distinct;
            } else {
               repeat[pc][path] = SET_WITH(repeat[pc][path], or & OR_ON_PLANE_MASK);
            }
         }
         if (displayRepeat && repeat[pc][path]) {
            printf("repeat %s%s", setToString(repeat[pc][path], orToString), EOL);
         }
      }
      if (displayRepeat) {
         displayWideRowRange(eCube, startr, endr, 0);
      }
   }
   free(sps);
   spSetHeight(height);
   initDisplay(-1);
}
