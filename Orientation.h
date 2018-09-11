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
   eZxPlusPlus, eZxMinusPlus, eZxPlusMinus, eZxMinusMinus,
   eZyPlusPlus, eZyMinusPlus, eZyPlusMinus, eZyMinusMinus,
   eBxPlusPlus, eBxMinusPlus, eBxPlusMinus, eBxMinusMinus,
   eByPlusPlus, eByMinusPlus, eByPlusMinus, eByMinusMinus,
   eDxPlusPlus, eDxMinusPlus, eDxPlusMinus, eDxMinusMinus,
   eDyPlusPlus, eDyMinusPlus, eDyPlusMinus, eDyMinusMinus,
   eOrientations
} EOrientation;
const char* orToString(int);
int orientIsShort(const char*);
int orientOk(const char*);
int orientReport(const char*, int start);
EOrientation stringToOr(const char*);
#define OR_BUF_SIZE 5
typedef int TSetOfOrientations;
#define OR_Z_OFFSET eZxPlusPlus
#define OR_Z_COUNT eBxPlusPlus
#define OR_B_OFFSET OR_Z_COUNT
#define OR_D_OFFSET eDxPlusPlus
#define OR_Z_SET SET_ALL_OF(OR_Z_COUNT)
#define OR_BY_SET SET_WITH(SET_WITH(SET_WITH(SET_WITH(0, \
         eByPlusPlus), eByMinusPlus), eByPlusMinus), eByMinusMinus)
#define OR_DX_SET SET_WITH(SET_WITH(SET_WITH(SET_WITH(0, \
         eDxPlusPlus), eDxMinusPlus), eDxPlusMinus), eDxMinusMinus)
#define OR_DY_SET SET_WITH(SET_WITH(SET_WITH(SET_WITH(0, \
         eDyPlusPlus), eDyMinusPlus), eDyPlusMinus), eDyMinusMinus)
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
