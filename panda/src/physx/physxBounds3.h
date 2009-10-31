// Filename: physxBounds3.h
// Created by:  enn0x (31Oct09)
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

#ifndef PHYSXBOUNDS3_H
#define PHYSXBOUNDS3_H

#include "pandabase.h"
#include "luse.h"

#include "config_physx.h"

#include "NxPhysics.h"

////////////////////////////////////////////////////////////////////
//       Class : PhysxBounds3
// Description : 
////////////////////////////////////////////////////////////////////
class EXPCL_PANDAPHYSX PhysxBounds3 {

PUBLISHED:
  INLINE PhysxBounds3();
  INLINE ~PhysxBounds3();

  void bounds_of_obb(const LMatrix3f &orientation, const LPoint3f &translation, const LVector3f &half_dims);
  void combine(const PhysxBounds3 &b2);
  bool contain(const LPoint3f &p) const;
  void fatten(float distance);
  LVector3f get_dimensions() const;
  void include(const LPoint3f &v);
  bool intersects(const PhysxBounds3 &b) const;
  bool intersects2d(const PhysxBounds3 &b, unsigned axis_to_ignore) const;
  bool is_empty() const;
  void scale(float scale);
  void set(const LPoint3f &min, const LPoint3f &max);
  void set_center_extents(const LPoint3f &center, const LVector3f &extents);
  void set_empty();
  void set_infinite();
  void transform(const LMatrix3f &orientation, const LPoint3f &translation);

  LPoint3f get_max() const;
  LPoint3f get_min() const;

  void set_max(LPoint3f value);
  void set_min(LPoint3f value);

public:
  NxBounds3 _bounds;
};

#include "physxBounds3.I"

#endif // PHYSBOUNDS3_H
