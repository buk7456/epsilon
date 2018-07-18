#ifndef POINCARE_FLOOR_LAYOUT_NODE_H
#define POINCARE_FLOOR_LAYOUT_NODE_H

#include <poincare/bracket_pair_layout_node.h>
#include <poincare/layout_engine.h>

namespace Poincare {

class FloorLayoutNode : public BracketPairLayoutNode {
public:
  using BracketPairLayoutNode::BracketPairLayoutNode;
  int writeTextInBuffer(char * buffer, int bufferSize, int numberOfSignificantDigits = PrintFloat::k_numberOfStoredSignificantDigits) const override {
    return LayoutEngine::writePrefixSerializableRefTextInBuffer(SerializableRef(const_cast<FloorLayoutNode *>(this)), buffer, bufferSize, numberOfSignificantDigits, "floor");
  }

  // TreeNode
  size_t size() const override { return sizeof(FloorLayoutNode); }
#if TREE_LOG
  const char * description() const override { return "FloorLayout"; }
#endif

protected:
  bool renderTopBar() const override { return false; }
};

class FloorLayoutRef : public LayoutReference<FloorLayoutNode> {
public:
  FloorLayoutRef(TreeNode * n) : LayoutReference<FloorLayoutNode>(n) {}
  FloorLayoutRef(LayoutRef l) : LayoutReference<FloorLayoutNode>() {
    addChildTreeAtIndex(l, 0);
  }
};

}

#endif
