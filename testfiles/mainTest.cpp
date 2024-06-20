#include <curses.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <string.h>
 
       int;for;#include
 
#include "readFile.cpp"
#include "textBuffer.cpp"
#include "syntax.cpp"

typedef int32_t vec3[3];
typedef int32_t vec2[2];

char *fileBuffer;
fullTextBuffer fileBufferSort;

char *fileName;
 
void quit()
{
    endwin();
}
 
vec2 cursor = {0,0} 

int32_t MAXY;
int32_t x, y;

bool running = true;

vec2 displayOffset = {0,1};

void printBuffer(){

    erase();

    std::vector<std::vector<char>> Display;
    std::vector<char> displayRow;
    for(int i = 0+displayOffset[1]; i < y+displayOffset[1]; i++){
        for(int j = 0+displayOffset[0]; j < x+displayOffset[0]; j++){
            if(i < MAXY-1 && j < fileBufferSort.at(i).size()){
                displayRow.push_back(fileBufferSort.at(i).at(j));
            }
        }
            Display.push_back(displayRow);
        displayRow.clear();
    }

    //--> diplay vector 
    //change vector offset
    //check for size

    for(int y_ = 0; y_ < Display.size(); y_++){
        for(int x_ = 0; x_ < Display.at(y_).size(); x_++){
            mvprintw(y_,x_,"%c",Display.at(y_).at(x_));
        }
    }

    //for(int y_ = 0; y_ < fileBufferSort.size(); y_++){
    //    for(int x_ = 0; x_ < fileBufferSort.at(y_).size(); x_++){
    //        mvprintw(y_,x_,"%c",fileBufferSort.at(y_).at(x_));
    //    }
    //}

    color_set(1, 0);

    if(fileBufferSort.at(cursor[1]).at(cursor[0])=='\n'){
        mvprintw(cursor[1]-displayOffset[1],cursor[0]-displayOffset[0]," ");
    }else{
        mvprintw(cursor[1]-displayOffset[1],cursor[0]-displayOffset[0],"%c",fileBufferSort.at(cursor[1]).at(cursor[0]));
    }

    color_set(2, 0);
    mvprintw(y-1,0,"F1");
    color_set(1, 0);
    mvprintw(y-1,2,"TO EXIT");
    color_set(2, 0);

    mvprintw(y-1,9,"F2");
    color_set(1, 0);
    mvprintw(y-1,11,"TO SAVE");
    color_set(2, 0);

    mvprintw(y-1,18,"F3");
    color_set(1, 0);
    mvprintw(y-1,20,"TO SAVE AND EXIT");
    color_set(2, 0);
    
    color_set(1, 0);
    mvprintw(y-1,x-7,"       ");
    mvprintw(y-1,x-7,"%d",cursor[0]);
    mvprintw(y-1,x-4,";");
    mvprintw(y-1,x-3,"%d",cursor[1]);
    color_set(2, 0);

    refresh();
}
