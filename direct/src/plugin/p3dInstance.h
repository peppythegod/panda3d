// Filename: p3dInstance.h
// Created by:  drose (29May09)
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

#ifndef P3DINSTANCE_H
#define P3DINSTANCE_H

#include "p3d_plugin_common.h"
#include "p3dFileDownload.h"
#include "p3dFileParams.h"
#include "p3dWindowParams.h"
#include "p3dReferenceCount.h"
#include "get_tinyxml.h"

#ifdef __APPLE__
#include "subprocessWindowBuffer.h"
#include <CoreFoundation/CoreFoundation.h>
#endif

#ifndef _WIN32
#include <sys/time.h>
#endif

#include <deque>
#include <map>

class P3DSession;
class P3DSplashWindow;
class P3DDownload;
class P3DPackage;
class P3DObject;
class P3DToplevelObject;
class P3DTemporaryFile;

////////////////////////////////////////////////////////////////////
//       Class : P3DInstance
// Description : This is an instance of a Panda3D window, as seen in
//               the parent-level process.
////////////////////////////////////////////////////////////////////
class P3DInstance : public P3D_instance, public P3DReferenceCount {
public:
  P3DInstance(P3D_request_ready_func *func, 
              const P3D_token tokens[], size_t num_tokens, 
              int argc, const char *argv[], void *user_data);
  ~P3DInstance();

  void set_p3d_url(const string &p3d_url);
  void set_p3d_filename(const string &p3d_filename);
  inline const P3DFileParams &get_fparams() const;

  void set_wparams(const P3DWindowParams &wparams);
  inline const P3DWindowParams &get_wparams() const;

  P3D_object *get_panda_script_object() const;
  void set_browser_script_object(P3D_object *object);

  bool has_request();
  P3D_request *get_request();
  void bake_requests();
  void add_raw_request(TiXmlDocument *doc);
  void add_baked_request(P3D_request *request);
  void finish_request(P3D_request *request, bool handled);

  bool feed_url_stream(int unique_id,
                       P3D_result_code result_code,
                       int http_status_code, 
                       size_t total_expected_data,
                       const unsigned char *this_data, 
                       size_t this_data_size);

  bool handle_event(P3D_event_data event);

  inline int get_instance_id() const;
  inline const string &get_session_key() const;
  inline const string &get_python_version() const;

  inline P3D_request_ready_func *get_request_ready_func() const;

  void add_package(P3DPackage *package);
  bool get_packages_info_ready() const;
  bool get_packages_ready() const;
  bool get_packages_failed() const;
  
  void start_download(P3DDownload *download);
  inline bool is_started() const;
  void request_stop();
  void request_refresh();

  TiXmlElement *make_xml();

private:
  class SplashDownload : public P3DFileDownload {
  public:
    SplashDownload(P3DInstance *inst);
  protected:
    virtual void download_finished(bool success);
  private:
    P3DInstance *_inst;
  };
  class InstanceDownload : public P3DFileDownload {
  public:
    InstanceDownload(P3DInstance *inst);
  protected:
    virtual void download_progress();
    virtual void download_finished(bool success);
  private:
    P3DInstance *_inst;
  };

  void scan_app_desc_file(TiXmlDocument *doc);

  void send_browser_script_object();
  P3D_request *make_p3d_request(TiXmlElement *xrequest);
  void handle_notify_request(const string &message);
  void handle_script_request(const string &operation, P3D_object *object, 
                             const string &property_name, P3D_object *value,
                             bool needs_response, int unique_id);
  void make_splash_window();
  void report_package_info_ready(P3DPackage *package);
  void start_next_download();
  void report_instance_progress(double progress);
  void report_package_progress(P3DPackage *package, double progress);
  void report_package_done(P3DPackage *package, bool progress);
  void set_install_label(const string &install_label);

  void paint_window();
  void add_modifier_flags(unsigned int &swb_flags, int modifiers);

  void send_notify(const string &message);

#ifdef __APPLE__
  static void timer_callback(CFRunLoopTimerRef timer, void *info);
#endif  // __APPLE__

  P3D_request_ready_func *_func;
  P3D_object *_browser_script_object;
  P3DToplevelObject *_panda_script_object;

  P3DTemporaryFile *_temp_p3d_filename;
  P3DTemporaryFile *_temp_splash_image;

  bool _got_fparams;
  P3DFileParams _fparams;

  bool _got_wparams;
  P3DWindowParams _wparams;

  int _instance_id;
  string _session_key;
  string _python_version;
  string _log_basename;
  bool _full_disk_access;
  bool _hidden;

  // Not ref-counted: session is the parent.
  P3DSession *_session;

#ifdef __APPLE__
  // On OSX, we have to get a copy of the framebuffer data back from
  // the child process, and draw it to the window, here in the parent
  // process.  Crazy!
  int _shared_fd;
  size_t _shared_mmap_size;
  string _shared_filename;
  SubprocessWindowBuffer *_swbuffer;
  char *_reversed_buffer;
  bool _mouse_active;

  CFRunLoopTimerRef _frame_timer;
#endif  // __APPLE__

  P3DSplashWindow *_splash_window;
  string _install_label;
  bool _instance_window_opened;

  // Members for deciding whether and when to display the progress bar
  // for downloading the initial instance data.
#ifdef _WIN32
  int _start_dl_instance_tick;
#else
  struct timeval _start_dl_instance_timeval;
#endif
  bool _show_dl_instance_progress;

  typedef vector<P3DPackage *> Packages;
  Packages _packages;
  Packages _downloading_packages;
  int _download_package_index;
  size_t _total_download_size;
  size_t _total_downloaded;

  // We keep the _panda3d pointer separately because it's so
  // important, but it's in the above vector also.
  P3DPackage *_panda3d;  

  typedef map<int, P3DDownload *> Downloads;
  Downloads _downloads;

  // The _raw_requests queue might be filled up by the read thread, so
  // we protect it in a lock.
  LOCK _request_lock;
  typedef deque<TiXmlDocument *> RawRequests;
  RawRequests _raw_requests;
  bool _requested_stop;

  // The _baked_requests queue is only touched in the main thread; no
  // lock needed.
  typedef deque<P3D_request *> BakedRequests;
  BakedRequests _baked_requests;

  friend class P3DSession;
  friend class SplashDownload;
  friend class P3DWindowParams;
  friend class P3DPackage;
};

#include "p3dInstance.I"

#endif