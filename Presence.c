/**
 * Copyright 2018 Pushkar Piggott
 *
 * Presence.c
 */

#include "Presence.h"
#include "Piece.h"
#include "SetOf.h"

///////////////////////////////////////////////////////////////////////////////
const char* sopToString(char* buf, TSetOfPresences sop) {
   char* p = buf;
   int i;
   for (i = 0; i < pieceCount; ++i) {
      if (SET_HAS(sop, i)) {
         *p++ = GLYPH(i);
      }
   }
   *p = 0;
   return buf;
}
