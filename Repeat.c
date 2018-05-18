/**
 * Copyright 2018 Pushkar Piggott
 *
 * Repeat.c
 */

#include "Repeat.h"
#include "Orientation.h"
#include "Piece.h"
#include "Display.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define AS_INDEX(x) (2 * x + 1)

///////////////////////////////////////////////////////////////////////////////
TPathRepeat** repeat = 0;

///////////////////////////////////////////////////////////////////////////////
void findRepeat(int displayRepeat) {
   repeat = (TPathRepeat**)malloc(pieceCount * sizeof(TPathRepeat*));
   repeat[0] = 0;
   int maxSps = 16;
   TPlace* sps = SP_NEW(maxSps);
   int pc;
   for (pc = eFirstPiece; pc < pieceCount; ++pc) {
      int paths;
      for (paths = 0; pieces[pc][paths]; ++paths) {}
      repeat[pc] = (TPathRepeat*)malloc(paths * sizeof(TPathRepeat));
      memset(*repeat[pc], 0, paths * sizeof(TPathRepeat));
      EPlane plane;
      for (plane = e001; plane < ePlanes; ++plane) {
         // use every other sp to allow overflow 
         int distinct = 0;
         int path;
         for (path = 0; pieces[pc][path]; ++path) {
            EOrientation or;
            for (or = 0; or < eOrientations; ++or) {
               if (orients[or].plane != plane) {
                  continue;
               }
               TPosition pos[eDimensions] = {spHeight / 2, spHeight / 2, spHeight / 2};
               while (AS_INDEX(distinct) + 1 >= maxSps) {
                  maxSps *= 2;
                  sps = SP_EXTEND(maxSps, sps);
               }
               spClear(sps + SPS(AS_INDEX(distinct) - 1));
               spClear(sps + SPS(AS_INDEX(distinct)));
               spClear(sps + SPS(AS_INDEX(distinct) + 1));
               SP_SET(sps + SPS(AS_INDEX(distinct)), pc, pos);
               walk(pc, pieces[pc][path], or, pos, sps + spXYZ * AS_INDEX(distinct));
               int eq;
               for (eq = distinct - 1; eq >= 0; --eq) {
                  if (spEqual(sps + SPS(AS_INDEX(distinct)), 
                              sps + SPS(AS_INDEX(eq)))
                   && spEqual(sps + SPS(AS_INDEX(distinct) - 1), 
                              sps + SPS(AS_INDEX(eq) - 1))
                   && spEqual(sps + SPS(AS_INDEX(distinct) + 1), 
                              sps + SPS(AS_INDEX(eq) + 1))) {
                     break;
                  }
               }
               if (displayRepeat) {
                  if (eq == -1) {
                     printf("%c %s %s: %d%s", GLYPH(pc), pieces[pc][path], orToString(or), distinct, EOL);
                  } else {
                     printf("(%c %s %s == %d)%s", GLYPH(pc), pieces[pc][path], orToString(or), eq, EOL);
                  }
                  displayWide(eCube, sps + SPS(AS_INDEX(distinct)));
               }
               if (eq < 0) {
                  ++distinct;
               } else {
                  const TOrient* por = &orients[or];
                  repeat[pc][path][por->align][por->fwd[eY]][por->fwd[eX]] = 1;
               }
            }
         }
         if (displayRepeat) {
            displayWide(eCube, 0);
         }
      }
   }
   free(sps);
}
