
/**
 * Copyright 2018 Pushkar Piggott
 *
 * Symmetry.h
 */
#ifndef SymmetryH
#define SymmetryH

#include "Orientation.h"

///////////////////////////////////////////////////////////////////////////////
typedef enum {e0, e90, e180, e270, eRotations} ERotation;
const char* rotationToString(int); 
typedef int TSetOfRotations;
typedef enum {e100Reflection, e010Reflection, 
              e110Reflection, e1T0Reflection, eReflectionPlanes} EReflectionPlane;
const char* reflectionPlaneToString(int); 
int reflectionPlaneToChar(int); 
typedef int TSetOfReflectionPlanes;
TSetOfReflectionPlanes onReflectionPlanes(const TPosition*);
#define ON_AXIS(p) (!((p)[eZ] & 1) && (p)[eX] == (p)[eZ] / 2 && (p)[eX] == (p)[eY])
#endif
