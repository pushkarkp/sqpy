/**
 * Copyright 2018 Pushkar Piggott
 *
 * Presence.c
 */

#include "Presence.h"

///////////////////////////////////////////////////////////////////////////////
const char* presToString(EPresence pc) {
   switch (pc) {
      case -1:
         return "_";
      case 0:
         return "`";
      case 1:
         return "a";
      case 2:
         return "b";
      case 3:
         return "c"; 
      case 4:
         return "d";
      case 5:
         return "e";
      case 6:
         return "f";
      case 7:
         return "g";
      case 8:
         return "h";
      case 9:
         return "i";
      case 10:
         return "j";
      case 11:
         return "k";
      case 12:
         return "l";
      case 13:
         return "m";
      case 14:
         return "n";
      case 15:
         return "o";
      case 16:
         return "p";
      case 17:
         return "q";
      case 18:
         return "r";
      case 19:
         return "s";
      case 20:
         return "t";
      case 21:
         return "u";
      case 22:
         return "v";
      case 23:
         return "w";
      case 24:
         return "x";
      case 25:
         return "y";
      case 26:
         return "z";
      default:
         return "x";
   } 
}
