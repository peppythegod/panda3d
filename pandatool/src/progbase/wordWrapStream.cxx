// Filename: wordWrapStream.cxx
// Created by:  drose (28Jun00)
// 
////////////////////////////////////////////////////////////////////

#include "wordWrapStream.h"


////////////////////////////////////////////////////////////////////
//     Function: WordWrapStream::Constructor
//       Access: Public
//  Description: 
////////////////////////////////////////////////////////////////////
WordWrapStream::
WordWrapStream(ProgramBase *program) : 
  ostream(&_lsb),
  _lsb(this, program) 
{
}
