// Filename: texturePlacement.cxx
// Created by:  drose (30Nov00)
//
////////////////////////////////////////////////////////////////////
//
// PANDA 3D SOFTWARE
// Copyright (c) 2001, Disney Enterprises, Inc.  All rights reserved
//
// All use of this software is subject to the terms of the Panda 3d
// Software license.  You should have received a copy of this license
// along with this source code; you will also find a current copy of
// the license at http://www.panda3d.org/license.txt .
//
// To contact the maintainers of this program write to
// panda3d@yahoogroups.com .
//
////////////////////////////////////////////////////////////////////

#include "texturePlacement.h"
#include "textureReference.h"
#include "textureImage.h"
#include "paletteGroup.h"
#include "paletteImage.h"
#include "palettizer.h"
#include "eggFile.h"
#include "destTextureImage.h"

#include <indent.h>
#include <datagram.h>
#include <datagramIterator.h>
#include <bamReader.h>
#include <bamWriter.h>
#include <pnmImage.h>

TypeHandle TexturePlacement::_type_handle;

////////////////////////////////////////////////////////////////////
//     Function: TexturePlacement::Default Constructor
//       Access: Private
//  Description: The default constructor is only for the convenience
//               of the Bam reader.
////////////////////////////////////////////////////////////////////
TexturePlacement::
TexturePlacement() {
  _texture = (TextureImage *)NULL;
  _group = (PaletteGroup *)NULL;
  _image = (PaletteImage *)NULL;
  _dest = (DestTextureImage *)NULL;
  _has_uvs = false;
  _size_known = false;
  _is_filled = true;
  _omit_reason = OR_none;
}

////////////////////////////////////////////////////////////////////
//     Function: TexturePlacement::Constructor
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
TexturePlacement::
TexturePlacement(TextureImage *texture, PaletteGroup *group) :
  _texture(texture),
  _group(group)
{
  _omit_reason = OR_working;

  if (!texture->is_size_known()) {
    // If we were never able to figure out what size the texture
    // actually is, then we can't place the texture on a palette.
    _omit_reason = OR_unknown;
  }

  _image = (PaletteImage *)NULL;
  _dest = (DestTextureImage *)NULL;
  _has_uvs = false;
  _size_known = false;
  _is_filled = false;
}

////////////////////////////////////////////////////////////////////
//     Function: TexturePlacement::Destructor
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
TexturePlacement::
~TexturePlacement() {
  // Make sure we tell all our egg references they're not using us any
  // more.
  References::iterator ri;
  References copy_references = _references;
  for (ri = copy_references.begin(); ri != copy_references.end(); ++ri) {
    TextureReference *reference = (*ri);
    nassertv(reference->get_placement() == this);
    reference->clear_placement();
  }

  // And also our group, etc.
  _group->unplace(this);
}

////////////////////////////////////////////////////////////////////
//     Function: TexturePlacement::get_texture
//       Access: Public
//  Description: Returns the texture that this placement represents.
////////////////////////////////////////////////////////////////////
TextureImage *TexturePlacement::
get_texture() const {
  return _texture;
}

////////////////////////////////////////////////////////////////////
//     Function: TexturePlacement::get_properties
//       Access: Public
//  Description: Returns the grouping properties of the image.
////////////////////////////////////////////////////////////////////
const TextureProperties &TexturePlacement::
get_properties() const {
  return _texture->get_properties();
}

////////////////////////////////////////////////////////////////////
//     Function: TexturePlacement::get_group
//       Access: Public
//  Description: Returns the group that this placement represents.
////////////////////////////////////////////////////////////////////
PaletteGroup *TexturePlacement::
get_group() const {
  return _group;
}

////////////////////////////////////////////////////////////////////
//     Function: TexturePlacement::add_egg
//       Access: Public
//  Description: Records the fact that a particular egg file is using
//               this particular TexturePlacement.
////////////////////////////////////////////////////////////////////
void TexturePlacement::
add_egg(TextureReference *reference) {
  reference->mark_egg_stale();

  // Turns out that turning these off is a bad idea, because it may
  // make us forget the size information halfway through processing.
  /*
  _has_uvs = false;
  _size_known = false;
  */
  _references.insert(reference);
}

////////////////////////////////////////////////////////////////////
//     Function: TexturePlacement::remove_egg
//       Access: Public
//  Description: Notes that a particular egg file is no longer using
//               this particular TexturePlacement.
////////////////////////////////////////////////////////////////////
void TexturePlacement::
remove_egg(TextureReference *reference) {
  reference->mark_egg_stale();
  /*
    _has_uvs = false;
    _size_known = false;
  */
  _references.erase(reference);
}

////////////////////////////////////////////////////////////////////
//     Function: TexturePlacement::mark_eggs_stale
//       Access: Public
//  Description: Marks all the egg files that reference this placement
//               stale.  Presumably this is called after moving the
//               texture around in the palette or something.
////////////////////////////////////////////////////////////////////
void TexturePlacement::
mark_eggs_stale() {
  References::iterator ri;
  for (ri = _references.begin(); ri != _references.end(); ++ri) {
    TextureReference *reference = (*ri);

    reference->mark_egg_stale();
  }
}

////////////////////////////////////////////////////////////////////
//     Function: TexturePlacement::set_dest
//       Access: Public
//  Description: Sets the DestTextureImage that corresponds to this
//               texture as it was copied to the install directory.
////////////////////////////////////////////////////////////////////
void TexturePlacement::
set_dest(DestTextureImage *dest) {
  _dest = dest;
}

////////////////////////////////////////////////////////////////////
//     Function: TexturePlacement::get_dest
//       Access: Public
//  Description: Returns the DestTextureImage that corresponds to this
//               texture as it was copied to the install directory.
////////////////////////////////////////////////////////////////////
DestTextureImage *TexturePlacement::
get_dest() const {
  return _dest;
}

////////////////////////////////////////////////////////////////////
//     Function: TexturePlacement::determine_size
//       Access: Public
//  Description: Attempts to determine the appropriate size of the
//               texture for the given placement.  This is based on
//               the UV range of the egg files that reference the
//               texture.  Returns true on success, or false if the
//               texture size cannot be determined (e.g. the texture
//               file is unknown).
//
//               After this returns true, get_x_size() and
//               get_y_size() can be safely called.
////////////////////////////////////////////////////////////////////
bool TexturePlacement::
determine_size() {
  if (!_texture->is_size_known()) {
    // Too bad.
    force_replace();
    _omit_reason = OR_unknown;
    return false;
  }

  // This seems to be unnecessary (because of omit_solitary() and
  // not_solitary()), and in fact bitches the logic in omit_solitary()
  // and not_solitary() so that we call mark_egg_stale()
  // unnecessarily.
  /*
  if (_omit_reason == OR_solitary) {
    // If the texture was previously 'omitted' for being solitary, we
    // give it a second chance now.
    _omit_reason = OR_none;
  }
  */

  // Determine the actual minmax of the UV's in use, as well as
  // whether we should wrap or clamp.
  _has_uvs = false;
  _position._wrap_u = EggTexture::WM_clamp;
  _position._wrap_v = EggTexture::WM_clamp;

  TexCoordd max_uv, min_uv;

  References::iterator ri;
  for (ri = _references.begin(); ri != _references.end(); ++ri) {
    TextureReference *reference = (*ri);
    if (reference->has_uvs()) {
      const TexCoordd &n = reference->get_min_uv();
      const TexCoordd &x = reference->get_max_uv();

      if (_has_uvs) {
        min_uv.set(min(min_uv[0], n[0]), min(min_uv[1], n[1]));
        max_uv.set(max(max_uv[0], x[0]), max(max_uv[1], x[1]));
      } else {
        min_uv = n;
        max_uv = x;
        _has_uvs = true;
      }
    }

    // If any reference repeats the texture, the texture repeats in
    // the palette.
    if (reference->get_wrap_u() == EggTexture::WM_repeat) {
      _position._wrap_u = EggTexture::WM_repeat;
    }
    if (reference->get_wrap_v() == EggTexture::WM_repeat) {
      _position._wrap_v = EggTexture::WM_repeat;
    }
  }

  nassertr(_has_uvs, false);
  TexCoordd rounded_min_uv = min_uv;
  TexCoordd rounded_max_uv = max_uv;

  // If so requested, round the minmax out to the next _round_unit.
  // This cuts down on unnecessary resizing of textures within the
  // palettes as the egg references change in trivial amounts.
  if (pal->_round_uvs) {
    rounded_max_uv[0] =
      ceil((rounded_max_uv[0] - pal->_round_fuzz) / pal->_round_unit) *
      pal->_round_unit;
    rounded_max_uv[1] =
      ceil((rounded_max_uv[1] - pal->_round_fuzz) / pal->_round_unit) *
      pal->_round_unit;

    rounded_min_uv[0] =
      floor((rounded_min_uv[0] + pal->_round_fuzz) / pal->_round_unit) *
      pal->_round_unit;
    rounded_min_uv[1] =
      floor((rounded_min_uv[1] + pal->_round_fuzz) / pal->_round_unit) *
      pal->_round_unit;
  }

  // Now determine the size in pixels we require based on the UV's
  // that actually reference this texture.
  compute_size_from_uvs(rounded_min_uv, rounded_max_uv);

  // Now, can it be placed?
  if (_texture->get_omit()) {
    // Not if the user says it can't.
    force_replace();
    _omit_reason = OR_omitted;

  } else if (get_uv_area() > _texture->get_coverage_threshold()) {
    // If the texture repeats too many times, we can't place it.
    force_replace();
    _omit_reason = OR_coverage;

  } else if ((_position._x_size > pal->_pal_x_size ||
              _position._y_size > pal->_pal_y_size) ||
             (_position._x_size == pal->_pal_x_size &&
              _position._y_size == pal->_pal_y_size)) {
    // If the texture exceeds the size of an empty palette image in
    // either dimension, or if it exactly equals the size of an empty
    // palette image in both dimensions, we can't place it because
    // it's too big.
    force_replace();
    _omit_reason = OR_size;

  } else if (_omit_reason == OR_omitted ||
             _omit_reason == OR_size ||
             _omit_reason == OR_coverage ||
             _omit_reason == OR_unknown) {
    // On the other hand, if the texture was previously omitted
    // explicitly, or because of its size or coverage, now it seems to
    // fit.
    force_replace();
    _omit_reason = OR_working;

  } else if (is_placed()) {
    // It *can* be placed.  If it was already placed previously, can
    // we leave it where it is?

    if (_position._x_size != _placed._x_size ||
        _position._y_size != _placed._y_size ||
        _position._min_uv[0] < _placed._min_uv[0] ||
        _position._min_uv[1] < _placed._min_uv[1] ||
        _position._max_uv[0] > _placed._max_uv[0] ||
        _position._max_uv[1] > _placed._max_uv[1]) {
      // If the texture was previously placed but is now the wrong
      // size, or if the area we need to cover is different, we need
      // to re-place it.

      // However, we make a special exception: if it would have fit
      // without rounding up the UV's, then screw rounding it up and
      // just leave it alone.
      if (pal->_round_uvs) {
        compute_size_from_uvs(min_uv, max_uv);
        if (_position._x_size <= _placed._x_size &&
            _position._y_size <= _placed._y_size &&
            _position._min_uv[0] >= _placed._min_uv[0] &&
            _position._min_uv[1] >= _placed._min_uv[1] &&
            _position._max_uv[0] <= _placed._max_uv[0] &&
            _position._max_uv[1] <= _placed._max_uv[1]) {
          // No problem!  It fits here, so leave well enough alone.
        } else {
          // That's not good enough either, so go back to rounding.
          compute_size_from_uvs(rounded_min_uv, rounded_max_uv);
          force_replace();
        }
      } else {
        force_replace();
      }
    }

    if (_position._wrap_u != _placed._wrap_u ||
        _position._wrap_v != _placed._wrap_v) {
      // The wrap mode properties have changed slightly.  We may or
      // may not need to re-place it, but we will need to update it.
      _is_filled = false;
      _placed._wrap_u = _position._wrap_u;
      _placed._wrap_v = _position._wrap_v;
    }
  }

  return true;
}

////////////////////////////////////////////////////////////////////
//     Function: TexturePlacement::is_size_known
//       Access: Public
//  Description: Returns true if the texture's size is known, false
//               otherwise.  Usually this can only be false after
//               determine_size() has been called there is something
//               wrong with the texture (in which case the placement
//               will automatically omit itself from the palette
//               anyway).
////////////////////////////////////////////////////////////////////
bool TexturePlacement::
is_size_known() const {
  return _size_known;
}

////////////////////////////////////////////////////////////////////
//     Function: TexturePlacement::get_omit_reason
//       Access: Public
//  Description: Returns the reason the texture has been omitted from
//               a palette image, or OR_none if it has not.
////////////////////////////////////////////////////////////////////
OmitReason TexturePlacement::
get_omit_reason() const {
  return _omit_reason;
}

////////////////////////////////////////////////////////////////////
//     Function: TexturePlacement::get_x_size
//       Access: Public
//  Description: Returns the size in the X dimension, in pixels, of
//               the texture image as it must appear in the palette.
//               This accounts for any growing or shrinking of the
//               texture due to the UV coordinate range.
////////////////////////////////////////////////////////////////////
int TexturePlacement::
get_x_size() const {
  nassertr(_size_known, 0);
  return _position._x_size;
}

////////////////////////////////////////////////////////////////////
//     Function: TexturePlacement::get_y_size
//       Access: Public
//  Description: Returns the size in the Y dimension, in pixels, of
//               the texture image as it must appear in the palette.
//               This accounts for any growing or shrinking of the
//               texture due to the UV coordinate range.
////////////////////////////////////////////////////////////////////
int TexturePlacement::
get_y_size() const {
  nassertr(_size_known, 0);
  return _position._y_size;
}

////////////////////////////////////////////////////////////////////
//     Function: TexturePlacement::get_uv_area
//       Access: Public
//  Description: Returns the total area of the rectangle occupied by
//               the UV minmax box, in UV coordinates.  1.0 is the
//               entire texture; values greater than 1 imply the
//               texture repeats.
////////////////////////////////////////////////////////////////////
double TexturePlacement::
get_uv_area() const {
  if (!_has_uvs) {
    return 0.0;
  }

  TexCoordd range = _position._max_uv - _position._min_uv;
  return range[0] * range[1];
}

////////////////////////////////////////////////////////////////////
//     Function: TexturePlacement::is_placed
//       Access: Public
//  Description: Returns true if the texture has been placed on a
//               palette image, false otherwise.  This will generally
//               be true if get_omit_reason() returns OR_none or
//               OR_solitary and false otherwise.
////////////////////////////////////////////////////////////////////
bool TexturePlacement::
is_placed() const {
  return _image != (PaletteImage *)NULL;
}

////////////////////////////////////////////////////////////////////
//     Function: TexturePlacement::get_image
//       Access: Public
//  Description: Returns the particular PaletteImage on which the
//               texture has been placed.
////////////////////////////////////////////////////////////////////
PaletteImage *TexturePlacement::
get_image() const {
  nassertr(is_placed(), (PaletteImage *)NULL);
  return _image;
}

////////////////////////////////////////////////////////////////////
//     Function: TexturePlacement::get_page
//       Access: Public
//  Description: Returns the particular PalettePage on which the
//               texture has been placed.
////////////////////////////////////////////////////////////////////
PalettePage *TexturePlacement::
get_page() const {
  nassertr(is_placed(), (PalettePage *)NULL);
  return _image->get_page();
}

////////////////////////////////////////////////////////////////////
//     Function: TexturePlacement::get_placed_x
//       Access: Public
//  Description: Returns the X pixel at which the texture has been
//               placed within its PaletteImage.  It is an error to
//               call this unless is_placed() returns true.
////////////////////////////////////////////////////////////////////
int TexturePlacement::
get_placed_x() const {
  nassertr(is_placed(), 0);
  return _placed._x;
}

////////////////////////////////////////////////////////////////////
//     Function: TexturePlacement::get_placed_y
//       Access: Public
//  Description: Returns the Y pixel at which the texture has been
//               placed within its PaletteImage.  It is an error to
//               call this unless is_placed() returns true.
////////////////////////////////////////////////////////////////////
int TexturePlacement::
get_placed_y() const {
  nassertr(is_placed(), 0);
  return _placed._y;
}

////////////////////////////////////////////////////////////////////
//     Function: TexturePlacement::get_placed_x_size
//       Access: Public
//  Description: Returns the size in the X dimension, in pixels, of
//               the texture image as it has been placed within the
//               palette.
////////////////////////////////////////////////////////////////////
int TexturePlacement::
get_placed_x_size() const {
  nassertr(is_placed(), 0);
  return _placed._x_size;
}

////////////////////////////////////////////////////////////////////
//     Function: TexturePlacement::get_placed_y_size
//       Access: Public
//  Description: Returns the size in the Y dimension, in pixels, of
//               the texture image as it has been placed within the
//               palette.
////////////////////////////////////////////////////////////////////
int TexturePlacement::
get_placed_y_size() const {
  nassertr(is_placed(), 0);
  return _placed._y_size;
}

////////////////////////////////////////////////////////////////////
//     Function: TexturePlacement::get_placed_uv_area
//       Access: Public
//  Description: Returns the total area of the rectangle occupied by
//               the UV minmax box, as it has been placed.  See also
//               get_uv_area().
////////////////////////////////////////////////////////////////////
double TexturePlacement::
get_placed_uv_area() const {
  nassertr(is_placed(), 0);
  TexCoordd range = _placed._max_uv - _placed._min_uv;
  return range[0] * range[1];
}

////////////////////////////////////////////////////////////////////
//     Function: TexturePlacement::place_at
//       Access: Public
//  Description: Assigns the texture to a particular position within
//               the indicated PaletteImage.  It is an error to call
//               this if the texture has already been placed
//               elsewhere.
////////////////////////////////////////////////////////////////////
void TexturePlacement::
place_at(PaletteImage *image, int x, int y) {
  nassertv(!is_placed());
  nassertv(_size_known);

  _image = image;
  _is_filled = false;
  _position._x = x;
  _position._y = y;
  _placed = _position;
  _omit_reason = OR_none;
}

////////////////////////////////////////////////////////////////////
//     Function: TexturePlacement::force_replace
//       Access: Public
//  Description: Removes the texture from its particular PaletteImage,
//               but does not remove it from the PaletteGroup.  It
//               will be re-placed when the PaletteGroup::place_all()
//               is called.
////////////////////////////////////////////////////////////////////
void TexturePlacement::
force_replace() {
  if (_image != (PaletteImage *)NULL) {
    _image->unplace(this);
    _image = (PaletteImage *)NULL;
  }
  if (_omit_reason == OR_none) {
    mark_eggs_stale();
  }
  _omit_reason = OR_working;
}

////////////////////////////////////////////////////////////////////
//     Function: TexturePlacement::omit_solitary
//       Access: Public
//  Description: Sets the omit reason (returned by get_omit()) to
//               OR_solitary, indicating that the palettized version
//               of the texture should not be used because it is the
//               only texture on a PaletteImage.  However, the texture
//               is still considered placed, and is_placed() will
//               return true.
////////////////////////////////////////////////////////////////////
void TexturePlacement::
omit_solitary() {
  nassertv(is_placed());
  if (_omit_reason != OR_solitary) {
    mark_eggs_stale();
    _omit_reason = OR_solitary;
  }
}

////////////////////////////////////////////////////////////////////
//     Function: TexturePlacement::not_solitary
//       Access: Public
//  Description: Indicates that the texture, formerly indicated as
//               solitary, is now no longer.
////////////////////////////////////////////////////////////////////
void TexturePlacement::
not_solitary() {
  nassertv(is_placed());
  if (_omit_reason != OR_none) {
    mark_eggs_stale();
    _omit_reason = OR_none;
  }
}

////////////////////////////////////////////////////////////////////
//     Function: TexturePlacement::intersects
//       Access: Public
//  Description: Returns true if the particular position this texture
//               has been assigned to overlaps the rectangle whose
//               top left corner is at x, y and whose size is given by
//               x_size, y_size, or false otherwise.
////////////////////////////////////////////////////////////////////
bool TexturePlacement::
intersects(int x, int y, int x_size, int y_size) {
  nassertr(is_placed(), false);

  int hright = x + x_size;
  int hbot = y + y_size;

  int mright = _placed._x + _placed._x_size;
  int mbot = _placed._y + _placed._y_size;

  return !(x >= mright || hright <= _placed._x ||
           y >= mbot || hbot <= _placed._y);
}

////////////////////////////////////////////////////////////////////
//     Function: TexturePlacement::compute_tex_matrix
//       Access: Public
//  Description: Stores in the indicated matrix the appropriate
//               texture matrix transform for the new placement of the
//               texture.
////////////////////////////////////////////////////////////////////
void TexturePlacement::
compute_tex_matrix(LMatrix3d &transform) {
  nassertv(is_placed());

  LMatrix3d source_uvs = LMatrix3d::ident_mat();

  TexCoordd range = _placed._max_uv - _placed._min_uv;
  if (range[0] != 0.0 && range[1] != 0.0) {
    source_uvs =
      LMatrix3d::translate_mat(-_placed._min_uv) *
      LMatrix3d::scale_mat(1.0 / range[0], 1.0 / range[1]);
  }

  int top = _placed._y + _placed._margin;
  int left = _placed._x + _placed._margin;
  int x_size = _placed._x_size - _placed._margin * 2;
  int y_size = _placed._y_size - _placed._margin * 2;

  int bottom = top + y_size;
  int pal_x_size = _image->get_x_size();
  int pal_y_size = _image->get_y_size();

  LVecBase2d t((double)left / (double)pal_x_size,
               (double)(pal_y_size - bottom) / (double)pal_y_size);
  LVecBase2d s((double)x_size / (double)pal_x_size,
               (double)y_size / (double)pal_y_size);

  LMatrix3d dest_uvs
    (s[0],  0.0,  0.0,
     0.0, s[1],  0.0,
     t[0], t[1],  1.0);

  transform = source_uvs * dest_uvs;
}

////////////////////////////////////////////////////////////////////
//     Function: TexturePlacement::write_placed
//       Access: Public
//  Description: Writes the placement position information on a line
//               by itself.
////////////////////////////////////////////////////////////////////
void TexturePlacement::
write_placed(ostream &out, int indent_level) {
  indent(out, indent_level)
    << get_texture()->get_name();

  if (is_placed()) {
    out << " at "
        << get_placed_x() << " " << get_placed_y() << " to "
        << get_placed_x() + get_placed_x_size() << " "
        << get_placed_y() + get_placed_y_size() << " (coverage "
        << get_placed_uv_area() << ")";

    if (_placed._wrap_u != EggTexture::WM_unspecified ||
        _placed._wrap_v != EggTexture::WM_unspecified) {
      if (_placed._wrap_u != _placed._wrap_v) {
        out << " (" << _placed._wrap_u << ", " << _placed._wrap_v << ")";
      } else {
        out << " " << _placed._wrap_u;
      }
    }
    out << "\n";
  } else {
    out << " not yet placed.\n";
  }
};

////////////////////////////////////////////////////////////////////
//     Function: TexturePlacement::is_filled
//       Access: Public
//  Description: Returns true if the texture has been filled
//               (i.e. fill_image() has been called) since it was
//               placed.
////////////////////////////////////////////////////////////////////
bool TexturePlacement::
is_filled() const {
  return _is_filled;
}

////////////////////////////////////////////////////////////////////
//     Function: TexturePlacement::mark_unfilled
//       Access: Public
//  Description: Marks the texture as unfilled, so that it will need
//               to be copied into the palette image again.
////////////////////////////////////////////////////////////////////
void TexturePlacement::
mark_unfilled() {
  _is_filled = false;
}

////////////////////////////////////////////////////////////////////
//     Function: TexturePlacement::fill_image
//       Access: Public
//  Description: Fills in the rectangle of the palette image
//               represented by the texture placement with the image
//               pixels.
////////////////////////////////////////////////////////////////////
void TexturePlacement::
fill_image(PNMImage &image) {
  nassertv(is_placed());

  _is_filled = true;

  // We determine the pixels to place the source image at by
  // transforming the unit texture box: the upper-left and lower-right
  // corners.  These corners, in the final texture coordinate space,
  // represent where on the palette image the original texture should
  // be located.

  LMatrix3d transform;
  compute_tex_matrix(transform);
  TexCoordd ul = TexCoordd(0.0, 1.0) * transform;
  TexCoordd lr = TexCoordd(1.0, 0.0) * transform;

  // Now we convert those texture coordinates back to pixel units.
  int pal_x_size = _image->get_x_size();
  int pal_y_size = _image->get_y_size();

  int top = (int)floor((1.0 - ul[1]) * pal_y_size + 0.5);
  int left = (int)floor(ul[0] * pal_x_size + 0.5);
  int bottom = (int)floor((1.0 - lr[1]) * pal_y_size + 0.5);
  int right = (int)floor(lr[0] * pal_x_size + 0.5);

  // And now we can determine the size to scale the image to based on
  // that.  This may not be the same as texture->size() because of
  // margins.
  int x_size = right - left;
  int y_size = bottom - top;
  nassertv(x_size >= 0 && y_size >= 0);

  // Now we get a PNMImage that represents the source texture at that
  // size.
  const PNMImage &source_full = _texture->read_source_image();
  if (!source_full.is_valid()) {
    flag_error_image(image);
    return;
  }

  PNMImage source(x_size, y_size, source_full.get_num_channels(),
                  source_full.get_maxval());
  source.quick_filter_from(source_full);

  bool alpha = image.has_alpha();
  bool source_alpha = source.has_alpha();

  // Now copy the pixels.  We do this by walking through the
  // rectangular region on the palette image that we have reserved for
  // this texture; for each pixel in this region, we determine its
  // appropriate color based on its relation to the actual texture
  // image location (determined above), and on whether the texture
  // wraps or clamps.
  for (int y = _placed._y; y < _placed._y + _placed._y_size; y++) {
    int sy = y - top;

    if (_placed._wrap_v == EggTexture::WM_clamp) {
      // Clamp at [0, y_size).
      sy = max(min(sy, y_size - 1), 0);

    } else {
      // Wrap: sign-independent modulo.
      sy = (sy < 0) ? y_size - 1 - ((-sy - 1) % y_size) : sy % y_size;
    }

    for (int x = _placed._x; x < _placed._x + _placed._x_size; x++) {
      int sx = x - left;

      if (_placed._wrap_u == EggTexture::WM_clamp) {
        // Clamp at [0, x_size).
        sx = max(min(sx, x_size - 1), 0);

      } else {
        // Wrap: sign-independent modulo.
        sx = (sx < 0) ? x_size - 1 - ((-sx - 1) % x_size) : sx % x_size;
      }

      image.set_xel(x, y, source.get_xel(sx, sy));
      if (alpha) {
        if (source_alpha) {
          image.set_alpha(x, y, source.get_alpha(sx, sy));
        } else {
          image.set_alpha(x, y, 1.0);
        }
      }
    }
  }
}

////////////////////////////////////////////////////////////////////
//     Function: TexturePlacement::flag_error_image
//       Access: Public
//  Description: Sets the rectangle of the palette image
//               represented by the texture placement to red, to
//               represent a missing texture.
////////////////////////////////////////////////////////////////////
void TexturePlacement::
flag_error_image(PNMImage &image) {
  nassertv(is_placed());
  for (int y = _placed._y; y < _placed._y + _placed._y_size; y++) {
    for (int x = _placed._x; x < _placed._x + _placed._x_size; x++) {
      image.set_xel_val(x, y, 1, 0, 0);
    }
  }
  if (image.has_alpha()) {
    for (int y = _placed._y; y < _placed._y + _placed._y_size; y++) {
      for (int x = _placed._x; x < _placed._x + _placed._x_size; x++) {
        image.set_alpha_val(x, y, 1);
      }
    }
  }
}

////////////////////////////////////////////////////////////////////
//     Function: TexturePlacement::compute_size_from_uvs
//       Access: Private
//  Description: A support function for determine_size(), this
//               computes the appropriate size of the texture in
//               pixels based on the UV coverage (as well as on the
//               size of the source texture).
////////////////////////////////////////////////////////////////////
void TexturePlacement::
compute_size_from_uvs(const TexCoordd &min_uv, const TexCoordd &max_uv) {
  _position._min_uv = min_uv;
  _position._max_uv = max_uv;

  TexCoordd range = _position._max_uv - _position._min_uv;

  _position._x_size = (int)floor(_texture->get_x_size() * range[0] + 0.5);
  _position._y_size = (int)floor(_texture->get_y_size() * range[1] + 0.5);

  // We arbitrarily require at least four pixels in each dimension.
  // Fewer than this may be asking for trouble.
  _position._x_size = max(_position._x_size, 4);
  _position._y_size = max(_position._y_size, 4);

  _position._margin = _texture->get_margin();

  // Normally, we have interior margins, but if the image size is too
  // small--i.e. the margin size is too great a percentage of the
  // image size--we'll make them exterior margins so as not to overly
  // degrade the quality of the image.
  if ((double)_position._margin / (double)_position._x_size > 0.10) {
    _position._x_size += _position._margin * 2;
  }
  if ((double)_position._margin / (double)_position._y_size > 0.10) {
    _position._y_size += _position._margin * 2;
  }

  _size_known = true;
}



////////////////////////////////////////////////////////////////////
//     Function: TexturePlacement::register_with_read_factory
//       Access: Public, Static
//  Description: Registers the current object as something that can be
//               read from a Bam file.
////////////////////////////////////////////////////////////////////
void TexturePlacement::
register_with_read_factory() {
  BamReader::get_factory()->
    register_factory(get_class_type(), make_TexturePlacement);
}

////////////////////////////////////////////////////////////////////
//     Function: TexturePlacement::write_datagram
//       Access: Public, Virtual
//  Description: Fills the indicated datagram up with a binary
//               representation of the current object, in preparation
//               for writing to a Bam file.
////////////////////////////////////////////////////////////////////
void TexturePlacement::
write_datagram(BamWriter *writer, Datagram &datagram) {
  TypedWritable::write_datagram(writer, datagram);
  writer->write_pointer(datagram, _texture);
  writer->write_pointer(datagram, _group);
  writer->write_pointer(datagram, _image);
  writer->write_pointer(datagram, _dest);

  datagram.add_bool(_has_uvs);
  datagram.add_bool(_size_known);
  _position.write_datagram(writer, datagram);

  datagram.add_bool(_is_filled);
  _placed.write_datagram(writer, datagram);
  datagram.add_int32((int)_omit_reason);

  datagram.add_int32(_references.size());
  References::const_iterator ri;
  for (ri = _references.begin(); ri != _references.end(); ++ri) {
    writer->write_pointer(datagram, (*ri));
  }

}

////////////////////////////////////////////////////////////////////
//     Function: TexturePlacement::complete_pointers
//       Access: Public, Virtual
//  Description: Called after the object is otherwise completely read
//               from a Bam file, this function's job is to store the
//               pointers that were retrieved from the Bam file for
//               each pointer object written.  The return value is the
//               number of pointers processed from the list.
////////////////////////////////////////////////////////////////////
int TexturePlacement::
complete_pointers(TypedWritable **p_list, BamReader *manager) {
  int index = TypedWritable::complete_pointers(p_list, manager);

  if (p_list[index] != (TypedWritable *)NULL) {
    DCAST_INTO_R(_texture, p_list[index], index);
  }
  index++;

  if (p_list[index] != (TypedWritable *)NULL) {
    DCAST_INTO_R(_group, p_list[index], index);
  }
  index++;

  if (p_list[index] != (TypedWritable *)NULL) {
    DCAST_INTO_R(_image, p_list[index], index);
  }
  index++;

  if (Palettizer::_read_pi_version >= 2) {
    if (p_list[index] != (TypedWritable *)NULL) {
      DCAST_INTO_R(_dest, p_list[index], index);
    }
    index++;
  }

  int i;
  for (i = 0; i < _num_references; i++) {
    TextureReference *reference;
    DCAST_INTO_R(reference, p_list[index], index);
    _references.insert(reference);
    index++;
  }

  return index;
}

////////////////////////////////////////////////////////////////////
//     Function: TexturePlacement::make_TexturePlacement
//       Access: Protected
//  Description: This method is called by the BamReader when an object
//               of this type is encountered in a Bam file; it should
//               allocate and return a new object with all the data
//               read.
////////////////////////////////////////////////////////////////////
TypedWritable* TexturePlacement::
make_TexturePlacement(const FactoryParams &params) {
  TexturePlacement *me = new TexturePlacement;
  DatagramIterator scan;
  BamReader *manager;

  parse_params(params, scan, manager);
  me->fillin(scan, manager);
  return me;
}

////////////////////////////////////////////////////////////////////
//     Function: TexturePlacement::fillin
//       Access: Protected
//  Description: Reads the binary data from the given datagram
//               iterator, which was written by a previous call to
//               write_datagram().
////////////////////////////////////////////////////////////////////
void TexturePlacement::
fillin(DatagramIterator &scan, BamReader *manager) {
  TypedWritable::fillin(scan, manager);

  manager->read_pointer(scan, this);  // _texture
  manager->read_pointer(scan, this);  // _group
  manager->read_pointer(scan, this);  // _image

  if (Palettizer::_read_pi_version >= 2) {
    manager->read_pointer(scan, this);  // _dest
  }

  _has_uvs = scan.get_bool();
  _size_known = scan.get_bool();
  _position.fillin(scan, manager);

  _is_filled = scan.get_bool();
  _placed.fillin(scan, manager);
  _omit_reason = (OmitReason)scan.get_int32();

  _num_references = scan.get_int32();
  manager->read_pointers(scan, this, _num_references);
}


////////////////////////////////////////////////////////////////////
//     Function: SortPlacementBySize::Function Operator
//       Access: Public
//  Description: Compares two TexturePlacement objects and returns
//               true if the first one is bigger than the second one,
//               false otherwise.
////////////////////////////////////////////////////////////////////
bool SortPlacementBySize::
operator ()(TexturePlacement *a, TexturePlacement *b) const {
  if (a->get_y_size() < b->get_y_size()) {
    return false;

  } else if (b->get_y_size() < a->get_y_size()) {
    return true;

  } else if (a->get_x_size() < b->get_x_size()) {
    return false;

  } else if (b->get_x_size() < a->get_x_size()) {
    return true;
  }
  return false;
}
