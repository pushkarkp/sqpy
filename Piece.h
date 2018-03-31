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
extern int pathMaxLength;
void setPathMaxLength();

#endif
