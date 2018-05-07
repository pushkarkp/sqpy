/**
 * Copyright 2018 Pushkar Piggott
 *
 * Presence.h
 */
#ifndef PresenceH
#define PresenceH

///////////////////////////////////////////////////////////////////////////////
typedef enum { eMargin = -1, eAbsent, eFirstPiece } EPresence;
#define PIECE(c) (c - '`')
#define GLYPH(i) (i + '`')

///////////////////////////////////////////////////////////////////////////////
typedef int TSetOfPresences;
const char* sopToString(char* buf, TSetOfPresences);
#define SOP_BUF_SIZE (pieceCount + 1)

#endif
