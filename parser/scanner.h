#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace ptcc {
namespace parser {

enum class ExprType {
  Constant,
  Identifier,
  String,
  BinaryExpr,
};

enum class BinOp {
  Plus,
  Mult,
  Div,
};

enum class ConstantType {
  Int32,
  Int64,
  Uint32,
  Uint64,
  Float,
  Double,
  Enum,
};

struct Expression {
  ExprType m_type;
  std::string m_text;
  int m_token{0};
};

struct BinaryExpression : Expression {
  std::shared_ptr<Expression> m_exp1;
  std::shared_ptr<Expression> m_exp2;
  BinOp m_op;
};

struct Constant {
  ConstantType m_type;
  std::string m_text;
  int64_t m_ivalue;
  double m_dvalue;
};

enum class TypeKind {
  TypeDef,
  Struct,
  Union,
  Void,
  Int32,
  UInt32,
  Int64,
  UInt64,
  Float,
  Double,
  Char,
  Short,
  Bool,
};

struct TypeSpec {
  TypeKind m_kind;
};

struct FieldDecl {
  TypeSpec m_type;
  std::string m_name;
};

struct StructDecl {
  std::string m_name;
  std::vector<FieldDecl> m_members;
};

struct StructDeclList {
  std::vector<std::string> m_names;
};

struct FieldDeclInProgress {
  TypeSpec m_type;
  std::vector<std::string> m_names;
};

struct Token {
  int m_token{0};
  std::string m_text;

  std::shared_ptr<Expression> m_expr;
  std::shared_ptr<Constant> m_constant;
  std::shared_ptr<TypeSpec> m_type;
  std::shared_ptr<StructDecl> m_struct;
  std::shared_ptr<FieldDeclInProgress> m_fieldDecl;
};
}
} // ptc::scanner
