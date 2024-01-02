#ifndef ESCHER_EDITABLE_FUNCTION_CELL_H
#define ESCHER_EDITABLE_FUNCTION_CELL_H

#include <escher/button_cell.h>
#include <escher/editable_expression_model_cell.h>

#include "function_cell.h"

namespace Graph {

class EditableFunctionCell
    : public TemplatedFunctionCell<Shared::WithEditableExpressionCell> {
 public:
  EditableFunctionCell(Escher::Responder* parentResponder,
                       Escher::LayoutFieldDelegate* layoutFieldDelegate);

  void updateSubviewsBackgroundAfterChangingState() override {
    m_expressionBackground = backgroundColor();
  }

  bool isEmpty() { return expressionCell()->layoutField()->isEmpty(); }

  void updateButton() {
    layoutSubviews();
    markRectAsDirty(m_buttonCell.bounds());
    markRectAsDirty(expressionCell()->layoutField()->bounds());
  }

 private:
  int numberOfSubviews() const override { return 3; }
  void layoutSubviews(bool force = false) override;
  Escher::View* subviewAtIndex(int index) override;

  static constexpr KDCoordinate k_expressionMargin = 5;
  static constexpr KDCoordinate k_templateButtonMargin = 5;
  Escher::ButtonCell m_buttonCell;
};

}  // namespace Graph

#endif
