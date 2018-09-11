/**
 * Copyright 2018 Pushkar Piggott
 *
 * Position.h
 */
#ifndef PositionH
#define PositionH

///////////////////////////////////////////////////////////////////////////////
typedef enum {eZPlane, eBPlane, eDPlane, ePlanes} EPlane;
int planeToChar(EPlane);
int charToPlane(int);
typedef enum {eX, eY, eZ, e2dDimensions = eZ, eDimensions} EDimension;
int dimToChar(EDimension);
int charToDim(int);
#define INVERT_DIM(eDim) ((eDim) ? eX : eY)
typedef enum {ePlus, eMinus, eSigns} ESign;
int signToChar(ESign);
int charToSign(int);
#define SIGN_AS_INT(eSign) (-((eSign) * 2 - 1))
struct TPosSpace { int x; int y; int z; };
#define POS_COPY(d, s) { *(struct TPosSpace*)(d) = *(struct TPosSpace*)(s); }

///////////////////////////////////////////////////////////////////////////////
typedef int TPosition;
const char* posToString(char*, const TPosition*);
// "(nn,nn,nn)0"
#define POS_BUF_SIZE 11

#endif
