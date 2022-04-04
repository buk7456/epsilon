#include "multiple_histograms_view.h"
#include <assert.h>

namespace Statistics {

MultipleHistogramsView::MultipleHistogramsView(Store * store, Shared::CurveViewRange * curveViewRange) :
  MultipleDataView(store),
  m_histogramView1(store, 0, curveViewRange, Shared::DoublePairStore::colorOfSeriesAtIndex(0)),
  m_histogramView2(store, 1, curveViewRange, Shared::DoublePairStore::colorOfSeriesAtIndex(1)),
  m_histogramView3(store, 2, curveViewRange, Shared::DoublePairStore::colorOfSeriesAtIndex(2))
{
  for (int i = 0; i < Store::k_numberOfSeries; i++) {
    HistogramView * histView = dataViewAtIndex(i);
    histView->setDisplayLabels(false);
  }
}

HistogramView *  MultipleHistogramsView::dataViewAtIndex(int index) {
  assert(index >= 0 && index < 3);
  HistogramView * views[] = {&m_histogramView1, &m_histogramView2, &m_histogramView3};
  return views[index];
}

int MultipleHistogramsView::seriesOfSubviewAtIndex(int index) {
  assert(index >= 0 && index < numberOfSubviews() - 1);
  return static_cast<HistogramView *>(subviewAtIndex(index))->series();
}

void MultipleHistogramsView::layoutSubviews(bool force) {
  MultipleDataView::layoutSubviews();
  assert(m_store->numberOfValidSeries() > 0);
  int displayedSubviewIndex = 0;
  for (int i = 0; i < Store::k_numberOfSeries; i++) {
    if (m_store->seriesIsValid(i)) {
      displayedSubviewIndex++;
    }
  }
}

void MultipleHistogramsView::changeDataViewSelection(int index, bool select) {
  MultipleDataView::changeDataViewSelection(index, select);
  dataViewAtIndex(index)->setDisplayLabels(select);
  if (select == false) {
    // Set the hightlight to default selected bar to prevent blinking
    dataViewAtIndex(index)->setHighlight(m_store->startOfBarAtIndex(index, k_defaultSelectedIndex), m_store->endOfBarAtIndex(index, k_defaultSelectedIndex));
  }
}

}
