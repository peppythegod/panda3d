// Filename: config_physx.h
// Created by:  enn0x (01Sep09)
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

#ifndef CONFIG_PHYSX_H
#define CONFIG_PHYSX_H

#include "pandabase.h"
#include "notifyCategoryProxy.h"
#include "dconfig.h"

NotifyCategoryDecl(physx, EXPCL_PANDAPHYSX, EXPTP_PANDAPHYSX);

extern EXPCL_PANDAPHYSX void init_libphysx();

#endif // CONFIG_PHYSX_H
