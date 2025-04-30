#pragma once
#include "lexer.hpp"

#include <vector>

// Forward Declarations
struct ArrayLiteral;
struct BooleanLiteral;
struct CallExpression;
struct DoExpression;
struct Expression;
struct FloatLiteral;
struct ForExpression;
struct FunctionLiteral;
struct HashLiteral;
struct IdentifierLiteral;
struct IfExpression;
struct IndexExpression;
struct InfixExpression;
struct IntegerLiteral;
struct PostfixExpression;
struct PrefixExpression;
struct StringLiteral;
struct WhileExpression;

struct AssignmentExpressionStatement;
struct BlockStatement;
struct ExpressionStatement;
struct FunctionStatement;
struct IdentifierStatement;
struct LetStatement;
struct ReturnStatement;
struct Statement;

class Parser {
  public:
    Parser(std::string&);
    ~Parser() { this->errors.clear(); };

    Token currentToken;
    Token peekToken;
    int linenumber{1};
    std::vector<std::string> errors;

    bool expectPeek(TokenType);
    void nextToken();
    std::shared_ptr<Statement> parseStatement();
    void peekErrors(TokenType);

  private:
    std::shared_ptr<Lexer> lexer;

    void checkIdentifierDataType(std::shared_ptr<IdentifierStatement>);
    void checkFunctionReturn(std::shared_ptr<FunctionStatement>);
    void checkFunctionReturnDataType(std::shared_ptr<ReturnStatement>);
    int currentPrecedence();
    int peekPrecedence();

    std::shared_ptr<ArrayLiteral> parseArrayLiteral();
    std::shared_ptr<AssignmentExpressionStatement> parseAssignmentExpression();
    std::shared_ptr<BooleanLiteral> parseBooleanLiteral();
    std::shared_ptr<CallExpression> parseCallExpression(std::shared_ptr<Expression>);
    std::shared_ptr<DoExpression> parseDoExpression();
    std::shared_ptr<Expression> parseExpression(int);
    std::vector<std::shared_ptr<Expression>> parseExpressionList(TokenType);
    std::shared_ptr<FloatLiteral> parseFloatLiteral();
    std::shared_ptr<ForExpression> parseForExpression();
    std::shared_ptr<LetStatement> parseForLetStatement();
    std::shared_ptr<FunctionLiteral> parseFunctionLiteral();
    std::vector<std::shared_ptr<IdentifierLiteral>> parseFunctionParameters();
    std::shared_ptr<Expression> parseGroupedExpression();
    std::shared_ptr<HashLiteral> parseHashLiteral();
    std::shared_ptr<IdentifierLiteral> parseIdentifier();
    std::shared_ptr<IfExpression> parseIfExpression();
    std::shared_ptr<Expression> parseIndexExpression(std::shared_ptr<Expression>);
    std::shared_ptr<InfixExpression> parseInfixExpression(std::shared_ptr<Expression>);
    std::shared_ptr<IntegerLiteral> parseIntegerLiteral();
    std::shared_ptr<Expression> parseLeftPrefix(int);
    std::shared_ptr<PostfixExpression> parsePostfixExpression(std::shared_ptr<Expression>);
    std::shared_ptr<PrefixExpression> parsePrefixExpression();
    std::shared_ptr<StringLiteral> parseStringLiteral();
    std::shared_ptr<WhileExpression> parseWhileExpression();

    std::shared_ptr<BlockStatement> parseBlockStatement();
    std::shared_ptr<ExpressionStatement> parseExpressionStatement();
    std::shared_ptr<FunctionStatement> parseFunctionStatement();
    std::shared_ptr<IdentifierStatement> parseIdentifierStatement();
    std::shared_ptr<LetStatement> parseLetStatement();
    std::shared_ptr<ReturnStatement> parseReturnStatement();
};

// Prefix Functions
enum prefix {
    PREFIX_STD,
    PREFIX_IDENT,
    PREFIX_INT,
    PREFIX_FLOAT,
    PREFIX_STRING,
    PREFIX_BOOL,
    PREFIX_IF,
    PREFIX_FUNCTION,
    PREFIX_DO,
    PREFIX_WHILE,
    PREFIX_FOR,
    PREFIX_INCREMENT,
    PREFIX_DECREMENT,
    PREFIX_GROUPED_EXPR,
    PREFIX_ASSIGN,
    PREFIX_ARRAY,
    PREFIX_HASH,
};

const std::unordered_map<TokenType, int> prefixFunctions = {
    {::IDENT,    PREFIX_IDENT       },
    {::INT,      PREFIX_INT         },
    {::FLOAT,    PREFIX_FLOAT       },
    {::STRING,   PREFIX_STRING      },
    {::BANG,     PREFIX_STD         },
    {::MINUS,    PREFIX_STD         },
    {::TRUE,     PREFIX_BOOL        },
    {::FALSE,    PREFIX_BOOL        },
    {::LPAREN,   PREFIX_GROUPED_EXPR},
    {::IF,       PREFIX_IF          },
    {::FUNCTION, PREFIX_FUNCTION    },
    {::DO,       PREFIX_DO          },
    {::WHILE,    PREFIX_WHILE       },
    {::FOR,      PREFIX_FOR         },
    {::PLUS_EQ,  PREFIX_ASSIGN      },
    {::MINUS_EQ, PREFIX_ASSIGN      },
    {::MULT_EQ,  PREFIX_ASSIGN      },
    {::DIV_EQ,   PREFIX_ASSIGN      },
    {::LBRACKET, PREFIX_ARRAY       },
    {::LBRACE,   PREFIX_HASH        },
};

// Infix Functions
enum infix {
    INFIX_STD,
    INFIX_CALL,
    INFIX_INDEX,
};

const std::unordered_map<TokenType, int> infixFunctions = {
    {::PLUS,     INFIX_STD  },
    {::MINUS,    INFIX_STD  },
    {::SLASH,    INFIX_STD  },
    {::ASTERISK, INFIX_STD  },
    {::EQ,       INFIX_STD  },
    {::NOT_EQ,   INFIX_STD  },
    {::LT,       INFIX_STD  },
    {::GT,       INFIX_STD  },
    {::LPAREN,   INFIX_CALL },
    {::LBRACKET, INFIX_INDEX},
};

enum postfix { POSTFIX };

const std::unordered_map<TokenType, int> postfixFunctions = {
    {::INCREMENT, POSTFIX},
    {::DECREMENT, POSTFIX},
};
