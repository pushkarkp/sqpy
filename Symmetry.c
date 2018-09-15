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
      default: return "";
   }
}

///////////////////////////////////////////////////////////////////////////////
const char* reflectionPlaneToString(int rp) { 
   switch (rp) {
      case eXReflection: return "x";
      case eYReflection: return "y";
      case eBReflection: return "b";
      case eDReflection: return "d";
      default: return "";
   }
}

///////////////////////////////////////////////////////////////////////////////
int reflectionPlaneToChar(int rp) { 
   switch (rp) {
      case eXReflection: return 'x';
      case eYReflection: return 'y';
      case eBReflection: return 'b';
      case eDReflection: return 'd';
      default: return ' ';
   }
}

///////////////////////////////////////////////////////////////////////////////
TSetOfReflectionPlanes onReflectionPlanes(const TPosition* p) {
//char buf[POS_BUF_SIZE];
//printf("%s ", posToString(buf, p));
   TSetOfReflectionPlanes sorp = 0;
   if (!(p[eZ] & 1)) {
      if (p[eX] == p[eZ] / 2) {
//printf("y: p[eX] (%d) == p[eZ] / 2 (%d) ", p[eX], p[eZ] / 2);
         sorp = SET_WITH(sorp, eXReflection);
      }
      if (p[eY] == p[eZ] / 2) {
//printf("x: p[eY] (%d) == p[eZ] / 2 (%d) ", p[eY], p[eZ] / 2);
         sorp = SET_WITH(sorp, eYReflection);
      }
   }
   if (p[eX] == p[eY]) {
//printf("b: p[eX] (%d) == p[eY] (%d) ", p[eX], p[eY]);
      sorp = SET_WITH(sorp, eBReflection);
   }
   if (p[eX] == p[eZ] - p[eY]) {
//printf("d: p[eX] (%d) == (%d) p[eZ] (%d) - p[eY] (%d) ", p[eX], p[eZ] - p[eY], p[eZ], p[eY]);
      sorp = SET_WITH(sorp, eDReflection);
   }
//printf("\r\n");
   return sorp;
}
