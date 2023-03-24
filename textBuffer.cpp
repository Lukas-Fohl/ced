#include <string.h>
#include <stdio.h>
#include <vector>
#include <iostream>

#include "textBuffer.hpp"

int CharsToFullTextBuffer(char *CharBufferIn, fullTextBuffer *BufferIn){
    textLine temp;
    int x = 0;
    for(int i = 0; CharBufferIn[i]!='\0'; i++){
        temp.push_back(CharBufferIn[i]);
        if(CharBufferIn[i] == '\n'){
            if(BufferIn == nullptr){
                return 0;
            }
            BufferIn->push_back((textLine)temp);
            temp.clear();
        }
        x = i;
    }
    return x;
}

void addLine(fullTextBuffer *FullBufferIn, int index){
    textLine TLTemp = {' ','\n'};
    fullTextBuffer::iterator it = (fullTextBuffer::iterator)FullBufferIn->begin();
    std::advance(it, index);
    FullBufferIn->insert(it, TLTemp);

    /*
    fullTextBuffer temp = *FullBufferIn;
    textLine TLtemp;
    temp.push_back(FullBufferIn->at(index));
    for(int i = temp.size(); i > index; i-=1){
        temp.at(i) = temp.at(i-1);
    }
    temp.at(index) = FullBufferIn->at(index);//TLtemp
    FullBufferIn = (fullTextBuffer*)&temp;
    */
}

void addChar(textLine *lineIn, char charIn, int index){
    auto it = lineIn->begin();
    std::advance(it, index);
    lineIn->insert(it, charIn);
}

void delChar(textLine *lineIn, int index){
    auto it = lineIn->begin();
    std::advance(it, index);
    lineIn->erase(it);
}

void delLine(fullTextBuffer *FullBufferIn, int index){
    fullTextBuffer::iterator it = FullBufferIn->begin();
    std::advance(it, index);
    FullBufferIn->erase(it);
}