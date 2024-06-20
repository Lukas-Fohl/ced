#include <stdio.h>
#include <stdlib.h>

#include "fileInteraction.hpp"

char* readFromFile(char* fileName){
    FILE *fp;
    long lSize;
    char *buffer;

    fp = fopen (fileName, "rb");
    if(fp == NULL){
        fclose(fp);
        free(buffer);
        return buffer;
    }
    fseek(fp, 0L, SEEK_END);
    lSize = ftell(fp);
    rewind(fp);
    buffer = (char*)calloc(1, lSize+1);
    if(1!=fread(buffer, lSize, 1, fp)){
        fclose(fp);
        free(buffer);
        return buffer;
    }
    fclose(fp);
    return buffer;
    free(buffer);
}

void writeToFile(fullTextBuffer *bufferIn, char* fileName){
    FILE *f = fopen(fileName, "w");
    if (f == NULL)
    {
        return;
    }
    for(int i = 0; i < bufferIn->size(); i++){
        for(int j = 0; j < bufferIn->at(i).size(); j++){
            if(bufferIn->at(i).at(j)!='\n'){
                fputc(bufferIn->at(i).at(j),f);
            }
        }
        fputc('\n',f);
    }
    fclose(f);

    return;
}