/**
 * Copyright 2018 Pushkar Piggott
 *
 * SqPy.c
 */

#include "Piece.h"
#include "Repeat.h"
#include "Solve.h"
#include "Display.h"
#include "Topics.h"
#include "SetOf.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE 128

///////////////////////////////////////////////////////////////////////////////
static const char const* ERR_OPEN = "failed to open file \"%s\"";
static const char const* ERR_FILE = "file error";
static const char const* ERR_NO_ORIENT = "no orientation matched \"%s\"";
static const char const* ERR_NO_TOPIC = "no display topic matched \"%s\"";
static const char const* ERR_BAD_TOPIC = "more than one display topic matched \"%s\"";
static int useOnce = 0;
static int pc = eAbsent;
static int pathIndex = -1;
static TPath path = ".a.A";
static TSet soor = 0;
static int pageWidth = 70;
static TSet topics = 0;

int getOptions(const char**, const char*);

///////////////////////////////////////////////////////////////////////////////
void usage() {
   printf("usage:%s", EOL);
   printf("sqpy [<file>] [-u] [-h <height>] [-p <paths> [<count>]] [-i <piece>] [-a <path index>] [-g <page width>] [-d <topics>]%s", EOL);
   printf(" <file>                    file contains arguments and one piece per line%s", EOL);
   printf(" -u                        stop when pieces all used, even if space remains%s", EOL);
   printf(" -h <height>               the pyramid height%s", EOL);
   printf(" -p <paths> [<count>]      a piece%s", EOL);
   printf(" -i <piece> (all)          the initial piece by letter or index%s", EOL);
   printf(" -a <path or index> (.a.A) the path to orient%s", EOL);
   printf(" -0 <orientation index>    the orientation to show the paths%s", EOL);
   printf(" -o <orientations match>   the orientations to show the paths%s", EOL);
   printf(" -g <page width> (70)      the width of the display page%s", EOL);
   printf(" -d <topics>               display one or more topics (see below)%s", EOL);
   printf("%sDisplay topics (any unique prefix matches):%s", EOL, EOL);
   char* str = setToString(ALL_TOPICS, displayTopicToString);
   printf("%s%s", str, EOL);
   free(str);
   exit(0);
}

///////////////////////////////////////////////////////////////////////////////
int getNextOptionOffset(const char** argv) {
   int i;
   for (i = 1; argv[i] && argv[i][0] != '-'; ++i) {}
   return i;
}

///////////////////////////////////////////////////////////////////////////////
const char* getOptionExtraValue(int* pi, const char** argv) {
   if (argv[*pi + 1] && argv[*pi + 1][0] != '-') {
      ++*pi;
      return argv[*pi];
   }
   return 0;
}

///////////////////////////////////////////////////////////////////////////////
const char* getOptionValue(int* pi, const char** argv) {
   if (argv[*pi][2]) {
      return &argv[*pi][2];
   }
   return getOptionExtraValue(pi, argv);
}

///////////////////////////////////////////////////////////////////////////////
const char* getMandatory(const char* v, char c, const char* prefix) {
   if (!v) {
      printf("%smissing value for option '-%c'%s", prefix?prefix:"", c, EOL);
      usage();
   }
   return v;
}

///////////////////////////////////////////////////////////////////////////////
const char* getNumber(const char* v, char c, const char* prefix) {
   if (v && !isdigit(v[0])) {
      printf("%svalue for option '-%c' is not a number%s", prefix?prefix:"", c, EOL);
      usage();
   }
   return v;
}

///////////////////////////////////////////////////////////////////////////////
char* cutComment(char* line) {
   for (; *line && isspace(*line); ++line) {}
   int i;
   for (i = 0; line[i]; ++i) {
      if (line[i] == '/' && line[i + 1] == '/') {
         line[i] = 0;
         break;
      }
   }
   return line;
}
         
///////////////////////////////////////////////////////////////////////////////
int toArgv(const char** argv, char* line, const char* opt) {
   int argc = 0;
   if (opt) {
      argv[argc++] = opt;
   }
   int i;
   for (i = 0; line[i]; ++i) {
      if (isspace(line[i])) {
         line[i] = 0;
      } else if (i == 0 || line[i - 1] == 0) {
         argv[argc++] = &line[i];
      }         
   }
   argv[argc] = 0;
   return argc;
}

///////////////////////////////////////////////////////////////////////////////
int readPathArg(const char* path, const char* n, const char* prefix) {
   if (!pathOk(path, prefix)) {
      return 0;
   }
   TPiece pc = pcCreate(path);
   if (!pc) {
      printf("%sfailed to create paths from '%s'%s", prefix?prefix:"", path, EOL);
      return 0;
   }
   int times = (n) ? strtol(n, 0, 10) : 1;
   pcAdd(pc, times);
   return 1;
}

///////////////////////////////////////////////////////////////////////////////
char* read(const char* fname) {
   FILE *f;
   if (!(f = fopen(fname, "r"))) {
      char* buf = (char*)malloc((strlen(ERR_OPEN) + strlen(fname)) * sizeof(char));
      sprintf(buf, ERR_OPEN, fname);
      return buf;
   }
   char lineBuf[MAX_LINE];
   int ln;
   for (ln = 1; fgets(lineBuf, MAX_LINE, f); ++ln) {
      char prefix[16];
      sprintf(prefix, "on line %d: ", ln);
      char* line = lineBuf;
      line = cutComment(line);
      const char* opt = (line[0] && line[0] == '-') ? 0 : "-p";
      const char* argv[16] = {0};
      int argc = toArgv(argv, line, opt);
      if ((argc > 1 || (argc && !opt)) 
       && !getOptions(argv, prefix)) { 
         break;
      }
   }
   if (!feof(f)) {
      return strdup(ERR_FILE);
   }

   fclose(f);
   return 0;
}

///////////////////////////////////////////////////////////////////////////////
int getOptions(const char** argv, const char* prefix) {
   int i;
   for (i = 0; argv[i] != NULL && argv[i][0] == '-'; ++i) {
      switch (argv[i][1]) {
      case 'u': {
         useOnce = 1;
         break;
      }
      case 's': {
         showOrientationsSymmetry(0);
         return 0;
      }
      case 'h': {
         const char* n = getNumber(getMandatory(getOptionValue(&i, argv), argv[i][1], prefix), argv[i][1], prefix);
         spSetHeight(strtol(n, 0, 10));
         initDisplay(pageWidth);
         break;
      }
      case 'p': {
         const char* path = getMandatory(getOptionValue(&i, argv), argv[i][1], prefix);
         const char* n = getNumber(getOptionExtraValue(&i, argv), argv[i - 1][1], prefix);
         if (!readPathArg(path, n, prefix)) {
            return 0;
         }
         break;
      }
      case 'i': {
         const char* n = getMandatory(getOptionValue(&i, argv), argv[i][1], prefix);
         pc = strtol(n, 0, 10);
         if (pc == 0 && n[0] != '0') {
            pc = PIECE(n[0]);
         }
         break;
      }
      case 'a': {
         const char* n = getMandatory(getOptionValue(&i, argv), argv[i][1], prefix);
         if (strtol(n, 0, 10) != 0 || n[0] == '0') {
            pathIndex = strtol(n, 0, 10);
         } else {
            path = n;
         }
         break;
      }
      case '0': {
         const char* n = getNumber(getMandatory(getOptionValue(&i, argv), argv[i][1], prefix), argv[i][1], prefix);
         soor = SET_WITH(soor, strtol(n, 0, 10));
         break;
      }
      case 'o': {
         const char* v = getMandatory(getOptionValue(&i, argv), argv[i][1], prefix);
         TSetOfOrientations s = matchOrientation(v);
         if (setCount(s) == 0) {
            printf(ERR_NO_ORIENT, v);
            printf("%s", EOL);
            usage();
         }
         soor |= s;
         break;
      }
      case 'g': {
         const char* n = getNumber(getMandatory(getOptionValue(&i, argv), argv[i][1], prefix), argv[i][1], prefix);
         pageWidth = strtol(n, 0, 10);
         initDisplay(pageWidth);
         break;
      }
      case 'd': {
         const char* v;
         while ((v = getOptionExtraValue(&i, argv)) != 0) {
            TSet sot = matchDisplayTopics(v);
            if (setCount(sot) != 1) {
               printf(setCount(sot) == 0 ? ERR_NO_TOPIC : ERR_BAD_TOPIC, v);
               char* str = setToString(ALL_TOPICS, displayTopicToString);
               printf("%sTopics available: %s%s", EOL, str, EOL);
               free(str);
               return 0;
            }
            topics |= sot;
         }
         break;
      }
      default:
         printf("unrecognized option '-%c'%s", argv[i][1], EOL);
         usage();
         break;
      }
   }
   return 1;
}

///////////////////////////////////////////////////////////////////////////////
void showOptions() {
   printf("spHeight %d useOnce %d pieces %d%s", spHeight, useOnce, pieceCount - 1, EOL);
   if (pieceCount > 1) {
      printf("Pieces:%s", EOL);
   }
   int i;
   for (i = eFirstPiece; i < pieceCount; ++i) {
       printf(" %c:", GLYPH(i));
      int p;
      for (p = 0; pieces[i][p]; ++p) {
         printf(" %s", pieces[i][p]);
      }
      printf("%s", EOL);
   }
   if (pc != 0) {
      printf(" initial piece %d (%c)%s", pc, GLYPH(pc), EOL);
   }
   if (pathIndex != -1) {
      printf(" path %d (%s)%s", pathIndex, pieces[pc][pathIndex], EOL);
   }
   if (soor != 0) {
      char* str = setToString(soor, orToString);
      printf("orientations: %s%s", str, EOL);
      free(str);
   }
   printf("page width %d%s", pageWidth, EOL);
   if (topics) {
      char* str = setToString(topics, displayTopicToString);
      printf("Topics: %s%s", str, EOL);
      free(str);
   }
}

///////////////////////////////////////////////////////////////////////////////
int main(int argc, const char** argv) {
   int i = 1;
   if (argv[i] && argv[i][0] != '-') {
      char* err = read(argv[i]);
      ++i;
      if (err) {
         printf("%s%s", err, EOL);
         free(err);
         return 0;
      }
   }
   if (!getOptions(&argv[i], 0)) {
      return 0;
   }
   if (pieceCount == eFirstPiece) {
      const char* argvp[2] = {path, 0};
      pcRead(1, argvp);
   }
   if (spHeight == 0) {
      pcSetHeight();
      initDisplay(pageWidth);
   }
   if (SET_HAS(topics, eDisplaySettings)) {
      showOptions();
   }
   if (SET_HAS(topics, eDisplayPaths)) {
      pcDisplayAll();
      exit(0);
   }
   if (SET_HAS(topics, eDisplayPyramid)) {
      spTestPyramid();
      exit(0);
   }
   if (SET_HAS(topics, eDisplayOrder)) {
      spTestOrder();
      exit(0);
   }
   if (SET_HAS(topics, eDisplayOrientations)) {
      pcTestOrientations(pc, pathIndex, soor);
      exit(0);
   }
   if (SET_HAS(topics, eDisplayRepeat)) {
      findRepeat(1);
      exit(0);
   }
   findRepeat(0);
   printf("Total solutions %d%s", solve(pc, useOnce, topics), EOL);
   return 0;
}