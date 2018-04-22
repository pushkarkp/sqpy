/**
 * Copyright 2018 Pushkar Piggott
 *
 * Symmetry.h
 */
#ifndef SymmetryH
#define SymmetryH

#include "Orientation.h"

///////////////////////////////////////////////////////////////////////////////
typedef enum {e0, e180, e90, eRotations} ERotation;
const char* rotationToString(ERotation); 
typedef enum {e100Reflection, e010Reflection, 
              e110Reflection, e1T0Reflection, eReflectionPlanes} EReflectionPlane;
typedef int TSetOfReflectionPlanes;
const char* sorpToString(char*, TSetOfReflectionPlanes);
#define SORP_BUF_SIZE 16 
#define ON_AXIS(p) (!((p)[eZ] & 1) && (p)[eX] == (p)[eY] && (p)[eX] == (p)[eZ] / 2)

///////////////////////////////////////////////////////////////////////////////
TSetOfOrientations rotateSkip(ERotation);
TSetOfOrientations reflectSkip(TSetOfReflectionPlanes);
TSetOfReflectionPlanes onPlanes(const TPosition*);
#endif
