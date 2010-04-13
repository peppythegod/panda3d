// Filename: colladaAsset.h
// Created by: Xidram (13Apr10)
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

#ifndef COLLADAASSET_H
#define COLLADAASSET_H

#include "typedReferenceCount.h"
#include "config_collada.h"
#include "coordinateSystem.h"

////////////////////////////////////////////////////////////////////
//       Class : ColladaAsset
// Description : Object that represents the <asset> COLLADA tag.
//
////////////////////////////////////////////////////////////////////
class EXPCL_COLLADA ColladaAsset : public TypedReferenceCount {
PUBLISHED:
  ColladaAsset();
  INLINE CoordinateSystem get_coordinate_system() const;

  bool load_xml(const TiXmlElement *xelement);
  TiXmlElement * make_xml() const;

private:
  CoordinateSystem _coordsys;

public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    TypedReferenceCount::init_type();
    register_type(_type_handle, "ColladaAsset",
                  TypedReferenceCount::get_class_type());
  }
  virtual TypeHandle get_type() const {
    return get_class_type();
  }
  virtual TypeHandle force_init_type() {init_type(); return get_class_type();}

private:
  static TypeHandle _type_handle;
};

#include "colladaAsset.I"

#endif

