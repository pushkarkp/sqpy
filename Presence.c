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
   int pc;
   for (pc = eFirstPiece; pc < pieceCount; ++pc) {
      if (SET_HAS(sop, pc)) {
         *p++ = GLYPH(pc);
      }
   }
   *p = 0;
   return buf;
}
