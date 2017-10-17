#include <poincare/factorial.h>
#include "layout/string_layout.h"
#include "layout/horizontal_layout.h"
extern "C" {
#include <assert.h>
}
#include <cmath>

namespace Poincare {

Factorial::Factorial(const Expression * argument, bool clone) :
  StaticHierarchy<1>(&argument, clone)
{
}

Expression::Type Factorial::type() const {
  return Type::Factorial;
}

Expression * Factorial::clone() const {
  Factorial * a = new Factorial(m_operands[0], true);
  return a;
}

template<typename T>
Complex<T> Factorial::computeOnComplex(const Complex<T> c, AngleUnit angleUnit) {
  T n = c.a();
  if (c.b() != 0 || isnan(n) || n != (int)n || n < 0) {
    return Complex<T>::Float(NAN);
  }
  T result = 1;
  for (int i = 1; i <= (int)n; i++) {
    result *= (T)i;
    if (isinf(result)) {
      return Complex<T>::Float(result);
    }
  }
  return Complex<T>::Float(std::round(result));
}

ExpressionLayout * Factorial::privateCreateLayout(FloatDisplayMode floatDisplayMode, ComplexFormat complexFormat) const {
  assert(floatDisplayMode != FloatDisplayMode::Default);
  assert(complexFormat != ComplexFormat::Default);
  ExpressionLayout * childrenLayouts[2];
  childrenLayouts[0] = operand(0)->createLayout(floatDisplayMode, complexFormat);
  childrenLayouts[1] = new StringLayout("!", 1);
  return new HorizontalLayout(childrenLayouts, 2);
}

int Factorial::compareToGreaterTypeExpression(const Expression * e) const {
  if (operand(0)->compareTo(e) == 0) {
    return 1;
  }
  return operand(0)->compareTo(e);
}

int Factorial::writeTextInBuffer(char * buffer, int bufferSize) const {
  if (bufferSize == 0) {
    return -1;
  }
  buffer[bufferSize-1] = 0;
  int numberOfChar = operand(0)->writeTextInBuffer(buffer, bufferSize);
  if (numberOfChar >= bufferSize-1) {
    return numberOfChar;
  }
  buffer[numberOfChar++] = '!';
  buffer[numberOfChar] = 0;
  return numberOfChar;
}

int Factorial::compareToSameTypeExpression(const Expression * e) const {
  return operand(0)->compareTo(e->operand(0));
}

}
