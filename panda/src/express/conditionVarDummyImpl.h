// Filename: conditionVarDummyImpl.h
// Created by:  drose (09Aug02)
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

#ifndef CONDITIONVARDUMMYIMPL_H
#define CONDITIONVARDUMMYIMPL_H

#include "pandabase.h"
#include "selectIpcImpl.h"

#ifdef IPC_DUMMY_IMPL

#include "notify.h"

class MutexDummyImpl;

////////////////////////////////////////////////////////////////////
//       Class : ConditionVarDummyImpl
// Description : A fake condition variable implementation for
//               single-threaded applications that don't need any
//               synchronization control.
////////////////////////////////////////////////////////////////////
class EXPCL_PANDAEXPRESS ConditionVarDummyImpl {
public:
  INLINE ConditionVarDummyImpl(MutexDummyImpl &mutex);
  INLINE ~ConditionVarDummyImpl();

  INLINE void wait();
  INLINE void signal();
  INLINE void signal_all();
};

#include "conditionVarDummyImpl.I"

#endif  // IPC_DUMMY_IMPL

#endif
