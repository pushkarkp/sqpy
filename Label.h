/**
 * Copyright 2018 Pushkar Piggott
 *
 * Label.h
 */
#ifndef LabelH
#define LabelH

#include "SetOf.h"
#include "Piece.h"

///////////////////////////////////////////////////////////////////////////////
typedef int TLabel;

#define LABEL_BITS (sizeof(int) * 8)
#define LABEL_FULL_BIT (1 << LABEL_BITS - 1)
#define LABEL_SET_FULL(lab) ((lab) | LABEL_FULL_BIT)
#define LABEL_IS_FULL(lab) ((lab) & LABEL_FULL_BIT)
#define LABEL_NO_FULL(lab) ((lab) & ~LABEL_FULL_BIT)
#define LABEL_SHAPES (SET_ALL_OF(pieceCount))
#define LABEL_GET_SHAPES(lab) ((lab) & LABEL_SHAPES)
#define LABEL_GET_PLAY_COUNT(lab) (LABEL_NO_FULL(lab) >> (LABEL_BITS - 8))
#define LABEL_MAKE(full, plays, shapes) ((full) ? LABEL_FULL_BIT : 0 | (plays) << LABEL_BITS - 8 | (shapes))
 
#endif
