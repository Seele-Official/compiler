#include"parser.cpp"


enum class SymbolType{
    VARIABLE,
    FUNCTION
};

class ValueType{
    public:
        enum class Type{
            INT,
            CHAR,
            VOID
        };
        Type type;
        uint8_t pointLevel;
        ValueType(Type type, uint8_t pointLevel = 0){
            this->type = type;
            this->pointLevel = pointLevel;
        }
        ValueType(){
            this->type = Type::INT;
            this->pointLevel = 0;
        }
        ValueType& operator=(const ValueType &other){
            this->type = other.type;
            this->pointLevel = other.pointLevel;
            return *this;
        }
        bool operator==(const ValueType &other){
            return this->type == other.type && this->pointLevel == other.pointLevel;
        }
        bool operator!=(const ValueType &other){
            return this->type != other.type || this->pointLevel != other.pointLevel;
        }
};
std::ostream &operator<<(std::ostream &os, SymbolType &s){
    switch (s)
    {
    case SymbolType::VARIABLE:
        os << "variable";
        break;
    case SymbolType::FUNCTION:
        os << "function";
        break;
    default:
        break;
    }
    return os;
}
std::ostream &operator<<(std::ostream &os, ValueType &v){
    switch (v.type){
        case ValueType::Type::INT:
            os << "int";
            break;
        case ValueType::Type::CHAR:
            os << "char";
            break;
        case ValueType::Type::VOID:
            os << "void";
            break;
        default:
            break;
    }
    for (uint8_t i = 0; i < v.pointLevel; i++){
        os << "*";
    }
    return os;
}
class Symbol{
    private:
        SymbolType symbolType;
        ValueType valueType; //return type for function or value type for variable
        String::string name;

        List::list<ValueType> parameters;
    public:
        Symbol(){
        }
        Symbol(Symbol &&other){
            this->symbolType = other.symbolType;
            this->name = std::move(other.name);
            this->valueType = other.valueType;
            if (other.symbolType == SymbolType::FUNCTION){
                this->parameters = std::move(other.parameters);
            }
        }
        Symbol(Symbol &other){
            this->symbolType = other.symbolType;
            this->name = other.name;
            this->valueType = other.valueType;
            if (other.symbolType == SymbolType::FUNCTION){
                this->parameters = other.parameters;
            }
        }

        Symbol(SymbolType type, ValueType valueType, String::string &name){
            this->symbolType = type;
            this->name = std::move(name);
            this->valueType = valueType;
        }
        Symbol(SymbolType type, ValueType valueType, String::string &name, List::list<ValueType> &parameters){
            this->symbolType = type;
            this->name = std::move(name);
            this->valueType = valueType;
            this->parameters = std::move(parameters);
        }
        SymbolType getType(){
            return this->symbolType;
        }
        ValueType getValueType(){
            return this->valueType;
        }
        String::string getName(){
            return this->name;
        }
        
        List::list<ValueType> getParameters(){
            return this->parameters;
        }
        bool operator==(const Symbol &s){
            return this->name == s.name;
        }
        bool operator!=(const Symbol &s){
            return this->name != s.name;
        }
        void operator=(const Symbol &other){
            this->symbolType = other.symbolType;
            this->name = other.name;
            this->valueType = other.valueType;
            if (other.symbolType == SymbolType::FUNCTION){
                this->parameters = other.parameters;
            }
        }
        Symbol &operator=(Symbol &&other){
            if (this == &other){
                return *this;
            }
            this->symbolType = other.symbolType;
            this->name = std::move(other.name);
            this->valueType = other.valueType;
            if (other.symbolType == SymbolType::FUNCTION){
                this->parameters = std::move(other.parameters);
            }
            return *this;
        }



        friend std::ostream &operator<<(std::ostream &os, Symbol &s){
            os << "Symbol(" << s.symbolType << ", " << s.valueType << ", " << s.name;
            if (s.symbolType == SymbolType::FUNCTION){
                os << ", parameters: " << s.parameters;
            }
            os << ")";
            return os;
        }

};




class symbolTable{
    private:
        symbolTable *fatherTable;
        List::list<Symbol> symbols;
    public:
        symbolTable(){
            this->fatherTable = nullptr;
        }
        symbolTable(symbolTable *fatherTable){
            this->fatherTable = fatherTable;
        }
        symbolTable(symbolTable &&other){
            this->fatherTable = other.fatherTable;
            this->symbols = std::move(other.symbols);
        }
        
        symbolTable &operator=(symbolTable &&other){
            if (this == &other){
                return *this;
            }
            this->fatherTable = other.fatherTable;
            this->symbols = std::move(other.symbols);
            return *this;
        }
        
        void addSymbol(Symbol &symbol){
            this->symbols.push_back(std::move(symbol));
        }
        bool hasSymbol(Symbol &symbol){
            if (this->symbols.find(symbol) != -1){
                return true;
            }
            return false;
        }
        bool hsSymbolInAll(Symbol &symbol){
            symbolTable *current = this;
            while (current != nullptr){
                if (current->hasSymbol(symbol)){
                    return true;
                }
                current = current->fatherTable;
            }
            return false;
        }
        Symbol findSymbol(String::string name){
            for (auto it = this->symbols.begin(); it != this->symbols.end(); it++){
                if ((*it).getName() == name){
                    return *it;
                }
            }
            if (this->fatherTable != nullptr){
                return this->fatherTable->findSymbol(name);
            }
            throw std::invalid_argument("Symbol not found");
        }
        
        
        friend std::ostream &operator<<(std::ostream &os, symbolTable &st){
            os << "SymbolTable(" << st.symbols << ")";
            return os;
        }

};


class SemanticAnalyzer{
    public:
        SemanticAnalyzer(Node *node){
            this->node = node;
        }
        void analyze(){
            this->analyzeProgram(this->node);
        }
    private:
        Node *node;
        symbolTable globalTable;
        symbolTable localTable;


        void analyzeProgram(Node *node){
            for (auto it = node->begin(); it != node->end(); it++){
                this->analyzeFunctionDeclaration(*it);
            }
            return;
        }
        ValueType analyzeType(Node *node){
            auto func = [](KEYWORDS keyword){
                switch (keyword)
                {
                case KEYWORDS::INT:
                    return ValueType::Type::INT;
                    break;
                case KEYWORDS::CHAR:
                    return ValueType::Type::CHAR;
                    break;
                case KEYWORDS::VOID:
                    return ValueType::Type::VOID;
                    break;
                default:
                    break;
                }
            };

            if (node->getType() == NODE_TYPE::POINTER){
                uint8_t level = 0;
                while (node->getType() == NODE_TYPE::POINTER){
                    level++;
                    node = node->getChild(0);
                }
                return ValueType(func(node->getToken().kValue), level);
            }else{
                return ValueType(func(node->getToken().kValue));
            }
        }
        String::string analyzeIdentifier(Node *identifierNode){
            if (identifierNode->getType() == NODE_TYPE::ARRAY){
                return identifierNode->getChild(0)->getToken().Value;
            }
            return identifierNode->getToken().Value;
        }
        ValueType analyzeOperator(Node *node){
            ValueType left, right;
            OPERATORS op = node->getToken().oValue;

            if (op != OPERATORS::INCREMENT && op != OPERATORS::DECREMENT){
                left = this->analyzeExpression(node->getChild(0));
                right = this->analyzeExpression(node->getChild(1));
                if (left == right){
                    return left;
                }else{
                    throw std::invalid_argument("Type mismatch");
                }
            }else{
                return this->analyzeExpression(node->getChild(0));
            }

        }
        // 保证表达式的类型一致
        ValueType analyzeExpression(Node *node){
            switch (node->getType()){
                case NODE_TYPE::CONSTANT:
                    return ValueType::Type::INT;
                    break;
                case NODE_TYPE::STRING_LITERAL:
                    return ValueType(ValueType::Type::CHAR, 1);
                    break;
                case NODE_TYPE::IDENTIFIER:
                    return this->localTable.findSymbol(node->getToken().Value).getValueType();
                    break;
                case NODE_TYPE::FUNCTION_CALL:
                    return this->localTable.findSymbol(node->getChild(0)->getToken().Value).getValueType();
                    break;
                case NODE_TYPE::OPERATOR:
                    return this->analyzeOperator(node);
                    break;
                case NODE_TYPE::EXPRESSION:
                    return this->analyzeExpression(node->getChild(0));
                    break;
                default:
                    break;
            }
        }

        Symbol analyzeVariableDeclaration(Node *node){
            Node *type;
            Node *identifier;
            ValueType valueType;
            if (node->getChildrenLength() == 2){
                type = node->getChild(0);
                identifier = node->getChild(1);
                valueType = this->analyzeType(type);
            }else{
                ValueType exprtype = this->analyzeExpression(node->getChild(0));
                type = node->getChild(1);
                identifier = node->getChild(2);
                valueType = this->analyzeType(type);
                if (exprtype != valueType){
                    throw std::invalid_argument("Type mismatch");
                }
            }
            if (valueType == ValueType::Type::VOID){
                throw std::invalid_argument("Void variable");
            }
            
            String::string name = this->analyzeIdentifier(identifier);
            Symbol symbol(SymbolType::VARIABLE, valueType, name);
            return symbol;
        }
        void analyzeCompoundStatement(Node *node){
            for (auto it = node->begin(); it != node->end(); it++){
                Symbol symbol;
                switch ((*it)->getType()){
                    case NODE_TYPE::VARIABLE_DECLARATION:
                        symbol = std::move(this->analyzeVariableDeclaration(*it));
                        if (localTable.hasSymbol(symbol)){
                            std::cout << localTable << std::endl;
                            std::cout << symbol << std::endl;
                            std::cout << globalTable << std::endl;
                            throw std::invalid_argument("Variable redeclaration");
                        }
                        this->localTable.addSymbol(symbol);
                }
            }
        }

        void analyzeFunctionDeclaration(Node *node){
            Node *type = node->getChild(0);
            Node *identifier = node->getChild(1);
            Node *parameterList;
            Node *compoundStatement;
            if (node->getChildrenLength() == 4){
                parameterList = node->getChild(2);
                compoundStatement = node->getChild(3);
            }else{
                parameterList = nullptr;
                compoundStatement = node->getChild(2);
            }


            ValueType valueType = this->analyzeType(type);

            String::string name = this->analyzeIdentifier(identifier);
            List::list<ValueType> parameters;

            this->localTable = std::move(symbolTable(&this->globalTable));
            if (parameterList != nullptr){
                for (auto it = parameterList->begin(); it != parameterList->end(); it++){
                    ValueType v = this->analyzeType((*it)->getChild(0));
                    parameters.push_back(v);
                    String::string n = std::move(this->analyzeIdentifier((*it)->getChild(1)));
                    Symbol symbol(SymbolType::VARIABLE, v, n);
                    localTable.addSymbol(symbol);
                } 
            }
            std::cout << localTable << std::endl;
            Symbol symbol(SymbolType::FUNCTION, valueType, name, parameters);
            this->globalTable.addSymbol(symbol);
            this->analyzeCompoundStatement(compoundStatement);
            std::cout << localTable << std::endl << std::endl;
            return;
        }
        


};

std::streampos getFileSize(std::ifstream &file){
    std::streampos current = file.tellg();
    file.seekg(0, std::ios::end);
    std::streampos end = file.tellg();
    file.seekg(current, std::ios::beg);
    return end;
}
int main(){
    std::ifstream inputFile("output/code.c");
    if (!inputFile.is_open()) {
        std::cerr << "无法打开输入文件" << std::endl;
        return 1;
    }
    size_t size = getFileSize(inputFile);
    String::string code("", 0, size);
    char ch;
    while (inputFile.get(ch)){
        code.push_back(ch);
    }
    inputFile.close();
    

    Parser p(std::move(tokenLizer(code)));
    Node *node = p.parse();
    std::cout << node << std::endl;
    // std::ofstream outputFile("output/ast.json");
    // if (!outputFile.is_open()) {
    //     std::cerr << "无法打开输出文件" << std::endl;
    //     return 1;
    // }
    // outputFile << node;

    SemanticAnalyzer sa(node);
    sa.analyze();

    delete node;
    return 0;
}