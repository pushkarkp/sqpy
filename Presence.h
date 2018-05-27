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
const char* presToString(int);

#endif
