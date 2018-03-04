/**
 * Copyright 2018 Pushkar Piggott
 *
 * Move.c
 */

#include "Move.h"

#include <stdlib.h>
#include <stdio.h>

///////////////////////////////////////////////////////////////////////////////
const TMoves moves = { // Planes
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
         {-1, 1, 1},
         {1, -1, -1},
      },
      { // eY Signs
         {1, -1, 1},
         {-1, 1, -1},
      }
   }
};

///////////////////////////////////////////////////////////////////////////////
const char* moveToString(char* buf, const TMove* m) {
   sprintf(buf, "(%d, %d, %d)", 
           m->d[eX], m->d[eY], m->d[eZ]);
   return buf;
}

///////////////////////////////////////////////////////////////////////////////
// Converts one char from a path to a move.
// lower case -> given axis, upper -> other axis
// [Aa].. is positive, ..[Zz] is negative
int charToMove(const TMove** pmove, int c, EOrientation or) {
// bit distinguishing upper and lower case
#define SHIFT 0x20
// last before 'a'
#define BELOW '`'
// first after 'z'
#define ABOVE '{'
   const TOrient* por = &orients[or];
   EDimension dim = por->align;
   if (c <= 'Z') {
      // upper case -> other axis
      dim = INVERT_DIM(dim);
   } 
//printf("charToMove %c por->plane %s dim %s ", c, planeToString(por->plane), dimToString(dim));
   // henceforth ignore case (use lower)
   c |= SHIFT;
   // convert char to the distance from nearest end of the alphabet
   int count = c - ((c < 'n') ? BELOW : ABOVE);
   // minus reverses sign
   int sign = SIGN_AS_INT(por->fwd[dim]);
//printf("count %d sign %d ((sign * count) < 0) ? eMinus : ePlus %s ", count, sign, signToString((sign * count) < 0 ? eMinus : ePlus));
   *pmove = &moves[por->plane][dim][(sign * count) < 0 ? eMinus : ePlus];
//char buf[16];
//printf("%s\r\n", moveToString(buf, *pmove));
   return abs(count);
}

///////////////////////////////////////////////////////////////////////////////
void step(TPosition* p, const TMove* m) {
   int i;
   for (i = 0; i < eDimensions; ++i) {
      p->d[i] += m->d[i];
   }
}
