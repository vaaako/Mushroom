#pragma once

#include <memory>
#include <string>
#include <unordered_map>


enum class ValueType {
	Null,
	Number,
	Boolean,
	Object
};

// Base struct
struct RuntimeVal {
	const ValueType type;

	RuntimeVal(const ValueType type);
	virtual ~RuntimeVal() = default;  // This may not call the derived destructor (clone) if base destructor is not virtual (original)

	virtual RuntimeVal* clone() const {
		return new RuntimeVal(*this);
	}
};



struct NullVal : public RuntimeVal {
	const std::string value = "null";

	NullVal();

	NullVal* clone() const override {
		return new NullVal(*this);
	}
};

struct NumberVal : public RuntimeVal {
	const double value;

	NumberVal(const double value);

	NumberVal* clone() const override {
		return new NumberVal(*this);
	}
};

struct BooleanVal : public RuntimeVal {
	const bool value;

	BooleanVal(const bool value);

	BooleanVal* clone() const override {
		return new BooleanVal(*this);
	}
};


// struct ObjectVal : public RuntimeVal {
// 	std::unordered_map<std::string, const RuntimeVal*> properties;

// 	ObjectVal();
// 	~ObjectVal() override {
// 		for(auto pair : this->properties) {
// 			if(pair.second != nullptr)
// 				delete pair.second;
// 		}
// 	}

// 	ObjectVal* clone() const override {
// 		return new ObjectVal(*this);
// 	}

// 	inline void insert(const std::string& key, const RuntimeVal* value) {
// 		properties.emplace(key, value);
// 	}
// };

struct ObjectVal : public RuntimeVal {
	std::unordered_map<std::string, const std::shared_ptr<const RuntimeVal>> properties;

	ObjectVal();
	ObjectVal(std::unordered_map<std::string, const std::shared_ptr<const RuntimeVal>> properties);
	~ObjectVal() override = default;

	ObjectVal* clone() const override {
		return new ObjectVal(*this);
	}

	inline void insert(const std::string& key, const std::shared_ptr<const RuntimeVal> value) {
		properties.emplace(key, value);
	}
};


// struct ObjectVal : public RuntimeVal {
// 	std::unordered_map<std::string, const RuntimeVal&> properties;

// 	ObjectVal();
// 	~ObjectVal() override = default;

// 	ObjectVal* clone() const override {
// 		return new ObjectVal(*this);
// 	}

// 	inline void insert(const std::string& key, const RuntimeVal& value) {
// 		properties.emplace(key, value);
// 	}
// };