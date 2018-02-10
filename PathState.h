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
typedef struct {
   TPosition pos[ePositionStores];
} TPathState;

///////////////////////////////////////////////////////////////////////////////
void psInit(TPathState*);
const char* psRead(TPosition*, const char*, TPathState*);

#endif
