#include "histogram_parameter_controller.h"
#include "app.h"
#include <assert.h>

using namespace Shared;

namespace Statistics {

HistogramParameterController::HistogramParameterController(Responder * parentResponder, Store * store) :
  FloatParameterController(parentResponder),
  m_store(store)
{
}

const char * HistogramParameterController::title() {
  return I18n::translate(I18n::Message::HistogramSet);
}

int HistogramParameterController::numberOfRows() {
  return 1+k_numberOfCells;
}

void HistogramParameterController::willDisplayCellForIndex(HighlightCell * cell, int index) {
  if (index == numberOfRows()-1) {
    return;
  }
  MessageTableCellWithEditableText * myCell = (MessageTableCellWithEditableText *)cell;
  I18n::Message labels[k_numberOfCells] = {I18n::Message::RectangleWidth, I18n::Message::BarStart};
  myCell->setMessage(labels[index]);
  FloatParameterController::willDisplayCellForIndex(cell, index);
}

void HistogramParameterController::unloadView() {
  for (int i = 0; i < k_numberOfCells; i++) {
    assert(m_cells[i] != nullptr);
    delete m_cells[i];
    m_cells[i] = nullptr;
  }
  FloatParameterController::unloadView();
}

float HistogramParameterController::parameterAtIndex(int index) {
  assert(index >= 0 && index < k_numberOfCells);
  if (index == 0) {
    return m_store->barWidth();
  }
  return m_store->firstDrawnBarAbscissa();
}

bool HistogramParameterController::setParameterAtIndex(int parameterIndex, float f) {
  assert(parameterIndex >= 0 && parameterIndex < k_numberOfCells);
  if (parameterIndex == 0) {
    if (f <= 0.0f) {
      app()->displayWarning(I18n::Message::ForbiddenValue);
      return false;
    }
    m_store->setBarWidth(f);
  } else {
    m_store->setFirstDrawnBarAbscissa(f);
  }
  return true;
}

HighlightCell * HistogramParameterController::reusableParameterCell(int index, int type) {
  assert(index >= 0 && index < k_numberOfCells);
  return m_cells[index];
}

int HistogramParameterController::reusableParameterCellCount(int type) {
  return k_numberOfCells;
}

View * HistogramParameterController::createView() {
  SelectableTableView * tableView = (SelectableTableView *)FloatParameterController::createView();
  for (int i = 0; i < k_numberOfCells; i++) {
    assert(m_cells[i] == nullptr);
    m_cells[i] = new MessageTableCellWithEditableText(tableView, this, m_draftTextBuffer, I18n::Message::Default);
  }
  return tableView;
}

}

