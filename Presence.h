/**
 * Copyright 2018 Pushkar Piggott
 *
 * Presence.h
 */
#ifndef PresenceH
#define PresenceH

///////////////////////////////////////////////////////////////////////////////
typedef enum { eMargin = -1, eAbsent, eFirstPiece } EPresence;
#define PIECE(c) ((c) - '`')
#define GLYPH(i) ((i) + '`')
#define PIECE_SIZE (8)
#define MAKE_PLAY(i, n) (PIECE(i) | (n) << PIECE_SIZE)
#define PLAY_PIECE(pl) GLYPH((pl) & (1 << PIECE_SIZE) - 1)
#define PLAY_N(pl) ((pl) >> PIECE_SIZE)

const char* presToString(int);
#endif
