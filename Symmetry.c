/**
 * Copyright 2018 Pushkar Piggott
 *
 * Symmetry.c
 */

#include "Symmetry.h"
#include "Repeat.h"
#include "SetOf.h"
#include "Display.h"

#include <stdio.h>
#include <string.h>

///////////////////////////////////////////////////////////////////////////////
const char* rotationToString(int rot) {
   switch (rot) {
      case e0: return "0";
      case e90: return "90";
      case e180: return "180";
      case e270: return "270";
      default: return "unknown";
   }
}

///////////////////////////////////////////////////////////////////////////////
const char* reflectionPlaneToString(int rp) { 
   switch (rp) {
      case e100Reflection: return "x";
      case e010Reflection: return "y";
      case e110Reflection: return "b";
      case e1T0Reflection: return "d";
      default: return "unknown";
   }
}

///////////////////////////////////////////////////////////////////////////////
int reflectionPlaneToChar(int rp) { 
   switch (rp) {
      case e100Reflection: return 'x';
      case e010Reflection: return 'y';
      case e110Reflection: return 'b';
      case e1T0Reflection: return 'd';
      default: return ' ';
   }
}

///////////////////////////////////////////////////////////////////////////////
TSetOfReflectionPlanes onReflectionPlanes(const TPosition* p) {
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
