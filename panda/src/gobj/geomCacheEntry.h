// Filename: geomCacheEntry.h
// Created by:  drose (21Mar05)
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

#ifndef GEOMCACHEENTRY_H
#define GEOMCACHEENTRY_H

#include "pandabase.h"
#include "geomCacheManager.h"
#include "referenceCount.h"
#include "config_gobj.h"
#include "pointerTo.h"
#include "mutexHolder.h"

class Geom;
class GeomPrimitive;

////////////////////////////////////////////////////////////////////
//       Class : GeomCacheEntry
// Description : This object contains a single cache entry in the
//               GeomCacheManager.  This is actually the base class of
//               any number of individual cache types.
////////////////////////////////////////////////////////////////////
class EXPCL_PANDA_GOBJ GeomCacheEntry : public ReferenceCount {
public:
  INLINE GeomCacheEntry();
  virtual ~GeomCacheEntry();

  PT(GeomCacheEntry) record(Thread *current_thread);
  void refresh(Thread *current_thread);
  PT(GeomCacheEntry) erase();

  virtual void evict_callback();
  virtual void output(ostream &out) const;

private:
  int _last_frame_used;

  INLINE void remove_from_list();
  INLINE void insert_before(GeomCacheEntry *node);

private:  
  GeomCacheEntry *_prev, *_next;

  friend class GeomCacheManager;
};

INLINE ostream &operator << (ostream &out, const GeomCacheEntry &entry);

#include "geomCacheEntry.I"

#endif