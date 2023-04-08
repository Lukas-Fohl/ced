#include <curses.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <stdarg.h>

        int;for;char;

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

vec2 cursor = {0,0};

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

void runLoop(){

    printBuffer();

    while(running)
    {
        refresh();

        //NAVIGATION

        auto value_ = getch();

        switch(value_){
            case KEY_UP:
                //up
                if(cursor[1]-displayOffset[1]==0&&cursor[1]!=0){
                    displayOffset[1]--;
                }
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
                if(cursor[1]-displayOffset[1]==y-2&&cursor[1]<MAXY-1){
                    displayOffset[1]++;
                }
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
                if(fileBufferSort.at(cursor[1]).size()!=0){//because '\n' is alway there
                    if((cursor[0]==fileBufferSort.at(cursor[1]).size()-2||(fileBufferSort.at(cursor[1]).size() == 1)&&cursor[1]+1<MAXY-1)){
                        cursor[0] = 0;
                        cursor[1] = cursor[1]+1;
                        if(cursor[1]-displayOffset[1]==y-1&&cursor[1]<MAXY-1){
                           displayOffset[1]++;
                        }
                        break;
                    }
                    if(!(fileBufferSort.at(cursor[1]).size() <= 1)){
                        cursor[0]+=(cursor[0]<fileBufferSort.at(cursor[1]).size()-2)?1:0;
                    }
                }
            break;
            case KEY_LEFT:
                //left
                if(cursor[0]<=0&&cursor[1]!=0){
                    displayOffset[1]--;
                }
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
            case '\n':
                addLine(&fileBufferSort, cursor[1]);
                cursor[1]++;
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
                if(fileBufferSort.at(cursor[1]).size() <= 1 && cursor[1]<MAXY-1){
                    delLine(&fileBufferSort, cursor[1]);
                    break;
                }
                if(cursor[0] == fileBufferSort.at(cursor[1]).size()-2 && cursor[1]<MAXY-1){
                    if(fileBufferSort.at(cursor[1]+1).size()==1){
                        delLine(&fileBufferSort,cursor[1]+1);       
                    }
                    for(int i = 0; i < fileBufferSort.at(cursor[1]+1).size()-2; i++){
                        addChar(&fileBufferSort.at(cursor[1]),fileBufferSort.at(cursor[1]+1).at(i),fileBufferSort.at(cursor[1]).size()-1);
                    }
                    delLine(&fileBufferSort,cursor[1]+1);   
                    break;
                }
                delChar(&fileBufferSort.at(cursor[1]), cursor[0]);
            break;
            case KEY_BACKSPACE:
                if(cursor[0]-1>=0){
                    delChar(&fileBufferSort.at(cursor[1]), cursor[0]-1);
                    cursor[0]--;
                }else{
                    if(fileBufferSort.at(cursor[1]).size()==1){
                        delLine(&fileBufferSort,cursor[1]);   
                        //cursor[1]--;
                        cursor[0] = fileBufferSort.at(cursor[1]).size()-2;
                        break;
                    }
                    else if(fileBufferSort.at(cursor[1]).size()>1){
                        if(fileBufferSort.at(cursor[1]-1).size()>1){
                            for(int i = 0; i < fileBufferSort.at(cursor[1]).size()-1; i++){
                                addChar(&fileBufferSort.at(cursor[1]-1),fileBufferSort.at(cursor[1]).at(i),fileBufferSort.at(cursor[1]-1).size()-1);
                            }
                        }else{
                            for(int i = 0; i < fileBufferSort.at(cursor[1]).size()-1; i++){
                                addChar(&fileBufferSort.at(cursor[1]-1),fileBufferSort.at(cursor[1]).at(i),fileBufferSort.at(cursor[1]-1).size()-1);
                            }
                        }
                    }
                    delLine(&fileBufferSort,cursor[1]);   
                    //cursor[1]--;
                    break;
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
                writeToFile(&fileBufferSort, fileName);
            break;
            case KEY_F(3):
                writeToFile(&fileBufferSort, fileName);
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
    curs_set(false);
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

    if(fileBufferSort.size()==0){
        addLine(&fileBufferSort,0);
    }

    MAXY = fileBufferSort.size();

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
//Display part of file                                  DONE
//scorle                                                DONE
//save buffer                                           DONE
//del                                                   DONE
//add                                                   DONE
//save and exit text                                    DONE
//keys                                                  DONE
//enter file-name                                       DONE
//Line interaction                                      DONE
//support for shortcuts                                     TODO
//mark                                                      TODO
//->cut, copy, past, del                                    TODO
//Line count                                                TODO
//->jump???                                                 TODO
