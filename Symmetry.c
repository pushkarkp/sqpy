/**
 * Copyright 2018 Pushkar Piggott
 *
 * Symmetry.c
 */

#include "Symmetry.h"
#include "SetOf.h"

#include <string.h>

///////////////////////////////////////////////////////////////////////////////
const char* rotationToString(ERotation rot) {
   switch (rot) {
      case e0: return "0";
      case e180: return "180";
      case e90: return "90";
      default: return "unknown rotation";
   }
}

///////////////////////////////////////////////////////////////////////////////
const char* sorpToString(char* buf, TSetOfReflectionPlanes sorp) {
   if (!sorp) {
      return "none";
   }
   if (sorp > 15) {
      return "unknown plane";
   }
   *buf = 0;
   const char* sep = "";
   int i;
   for (i = 0; i < eReflectionPlanes; ++i) {
      if (SET_HAS(sorp, i)) {
         strcat(buf, sep);
         sep = " ";
         switch (i) {
            case e100Reflection: strcat(buf, "100"); break;
            case e010Reflection: strcat(buf, "010"); break;
            case e110Reflection: strcat(buf, "110"); break;
            case e1T0Reflection: strcat(buf, "1T0"); break;
         }
      }
   }
   return buf;
}

///////////////////////////////////////////////////////////////////////////////
TSetOfOrientations rotateSkip(ERotation rot) {
   static TSetOfOrientations soo90 = 
      SET_WITH(SET_WITH(SET_WITH(SET_WITH(SET_WITH(SET_WITH(0,
       e001XPlusPlus), e001XMinusPlus),
       e110XPlusPlus), e110XMinusPlus), e110XPlusMinus), e110XMinusMinus);
   static TSetOfOrientations soo180 = 
      SET_WITH(SET_WITH(SET_WITH(SET_WITH(
      SET_WITH(SET_WITH(SET_WITH(SET_WITH(
      SET_WITH(SET_WITH(SET_WITH(SET_WITH(0,
         e001XPlusPlus), e001XMinusPlus), e001YPlusPlus), e001YMinusPlus),
         e110XPlusPlus), e110XMinusPlus), e110XPlusMinus), e110XMinusMinus), 
         e1T0XPlusPlus), e1T0XMinusPlus), e1T0XPlusMinus), e1T0XMinusMinus);
   switch (rot) {
      case e90:
         return ~soo90 & SET_ALL(eOrientations);
      case e180:
         return ~soo180 & SET_ALL(eOrientations);
      case e0:
      default:
         return 0;
   }
}

/* e001XPlusPlus, e001XMinusPlus, e001XPlusMinus, e001XMinusMinus,
   e001YPlusPlus, e001YMinusPlus, e001YPlusMinus, e001YMinusMinus,
   e110XPlusPlus, e110XMinusPlus, e110XPlusMinus, e110XMinusMinus,
   e110YPlusPlus, e110YMinusPlus, e110YPlusMinus, e110YMinusMinus,
   e1T0XPlusPlus, e1T0XMinusPlus, e1T0XPlusMinus, e1T0XMinusMinus,
   e1T0YPlusPlus, e1T0YMinusPlus, e1T0YPlusMinus, e1T0YMinusMinus,*/
///////////////////////////////////////////////////////////////////////////////
TSetOfOrientations reflectSkip(TSetOfReflectionPlanes sorp) {
   static TSetOfOrientations soo[eReflectionPlanes] = {
      SET_WITH(SET_WITH(SET_WITH(SET_WITH(
      SET_WITH(SET_WITH(SET_WITH(SET_WITH(
      SET_WITH(SET_WITH(SET_WITH(SET_WITH(0,
         e001XMinusPlus), e001XMinusMinus), e001YMinusPlus), e001YMinusMinus),
         e110YPlusPlus), e110YMinusPlus), e110YPlusMinus), e110YMinusMinus),
         e1T0YPlusPlus), e1T0YMinusPlus), e1T0YPlusMinus), e1T0YMinusMinus),
      SET_WITH(SET_WITH(SET_WITH(SET_WITH(
      SET_WITH(SET_WITH(SET_WITH(SET_WITH(
      SET_WITH(SET_WITH(SET_WITH(SET_WITH(0,
         e001XPlusMinus), e001XMinusMinus), e001YPlusMinus), e001YMinusMinus),
         e110YPlusPlus), e110YMinusPlus), e110YPlusMinus), e110YMinusMinus),
         e1T0XPlusPlus), e1T0XMinusPlus), e1T0XPlusMinus), e1T0XMinusMinus),
      SET_WITH(SET_WITH(SET_WITH(SET_WITH(
      SET_WITH(SET_WITH(SET_WITH(SET_WITH(0,
         e001YPlusPlus), e001YMinusPlus), e001YPlusMinus), e001YMinusMinus),
         e1T0YPlusPlus), e1T0YMinusPlus), e1T0YPlusMinus), e1T0YMinusMinus),
      SET_WITH(SET_WITH(SET_WITH(SET_WITH(
      SET_WITH(SET_WITH(SET_WITH(SET_WITH(0,
         e001XPlusMinus), e001XMinusMinus), e001YPlusMinus), e001YMinusMinus),
         e110YPlusPlus), e110YMinusPlus), e1T0YPlusMinus), e1T0YMinusMinus)
   };
   if (!sorp) {
      return 0;
   }
   TSetOfOrientations s = 0;
   EReflectionPlane rp;
   for (rp = 0; rp < eReflectionPlanes; ++rp) {
      if (SET_HAS(sorp, rp)) {
         s |= soo[rp];
      }
   }
   return s;
}

///////////////////////////////////////////////////////////////////////////////
TSetOfReflectionPlanes onPlanes(const TPosition* p) {
   TSetOfReflectionPlanes sorp = 0;
   if (!(p->d[eZ] & 1)) {
      if (p->d[eX] == p->d[eZ] / 2) {
         sorp = SET_WITH(sorp, e100Reflection);
      }
      if (p->d[eY] == p->d[eZ] / 2) {
         sorp = SET_WITH(sorp, e010Reflection);
      }
   }
   if (p->d[eX] == p->d[eY]) {
      sorp = SET_WITH(sorp, e110Reflection);
   }
   if (p->d[eX] == p->d[eZ] - p->d[eY]) {
      sorp = SET_WITH(sorp, e1T0Reflection);
   }
   return sorp;
}
