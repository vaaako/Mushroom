#pragma once

#include "values.hpp"
#include <string>
#include <vector>

enum class NodeType {
	// Statements
	Program,
	VarDeclaration,

	// Expressions
	AssigmentExpr,
	BinaryExpr,
	UnaryExpr,

	// Literals
	NumericLiteral,
	Identifier,
	Property,
	ObjectLiteral
};


// Stmt is the base struct for statements
struct Statement {
	const NodeType kind;

	Statement(const NodeType kind);

	
	virtual ~Statement() = default; /** * This will prevent memory leak when deleting a Statement
	* Ex.: BinaryExpr overrides this destructor, so when deleting it will use BinaryExpr destructor
	* instead of this destructor (default behavior) and no memory leaks will occur */

	inline NodeType get_kind() {
		return this->kind;
	}
};


// Program represents a block of statements
struct Program : public Statement {
	std::vector<const Statement*> body;

	Program();
	~Program() {
		for(const Statement* stmt : this->body) {
			delete stmt;
		}
	}

	inline void insert(const Statement* stmt) {
		body.emplace_back(stmt);
	}
};





/**
 *  EXPRESSIONS */

// Expr is the base struct for expressions
struct Expr : public Statement {
	// No need to each node type to have a kind
	Expr(const NodeType kind);
};


// This is a Statement, but I need Expr
struct VarDeclaration : public Statement {
	const std::string identifier;
	const ValueType valueType;
	const Expr* value;
	const bool constant;

	VarDeclaration(const std::string& identifier, const ValueType valueType, const Expr* value, const bool constant);
	~VarDeclaration() override {
		delete this->value;
	}
};


struct AssigmentExpr : public Expr {
	const Expr* assigne; // Expr* and not a simple string because of objects (e.g. foo.bar = "new value")
	const Expr* value;

	AssigmentExpr(const Expr* assigne, const Expr* value);
	~AssigmentExpr() override {
		delete assigne;
		delete value;
	}
};


// BinaryExpr represents a binary operation (Ex.: 1 + 1, 1 + (2 + 2))
struct BinaryExpr : public Expr {
	const Expr* left;
	const Expr* right;
	const char operatorChar; // Operator is a keyword in C++

	BinaryExpr(const Expr* left, const Expr* right, const char& operatorChar);
	~BinaryExpr() override {
		delete this->left;
		delete this->right;
	}
};

struct UnaryExpr : public Expr {
	const Expr* expr;
	const char operatorChar;

	UnaryExpr(const Expr* expr, const char& operatorChar);
	~UnaryExpr() override {
		delete this->expr;
	}
};



/**
 *  LITERALS */

// NumericLiteral represents a numeric constant
struct NumericLiteral : public Expr {
	const double value;

	NumericLiteral(const double value);
};

// Identifier represents a user-defined variable or symbol
struct Identifier : public Expr {
	const std::string symbol;

	Identifier(const std::string& symbol);
};


struct Property : public Expr {
	const std::string key;
	const Expr* value;

	Property(const std::string& key, const Expr* value);
	~Property() override {
		if(value != nullptr) {
			delete this->value;
		}
	}
};

struct ObjectLiteral : public Expr {
	std::vector<const Property*> properties;

	ObjectLiteral();
	ObjectLiteral(const std::vector<const Property*> properties);
	~ObjectLiteral() override {
		for(const Property* property : this->properties) {
			delete property;
		}
	}

	inline void insert(const Property* property) {
		this->properties.emplace_back(property);
	}
};

