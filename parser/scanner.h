#pragma once

#include <cstdint>
#include <string>

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

struct Token {
  int m_token{0};
  std::string m_text;

  std::shared_ptr<Expression> m_expr;
  std::shared_ptr<Constant> m_constant;
};
}
} // ptc::scanner
