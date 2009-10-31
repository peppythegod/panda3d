// Filename: physxEnums.h
// Created by:  enn0x (23Sep09)
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

#ifndef PHYSXENUMS_H
#define PHYSXENUMS_H

#include "pandabase.h"

#include "NoMinMax.h"
#include "NxPhysics.h"

#ifdef CPPPARSER

// PhysxParameter
#define NX_PENALTY_FORCE 0
#define NX_SKIN_WIDTH 1
#define NX_DEFAULT_SLEEP_LIN_VEL_SQUARED 2
#define NX_DEFAULT_SLEEP_ANG_VEL_SQUARED 3
#define NX_BOUNCE_THRESHOLD 4
#define NX_DYN_FRICT_SCALING 5
#define NX_STA_FRICT_SCALING 6
#define NX_MAX_ANGULAR_VELOCITY 7
#define NX_CONTINUOUS_CD 8
#define NX_VISUALIZATION_SCALE 9
#define NX_VISUALIZE_WORLD_AXES 10
#define NX_VISUALIZE_BODY_AXES 11
#define NX_VISUALIZE_BODY_MASS_AXES 12
#define NX_VISUALIZE_BODY_LIN_VELOCITY 13
#define NX_VISUALIZE_BODY_ANG_VELOCITY 14
#define NX_VISUALIZE_BODY_JOINT_GROUPS 22
#define NX_VISUALIZE_JOINT_LOCAL_AXES 27
#define NX_VISUALIZE_JOINT_WORLD_AXES 28
#define NX_VISUALIZE_JOINT_LIMITS 29
#define NX_VISUALIZE_CONTACT_POINT 33
#define NX_VISUALIZE_CONTACT_NORMAL 34
#define NX_VISUALIZE_CONTACT_ERROR 35
#define NX_VISUALIZE_CONTACT_FORCE 36
#define NX_VISUALIZE_ACTOR_AXES 37
#define NX_VISUALIZE_COLLISION_AABBS 38
#define NX_VISUALIZE_COLLISION_SHAPES 39
#define NX_VISUALIZE_COLLISION_AXES 40
#define NX_VISUALIZE_COLLISION_COMPOUNDS 41
#define NX_VISUALIZE_COLLISION_VNORMALS 42
#define NX_VISUALIZE_COLLISION_FNORMALS 43
#define NX_VISUALIZE_COLLISION_EDGES 44
#define NX_VISUALIZE_COLLISION_SPHERES 45
#define NX_VISUALIZE_COLLISION_STATIC 47
#define NX_VISUALIZE_COLLISION_DYNAMIC 48
#define NX_VISUALIZE_COLLISION_FREE 49
#define NX_VISUALIZE_COLLISION_CCD 50
#define NX_VISUALIZE_COLLISION_SKELETONS 51
#define NX_VISUALIZE_FLUID_EMITTERS 52
#define NX_VISUALIZE_FLUID_POSITION 53
#define NX_VISUALIZE_FLUID_VELOCITY 54
#define NX_VISUALIZE_FLUID_KERNEL_RADIUS 55
#define NX_VISUALIZE_FLUID_BOUNDS 56
#define NX_VISUALIZE_FLUID_PACKETS 57
#define NX_VISUALIZE_FLUID_MOTION_LIMIT 58
#define NX_VISUALIZE_FLUID_DYN_COLLISION 59
#define NX_VISUALIZE_FLUID_STC_COLLISION 60
#define NX_VISUALIZE_FLUID_MESH_PACKETS 61
#define NX_VISUALIZE_FLUID_DRAINS 62
#define NX_VISUALIZE_FLUID_PACKET_DATA 90
#define NX_VISUALIZE_CLOTH_MESH 63
#define NX_VISUALIZE_CLOTH_COLLISIONS 64
#define NX_VISUALIZE_CLOTH_SELFCOLLISIONS 65
#define NX_VISUALIZE_CLOTH_WORKPACKETS 66
#define NX_VISUALIZE_CLOTH_SLEEP 67
#define NX_VISUALIZE_CLOTH_SLEEP_VERTEX 94
#define NX_VISUALIZE_CLOTH_TEARABLE_VERTICES 80
#define NX_VISUALIZE_CLOTH_TEARING 81
#define NX_VISUALIZE_CLOTH_ATTACHMENT 82
#define NX_VISUALIZE_CLOTH_VALIDBOUNDS 92
#define NX_VISUALIZE_SOFTBODY_MESH 83
#define NX_VISUALIZE_SOFTBODY_COLLISIONS 84
#define NX_VISUALIZE_SOFTBODY_WORKPACKETS 85
#define NX_VISUALIZE_SOFTBODY_SLEEP 86
#define NX_VISUALIZE_SOFTBODY_SLEEP_VERTEX 95
#define NX_VISUALIZE_SOFTBODY_TEARABLE_VERTICES 87
#define NX_VISUALIZE_SOFTBODY_TEARING 88
#define NX_VISUALIZE_SOFTBODY_ATTACHMENT 89
#define NX_VISUALIZE_SOFTBODY_VALIDBOUNDS 93
#define NX_ADAPTIVE_FORCE 68
#define NX_COLL_VETO_JOINTED 69
#define NX_TRIGGER_TRIGGER_CALLBACK 70
#define NX_SELECT_HW_ALGO 71
#define NX_VISUALIZE_ACTIVE_VERTICES 72
#define NX_CCD_EPSILON 73
#define NX_SOLVER_CONVERGENCE_THRESHOLD 74
#define NX_BBOX_NOISE_LEVEL 75
#define NX_IMPLICIT_SWEEP_CACHE_SIZE 76
#define NX_DEFAULT_SLEEP_ENERGY 77
#define NX_CONSTANT_FLUID_MAX_PACKETS 78
#define NX_CONSTANT_FLUID_MAX_PARTICLES_PER_STEP 79
#define NX_VISUALIZE_FORCE_FIELDS 91
#define NX_ASYNCHRONOUS_MESH_CREATION 96
#define NX_FORCE_FIELD_CUSTOM_KERNEL_EPSILON 97
#define NX_IMPROVED_SPRING_SOLVER 98
#define NX_FAST_MASSIVE_BP_VOLUME_DELETION 99
#define NX_LEGACY_JOINT_DRIVE 100

// PhysxActorFlag
#define NX_AF_DISABLE_COLLISION 1<<0
#define NX_AF_DISABLE_RESPONSE 1<<1
#define NX_AF_LOCK_COM 1<<2
#define NX_AF_FLUID_DISABLE_COLLISION 1<<3
#define NX_AF_CONTACT_MODIFICATION 1<<4
#define NX_AF_FORCE_CONE_FRICTION 1<<5
#define NX_AF_USER_ACTOR_PAIR_FILTERING 1<<6 

// PhysxBodyFlag
#define NX_BF_DISABLE_GRAVITY 1<<0
#define NX_BF_FROZEN_POS_X 1<<1
#define NX_BF_FROZEN_POS_Y 1<<2
#define NX_BF_FROZEN_POS_Z 1<<3
#define NX_BF_FROZEN_ROT_X 1<<4
#define NX_BF_FROZEN_ROT_Y 1<<5
#define NX_BF_FROZEN_ROT_Z 1<<6
#define NX_BF_FROZEN_POS 256
#define NX_BF_FROZEN_ROT 131072
#define NX_BF_FROZEN 131328
#define NX_BF_KINEMATIC 1<<7
#define NX_BF_VISUALIZATION 1<<8
#define NX_BF_FILTER_SLEEP_VEL 1<<10
#define NX_BF_ENERGY_SLEEP_TEST 1<<11

// PhysxContactPairFlag
#define NX_IGNORE_PAIR 1<<0
#define NX_NOTIFY_ON_START_TOUCH 1<<1
#define NX_NOTIFY_ON_END_TOUCH 1<<2
#define NX_NOTIFY_ON_TOUCH 1<<3
#define NX_NOTIFY_ON_IMPACT 1<<4
#define NX_NOTIFY_ON_ROLL 1<<5
#define NX_NOTIFY_ON_SLIDE 1<<6
#define NX_NOTIFY_FORCES 1<<7
#define NX_NOTIFY_ON_START_TOUCH_FORCE_THRESHOLD 1<<8 
#define NX_NOTIFY_ON_END_TOUCH_FORCE_THRESHOLD 1<<9  
#define NX_NOTIFY_ON_TOUCH_FORCE_THRESHOLD 1<<10
#define NX_NOTIFY_CONTACT_MODIFICATION 1<<16

// PhysxCombineMode
#define NX_CM_AVERAGE 0
#define NX_CM_MIN 1
#define NX_CM_MULTIPLY 2
#define NX_CM_MAX 3

// PhysxD6JointDriveType
#define NX_D6JOINT_DRIVE_POSITION 1<<0
#define NX_D6JOINT_DRIVE_VELOCITY 1<<1

// PhysxD6JointFlag
#define NX_D6JOINT_SLERP_DRIVE 1<<0
#define NX_D6JOINT_GEAR_ENABLED 1<<1

// PhysxD6JointMotion
#define NX_D6JOINT_MOTION_LOCKED 0
#define NX_D6JOINT_MOTION_LIMITED 1
#define NX_D6JOINT_MOTION_FREE 2

// PhysxDistanceJointFlag
#define NX_DJF_MAX_DISTANCE_ENABLED 1<<0
#define NX_DJF_MIN_DISTANCE_ENABLED 1<<1
#define NX_DJF_SPRING_ENABLED 1<<2

// PhysxFilterOp
#define NX_FILTEROP_AND 0
#define NX_FILTEROP_OR 1
#define NX_FILTEROP_XOR 2
#define NX_FILTEROP_NAND 3
#define NX_FILTEROP_NOR 4
#define NX_FILTEROP_NXOR 5
#define NX_FILTEROP_SWAP_AND 6

// PhysxMaterialFlag
#define NX_MF_ANISOTROPIC 1<<0
#define NX_MF_DISABLE_FRICTION 1<<4
#define NX_MF_DISABLE_STRONG_FRICTION 1<<5

// PhysxForceMode
#define NX_FORCE 0
#define NX_IMPULSE 1
#define NX_VELOCITY_CHANGE 2
#define NX_SMOOTH_IMPULSE 3
#define NX_SMOOTH_VELOCITY_CHANGE 4
#define NX_ACCELERATION 5

// PhysxJointFlag
#define NX_JF_COLLISION_ENABLED 1<<0
#define NX_JF_VISUALIZATION 1<<1

// PhysxProjectionMode
#define NX_JPM_NONE 0
#define NX_JPM_POINT_MINDIST 1
#define NX_JPM_LINEAR_MINDIST 2

// PhysxPulleyJointFlag
#define NX_PJF_IS_RIGID 1<<0
#define NX_PJF_MOTOR_ENABLED 1<<1

// PhysxRevoluteJointFlag
#define NX_RJF_LIMIT_ENABLED 1<<0
#define NX_RJF_MOTOR_ENABLED 1<<1
#define NX_RJF_SPRING_ENABLED 1<<2

// PhysxShapeFlag
#define NX_TRIGGER_ON_ENTER 1<<0
#define NX_TRIGGER_ON_LEAVE 1<<1
#define NX_TRIGGER_ON_STAY 1<<2
#define NX_TRIGGER_ENABLE 7
#define NX_SF_VISUALIZATION 1<<3
#define NX_SF_DISABLE_COLLISION 1<<4
#define NX_SF_FEATURE_INDICES 1<<5
#define NX_SF_DISABLE_RAYCASTING 1<<6
#define NX_SF_POINT_CONTACT_FORCE 1<<7
#define NX_SF_FLUID_DRAIN 1<<8
#define NX_SF_FLUID_DISABLE_COLLISION 1<<10
#define NX_SF_FLUID_TWOWAY 1<<11
#define NX_SF_DISABLE_RESPONSE 1<<12
#define NX_SF_DYNAMIC_DYNAMIC_CCD 1<<13
#define NX_SF_DISABLE_SCENE_QUERIES 1<<14
#define NX_SF_CLOTH_DRAIN 1<<15
#define NX_SF_CLOTH_DISABLE_COLLISION 1<<16
#define NX_SF_CLOTH_TWOWAY 1<<17
#define NX_SF_SOFTBODY_DRAIN 1<<18
#define NX_SF_SOFTBODY_DISABLE_COLLISION 1<<19
#define NX_SF_SOFTBODY_TWOWAY 1<<20

// PhysxShapesType
#define NX_STATIC_SHAPES 1<<0
#define NX_DYNAMIC_SHAPES 1<<1
#define NX_ALL_SHAPES (1<<0|1<<1)

// PhysxSphericalJointFlag
#define NX_SJF_TWIST_LIMIT_ENABLED 1<<0
#define NX_SJF_SWING_LIMIT_ENABLED 1<<1
#define NX_SJF_TWIST_SPRING_ENABLED 1<<2
#define NX_SJF_SWING_SPRING_ENABLED 1<<3
#define NX_SJF_JOINT_SPRING_ENABLED 1<<4
#define NX_SJF_PERPENDICULAR_DIR_CONSTRAINTS 1<<5

// PhysxUpAxis
#define NX_X 1
#define NX_Y 2
#define NX_Z 3

#endif // CPPPARSER

////////////////////////////////////////////////////////////////////
//       Class : BamEnums
// Description : This class exists just to provide scoping for the
//               enums shared by PhysX classes.
////////////////////////////////////////////////////////////////////
class EXPCL_PANDAPHYSX PhysxEnums {
PUBLISHED:

  enum PhysxParameter {
    P_penalty_force                         = NX_PENALTY_FORCE,
    P_skin_width                            = NX_SKIN_WIDTH,
    P_default_sleep_lin_vel_squared         = NX_DEFAULT_SLEEP_LIN_VEL_SQUARED,
    P_default_sleep_ang_vel_squared         = NX_DEFAULT_SLEEP_ANG_VEL_SQUARED,
    P_bounce_threshold                      = NX_BOUNCE_THRESHOLD,
    P_dyn_frict_scaling                     = NX_DYN_FRICT_SCALING,
    P_sta_frict_scaling                     = NX_STA_FRICT_SCALING,
    P_max_angular_velocity                  = NX_MAX_ANGULAR_VELOCITY,
    P_continuous_cd                         = NX_CONTINUOUS_CD,
    P_visualization_scale                   = NX_VISUALIZATION_SCALE,
    P_adaptive_force                        = NX_ADAPTIVE_FORCE,
    P_coll_veta_jointed                     = NX_COLL_VETO_JOINTED,
    P_trigger_trigger_callback              = NX_TRIGGER_TRIGGER_CALLBACK,
    P_select_hw_algo                        = NX_SELECT_HW_ALGO,
    P_ccd_epsilon                           = NX_CCD_EPSILON,
    P_solver_convergence_threshold          = NX_SOLVER_CONVERGENCE_THRESHOLD,
    P_bbox_noise_level                      = NX_BBOX_NOISE_LEVEL,
    P_implicit_sweep_cache_size             = NX_IMPLICIT_SWEEP_CACHE_SIZE,
    P_default_sleep_energy                  = NX_DEFAULT_SLEEP_ENERGY,
    P_constant_fluid_max_packets            = NX_CONSTANT_FLUID_MAX_PACKETS,
    P_constant_fluid_max_particles_per_step = NX_CONSTANT_FLUID_MAX_PARTICLES_PER_STEP,
    P_asynchronous_mesh_creation            = NX_ASYNCHRONOUS_MESH_CREATION,
    P_force_field_custom_kernel_epsilon     = NX_FORCE_FIELD_CUSTOM_KERNEL_EPSILON,
    P_improved_spring_solver                = NX_IMPROVED_SPRING_SOLVER,
    P_fast_massive_bp_volume_deletion       = NX_FAST_MASSIVE_BP_VOLUME_DELETION,
    P_legacy_joint_drive                    = NX_LEGACY_JOINT_DRIVE,

    P_visualize_world_axes                  = NX_VISUALIZE_WORLD_AXES,
    P_visualize_body_axes                   = NX_VISUALIZE_BODY_AXES,
    P_visualize_body_mass_axes              = NX_VISUALIZE_BODY_MASS_AXES,
    P_visualize_body_lin_velocity           = NX_VISUALIZE_BODY_LIN_VELOCITY,
    P_visualize_body_ang_velocity           = NX_VISUALIZE_BODY_ANG_VELOCITY,
    P_visualize_body_joint_groups           = NX_VISUALIZE_BODY_JOINT_GROUPS,
    P_visualize_joint_local_axes            = NX_VISUALIZE_JOINT_LOCAL_AXES,
    P_visualize_joint_world_axes            = NX_VISUALIZE_JOINT_WORLD_AXES,
    P_visualize_joint_limits                = NX_VISUALIZE_JOINT_LIMITS,
    P_visualize_contact_point               = NX_VISUALIZE_CONTACT_POINT,
    P_visualize_contact_normal              = NX_VISUALIZE_CONTACT_NORMAL,
    P_visualize_contact_error               = NX_VISUALIZE_CONTACT_ERROR,
    P_visualize_contact_force               = NX_VISUALIZE_CONTACT_FORCE,
    P_visualize_actor_axes                  = NX_VISUALIZE_ACTOR_AXES,
    P_visualize_collision_aabbs             = NX_VISUALIZE_COLLISION_AABBS,
    P_visualize_collision_shapes            = NX_VISUALIZE_COLLISION_SHAPES,
    P_visualize_collision_axes              = NX_VISUALIZE_COLLISION_AXES,
    P_visualize_collision_compounds         = NX_VISUALIZE_COLLISION_COMPOUNDS,
    P_visualize_collision_vnormals          = NX_VISUALIZE_COLLISION_VNORMALS,
    P_visualize_collision_fnormals          = NX_VISUALIZE_COLLISION_FNORMALS,
    P_visualize_collision_edges             = NX_VISUALIZE_COLLISION_EDGES,
    P_visualize_collision_spheres           = NX_VISUALIZE_COLLISION_SPHERES,
    P_visualize_collision_static            = NX_VISUALIZE_COLLISION_STATIC,
    P_visualize_collision_dynamic           = NX_VISUALIZE_COLLISION_DYNAMIC,
    P_visualize_collision_free              = NX_VISUALIZE_COLLISION_FREE,
    P_visualize_collision_ccd               = NX_VISUALIZE_COLLISION_CCD,
    P_visualize_collision_skeletons         = NX_VISUALIZE_COLLISION_SKELETONS,
    P_visualize_fluid_emitters              = NX_VISUALIZE_FLUID_EMITTERS,
    P_visualize_fluid_position              = NX_VISUALIZE_FLUID_POSITION,
    P_visualize_fluid_velocity              = NX_VISUALIZE_FLUID_VELOCITY,
    P_visualize_fluid_kernel_radius         = NX_VISUALIZE_FLUID_KERNEL_RADIUS,
    P_visualize_fluid_bounds                = NX_VISUALIZE_FLUID_BOUNDS,
    P_visualize_fluid_packets               = NX_VISUALIZE_FLUID_PACKETS,
    P_visualize_fluid_motion_limit          = NX_VISUALIZE_FLUID_MOTION_LIMIT,
    P_visualize_fluid_dyn_collision         = NX_VISUALIZE_FLUID_DYN_COLLISION,
    P_visualize_fluid_stc_collision         = NX_VISUALIZE_FLUID_STC_COLLISION,
    P_visualize_fluid_mesh_packets          = NX_VISUALIZE_FLUID_MESH_PACKETS,
    P_visualize_fluid_drains                = NX_VISUALIZE_FLUID_DRAINS,
    P_visualize_fluid_packet_data           = NX_VISUALIZE_FLUID_PACKET_DATA,
    P_visualize_cloth_mesh                  = NX_VISUALIZE_CLOTH_MESH,
    P_visualize_cloth_collisions            = NX_VISUALIZE_CLOTH_COLLISIONS,
    P_visualize_cloth_selfcollisions        = NX_VISUALIZE_CLOTH_SELFCOLLISIONS,
    P_visualize_cloth_workpackets           = NX_VISUALIZE_CLOTH_WORKPACKETS,
    P_visualize_cloth_sleep                 = NX_VISUALIZE_CLOTH_SLEEP,
    P_visualize_cloth_sleep_vertex          = NX_VISUALIZE_CLOTH_SLEEP_VERTEX,
    P_visualize_cloth_tearable_vertices     = NX_VISUALIZE_CLOTH_TEARABLE_VERTICES,
    P_visualize_cloth_tearing               = NX_VISUALIZE_CLOTH_TEARING,
    P_visualize_cloth_attachment            = NX_VISUALIZE_CLOTH_ATTACHMENT,
    P_visualize_cloth_validbounds           = NX_VISUALIZE_CLOTH_VALIDBOUNDS,
    P_visualize_softbody_mesh               = NX_VISUALIZE_SOFTBODY_MESH,
    P_visualize_softbody_collisions         = NX_VISUALIZE_SOFTBODY_COLLISIONS,
    P_visualize_softbody_workpackets        = NX_VISUALIZE_SOFTBODY_WORKPACKETS,
    P_visualize_softbody_sleep              = NX_VISUALIZE_SOFTBODY_SLEEP,
    P_visualize_softbody_sleep_vertex       = NX_VISUALIZE_SOFTBODY_SLEEP_VERTEX,
    P_visualize_softbody_tearable_vertices  = NX_VISUALIZE_SOFTBODY_TEARABLE_VERTICES,
    P_visualize_softbody_tearing            = NX_VISUALIZE_SOFTBODY_TEARING,
    P_visualize_softbody_attachment         = NX_VISUALIZE_SOFTBODY_ATTACHMENT,
    P_visualize_softbody_validbounds        = NX_VISUALIZE_SOFTBODY_VALIDBOUNDS,
    P_visualize_active_vertices             = NX_VISUALIZE_ACTIVE_VERTICES,
    P_visualize_force_fields                = NX_VISUALIZE_FORCE_FIELDS,
  };

  enum PhysxActorFlag {
    AF_disable_collision         = NX_AF_DISABLE_COLLISION,
    AF_disable_response          = NX_AF_DISABLE_RESPONSE,
    AF_lock_com                  = NX_AF_LOCK_COM,
    AF_fluid_disable_collision   = NX_AF_FLUID_DISABLE_COLLISION,
    AF_contact_modification      = NX_AF_CONTACT_MODIFICATION,
    AF_force_cone_friction       = NX_AF_FORCE_CONE_FRICTION,
    AF_user_actor_pair_filtering = NX_AF_USER_ACTOR_PAIR_FILTERING
  };

  enum PhysxBodyFlag {
    BF_disable_gravity   = NX_BF_DISABLE_GRAVITY,
    Bf_frozen_pos_x      = NX_BF_FROZEN_POS_X,
    BF_frozen_pos_y      = NX_BF_FROZEN_POS_Y,
    BF_frozen_pos_z      = NX_BF_FROZEN_POS_Z,
    BF_frozen_rot_x      = NX_BF_FROZEN_ROT_X,
    BF_frozen_rot_y      = NX_BF_FROZEN_ROT_Y,
    BF_frozen_rot_z      = NX_BF_FROZEN_ROT_Z,
    BF_frozen_pos        = NX_BF_FROZEN_POS,
    BF_frozen_rot        = NX_BF_FROZEN_ROT,
    BF_frozen            = NX_BF_FROZEN,
    BF_kinematic         = NX_BF_KINEMATIC,
    BF_visualization     = NX_BF_VISUALIZATION,
    BF_filter_sleep_vel  = NX_BF_FILTER_SLEEP_VEL,
    BF_energy_sleep_test = NX_BF_ENERGY_SLEEP_TEST
  };

  enum PhysxContactPairFlag {
    CPF_ignore_pair                     = NX_IGNORE_PAIR,
    CPF_notify_on_start_touch           = NX_NOTIFY_ON_START_TOUCH,
    CPF_notify_on_end_touch             = NX_NOTIFY_ON_END_TOUCH,
    CPF_notify_on_touch                 = NX_NOTIFY_ON_TOUCH,
    CPF_notify_on_impact                = NX_NOTIFY_ON_IMPACT,
    CPF_notify_on_roll                  = NX_NOTIFY_ON_ROLL,
    CPF_notify_on_slide                 = NX_NOTIFY_ON_SLIDE,
    CPF_notify_forces                   = NX_NOTIFY_FORCES,
    CPF_notify_on_start_touch_threshold = NX_NOTIFY_ON_START_TOUCH_FORCE_THRESHOLD,
    CPF_notify_on_end_touch_threshold   = NX_NOTIFY_ON_END_TOUCH_FORCE_THRESHOLD,
    CPF_notify_on_touch_threshold       = NX_NOTIFY_ON_TOUCH_FORCE_THRESHOLD,
    CPF_notify_contact_modifications    = NX_NOTIFY_CONTACT_MODIFICATION
  };

  enum PhysxCombineMode {
    CM_average  = NX_CM_AVERAGE,
    CM_min      = NX_CM_MIN,
    CM_multiply = NX_CM_MULTIPLY,
    CM_max      = NX_CM_MAX
  };

  enum PhysxD6JointDriveType {
    D6_joint_drive_position = NX_D6JOINT_DRIVE_POSITION,
    D6_joint_drive_velocity = NX_D6JOINT_DRIVE_VELOCITY
  };

  enum PhysxD6JointFlag {
    D6_joint_slerp_drive   = NX_D6JOINT_SLERP_DRIVE,
    D6_joint_gear_disabled = NX_D6JOINT_GEAR_ENABLED
  };

  enum PhysxD6JointMotion {
    D6_joint_motion_locked  = NX_D6JOINT_MOTION_LOCKED,
    D6_joint_motion_limited = NX_D6JOINT_MOTION_LIMITED,
    D6_joint_motion_free    = NX_D6JOINT_MOTION_FREE
  };

  enum PhysxDistanceJointFlag {
    DJF_max_distance_enabled  = NX_DJF_MAX_DISTANCE_ENABLED,
    DJF_mix_distance_enabled  = NX_DJF_MIN_DISTANCE_ENABLED,
    DJF_spring_enabled        = NX_DJF_SPRING_ENABLED
  };

  enum PhysxFilterOp {
    FO_and       = NX_FILTEROP_AND,
    FO_or        = NX_FILTEROP_OR,
    FO_xor       = NX_FILTEROP_XOR,
    FO_nand      = NX_FILTEROP_NAND,
    FO_nor       = NX_FILTEROP_NOR,
    FO_nxor      = NX_FILTEROP_NXOR,
    FO_swap_and  = NX_FILTEROP_SWAP_AND
  };

  enum PhysxForceMode {
    FM_force                  = NX_FORCE,
    FM_impulse                = NX_IMPULSE,
    FM_velocity_change        = NX_VELOCITY_CHANGE,
    FM_smooth_impulse         = NX_SMOOTH_IMPULSE,
    FM_smooth_velocity_change = NX_SMOOTH_VELOCITY_CHANGE,
    FM_acceleration           = NX_ACCELERATION
  };

  enum PhysxJointFlag {
    JF_collision_enabled = NX_JF_COLLISION_ENABLED,
    JF_visualization     = NX_JF_VISUALIZATION
  };

  enum PhysxMaterialFlag {
    MF_anisotropic             = NX_MF_ANISOTROPIC,
    MF_disable_friction        = NX_MF_DISABLE_FRICTION,
    MF_disable_strong_friction = NX_MF_DISABLE_STRONG_FRICTION
  };

  enum PhysxProjectionMode {
    PM_none             = NX_JPM_NONE,
    PM_point_mindist    = NX_JPM_POINT_MINDIST,
    PM_linear_mindist   = NX_JPM_LINEAR_MINDIST
  };

  enum PhysxPulleyJointFlag {
    PJF_is_rigid       = NX_PJF_IS_RIGID,
    PJF_motor_enabled  = NX_PJF_MOTOR_ENABLED
  };

  enum PhysxRevoluteJointFlag {
    RJF_limit_enabled    = NX_RJF_LIMIT_ENABLED,
    RJF_motor_enabled    = NX_RJF_MOTOR_ENABLED,
    RJF_spring_enabled   = NX_RJF_SPRING_ENABLED
  };

  enum PhysxShapeFlag {
    SF_trigger_on_enter           = NX_TRIGGER_ON_ENTER,
    SF_trigger_on_leave           = NX_TRIGGER_ON_LEAVE,
    SF_trigger_on_stay            = NX_TRIGGER_ON_STAY,
    SF_trigger_enable             = NX_TRIGGER_ENABLE,
    SF_visualization              = NX_SF_VISUALIZATION,
    SF_disable_collision          = NX_SF_DISABLE_COLLISION,
    SF_disable_raycasting         = NX_SF_DISABLE_RAYCASTING,
    SF_disable_response           = NX_SF_DISABLE_RESPONSE,
    SF_disable_scene_queries      = NX_SF_DISABLE_SCENE_QUERIES,
    SF_point_contact_force        = NX_SF_POINT_CONTACT_FORCE,
    SF_feature_indices            = NX_SF_FEATURE_INDICES,
    SF_dynamic_dynamic_ccd        = NX_SF_DYNAMIC_DYNAMIC_CCD,
    SF_fluid_drain                = NX_SF_FLUID_DRAIN,
    SF_fluid_disable_collision    = NX_SF_FLUID_DISABLE_COLLISION,
    SF_fluid_twoway               = NX_SF_FLUID_TWOWAY,
    SF_cloth_drain                = NX_SF_CLOTH_DRAIN,
    SF_cloth_disable_collision    = NX_SF_CLOTH_DISABLE_COLLISION,
    SF_cloth_twoway               = NX_SF_CLOTH_TWOWAY,
    SF_softbody_drain             = NX_SF_SOFTBODY_DRAIN,
    SF_softbody_disable_collision = NX_SF_SOFTBODY_DISABLE_COLLISION,
    SF_softbody_twoway            = NX_SF_SOFTBODY_TWOWAY
  };

  enum PhysxShapesType {
    ST_static   = NX_STATIC_SHAPES ,
    ST_dynamic  = NX_DYNAMIC_SHAPES ,
    ST_all      = NX_ALL_SHAPES 
  };

  enum PhysxSphericalJointFlag {
    SJF_twist_limit_enabled           = NX_SJF_TWIST_LIMIT_ENABLED,
    SJF_swing_limit_enabled           = NX_SJF_SWING_LIMIT_ENABLED,
    SJF_twist_spring_enabled          = NX_SJF_TWIST_SPRING_ENABLED,
    SJF_swing_spring_enabled          = NX_SJF_SWING_SPRING_ENABLED,
    SJF_joint_spring_enabled          = NX_SJF_JOINT_SPRING_ENABLED,
    SJF_perpendicular_dir_constraints = NX_SJF_PERPENDICULAR_DIR_CONSTRAINTS
  };

  enum PhysxUpAxis {
    X_up  = NX_X,
    Y_up  = NX_Y,
    Z_up  = NX_Z
  };

};

EXPCL_PANDAPHYSX ostream &operator << (ostream &out, PhysxEnums::PhysxUpAxis axis);
EXPCL_PANDAPHYSX istream &operator >> (istream &in, PhysxEnums::PhysxUpAxis &axis);

#endif
