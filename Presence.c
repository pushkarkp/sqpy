/**
 * Copyright 2018 Pushkar Piggott
 *
 * Presence.c
 */

#include "Presence.h"

///////////////////////////////////////////////////////////////////////////////
const char glyph[ePresences] = ".abcdefghijkl";

///////////////////////////////////////////////////////////////////////////////
const char* presenceToString(EPresence ePresence) {
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
