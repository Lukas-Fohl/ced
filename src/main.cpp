#include <curses.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <stdarg.h>

#include "fileInteraction.cpp"
#include "textBuffer.cpp"
#include "syntax.cpp"
#include "navigation.cpp"

typedef int32_t vec3[3];
typedef int32_t vec2[2];

char *fileBuffer;
fullTextBuffer fileBufferSort;

char *fileName;
 
void quit(){
    endwin();
}

vec2 cursor = {0,0};

int32_t MAXY;
int32_t x, y;

bool running = true;

vec2 displayOffset = {0,0};
int screenOffset = 4;

void printBuffer(){

    erase();

    fullTextBuffer display;
    textLine displayRow;
    for(int i = displayOffset[1]; i < y-1+displayOffset[1]; i++){//out of range
        for(int j = displayOffset[0]; j < x+displayOffset[0]; j++){
            if(i < MAXY && j < fileBufferSort.at(i).size()){
                displayRow.push_back(fileBufferSort.at(i).at(j));
            }
        }
        display.push_back(displayRow);
        displayRow.clear();
    }

    init_color(COLOR_RED, 98, 114, 164);

    init_pair(1, COLOR_BLACK, COLOR_WHITE); //cursor
    init_pair(2, COLOR_WHITE, COLOR_RED); //normal text


    init_pair(5,COLOR_CYAN, COLOR_RED);
    init_pair(6,COLOR_GREEN, COLOR_RED);
    init_pair(7,COLOR_MAGENTA, COLOR_RED);
    init_pair(8,COLOR_YELLOW, COLOR_RED);
    init_pair(9,COLOR_BLUE, COLOR_RED);

    screenOffset = std::to_string(displayOffset[1] + y).size()+1;

    for(int x_ = 0; x_ < x; x_++){
        for(int y_ = 0; y_ < y; y_++){
            if(x_ != screenOffset-1 ){
                attron(COLOR_PAIR(2));
                mvaddch(y_,x_,' ');
            }
        }
    }

    for(int y_ = 0; y_ < display.size(); y_++){
        for(int x_ = 0; x_ < display.at(y_).size(); x_++){
            if(x_< display.at(y_).size()-1){
                if(display.at(y_).at(x_)=='\n'){
                    attron(COLOR_PAIR(2));
                    mvaddch(y_,x_+screenOffset,' ');
                    continue;
                }
                attron(COLOR_PAIR(setSyntaxColor(&display.at(y_),x_)));;
                mvaddch(y_,x_+screenOffset,display.at(y_).at(x_));
            }else{
                attron(COLOR_PAIR(2));
                mvaddch(y_,x_+screenOffset,' ');
            }
            attron(COLOR_PAIR(2));
            for(int i = 0; i < screenOffset-1; i++){
                attron(COLOR_PAIR(2));
                mvaddch(y_,i,' ');
            }
            mvprintw(y_,0,std::to_string((y_+displayOffset[1])).c_str());
            //mvprintw(y_,x_,"%c",display.at(y_).at(x_));
        }
    }

    attron(COLOR_PAIR(1));

    //cursor
    if(fileBufferSort.at(cursor[1]).at(cursor[0])=='\n'){
        mvprintw(cursor[1]-displayOffset[1],cursor[0]-displayOffset[0]+screenOffset," ");
    }else{
        mvprintw(cursor[1]-displayOffset[1],cursor[0]-displayOffset[0]+screenOffset,"%c",fileBufferSort.at(cursor[1]).at(cursor[0]));
    }

    attron(COLOR_PAIR(2));
    mvprintw(y-1,0,"F1");
    attron(COLOR_PAIR(1));
    mvprintw(y-1,2,"TO EXIT");
    attron(COLOR_PAIR(2));

    mvprintw(y-1,10,"F3");
    attron(COLOR_PAIR(1));
    mvprintw(y-1,12,"TO SAVE");
    attron(COLOR_PAIR(2));

    mvprintw(y-1,20,"F5");
    attron(COLOR_PAIR(1));
    mvprintw(y-1,22,"TO SAVE AND EXIT");
    attron(COLOR_PAIR(2));
    
    attron(COLOR_PAIR(1));
    mvprintw(y-1,x-7,"       ");
    mvprintw(y-1,x-7,"%d",cursor[0]);
    mvprintw(y-1,x-4,";");
    mvprintw(y-1,x-3,"%d",cursor[1]);
    attron(COLOR_PAIR(2));

    refresh();
}

void runLoop(){

    printBuffer();

    while(running){
        refresh();

        //NAVIGATION

        auto value_ = getch();

        /*
        auto value_,
        vec2 cursor,
        vec2 displayOffset,
        int32_t MAXY,
        char *fileName,
        bool running

        change cursor
        change displayOffset

        access pointer to fileBufferSort
        */
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
                if(cursor[1]-displayOffset[1]==y-2 && displayOffset[1]+y<MAXY){
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
                if(cursor[0]<=0&&displayOffset[1]!=0){
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
                if(fileBufferSort.at(cursor[1]).size()-2 == cursor[0]){
                    addLine(&fileBufferSort, cursor[1]+1);
                    cursor[0] = 0;
                    cursor[1]++;
                }else if(cursor[0] <= 0){
                    addLine(&fileBufferSort, cursor[1]);
                    cursor[1]++;
                }else{
                    addLine(&fileBufferSort, cursor[1]+1);
                    for(int i = cursor[0]; i < fileBufferSort.at(cursor[1]).size(); i++){
                        addChar(&fileBufferSort.at(cursor[1]+1),fileBufferSort.at(cursor[1]).at(i),i-cursor[0]);
                    }
                    int temp = fileBufferSort.at(cursor[1]).size();
                    for(int i = cursor[0]; i < temp; i++){
                        delChar(&fileBufferSort.at(cursor[1]),cursor[0]);
                    }
                }

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
                    delChar(&fileBufferSort.at(cursor[1]), cursor[0]);
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
            case KEY_F(3):
                writeToFile(&fileBufferSort, fileName);
            break;
            case KEY_F(5):
                writeToFile(&fileBufferSort, fileName);
                running =  false;
            break;
            default:
                cursor[0]++;
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

    //addLine(&fileBufferSort,fileBufferSort.size()-1);

    runLoop();

    return 0;
}

//TODO:
/*

prio:
    rewrite navigation
        put into other file
        simplify return

transform file to strct aarray of struct array:
navigate                                              DONE
change buffer:
display part of file                                  DONE
scorle                                                DONE
save buffer                                           DONE
del                                                     TODO
add                                                     TODO
save and exit text                                    DONE
keys                                                  DONE
enter file-name                                       DONE
Line interaction                                        TODO
support for shortcuts                                   TODO
mark                                                    TODO
cut, copy, past, del                                    TODO
Line count                                              TODO
jump???                                                 TODO
syntax                                                DONE
horizontal                                            DONE
file Navigation (fuzzy)                                 TODO
del last char                                         DONE
new line in middel of line                            DONE

PROBLEMS:
  LAST 2 LINES MISSING FOR SOME REASON
  SYNTAX NOT WORKING AFTER 8 CHARS
  NOT ABLE TO SCROLE TO THE END SOME HOW
*/