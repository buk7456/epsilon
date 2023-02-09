#ifndef SHARED_EXPRESSION_MODEL_LIST_CONTROLLER_H
#define SHARED_EXPRESSION_MODEL_LIST_CONTROLLER_H

#include <apps/i18n.h>
#include <escher/even_odd_message_text_cell.h>
#include <escher/input_view_controller.h>
#include <escher/selectable_table_view_data_source.h>
#include <escher/selectable_table_view_delegate.h>
#include <escher/expression_field.h>
#include "layout_field_delegate.h"
#include "expression_model_store.h"

namespace Shared {

class ExpressionModelListController : public Escher::ViewController, public Escher::SelectableTableViewDataSource, public Escher::SelectableTableViewDelegate, public LayoutFieldDelegate {
public:
  ExpressionModelListController(Escher::Responder * parentResponder, I18n::Message text);
  virtual void editExpression(Ion::Events::Event event);
  virtual bool editSelectedRecordWithText(const char * text);
  virtual void getTextForSelectedRecord(char * text, size_t size);
  bool handleEventOnExpression(Ion::Events::Event event, bool inTemplateMenu = false);
  bool handleEventOnExpressionInTemplateMenu(Ion::Events::Event event) { return handleEventOnExpression(event, true); }
protected:
  constexpr static KDFont::Size k_font = KDFont::Size::Large;
  constexpr static KDCoordinate k_expressionMargin = 5;
  // TableViewDataSource
  virtual int numberOfExpressionRows() const;
  bool isAddEmptyRow(int j) const;
  static KDCoordinate ExpressionRowHeightFromLayoutHeight(KDCoordinate height);
  virtual KDCoordinate expressionRowHeight(int j);
  virtual void willDisplayExpressionCellAtIndex(Escher::HighlightCell * cell, int j);
  // Responder
  virtual void addModel();
  virtual void didChangeModelsList() { resetSizesMemoization(); }
  virtual bool removeModelRow(Ion::Storage::Record record);
  virtual int modelIndexForRow(int j) { return j; }
  // ViewController
  virtual Escher::SelectableTableView * selectableTableView() = 0;
  virtual ExpressionModelStore * modelStore() const = 0;
  Escher::EvenOddMessageTextCell m_addNewModel;
  // LayoutDelegate
  bool layoutFieldDidFinishEditing(Escher::LayoutField * layoutField, Poincare::Layout layout, Ion::Events::Event event) override;
  void layoutFieldDidChangeSize(Escher::LayoutField * layoutField) override;
  bool layoutFieldDidAbortEditing(Escher::LayoutField * layoutField) override;
  // Memoization
  virtual void resetSizesMemoization() {}
  // EditableCell
  virtual Escher::ExpressionField * expressionField() = 0;
  int16_t m_editedCellIndex;
private:
  bool addEmptyModel();

};

}

#endif
