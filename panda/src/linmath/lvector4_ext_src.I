// Filename: lvector4_ext_src.I
// Created by:  rdb (02Jan11)
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


#ifndef CPPPARSER
IMPORT_THIS struct Dtool_PyTypedObject FLOATNAME(Dtool_LVector2);
IMPORT_THIS struct Dtool_PyTypedObject FLOATNAME(Dtool_LVector3);
IMPORT_THIS struct Dtool_PyTypedObject FLOATNAME(Dtool_LVector4);
#endif

////////////////////////////////////////////////////////////////////
//     Function: LVector4::python_repr
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
INLINE void EXT_CONST_METHOD_ARGS(FLOATNAME(LVector4),
python_repr, ostream &out, const string &class_name) {
  out << class_name << "("
      << MAYBE_ZERO(this->_v(0)) << ", "
      << MAYBE_ZERO(this->_v(1)) << ", "
      << MAYBE_ZERO(this->_v(2)) << ", "
      << MAYBE_ZERO(this->_v(3)) << ")";
}

////////////////////////////////////////////////////////////////////
//     Function: LVector4::__getattr__
//       Access: Published
//  Description: This is used to implement swizzle masks.
////////////////////////////////////////////////////////////////////
INLINE PyObject *EXT_CONST_METHOD_ARGS(FLOATNAME(LVector4),
__getattr__, const string &attr_name) {
  // Validate the attribute name.
  for (string::const_iterator it = attr_name.begin(); it < attr_name.end(); it++) {
    if (*it < 'w' || *it > 'z') {
      return NULL;
    }
  }

  if (attr_name.size() == 1) {
    if (attr_name[0] == 'w') {
      return PyFloat_FromDouble(this->_v(3));
    } else {
      return PyFloat_FromDouble(this->_v(attr_name[0] - 'x'));
    }

  } else if (attr_name.size() == 2) {
    FLOATNAME(LVector2) *vec = new FLOATNAME(LVector2);
    vec->_v(0) = this->_v((attr_name[0] == 'w') ? 3 : attr_name[0] - 'x');
    vec->_v(1) = this->_v((attr_name[1] == 'w') ? 3 : attr_name[1] - 'x');
    return DTool_CreatePyInstance((void *)vec, FLOATNAME(Dtool_LVector2), true, false);

  } else if (attr_name.size() == 3) {
    FLOATNAME(LVector3) *vec = new FLOATNAME(LVector3);
    vec->_v(0) = this->_v((attr_name[0] == 'w') ? 3 : attr_name[0] - 'x');
    vec->_v(1) = this->_v((attr_name[1] == 'w') ? 3 : attr_name[1] - 'x');
    vec->_v(2) = this->_v((attr_name[2] == 'w') ? 3 : attr_name[2] - 'x');
    return DTool_CreatePyInstance((void *)vec, FLOATNAME(Dtool_LVector3), true, false);

  } else if (attr_name.size() == 4) {
    FLOATNAME(LVector4) *vec = new FLOATNAME(LVector4);
    vec->_v(0) = this->_v((attr_name[0] == 'w') ? 3 : attr_name[0] - 'x');
    vec->_v(1) = this->_v((attr_name[1] == 'w') ? 3 : attr_name[1] - 'x');
    vec->_v(2) = this->_v((attr_name[2] == 'w') ? 3 : attr_name[2] - 'x');
    vec->_v(3) = this->_v((attr_name[3] == 'w') ? 3 : attr_name[3] - 'x');
    return DTool_CreatePyInstance((void *)vec, FLOATNAME(Dtool_LVector4), true, false);
  }

  return NULL;
}

int EXT_METHOD_ARGS(FLOATNAME(LVecBase4), __setattr__, PyObject*, const string&, PyObject*);

////////////////////////////////////////////////////////////////////
//     Function: LVector4::__setattr__
//       Access: Published
//  Description: This is used to implement write masks.
////////////////////////////////////////////////////////////////////
INLINE int EXT_METHOD_ARGS(FLOATNAME(LVector4),
__setattr__, PyObject *self, const string &attr_name, PyObject *assign) {
  return CALL_EXT_METHOD(FLOATNAME(LVecBase4), __setattr__, this, self, attr_name, assign);
}

