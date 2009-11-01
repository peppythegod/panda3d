// Filename: physxSegment.cxx
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

#include "physxSegment.h"
#include "physxManager.h"

////////////////////////////////////////////////////////////////////
//     Function: PhysxSegment::Constructor
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
PhysxSegment::
PhysxSegment(const LPoint3f &p0, const LPoint3f &p1) {

  _segment.p0 = PhysxManager::point3_to_nxVec3(p0);
  _segment.p1 = PhysxManager::point3_to_nxVec3(p1);
}

////////////////////////////////////////////////////////////////////
//     Function : PhysxSegment::get_p0
//       Access : Published
//  Description :
////////////////////////////////////////////////////////////////////
LPoint3f PhysxSegment::
get_p0() const {

  return PhysxManager::nxVec3_to_vec3(_segment.p0);
}

////////////////////////////////////////////////////////////////////
//     Function : PhysxSegment::set_p0
//       Access : Published
//  Description :
////////////////////////////////////////////////////////////////////
void PhysxSegment::
set_p0(LPoint3f p) {

  _segment.p0 = PhysxManager::vec3_to_nxVec3(p);
}

////////////////////////////////////////////////////////////////////
//     Function : PhysxSegment::get_p1
//       Access : Published
//  Description :
////////////////////////////////////////////////////////////////////
LPoint3f PhysxSegment::
get_p1() const {

  return PhysxManager::nxVec3_to_vec3(_segment.p1);
}

////////////////////////////////////////////////////////////////////
//     Function : PhysxSegment::set_p1
//       Access : Published
//  Description :
////////////////////////////////////////////////////////////////////
void PhysxSegment::
set_p1(LPoint3f p) {

  _segment.p1 = PhysxManager::vec3_to_nxVec3(p);
}

////////////////////////////////////////////////////////////////////
//     Function : PhysxSegment::get_origin
//       Access : Published
//  Description :
////////////////////////////////////////////////////////////////////
LPoint3f PhysxSegment::
get_origin() const {

  return PhysxManager::nxVec3_to_point3(_segment.getOrigin());
}

////////////////////////////////////////////////////////////////////
//     Function : PhysxSegment::compute_direction
//       Access : Published
//  Description :
////////////////////////////////////////////////////////////////////
void PhysxSegment::
compute_direction(LPoint3f &dir) const {

  _segment.computeDirection(PhysxManager::point3_to_nxVec3(dir));
}

////////////////////////////////////////////////////////////////////
//     Function : PhysxSegment::compute_length
//       Access : Published
//  Description :
////////////////////////////////////////////////////////////////////
float PhysxSegment::
compute_length() const {

  return _segment.computeLength();
}

////////////////////////////////////////////////////////////////////
//     Function : PhysxSegment::compute_point
//       Access : Published
//  Description :
////////////////////////////////////////////////////////////////////
void PhysxSegment::
compute_point(LPoint3f &p, float t) const {

  _segment.computePoint(PhysxManager::point3_to_nxVec3(p), t);
}

////////////////////////////////////////////////////////////////////
//     Function : PhysxSegment::compute_square_length
//       Access : Published
//  Description :
////////////////////////////////////////////////////////////////////
float PhysxSegment::
compute_square_length() const {

  return _segment.computeSquareLength();
}

////////////////////////////////////////////////////////////////////
//     Function : PhysxSegment::set_origin_direction
//       Access : Published
//  Description :
////////////////////////////////////////////////////////////////////
void PhysxSegment::
set_origin_direction(const LPoint3f &origin, const LVector3f &direction) {

  _segment.setOriginDirection(PhysxManager::point3_to_nxVec3(origin),
                              PhysxManager::vec3_to_nxVec3(direction));
}
