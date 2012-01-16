// Filename: bulletCharacterControllerNode.h
// Created by:  enn0x (21Nov10)
//
////////////////////////////////////////////////////////////////////
//
// PANDA 3D SOFTWARE
// Copyright (c) Carnegie Mellon University.  All rights reserved.
//
// All use of this software is subject to the terms of the revised BSD
// license.  You should have received a copy of this license along
// with this source code in a file named "LICENSE."
//
////////////////////////////////////////////////////////////////////

#ifndef __BULLET_CHARACTER_CONTROLLER_NODE_H__
#define __BULLET_CHARACTER_CONTROLLER_NODE_H__

#include "pandabase.h"

#include "bullet_includes.h"
#include "bullet_utils.h"
#include "bulletShape.h"

#include "pandaNode.h"
#include "collideMask.h"
#include "luse.h"
#include "transformState.h"
#include "nodePath.h"

////////////////////////////////////////////////////////////////////
//       Class : BulletCharacterControllerNode
// Description : 
////////////////////////////////////////////////////////////////////
class EXPCL_PANDABULLET BulletCharacterControllerNode : public PandaNode {

PUBLISHED:
  BulletCharacterControllerNode(BulletShape *shape, PN_stdfloat step_height, const char *name="character");
  INLINE ~BulletCharacterControllerNode();

  void set_linear_movement(const LVector3 &velocity, bool is_local);
  void set_angular_movement(PN_stdfloat omega);

  BulletShape *get_shape() const;

  PN_stdfloat get_gravity() const;
  PN_stdfloat get_max_slope() const;

  void set_fall_speed(PN_stdfloat fall_speed);
  void set_jump_speed(PN_stdfloat jump_speed);
  void set_max_jump_height(PN_stdfloat max_jump_height);
  void set_max_slope(PN_stdfloat max_slope);
  void set_gravity(PN_stdfloat gravity);
  void set_use_ghost_sweep_test(bool value);

  bool is_on_ground() const;
  bool can_jump() const;
  void do_jump();

public:
  virtual CollideMask get_legal_collide_mask() const;

  virtual bool safe_to_flatten() const;
  virtual bool safe_to_transform() const;
  virtual bool safe_to_modify_transform() const;
  virtual bool safe_to_combine() const;
  virtual bool safe_to_combine_children() const;
  virtual bool safe_to_flatten_below() const;

  INLINE btPairCachingGhostObject *get_ghost() const;
  INLINE btKinematicCharacterController *get_character() const;

  void sync_p2b(PN_stdfloat dt, int num_substeps);
  void sync_b2p();

protected:
  virtual void transform_changed();

private:
  CPT(TransformState) _sync;
  bool _sync_disable;

  BulletUpAxis _up;

  btKinematicCharacterController *_character;
  btPairCachingGhostObject *_ghost;

  PT(BulletShape) _shape;

  LVector3 _linear_movement;
  bool _linear_movement_is_local;
  PN_stdfloat _angular_movement;

////////////////////////////////////////////////////////////////////
public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    PandaNode::init_type();
    register_type(_type_handle, "BulletCharacterControllerNode", 
                  PandaNode::get_class_type());
  }
  virtual TypeHandle get_type() const {
    return get_class_type();
  }
  virtual TypeHandle force_init_type() {
    init_type();
    return get_class_type();
  }

private:
  static TypeHandle _type_handle;
};

#include "bulletCharacterControllerNode.I"

#endif // __BULLET_CHARACTER_CONTROLLER_NODE_H__

