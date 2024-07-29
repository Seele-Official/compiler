#include"parser.cpp"

// enum class NODE_TYPE
// {
//     PROGRAM,
//     TYPE,
//     POINTER,
//     FUNCTION_DECLARATION,
//     VARIABLE_DECLARATION,
//     PARAMETER,
//     PARAMETER_LIST,
//     EXPRESSION_LIST,
//     COMPOUND_STATEMENT,
//     STATEMENT,
//     EXPRESSION_STATEMENT,
//     SELECTION_STATEMENT,
//     ITERATION_STATEMENT,
//     JUMP_STATEMENT,
//     EXPRESSION,
//     OPERATOR,
//     DEREFERENCE,
//     ADDRESSOF,
//     ARRAY,
//     FUNCTION_CALL,
//     IDENTIFIER,
//     CONSTANT,
//     STRING_LITERAL
// };
template <typename T>
class symbolTable{
    private:
        symbolTable<T> *fatherTable;
        List::list<T> symbols;
    public:
        symbolTable(symbolTable<T> *fatherTable){
            this->fatherTable = fatherTable;
        }
        void addSymbol(T &symbol){
            this->symbols.push_back(symbol);
        }
        bool hasSymbol(T &symbol){
            if (this->symbols.find(symbol) != -1){
                return true;
            }
            return false;
        }
        bool hasSymbolInAll()
};

class SemanticAnalyzer{
    public:
        SemanticAnalyzer(Node *node){
            this->node = node;
        }
        void analyze(){
            this->analyzeNode(this->node);
        }
    private:
        Node *node;



        void analyzeNode(Node *node){
            switch (node->getType()){
                case NODE_TYPE::PROGRAM:
                    this->analyzeProgram(node);
                    break;
                case NODE_TYPE::TYPE:
                    this->analyzeType(node);
                    break;
                case NODE_TYPE::POINTER:
                    this->analyzePointer(node);
                    break;
                case NODE_TYPE::FUNCTION_DECLARATION:
                    this->analyzeFunctionDeclaration(node);
                    break;
                case NODE_TYPE::VARIABLE_DECLARATION:
                    this->analyzeVariableDeclaration(node);
                    break;
                case NODE_TYPE::PARAMETER:
                    this->analyzeParameter(node);
                    break;
                case NODE_TYPE::PARAMETER_LIST:
                    this->analyzeParameterList(node);
                    break;
                case NODE_TYPE::EXPRESSION_LIST:
                    this->analyzeExpressionList(node);
                    break;
                case NODE_TYPE::COMPOUND_STATEMENT:
                    this->analyzeCompoundStatement(node);
                    break;
                case NODE_TYPE::STATEMENT:
                    this->analyzeStatement(node);
                    break;
                case NODE_TYPE::EXPRESSION_STATEMENT:
                    this->analyzeExpressionStatement(node);
                    break;
                case NODE_TYPE::SELECTION_STATEMENT:
                    this->analyzeSelectionStatement(node);
                    break;
                case NODE_TYPE::ITERATION_STATEMENT:
                    this->analyzeIterationStatement(node);
                    break;
                case NODE_TYPE::JUMP_STATEMENT:
                    this->analyzeJumpStatement(node);
                    break;
                case NODE_TYPE::EXPRESSION:
                    this->analyzeExpression(node);
                    break;
                case NODE_TYPE::OPERATOR:
                    this->analyzeOperator(node);
                    break;
                case NODE_TYPE::DEREFERENCE:
                    this->analyzeDereference(node);
                    break;
                case NODE_TYPE::ADDRESSOF:
                    this->analyzeAddressOf(node);
                    break;
                case NODE_TYPE::ARRAY:
                    this->analyzeArray(node);
                    break;
                case NODE_TYPE::FUNCTION_CALL:
                    this->analyzeFunctionCall(node);
                    break;
                case NODE_TYPE::IDENTIFIER:
                    this->analyzeIdentifier(node);
                    break;
                case NODE_TYPE::CONSTANT:
                    this->analyzeConstant(node);
                    break;
                case NODE_TYPE::STRING_LITERAL:
                    this->analyzeStringLiteral(node);
                    break;
            }
        }
        void analyzeProgram(Node *node){
            for (auto it = node->begin(); it != node->end(); it++){
                this->analyzeNode(*it);
            }
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
    

    List::list<TOKEN> *tokens = tokenLizer(code);
    Parser p(tokens);
    Node *node = p.parse();
    
    std::ofstream outputFile("output/ast.json");
    if (!outputFile.is_open()) {
        std::cerr << "无法打开输出文件" << std::endl;
        return 1;
    }
    outputFile << node;

    for (auto it = node->begin(); it != node->end(); it++){
        std::cout<< "it:" << *it << std::endl;
    }


    delete tokens;
    delete node;
    return 0;
}