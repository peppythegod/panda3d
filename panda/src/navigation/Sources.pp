#define OTHER_LIBS interrogatedb:c dconfig:c dtoolconfig:m \
                   dtoolutil:c dtoolbase:c dtool:m prc:c

#begin lib_target
  #define TARGET p3navigation
  #define LOCAL_LIBS pgraph express putil pandabase

  #define COMBINED_SOURCES navigation_composite1.cxx

  #define SOURCES \
    config_navigation.h \
    recastNavMesh.h recastNavMesh.I

  #define INCLUDED_SOURCES \
    config_navigation.cxx \
    recastNavMesh.cxx

  #define INSTALL_HEADERS \
    config_navigation.h \
    recastNavMesh.h recastNavMesh.I

  #define IGATESCAN all

#end lib_target

