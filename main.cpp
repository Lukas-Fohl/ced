#include <curses.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <stdarg.h>

#include "readFile.cpp"
#include "textBuffer.cpp"

typedef int32_t vec3[3];
typedef int32_t vec2[2];

char *fileBuffer;
fullTextBuffer fileBufferSort;

char *fileName;

void quit()
{
    endwin();

}

vec2 cursor = {0,0};
int32_t MAXY;
int32_t x, y;

bool running = true;

void printBuffer(){

    erase();

    for(int y = 0; y < fileBufferSort.size(); y++){
        for(int x = 0; x < fileBufferSort.at(y).size(); x++){
            mvprintw(y,x,"%c",fileBufferSort.at(y).at(x));
        }
    }

    color_set(1, 0);

    mvprintw(cursor[1],cursor[0]," ");

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

void runLoop(){

    printBuffer();

    while(running)
    {
        MAXY = fileBufferSort.size();

        refresh();

        //NAVIGATION

        auto value_ = getch();

        switch(value_){
            case KEY_UP:
                //up
                if(cursor[1]>=1){
                    cursor[1]-=1;    
                    if(fileBufferSort.at(cursor[1]).size() == 1){
                        cursor[0] = 0;
                    }
                    else if(fileBufferSort.at(cursor[1]).size()-2<cursor[0]){
                        cursor[0] = fileBufferSort.at(cursor[1]).size()-2;
                    }
                }
            break;
            case KEY_DOWN:
                //down
                if(cursor[1]<MAXY-1){
                    cursor[1]++;
                    if(fileBufferSort.at(cursor[1]).size() <= 1){
                        cursor[0] = 0;
                    }
                    else if(fileBufferSort.at(cursor[1]).size()-2<cursor[0]){
                        cursor[0] = fileBufferSort.at(cursor[1]).size()-2;
                    }
                }
            break;
            case KEY_RIGHT:
                //right
                if((cursor[0]==fileBufferSort.at(cursor[1]).size()-2||fileBufferSort.at(cursor[1]).size() == 1)&&cursor[1]+1<MAXY-1){
                    cursor[0] = 0;
                    cursor[1] = cursor[1]+1;
                    break;
                }
                if(!(fileBufferSort.at(cursor[1]).size() == 1)){
                    cursor[0]+=(cursor[0]<fileBufferSort.at(cursor[1]).size()-2)?1:0;
                }
            break;
            case KEY_LEFT:
                //left
                if((cursor[0]==0||fileBufferSort.at(cursor[1]).size() == 1)&&cursor[1]-1>=0){
                    cursor[0] = fileBufferSort.at(cursor[1]-1).size()-2;
                    cursor[1] = cursor[1]-1;
                    if(fileBufferSort.at(cursor[1]).size() == 1){
                        cursor[0] = 0;
                    }
                    break;
                }
                if(!(fileBufferSort.at(cursor[1]).size() == 1)){
                    cursor[0]-=(cursor[0]>=1)?1:0;
                }
            break;
            case KEY_ENTER:
                addLine(&fileBufferSort, cursor[1]);
            break;
            case KEY_HOME:  
                cursor[0] = 0;
            break;
            case KEY_END:
                if(fileBufferSort.at(cursor[1]).size()==1){
                    cursor[0] = 0;
                    break;
                }
                cursor[0] = fileBufferSort.at(cursor[1]).size()-2;
            break;
            case KEY_DC:
                delChar(&fileBufferSort.at(cursor[1]), cursor[0]);
            break;
            case KEY_BACKSPACE:
                if(cursor[0]-1>=0){
                    delChar(&fileBufferSort.at(cursor[1]), cursor[0]-1);
                    cursor[0]--;
                }
            break;
            case '\t':
                for(int i = 0; i < 4; i++){
                    addChar(&fileBufferSort.at(cursor[1]), '\t', cursor[0]+i);
                }
                cursor[0]+=4;
            break;
            case KEY_F(1):
                running =  false;
            break;
            case KEY_F(2):
                writeToFile(fileBufferSort, fileName);
            break;
            case KEY_F(3):
                writeToFile(fileBufferSort, fileName);
                running =  false;
            break;
            default:
                addChar(&fileBufferSort.at(cursor[1]), value_, cursor[0]);
            break;
        }

        //PRINT
        
        MAXY = fileBufferSort.size();

        printBuffer();
    }
}

//setup
int main(int argc, char *argv[])
{
    /**************/
    //SET UP NCURSES
    /**************/

    initscr();
    atexit(quit);
    keypad(stdscr, TRUE);
    start_color();
    curs_set(true);
    cbreak();
    noecho();
    getmaxyx(stdscr, y, x);

    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    init_pair(2, COLOR_WHITE, COLOR_BLACK);

    /********/
    //GET FILE
    /********/

    if((char*)argv[1] == "."){
        return 0;
    }

    fileName = (char*)argv[1];

    fileBuffer = readFromFile(fileName);
    CharsToFullTextBuffer(fileBuffer, &fileBufferSort);

    /***************/
    //RUN EDITOR LOOP
    /***************/

    runLoop();

    return 0;
}

//TODO:
//transform file to strct aarray of struct array:
//-> navigate                                           done
//change buffer:
//Display part of file                                      TODO
//scorle                                                    TODO
//del                                                   DONE
//add                                                   DONE
//save buffer                                               TODO
//File-Navigator                                            TODO
//save and exit text                                        TODO
//keys                                                  DONE
//enter file-name                                       DONE
//support for shortcuts                                     TODO
//mark                                                      TODO
//->cut, copy, past, del                                    TODO
//Line count                                                TODO
//->jump???                                                 TODO