// Filename: eggMiscFuncs.cxx
// Created by:  drose (16Jan99)
// 
////////////////////////////////////////////////////////////////////

#include <pandabase.h>
#include "eggMiscFuncs.h"

#include <indent.h>

#include <ctype.h>


////////////////////////////////////////////////////////////////////
//     Function: enquote_string
//  Description: Writes the string to the indicated output stream.  If
//               the string contains any characters special to egg,
//               writes quotation marks around it.  If always_quote is
//               true, writes quotation marks regardless.
////////////////////////////////////////////////////////////////////
ostream &
enquote_string(ostream &out, const string &str, int indent_level,
	       bool always_quote) {
  indent(out, indent_level);

  // First, see if we need to enquote it.
  bool legal = !always_quote;
  string::const_iterator p;
  for (p = str.begin(); p != str.end() && legal; ++p) {
    legal = (isalnum(*p) || *p=='-' || *p=='_' || *p=='#' || *p=='.');
  }

  if (legal) {
    out << str;

  } else {
    out << '"';
    
    for (p = str.begin(); p != str.end(); ++p) {
      switch (*p) {
      case '"':
	// Can't output nested quote marks at all.
	out << "'";
	break;
	
      case '\n':
	// A newline necessitates ending the quotes, newlining, and
	// beginning again.
	out << "\"\n";
	indent(out, indent_level) << '"';
	break;
	
      default:
	out << *p;
      }
    }
    out << '"';
  }

  return out;
}


////////////////////////////////////////////////////////////////////
//     Function: write_transform
//  Description: A helper function to write out a 3x3 transform
//               matrix.
////////////////////////////////////////////////////////////////////
void
write_transform(ostream &out, const LMatrix3d &mat, int indent_level) {
  indent(out, indent_level) << "<Transform> {\n";

  indent(out, indent_level+2) << "<Matrix3> {\n";

  for (int r = 0; r < 3; r++) {
    indent(out, indent_level+3);
    for (int c = 0; c < 3; c++) {
      out << " " << mat(r, c);
    }
    out << "\n";
  }
  indent(out, indent_level+2) << "}\n";
  indent(out, indent_level) << "}\n";
}

////////////////////////////////////////////////////////////////////
//     Function: write_transform
//  Description: A helper function to write out a 4x4 transform
//               matrix.
////////////////////////////////////////////////////////////////////
void
write_transform(ostream &out, const LMatrix4d &mat, int indent_level) {
  indent(out, indent_level) << "<Transform> {\n";

  bool written = false;

  /*
  int mat_type = mat.getMatType();
  if ((mat_type &
       ~(PFMAT_TRANS | PFMAT_ROT | PFMAT_SCALE | PFMAT_NONORTHO))==0) {
    // Write out the matrix componentwise if possible.
    pfVec3 s, r, t;
    if (ExtractMatrix(mat, s, r, t)) {
      if (!s.almostEqual(pfVec3(1.0, 1.0, 1.0), 0.0001)) {
	Indent(out, indent+2) << "<Scale> { " << s << " }\n";
      }
      if (fabs(r[0]) > 0.0001) {
	Indent(out, indent+2) << "<RotX> { " << r[0] << " }\n";
      }
      if (fabs(r[1]) > 0.0001) {
	Indent(out, indent+2) << "<RotY> { " << r[1] << " }\n";
      }
      if (fabs(r[2]) > 0.0001) {
	Indent(out, indent+2) << "<RotZ> { " << r[2] << " }\n";
      }
      if (!t.almostEqual(pfVec3(0.0, 0.0, 0.0), 0.0001)) {
	Indent(out, indent+2) << "<Translate> { " << t << " }\n";
      }
      written = true;
    }
  }
  */

  if (!written) {
    // It's some non-affine matrix, or it has a shear; write out the
    // general 4x4.
    indent(out, indent_level+2) << "<Matrix4> {\n";

    for (int r = 0; r < 4; r++) {
      indent(out, indent_level+3);
      for (int c = 0; c < 4; c++) {
	out << " " << mat(r, c);
      }
      out << "\n";
    }
    indent(out, indent_level+2) << "}\n";
  }

  indent(out, indent_level) << "}\n";
}
