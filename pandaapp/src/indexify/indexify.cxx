// Filename: indexify.cxx
// Created by:  drose (03Apr02)
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

#include "indexify.h"
#include "rollDirectory.h"
#include "notify.h"
#include "pnmTextMaker.h"
#include "default_font.h"
#include "default_index_icons.h"
#include "indexParameters.h"
#include "string_utils.h"

////////////////////////////////////////////////////////////////////
//     Function: Indexify::Constructor
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
Indexify::
Indexify() {
  clear_runlines();
  add_runline("[opts] roll1-dir roll2-dir [roll3-dir ...]");

  set_program_description
    ("This program reads a collection of directories containing photo "
     "archives (typically JPEG files), and will generate a number of "
     "thumbnail images and a series of HTML pages to browse them.  It is "
     "especially useful in preparation for burning the photo archives to "
     "CD.\n\n"

     "A number of directories is named on the command line; each "
     "directory must contain a number of image files, and all directories "
     "should be within the same parent directory.  Each directory is "
     "considered a \"roll\", which may or may not correspond to a physical "
     "roll of film, and the photos within each directory are grouped "
     "correspondingly on the generated HTML pages.\n\n"

     "If a file exists by the same name as an image file but with the "
     "extension \"cm\", that file is taken to be a HTML comment about that "
     "particular image and is inserted the HTML page for that image.  "
     "Similarly, if there is a file within a roll directory with the same "
     "name as the directory itself (but with the extension \"cm\"), that file "
     "is inserted into the front page to introduce that particular roll.\n\n"

     "Normally, all image files with the specified extension (normally "
     "\"jpg\") within a roll directory are included in the index, and sorted "
     "into alphabetical (or numeric) order.  If you wish to specify a "
     "different order, or use only a subset of the images in a directory, "
     "create a file in the roll directory with the same name as the "
     "directory itself, and the extension \"ls\".  This file should "
     "simply list the filenames (with or without extension) within the "
     "roll directory in the order they should be listed.  If the ls "
     "file exists but is empty, it indicates that the files should be "
     "listed in reverse order, as from a camera that loads its film "
     "upside-down.");

  add_option
    ("t", "title", 0,
     "Specifies the title to give to the front HTML page.",
     &Indexify::dispatch_string, NULL, &_front_title);

  add_option
    ("a", "archive-dir", 0,
     "Write the generated files to the indicated directory, instead of "
     "the directory above roll1-dir.",
     &Indexify::dispatch_filename, NULL, &_archive_dir);

  add_option
    ("r", "relative-dir", 0,
     "When -a is specifies to place the generate html files in a directory "
     "other than the one above the actual roll directories, you may need "
     "to specify how the html files will address the roll directories.  This "
     "parameter specifies the relative path to the directory above the roll "
     "directories, from the directory named by -a.",
     &Indexify::dispatch_filename, NULL, &_roll_dir_root);

  add_option
    ("f", "", 0,
     "Forces the regeneration of all reduced and thumbnail images, even if "
     "image files already exist that seem to be newer than the source "
     "image files.",
     &Indexify::dispatch_none, &force_regenerate);

  add_option
    ("r", "", 0,
     "Specifies that roll directory names are encoded using the Rose "
     "convention of six digits: mmyyss, where mm and yy are the month and "
     "year, and ss is a sequence number of the roll within the month.  This "
     "name will be reformatted to m-yy/s for output.",
     &Indexify::dispatch_none, &format_rose);

  add_option
    ("d", "", 0,
     "Run in \"dummy\" mode; don't load any images, but instead just "
     "draw an empty box indicating where the thumbnails will be.",
     &Indexify::dispatch_none, &dummy_mode);

  add_option
    ("slide", "", 0,
     "Draw a frame, like a slide mount, around each thumbnail image.",
     &Indexify::dispatch_none, &draw_frames);

  add_option
    ("e", "extension", 0,
     "Specifies the filename extension (without a leading dot) to identify "
     "photo files within the roll directories.  This is normally jpg.",
     &Indexify::dispatch_string, NULL, &_photo_extension);

  add_option
    ("i", "", 0,
     "Indicates that default navigation icon images should be generated "
     "into a directory called \"icons\" which will be created within the "
     "directory named by -a.  This is meaningful only if -prev, -next, and "
     "-up are not explicitly specified.",
     &Indexify::dispatch_none, &_generate_icons);

  add_option
    ("omit-rh", "", 0,
     "Omits roll headers introducing each roll directory, including any "
     "headers defined in roll.cm files.",
     &Indexify::dispatch_none, &omit_roll_headers);

  add_option
    ("omit-full", "", 0,
     "Omits links to the full-size images.",
     &Indexify::dispatch_none, &omit_full_links);

  add_option
    ("caption", "size[,spacing]", 0,
     "Specifies the font size in pixels of the thumbnail captions.  If the "
     "optional spacing parameter is included, it is the number of pixels "
     "below each thumbnail that the caption should be placed.  Specify "
     "-caption 0 to disable thumbnail captions.",
     &Indexify::dispatch_caption, NULL);

  add_option
    ("fnum", "", 0,
     "Writes the frame number of each thumbnail image into the caption "
     "on the index page, instead of the image filename.  This only works "
     "if the photo image filenames consist of the roll directory name "
     "concatenated with a frame number.",
     &Indexify::dispatch_none, &caption_frame_numbers);

  add_option
    ("font", "fontname", 0,
     "Specifies the filename of the font to use to generate the thumbnail "
     "captions.",
     &Indexify::dispatch_filename, NULL, &_font_filename);

  add_option
    ("fontaa", "factor", 0,
     "Specifies a scale factor to apply to the fonts used for captioning "
     "when generating text for the purpose of antialiasing the fonts a "
     "little better than FreeType can do by itself.  The letters are "
     "generated large and then scaled to their proper size.  Normally this "
     "should be a number in the range 3 to 4 for best effect.",
     &Indexify::dispatch_double, NULL, &_font_aa_factor);

  add_option
    ("thumb", "x,y", 0,
     "Specifies the size in pixels of the thumbnail images.",
     &Indexify::dispatch_int_pair, NULL, &thumb_width);

  add_option
    ("reduced", "x,y", 0,
     "Specifies the size in pixels of reduced images (images presented after "
     "the first click on a thumbnail).",
     &Indexify::dispatch_int_pair, NULL, &reduced_width);

  add_option
    ("space", "x,y", 0,
     "Specifies the x,y spacing between thumbnail images, in pixels.",
     &Indexify::dispatch_int_pair, NULL, &thumb_x_space);

  add_option
    ("index", "x,y", 0,
     "Specifies the size in pixels of the index images (the images that "
     "contain an index of thumbnails).",
     &Indexify::dispatch_int_pair, NULL, &max_index_width);

  add_option
    ("prev", "filename", 0,
     "Specifies the relative pathname from the archive directory (or "
     "absolute pathname) to the \"previous\" photo icon.",
     &Indexify::dispatch_filename, NULL, &prev_icon);

  add_option
    ("next", "filename", 0,
     "Specifies the relative pathname from the archive directory (or "
     "absolute pathname) to the \"next\" photo icon.",
     &Indexify::dispatch_filename, NULL, &next_icon);

  add_option
    ("up", "filename", 0,
     "Specifies the relative pathname from the archive directory (or "
     "absolute pathname) to the \"up\" photo icon.",
     &Indexify::dispatch_filename, NULL, &up_icon);

  _photo_extension = "jpg";
  _text_maker = (PNMTextMaker *)NULL;
  _font_aa_factor = 4.0;
}

////////////////////////////////////////////////////////////////////
//     Function: Indexify::Destructor
//       Access: Public, Virtual
//  Description:
////////////////////////////////////////////////////////////////////
Indexify::
~Indexify() {
  RollDirs::iterator di;
  for (di = _roll_dirs.begin(); di != _roll_dirs.end(); ++di) {
    RollDirectory *roll_dir = (*di);
    delete roll_dir;
  }

  if (_text_maker != (PNMTextMaker *)NULL) {
    delete _text_maker;
  }
}

////////////////////////////////////////////////////////////////////
//     Function: Indexify::handle_args
//       Access: Protected, Virtual
//  Description: Does something with the additional arguments on the
//               command line (after all the -options have been
//               parsed).  Returns true if the arguments are good,
//               false otherwise.
////////////////////////////////////////////////////////////////////
bool Indexify::
handle_args(ProgramBase::Args &args) {
  if (args.empty()) {
    nout << "You must specify the roll directories containing archive photos on the command line.\n";
    return false;
  }

  RollDirectory *prev_roll_dir = (RollDirectory *)NULL;
  Args::const_iterator ai;
  for (ai = args.begin(); ai != args.end(); ++ai) {
    Filename filename = Filename::from_os_specific(*ai);
    filename.standardize();
    if (filename.is_directory()) {
      string basename = filename.get_basename();
      if (basename == "icons" || basename == "html" || basename == "reduced") {
	nout << "Ignoring " << filename << "; indexify-generated directory.\n";

      } else {
	RollDirectory *roll_dir = new RollDirectory(filename);
	if (prev_roll_dir != (RollDirectory *)NULL) {
	  roll_dir->_prev = prev_roll_dir;
	  prev_roll_dir->_next = roll_dir;
	}
	
	_roll_dirs.push_back(roll_dir);
	prev_roll_dir = roll_dir;
      }

    } else if (filename.exists()) {
      nout << "Ignoring " << filename << "; not a directory.\n";

    } else {
      nout << filename << " does not exist.\n";
      return false;
    }
  }

  return true;
}

////////////////////////////////////////////////////////////////////
//     Function: Indexify::post_command_line
//       Access: Protected, Virtual
//  Description: This is called after the command line has been
//               completely processed, and it gives the program a
//               chance to do some last-minute processing and
//               validation of the options and arguments.  It should
//               return true if everything is fine, false if there is
//               an error.
////////////////////////////////////////////////////////////////////
bool Indexify::
post_command_line() {
  if (_roll_dirs.empty()) {
    nout << "No roll directories.\n";
    return false;
  }

  if (_archive_dir.empty()) {
    // Choose a default archive directory, above the first roll directory.
    _archive_dir = _roll_dirs.front()->get_dir().get_dirname();
    if (_archive_dir.empty()) {
      _archive_dir = ".";
    }
  }
  _archive_dir.standardize();

  if (!_roll_dir_root.empty()) {
    _roll_dir_root.standardize();
  }

  if (_front_title.empty()) {
    // Supply a default title.
    if (_roll_dirs.size() == 1) {
      _front_title = _roll_dirs.front()->get_name();
    } else {
      _front_title = _roll_dirs.front()->get_name() + " to " + _roll_dirs.back()->get_name();
    }
  }
    
  if (caption_font_size != 0) {
    if (!_font_filename.empty()) {
      _text_maker = new PNMTextMaker(_font_filename, 0);
      if (!_text_maker->is_valid()) {
	delete _text_maker;
	_text_maker = (PNMTextMaker *)NULL;
      }
    }
    
    if (_text_maker == (PNMTextMaker *)NULL) {
      _text_maker = new PNMTextMaker(default_font, default_font_size, 0);
      if (!_text_maker->is_valid()) {
	nout << "Unable to open default font.\n";
	delete _text_maker;
	_text_maker = (PNMTextMaker *)NULL;
      }
    }
    
    if (_text_maker != (PNMTextMaker *)NULL) {
      _text_maker->set_pixel_size(caption_font_size, _font_aa_factor);
      _text_maker->set_align(PNMTextMaker::A_center);
    }
  }

  if (_generate_icons) {
    if (prev_icon.empty()) {
      prev_icon = Filename("icons", default_left_icon_filename);
      Filename icon_filename(_archive_dir, prev_icon);

      if (force_regenerate || !icon_filename.exists()) {
	nout << "Generating " << icon_filename << "\n";
	icon_filename.make_dir();
	icon_filename.set_binary();

	ofstream output;
	if (!icon_filename.open_write(output)) {
	  nout << "Unable to write to " << icon_filename << "\n";
	  exit(1);
	}
	output.write(default_left_icon, default_left_icon_size);
      }
    }
    if (next_icon.empty()) {
      next_icon = Filename("icons", default_right_icon_filename);
      Filename icon_filename(_archive_dir, next_icon);
      if (force_regenerate || !icon_filename.exists()) {
	nout << "Generating " << icon_filename << "\n";
	icon_filename.make_dir();
	icon_filename.set_binary();
	
	ofstream output;
	if (!icon_filename.open_write(output)) {
	  nout << "Unable to write to " << icon_filename << "\n";
	  exit(1);
	}
	output.write(default_right_icon, default_right_icon_size);
      }
    }
    if (up_icon.empty()) {
      up_icon = Filename("icons", default_up_icon_filename);
      Filename icon_filename(_archive_dir, up_icon);
      if (force_regenerate || !icon_filename.exists()) {
	nout << "Generating " << icon_filename << "\n";
	icon_filename.make_dir();
	icon_filename.set_binary();
      
	ofstream output;
	if (!icon_filename.open_write(output)) {
	  nout << "Unable to write to " << icon_filename << "\n";
	  exit(1);
	}
	output.write(default_up_icon, default_up_icon_size);
      }
    }
  }

  finalize_parameters();


  return ProgramBase::post_command_line();
}


////////////////////////////////////////////////////////////////////
//     Function: Indexify::dispatch_caption
//       Access: Protected, Static
//  Description: Dispatch function for the -caption parameter, which
//               takes either one or two numbers separated by a comma,
//               representing the caption font size and the optional
//               pixel spacing of the caption under the image.
////////////////////////////////////////////////////////////////////
bool Indexify::
dispatch_caption(const string &opt, const string &arg, void *) {
  vector_string words;
  tokenize(arg, words, ",");

  int caption_spacing = 0;

  bool okflag = false;
  if (words.size() == 1) {
    okflag =
      string_to_int(words[0], caption_font_size);

  } else if (words.size() == 2) {
    okflag =
      string_to_int(words[0], caption_font_size) &&
      string_to_int(words[1], caption_spacing);
  }

  if (!okflag) {
    nout << "-" << opt
         << " requires one or two integers separated by a comma.\n";
    return false;
  }

  thumb_caption_height = caption_font_size + caption_spacing;

  return true;
}

////////////////////////////////////////////////////////////////////
//     Function: Indexify::run
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
void Indexify::
run() {
  bool all_ok = true;

  RollDirs::iterator di;
  for (di = _roll_dirs.begin(); di != _roll_dirs.end(); ++di) {
    RollDirectory *roll_dir = (*di);
    if (!roll_dir->scan(_photo_extension)) {
      nout << "Unable to read " << *roll_dir << "\n";
      all_ok = false;
    }
    roll_dir->collect_index_images();
  }

  if (!all_ok) {
    exit(1);
  }

  // First, generate all the images.
  for (di = _roll_dirs.begin(); di != _roll_dirs.end(); ++di) {
    RollDirectory *roll_dir = (*di);
    if (!roll_dir->generate_images(_archive_dir, _text_maker)) {
      nout << "Failure.\n";
      exit(1);
    }
  }

  // Then go back and generate the HTML.

  Filename html_filename(_archive_dir, "index.htm");
  nout << "Generating " << html_filename << "\n";
  html_filename.set_text();
  ofstream root_html;
  if (!html_filename.open_write(root_html)) {
    nout << "Unable to write to " << html_filename << "\n";
    exit(1);
  }

  root_html
    << "<html>\n"
    << "<head>\n"
    << "<title>" << _front_title << "</title>\n"
    << "</head>\n"
    << "<body>\n"
    << "<h1>" << _front_title << "</h1>\n";

  for (di = _roll_dirs.begin(); di != _roll_dirs.end(); ++di) {
    RollDirectory *roll_dir = (*di);
    if (!roll_dir->generate_html(root_html, _archive_dir, _roll_dir_root)) {
      nout << "Failure.\n";
      exit(1);
    }
  }

  root_html
    << "</body>\n"
    << "</html>\n";
}


int main(int argc, char *argv[]) {
  Indexify prog;
  prog.parse_command_line(argc, argv);
  prog.run();
  return 0;
}
