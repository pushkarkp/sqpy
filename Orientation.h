/**
 * Copyright 2018 Pushkar Piggott
 *
 * Orientation.h
 */
#ifndef OrientationH
#define OrientationH

#include "Position.h"

///////////////////////////////////////////////////////////////////////////////
typedef enum {
   e001XPlusPlus, e001XMinusPlus, e001XPlusMinus, e001XMinusMinus,
   e001YPlusPlus, e001YMinusPlus, e001YPlusMinus, e001YMinusMinus,
   e110XPlusPlus, e110XMinusPlus, e110XPlusMinus, e110XMinusMinus,
   e110YPlusPlus, e110YMinusPlus, e110YPlusMinus, e110YMinusMinus,
   e1T0XPlusPlus, e1T0XMinusPlus, e1T0XPlusMinus, e1T0XMinusMinus,
   e1T0YPlusPlus, e1T0YMinusPlus, e1T0YPlusMinus, e1T0YMinusMinus,
   eOrientations
} EOrientation;
const char* orToString(EOrientation);
typedef int TSetOfOrientations;
typedef struct {
   EPlane plane;
   EDimension align;
   ESign fwd[e2dDimensions];
} TOrient;
const char* orientToString(char*, const TOrient*);
#define GET_ORIENTATION(plane, align, fwdY, fwdX) ((plane) * e2dDimensions * 2 * eSigns + (align) * 2 * eSigns + (fwdY) * eSigns + (fwdX))
#define PLANE_ORIENTATIONS 8
#define ORIENTATIONS (PLANE_ORIENTATIONS * ePlanes)

///////////////////////////////////////////////////////////////////////////////
extern const TOrient orients[ORIENTATIONS];
#endif