enum class BuiltinType {
  Char,
  UChar,
  Int16,
  UInt16,
  Int32,
  UInt32,
  Int64,
  UInt64,
  Float,
  Double,
};

enum class TypeKind {
  Builtin,
  Struct,
  Union,
};

struct InternalType {
  TypeKind m_kind;
  BuiltinType m_btype;
};
