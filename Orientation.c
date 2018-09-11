 /**
 * Copyright 2018 Pushkar Piggott
 *
 * Orientation.c
 */

#include "Orientation.h"
#include "SetOf.h"
#include "Display.h"

#include <stdio.h>
#include <string.h>

///////////////////////////////////////////////////////////////////////////////
const TOrient orients[eOrientations] = {
   {eZPlane, eX, {ePlus, ePlus}},
   {eZPlane, eX, {eMinus, ePlus}},
   {eZPlane, eX, {ePlus, eMinus}},
   {eZPlane, eX, {eMinus, eMinus}},
   {eZPlane, eY, {ePlus, ePlus}},
   {eZPlane, eY, {eMinus, ePlus}},
   {eZPlane, eY, {ePlus, eMinus}},
   {eZPlane, eY, {eMinus, eMinus}},
   {eBPlane, eX, {ePlus, ePlus}},
   {eBPlane, eX, {eMinus, ePlus}},
   {eBPlane, eX, {ePlus, eMinus}},
   {eBPlane, eX, {eMinus, eMinus}},
   {eBPlane, eY, {ePlus, ePlus}},
   {eBPlane, eY, {eMinus, ePlus}},
   {eBPlane, eY, {ePlus, eMinus}},
   {eBPlane, eY, {eMinus, eMinus}},
   {eDPlane, eX, {ePlus, ePlus}},
   {eDPlane, eX, {eMinus, ePlus}},
   {eDPlane, eX, {ePlus, eMinus}},
   {eDPlane, eX, {eMinus, eMinus}},
   {eDPlane, eY, {ePlus, ePlus}},
   {eDPlane, eY, {eMinus, ePlus}},
   {eDPlane, eY, {ePlus, eMinus}},
   {eDPlane, eY, {eMinus, eMinus}}
};

///////////////////////////////////////////////////////////////////////////////
const char* orToString(int or) {
   if (or < 0 || or > eOrientations) {
         return "bad orientation";
   } 
   static char buf[eOrientations][OR_BUF_SIZE];
   return orientToString(buf[or], &orients[or]);
}

///////////////////////////////////////////////////////////////////////////////
int orientIsShort(const char* s) {
   return charToSign(s[1]) == -1;
}
   
///////////////////////////////////////////////////////////////////////////////
EOrientation stringToOr(const char* s) {
   EPlane plane = charToPlane(s[0]);
   EDimension dim = eX;
   ESign sign[2] = {ePlus, ePlus};
   if (charToSign(s[1]) != -1) {
      sign[0] = charToSign(s[1]);
      sign[1] = charToSign(s[2]);
      dim = charToDim(s[3]);
   }
   if (plane == -1 || dim == -1
    || sign[0] == -1 || sign[1] == -1) {
      return -1;
   }
   return plane * 2 * eSigns * eSigns
        + sign[1] * eSigns
        + sign[0]
        + dim * eSigns * eSigns;
}
 
///////////////////////////////////////////////////////////////////////////////
const char* orientToString(char* buf, const TOrient* or) {
   sprintf(buf, "%c%c%c%c", 
           planeToChar(or->plane),
           signToChar(or->fwd[eX]),
           signToChar(or->fwd[eY]),
           dimToChar(or->align));
   return buf;
}

///////////////////////////////////////////////////////////////////////////////
int orientOk(const char* s) {
   return charToPlane(s[0]) != -1
       && (charToSign(s[1]) == -1
        || (charToSign(s[1]) != -1
         && charToSign(s[2]) != -1
         && charToDim(s[3]) != -1));
}

///////////////////////////////////////////////////////////////////////////////
int orientReport(const char* s, int start) {
   typedef int (*fun)(int);
   static fun cvt[4] = {charToPlane, charToDim, charToSign, charToSign};
   static char* msg[4] = {"plane", "dimension", "sign", "sign"};
   int i;
   for (i = 0; i < 4; ++i) {
      if (cvt[i](s[start + i]) == -1) {
         if (i == 1) {
            return 1;
         }
         printf("%s%s%*c - %s expected%s", s, EOL, start + i + 1, '^', msg[i], EOL);
         return 0;
      }
   }
   return 1;
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
