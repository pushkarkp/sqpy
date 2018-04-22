/**
 * Copyright 2018 Pushkar Piggott
 *
 * Position.c
 */

#include "Position.h"

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
const char* dimToString(EDimension dim) {
   switch (dim) {
      case eX: return "x";
      case eY: return "y";
      case eZ: return "z";
      default: return "Unknown dimension";
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
const char* posToString(char* buf, const TPosition* p) {
   sprintf(buf, "(%d,%d,%d)", p[eX], p[eY], p[eZ]);
   return buf;
}
