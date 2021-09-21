#ifndef PROBABILITY_ABSTRACT_DYNAMIC_TABLE_VIEW_DATA_SOURCE_H
#define PROBABILITY_ABSTRACT_DYNAMIC_TABLE_VIEW_DATA_SOURCE_H

#include <escher/highlight_cell.h>

namespace Probability {

/* This DataSource allocated its cells in a external buffer provided by the app. */

class DynamicTableViewDataSourceDelegate;

class DynamicTableViewDataSourceDestructor {
public:
  virtual void destroyCells() = 0;
};

template <typename T, int N>
class DynamicTableViewDataSource : public DynamicTableViewDataSourceDestructor {
public:
  DynamicTableViewDataSource(DynamicTableViewDataSourceDelegate * delegate) : m_cells(nullptr), m_delegate(delegate) {}
  Escher::HighlightCell * cell(int i);
  void destroyCells() override;
protected:
  void createCells();
  T * m_cells;
private:
  DynamicTableViewDataSourceDelegate * m_delegate;
};

class DynamicTableViewDataSourceDelegate {
public:
  virtual void initCell(void * cell) = 0;
};

}  // namespace Probability

#endif /* PROBABILITY_ABSTRACT_DYNAMIC_TABLE_VIEW_DATA_SOURCE_H */
