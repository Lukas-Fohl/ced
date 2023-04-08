#ifndef SYNTAX
#define SYNTAX 1

#include "textBuffer.hpp"

std::vector<std::string> types = {"short", "int", "long", "float", "double", "char", "void", "bool", "FILE", "auto"};
std::vector<std::string> containers = {"enum", "struct", "union", "typedef","#include"};
std::vector<std::string> modifiers = {"const", "volatile", "extern", "static", "register", "signed", "unsigned"};
std::vector<std::string> flow = {"if", "else","goto","case", "default","continue", "break"};
std::vector<std::string> loops = {"for", "do", "while" "switch"};
std::vector<std::string> operators = {"-","+","*","/","\\","!","&","~",".","%","=","|",":","?","(",")","[","]","{","}","\"","\'","^"," ","\n","\t","\r",";"};

int setSyntaxColor(textLine* displayedText,int position);

#endif
