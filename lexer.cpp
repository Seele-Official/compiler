#include<iostream>
#include<fstream>

#include"lib/String.h"
#include"lib/List.h"




enum class TOKEN_TYPE {
    OPERATORS,
    KEYWORDS,
    SEPARATORS,
    IDENTIFIER,
    CONSTANT,
    STRING_LITERAL,
    UNDEFINED,
    NONE
};
// 定义关键字枚举
enum class KEYWORDS {
    INT, // "int"
    VOID, // "void"
    CHAR, // "char"
    IF, // "if"
    ELSE, // "else"
    WHILE, // "while"
    FOR, // "for"
    RETURN, // "return"
    BREAK // "break"
};

// 定义操作符枚举
enum class OPERATORS {
    PLUS, // "+"
    MINUS, // "-"
    MULTIPLY, // "*"
    DIVIDE, // "/"
    MODULO, // "%"
    AND, // "&"
    OR, // "|"
    XOR, // "^"
    ASSIGN, // "="
    GREATER, // ">"
    LESS, // "<"

    INCREMENT, // "++"
    DECREMENT, // "--"
    LOGICAL_AND, // "&&"
    LOGICAL_OR, // "||"
    GREATER_EQUAL, // ">="
    LESS_EQUAL, // "<="
    EQUAL, // "=="
    NOT_EQUAL, // "!="
    SHIFT_LEFT, // "<<"
    SHIFT_RIGHT, // ">>"    
    ADD_ASSIGN, // "+="
    SUB_ASSIGN, // "-="
    MUL_ASSIGN, // "*="
    DIV_ASSIGN, // "/="
    MOD_ASSIGN, // "%="
    AND_ASSIGN, // "&="
    XOR_ASSIGN, // "^="
    OR_ASSIGN, // "|="

    SHL_ASSIGN, // "<<="
    SHR_ASSIGN, // ">>="
};

// 定义分隔符枚举
enum class SEPARATORS {
    LPAREN, // "("
    RPAREN, // ")"
    LBRACE, // "{"
    RBRACE, // "}"
    LBRACKET, // "["
    RBRACKET, // "]"
    SEMICOLON, // ";"
    COMMA // ","
};


List::list<String::string> KEYWORDS_LIST = {"int", "void", "char", "if", "else", "while", "for", "return", "break"};
List::list<String::string> OPERATORS_LIST = {"+", "-", "*", "/", "%", "&", "|", "^", "=", ">", "<", "++", "--", "&&", "||", ">=", "<=", "==", "!=", "<<", ">>", "+=", "-=", "*=", "/=", "%=", "&=", "^=", "|=", "<<=", ">>="};
List::list<String::string> SEPARATORS_LIST = {"(", ")", "{", "}", "[", "]", ";", ","};
std::ostream &operator<<(std::ostream &os, const KEYWORDS &k){
    std::cout << KEYWORDS_LIST[(size_t)k];
    return os;
}

std::ostream &operator<<(std::ostream &os, const OPERATORS &o){
    std::cout << OPERATORS_LIST[(size_t)o];
    return os;
}

std::ostream &operator<<(std::ostream &os, const SEPARATORS &s){
    std::cout << SEPARATORS_LIST[(size_t)s];
    return os;
}


class TOKEN{
    public:
        TOKEN_TYPE type;
        KEYWORDS kValue;
        OPERATORS oValue;
        SEPARATORS sValue;
        String::string Value;
        TOKEN(const String::string &value){
            if(KEYWORDS_LIST.find(value) != -1){
                this->type = TOKEN_TYPE::KEYWORDS;
                this->kValue = (KEYWORDS)KEYWORDS_LIST.find(value);
                this->Value = value;
            }else if(OPERATORS_LIST.find(value) != -1){
                this->type = TOKEN_TYPE::OPERATORS;
                this->oValue = (OPERATORS)OPERATORS_LIST.find(value);
                this->Value = value;
            }else if(SEPARATORS_LIST.find(value) != -1){
                this->type = TOKEN_TYPE::SEPARATORS;
                this->sValue = (SEPARATORS)SEPARATORS_LIST.find(value);
                this->Value = value;
            }else if(value[0] == '"' && value[value.getLength()-1] == '"'){
                this->type = TOKEN_TYPE::STRING_LITERAL;
                this->Value = value.slice(1, value.getLength()-1);
            }else if(value.isNumber()){
                this->type = TOKEN_TYPE::CONSTANT;
                this->Value = value;
            }else if(value[0] == '\'' && value[value.getLength()-1] == '\''){
                this->type = TOKEN_TYPE::CONSTANT;
                this->Value = value.slice(1, value.getLength()-1);
            }else if(value.isIdentifier()){
                this->type = TOKEN_TYPE::IDENTIFIER;
                this->Value = value;
            }else{
                throw std::invalid_argument("Invalid token");
            }
        }

        TOKEN(const TOKEN &t){
            this->type = t.type;
            this->kValue = t.kValue;
            this->oValue = t.oValue;
            this->sValue = t.sValue;
            this->Value = t.Value;
        }


        TOKEN(){
            this->type = TOKEN_TYPE::NONE;
        }




        bool operator==(const TOKEN &t){
            if(this->type != t.type){
                return false;
            }
            if(this->type == TOKEN_TYPE::KEYWORDS){
                return this->kValue == t.kValue;
            }
            if(this->type == TOKEN_TYPE::OPERATORS){
                return this->oValue == t.oValue;
            }
            if(this->type == TOKEN_TYPE::SEPARATORS){
                return this->sValue == t.sValue;
            }
            return this->Value == t.Value;
        }
        bool operator!=(const TOKEN &t){
            return !(*this == t);
        }
        friend std::ostream &operator<<(std::ostream &os, const TOKEN &t){
            os << "TOKEN(";
            switch (t.type)
            {
            case TOKEN_TYPE::OPERATORS:
                os << "OPERATORS";
                break;
            case TOKEN_TYPE::KEYWORDS:
                os << "KEYWORDS";
                break;
            case TOKEN_TYPE::SEPARATORS:
                os << "SEPARATORS";
                break;
            case TOKEN_TYPE::IDENTIFIER:
                os << "IDENTIFIER";
                break;
            case TOKEN_TYPE::CONSTANT:
                os << "CONSTANT";
                break;
            case TOKEN_TYPE::STRING_LITERAL:
                os << "STRING_LITERAL";
                break;
            case TOKEN_TYPE::UNDEFINED:
                os << "UNDEFINED" << ")";
                return os;
            case TOKEN_TYPE::NONE:
                os << "NONE" << ")";
                return os;
            default:
                break;
            }
            os << ", " << t.Value << ")";
            return os;
        }
};


List::list<TOKEN> tokenLizer(const String::string &code){
    List::list<TOKEN> tokens;
    String::string tempStr((size_t)100);
    for (size_t i = 0; i < code.getLength(); i++) {
        if (code[i] == ' ' || code[i] == '\n' || code[i] == '\t') {
            if (tempStr.isEmpty() == false) {
                tokens.push_back(tempStr);
                tempStr.clear();
            }
            continue;
        }
        
        if (size_t index = SEPARATORS_LIST.find(code[i]); index != -1) {
            if (tempStr.isEmpty() == false) {
                tokens.push_back(tempStr);
                tempStr.clear();
            }
            tokens.push_back(code.slice(i, i+1));
            continue;
        }

        if (code[i] == '"') {
            if (tempStr.isEmpty() == false) {
                tokens.push_back(tempStr);
                tempStr.clear();
            }
            size_t j = i;
            tempStr.push_back(code[j++]);
            while (code[j] != '"') {
                tempStr.push_back(code[j]);
                j++;
            }
            tempStr.push_back(code[j]);
            tokens.push_back(tempStr);
            tempStr.clear();
            i = j;
            continue;
        }


        if (size_t index = OPERATORS_LIST.find(code[i]); index != -1) {
            if (size_t index2 = OPERATORS_LIST.find(code.slice(i, i+2)); index2 != -1) {
                if (size_t index3 = OPERATORS_LIST.find(code.slice(i, i+3)); index3 != -1) {
                    if (tempStr.isEmpty() == false) {
                        tokens.push_back(tempStr);
                        tempStr.clear();
                    }
                    tokens.push_back(code.slice(i, i+3));
                    i += 2;
                    continue;
                }
                if (tempStr.isEmpty() == false) {
                    tokens.push_back(tempStr);
                    tempStr.clear();
                }
                tokens.push_back(code.slice(i, i+2));
                i++;
                continue;
            }
            if (tempStr.isEmpty() == false) {
                tokens.push_back(tempStr);
                tempStr.clear();
            }
            tokens.push_back(code.slice(i, i+1));
            continue;
        }
        tempStr.push_back(code[i]);
    }
    
    if (tempStr.isEmpty() == false) {
        tokens.push_back(tempStr);
    }
    return tokens;
}




