// Filename: physxManager.h
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

#include "config_physx.h"
#include "pandaSystem.h"

//#include "physxScene.h"
//#...

Configure(config_physx);
NotifyCategoryDef(physx,"");

ConfigureFn(config_physx) {
  init_libphysx();
}

////////////////////////////////////////////////////////////////////
//     Function: init_libphysx
//  Description: Initializes the library.  This must be called at
//               least once before any of the functions or classes in
//               this library can be used.  Normally it will be
//               called by the static initializers and need not be
//               called explicitly, but special cases exist.
////////////////////////////////////////////////////////////////////
void
init_libphysx() {
  static bool initialized = false;
  if (initialized) {
    return;
  }
  initialized = true;

  //PhysxScene::init_type();
  //...

  PandaSystem *ps = PandaSystem::get_global_ptr();
  ps->add_system( "PhysX" );
}

