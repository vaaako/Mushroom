#include "../headers/types/node.hpp"

// Normally I would just let this in node.hpp, but I'm putting here to not get messy
// (since all header are in the same folder)

/**
 *  STATEMENTS */
Statement::Statement(const NodeType kind) : kind(kind) {}
Program::Program() : Statement(NodeType::Program) {}

VarDeclaration::VarDeclaration(const std::string& identifier, const ValueType valueType, const Expr* value, const bool constant) : Statement(NodeType::VarDeclaration), identifier(identifier), valueType(valueType), value(value), constant(constant) {}


/**
 *  EXPRESSIONS */
Expr::Expr(const NodeType kind) : Statement(kind) {}

AssigmentExpr::AssigmentExpr(const Expr* assigne, const Expr* value) : Expr(NodeType::AssigmentExpr), assigne(assigne), value(value) {}

BinaryExpr::BinaryExpr(const Expr* left, const Expr* right, const char& operatorChar)
	: Expr(NodeType::BinaryExpr), left(left), right(right), operatorChar(operatorChar) {}

UnaryExpr::UnaryExpr(const Expr* expr, const char& operatorChar) : Expr(NodeType::UnaryExpr), expr(expr), operatorChar(operatorChar) {}



/**
 *  LITERALS */
NumericLiteral::NumericLiteral(const double value)
	: Expr(NodeType::NumericLiteral), value(value) {}

Identifier::Identifier(const std::string& symbol)
	: Expr(NodeType::Identifier), symbol(symbol) {}


Property::Property(const std::string& key, const Expr* value) : Expr(NodeType::Property), key(key), value(value) {}

ObjectLiteral::ObjectLiteral() : Expr(NodeType::ObjectLiteral) {}
ObjectLiteral::ObjectLiteral(const std::vector<const Property*> properties) : Expr(NodeType::ObjectLiteral), properties(properties) {}
