// Filename: config_gobj.cxx
// Created by:  drose (01Oct99)
//
////////////////////////////////////////////////////////////////////
//
// PANDA 3D SOFTWARE
// Copyright (c) 2001 - 2004, Disney Enterprises, Inc.  All rights reserved
//
// All use of this software is subject to the terms of the Panda 3d
// Software license.  You should have received a copy of this license
// along with this source code; you will also find a current copy of
// the license at http://etc.cmu.edu/panda3d/docs/license/ .
//
// To contact the maintainers of this program write to
// panda3d-general@lists.sourceforge.net .
//
////////////////////////////////////////////////////////////////////

#include "config_util.h"
#include "boundedObject.h"
#include "config_gobj.h"
#include "drawable.h"
#include "geom.h"
#include "geomprimitives.h"
#include "qpgeom.h"
#include "qpgeomMunger.h"
#include "qpgeomPrimitive.h"
#include "qpgeomTriangles.h"
#include "qpgeomTristrips.h"
#include "qpgeomTrifans.h"
#include "qpgeomLines.h"
#include "qpgeomLinestrips.h"
#include "qpgeomPoints.h"
#include "qpgeomVertexArrayData.h"
#include "qpgeomVertexArrayFormat.h"
#include "qpgeomVertexData.h"
#include "qpgeomVertexFormat.h"
#include "material.h"
#include "orthographicLens.h"
#include "matrixLens.h"
#include "perspectiveLens.h"
#include "lens.h"
#include "sliderTable.h"
#include "texture.h"
#include "textureStage.h"
#include "textureContext.h"
#include "transformBlendPalette.h"
#include "transformPalette.h"
#include "userVertexSlider.h"
#include "userVertexTransform.h"
#include "vertexTransform.h"
#include "vertexSlider.h"
#include "geomContext.h"
#include "vertexBufferContext.h"
#include "indexBufferContext.h"
#include "internalName.h"

#include "dconfig.h"
#include "string_utils.h"

Configure(config_gobj);
NotifyCategoryDef(gobj, "");

ConfigVariableInt max_texture_dimension
("max-texture-dimension", -1,
 PRC_DESC("Set this to the maximum size a texture is allowed to be in either "
          "dimension.  This is generally intended as a simple way to restrict "
          "texture sizes for limited graphics cards.  When this is greater "
          "than zero, each texture image loaded from a file (but only those "
          "loaded from a file) will be automatically scaled down, if "
          "necessary, so that neither dimension is larger than this value."));

ConfigVariableBool keep_texture_ram
("keep-texture-ram", false,
 PRC_DESC("Set this to true to retain the ram image for each texture after it "
          "has been prepared with the GSG.  This will allow the texture to be "
          "prepared with multiple GSG's, or to be re-prepared later after it is "
          "explicitly released from the GSG, without having to reread the "
          "texture image from disk; but it will consume memory somewhat "
          "wastefully."));

ConfigVariableBool keep_geom_ram
("keep-geom-ram", true,
 PRC_DESC("Set this to true to retain the vertices in ram for each geom "
          "after it has been prepared with the GSG.  This is similar to "
          "keep-texture-ram, but it is a little more dangerous, because if "
          "anyone calls release_all_geoms() on the GSG (or if there are "
          "multiple GSG's rendering a given geom), Panda won't be able to "
          "restore the vertices."));

ConfigVariableBool retained_mode
("retained-mode", false,
 PRC_DESC("Set this true to allow the use of retained mode rendering, which "
          "creates specific cache information (like display lists or vertex "
          "buffers) with the GSG for static geometry, when supported by the "
          "GSG.  Set it false to use only immediate mode, which sends the "
          "vertices to the GSG every frame.  This is used only in the "
          "original Geom implementation; it is replaced by display-lists "
          "in the experimental Geom rewrite."));

ConfigVariableBool vertex_buffers
("vertex-buffers", false,
 PRC_DESC("Set this true to allow the use of vertex buffers (or buffer "
          "objects, as OpenGL dubs them) for rendering vertex data.  This "
          "can greatly improve rendering performance, especially on "
          "higher-end graphics cards, at the cost of some additional "
          "graphics memory (which might otherwise be used for textures "
          "or offscreen buffers)."));

ConfigVariableBool display_lists
("display-lists", false,
 PRC_DESC("Set this true to allow the use of OpenGL display lists for "
          "rendering static geometry.  On some systems, this can result "
          "in a performance improvement over vertex buffers alone; on "
          "other systems (particularly low-end systems) it makes little to "
          "no difference.  This has no effect on DirectX rendering.  If "
          "vertex-buffers is also enabled, then OpenGL buffer objects "
          "will also be created for dynamic geometry."));

ConfigVariableBool use_qpgeom
("use-qpgeom", false,
 PRC_DESC("A temporary variable while the experimental Geom rewrite is "
          "underway.  Set this true if you want to use the experimental "
          "code.  You don't really want to set this true."));


ConfigVariableEnum<BamTextureMode> bam_texture_mode
("bam-texture-mode", BTM_relative,
 PRC_DESC("Set this to specify how textures should be written into Bam files."
          "See the panda source or documentation for available options."));

ConfigVariableEnum<AutoTextureScale> textures_power_2
("textures-power-2", ATS_down,
 PRC_DESC("Specify whether textures should automatically be constrained to "
          "dimensions which are a power of 2 when they are loaded from "
          "disk.  Set this to 'none' to disable this feature, or to "
          "'down' or 'up' to scale down or up to the nearest power of 2, "
          "respectively.  This only has effect on textures which are not "
          "already a power of 2."));

ConfigVariableEnum<AutoTextureScale> textures_square
("textures-square", ATS_none,
 PRC_DESC("Specify whether textures should automatically be constrained to "
          "a square aspect ratio when they are loaded from disk.  Set this "
          "to 'none', 'down', or 'up'.  See textures-power-2."));

ConfigVariableString fake_texture_image
("fake-texture-image", "",
 PRC_DESC("Set this to enable a speedy-load mode in which you don't care "
          "what the world looks like, you just want it to load in minimal "
          "time.  This causes all texture loads via the TexturePool to use "
          "the same texture file, which will presumably only be loaded "
          "once."));

ConfigVariableInt vertex_convert_cache
("vertex-convert-cache", 4194304, // 4 MB
 PRC_DESC("This is the amount of memory, in bytes, that should be set "
          "aside for storing pre-processed data for rendering vertices.  "
          "This is not a limit on the actual vertex data, which is "
          "determined by the model; it is also not a limit on the "
          "amount of memory used by the video driver or the system "
          "graphics interface, which Panda has no control over."));

ConfigVariableDouble default_near
("default-near", 1.0,
 PRC_DESC("The default near clipping distance for all cameras."));

ConfigVariableDouble default_far
("default-far", 1000.0,
 PRC_DESC("The default far clipping distance for all cameras."));

ConfigVariableDouble default_fov
("default-fov", 40.0,
 PRC_DESC("The default field of view in degrees for all cameras."));


ConfigVariableDouble default_keystone
("default-keystone", 0.0f,
 PRC_DESC("The default keystone correction, as an x y pair, for all cameras."));



ConfigureFn(config_gobj) {
  BoundedObject::init_type();
  Geom::init_type();
  GeomLine::init_type();
  GeomLinestrip::init_type();
  GeomPoint::init_type();
  GeomSprite::init_type();
  GeomPolygon::init_type();
  GeomQuad::init_type();
  GeomSphere::init_type();
  GeomTri::init_type();
  GeomTrifan::init_type();
  GeomTristrip::init_type();
  qpGeom::init_type();
  qpGeomMunger::init_type();
  qpGeomPrimitive::init_type();
  qpGeomTriangles::init_type();
  qpGeomTristrips::init_type();
  qpGeomTrifans::init_type();
  qpGeomLines::init_type();
  qpGeomLinestrips::init_type();
  qpGeomPoints::init_type();
  qpGeomVertexArrayData::init_type();
  qpGeomVertexArrayFormat::init_type();
  qpGeomVertexData::init_type();
  qpGeomVertexFormat::init_type();
  TextureContext::init_type();
  GeomContext::init_type();
  VertexBufferContext::init_type();
  IndexBufferContext::init_type();
  Material::init_type();
  OrthographicLens::init_type();
  MatrixLens::init_type();
  PerspectiveLens::init_type();
  Lens::init_type();
  SliderTable::init_type();
  Texture::init_type();
  dDrawable::init_type();
  TextureStage::init_type();
  TransformBlendPalette::init_type();
  TransformPalette::init_type();
  UserVertexSlider::init_type();
  UserVertexTransform::init_type();
  VertexTransform::init_type();
  VertexSlider::init_type();
  InternalName::init_type();

  //Registration of writeable object's creation
  //functions with BamReader's factory
  GeomPoint::register_with_read_factory();
  GeomLine::register_with_read_factory();
  GeomLinestrip::register_with_read_factory();
  GeomSprite::register_with_read_factory();
  GeomPolygon::register_with_read_factory();
  GeomQuad::register_with_read_factory();
  GeomTri::register_with_read_factory();
  GeomTristrip::register_with_read_factory();
  GeomTrifan::register_with_read_factory();
  GeomSphere::register_with_read_factory();
  qpGeom::register_with_read_factory();
  qpGeomTriangles::register_with_read_factory();
  qpGeomTristrips::register_with_read_factory();
  qpGeomTrifans::register_with_read_factory();
  qpGeomLines::register_with_read_factory();
  qpGeomLinestrips::register_with_read_factory();
  qpGeomPoints::register_with_read_factory();
  qpGeomVertexArrayData::register_with_read_factory();
  qpGeomVertexArrayFormat::register_with_read_factory();
  qpGeomVertexData::register_with_read_factory();
  qpGeomVertexFormat::register_with_read_factory();
  Material::register_with_read_factory();
  OrthographicLens::register_with_read_factory();
  MatrixLens::register_with_read_factory();
  PerspectiveLens::register_with_read_factory();
  SliderTable::register_with_read_factory();
  Texture::register_with_read_factory();
  TextureStage::register_with_read_factory();
  TransformBlendPalette::register_with_read_factory();
  TransformPalette::register_with_read_factory();
  UserVertexSlider::register_with_read_factory();
  UserVertexTransform::register_with_read_factory();
  InternalName::register_with_read_factory();
}

ostream &
operator << (ostream &out, BamTextureMode btm) {
  switch (btm) {
  case BTM_unchanged:
    return out << "unchanged";
   
  case BTM_fullpath:
    return out << "fullpath";
    
  case BTM_relative:
    return out << "relative";
    
  case BTM_basename:
    return out << "basename";

  case BTM_rawdata:
    return out << "rawdata";
  }

  return out << "**invalid BamTextureMode (" << (int)btm << ")**";
}

istream &
operator >> (istream &in, BamTextureMode &btm) {
  string word;
  in >> word;

  if (cmp_nocase(word, "unchanged") == 0) {
    btm = BTM_unchanged;
  } else if (cmp_nocase(word, "fullpath") == 0) {
    btm = BTM_fullpath;
  } else if (cmp_nocase(word, "relative") == 0) {
    btm = BTM_relative;
  } else if (cmp_nocase(word, "basename") == 0) {
    btm = BTM_basename;
  } else if (cmp_nocase(word, "rawdata") == 0) {
    btm = BTM_rawdata;

  } else {
    gobj_cat->error() << "Invalid BamTextureMode value: " << word << "\n";
    btm = BTM_relative;
  }

  return in;
}

ostream &
operator << (ostream &out, AutoTextureScale ats) {
  switch (ats) {
  case ATS_none:
    return out << "none";
   
  case ATS_down:
    return out << "down";
    
  case ATS_up:
    return out << "up";
  }

  return out << "**invalid AutoTextureScale (" << (int)ats << ")**";
}

istream &
operator >> (istream &in, AutoTextureScale &ats) {
  string word;
  in >> word;

  if (cmp_nocase(word, "none") == 0 ||
      cmp_nocase(word, "0") == 0 ||
      cmp_nocase(word, "#f") == 0 ||
      tolower(word[0] == 'f')) {
    ats = ATS_none;

  } else if (cmp_nocase(word, "down") == 0 ||
          cmp_nocase(word, "1") == 0 ||
          cmp_nocase(word, "#t") == 0 ||
          tolower(word[0] == 't')) {
    ats = ATS_down;

  } else if (cmp_nocase(word, "up") == 0) {
    ats = ATS_up;

  } else {
    gobj_cat->error() << "Invalid AutoTextureScale value: " << word << "\n";
    ats = ATS_none;
  }

  return in;
}
