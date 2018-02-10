/**
 * Copyright 2018 Pushkar Piggott
 *
 * Position.h
 */
#ifndef PositionH
#define PositionH

///////////////////////////////////////////////////////////////////////////////
typedef enum {e001, e110, e1T0, ePlanes} EPlane;
const char* planeToString(EPlane);
typedef enum {eX, eY, eZ, e2dDimensions = eZ, eDimensions} EDimension;
const char* dimToString(EDimension);
#define INVERT_DIM(eDim) ((eDim) ? eX : eY)
typedef enum {ePlus, eMinus, eSigns} ESign;
const char* signToString(ESign);
#define SIGN_AS_INT(eSign) (-((eSign) * 2 - 1))

typedef struct {
   EPlane plane;
   EDimension align;
   ESign fwd[e2dDimensions];
} TOrientation;
const char* orToString(char*, const TOrientation*);
TOrientation* orNext(TOrientation*);
#define PLANE_ORIENTATIONS 8

///////////////////////////////////////////////////////////////////////////////
typedef struct {
   int d[eDimensions];
} TPosition;
const char* posToString(char*, const TPosition*);
typedef struct {
   int d[eDimensions];
} TMove;
const char* moveToString(char*, const TMove*);
typedef TMove TMoves[ePlanes][e2dDimensions][eSigns];

///////////////////////////////////////////////////////////////////////////////
extern const TMoves moves;

///////////////////////////////////////////////////////////////////////////////
int charToMove(const TMove**, int, const TOrientation*);
void step(TPosition*, const TMove*);

#endif
