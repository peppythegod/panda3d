// Filename: eggMultiFilter.cxx
// Created by:  drose (02Nov00)
// 
////////////////////////////////////////////////////////////////////

#include "eggMultiFilter.h"

#include <notify.h>
#include <eggData.h>

////////////////////////////////////////////////////////////////////
//     Function: EggMultiFilter::Constructor
//       Access: Public
//  Description: 
////////////////////////////////////////////////////////////////////
EggMultiFilter::
EggMultiFilter(bool allow_empty) : _allow_empty(allow_empty) {
  clear_runlines();
  add_runline("-o output.egg [opts] input.egg");
  add_runline("-d dirname [opts] file.egg [file.egg ...]");
  add_runline("-inplace [opts] file.egg [file.egg ...]");

  add_option
    ("o", "filename", 50,
     "Specify the filename to which the resulting egg file will be written.  "
     "This is only valid when there is only one input egg file on the command "
     "line.  If you want to process multiple files simultaneously, you must "
     "use either -d or -inplace.",
     &EggMultiFilter::dispatch_filename, &_got_output_filename, &_output_filename);

  add_option
    ("d", "dirname", 50, 
     "Specify the name of the directory in which to write the resulting egg "
     "files.  If you are processing only one egg file, this may be omitted "
     "in lieu of the -o option.  If you are processing multiple egg files, "
     "this may be omitted only if you specify -inplace instead.",
     &EggMultiFilter::dispatch_filename, &_got_output_dirname, &_output_dirname);

  add_option
    ("inplace", "", 50, 
     "If this option is given, the input files will be rewritten in place with "
     "the results.  This obviates the need to specify -d for an output "
     "directory; however, it's risky because the original input "
     "files are lost.",
     &EggMultiFilter::dispatch_none, &_inplace);
}


////////////////////////////////////////////////////////////////////
//     Function: EggMultiFilter::handle_args
//       Access: Protected, Virtual
//  Description: Does something with the additional arguments on the
//               command line (after all the -options have been
//               parsed).  Returns true if the arguments are good,
//               false otherwise.
////////////////////////////////////////////////////////////////////
bool EggMultiFilter::
handle_args(ProgramBase::Args &args) {
  if (args.empty()) {
    if (!_allow_empty) {
      nout << "You must specify the egg file(s) to read on the command line.\n";
      return false;
    }
  } else {
    // These only apply if we have specified any egg files.

    if (_got_output_filename && args.size() == 1) {
      if (_got_output_dirname) {
	nout << "Cannot specify both -o and -d.\n";
	return false;
      } else if (_inplace) {
	nout << "Cannot specify both -o and -inplace.\n";
	return false;
      }

    } else {
      if (_got_output_filename) {
	nout << "Cannot use -o when multiple egg files are specified.\n";
	return false;
      }

      if (_got_output_dirname && _inplace) {
	nout << "Cannot specify both -inplace and -d.\n";
	return false;
	
      } else if (!_got_output_dirname && !_inplace) {
	nout << "You must specify either -inplace or -d.\n";
	return false;
      }
    }
  }


  Args::const_iterator ai;
  for (ai = args.begin(); ai != args.end(); ++ai) {
    EggData *data = read_egg(*ai);
    if (data == (EggData *)NULL) {
      // Rather than returning false, we simply exit here, so the
      // ProgramBase won't try to tell the user how to run the program
      // just because we got a bad egg file.
      exit(1);
    }

    _eggs.push_back(data);
  }

  return true;
}

////////////////////////////////////////////////////////////////////
//     Function: EggMultiFilter::post_command_line
//       Access: Protected, Virtual
//  Description: 
////////////////////////////////////////////////////////////////////
bool EggMultiFilter::
post_command_line() {
  Eggs::iterator ei;
  for (ei = _eggs.begin(); ei != _eggs.end(); ++ei) {
    EggData *data = (*ei);
    if (_got_coordinate_system) {
      data->set_coordinate_system(_coordinate_system);
    }
    append_command_comment(*data);
  }
  
  return EggMultiBase::post_command_line();
}

////////////////////////////////////////////////////////////////////
//     Function: EggMultiFilter::write_eggs
//       Access: Protected
//  Description: Writes out all of the egg files in the _eggs vector,
//               to the output directory if one is specified, or over
//               the input files if -inplace was specified.
////////////////////////////////////////////////////////////////////
void EggMultiFilter::
write_eggs() {
  Eggs::iterator ei;
  for (ei = _eggs.begin(); ei != _eggs.end(); ++ei) {
    EggData *data = (*ei);
    Filename filename = data->get_egg_filename();

    if (_got_output_filename) {
      nassertv(!_inplace && !_got_output_dirname && _eggs.size() == 1);
      filename = _output_filename;

    } else if (_got_output_dirname) {
      nassertv(!_inplace);
      filename.set_dirname(_output_dirname);

    } else {
      nassertv(_inplace);
    }

    nout << "Writing " << filename << "\n";
    if (!data->write_egg(filename)) {
      // Error writing an egg file; abort.
      exit(1);
    }
  }
}
