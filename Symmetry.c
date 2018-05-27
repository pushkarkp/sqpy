/**
 * Copyright 2018 Pushkar Piggott
 *
 * Symmetry.c
 */

#include "Symmetry.h"
#include "SetOf.h"
#include "Display.h"

#include <stdio.h>
#include <string.h>

///////////////////////////////////////////////////////////////////////////////
const char* rotationToString(int rot) {
   switch (rot) {
      case e0: return "0";
      case e180: return "180";
      case e90: return "90";
      default: return "unknown";
   }
}

///////////////////////////////////////////////////////////////////////////////
const char* reflectionPlaneToString(int rp) { 
   switch (rp) {
      case e100Reflection: return "100";
      case e010Reflection: return "010";
      case e110Reflection: return "110";
      case e1T0Reflection: return "1T0";
      default: return "unknown";
   }
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
         return ~soo90 & SET_ALL_OF(eOrientations);
      case e180:
         return ~soo180 & SET_ALL_OF(eOrientations);
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
   if (!(p[eZ] & 1)) {
      if (p[eX] == p[eZ] / 2) {
         sorp = SET_WITH(sorp, e100Reflection);
      }
      if (p[eY] == p[eZ] / 2) {
         sorp = SET_WITH(sorp, e010Reflection);
      }
   }
   if (p[eX] == p[eY]) {
      sorp = SET_WITH(sorp, e110Reflection);
   }
   if (p[eX] == p[eZ] - p[eY]) {
      sorp = SET_WITH(sorp, e1T0Reflection);
   }
   return sorp;
}

///////////////////////////////////////////////////////////////////////////////
void showOrientationsSymmetry(TSetOfOrientations soor) {
   printf("Orientation    Symmetry%s", EOL);
   TSetOfOrientations skip90 = rotateSkip(e90);
   TSetOfOrientations skip180 = rotateSkip(e180);
   TSetOfOrientations skip100 = reflectSkip(SET_WITH(0, e100Reflection));
   TSetOfOrientations skip010 = reflectSkip(SET_WITH(0, e010Reflection));
   TSetOfOrientations skip110 = reflectSkip(SET_WITH(0, e110Reflection));
   TSetOfOrientations skip1T0 = reflectSkip(SET_WITH(0, e1T0Reflection));
   EOrientation or;
   for (or = 0; or < eOrientations; ++or) {
      if (soor != 0 && !SET_HAS(soor, or)) {
         continue;
      }
      const char* isSkip = (SET_HAS(skip90, or) || SET_HAS(skip180, or) 
                         || SET_HAS(skip100, or) || SET_HAS(skip010, or) 
                         || SET_HAS(skip110, or) || SET_HAS(skip1T0, or)) ? "" : "no ";
      printf("%s (%s%s%s%s%s%s%sskip)%s", orToString(or), isSkip, 
               SET_HAS(skip90, or) ? "90 " : "",
               SET_HAS(skip180, or) ? "180 " : "",
               SET_HAS(skip100, or) ? "100 " : "",
               SET_HAS(skip010, or) ? "010 " : "",
               SET_HAS(skip110, or) ? "110 " : "",
               SET_HAS(skip1T0, or) ? "1T0 " : "",
               EOL);
   }
}

