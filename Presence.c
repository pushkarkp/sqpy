/**
 * Copyright 2018 Pushkar Piggott
 *
 * Presence.c
 */

#include "Presence.h"
#include "SetOf.h"

///////////////////////////////////////////////////////////////////////////////
const char* presToString(EPresence ePresence) {
   switch (ePresence) {
      case eMargin: return "Margin";
      case eAbsent: return "Absent";
      case eGrey: return "Grey";
      case ePinkPurple: return "PinkPurple";
      case eLightBlue: return "LightBlue";
      case eDarkGreen: return "DarkGreen";
      case eYellow: return "Yellow";
      case eDarkBlue: return "DarkBlue";
      case eRed: return "Red";
      case ePink: return "Pink";
      case eBluePurple: return "BluePurple";
      case eLightGreen: return "LightGreen";
      case eOrange: return "Orange";
      case eWhite: return "White";
      default: return "Unknown presence";
   }
}

///////////////////////////////////////////////////////////////////////////////
const char* sopToString(char* buf, TSetOfPresences sop) {
   char* p = buf;
   int i;
   for (i = 0; i < ePresences; ++i) {
      if (SET_HAS(sop, i)) {
         *p++ = GLYPH(i);
      }
   }
   *p = 0;
   return buf;
}
