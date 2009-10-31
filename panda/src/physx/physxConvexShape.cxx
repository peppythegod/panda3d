// Filename: physxConvexShape.cxx
// Created by:  enn0x (14Oct09)
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

#include "physxConvexShape.h"

TypeHandle PhysxConvexShape::_type_handle;

////////////////////////////////////////////////////////////////////
//     Function: PhysxConvexShape::link
//       Access: Public
//  Description: 
////////////////////////////////////////////////////////////////////
void PhysxConvexShape::
link(NxShape *shapePtr) {

  ref();
  _ptr = shapePtr->isConvexMesh();
  _ptr->userData = this;
  _error_type = ET_ok;
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxConvexShape::unlink
//       Access: Public
//  Description: 
////////////////////////////////////////////////////////////////////
void PhysxConvexShape::
unlink() {

  _ptr->userData = NULL;
  _error_type = ET_released;
  unref();
}
