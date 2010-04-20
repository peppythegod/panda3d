// Filename: colladaGeometry.cxx
// Created by: rdb (20Apr10)
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

#include "colladaGeometry.h"

TypeHandle ColladaGeometry::_type_handle;
const string ColladaGeometry::_element_name ("geometry");
const string ColladaGeometry::_library_name ("library_geometries");

////////////////////////////////////////////////////////////////////
//     Function: ColladaGeometry::clear
//       Access: Public
//  Description: Resets the ColladaGeometry to its initial state.
////////////////////////////////////////////////////////////////////
void ColladaGeometry::
clear () {
  ColladaAssetElement::clear();
}
