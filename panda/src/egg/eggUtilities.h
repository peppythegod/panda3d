// Filename: eggUtilities.h
// Created by:  drose (28Jan99)
//
////////////////////////////////////////////////////////////////////

#ifndef EGGUTILITIES_H
#define EGGUTILITIES_H

////////////////////////////////////////////////////////////////////
//
// eggUtilities.h
//
// Handy functions that operate on egg structures, but don't
// necessarily belong in any one class.
//
////////////////////////////////////////////////////////////////////

#include <pandabase.h>

#include "eggTexture.h"
#include "pt_EggTexture.h"

#include <filename.h>
#include <pointerTo.h>

#include <set>
#include <map>

class EggNode;
class EggVertex;

typedef set< PT_EggTexture > EggTextures;
typedef map<Filename, EggTextures> EggTextureFilenames;


////////////////////////////////////////////////////////////////////
//     Function: get_textures_by_filename
//  Description: Extracts from the egg subgraph beginning at the
//               indicated node a set of all the texture objects
//               referenced, grouped together by filename.  Texture
//               objects that share a common filename (but possibly
//               differ in other properties) are returned together in
//               the same element of the map.
////////////////////////////////////////////////////////////////////
void
get_textures_by_filename(const EggNode *node, EggTextureFilenames &result);


////////////////////////////////////////////////////////////////////
//     Function: split_vertex
//  Description: Splits a vertex into two or more vertices, each an
//               exact copy of the original and in the same vertex
//               pool.  See the more detailed comments in
//               eggUtilities.I.
////////////////////////////////////////////////////////////////////
template<class FunctionObject>
void
split_vertex(EggVertex *vert, const FunctionObject &sequence);


#include "eggUtilities.I"

#endif
