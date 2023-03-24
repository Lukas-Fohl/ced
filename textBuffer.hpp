#ifndef TEXT_BUFFER
#define TEXT_BUFFER 1

#include <vector>

//typedef struct textLine{std::vector<char> content;}textLine;
//typedef struct fullTextbuffer{std::vector<textLine> content;}fullTextBuffer;

typedef std::vector<char> textLine;
typedef std::vector<textLine> fullTextBuffer;

int CharsToFullTextBuffer(char *CharBufferIn, fullTextBuffer *BufferIn);
void addLine(fullTextBuffer *FullBufferIn, int index);
void addChar(textLine *lineIn, char charIn ,int index);
void delChar(textLine *lineIn, int index);
void delLine(fullTextBuffer *FullBufferIn, int index);

#endif