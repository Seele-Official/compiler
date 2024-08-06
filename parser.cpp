#include"lexer.cpp"

enum class NODE_TYPE
{
    PROGRAM,
    TYPE,
    POINTER,
    FUNCTION_DECLARATION,
    VARIABLE_DECLARATION,
    PARAMETER,
    PARAMETER_LIST,
    EXPRESSION_LIST,
    COMPOUND_STATEMENT,
    EXPRESSION_STATEMENT,
    SELECTION_STATEMENT,
    ITERATION_STATEMENT,
    JUMP_STATEMENT,
    EXPRESSION,
    OPERATOR,
    DEREFERENCE,
    ADDRESSOF,
    ARRAY,
    FUNCTION_CALL,
    IDENTIFIER,
    CONSTANT,
    STRING_LITERAL
};
std::ostream& operator<<(std::ostream& os, const NODE_TYPE& type){
    switch (type)
    {
    case NODE_TYPE::PROGRAM:
        os << "PROGRAM";
        break;
    case NODE_TYPE::TYPE:
        os << "TYPE";
        break;
    case NODE_TYPE::POINTER:
        os << "POINTER";
        break;
    case NODE_TYPE::FUNCTION_DECLARATION:
        os << "FUNCTION_DECLARATION";
        break;
    case NODE_TYPE::VARIABLE_DECLARATION:
        os << "VARIABLE_DECLARATION";
        break;
    case NODE_TYPE::PARAMETER:
        os << "PARAMETER";
        break;
    case NODE_TYPE::PARAMETER_LIST:
        os << "PARAMETER_LIST";
        break;
    case NODE_TYPE::EXPRESSION_LIST:
        os << "EXPRESSION_LIST";
        break;
    case NODE_TYPE::COMPOUND_STATEMENT:
        os << "COMPOUND_STATEMENT";
        break;
    case NODE_TYPE::EXPRESSION_STATEMENT:
        os << "EXPRESSION_STATEMENT";
        break;
    case NODE_TYPE::SELECTION_STATEMENT:
        os << "SELECTION_STATEMENT";
        break;
    case NODE_TYPE::ITERATION_STATEMENT:
        os << "ITERATION_STATEMENT";
        break;
    case NODE_TYPE::JUMP_STATEMENT:
        os << "JUMP_STATEMENT";
        break;
    case NODE_TYPE::EXPRESSION:
        os << "EXPRESSION";
        break;
    case NODE_TYPE::OPERATOR:
        os << "OPERATOR";
        break;
    case NODE_TYPE::DEREFERENCE:
        os << "DEREFERENCE";
        break;
    case NODE_TYPE::ADDRESSOF:
        os << "ADDRESSOF";
        break;
    case NODE_TYPE::ARRAY:
        os << "ARRAY";
        break;
    case NODE_TYPE::FUNCTION_CALL:
        os << "FUNCTION_CALL";
        break;
    case NODE_TYPE::IDENTIFIER:
        os << "IDENTIFIER";
        break;
    case NODE_TYPE::CONSTANT:
        os << "CONSTANT";
        break;
    case NODE_TYPE::STRING_LITERAL:
        os << "STRING_LITERAL";
        break;
    default:
        break;
    }
    return os;
}
class Node
{
private:
    NODE_TYPE type;
    List::list<Node*> children;
    TOKEN token;
public:
    void addChild(Node* child){
        children.push_back(child);
    }
    friend std::ostream& operator<<(std::ostream& os, const Node* node){
        os << "{";
        os << "\"type\": \"" << node->type << "\"";
        if (node->token.type != TOKEN_TYPE::NONE){
            os << ", " << "\"token\": \"" << node->token << "\"";
        }
        
        if (node->children.getLength() > 0){
            os << ", " << "\"children\": [";
            for (size_t i = 0; i < node->children.getLength(); i++)
            {
                os << node->children[i];
                if(i != node->children.getLength() - 1){
                    os << ", ";
                }
            }
            os << "]";
        }
        os << "}";
        return os;
    }
    
    NODE_TYPE getType(){
        return this->type;
    }
    TOKEN getToken(){
        return token;
    }
    size_t getChildrenLength(){
        return children.getLength();
    }
    Node* getChild(size_t index){
        return children[index];
    }
    List::list<Node*>::iterator begin(){
        return children.begin();
    }
    List::list<Node*>::iterator end(){
        return children.end();
    }

    Node(NODE_TYPE type, TOKEN token);
    Node(NODE_TYPE type);
    Node();
    ~Node();
};
Node::Node(NODE_TYPE type, TOKEN token){
    this->type = type;
    this->token = token;
}
Node::Node(NODE_TYPE type){
    this->type = type;
}
Node::Node()
{
}
Node::~Node()
{
    Node* child;
    while (children.getLength() > 0)
    {
        child = children.pop_front();
        delete child;
    }   
}

class Parser
{
private:
    List::list<TOKEN> tokens;
    TOKEN getCurrentToken(){
        try{
            return tokens[0];
        } catch (std::out_of_range e){
            std::cout << "No more tokens" << std::endl;
            return TOKEN();
        }
    }
    TOKEN getNextToken(){
        try{
            return tokens[1];
        } catch (std::out_of_range e){
            std::cout << "No more tokens" << std::endl;
            return TOKEN();
        }
    }
    TOKEN getNextNextToken(){
        try{
            return tokens[2];
        } catch (std::out_of_range e){
            std::cout << "No more tokens" << std::endl;
            return TOKEN();
        }
    }
    void consumeToken(){
        tokens.pop_front();
    }
    Node* parsePrimaryExpression();
    Node* parsePostfixExpression();
    Node* parseUnaryExpression();
    Node* parseMultiplicativeExpression();
    Node* parseAdditiveExpression();
    Node* parseShiftExpression();
    Node* parseRelationalExpression();
    Node* parseBitwiseExpression();
    Node* parseLogicalAndExpression();
    Node* parseLogicalOrExpression();
    Node* parseAssignmentExpression();
    Node* parseExpression();



    Node* parseExpressionStatement();
    Node* parseCompoundStatement();
    Node* parseSelectionStatenment();
    Node* parseIterationStatement();
    Node* parseJumpStatement();
    Node* parseTypeSpecifier();
    Node* parseVariableDeclaration();

    Node* parseStatement();

    Node* parseParameterList();
    Node* parseFunctionDeclaration();
    Node* parseDeclarator();
    Node* parseProgram();

public:
    Parser(List::list<TOKEN> &&tokens);
    ~Parser();
    Node* parse();
};

Node* Parser::parsePrimaryExpression(){
    TOKEN currentToken = getCurrentToken();
    if(currentToken.type == TOKEN_TYPE::IDENTIFIER){
        consumeToken();
        return new Node(NODE_TYPE::IDENTIFIER, currentToken);
    } else if(currentToken.type == TOKEN_TYPE::CONSTANT){
        consumeToken();
        return new Node(NODE_TYPE::CONSTANT, currentToken);
    } else if(currentToken.type == TOKEN_TYPE::STRING_LITERAL){
        consumeToken();
        return new Node(NODE_TYPE::STRING_LITERAL, currentToken);
    } else if(currentToken.type == TOKEN_TYPE::SEPARATORS && currentToken.sValue == SEPARATORS::LPAREN){
        consumeToken();
        Node* expression = parseExpression();
        TOKEN closingBracket = getCurrentToken();
        if(closingBracket.type != TOKEN_TYPE::SEPARATORS && closingBracket.sValue != SEPARATORS::RPAREN){
            throw std::runtime_error("Expected closing bracket");
        }
        consumeToken();
        return expression;
    } else {
        throw std::runtime_error("Expected primary expression");
    }
}
Node* Parser::parsePostfixExpression(){
    Node* postfixExprNode = parsePrimaryExpression();

    TOKEN currentToken = getCurrentToken();
    if(currentToken.type == TOKEN_TYPE::SEPARATORS && currentToken.sValue == SEPARATORS::LBRACKET){
        consumeToken();
        Node* expression = parseExpression();
        TOKEN closingBracket = getCurrentToken();
        if(closingBracket.type != TOKEN_TYPE::SEPARATORS && closingBracket.sValue != SEPARATORS::RBRACKET){
            throw std::runtime_error("Expected closing bracket");
        }
        consumeToken();
        Node* arrayNode = new Node(NODE_TYPE::ARRAY);
        arrayNode->addChild(postfixExprNode);
        arrayNode->addChild(expression);
        postfixExprNode = arrayNode;
    } else if(currentToken.type == TOKEN_TYPE::SEPARATORS && currentToken.sValue == SEPARATORS::LPAREN){
        consumeToken();
        Node* functionCallNode = new Node(NODE_TYPE::FUNCTION_CALL);
        functionCallNode->addChild(postfixExprNode);
        if (getCurrentToken().type == TOKEN_TYPE::SEPARATORS && getCurrentToken().sValue == SEPARATORS::RPAREN){
            consumeToken();
            postfixExprNode = functionCallNode;
        }
        Node* expression = parseExpression();
        functionCallNode->addChild(expression);
        postfixExprNode = functionCallNode;
        TOKEN closingBracket = getCurrentToken();
        if(closingBracket.type != TOKEN_TYPE::SEPARATORS && closingBracket.sValue != SEPARATORS::RPAREN){
            throw std::runtime_error("Expected closing bracket");
        }else{
            consumeToken();
        }
    } else if(currentToken.type == TOKEN_TYPE::OPERATORS && (currentToken.oValue == OPERATORS::INCREMENT || currentToken.oValue == OPERATORS::DECREMENT)){
        consumeToken();
        Node* node = new Node(NODE_TYPE::OPERATOR, currentToken);
        node->addChild(postfixExprNode);
        postfixExprNode = node;
    }
    
    
    return postfixExprNode;
}
Node* Parser::parseUnaryExpression(){
    TOKEN currentToken = getCurrentToken();
    if (currentToken.type == TOKEN_TYPE::OPERATORS){
        Node* node;
        switch (currentToken.oValue){
            case OPERATORS::MULTIPLY:
                consumeToken();
                node = new Node(NODE_TYPE::DEREFERENCE, currentToken);
                node->addChild(parseUnaryExpression());
                return node;
            case OPERATORS::AND:
                consumeToken();
                node = new Node(NODE_TYPE::ADDRESSOF, currentToken);
                node->addChild(parseUnaryExpression());
                return node;
        }
    }
    Node* postfixExprNode = parsePostfixExpression();
    return postfixExprNode;
}
Node* Parser::parseMultiplicativeExpression(){
    Node* unaryExprNode = parseUnaryExpression();
    TOKEN currentToken = getCurrentToken();
    if (currentToken.type == TOKEN_TYPE::OPERATORS && (currentToken.Value == "*" || currentToken.Value == "/" || currentToken.Value == "%")){
        consumeToken();
        Node* mulExprNode = parseMultiplicativeExpression();
        Node* node = new Node(NODE_TYPE::OPERATOR, currentToken);
        node->addChild(unaryExprNode);
        node->addChild(mulExprNode);
        return node;
    }
    return unaryExprNode;
}
Node* Parser::parseAdditiveExpression(){
    Node* mulExprNode = parseMultiplicativeExpression();
    TOKEN currentToken = getCurrentToken();
    if (currentToken.type == TOKEN_TYPE::OPERATORS && (currentToken.Value == "+" || currentToken.Value == "-")){
        consumeToken();
        Node* addExprNode = parseAdditiveExpression();
        Node* node = new Node(NODE_TYPE::OPERATOR, currentToken);
        node->addChild(mulExprNode);
        node->addChild(addExprNode);
        return node;
    }
    return mulExprNode;
}
Node* Parser::parseShiftExpression(){
    Node* addExprNode = parseAdditiveExpression();
    TOKEN currentToken = getCurrentToken();
    if (currentToken.type == TOKEN_TYPE::OPERATORS && (currentToken.Value == "<<" || currentToken.Value == ">>")){
        consumeToken();
        Node* shiftExprNode = parseShiftExpression();
        Node* node = new Node(NODE_TYPE::OPERATOR, currentToken);
        node->addChild(addExprNode);
        node->addChild(shiftExprNode);
        return node;
    }
    return addExprNode;
}
Node* Parser::parseRelationalExpression(){
    Node* shiftExprNode = parseShiftExpression();
    TOKEN currentToken = getCurrentToken();
    if (currentToken.type == TOKEN_TYPE::OPERATORS && (currentToken.Value == "<" || currentToken.Value == ">" || currentToken.Value == "<=" || currentToken.Value == ">=" || currentToken.Value == "==" || currentToken.Value == "!=")){
        consumeToken();
        Node* relExprNode = parseRelationalExpression();
        Node* node = new Node(NODE_TYPE::OPERATOR, currentToken);
        node->addChild(shiftExprNode);
        node->addChild(relExprNode);
        return node;
    }
    return shiftExprNode;
}
Node* Parser::parseBitwiseExpression(){
    Node* relExprNode = parseRelationalExpression();
    TOKEN currentToken = getCurrentToken();
    if (currentToken.type == TOKEN_TYPE::OPERATORS && (currentToken.Value == "&" || currentToken.Value == "|" || currentToken.Value == "^")){
        consumeToken();
        Node* bitwiseExprNode = parseBitwiseExpression();
        Node* node = new Node(NODE_TYPE::OPERATOR, currentToken);
        node->addChild(relExprNode);
        node->addChild(bitwiseExprNode);
        return node;
    }
    return relExprNode;
}
Node* Parser::parseLogicalAndExpression(){
    Node* bitwiseExprNode = parseBitwiseExpression();
    TOKEN currentToken = getCurrentToken();
    if (currentToken.type == TOKEN_TYPE::OPERATORS && currentToken.Value == "&&"){
        consumeToken();
        Node* logicalAndExprNode = parseLogicalAndExpression();
        Node* node = new Node(NODE_TYPE::OPERATOR, currentToken);
        node->addChild(bitwiseExprNode);
        node->addChild(logicalAndExprNode);
        return node;
    }
    return bitwiseExprNode;
}
Node* Parser::parseLogicalOrExpression(){
    Node* logicalAndExprNode = parseLogicalAndExpression();
    TOKEN currentToken = getCurrentToken();
    if (currentToken.type == TOKEN_TYPE::OPERATORS && currentToken.Value == "||"){
        consumeToken();
        Node* logicalOrExprNode = parseLogicalOrExpression();
        Node* node = new Node(NODE_TYPE::OPERATOR, currentToken);
        node->addChild(logicalAndExprNode);
        node->addChild(logicalOrExprNode);
        return node;
    }
    return logicalAndExprNode;
}
Node* Parser::parseAssignmentExpression(){
    Node* logicalOrExprNode = parseLogicalOrExpression();
    TOKEN currentToken = getCurrentToken();
    if (currentToken.type == TOKEN_TYPE::OPERATORS && (currentToken.Value == "=" || currentToken.Value == "+=" || currentToken.Value == "-=" || currentToken.Value == "*=" || currentToken.Value == "/=" || currentToken.Value == "%=" || currentToken.Value == "<<=" || currentToken.Value == ">>=" || currentToken.Value == "&=" || currentToken.Value == "|=" || currentToken.Value == "^=")){
        consumeToken();
        Node* assignmentExprNode = parseAssignmentExpression();
        Node* node = new Node(NODE_TYPE::OPERATOR, currentToken);
        node->addChild(logicalOrExprNode);
        node->addChild(assignmentExprNode);
        return node;
    }
    return logicalOrExprNode;
}
Node* Parser::parseExpression(){
    Node* expressionNode = new Node(NODE_TYPE::EXPRESSION);
    Node* tempNode = parseAssignmentExpression();

    TOKEN currentToken = getCurrentToken();
    while (currentToken.type == TOKEN_TYPE::SEPARATORS && currentToken.sValue == SEPARATORS::COMMA){
        consumeToken();
        Node* commaNode = new Node(NODE_TYPE::OPERATOR, currentToken);
        commaNode->addChild(tempNode);
        Node* assignmentExprNode = parseAssignmentExpression();
        commaNode->addChild(assignmentExprNode);
        tempNode = commaNode;
        currentToken = getCurrentToken();
    }
    expressionNode->addChild(tempNode);
    return expressionNode;
}




Node* Parser::parseExpressionStatement(){
    TOKEN currentToken = getCurrentToken();
    if (currentToken.type == TOKEN_TYPE::SEPARATORS && currentToken.sValue == SEPARATORS::SEMICOLON){
        consumeToken();
        return new Node(NODE_TYPE::EXPRESSION_STATEMENT);
    }
    Node* expressionNode = parseExpression();
    currentToken = getCurrentToken();
    if (currentToken.type == TOKEN_TYPE::SEPARATORS && currentToken.sValue == SEPARATORS::SEMICOLON){
        consumeToken();
        Node* expressionStatementNode = new Node(NODE_TYPE::EXPRESSION_STATEMENT);
        expressionStatementNode->addChild(expressionNode);
        return expressionStatementNode;
    } else {
        throw std::runtime_error("Expected ;");
    }
}
Node* Parser::parseCompoundStatement(){
    TOKEN currentToken = getCurrentToken();
    if (currentToken.type == TOKEN_TYPE::SEPARATORS && currentToken.sValue == SEPARATORS::LBRACE){
        consumeToken();
        Node* compoundStatementNode = new Node(NODE_TYPE::COMPOUND_STATEMENT);
        while (true)
        {
            currentToken = getCurrentToken();
            if (currentToken.type == TOKEN_TYPE::SEPARATORS && currentToken.sValue == SEPARATORS::RBRACE){
                consumeToken();
                break;
            }
            Node* statementNode = parseStatement();
            compoundStatementNode->addChild(statementNode);
        }
        return compoundStatementNode;
    } else {
        throw std::runtime_error("Expected {");
    }
}
Node* Parser::parseSelectionStatenment(){
    TOKEN currentToken = getCurrentToken();
    if (currentToken.type == TOKEN_TYPE::KEYWORDS && currentToken.kValue == KEYWORDS::IF){
        consumeToken();
        currentToken = getCurrentToken();
        if (currentToken.type == TOKEN_TYPE::SEPARATORS && currentToken.sValue == SEPARATORS::LPAREN){
            consumeToken();
            Node* expressionNode = parseExpression();
            currentToken = getCurrentToken();
            if (currentToken.type == TOKEN_TYPE::SEPARATORS && currentToken.sValue == SEPARATORS::RPAREN){
                consumeToken();
                Node* statementNode = parseStatement();
                currentToken = getCurrentToken();
                if (currentToken.type == TOKEN_TYPE::KEYWORDS && currentToken.kValue == KEYWORDS::ELSE){
                    consumeToken();
                    Node* elseStatementNode = parseStatement();
                    Node* selectionStatementNode = new Node(NODE_TYPE::SELECTION_STATEMENT, currentToken);
                    selectionStatementNode->addChild(expressionNode);
                    selectionStatementNode->addChild(statementNode);
                    selectionStatementNode->addChild(elseStatementNode);
                    return selectionStatementNode;
                } else {
                    Node* selectionStatementNode = new Node(NODE_TYPE::SELECTION_STATEMENT, currentToken);
                    selectionStatementNode->addChild(expressionNode);
                    selectionStatementNode->addChild(statementNode);
                    return selectionStatementNode;
                }
            } else {
                throw std::runtime_error("Expected closing bracket");
            }
        } else {
            throw std::runtime_error("Expected opening bracket");
        }
    } else {
        throw std::runtime_error("Expected selection statement");
    }
}
Node* Parser::parseIterationStatement(){
    TOKEN currentToken = getCurrentToken();
    if (currentToken.type == TOKEN_TYPE::KEYWORDS && currentToken.kValue == KEYWORDS::WHILE){
        consumeToken();
        currentToken = getCurrentToken();
        if (currentToken.type == TOKEN_TYPE::SEPARATORS && currentToken.sValue == SEPARATORS::LPAREN){
            consumeToken();
            Node* expressionNode = parseExpression();
            currentToken = getCurrentToken();
            if (currentToken.type == TOKEN_TYPE::SEPARATORS && currentToken.sValue == SEPARATORS::RPAREN){
                consumeToken();
                Node* statementNode = parseStatement();
                Node* iterationStatementNode = new Node(NODE_TYPE::ITERATION_STATEMENT, currentToken);
                iterationStatementNode->addChild(expressionNode);
                iterationStatementNode->addChild(statementNode);
                return iterationStatementNode;
            } else {
                throw std::runtime_error("Expected closing bracket");
            }
        } else {
            throw std::runtime_error("Expected opening bracket");
        }
    } else if (currentToken.type == TOKEN_TYPE::KEYWORDS && currentToken.kValue == KEYWORDS::FOR){
        consumeToken();
        currentToken = getCurrentToken();
        if (currentToken.type == TOKEN_TYPE::SEPARATORS && currentToken.sValue == SEPARATORS::LPAREN){
            consumeToken();
            
            Node* expression1Node;
            currentToken = getCurrentToken();
            if (currentToken.type == TOKEN_TYPE::KEYWORDS && (currentToken.kValue == KEYWORDS::CHAR || currentToken.kValue == KEYWORDS::INT)){
                expression1Node = parseVariableDeclaration();
            } else {
                expression1Node = parseExpressionStatement();
            }
            Node* expression2Node = parseExpression();
            currentToken = getCurrentToken();
            if (currentToken.type == TOKEN_TYPE::SEPARATORS && currentToken.sValue == SEPARATORS::SEMICOLON){
                consumeToken();
            } else {
                throw std::runtime_error("Expected ;");
            }
            Node* expression3Node = parseExpression();
            currentToken = getCurrentToken();
            if (currentToken.type == TOKEN_TYPE::SEPARATORS && currentToken.sValue == SEPARATORS::RPAREN){
                consumeToken();
                Node* statementNode = parseStatement();
                Node* iterationStatementNode = new Node(NODE_TYPE::ITERATION_STATEMENT, currentToken);
                iterationStatementNode->addChild(expression1Node);
                iterationStatementNode->addChild(expression2Node);
                iterationStatementNode->addChild(expression3Node);
                iterationStatementNode->addChild(statementNode);
                return iterationStatementNode;
            } else {
                throw std::runtime_error("Expected closing bracket");
            }
        } else {
            throw std::runtime_error("Expected opening bracket");
        }
    } else {
        throw std::runtime_error("Expected iteration statement");
    }
}
Node* Parser::parseJumpStatement(){
    TOKEN currentToken = getCurrentToken();
    if (currentToken.type == TOKEN_TYPE::KEYWORDS){
        Node* jumpStatementNode;
        switch (currentToken.kValue){
            case KEYWORDS::RETURN:
                consumeToken();
                jumpStatementNode = new Node(NODE_TYPE::JUMP_STATEMENT, currentToken);
                jumpStatementNode->addChild(parseExpressionStatement());
                return jumpStatementNode;
            case KEYWORDS::BREAK:
                consumeToken();
                jumpStatementNode = new Node(NODE_TYPE::JUMP_STATEMENT, currentToken);
                currentToken = getCurrentToken();
                if (currentToken.type == TOKEN_TYPE::SEPARATORS && currentToken.sValue == SEPARATORS::SEMICOLON){
                    consumeToken();
                } else {
                    throw std::runtime_error("Expected ;");
                }
                return jumpStatementNode;
            default:
                throw std::runtime_error("Expected jump statement");
        }
    }else{
        throw std::runtime_error("Expected jump statement");
    }
}
Node* Parser::parseTypeSpecifier(){
    TOKEN currentToken = getCurrentToken();
    if (currentToken.type == TOKEN_TYPE::KEYWORDS && (currentToken.kValue == KEYWORDS::INT || currentToken.kValue == KEYWORDS::CHAR || currentToken.kValue == KEYWORDS::VOID)){
        consumeToken();
        Node* node = new Node(NODE_TYPE::TYPE, currentToken);
        while (true)
        {
            currentToken = getCurrentToken();
            if (currentToken.type == TOKEN_TYPE::OPERATORS && currentToken.oValue == OPERATORS::MULTIPLY){
                consumeToken();
                Node* pointerNode = new Node(NODE_TYPE::POINTER, currentToken);
                pointerNode->addChild(node);
                node = pointerNode;
            } else {
                break;
            }
        }
        return node;
    }else{
        std::runtime_error("Expected type specifier");
    }
}
Node* Parser::parseVariableDeclaration(){
    Node* typeSpecifierNode = parseTypeSpecifier();
    TOKEN currentToken = getCurrentToken();
    if (currentToken.type == TOKEN_TYPE::IDENTIFIER){
        consumeToken();
        Node* variableDeclarationNode = new Node(NODE_TYPE::VARIABLE_DECLARATION);

        Node* identifierNode = new Node(NODE_TYPE::IDENTIFIER, currentToken);
        currentToken = getCurrentToken();
        if (currentToken.type == TOKEN_TYPE::SEPARATORS && currentToken.sValue == SEPARATORS::LBRACKET){
            consumeToken();
            currentToken = getCurrentToken();
            if (currentToken.type == TOKEN_TYPE::CONSTANT){
                consumeToken();
                Node* arraySizeNode = new Node(NODE_TYPE::CONSTANT, currentToken);
                TOKEN closingBracket = getCurrentToken();
                if(closingBracket.type != TOKEN_TYPE::SEPARATORS && closingBracket.sValue != SEPARATORS::RBRACKET){
                    throw std::runtime_error("Expected closing bracket");
                }
                consumeToken();
                Node* arrayNode = new Node(NODE_TYPE::ARRAY);
                arrayNode->addChild(identifierNode);
                arrayNode->addChild(arraySizeNode);
                identifierNode = arrayNode;
            } else {
                throw std::runtime_error("Expected constant");
            }

        }

        //init
        currentToken = getCurrentToken();
        if (currentToken.type == TOKEN_TYPE::OPERATORS && currentToken.Value == "="){
            consumeToken();

            currentToken = getCurrentToken();
            if (currentToken.type == TOKEN_TYPE::SEPARATORS && currentToken.sValue == SEPARATORS::LBRACE){
                consumeToken();
                Node* expressionListNode = new Node(NODE_TYPE::EXPRESSION_LIST);

                Node* expressionNode = new Node(NODE_TYPE::EXPRESSION);
                Node* tempNode = parseAssignmentExpression();
                expressionNode->addChild(tempNode);
                expressionListNode->addChild(expressionNode);

                currentToken = getCurrentToken();
                while (currentToken.type == TOKEN_TYPE::SEPARATORS && currentToken.sValue == SEPARATORS::COMMA){
                    consumeToken();
                    expressionNode = new Node(NODE_TYPE::EXPRESSION);
                    Node* tempNode = parseAssignmentExpression();
                    expressionNode->addChild(tempNode);
                    expressionListNode->addChild(expressionNode);
                    currentToken = getCurrentToken();
                }

                currentToken = getCurrentToken();
                if (currentToken.type == TOKEN_TYPE::SEPARATORS && currentToken.sValue == SEPARATORS::RBRACE){
                    consumeToken();
                    variableDeclarationNode->addChild(expressionListNode);
                } else {
                    throw std::runtime_error("Expected closing brace");
                }
            }else{
                Node* expressionNode = parseExpression();
                variableDeclarationNode->addChild(expressionNode);
            }
        } 

        currentToken = getCurrentToken();
        if (currentToken.type == TOKEN_TYPE::SEPARATORS && currentToken.sValue == SEPARATORS::SEMICOLON){
            consumeToken();
        } else {
            throw std::runtime_error("Expected ;");
        }

        variableDeclarationNode->addChild(typeSpecifierNode);
        variableDeclarationNode->addChild(identifierNode);
        return variableDeclarationNode;
    } else {
        throw std::runtime_error("Expected identifier");
    }
}
Node* Parser::parseStatement(){
    TOKEN currentToken = getCurrentToken();
    if (currentToken.type == TOKEN_TYPE::KEYWORDS){
        switch (currentToken.kValue){
            case KEYWORDS::IF:
                return parseSelectionStatenment();
            case KEYWORDS::WHILE:
                return parseIterationStatement();
            case KEYWORDS::FOR:
                return parseIterationStatement();
            case KEYWORDS::RETURN:
                return parseJumpStatement();
            case KEYWORDS::BREAK:
                return parseJumpStatement();
            case KEYWORDS::INT:
                return parseVariableDeclaration();
            case KEYWORDS::CHAR:
                return parseVariableDeclaration();
            default:
                std::runtime_error("Expected statement");
        }
    }else if (currentToken.type == TOKEN_TYPE::SEPARATORS && currentToken.sValue == SEPARATORS::LBRACE){
        return parseCompoundStatement();
    }else{
        return parseExpressionStatement();
    }
}

Node* Parser::parseParameterList(){
    Node* typeSpecifierNode = parseTypeSpecifier();
    TOKEN currentToken = getCurrentToken();
    Node* identifierNode;
    if (currentToken.type == TOKEN_TYPE::IDENTIFIER){
        consumeToken();
        identifierNode = new Node(NODE_TYPE::IDENTIFIER, currentToken);
    } else {
        throw std::runtime_error("Expected identifier");
    }
    Node* paramter = new Node(NODE_TYPE::PARAMETER);
    paramter->addChild(typeSpecifierNode);
    paramter->addChild(identifierNode);

    Node* parameterListNode = new Node(NODE_TYPE::PARAMETER_LIST);
    parameterListNode->addChild(paramter);

    currentToken = getCurrentToken();
    while (currentToken.type == TOKEN_TYPE::SEPARATORS && currentToken.sValue == SEPARATORS::COMMA){
        consumeToken();
        typeSpecifierNode = parseTypeSpecifier();
        currentToken = getCurrentToken();
        if (currentToken.type == TOKEN_TYPE::IDENTIFIER){
            consumeToken();
            identifierNode = new Node(NODE_TYPE::IDENTIFIER, currentToken);
        } else {
            throw std::runtime_error("Expected identifier");
        }
        paramter = new Node(NODE_TYPE::PARAMETER);
        paramter->addChild(typeSpecifierNode);
        paramter->addChild(identifierNode);
        parameterListNode->addChild(paramter);
        currentToken = getCurrentToken();
    }
    return parameterListNode;
}
Node* Parser::parseFunctionDeclaration(){
    Node* typeSpecifierNode = parseTypeSpecifier();
    TOKEN currentToken = getCurrentToken();
    if (currentToken.type == TOKEN_TYPE::IDENTIFIER){
        consumeToken();
        Node* identifierNode = new Node(NODE_TYPE::IDENTIFIER, currentToken);
        currentToken = getCurrentToken();
        if (currentToken.type == TOKEN_TYPE::SEPARATORS && currentToken.sValue == SEPARATORS::LPAREN){
            consumeToken();
            currentToken = getCurrentToken();
            if (currentToken.type == TOKEN_TYPE::SEPARATORS && currentToken.sValue == SEPARATORS::RPAREN){
                consumeToken();
                Node* functionDeclarationNode = new Node(NODE_TYPE::FUNCTION_DECLARATION);
                Node* compoundStatementNode = parseCompoundStatement();
                functionDeclarationNode->addChild(typeSpecifierNode);
                functionDeclarationNode->addChild(identifierNode);
                functionDeclarationNode->addChild(compoundStatementNode);
                return functionDeclarationNode;
            } else {
                Node* parameterListNode = parseParameterList();
                currentToken = getCurrentToken();
                if (currentToken.type == TOKEN_TYPE::SEPARATORS && currentToken.sValue == SEPARATORS::RPAREN){
                    consumeToken();
                    Node* functionDeclarationNode = new Node(NODE_TYPE::FUNCTION_DECLARATION);
                    Node* compoundStatementNode = parseCompoundStatement();
                    functionDeclarationNode->addChild(typeSpecifierNode);
                    functionDeclarationNode->addChild(identifierNode);
                    functionDeclarationNode->addChild(parameterListNode);
                    functionDeclarationNode->addChild(compoundStatementNode);
                    return functionDeclarationNode;
                } else {
                    throw std::runtime_error("Expected closing bracket");
                }
            }
        } else {
            throw std::runtime_error("Expected opening bracket");
        }
    }else{
        throw std::runtime_error("Expected identifier");
    }

}

Node* Parser::parseProgram(){
    Node* programNode = new Node(NODE_TYPE::PROGRAM);
    while (tokens.getLength() > 0)
    {
        Node* functionDeclarationNode = parseFunctionDeclaration();
        programNode->addChild(functionDeclarationNode);
    }
    return programNode;
}
Node* Parser::parse(){
    Node* Node = parseProgram();
    return Node;
}

Parser::Parser(List::list<TOKEN> &&tokens)
{
    this->tokens = std::move(tokens);
}

Parser::~Parser()
{
}




