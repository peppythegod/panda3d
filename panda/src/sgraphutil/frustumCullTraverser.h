// Filename: frustumCullTraverser.h
// Created by:  drose (14Apr00)
// 
////////////////////////////////////////////////////////////////////

#ifndef FRUSTUMCULLTRAVERSER_H
#define FRUSTUMCULLTRAVERSER_H

#include <pandabase.h>

#include <node.h>
#include <nodeRelation.h>
#include <typeHandle.h>
#include <geometricBoundingVolume.h>
#include <graphicsStateGuardian.h>


///////////////////////////////////////////////////////////////////
// 	 Class : FrustumCullTraverser
// Description : A special kind of depth-first traverser that can
//               prune the graph based on a lack of intersection with
//               a given bounding volume; i.e. it performs
//               view-frustum culling.
////////////////////////////////////////////////////////////////////
template<class Visitor, class LevelState>
class FrustumCullTraverser {
public:
  typedef TYPENAME Visitor::TransitionWrapper TransitionWrapper;
  typedef TYPENAME Visitor::AttributeWrapper AttributeWrapper;

  FrustumCullTraverser(Node *root,
		       Visitor &visitor,
		       const AttributeWrapper &initial_render_state,
		       const LevelState &initial_level_state,
		       GraphicsStateGuardian *gsg, 
		       TypeHandle graph_type);

protected:
  void traverse(NodeRelation *arc, 
		AttributeWrapper render_state,
		LevelState level_state,
		PT(GeometricBoundingVolume) local_frustum, 
		bool all_in);
  void traverse(Node *node, 
		AttributeWrapper &render_state,
		LevelState &level_state,
		GeometricBoundingVolume *local_frustum, 
		bool all_in);

  Visitor &_visitor;
  AttributeWrapper _initial_render_state;
  GraphicsStateGuardian *_gsg;
  TypeHandle _graph_type;

  // If we are performing view-frustum culling, this is a pointer to
  // the bounding volume that encloses the view frustum, in its own
  // coordinate space.  If we are not performing view-frustum culling,
  // this will be a NULL pointer.
  PT(GeometricBoundingVolume) _view_frustum;

  // This is a list of arcs we have passed so we can perform
  // unambiguous wrt's.
  typedef vector<NodeRelation *> ArcStack;
  ArcStack _arc_stack;
};

// Convenience function.
template<class Visitor, class AttributeWrapper, class LevelState>
INLINE void
fc_traverse(Node *root, Visitor &visitor,
	    const AttributeWrapper &initial_render_state, 
	    const LevelState &initial_level_state,
	    GraphicsStateGuardian *gsg, TypeHandle graph_type) {
  FrustumCullTraverser<Visitor, LevelState> 
    fct(root, visitor, initial_render_state, 
	initial_level_state, gsg, graph_type);
}

#include "frustumCullTraverser.I"

#endif
