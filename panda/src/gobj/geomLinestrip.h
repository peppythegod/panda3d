// Filename: geomLinestrip.h
// Created by:  charles (13Jul00)
// 
////////////////////////////////////////////////////////////////////

#ifndef GEOMLINESTRIP_H
#define GEOMLINESTRIP_H

#include "geom.h"

////////////////////////////////////////////////////////////////////
//       Class : GeomLinestrip
// Description : Linestrip Primitive
////////////////////////////////////////////////////////////////////
class EXPCL_PANDA GeomLinestrip : public Geom
{
public:
  GeomLinestrip(void) : Geom() { _width = 1.0; }
  virtual Geom *make_copy() const;
  virtual void print_draw_immediate( void ) const { }
  virtual void draw_immediate(GraphicsStateGuardianBase *gsg) const;

  // Undefined for this type of primitive
  virtual int get_num_vertices_per_prim() const {
    return 0;
  }
  virtual int get_num_more_vertices_than_components() const {
    return 1;
  }
  virtual bool uses_components() const {
    return true;
  }

  virtual int get_length(int prim) const {
    return _primlengths[prim];
  }

  virtual Geom *explode() const;

  INLINE void set_width(float width) { _width = width; }
  INLINE float get_width(void) const { return _width; }

protected:
  float _width;

public:
  static void register_with_read_factory(void);
  virtual void write_datagram(BamWriter* manager, Datagram &me);  

  static TypedWriteable *make_GeomLinestrip(const FactoryParams &params);

protected:
  void fillin(DatagramIterator& scan, BamReader* manager);

public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    Geom::init_type();
    register_type(_type_handle, "GeomLinestrip",
                  Geom::get_class_type());
  }
  virtual TypeHandle get_type() const {
    return get_class_type();
  }
  virtual TypeHandle force_init_type() {init_type(); return get_class_type();}

private:
  static TypeHandle _type_handle;
};

#endif // GEOMLINESTRIP_H
