// Filename: physxHeightFieldShape.cxx
// Created by:  enn0x (15Oct09)
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

#include "physxHeightFieldShape.h"

TypeHandle PhysxHeightFieldShape::_type_handle;

////////////////////////////////////////////////////////////////////
//     Function: PhysxHeightFieldShape::link
//       Access: Public
//  Description: 
////////////////////////////////////////////////////////////////////
void PhysxHeightFieldShape::
link(NxShape *shapePtr) {

  ref();
  _ptr = shapePtr->isHeightField();
  _ptr->userData = this;
  _error_type = ET_ok;
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxHeightFieldShape::unlink
//       Access: Public
//  Description: 
////////////////////////////////////////////////////////////////////
void PhysxHeightFieldShape::
unlink() {

  _ptr->userData = NULL;
  _error_type = ET_released;
  unref();
}
