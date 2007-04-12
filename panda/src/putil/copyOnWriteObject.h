// Filename: copyOnWriteObject.h
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

#ifndef COPYONWRITEOBJECT_H
#define COPYONWRITEOBJECT_H

#include "pandabase.h"

#include "cachedTypedWritableReferenceCount.h"
#include "pmutex.h"
#include "conditionVar.h"
#include "mutexHolder.h"

////////////////////////////////////////////////////////////////////
//       Class : CopyOnWriteObject
// Description : This base class provides basic reference counting,
//               but also can be used with a CopyOnWritePointer to
//               provide get_read_pointer() and get_write_pointer().
////////////////////////////////////////////////////////////////////
class EXPCL_PANDA CopyOnWriteObject : public CachedTypedWritableReferenceCount {
public:
  INLINE CopyOnWriteObject();
  INLINE CopyOnWriteObject(const CopyOnWriteObject &copy);
  INLINE void operator = (const CopyOnWriteObject &copy);

PUBLISHED:
  bool unref() const;
  INLINE void cache_ref() const;

protected:
  virtual PT(CopyOnWriteObject) make_cow_copy()=0;

private:
  enum LockStatus {
    LS_unlocked,
    LS_locked_read,
    LS_locked_write,
  };
  Mutex _lock_mutex;
  ConditionVar _lock_cvar;
  LockStatus _lock_status;

public:
  virtual TypeHandle get_type() const {
    return get_class_type();
  }
  virtual TypeHandle force_init_type() {init_type(); return get_class_type();}

PUBLISHED:
  static TypeHandle get_class_type() {
    return _type_handle;
  }

public:
  static void init_type() {
    CachedTypedWritableReferenceCount::init_type();
    register_type(_type_handle, "CopyOnWriteObject",
                  CachedTypedWritableReferenceCount::get_class_type());
  }

private:
  static TypeHandle _type_handle;

  friend class CopyOnWritePointer;
};

////////////////////////////////////////////////////////////////////
//       Class : CopyOnWriteObj
// Description : This is similar to RefCountObj, but it implements a
//               CopyOnWriteObject inheritance instead of a
//               ReferenceCount inheritance.
////////////////////////////////////////////////////////////////////
template<class Base>
class CopyOnWriteObj : public CopyOnWriteObject, public Base {
public:
  INLINE CopyOnWriteObj();
  INLINE CopyOnWriteObj(const Base &copy);
  INLINE CopyOnWriteObj(const CopyOnWriteObj<Base> &copy);

protected:
  virtual PT(CopyOnWriteObject) make_cow_copy();

public:
  virtual TypeHandle get_type() const {
    return get_class_type();
  }
  virtual TypeHandle force_init_type() {init_type(); return get_class_type();}

PUBLISHED:
  static TypeHandle get_class_type() {
    return _type_handle;
  }

public:
  static void init_type();

private:
  static TypeHandle _type_handle;
};

#include "copyOnWriteObject.I"

#endif
