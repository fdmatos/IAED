#include "node.h"
#define MAXCHARS 1025


char* getline();
Link insertLines (char **text, int nlines, Link root);
void erase (Link root, char** text, int nlines);
Link executeCommands (Link root, char** text, int nlines);
void S_command (char ** text, int nlines);
void L_command (Link root, char** text, int nlines);
Link F_command (Link root);
void W_command (Link root, char **text, int nlines);
void H_command (Link root);
int testLine (int *printedLines, int nPrLines, int line);
Word convertLower (Word word);