// Filename: vector_typedWritable.cxx
// Created by:  jason (19Jun00)
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

#include "vector_typedWritable.h"

#define EXPCL EXPCL_PANDA_PUTIL
#define EXPTP EXPTP_PANDA_PUTIL
#define TYPE TypedWritable *
#define NAME vector_typedWritable

#include "vector_src.cxx"

// Tell GCC that we'll take care of the instantiation explicitly here.
#ifdef __GNUC__
#pragma implementation
#endif
