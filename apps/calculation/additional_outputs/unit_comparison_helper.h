#ifndef CALCULATION_ADDITIONAL_OUTPUTS_UNIT_COMPARISON_HELPER_H
#define CALCULATION_ADDITIONAL_OUTPUTS_UNIT_COMPARISON_HELPER_H

#include <poincare/unit.h>
#include <apps/i18n.h>

namespace Calculation {

namespace UnitComparison {

constexpr static int k_sizeOfUnitComparisonBuffer = 5;
constexpr static int k_sizeOfUnitBuffer = 30;
constexpr static int k_numberOfSignicativeNumbers = 2;

typedef struct {
  I18n::Message title1;
  I18n::Message title2;
  I18n::Message subtitle;
  double value;
} ReferenceValue;

int SetUpperAndLowerReferenceValues(double inputValue, Poincare::Expression unit, const ReferenceValue ** referenceValues, Poincare::Expression * comparisonExpressions, bool saveComparison = true);

void FillRatioBuffer(double ratio, char * textBuffer);

}

}

#endif
