/**
 * Copyright 2018 Pushkar Piggott
 *
 * Move.c
 */

#include "Move.h"
#include "Path.h"
#include "Display.h"

#include <stdlib.h>
#include <stdio.h>

///////////////////////////////////////////////////////////////////////////////
const TMove moves[ePlanes][e2dDimensions][eSigns][eDimensions] = { // Planes
   { // e001 Dimensions
      { // eX Signs
         {1, 0, 0},
         {-1, 0, 0},
      },
      { // eY Signs
         {0, 1, 0},
         {0, -1, 0},
      }
   }, 
   { // e110 Dimensions
      { // eX Signs
         {0, 0, 1},
         {0, 0, -1},
      },
      { // eY Signs
         {1, 1, 1},
         {-1, -1, -1},
      }
   },
   { // e1T0 Dimensions
      { // eX Signs
         {1, 0, 1},
         {-1, 0, -1},
      },
      { // eY Signs
         {0, 1, 1},
         {0, -1, -1},
      }
   }
};

///////////////////////////////////////////////////////////////////////////////
const char* moveToString(char* buf, const TMove* m) {
   sprintf(buf, "(%d, %d, %d)",  m[eX], m[eY], m[eZ]);
   return buf;
}

///////////////////////////////////////////////////////////////////////////////
int moveFromChar(const TMove** pmove, int c, EOrientation or) {
   const TOrient* por = &orients[or];
   EDimension dim = por->align;
   if (PATH_STEP_IS_SIDEWAYS(c)) {
      dim = INVERT_DIM(dim);
   } 
   int advance = PATH_STEP_ADVANCE(c);
   // minus reverses sign
   int sign = SIGN_AS_INT(por->fwd[dim]);
   *pmove = moves[por->plane][dim][(sign * advance) < 0 ? eMinus : ePlus];
   return abs(advance);
}

///////////////////////////////////////////////////////////////////////////////
void moveStep(TPosition* p, const TMove* m) {
   int i;
   for (i = 0; i < eDimensions; ++i) {
      p[i] += m[i];
   }
}
