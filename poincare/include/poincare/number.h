#ifndef POINCARE_NUMBER_H
#define POINCARE_NUMBER_H

#include <poincare/expression.h>

namespace Poincare {

/* Number class has 3 subclasses:
 * - Undefined
 * - Integer
 * - Rational
 * - Float
 * - Decimal
 * - Infinity
 */

class Integer;
class Rational;

class NumberNode : public ExpressionNode {
public:
  bool isNumber() const override { return true; }
  int numberOfChildren() const override { return 0; }

  double doubleApproximation() const;
};

class Number : public Expression {
public:
  using Expression::Expression;
  NumberNode * numberNode() const { assert(!isAllocationFailure()); return static_cast<NumberNode *>(node()); }
  /* Return either a Integer, a Decimal or an Infinity. */
  static Number ParseInteger(const char * digits, size_t length, bool negative);
  /* Return either a DecimalInteger or an Infinity. */
  template <typename T> static Number Decimal(T f);
  /* This set of Functions return either a Rational or a Float
   * or Infinity in case of overflow. Decimal are not taken into
   * account as it is not an internal node - it will always be turned into a
   * Rational/Float beforehand. */
  static Number Addition(const Number i, const Number j);
  static Number Multiplication(const Number i, const Number j);
  static Number Power(const Number i, const Number j);
private:
  typedef Integer (*IntegerBinaryOperation)(const Integer i, const Integer j);
  typedef Rational (*RationalBinaryOperation)(const Rational i, const Rational j);
  typedef double (*DoubleBinaryOperation)(double i, double j);
  static Number BinaryOperation(const Number i, const Number j, IntegerBinaryOperation integerOp, RationalBinaryOperation rationalOp, DoubleBinaryOperation doubleOp);
};

}

#endif
