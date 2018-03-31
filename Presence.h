/**
 * Copyright 2018 Pushkar Piggott
 *
 * Presence.h
 */
#ifndef PresenceH
#define PresenceH

///////////////////////////////////////////////////////////////////////////////
typedef enum {
   eMargin = -1,
   eAbsent, 
   eGrey, ePinkPurple, eLightBlue, eDarkGreen, eYellow, eDarkBlue, 
   eRed, ePink, eBluePurple, eLightGreen, eOrange, eWhite,
   ePresences} EPresence;
const char* presToString(EPresence);
#define PIECE(c) (c - '`')
#define GLYPH(i) (i + '`')

///////////////////////////////////////////////////////////////////////////////
typedef int TSetOfPresences;
const char* sopToString(char* buf, TSetOfPresences);
#define SOP_BUF_SIZE (ePresences + 1)

#endif
