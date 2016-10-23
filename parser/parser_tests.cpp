#include "parser/scanner.h"
#include "parser/parser.h"
#include "gtest/gtest.h"

namespace ptcc {
namespace parser {

TEST(ToStringRepr, PointerToConstChar) {
  TypeSpec ptrToConstChar;
  ptrToConstChar.m_kind = TypeKind::Pointer;
  ptrToConstChar.m_otype = std::make_shared<TypeSpec>();
  ptrToConstChar.m_otype->m_kind = TypeKind::Char;
  ptrToConstChar.m_otype->m_quals.push_back(TypeQual::Const);
  EXPECT_EQ("const char*", pointerTypeToString(ptrToConstChar));
}

TEST(ToStringRepr, ConstPointerToConstChar) {
  TypeSpec constPtrToConstChar;
  constPtrToConstChar.m_kind = TypeKind::Pointer;
  constPtrToConstChar.m_quals.push_back(TypeQual::Const);
  constPtrToConstChar.m_otype = std::make_shared<TypeSpec>();
  constPtrToConstChar.m_otype->m_kind = TypeKind::Char;
  constPtrToConstChar.m_otype->m_quals.push_back(TypeQual::Const);
  EXPECT_EQ("const char* const", pointerTypeToString(constPtrToConstChar));
}

TEST(ToStringRepr, PointerToPointerToConstChar) {
  TypeSpec constCharTy;
  constCharTy.m_kind = TypeKind::Char;
  constCharTy.m_quals.push_back(TypeQual::Const);

  TypeSpec ptrToChar;
  ptrToChar.m_kind = TypeKind::Pointer;
  ptrToChar.m_otype = std::make_shared<TypeSpec>(constCharTy);

  TypeSpec ptrToPtr;
  ptrToPtr.m_kind = TypeKind::Pointer;
  ptrToPtr.m_otype = std::make_shared<TypeSpec>(ptrToChar);

  EXPECT_EQ("const char**", pointerTypeToString(ptrToPtr));
  EXPECT_EQ("const char*", pointerTypeToString(ptrToChar));
}

TEST(ToStringRepr, PointerToConstPointerToConstChar) {
  TypeSpec constCharTy;
  constCharTy.m_kind = TypeKind::Char;
  constCharTy.m_quals.push_back(TypeQual::Const);

  TypeSpec constPtrToChar;
  constPtrToChar.m_kind = TypeKind::Pointer;
  constPtrToChar.m_quals.push_back(TypeQual::Const);
  constPtrToChar.m_otype = std::make_shared<TypeSpec>(constCharTy);

  TypeSpec ptrToPtr;
  ptrToPtr.m_kind = TypeKind::Pointer;
  ptrToPtr.m_otype = std::make_shared<TypeSpec>(constPtrToChar);

  EXPECT_EQ("const char* const*", pointerTypeToString(ptrToPtr));
  EXPECT_EQ("const char* const", pointerTypeToString(constPtrToChar));
}

TEST(SetPtrTo, SetPtrToChar) {
  TypeSpec charTy;
  charTy.m_kind = TypeKind::Char;

  TypeSpec ptrToChar;
  ptrToChar.m_kind = TypeKind::Pointer;

  setPtrTo(&ptrToChar, charTy);
  EXPECT_EQ("char*", pointerTypeToString(ptrToChar));
}
}
}
