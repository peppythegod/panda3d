// Filename: modelNode.h
// Created by:  drose (09Nov00)
// 
////////////////////////////////////////////////////////////////////

#ifndef MODELNODE_H
#define MODELNODE_H

#include <pandabase.h>

#include <namedNode.h>

////////////////////////////////////////////////////////////////////
//       Class : ModelNode
// Description : This node is placed at key points within the scene
//               graph to indicate the roots of "models": subtrees
//               that are conceptually to be treated as a single unit,
//               like a car or a room, for instance.  It doesn't
//               affect rendering or any other operations; it's
//               primarily useful as a high-level model indication.
//
//               ModelNodes are created in response to a <Model> { 1 }
//               flag within an egg file.
////////////////////////////////////////////////////////////////////
class EXPCL_PANDA ModelNode : public NamedNode {
PUBLISHED:
  INLINE ModelNode(const string &name = "");

  INLINE void set_preserve_transform(bool preserve_transform);
  INLINE bool get_preserve_transform() const;

public:
  INLINE ModelNode(const ModelNode &copy);
  INLINE void operator = (const ModelNode &copy);
  
  virtual Node *make_copy() const;

  virtual bool safe_to_flatten() const;
  virtual bool safe_to_transform() const;

public:
  static void register_with_read_factory(void);

protected:
  virtual void write_datagram(BamWriter *manager, Datagram &me);  
  void fillin(DatagramIterator &scan, BamReader *manager);
  static TypedWriteable *make_ModelNode(const FactoryParams &params);

private:
  bool _preserve_transform;

public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    NamedNode::init_type();
    register_type(_type_handle, "ModelNode",
                  NamedNode::get_class_type());
  }
  virtual TypeHandle get_type(void) const {
    return get_class_type();
  }
  virtual TypeHandle force_init_type() {init_type(); return get_class_type();}

private:
  static TypeHandle _type_handle;
};

#include "modelNode.I"

#endif


