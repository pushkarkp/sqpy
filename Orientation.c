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
const char* orToString(EOrientation or) {
   switch (or) {
      case e001XPlusPlus:
         return "001XPlusPlus";
      case e001XMinusPlus:
         return "001XMinusPlus";
      case e001XPlusMinus:
         return "001XPlusMinus"; 
      case e001XMinusMinus:
         return "001XMinusMinus";
      case e001YPlusPlus:
         return "001YPlusPlus";
      case e001YMinusPlus:
         return "001YMinusPlus";
      case e001YPlusMinus:
         return "001YPlusMinus";
      case e001YMinusMinus:
         return "001YMinusMinus";
      case e110XPlusPlus:
         return "110XPlusPlus";
      case e110XMinusPlus:
         return "110XMinusPlus";
      case e110XPlusMinus:
         return "110XPlusMinus";
      case e110XMinusMinus:
         return "110XMinusMinus";
      case e110YPlusPlus:
         return "110YPlusPlus";
      case e110YMinusPlus:
         return "110YMinusPlus";
      case e110YPlusMinus:
         return "110YPlusMinus";
      case e110YMinusMinus:
         return "110YMinusMinus";
      case e1T0XPlusPlus:
         return "1T0XPlusPlus";
      case e1T0XMinusPlus:
         return "1T0XMinusPlus";
      case e1T0XPlusMinus:
         return "1T0XPlusMinus";
      case e1T0XMinusMinus:
         return "1T0XMinusMinus";
      case e1T0YPlusPlus:
         return "1T0YPlusPlus";
      case e1T0YMinusPlus:
         return "1T0YMinusPlus";
      case e1T0YPlusMinus:
         return "1T0YPlusMinus";
      case e1T0YMinusMinus:
         return "1T0YMinusMinus";
   } 
}

///////////////////////////////////////////////////////////////////////////////
const char* soorToString(char* buf, TSetOfOrientations soor) {
   *buf = 0;
   const char* sep = "";
   EOrientation or;
   for (int or = 0; or < eOrientations; ++or) {
      if (SET_HAS(soor, or)) {
         strcat(buf, sep);
         strcat(buf, orToString(or));
         sep = " ";
      }
   }
   return buf;
}

///////////////////////////////////////////////////////////////////////////////
const TOrient orients[ORIENTATIONS] = {
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
   sprintf(buf, "%s %s (%s, %s)", 
           planeToString(or->plane),
           dimToString(or->align),
           signToString(or->fwd[eX]),
           signToString(or->fwd[eY]));
   return buf;
}
