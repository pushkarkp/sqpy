/**
 * Copyright 2018 Pushkar Piggott
 *
 * PathState.c
 */

#include "PathState.h"

#include <string.h>
#include <stdio.h>

///////////////////////////////////////////////////////////////////////////////
TPath psRead(TPosition* pos, TPath path, TPosition ps[][eDimensions]) {
   EPositionStore eStore = ePositionStores;
   switch (*path) {
      case '.': eStore = eDot; break;
      case ',': eStore = eComma; break;
   }
   if (eStore == ePositionStores) {
      return path;
   }
   if (ps[eStore][eX] == -1) {
      POS_COPY(ps[eStore], pos);
   } else {
      POS_COPY(pos, ps[eStore]);
   }
   return path + 1;
}
