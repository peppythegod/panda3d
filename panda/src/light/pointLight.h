// Filename: pointLight.h
// Created by:  mike (09Jan97)
//
////////////////////////////////////////////////////////////////////
//
#ifndef POINTLIGHT_H
#define POINTLIGHT_H
//
////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////
#include <pandabase.h>

#include <graphicsStateGuardian.h>
#include <luse.h>
#include <map>
#include <namedNode.h>
#include "light.h"

////////////////////////////////////////////////////////////////////
// Defines
////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////
//       Class : PointLight
// Description :
////////////////////////////////////////////////////////////////////
class EXPCL_PANDA PointLight : public Light, public NamedNode
{
  public:

    PointLight( const string& name = "" );
    ~PointLight( void ) { }

    virtual void apply( GraphicsStateGuardian* gsg ) {
      gsg->apply_light( this );
    }

    virtual void output( ostream &out ) const;
    virtual void write( ostream &out, int indent_level = 0 ) const;

    INLINE Colorf get_specular( void ) const;
    INLINE void set_specular( const Colorf& color );

    INLINE float get_constant_attenuation( void ) const;
    INLINE void set_constant_attenuation( float att );

    INLINE float get_linear_attenuation( void ) const;
    INLINE void set_linear_attenuation( float att );

    INLINE float get_quadratic_attenuation( void ) const;
    INLINE void set_quadratic_attenuation( float att );

  protected:

    Colorf				_specular;
    float                               _constant_attenuation;
    float                               _linear_attenuation;
    float                               _quadratic_attenuation;

  public:

    static TypeHandle get_class_type( void ) {
      return _type_handle;
    }
    static void init_type( void ) {
      Light::init_type();
      NamedNode::init_type();
      register_type( _type_handle, "PointLight",
                        Light::get_class_type(),
			NamedNode::get_class_type() );
    }
    virtual TypeHandle get_type( void ) const {
      return get_class_type();
    }
    virtual TypeHandle force_init_type() {init_type(); return get_class_type();}
    virtual TypeHandle get_light_type( void ) const {
      return get_class_type();
    }

  private:

    static TypeHandle                   _type_handle;
};

INLINE ostream &operator << (ostream &out, const PointLight &light) {
  light.output(out);
  return out;
}

#include "pointLight.I"

#endif
