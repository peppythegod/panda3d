//
// genPyCode.pp
//
// This file defines the script to auto-generate a sensible genPyCode
// for the user based on the Config.pp variables in effect at the time
// ppremake is run.  The generated script will know which directories
// to generate its output to, as well as which source files to read
// for the input.
//

#define hash #

#define install_dir $[$[upcase $[PACKAGE]]_INSTALL]
#define install_lib_dir $[or $[INSTALL_LIB_DIR],$[install_dir]/lib]
#define install_bin_dir $[or $[INSTALL_BIN_DIR],$[install_dir]/bin]
#define install_igatedb_dir $[or $[INSTALL_IGATEDB_DIR],$[install_dir]/etc]

// If we're on Win32 without Cygwin, generate a genPyCode.bat file;
// for all other platforms, generate a genPyCode sh script.  Although
// it's true that on non-Win32 platforms we don't need the script
// (since the python file itself could be made directly executable),
// we generate the script anyway to be consistent with Win32, which
// does require it.

#if $[eq $[PLATFORM],Win32]

#output genPyCode.bat
@echo off
python $[osfilename $[install_bin_dir]/genPyCode.py]
#end genPyCode.bat

#else  // Win32

#output genPyCode
$[hash]! /bin/sh
python $[install_bin_dir]/genPyCode.py
#end genPyCode

#endif  // Win32

#output genPyCode.py
$[hash]! /usr/bin/env python

import os
import sys
import glob

#if $[CTPROJS]
# This script was generated while the user was using the ctattach
# tools.  That had better still be the case.

ctprojs = os.getenv('CTPROJS')
if not ctprojs:
    print "You are no longer attached to any trees!"
    sys.exit(1)
    
directDir = os.getenv('DIRECT')
if not directDir:
    print "You are not attached to DIRECT!"
    sys.exit(1)

# Make sure that direct/src/showbase/sitecustomize.py gets loaded.
parent, base = os.path.split(directDir)

if parent not in sys.path:
    sys.path.append(parent)

import direct.showbase.sitecustomize

#endif

from direct.ffi import DoGenPyCode
from direct.ffi import FFIConstants

# The following parameters were baked in to this script at the time
# ppremake was run in Direct.
DoGenPyCode.outputDir = '$[osfilename $[install_lib_dir]/pandac]'
DoGenPyCode.extensionsDir = '$[osfilename $[TOPDIR]/src/extensions]'
DoGenPyCode.interrogateLib = 'libdtoolconfig'
DoGenPyCode.codeLibs = '$[GENPYCODE_LIBS]'.split()
DoGenPyCode.etcPath = ['$[osfilename $[install_igatedb_dir]]']

#if $[>= $[OPTIMIZE], 4]
FFIConstants.wantComments = 0
FFIConstants.wantTypeChecking = 0
#endif

#if $[CTPROJS]

# Actually, the user is expected to be using ctattach, so never mind
# on the baked-in stuff--replace it with the dynamic settings from
# ctattach.
DoGenPyCode.outputDir = os.path.join(directDir, 'lib', 'pandac')
DoGenPyCode.extensionsDir = os.path.join(directDir, 'src', 'extensions')
DoGenPyCode.etcPath = []

# Look for additional packages (other than the basic three)
# that the user might be dynamically attached to.
packages = []
for proj in ctprojs.split():
    projName = proj.split(':')[0]
    packages.append(projName)
packages.reverse()

for package in packages:
    packageDir = os.getenv(package)
    if packageDir:
        etcDir = os.path.join(packageDir, 'etc')
        try:
            inFiles = glob.glob(os.path.join(etcDir, '*.in'))
        except:
            inFiles = []
        if inFiles:
            DoGenPyCode.etcPath.append(etcDir)

        if package not in ['DTOOL', 'DIRECT', 'PANDA']:
            libDir = os.path.join(packageDir, 'lib')
            try:
                files = os.listdir(libDir)
            except:
                files = []
            for file in files:
                if os.path.isfile(os.path.join(libDir, file)):
                    basename, ext = os.path.splitext(file)

                    # Try to import the library.  If we can import it,
                    # instrument it.
                    try:
                        __import__(basename, globals(), locals())
                        isModule = True
                    except:
                        isModule = False

                    if isModule:
                        if basename not in DoGenPyCode.codeLibs:
                            DoGenPyCode.codeLibs.append(basename)
#endif

DoGenPyCode.run()

#end genPyCode.py
