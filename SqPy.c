/**
 * Copyright 2018 Pushkar Piggott
 *
 * SqPy.c
 */

#include "Piece.h"
#include "Repeat.h"
#include "Solve.h"
#include "Display.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE 128

static const char const* ERR_OPEN = "failed to open file \"%s\"";
static const char const* ERR_FILE = "file error";
static const char const* ERROR_BAD_LINE = "%s on line %d \"%s\"";
static int showArgs = 0;
static int useOnce = 0;
static int pc = eAbsent;

int getOptions(const char**);

///////////////////////////////////////////////////////////////////////////////
void usage() {
   printf("usage:%s", EOL);
   printf("$ sqpy [<file>] [-a] [-u] [-h <height>] [-p <path> [<count>]] [-y | -l | -s [<orientation>] | -o [<piece>] [<path>]]%s", EOL);
   printf(" <file>                 arguments in a file and one piece per line%s", EOL);
   printf(" -a                     show arguments%s", EOL);
   printf(" -u                     stop when pieces all used, even if space remains%s", EOL);
   printf(" -h <height>            specify the pyramid height%s", EOL);
   printf(" -p <path> [<count>]    specify a piece%s", EOL);
   printf(" -i <piece>             specify the initial piece%s", EOL);
   printf(" -y                     show the pyramid%s", EOL);
   printf(" -l                     show the order of path start locations%s", EOL);
   printf(" -s [<orientation>]     show symmetries%s", EOL);
   printf(" -o [<path>]            show piece orientations%s", EOL);
   printf("                        show the solutions%s", EOL);
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
const char* getMandatory(const char* v, char c) {
   if (!v) {
      printf("missing value for option '-%c'%s", c, EOL);
      usage();
   }
   return v;
}

///////////////////////////////////////////////////////////////////////////////
const char* getNumber(const char* v, char c) {
   if (v && !isdigit(v[0])) {
      printf("value for option '-%c' is not a number%s", c, EOL);
      usage();
   }
   return v;
}

///////////////////////////////////////////////////////////////////////////////
char* cutComment(char* line) {
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
int toArgv(const char*** argv, char* line) {
   int argc = 0;
   int i;
   for (i = 0; line[i]; ++i) {
      if (isspace(line[i])) {
         line[i] = 0;
      } else if (i == 0 || line[i - 1] == 0) {
         ++argc;
      }         
   }
   if (argc == 0) {
      *argv = 0;
   } else {
      *argv = (const char**)malloc((argc + 1) * sizeof(const char*));
      int arg = 0;
      if (argc) {
         for (i = 0; ; ++i) {
            if (line[i] && (i == 0 || line[i - 1] == 0)) {
               (*argv)[arg++] = &line[i];
               if (arg == argc) {
                  break;
               }
            }
         }
      }
      (*argv)[argc] = 0;
   }
   return argc;
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
      char* line = strdup(lineBuf);
      cutComment(line);
      const char** argv;
      int argc = toArgv(&argv, line);
      if (!argc) { 
         continue;
      }
      if (argv[0][0] == '-') {
         int ok = getOptions(argv);
         free(argv);
         free(line);
         if (ok) {
            continue;
         }
         return 0;
      }
      char* err = getPiece(argc, argv);
      free(argv);
      free(line);
      if (err) {
         char* buf = (char*)malloc((strlen(ERROR_BAD_LINE) + strlen(err) + 4 + strlen(lineBuf)) * sizeof(char));
         sprintf(buf, ERROR_BAD_LINE, err, ln, lineBuf);
         free(err);
         return buf;
      }
   }
   if (!feof(f)) {
      return strdup(ERR_FILE);
   }

   fclose(f);
   return 0;
}

///////////////////////////////////////////////////////////////////////////////
void testPyramid() {
   TPlace* sps = SP_NEW(2);
   spInit(&sps[0]);
   displayWide(ePyramid, 65, &sps[0]);
   spEnumerate(&sps[1]);
   displayWide(ePyramid, 65, &sps[1]);
   displayWide(ePyramid, 65, &sps[1]);
   displayWide(ePyramid, 65, &sps[1]);
   displayWide(ePyramid, 65, &sps[1]);
   displayWide(ePyramid, 65, &sps[1]);
   displayWide(ePyramid, 65, &sps[1]);
   displayWide(ePyramid, 65, &sps[1]);
   displayWide(ePyramid, 65, &sps[1]);
   displayWide(ePyramid, 65, &sps[1]);
   displayWide(ePyramid, 65, &sps[1]);
   displayWide(ePyramid, 65, 0);
   free(sps);
}

///////////////////////////////////////////////////////////////////////////////
int getOptions(const char** argv) {
   int i;
   for (i = 0; argv[i] != NULL && argv[i][0] == '-'; ++i) {
      switch (argv[i][1]) {
      case 'a': {
         showArgs = 1;
         break;
      }
      case 'u': {
         useOnce = 1;
         break;
      }
      case 'h': {
         const char* n = getNumber(getMandatory(getOptionValue(&i, argv), argv[i][1]), argv[i][1]);
         spSetHeight(strtol(n, 0, 10));
         initDisplay();
         break;
      }
      case 'p': {
         int len = getNextOptionOffset(argv + i) - 1;
         char* err = getPiece(len, argv + i + 1);
         if (err) {
            printf("%s%s", err, EOL);
            free(err);
            return 0;
         }
         i += len;
         break;
      }
      case 'i': {
         const char* n = getMandatory(getOptionValue(&i, argv), argv[i][1]);
         pc = strtol(n, 0, 10);
         if (pc == 0 && n[0] != '0') {
            pc = PIECE(n[0]);
         }
         break;
      }
      case 'y': {
         testPyramid();
         exit(0);
      }
      case 'l': {
         testSpFind();
         exit(0);
      }
      case 's': {
         EOrientation or = e001XPlusPlus;
         const char* n = getNumber(getOptionValue(&i, argv), argv[i][1]);
         if (n) {
            or = strtol(n, 0, 10);
         }
         testPathSymmetry(or);
         exit(0);
      }
      case 'o': {
         if (pc == eAbsent) {
            pc = eFirstPiece;
         }
         const char* path = getOptionValue(&i, argv);
         if (!path) {
            path = "0";
         }
         if (isdigit(path[0])) {
            int p = strtol(path, 0, 10);
            int i;
            for (i = 0; pieces[pc][i]; ++i) {}
            if (p >= i) {
               printf("path index (%d) out of range [0..%d] for %c %s", p, i - 1, GLYPH(pc), EOL);
               return 0;
            }
            path = pieces[pc][p];
         }
         testPieceOrientations(pc, path);
         exit(0);
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
int main(int argc, const char** argv) {
   spSetHeight(5);
   initDisplay();
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
   if (!getOptions(&argv[i])) {
      return 0;
   }
   if (showArgs) {
      printf("spHeight %d pieceCount %d pc %d useOnce %d%s", spHeight, pieceCount, pc, useOnce, EOL);
      for (i = eFirstPiece; i < pieceCount; ++i) {
         int p;
         for (p = 0; pieces[i][p]; ++p) {
            printf("%s ", pieces[i][p]);
         }
         printf("%s", EOL);
      }
   }
   findRepeat();
   printf("solutions %d%s", solve(pc, useOnce), EOL);
   return 0;
}