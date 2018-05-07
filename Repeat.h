/**
 * Copyright 2018 Pushkar Piggott
 *
 * Repeat.h
 */
#ifndef RepeatH
#define RepeatH

#include "Position.h"

///////////////////////////////////////////////////////////////////////////////
// Whether a piece path at an orientation repeats a previous path orientation.
typedef char TPathRepeat[e2dDimensions][eSigns][eSigns];
extern TPathRepeat** repeat;
void findRepeat();

#endif
