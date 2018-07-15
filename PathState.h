/**
 * Copyright 2018 Pushkar Piggott
 *
 * PathState.h
 */
#ifndef PathStateH
#define PathStateH

#include "Position.h"
#include "Piece.h"

///////////////////////////////////////////////////////////////////////////////
typedef enum{/*eAsterisk, ePlusSign, */eComma, eDash, eDot, eSlash, ePathMarkers} EPathMarker;
//#define PATH_MARKERS "*+,-./"
#define MIN_MARKER ','
#define MAX_MARKER '/'
#define IS_MARKER(c) (c >= MIN_MARKER && c <= MAX_MARKER)
#define MARKER_INDEX(c) (c - MIN_MARKER)
#define AS_MARKER(i) (i + MIN_MARKER)

#define PATH_MARKER_STORES {{-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}}

///////////////////////////////////////////////////////////////////////////////
const char* psRead(TPosition*, TPath, TPosition[][eDimensions]);

#endif
