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
struct TPosSpace { int x; int y; int z; };
#define POS_COPY(d, s) { *(struct TPosSpace*)(d) = *(struct TPosSpace*)(s); }

///////////////////////////////////////////////////////////////////////////////
typedef int TPosition;
const char* posToString(char*, const TPosition*);
#define POS_BUF_SIZE 10

#endif
