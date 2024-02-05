#include "../headers/types/values.hpp"

// Normally I would just let this in node.hpp, but I'm putting here to not get messy
// (since all header are in the same folder)

RuntimeVal::RuntimeVal(const ValueType type) : type(type) {}

NullVal::NullVal() : RuntimeVal(ValueType::Null) {}

NumberVal::NumberVal(const double value) : RuntimeVal(ValueType::Number), value(value) {}

BooleanVal::BooleanVal(const bool value) : RuntimeVal(ValueType::Boolean), value(value) {}

ObjectVal::ObjectVal() : RuntimeVal(ValueType::Object) {}
ObjectVal::ObjectVal(std::unordered_map<std::string, const std::shared_ptr<const RuntimeVal>> properties) : RuntimeVal(ValueType::Object), properties(properties) {}

