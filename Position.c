/**
 * Copyright 2018 Pushkar Piggott
 *
 * Position.c
 */

#include "Position.h"

#include <string.h>
#include <stdio.h>

///////////////////////////////////////////////////////////////////////////////
int planeToChar(EPlane p) {
   switch (p) {
      case e001: return 'z';
      case e110: return 'b';
      case e1T0: return 'd';
      default: return ' ';
   }
}

///////////////////////////////////////////////////////////////////////////////
EPlane charToPlane(int c) {
   switch (c) {
      case 'z': return e001;
      case 'b': return e110;
      case 'd': return e1T0;
      default: return -1;
   }
}

///////////////////////////////////////////////////////////////////////////////
int dimToChar(EDimension dim) {
   switch (dim) {
      case eX: return 'x';
      case eY: return 'y';
      case eZ: return 'z';
      default: return ' ';
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
int signToChar(ESign s) {
   switch (s) {
      case ePlus: return '+';
      case eMinus: return '-';
      default: return ' ';
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
