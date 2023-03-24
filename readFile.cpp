#include <stdio.h>

#include "readFile.hpp"

char* readFromFile(char* fileName){
    FILE *fp;
    long lSize;
    char *buffer;

    fp = fopen (fileName, "rb");
    fseek(fp, 0L, SEEK_END);
    lSize = ftell(fp);
    rewind(fp);
    buffer = (char*)calloc(1, lSize+1);
    if(1!=fread(buffer, lSize, 1, fp)){
        fclose(fp),free(buffer),fputs("entire read fails",stderr),exit(1);
    }
    fclose(fp);
    return buffer;
    free(buffer);
}

void writeToFile(fullTextBuffer bufferIn, char* fileName){
    char *Buffer;
    for(int i = 0; i < bufferIn.size(); i++){
        for(int j = 0; j < bufferIn.at(i).size(); j++){
            Buffer += bufferIn.at(i).at(j);
        }
    }
    return;
}