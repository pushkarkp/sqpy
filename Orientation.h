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
const char* orToString(int);
typedef int TSetOfOrientations;
#define OR_001_OFFSET e001XPlusPlus
#define OR_001_COUNT e110XPlusPlus
#define OR_110_OFFSET OR_001_COUNT
#define OR_1T0_OFFSET e1T0XPlusPlus
#define OR_001_SET SET_ALL_OF(OR_001_COUNT)
TSetOfOrientations matchOrientation(const char*);

///////////////////////////////////////////////////////////////////////////////
typedef struct {
   EPlane plane;
   EDimension align;
   ESign fwd[e2dDimensions];
} TOrient;
const char* orientToString(char*, const TOrient*);
#define ORIENT_BUF_SIZE 36

///////////////////////////////////////////////////////////////////////////////
extern const TOrient orients[eOrientations];
#endif
