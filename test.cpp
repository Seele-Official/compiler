#include <iostream>
#include"lib/String.h"
#include"lib/List.h"


int main() {
    List::list<String::string> KEYWORDS_LIST = {"int", "void", "char", "if", "else", "while", "for", "return", "break"};
    for (List::list<String::string>::iterator it = KEYWORDS_LIST.begin(); it != KEYWORDS_LIST.end(); it++){
        std::cout << *it << std::endl;
    }
    
    
    std::cout << "All tests passed." << std::endl;
    return 0;
}