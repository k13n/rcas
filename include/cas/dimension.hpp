#ifndef CAS_DIMENSION_H_
#define CAS_DIMENSION_H_


namespace cas {

enum class Dimension {
  Path,
  Value,
  Leaf,
};

Dimension& operator!(Dimension& d);

} // namespace cas

#endif // CAS_DIMENSION_H_
