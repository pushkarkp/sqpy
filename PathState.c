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
//printf("psRead c %c pos x %d y %d z %d\r\n", *path, pos->d[eX], pos->d[eY], pos->d[eZ]);
   if (ps[eStore].d[eX] == -1) {
      ps[eStore] = *pos;
   } else {
      *pos = ps[eStore];
   }
//printf("psRead pos x %d y %d z %d\r\n", pos->d[eX], pos->d[eY], pos->d[eZ]);
   return path + 1;
}
