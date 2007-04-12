// Filename: copyOnWritePointer.cxx
// Created by:  drose (09Apr07)
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

#include "copyOnWritePointer.h"
#include "mutexHolder.h"
#include "config_util.h"

////////////////////////////////////////////////////////////////////
//     Function: CopyOnWritePointer::get_read_pointer
//       Access: Public
//  Description: Returns a pointer locked for read.  Until this
//               pointer dereferences, calls to get_write_pointer()
//               will force a copy.
////////////////////////////////////////////////////////////////////
CPT(CopyOnWriteObject) CopyOnWritePointer::
get_read_pointer() const {
  if (_object == (CopyOnWriteObject *)NULL) {
    return NULL;
  }

  MutexHolder holder(_object->_lock_mutex);
  while (_object->_lock_status == CopyOnWriteObject::LS_locked_write) {
    _object->_lock_cvar.wait();
  }

  _object->_lock_status = CopyOnWriteObject::LS_locked_read;
  return _object;
}

////////////////////////////////////////////////////////////////////
//     Function: CopyOnWritePointer::get_write_pointer
//       Access: Public
//  Description: Returns a pointer locked for write.  If another
//               thread or threads already hold the pointer locked for
//               read, then this will force a copy.
//
//               Until this pointer dereferences, calls to
//               get_read_pointer() or get_write_pointer() will block.
////////////////////////////////////////////////////////////////////
PT(CopyOnWriteObject) CopyOnWritePointer::
get_write_pointer() {
  if (_object == (CopyOnWriteObject *)NULL) {
    return NULL;
  }

  MutexHolder holder(_object->_lock_mutex);
  while (_object->_lock_status == CopyOnWriteObject::LS_locked_write) {
    _object->_lock_cvar.wait();
  }

  if (_object->_lock_status == CopyOnWriteObject::LS_locked_read) {
    // Someone else has a read copy of this pointer; we need to make
    // our own writable copy.
    nassertr(_object->get_ref_count() > _object->get_cache_ref_count(), NULL);
    if (util_cat.is_debug()) {
      util_cat.debug()
        << "Making copy of " << _object->get_type()
        << " because it is locked in read mode.\n";
    }
    PT(CopyOnWriteObject) new_object = _object->make_cow_copy();
    cache_unref_delete(_object);
    _object = new_object;
    _object->cache_ref();

  } else if (_object->get_cache_ref_count() > 1) {
    // No one else has it specifically read-locked, but there are
    // other CopyOnWritePointers holding the same object, so we should
    // make our own writable copy anyway.
    nassertr(_object->get_ref_count() == _object->get_cache_ref_count(), NULL);
    if (util_cat.is_debug()) {
      util_cat.debug()
        << "Making copy of " << _object->get_type()
        << " because it is shared by " << _object->get_ref_count()
        << " pointers.\n";
    }

    PT(CopyOnWriteObject) new_object = _object->make_cow_copy();
    cache_unref_delete(_object);
    _object = new_object;
    _object->cache_ref();

  } else {
    // No other thread has the pointer locked, and we're the only
    // CopyOnWritePointer with this object.  We can safely write to it
    // without making a copy.
    nassertr(_object->get_ref_count() == _object->get_cache_ref_count(), NULL);
  }
  _object->_lock_status = CopyOnWriteObject::LS_locked_write;

  return _object;
}
