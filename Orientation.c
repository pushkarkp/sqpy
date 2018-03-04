/**
 * Copyright 2018 Pushkar Piggott
 *
 * Orientation.c
 */

#include "Orientation.h"

#include <stdio.h>

///////////////////////////////////////////////////////////////////////////////
const char* orToString(EOrientation or) {
   switch (or) {
      case e001XPlusPlus:
         return "e001XPlusPlus";
      case e001XMinusPlus:
         return "e001XMinusPlus";
      case e001XPlusMinus:
         return "e001XPlusMinus"; 
      case e001XMinusMinus:
         return "e001XMinusMinus";
      case e001YPlusPlus:
         return "e001YPlusPlus";
      case e001YMinusPlus:
         return "e001YMinusPlus";
      case e001YPlusMinus:
         return "e001YPlusMinus";
      case e001YMinusMinus:
         return "e001YMinusMinus";
      case e110XPlusPlus:
         return "e110XPlusPlus";
      case e110XMinusPlus:
         return "e110XMinusPlus";
      case e110XPlusMinus:
         return "e110XPlusMinus";
      case e110XMinusMinus:
         return "e110XMinusMinus";
      case e110YPlusPlus:
         return "e110YPlusPlus";
      case e110YMinusPlus:
         return "e110YMinusPlus";
      case e110YPlusMinus:
         return "e110YPlusMinus";
      case e110YMinusMinus:
         return "e110YMinusMinus";
      case e1T0XPlusPlus:
         return "e1T0XPlusPlus";
      case e1T0XMinusPlus:
         return "e1T0XMinusPlus";
      case e1T0XPlusMinus:
         return "e1T0XPlusMinus";
      case e1T0XMinusMinus:
         return "e1T0XMinusMinus";
      case e1T0YPlusPlus:
         return "e1T0YPlusPlus";
      case e1T0YMinusPlus:
         return "e1T0YMinusPlus";
      case e1T0YPlusMinus:
         return "e1T0YPlusMinus";
      case e1T0YMinusMinus:
         return "e1T0YMinusMinus";
   } 
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
