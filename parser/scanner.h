#pragma once

#include <cstdint>
#include <memory>
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
  Array,
  Bool,
  Char,
  Double,
  Enum,
  Float,
  Int,
  Long,
  Pointer,
  Short,
  Signed,
  Struct,
  TypeDef,
  Union,
  Undecided,
  Unsigned,
  Void,
};

enum class TypeQual {
  Const,
  Restrict,
  Volatile,
  Atomic,
};

std::string tyQualToString(const TypeQual tyQual);

// Forward declare
struct StructUnionDecl;

struct TypeSpec {
  TypeKind m_kind;
  std::vector<TypeQual> m_quals;
  std::shared_ptr<TypeSpec> m_otype;
  std::shared_ptr<StructUnionDecl> m_struct;
  std::string m_typedefName;
};

std::string kindToString(const TypeKind kind);
std::string specToString(const TypeSpec typeSpec);
std::string structTypeToString(const TypeSpec typeSpec);
std::string unionTypeToString(const TypeSpec typeSpec);

struct FieldDecl {
  TypeSpec m_type;
  std::string m_name;
};

enum class StorageClassSpecifier {
  Typedef,
  Extern,
  Static,
  ThreadLocal,
  Auto,
  Register,
};

struct DeclarationSpecifiers {
  std::vector<StorageClassSpecifier> m_storageClassSpecs;
  std::vector<TypeSpec> m_typeSpecs;
};

struct StructUnionDecl {
  StructUnionDecl(std::string name, std::vector<FieldDecl> members)
      : m_name(name), m_members(members) {}

  std::string m_name;
  std::vector<FieldDecl> m_members;
};

struct StructDeclList {
  std::vector<std::string> m_names;
};

struct ParameterDecl {
  std::string m_id;
  std::shared_ptr<DeclarationSpecifiers> m_declSpecs;
  std::shared_ptr<TypeSpec> m_type;
};

struct Token {
  int m_token{0};
  std::string m_text;
  std::string m_id;

  std::shared_ptr<Expression> m_expr;
  std::shared_ptr<Constant> m_constant;
  std::shared_ptr<TypeSpec> m_type;
  std::shared_ptr<StructUnionDecl> m_struct;
  std::shared_ptr<DeclarationSpecifiers> m_declSpecs;
  std::vector<FieldDecl> m_fieldDecls;
  std::vector<TypeQual> m_typeQuals;
  std::vector<ParameterDecl> m_paramDecls;
  std::vector<Token> m_structDeclList;
};

std::string toString(const Token tok);
std::string toString(const ExprType exprType);
std::string toString(const Expression expr);
std::string toString(const TypeSpec typeSpec);
std::string toString(const FieldDecl fieldDecl);
std::string toString(const StructUnionDecl structUnionDecl);
std::string toString(const ConstantType constType);
std::string toString(const Constant constant);
std::string toString(const DeclarationSpecifiers declSpecs);
std::string toString(const StorageClassSpecifier storageClassSpec);
std::string toString(const TypeQual tyQual);
std::string toString(const ParameterDecl paramDecl);

template <typename T> std::string toString(const std::shared_ptr<T> token);
template <typename T> std::string toString(const std::vector<T> vec);

std::string pointerTypeToString(const TypeSpec tSpec);
}
} // ptc::scanner
