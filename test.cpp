#include <iostream>
#include"lib/String.h"
#include"lib/List.h"


int main() {
    List::list<String::string> KEYWORDS_LIST = {"int", "void", "char", "if", "else", "while", "for", "return", "break"};
    std::cout << KEYWORDS_LIST << std::endl;
    List::list<String::string> KEYWORDS_LIST2 = std::move(KEYWORDS_LIST);
    std::cout << KEYWORDS_LIST << std::endl;
    std::cout << KEYWORDS_LIST2 << std::endl;

    
    std::cout << "All tests passed." << std::endl;
    return 0;
}