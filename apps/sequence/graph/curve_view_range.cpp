#include "curve_view_range.h"
#include "graph_controller.h"
#include <poincare/preferences.h>
#include <ion.h>
#include <algorithm>
#include <cmath>

using namespace Shared;
using namespace Poincare;

namespace Sequence {

CurveViewRange::CurveViewRange(GraphController * delegate) :
  InteractiveCurveViewRange(delegate)
{}

void CurveViewRange::protectedNormalize(bool canChangeX, bool canChangeY, bool canShrink) {
  assert(m_delegate);
  Shared::InteractiveCurveViewRange::protectedNormalize(canChangeX, canChangeY, canShrink);

  /* The X axis is not supposed to go into the negatives, save for a small
   * margin. However, after normalizing, it could be the case. We thus shift
   * the X range rightward to the origin. */
  GraphController * delegate = static_cast<GraphController *>(m_delegate);
  float interestingXMin = delegate->interestingXMin();
  float xRange = xMax() - xMin();
  MemoizedCurveViewRange::protectedSetXMin(interestingXMin - k_displayLeftMarginRatio * xRange, false, INFINITY);
  MemoizedCurveViewRange::protectedSetXMax(xMin() + xRange, true, k_maxFloat);
}

}
