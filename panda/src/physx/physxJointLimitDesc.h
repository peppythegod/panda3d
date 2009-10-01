// Filename: physxJointLimitDesc.h
// Created by:  enn0x (28Sep09)
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

#ifndef PHYSXJOINTLIMITDESC_H
#define PHYSXJOINTLIMITDESC_H

#include "pandabase.h"
#include "typedReferenceCount.h"

#include "NoMinMax.h"
#include "NxPhysics.h"

////////////////////////////////////////////////////////////////////
//       Class : PhysxJointLimitDesc
// Description : Describes a joint limit.
////////////////////////////////////////////////////////////////////
class PhysxJointLimitDesc : public TypedReferenceCount {

PUBLISHED:
  INLINE PhysxJointLimitDesc();
  INLINE PhysxJointLimitDesc(float value, float restitution, float hardness);
  INLINE ~PhysxJointLimitDesc();

  void set_value(float value);
  void set_restitution(float restitution);
  void set_hardness(float hardness);

  float get_value() const;
  float get_restitution() const;
  float get_hardness() const;

public:
  INLINE PhysxJointLimitDesc(const NxJointLimitDesc &desc);
  INLINE NxJointLimitDesc desc() const;

private:
  NxJointLimitDesc _desc;

////////////////////////////////////////////////////////////////////
public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    TypedReferenceCount::init_type();
    register_type(_type_handle, "PhysxJointLimitDesc", 
                  TypedReferenceCount::get_class_type());
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

#include "physxJointLimitDesc.I"

#endif // PHYSXJOINTLIMITDESC_H
