// Filename: qpgeom.cxx
// Created by:  drose (06Mar05)
//
////////////////////////////////////////////////////////////////////
//
// PANDA 3D SOFTWARE
// Copyright (c) 2001 - 2004, Disney Enterprises, Inc.  All rights reserved
//
// All use of this software is subject to the terms of the Panda 3d
// Software license.  You should have received a copy of this license
// along with this source code; you will also find a current copy of
// the license at http://etc.cmu.edu/panda3d/docs/license/ .
//
// To contact the maintainers of this program write to
// panda3d-general@lists.sourceforge.net .
//
////////////////////////////////////////////////////////////////////

#include "qpgeom.h"
#include "qpgeomVertexCacheManager.h"
#include "pStatTimer.h"
#include "bamReader.h"
#include "bamWriter.h"

UpdateSeq qpGeom::_next_modified;
TypeHandle qpGeom::_type_handle;

////////////////////////////////////////////////////////////////////
//     Function: qpGeom::Constructor
//       Access: Published
//  Description: 
////////////////////////////////////////////////////////////////////
qpGeom::
qpGeom() {
}

////////////////////////////////////////////////////////////////////
//     Function: qpGeom::Copy Constructor
//       Access: Published
//  Description: 
////////////////////////////////////////////////////////////////////
qpGeom::
qpGeom(const qpGeom &copy) :
  /*
  TypedWritableReferenceCount(copy),
  BoundedObject(copy),
  */
  Geom(copy),
  _cycler(copy._cycler)  
{
}

////////////////////////////////////////////////////////////////////
//     Function: qpGeom::Copy Assignment Operator
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
void qpGeom::
operator = (const qpGeom &copy) {
  /*
  TypedWritableReferenceCount::operator = (copy);
  BoundedObject::operator = (copy);
  */
  clear_cache();
  Geom::operator = (copy);
  _cycler = copy._cycler;
  mark_bound_stale();
}

////////////////////////////////////////////////////////////////////
//     Function: qpGeom::Destructor
//       Access: Published, Virtual
//  Description: 
////////////////////////////////////////////////////////////////////
qpGeom::
~qpGeom() {
  // When we destruct, we should ensure that all of our cached
  // entries, across all pipeline stages, are properly removed from
  // the cache manager.
  qpGeomVertexCacheManager *cache_mgr = 
    qpGeomVertexCacheManager::get_global_ptr();

  int num_stages = _cycler.get_num_stages();
  for (int i = 0; i < num_stages; i++) {
    if (_cycler.is_stage_unique(i)) {
      CData *cdata = _cycler.write_stage(i);
      for (MungedCache::iterator ci = cdata->_munged_cache.begin();
           ci != cdata->_munged_cache.end();
           ++ci) {
        cache_mgr->remove_geom(this, (*ci).first);
      }
      cdata->_munged_cache.clear();
      _cycler.release_write_stage(i, cdata);
    }
  }
}

////////////////////////////////////////////////////////////////////
//     Function: qpGeom::make_copy
//       Access: Published, Virtual
//  Description: Temporarily redefined from Geom base class.
////////////////////////////////////////////////////////////////////
Geom *qpGeom::
make_copy() const {
  return new qpGeom(*this);
}

////////////////////////////////////////////////////////////////////
//     Function: qpGeom::modify_vertex_data
//       Access: Published
//  Description: Returns a modifiable pointer to the GeomVertexData,
//               so that application code may directly maniuplate the
//               geom's underlying data.
////////////////////////////////////////////////////////////////////
PT(qpGeomVertexData) qpGeom::
modify_vertex_data() {
  // Perform copy-on-write: if the reference count on the vertex data
  // is greater than 1, assume some other Geom has the same pointer,
  // so make a copy of it first.
  clear_cache();
  CDWriter cdata(_cycler);
  if (cdata->_data->get_ref_count() > 1) {
    cdata->_data = new qpGeomVertexData(*cdata->_data);
  }
  mark_bound_stale();
  return cdata->_data;
}

////////////////////////////////////////////////////////////////////
//     Function: qpGeom::set_vertex_data
//       Access: Published
//  Description: Replaces the Geom's underlying vertex data table with
//               a completely new table.
////////////////////////////////////////////////////////////////////
void qpGeom::
set_vertex_data(const qpGeomVertexData *data) {
  clear_cache();
  CDWriter cdata(_cycler);
  cdata->_data = (qpGeomVertexData *)data;
  mark_bound_stale();
}

////////////////////////////////////////////////////////////////////
//     Function: qpGeom::add_primitive
//       Access: Published
//  Description: Adds a new GeomPrimitive structure to the Geom
//               object.  This specifies a particular subset of
//               vertices that are used to define geometric primitives
//               of the indicated type.
////////////////////////////////////////////////////////////////////
void qpGeom::
add_primitive(const qpGeomPrimitive *primitive) {
  clear_cache();
  CDWriter cdata(_cycler);
  cdata->_primitives.push_back((qpGeomPrimitive *)primitive);

  if (cdata->_got_usage_hint) {
    cdata->_usage_hint = min(cdata->_usage_hint, primitive->get_usage_hint());
  }
}

////////////////////////////////////////////////////////////////////
//     Function: qpGeom::remove_primitive
//       Access: Published
//  Description: Removes the ith primitive from the list.
////////////////////////////////////////////////////////////////////
void qpGeom::
remove_primitive(int i) {
  clear_cache();
  CDWriter cdata(_cycler);
  nassertv(i >= 0 && i < (int)cdata->_primitives.size());
  if (cdata->_got_usage_hint &&
      cdata->_usage_hint == cdata->_primitives[i]->get_usage_hint()) {
    // Maybe we're raising the minimum usage_hint; we have to rederive
    // the usage_hint later.
    cdata->_got_usage_hint = false;
  }
  cdata->_primitives.erase(cdata->_primitives.begin() + i);
}

////////////////////////////////////////////////////////////////////
//     Function: qpGeom::clear_primitives
//       Access: Published
//  Description: Removes all the primitives from the Geom object (but
//               keeps the same table of vertices).  You may then
//               re-add primitives one at a time via calls to
//               add_primitive().
////////////////////////////////////////////////////////////////////
void qpGeom::
clear_primitives() {
  clear_cache();
  CDWriter cdata(_cycler);
  cdata->_primitives.clear();
}

////////////////////////////////////////////////////////////////////
//     Function: qpGeom::get_num_bytes
//       Access: Published
//  Description: Returns the number of bytes consumed by the geom and
//               its primitives (but not including its vertex table).
////////////////////////////////////////////////////////////////////
int qpGeom::
get_num_bytes() const {
  CDReader cdata(_cycler);

  int num_bytes = sizeof(qpGeom);
  Primitives::const_iterator pi;
  for (pi = cdata->_primitives.begin(); 
       pi != cdata->_primitives.end();
       ++pi) {
    num_bytes += (*pi)->get_num_bytes();
  }

  return num_bytes;
}

////////////////////////////////////////////////////////////////////
//     Function: qpGeom::get_modified
//       Access: Published
//  Description: Returns the maximum UpdateSeq of all the Geom's
//               individual primitives and vertex arrays.  This,
//               therefore, will change only when any part of the Geom
//               changes.
////////////////////////////////////////////////////////////////////
UpdateSeq qpGeom::
get_modified() const {
  CDReader cdata(_cycler);
  UpdateSeq seq;

  Primitives::const_iterator pi;
  for (pi = cdata->_primitives.begin(); 
       pi != cdata->_primitives.end();
       ++pi) {
    UpdateSeq pseq = (*pi)->get_modified();
    if (seq < pseq) {
      seq = pseq;
    }
  }

  int num_arrays = cdata->_data->get_num_arrays();
  for (int i = 0; i < num_arrays; ++i) {
    UpdateSeq aseq = cdata->_data->get_array(i)->get_modified();
    if (seq < aseq) {
      seq = aseq;
    }
  }

  return seq;
}

////////////////////////////////////////////////////////////////////
//     Function: qpGeom::munge_geom
//       Access: Published
//  Description: Applies the indicated munger to the geom and its
//               data, and returns a (possibly different) geom and
//               data, according to the munger's whim.  
//
//               The assumption is that for a particular geom and a
//               particular munger, the result will always be the
//               same; so this result may be cached.
////////////////////////////////////////////////////////////////////
void qpGeom::
munge_geom(const qpGeomMunger *munger,
           CPT(qpGeom) &result, CPT(qpGeomVertexData) &data) const {
  // Look up the munger in our cache--maybe we've recently applied it.
  {
    // Use read() and release_read() instead of CDReader, because the
    // call to record_geom() might recursively call back into this
    // object, and require a write.
    const CData *cdata = _cycler.read();
    MungedCache::const_iterator ci = cdata->_munged_cache.find(munger);
    if (ci != cdata->_munged_cache.end()) {
      _cycler.release_read(cdata);
      // Record a cache hit, so this element will stay in the cache a
      // while longer.
      qpGeomVertexCacheManager *cache_mgr = 
        qpGeomVertexCacheManager::get_global_ptr();
      cache_mgr->record_geom(this, munger, 
                             (*ci).second._geom->get_num_bytes() +
                             (*ci).second._data->get_num_bytes());

      result = (*ci).second._geom;
      data = (*ci).second._data;
      return;
    }
    _cycler.release_read(cdata);
  }

  // Ok, invoke the munger.
  PStatTimer timer(qpGeomMunger::_munge_pcollector);

  result = this;
  data = munger->munge_data(get_vertex_data());
  ((qpGeomMunger *)munger)->munge_geom_impl(result, data);

  {
    // Record the new result in the cache.
    {
      CDWriter cdata(((qpGeom *)this)->_cycler);
      MungeResult &mr = cdata->_munged_cache[munger];
      mr._geom = result;
      mr._data = data;
    }
    
    // And tell the cache manager about the new entry.  (It might
    // immediately request a delete from the cache of the thing we just
    // added.)
    qpGeomVertexCacheManager *cache_mgr = 
      qpGeomVertexCacheManager::get_global_ptr();
    cache_mgr->record_geom(this, munger, 
                           result->get_num_bytes() + data->get_num_bytes());
  }
}

////////////////////////////////////////////////////////////////////
//     Function: qpGeom::output
//       Access: Published
//  Description: 
////////////////////////////////////////////////////////////////////
void qpGeom::
output(ostream &out) const {
  CDReader cdata(_cycler);

  // Get a list of the primitive types contained within this object.
  pset<TypeHandle> types;
  Primitives::const_iterator pi;
  for (pi = cdata->_primitives.begin(); 
       pi != cdata->_primitives.end();
       ++pi) {
    types.insert((*pi)->get_type());
  }

  out << "Geom [";
  pset<TypeHandle>::iterator ti;
  for (ti = types.begin(); ti != types.end(); ++ti) {
    out << " " << (*ti);
  }
  out << " ], " << cdata->_data->get_num_vertices() << " vertices.";
}

////////////////////////////////////////////////////////////////////
//     Function: qpGeom::write
//       Access: Published
//  Description: 
////////////////////////////////////////////////////////////////////
void qpGeom::
write(ostream &out, int indent_level) const {
  CDReader cdata(_cycler);

  // Get a list of the primitive types contained within this object.
  Primitives::const_iterator pi;
  for (pi = cdata->_primitives.begin(); 
       pi != cdata->_primitives.end();
       ++pi) {
    (*pi)->write(out, indent_level);
  }
}

////////////////////////////////////////////////////////////////////
//     Function: qpGeom::clear_cache
//       Access: Published
//  Description: Removes all of the previously-cached results of
//               munge_geom().
////////////////////////////////////////////////////////////////////
void qpGeom::
clear_cache() {
  // Probably we shouldn't do anything at all here unless we are
  // running in pipeline stage 0.
  qpGeomVertexCacheManager *cache_mgr = 
    qpGeomVertexCacheManager::get_global_ptr();

  CData *cdata = CDWriter(_cycler);
  for (MungedCache::iterator ci = cdata->_munged_cache.begin();
       ci != cdata->_munged_cache.end();
       ++ci) {
    cache_mgr->remove_geom(this, (*ci).first);
  }
  cdata->_munged_cache.clear();
}

////////////////////////////////////////////////////////////////////
//     Function: qpGeom::draw
//       Access: Public
//  Description: Actually draws the Geom with the indicated GSG, using
//               the indicated vertex data (which might have been
//               pre-munged to support the GSG's needs).
////////////////////////////////////////////////////////////////////
void qpGeom::
draw(GraphicsStateGuardianBase *gsg, const qpGeomVertexData *vertex_data) const {
#ifdef DO_PIPELINING
  // Make sure the usage_hint is already updated before we start to
  // draw, so we don't end up with a circular lock if the GSG asks us
  // to update this while we're holding the read lock.
  {
    CDReader cdata(_cycler);
    if (!cdata->_got_usage_hint) {
      CDWriter cdataw(((qpGeom *)this)->_cycler, cdata);
      ((qpGeom *)this)->reset_usage_hint(cdataw);
    }
  }
  // TODO: fix up the race condition between this line and the next.
  // Maybe CDWriter's elevate-to-write should return the read lock to
  // its original locked state when it's done.
#endif  // DO_PIPELINING

  CDReader cdata(_cycler);
  
  if (gsg->begin_draw_primitives(this, vertex_data)) {
    Primitives::const_iterator pi;
    for (pi = cdata->_primitives.begin(); 
         pi != cdata->_primitives.end();
         ++pi) {
      (*pi)->draw(gsg);
    }
    gsg->end_draw_primitives();
  }
}

////////////////////////////////////////////////////////////////////
//     Function: qpGeom::get_next_modified
//       Access: Public
//  Description: Returns a monotonically increasing sequence.  Each
//               time this is called, a new sequence number is
//               returned, higher than the previous value.
//
//               This is used to ensure that
//               GeomVertexArrayData::get_modified() and
//               GeomPrimitive::get_modified() update from the same
//               space, so that Geom::get_modified() returns a
//               meaningful value.
////////////////////////////////////////////////////////////////////
UpdateSeq qpGeom::
get_next_modified() {
  ++_next_modified;
  return _next_modified;
}

////////////////////////////////////////////////////////////////////
//     Function: qpGeom::recompute_bound
//       Access: Protected, Virtual
//  Description: Recomputes the dynamic bounding volume for this Geom.
//               This includes all of the vertices.
////////////////////////////////////////////////////////////////////
BoundingVolume *qpGeom::
recompute_bound() {
  // First, get ourselves a fresh, empty bounding volume.
  BoundingVolume *bound = BoundedObject::recompute_bound();
  nassertr(bound != (BoundingVolume*)0L, bound);

  GeometricBoundingVolume *gbv = DCAST(GeometricBoundingVolume, bound);

  // Now actually compute the bounding volume by putting it around all
  // of our vertices.
  CDReader cdata(_cycler);

  const qpGeomVertexFormat *format = cdata->_data->get_format();

  int array_index = format->get_array_with(InternalName::get_vertex());
  if (array_index < 0) {
    // No vertex data.
    return bound;
  }

  const qpGeomVertexArrayFormat *array_format = format->get_array(array_index);
  const qpGeomVertexDataType *data_type = 
    array_format->get_data_type(InternalName::get_vertex());

  int stride = array_format->get_stride();
  int start = data_type->get_start();
  int num_components = data_type->get_num_components();

  CPTA_uchar array_data = cdata->_data->get_array(array_index)->get_data();

  if (stride == 3 * sizeof(PN_float32) && start == 0 && num_components == 3 &&
      (array_data.size() % stride) == 0) {
    // Here's an easy special case: it's a standalone table of vertex
    // positions, with nothing else in the middle, so we can use
    // directly as an array of LPoint3f's.
    const LPoint3f *vertices_begin = (const LPoint3f *)&array_data[0];
    const LPoint3f *vertices_end = (const LPoint3f *)&array_data[array_data.size()];
    gbv->around(vertices_begin, vertices_end);

  } else {
    // Otherwise, we have to copy the vertex positions out one at time.
    pvector<LPoint3f> vertices;

    size_t p = start;
    while (p + stride <= array_data.size()) {
      const PN_float32 *v = (const PN_float32 *)&array_data[p];

      LPoint3f vertex;
      qpGeomVertexData::to_vec3(vertex, v, num_components);

      vertices.push_back(vertex);
      p += stride;
    }

    const LPoint3f *vertices_begin = &vertices[0];
    const LPoint3f *vertices_end = vertices_begin + vertices.size();

    gbv->around(vertices_begin, vertices_end);
  }

  return bound;
}

////////////////////////////////////////////////////////////////////
//     Function: qpGeom::remove_cache_entry
//       Access: Private
//  Description: Removes a particular entry from the local cache; it
//               has already been removed from the cache manager.
//               This is only called from GeomVertexCacheManager.
////////////////////////////////////////////////////////////////////
void qpGeom::
remove_cache_entry(const qpGeomMunger *modifier) const {
  // We have to operate on stage 0 of the pipeline, since that's where
  // the cache really counts.  Because of the multistage pipeline, we
  // might not actually have a cache entry there (it might have been
  // added to stage 1 instead).  No big deal if we don't.
  CData *cdata = ((qpGeom *)this)->_cycler.write_stage(0);
  MungedCache::iterator ci = cdata->_munged_cache.find(modifier);
  if (ci != cdata->_munged_cache.end()) {
    cdata->_munged_cache.erase(ci);
  }
  ((qpGeom *)this)->_cycler.release_write_stage(0, cdata);
}

////////////////////////////////////////////////////////////////////
//     Function: qpGeom::reset_usage_hint
//       Access: Private
//  Description: Recomputes the minimum usage_hint.
////////////////////////////////////////////////////////////////////
void qpGeom::
reset_usage_hint(qpGeom::CDWriter &cdata) {
  cdata->_usage_hint = qpGeomUsageHint::UH_static;
  Primitives::const_iterator pi;
  for (pi = cdata->_primitives.begin(); 
       pi != cdata->_primitives.end();
       ++pi) {
    cdata->_usage_hint = min(cdata->_usage_hint, (*pi)->get_usage_hint());
  }
  cdata->_got_usage_hint = true;
}

////////////////////////////////////////////////////////////////////
//     Function: qpGeom::register_with_read_factory
//       Access: Public, Static
//  Description: Tells the BamReader how to create objects of type
//               qpGeom.
////////////////////////////////////////////////////////////////////
void qpGeom::
register_with_read_factory() {
  BamReader::get_factory()->register_factory(get_class_type(), make_from_bam);
}

////////////////////////////////////////////////////////////////////
//     Function: qpGeom::write_datagram
//       Access: Public, Virtual
//  Description: Writes the contents of this object to the datagram
//               for shipping out to a Bam file.
////////////////////////////////////////////////////////////////////
void qpGeom::
write_datagram(BamWriter *manager, Datagram &dg) {
  TypedWritable::write_datagram(manager, dg);

  manager->write_cdata(dg, _cycler);
}

////////////////////////////////////////////////////////////////////
//     Function: qpGeom::make_from_bam
//       Access: Protected, Static
//  Description: This function is called by the BamReader's factory
//               when a new object of type qpGeom is encountered
//               in the Bam file.  It should create the qpGeom
//               and extract its information from the file.
////////////////////////////////////////////////////////////////////
TypedWritable *qpGeom::
make_from_bam(const FactoryParams &params) {
  qpGeom *object = new qpGeom;
  DatagramIterator scan;
  BamReader *manager;

  parse_params(params, scan, manager);
  object->fillin(scan, manager);

  return object;
}

////////////////////////////////////////////////////////////////////
//     Function: qpGeom::fillin
//       Access: Protected
//  Description: This internal function is called by make_from_bam to
//               read in all of the relevant data from the BamFile for
//               the new qpGeom.
////////////////////////////////////////////////////////////////////
void qpGeom::
fillin(DatagramIterator &scan, BamReader *manager) {
  TypedWritable::fillin(scan, manager);

  manager->read_cdata(scan, _cycler);
}

////////////////////////////////////////////////////////////////////
//     Function: qpGeom::CData::make_copy
//       Access: Public, Virtual
//  Description:
////////////////////////////////////////////////////////////////////
CycleData *qpGeom::CData::
make_copy() const {
  return new CData(*this);
}

////////////////////////////////////////////////////////////////////
//     Function: qpGeom::CData::write_datagram
//       Access: Public, Virtual
//  Description: Writes the contents of this object to the datagram
//               for shipping out to a Bam file.
////////////////////////////////////////////////////////////////////
void qpGeom::CData::
write_datagram(BamWriter *manager, Datagram &dg) const {
}

////////////////////////////////////////////////////////////////////
//     Function: qpGeom::CData::complete_pointers
//       Access: Public, Virtual
//  Description: Receives an array of pointers, one for each time
//               manager->read_pointer() was called in fillin().
//               Returns the number of pointers processed.
////////////////////////////////////////////////////////////////////
int qpGeom::CData::
complete_pointers(TypedWritable **p_list, BamReader *manager) {
  int pi = CycleData::complete_pointers(p_list, manager);

  return pi;
}

////////////////////////////////////////////////////////////////////
//     Function: qpGeom::CData::fillin
//       Access: Public, Virtual
//  Description: This internal function is called by make_from_bam to
//               read in all of the relevant data from the BamFile for
//               the new qpGeom.
////////////////////////////////////////////////////////////////////
void qpGeom::CData::
fillin(DatagramIterator &scan, BamReader *manager) {
}
