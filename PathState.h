/**
 * Copyright 2018 Pushkar Piggott
 *
 * PathState.h
 */
#ifndef PathStateH
#define PathStateH

#include "Position.h"

///////////////////////////////////////////////////////////////////////////////
typedef enum{eDot, eComma, ePositionStores} EPositionStore;

///////////////////////////////////////////////////////////////////////////////
#define PS_INIT(ps) memset((ps), -1, sizeof(TPosition) * eDimensions * ePositionStores)

///////////////////////////////////////////////////////////////////////////////
const char* psRead(TPosition*, const char*, TPosition[][eDimensions]);

#endif
