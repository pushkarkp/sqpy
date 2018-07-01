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
   EPathMarker pm = MARKER_INDEX(*path);
   if (pm < 0 || pm >= ePathMarkers) {
      return path;
   }
   if (ps[pm][eX] == -1) {
      POS_COPY(ps[pm], pos);
   } else {
      POS_COPY(pos, ps[pm]);
   }
   return path + 1;
}
