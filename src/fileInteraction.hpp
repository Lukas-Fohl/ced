#ifndef FILE_INTERACTION_H
#define FILE_INTERACTION_H

#include "textBuffer.hpp"

char* readFromFile(char* fileName);
void writeToFile(fullTextBuffer *bufferIn, char* fileName);

#endif