/**
 * Copyright 2018 Pushkar Piggott
 *
 * PathState.c
 */

#include "PathState.h"
#include <string.h>
#include <stdio.h>

///////////////////////////////////////////////////////////////////////////////
const char* psRead(TPosition* pos, const char* path, TPosition* ps) {
   EPositionStore eStore = ePositionStores;
   switch (*path) {
      case '.': eStore = eDot; break;
      case ',': eStore = eComma; break;
   }
   if (eStore == ePositionStores) {
      return path;
   }
   if (ps[eStore].d[eX] == -1) {
      ps[eStore] = *pos;
   } else {
      *pos = ps[eStore];
   }
   return path + 1;
}
