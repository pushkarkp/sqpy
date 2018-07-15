 /**
 * Copyright 2018 Pushkar Piggott
 *
 * Orientation.c
 */

#include "Orientation.h"
#include "SetOf.h"

#include <stdio.h>
#include <string.h>

///////////////////////////////////////////////////////////////////////////////
const char* orToString(int or) {
   switch (or) {
      case e001XPlusPlus:
         return "001x++";
      case e001XMinusPlus:
         return "001x-+";
      case e001XPlusMinus:
         return "001x+-"; 
      case e001XMinusMinus:
         return "001x--";
      case e001YPlusPlus:
         return "001y++";
      case e001YMinusPlus:
         return "001y-+";
      case e001YPlusMinus:
         return "001y+-";
      case e001YMinusMinus:
         return "001y--";
      case e110XPlusPlus:
         return "110x++";
      case e110XMinusPlus:
         return "110x-+";
      case e110XPlusMinus:
         return "110x+-";
      case e110XMinusMinus:
         return "110x--";
      case e110YPlusPlus:
         return "110y++";
      case e110YMinusPlus:
         return "110y-+";
      case e110YPlusMinus:
         return "110y+-";
      case e110YMinusMinus:
         return "110y--";
      case e1T0XPlusPlus:
         return "1T0x++";
      case e1T0XMinusPlus:
         return "1T0x-+";
      case e1T0XPlusMinus:
         return "1T0x+-";
      case e1T0XMinusMinus:
         return "1T0x--";
      case e1T0YPlusPlus:
         return "1T0y++";
      case e1T0YMinusPlus:
         return "1T0y-+";
      case e1T0YPlusMinus:
         return "1T0y+-";
      case e1T0YMinusMinus:
         return "1T0y--";
      default:
         return "bad orientation";
   } 
}

///////////////////////////////////////////////////////////////////////////////
EOrientation stringToOr(const char* s) {
   EPlane plane = stringToPlane(s);
   EDimension dim = charToDim(s[3]);
   ESign sign[2] = {charToSign(s[5]), charToSign(s[4])};
   if (plane == -1 || dim == -1
    || sign[0] == -1 || sign[1] == -1) {
      return -1;
   }
   return plane * 2 * eSigns * eSigns
        + dim * eSigns * eSigns
        + sign[0] * eSigns
        + sign[1];
}
 
///////////////////////////////////////////////////////////////////////////////
const TOrient orients[eOrientations] = {
   {e001, eX, {ePlus, ePlus}},
   {e001, eX, {eMinus, ePlus}},
   {e001, eX, {ePlus, eMinus}},
   {e001, eX, {eMinus, eMinus}},
   {e001, eY, {ePlus, ePlus}},
   {e001, eY, {eMinus, ePlus}},
   {e001, eY, {ePlus, eMinus}},
   {e001, eY, {eMinus, eMinus}},
   {e110, eX, {ePlus, ePlus}},
   {e110, eX, {eMinus, ePlus}},
   {e110, eX, {ePlus, eMinus}},
   {e110, eX, {eMinus, eMinus}},
   {e110, eY, {ePlus, ePlus}},
   {e110, eY, {eMinus, ePlus}},
   {e110, eY, {ePlus, eMinus}},
   {e110, eY, {eMinus, eMinus}},
   {e1T0, eX, {ePlus, ePlus}},
   {e1T0, eX, {eMinus, ePlus}},
   {e1T0, eX, {ePlus, eMinus}},
   {e1T0, eX, {eMinus, eMinus}},
   {e1T0, eY, {ePlus, ePlus}},
   {e1T0, eY, {eMinus, ePlus}},
   {e1T0, eY, {ePlus, eMinus}},
   {e1T0, eY, {eMinus, eMinus}}
};

///////////////////////////////////////////////////////////////////////////////
const char* orientToString(char* buf, const TOrient* or) {
   sprintf(buf, "%s%s%s%s", 
           planeToString(or->plane),
           dimToString(or->align),
           signToString(or->fwd[eX]),
           signToString(or->fwd[eY]));
   return buf;
}

///////////////////////////////////////////////////////////////////////////////
TSetOfOrientations matchOrientation(const char* s) {
   if (!s) {
      return 0;
   }
   int len = strlen(s);
   TSetOfOrientations soo = 0;
   EOrientation o;
   for (o = 0; o < eOrientations; ++o) {
      if (0 == strncasecmp(orToString(o), s, len)) {
         soo = SET_WITH(soo, o);
      }
   }
   return soo;
}
