// This directory is still experimental.  Define HAVE_P3D_PLUGIN in
// your Config.pp to build it.
#define BUILD_DIRECTORY $[and $[HAVE_P3D_PLUGIN],$[HAVE_TINYXML],$[HAVE_OPENSSL],$[HAVE_ZLIB]]

#begin lib_target
  #define BUILD_TARGET $[HAVE_JPEG]
  #define USE_PACKAGES tinyxml openssl zlib jpeg x11
  #define TARGET p3d_plugin
  #define LIB_PREFIX

  #define OTHER_LIBS $[if $[OSX_PLATFORM],subprocbuffer]

  #define COMBINED_SOURCES \
    $[TARGET]_composite1.cxx

  #define SOURCES \
    fileSpec.cxx fileSpec.h fileSpec.I \
    find_root_dir.cxx find_root_dir.h \
    get_tinyxml.h \
    binaryXml.cxx binaryXml.h \
    fhandle.h \
    handleStream.cxx handleStream.h handleStream.I \
    handleStreamBuf.cxx handleStreamBuf.h handleStreamBuf.I \
    mkdir_complete.cxx mkdir_complete.h \
    p3d_lock.h p3d_plugin.h \
    p3d_plugin_config.h \
    p3d_plugin_common.h \
    p3dBoolObject.h \
    p3dConcreteSequence.h \
    p3dConcreteStruct.h \
    p3dConditionVar.h p3dConditionVar.I \
    p3dDownload.h p3dDownload.I \
    p3dFileDownload.h p3dFileDownload.I \
    p3dFileParams.h p3dFileParams.I \
    p3dFloatObject.h \
    p3dHost.h p3dHost.I \
    p3dInstance.h p3dInstance.I \
    p3dInstanceManager.h p3dInstanceManager.I \
    p3dIntObject.h \
    p3dMainObject.h \
    p3dMultifileReader.h p3dMultifileReader.I \
    p3dNoneObject.h \
    p3dObject.h p3dObject.I \
    p3dOsxSplashWindow.h p3dOsxSplashWindow.I \
    p3dPackage.h p3dPackage.I \
    p3dPythonObject.h \
    p3dReferenceCount.h p3dReferenceCount.I \
    p3dSession.h p3dSession.I \
    p3dSplashWindow.h p3dSplashWindow.I \
    p3dStringObject.h \
    p3dTemporaryFile.h p3dTemporaryFile.I \
    p3dUndefinedObject.h \
    p3dWinSplashWindow.h p3dWinSplashWindow.I \
    p3dX11SplashWindow.h \
    p3dWindowParams.h p3dWindowParams.I \
    run_p3dpython.h

  #define INCLUDED_SOURCES \
    p3d_plugin.cxx \
    p3dBoolObject.cxx \
    p3dConcreteSequence.cxx \
    p3dConcreteStruct.cxx \
    p3dConditionVar.cxx \
    p3dDownload.cxx \
    p3dFileDownload.cxx \
    p3dFileParams.cxx \
    p3dFloatObject.cxx \
    p3dHost.cxx \
    p3dInstance.cxx \
    p3dInstanceManager.cxx \
    p3dIntObject.cxx \
    p3dMainObject.cxx \
    p3dMultifileReader.cxx \
    p3dNoneObject.cxx \
    p3dObject.cxx \
    p3dOsxSplashWindow.cxx \
    p3dPackage.cxx \
    p3dPythonObject.cxx \
    p3dReferenceCount.cxx \
    p3dSession.cxx \
    p3dSplashWindow.cxx \
    p3dStringObject.cxx \
    p3dTemporaryFile.cxx \
    p3dUndefinedObject.cxx \
    p3dWinSplashWindow.cxx \
    p3dX11SplashWindow.cxx \
    p3dWindowParams.cxx

  #define INSTALL_HEADERS \
    p3d_plugin.h

  #define WIN_SYS_LIBS user32.lib gdi32.lib shell32.lib comctl32.lib

#end lib_target


#begin lib_target
// *****
// Note!  This lib is used to run P3DPythonRun within the parent 
// (browser) process, instead of forking a child.  This seems like
// it's going to be a bad idea in the long term.  This lib remains
// for now as an experiment, but it will likely be removed very soon.
// ****
  #define BUILD_TARGET $[HAVE_PYTHON]
  #define USE_PACKAGES tinyxml python
  #define TARGET libp3dpython
  #define LIB_PREFIX

  #define OTHER_LIBS \
    dtoolutil:c dtoolbase:c dtool:m \
    interrogatedb:c dconfig:c dtoolconfig:m \
    express:c pandaexpress:m \
    prc:c pstatclient:c pandabase:c linmath:c putil:c \
    pipeline:c event:c nativenet:c net:c panda:m

  #define SOURCES \
    binaryXml.cxx binaryXml.h \
    fhandle.h \
    handleStream.cxx handleStream.h handleStream.I \
    handleStreamBuf.cxx handleStreamBuf.h handleStreamBuf.I \
    p3d_lock.h p3d_plugin.h \
    p3d_plugin_config.h \
    p3dCInstance.cxx \
    p3dCInstance.h p3dCInstance.I \
    p3dPythonRun.cxx p3dPythonRun.h p3dPythonRun.I \
    run_p3dpython.h run_p3dpython.cxx

  #define WIN_SYS_LIBS user32.lib
#end lib_target

#begin bin_target
  #define BUILD_TARGET $[HAVE_PYTHON]
  #define USE_PACKAGES tinyxml python
  #define TARGET p3dpython

  #define OTHER_LIBS \
    dtoolutil:c dtoolbase:c dtool:m \
    interrogatedb:c dconfig:c dtoolconfig:m \
    express:c pandaexpress:m \
    prc:c pstatclient:c pandabase:c linmath:c putil:c \
    pipeline:c event:c nativenet:c net:c panda:m

  #define SOURCES \
    binaryXml.cxx binaryXml.h \
    fhandle.h \
    handleStream.cxx handleStream.h handleStream.I \
    handleStreamBuf.cxx handleStreamBuf.h handleStreamBuf.I \
    p3d_lock.h p3d_plugin.h \
    p3d_plugin_config.h \
    p3dCInstance.cxx \
    p3dCInstance.h p3dCInstance.I \
    p3dPythonRun.cxx p3dPythonRun.h p3dPythonRun.I \
    run_p3dpython.h run_p3dpython.cxx

  #define SOURCES $[SOURCES] \
    p3dPythonMain.cxx

  // If you have to link with a static Python library, define it here.
  #define EXTRA_LIBS $[EXTRA_P3DPYTHON_LIBS]

  #define WIN_SYS_LIBS user32.lib
#end bin_target

#begin static_lib_target
  #define TARGET plugin_common
  #define USE_PACKAGES tinyxml openssl

  #define SOURCES \
    load_plugin.cxx load_plugin.h \
    fileSpec.cxx fileSpec.h fileSpec.I \
    find_root_dir.cxx find_root_dir.h \
    is_pathsep.h is_pathsep.I \
    mkdir_complete.cxx mkdir_complete.h

#end static_lib_target

#include $[THISDIRPREFIX]p3d_plugin_config.h.pp