#include <vector>
#include <string>
#include <algorithm>
#include <iterator>

#include "syntax.hpp"
#include "textBuffer.hpp"

bool containsString(std::vector<std::string> vectorIn, std::string word_){
    return (std::find(vectorIn.begin(),vectorIn.end(),word_) != vectorIn.end());
}

template <typename T>

T contains(std::vector<T> vectorIn, T word_){
    return std::find(vectorIn.begin(),vectorIn.end(),word_);
}

bool containExact(std::vector<std::string> vectorIn, std::string word_){
    for(int i = 0; i < vectorIn.size(); i++){
        if(vectorIn.at(i) == word_){
            return true;
        }
    }
    return false;
}

int setSyntaxColor(textLine* textLineIn,int position){
    if(textLineIn->size()<=1){
        return 2;
    }
    std::string temp = "";
    char* actualWord = (char*)calloc((textLineIn->size()),(sizeof(char)));
    
    for(int textLinePointer = position; textLinePointer <= textLineIn->size()-1; textLinePointer++){
        if(containsString(operators,std::string(1,textLineIn->at(textLinePointer)))){
            break;
        }else{
            actualWord[textLinePointer] = textLineIn->at(textLinePointer);
        }
    }

    for(int textLinePointer = position; textLinePointer >= 0; textLinePointer--){
        if(containsString(operators,std::string(1,textLineIn->at(textLinePointer)))){
            break;
        }else{
            actualWord[textLinePointer] = textLineIn->at(textLinePointer);
        }
    }

    for(int i = 0; i < sizeof(actualWord)/sizeof(char); i++){
        if(actualWord[i]!='\0'){
            temp += actualWord[i];
        }
    }

    if(containExact(types,temp)){
        return 5;//type color
    }else if(containExact(containers,temp)){
        return 6;//container color
    }else if(containExact(modifiers,temp)){
        return 7;//modifiers color
    }else if(containExact(flow,temp)){
        return 8;//flow color
    }else if(containExact(loops,temp)){
        //containsString(loop,temp);
        return 9;//loops color
    }
    else{
        return 2;
    }
}