/**
 * Copyright 2018 Pushkar Piggott
 *
 * Piece.h
 */
#ifndef PieceH
#define PieceH

#include "Presence.h"
#include "SquarePyramid.h"
#include "Position.h"
#include "Orientation.h"

///////////////////////////////////////////////////////////////////////////////
typedef const char* TPath;
typedef const char** TPiece;
extern TPath testPath;
extern const TPiece* pieces;
extern int pieceCount;
extern int maxPathLength;
extern int* pieceMaxInstances;

///////////////////////////////////////////////////////////////////////////////
char* getPiece(int, const char**);
char* readPieces(const char*);

int walk(EPresence, TPath, EOrientation, const TPosition*, TPlace*);
int* dupPieceInstanceCounts(int*);
int sumPieceInstanceCounts(int*);
#endif
