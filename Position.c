/**
 * Copyright 2018 Pushkar Piggott
 *
 * Position.c
 */

#include "Position.h"
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
         {1, 1, 1},
         {-1, -1, -1},
      },
      { // eY Signs
         {0, 0, 1},
         {0, 0, -1},
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
const char* planeToString(EPlane p) {
   switch (p) {
      case e001: return "001";
      case e110: return "110";
      case e1T0: return "1T0";
      default: return "Unknown plane";
   }
}

///////////////////////////////////////////////////////////////////////////////
const char* dimToString(EDimension dim) {
   switch (dim) {
      case eX: return "x";
      case eY: return "y";
      case eZ: return "z";
      default: return "Unknown dimension";
   }
}

///////////////////////////////////////////////////////////////////////////////
const char* signToString(ESign s) {
   switch (s) {
      case ePlus: return "+";
      case eMinus: return "-";
      default: return "Unknown sign";
   }
}

///////////////////////////////////////////////////////////////////////////////
const char* orToString(char* buf, const TOrientation* or) {
   sprintf(buf, "%s %s (%s, %s)", 
           planeToString(or->plane),
           dimToString(or->align),
           signToString(or->fwd[eX]),
           signToString(or->fwd[eY]));
   return buf;
}

///////////////////////////////////////////////////////////////////////////////
const char* posToString(char* buf, const TPosition* pos) {
   sprintf(buf, "(%d, %d, %d)", 
           pos->d[eX], pos->d[eY], pos->d[eZ]);
   return buf;
}

///////////////////////////////////////////////////////////////////////////////
const char* moveToString(char* buf, const TMove* m) {
   sprintf(buf, "(%d, %d, %d)", 
           m->d[eX], m->d[eY], m->d[eZ]);
   return buf;
}

///////////////////////////////////////////////////////////////////////////////
TOrientation* orNext(TOrientation* or) {
   if (or->fwd[eX] + 1 != eSigns) {
      ++or->fwd[eX];
      return or;
   }
   or->fwd[eX] = ePlus;
   if (or->fwd[eY] + 1 != eSigns) {
      ++or->fwd[eY];
      return or;
   }
   or->fwd[eY] = ePlus;
   if (or->align + 1 != e2dDimensions) {
      ++or->align;
      return or;
   }
   or->align = eX;
   if (or->plane + 1 != ePlanes) {
      ++or->plane;
      return or;
   }
   or->plane = e001;
   return 0;
}

///////////////////////////////////////////////////////////////////////////////
int charToMove(const TMove** pmove, int c, const TOrientation* or) {
#define SHIFT 0x20
#define BELOW '`'
#define ABOVE '{'
   EDimension dim = or->align;
   if (c > 'Z') {
      dim = INVERT_DIM(dim);
   } 
//printf("charToMove %c or->plane %s dim %s ", c, planeToString(or->plane), dimToString(dim));
   c |= SHIFT;
   int count = c - ((c < 'n') ? BELOW : ABOVE);
   int sign = SIGN_AS_INT(or->fwd[dim]);
//printf("count %d sign %d ((sign * count) < 0) ? eMinus : ePlus %s ", count, sign, signToString((sign * count) < 0 ? eMinus : ePlus));
   *pmove = &moves[or->plane][dim][(sign * count) < 0 ? eMinus : ePlus];
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
