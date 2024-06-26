#ifndef CALCULATION_FUNCTION_LIST_CONTROLLER_H
#define CALCULATION_FUNCTION_LIST_CONTROLLER_H

#include <apps/shared/continuous_function_properties.h>
#include <ion/unicode/code_point.h>

#include "function_graph_cell.h"
#include "function_model.h"
#include "illustrated_expressions_list_controller.h"

namespace Calculation {

class FunctionListController : public IllustratedExpressionsListController {
 public:
  FunctionListController(EditExpressionController* editExpressionController)
      : IllustratedExpressionsListController(editExpressionController, true),
        m_graphCell(&m_model) {}
  void computeAdditionalResults(
      const Poincare::Expression input, const Poincare::Expression exactOutput,
      const Poincare::Expression approximateOutput) override;
  void viewDidDisappear() override;

 private:
  static constexpr CodePoint k_symbol =
      Shared::ContinuousFunctionProperties::k_cartesianSymbol;
  constexpr static int k_maxNumberOfOutputRows = 1;

  IllustrationCell* illustrationCell() override { return &m_graphCell; }
  I18n::Message messageAtIndex(int index) override;

  FunctionGraphCell m_graphCell;
  FunctionModel m_model;
};

}  // namespace Calculation

#endif
