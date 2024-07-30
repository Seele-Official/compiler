#include"parser.cpp"


enum class SymbolType{
    VARIABLE,
    FUNCTION
};
enum class ValueType{
    INT,
    CHAR,
    VOID,
    POINTER
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
    switch (v)
    {
    case ValueType::INT:
        os << "int";
        break;
    case ValueType::CHAR:
        os << "char";
        break;
    case ValueType::VOID:
        os << "void";
        break;
    case ValueType::POINTER:
        os << "pointer";
        break;
    default:
        break;
    }
    return os;
}
class Symbol{
    private:
        SymbolType type;
        String::string name;
        ValueType valueType;
        //return type for function or value type for variable

        List::list<ValueType> parameters;
    public:
        Symbol(){

        }
        Symbol(Symbol &&other){
            this->type = other.type;
            this->name = std::move(other.name);
            this->valueType = other.valueType;
            if (other.type == SymbolType::FUNCTION){
                this->parameters = std::move(other.parameters);
            }
        }
        Symbol(Symbol &other){
            this->type = other.type;
            this->name = other.name;
            this->valueType = other.valueType;
            if (other.type == SymbolType::FUNCTION){
                this->parameters = other.parameters;
            }
        }


        Symbol(SymbolType type, String::string &name, ValueType valueType){
            this->type = type;
            this->name = std::move(name);
            this->valueType = valueType;
        }
        Symbol(SymbolType type, String::string &name, ValueType valueType, List::list<ValueType> &parameters){
            this->type = type;
            this->name = std::move(name);
            this->valueType = valueType;
            this->parameters = std::move(parameters);
        }
        SymbolType getType(){
            return this->type;
        }
        String::string getName(){
            return this->name;
        }
        ValueType getValueType(){
            return this->valueType;
        }
        List::list<ValueType> getParameters(){
            return this->parameters;
        }
        bool operator==(const Symbol &s){
            return this->name == s.name;
        }
        void operator=(const Symbol &other){
            this->type = other.type;
            this->name = other.name;
            this->valueType = other.valueType;
            if (other.type == SymbolType::FUNCTION){
                this->parameters = other.parameters;
            }
        }
        Symbol &operator=(Symbol &&other){
            if (this == &other){
                return *this;
            }
            this->type = other.type;
            this->name = std::move(other.name);
            this->valueType = other.valueType;
            if (other.type == SymbolType::FUNCTION){
                this->parameters = std::move(other.parameters);
            }
            return *this;
        }



        friend std::ostream &operator<<(std::ostream &os, Symbol &s){
            os << "Symbol(" << s.type << ", " << s.valueType << ", " << s.name;
            if (s.type == SymbolType::FUNCTION){
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
            if (node->getType() == NODE_TYPE::POINTER){
                return ValueType::POINTER;
            }else{
                switch (node->getToken().kValue)
                {
                case KEYWORDS::INT:
                    return ValueType::INT;
                    break;
                case KEYWORDS::CHAR:
                    return ValueType::CHAR;
                    break;
                case KEYWORDS::VOID:
                    return ValueType::VOID;
                    break;
                default:
                    break;
                }
            }
            return;
        }
        String::string analyzeIdentifier(Node *identifierNode){
            return identifierNode->getToken().Value;
        }
        ValueType analyzeOperator(Node *node){
            ValueType left, right;
            if (node->getToken().oValue != OPERATORS::INCREMENT && node->getToken().oValue != OPERATORS::DECREMENT){
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
        ValueType analyzeExpression(Node *node){
            switch (node->getType()){
                case NODE_TYPE::CONSTANT:
                    return ValueType::INT;
                    break;
                case NODE_TYPE::STRING_LITERAL:
                    return ValueType::CHAR;
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
                default:
                    break;
            }
        }

        Symbol analyzeVariableDeclaration(Node *node){
            Node *type;
            Node *identifier;
            if (node->getChildrenLength() == 2){
                type = node->getChild(0);
                identifier = node->getChild(1);
            }else{
                this->analyzeExpression(node->getChild(0));
                type = node->getChild(1);
                identifier = node->getChild(2);
            }
            ValueType valueType = this->analyzeType(type);
            String::string name = this->analyzeIdentifier(identifier);
            Symbol symbol(SymbolType::VARIABLE, name, valueType);
            return symbol;
        }
        void analyzeCompoundStatement(Node *node){
            for (auto it = node->begin(); it != node->end(); it++){
                Symbol symbol;
                switch ((*it)->getType()){
                    case NODE_TYPE::VARIABLE_DECLARATION:
                        symbol = std::move(this->analyzeVariableDeclaration(*it));
                        if (localTable.hasSymbol(symbol)){
                            throw std::invalid_argument("Variable redeclaration");
                        }
                        this->localTable.addSymbol(symbol);
                }
            }
        }

        void analyzeFunctionDeclaration(Node *node){
            Node *type = node->getChild(0);
            Node *identifier = node->getChild(1);
            Node *parameterList = node->getChild(2);
            Node *compoundStatement = node->getChild(3);


            ValueType valueType = this->analyzeType(type);
            String::string name = this->analyzeIdentifier(identifier);
            List::list<ValueType> parameters;

            this->localTable = std::move(symbolTable(&this->globalTable));
            for (auto it = parameterList->begin(); it != parameterList->end(); it++){
                ValueType v = this->analyzeType((*it)->getChild(0));
                parameters.push_back(v);
                String::string n = std::move(this->analyzeIdentifier((*it)->getChild(1)));
                Symbol symbol(SymbolType::VARIABLE, n, v);

                localTable.addSymbol(symbol);
            }
            std::cout << localTable << std::endl;
            Symbol symbol(SymbolType::FUNCTION, name, valueType, parameters);
            std::cout << symbol << std::endl;
            this->globalTable.addSymbol(symbol);
            this->analyzeCompoundStatement(compoundStatement);
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
    String::string code(size);
    char ch;
    while (inputFile.get(ch)){
        code.push_back(ch);
    }
    inputFile.close();
    

    Parser p(std::move(tokenLizer(code)));
    Node *node = p.parse();
    
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