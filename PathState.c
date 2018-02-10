/**
 * Copyright 2018 Pushkar Piggott
 *
 * PathState.c
 */

#include "PathState.h"
#include <string.h>
#include <stdio.h>

///////////////////////////////////////////////////////////////////////////////
void psInit(TPathState* ps) {
   memset(ps, -1, sizeof(TPosition) * ePositionStores);
}

///////////////////////////////////////////////////////////////////////////////
const char* psRead(TPosition* pos, const char* path, TPathState* ps) {
   EPositionStore eStore = ePositionStores;
   switch (*path) {
      case '.': eStore = eDot; break;
      case ',': eStore = eComma; break;
   }
   if (eStore == ePositionStores) {
      return path;
   }
//printf("psRead c %c pos x %d y %d z %d\r\n", *path, pos->d[eX], pos->d[eY], pos->d[eZ]);
   if (ps->pos[eStore].d[eX] == -1) {
      ps->pos[eStore] = *pos;
   } else {
      *pos = ps->pos[eStore];
   }
//printf("psRead pos x %d y %d z %d\r\n", pos->d[eX], pos->d[eY], pos->d[eZ]);
   return path + 1;
}
