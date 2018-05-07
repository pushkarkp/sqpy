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
typedef enum{eDot, eComma, ePositionStores} EPositionStore;

///////////////////////////////////////////////////////////////////////////////
#define PS_INIT(ps) memset((ps), -1, sizeof(TPosition) * eDimensions * ePositionStores)

///////////////////////////////////////////////////////////////////////////////
const char* psRead(TPosition*, TPath, TPosition[][eDimensions]);

#endif
