/**
 * Copyright 2018 Pushkar Piggott
 *
 * Position.c
 */

#include "Position.h"

#include <string.h>
#include <stdio.h>

///////////////////////////////////////////////////////////////////////////////
const char* planeToString(EPlane p) {
   switch (p) {
      case e001: return "001";
      case e110: return "110";
      case e1T0: return "1T0";
      default: return "Unknown plane";
   }
}

///////////////////////////////////////////////////////////////////////////////
EPlane stringToPlane(const char* s) {
   if (strncmp("001", s, 3) == 0) {
      return e001;
   } else if (strncmp("110", s, 3) == 0) {
      return e110;
   } else if (strncmp("1T0", s, 3) == 0) {
      return e1T0;
   } else {
      return -1;
   }
}

///////////////////////////////////////////////////////////////////////////////
const char* dimToString(EDimension dim) {
   switch (dim) {
      case eX: return "x";
      case eY: return "y";
      case eZ: return "z";
      default: return "Unknown dimension";
   }
}

///////////////////////////////////////////////////////////////////////////////
EDimension charToDim(int c) {
   switch (c) {
      case 'X': case 'x': return eX;
      case 'Y': case 'y': return eY;
      case 'Z': case 'z': return eZ;
      default: return -1;
   }
}

///////////////////////////////////////////////////////////////////////////////
const char* signToString(ESign s) {
   switch (s) {
      case ePlus: return "+";
      case eMinus: return "-";
      default: return "Unknown sign";
   }
}

///////////////////////////////////////////////////////////////////////////////
ESign charToSign(int c) {
   switch (c) {
      case '+': return ePlus;
      case '-': return eMinus;
      default: return -1;
   }
}

///////////////////////////////////////////////////////////////////////////////
const char* posToString(char* buf, const TPosition* p) {
   sprintf(buf, "(%d,%d,%d)", p[eX], p[eY], p[eZ]);
   return buf;
}
