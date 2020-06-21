#ifndef CAS_NODE_H_
#define CAS_NODE_H_

#include "cas/dimension.hpp"
#include "cas/index_stats.hpp"
#include <cstdint>
#include <cstring>
#include <vector>
#include <functional>


namespace cas {

class Node;
using ChildIt = std::function<bool(uint8_t, Node&)>;


class Node {
public:
  Dimension dimension_;
  uint16_t nr_children_ = 0;
  uint16_t separator_pos_ = 0;
  std::vector<uint8_t> prefix_;

  Node(Dimension dimension);

  virtual ~Node() = default;

  virtual inline bool IsLeaf() {
    return false;
  };

  bool IsPathNode() const;
  bool IsValueNode() const;
  Dimension Type() const;
  virtual bool IsFull() const = 0;

  /**
   * Assumes key_byte is not yet present in node
   **/
  virtual void Put(uint8_t key_byte, Node *child) = 0;

  virtual Node* LocateChild(uint8_t key_byte) = 0;

  virtual Node* Grow() = 0;

  virtual void ReplaceBytePointer(uint8_t key_byte, Node* child) = 0;

  virtual void ForEachChild(uint8_t low, uint8_t high, const ChildIt& callback) = 0;

  virtual void ForEachChild(uint8_t low, const ChildIt& callback);

  virtual void ForEachChild(const ChildIt& callback);

  virtual size_t SizeBytes() = 0;

  virtual void Dump();

  void DumpRecursive();

  void DumpConcise(uint8_t edge_label, int indent);

  void CollectStats(IndexStats& stats, size_t depth);

  virtual int NodeWidth() = 0;

  size_t PathPrefixSize() const;

  size_t ValuePrefixSize() const;

protected:
  static void DumpBuffer(uint8_t *buffer, size_t length);

  static void DumpAddresses(Node **buffer, size_t length);

};


} // namespace cas

#endif  // CAS_NODE_H_
