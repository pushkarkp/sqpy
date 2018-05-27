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
const char* rotationToString(int); 
typedef enum {e100Reflection, e010Reflection, 
              e110Reflection, e1T0Reflection, eReflectionPlanes} EReflectionPlane;
const char* reflectionPlaneToString(int); 
typedef int TSetOfReflectionPlanes;
#define ON_AXIS(p) (!((p)[eZ] & 1) && (p)[eX] == (p)[eY] && (p)[eX] == (p)[eZ] / 2)

///////////////////////////////////////////////////////////////////////////////
TSetOfOrientations rotateSkip(ERotation);
TSetOfOrientations reflectSkip(TSetOfReflectionPlanes);
TSetOfReflectionPlanes onPlanes(const TPosition*);
void showOrientationsSymmetry(TSetOfOrientations);
#endif
