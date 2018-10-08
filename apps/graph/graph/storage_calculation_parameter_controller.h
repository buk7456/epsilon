#ifndef GRAPH_STORAGE_CALCULATION_PARAMETER_CONTROLLER_H
#define GRAPH_STORAGE_CALCULATION_PARAMETER_CONTROLLER_H

#include <escher.h>
#include "storage_tangent_graph_controller.h"
#include "extremum_graph_controller.h"
#include "integral_graph_controller.h"
#include "intersection_graph_controller.h"
#include "root_graph_controller.h"
#include "storage_graph_view.h"
#include "banner_view.h"
#include "../../shared/storage_cartesian_function.h"
#include "../../i18n.h"

namespace Graph {

class StorageCalculationParameterController : public ViewController, public SimpleListViewDataSource, public SelectableTableViewDataSource {
public:
  StorageCalculationParameterController(Responder * parentResponder, StorageGraphView * graphView, BannerView * bannerView, Shared::InteractiveCurveViewRange * range, Shared::CurveViewCursor * cursor, StorageCartesianFunctionStore * functionStore);
  View * view() override;
  const char * title() override;
  bool handleEvent(Ion::Events::Event event) override;
  void didBecomeFirstResponder() override;
  int numberOfRows() override;
  KDCoordinate cellHeight() override;
  HighlightCell * reusableCell(int index) override;
  int reusableCellCount() override;
  void willDisplayCellForIndex(HighlightCell * cell, int index) override;
  void setFunction(Shared::StorageCartesianFunction * function);
private:
  constexpr static int k_totalNumberOfCells = 6;
  MessageTableCell m_cells[k_totalNumberOfCells];
  SelectableTableView m_selectableTableView;
  Shared::StorageCartesianFunction * m_function;
  StorageTangentGraphController m_tangentGraphController;
  IntegralGraphController m_integralGraphController;
  MinimumGraphController m_minimumGraphController;
  MaximumGraphController m_maximumGraphController;
  RootGraphController m_rootGraphController;
  IntersectionGraphController m_intersectionGraphController;
};

}

#endif

