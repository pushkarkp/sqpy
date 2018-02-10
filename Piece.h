/**
 * Copyright 2018 Pushkar Piggott
 *
 * Piece.h
 */
#ifndef PieceH
#define PieceH

#include "Presence.h"

///////////////////////////////////////////////////////////////////////////////
#define MAX_PATHS 5
typedef const char* TPiece[MAX_PATHS + 1];
extern const TPiece pieces[ePresences];

///////////////////////////////////////////////////////////////////////////////
typedef struct {
   char piece[ePresences];
} TSetOfPieces;
const char* setOfPiecesToString(char* buf, TSetOfPieces*);

#endif
