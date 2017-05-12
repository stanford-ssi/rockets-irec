/* Include files */

#include "solver_simulink_sfun.h"
#include "c2_solver_simulink.h"
#include "mwmathutil.h"
#define CHARTINSTANCE_CHARTNUMBER      (chartInstance->chartNumber)
#define CHARTINSTANCE_INSTANCENUMBER   (chartInstance->instanceNumber)
#include "solver_simulink_sfun_debug_macros.h"
#define _SF_MEX_LISTEN_FOR_CTRL_C(S)   sf_mex_listen_for_ctrl_c_with_debugger(S, sfGlobalDebugInstanceStruct);

static void chart_debug_initialization(SimStruct *S, unsigned int
  fullDebuggerInitialization);
static void chart_debug_initialize_data_addresses(SimStruct *S);
static const mxArray* sf_opaque_get_hover_data_for_msg(void *chartInstance,
  int32_T msgSSID);

/* Type Definitions */

/* Named Constants */
#define CALL_EVENT                     (-1)

/* Variable Declarations */

/* Variable Definitions */
static real_T _sfTime_;
static const char * c2_debug_family_names[26] = { "workspace", "rocket", "CP",
  "CM", "k", "tol", "delta", "current_mass", "I", "f_x", "f_y", "moment", "a_x",
  "a_y", "alpha", "nargin", "nargout", "t", "t_step", "state", "rocketOD", "T",
  "mass", "wind", "aerodata", "statedot" };

static const char * c2_b_debug_family_names[19] = { "mu", "Tlaunch", "T0",
  "Trate", "hpos", "g", "avogadro", "k_boltz", "m_air", "R_air", "P_sl", "k",
  "nargin", "nargout", "h", "T", "P", "rho", "a" };

static const char * c2_c_debug_family_names[30] = { "umag", "phi", "resultant",
  "lambda", "aoa", "rho", "local_c", "mu", "mach", "k", "tol", "delta", "ft2m",
  "rocket_length", "Re_rocket_y", "rail_length", "launch_site", "weight", "CD",
  "CL", "S", "nargin", "nargout", "r", "u", "wind", "aerodata", "rocket",
  "Fdrag", "Flift" };

static const char * c2_d_debug_family_names[33] = { "theta", "k", "tol", "delta",
  "thrust", "Tx", "Ty", "Fdrag", "Flift", "Lx", "Ly", "Dx", "Dy", "G", "r_earth",
  "gravity", "site_elevation", "nargin", "nargout", "t", "t_step", "r", "u", "T",
  "current_mass", "wind", "aerodata", "rocket", "CP", "CM", "f_x", "f_y",
  "moment" };

/* Function Declarations */
static void initialize_c2_solver_simulink(SFc2_solver_simulinkInstanceStruct
  *chartInstance);
static void initialize_params_c2_solver_simulink
  (SFc2_solver_simulinkInstanceStruct *chartInstance);
static void enable_c2_solver_simulink(SFc2_solver_simulinkInstanceStruct
  *chartInstance);
static void disable_c2_solver_simulink(SFc2_solver_simulinkInstanceStruct
  *chartInstance);
static void c2_update_debugger_state_c2_solver_simulink
  (SFc2_solver_simulinkInstanceStruct *chartInstance);
static const mxArray *get_sim_state_c2_solver_simulink
  (SFc2_solver_simulinkInstanceStruct *chartInstance);
static void set_sim_state_c2_solver_simulink(SFc2_solver_simulinkInstanceStruct *
  chartInstance, const mxArray *c2_st);
static void finalize_c2_solver_simulink(SFc2_solver_simulinkInstanceStruct
  *chartInstance);
static void sf_gateway_c2_solver_simulink(SFc2_solver_simulinkInstanceStruct
  *chartInstance);
static void mdl_start_c2_solver_simulink(SFc2_solver_simulinkInstanceStruct
  *chartInstance);
static void c2_chartstep_c2_solver_simulink(SFc2_solver_simulinkInstanceStruct
  *chartInstance);
static void initSimStructsc2_solver_simulink(SFc2_solver_simulinkInstanceStruct *
  chartInstance);
static void c2_aerodynamics(SFc2_solver_simulinkInstanceStruct *chartInstance,
  real_T c2_r[3], real_T c2_u[3], real_T c2_b_wind, real_T c2_d_aerodata[17507],
  c2_sv2voxIt8FKzjMlUEnyXmnC *c2_rocket, real_T *c2_Fdrag, real_T *c2_Flift);
static void c2_getAtmoConditions(SFc2_solver_simulinkInstanceStruct
  *chartInstance, real_T c2_h, real_T *c2_b_T, real_T *c2_P, real_T *c2_rho,
  real_T *c2_a);
static void init_script_number_translation(uint32_T c2_machineNumber, uint32_T
  c2_chartNumber, uint32_T c2_instanceNumber);
static void c2_emlrt_marshallIn(SFc2_solver_simulinkInstanceStruct
  *chartInstance, const mxArray *c2_load, const char_T *c2_identifier,
  c2_szHckfuHwXXVCwxC2kXrI2C *c2_b_y);
static void c2_b_emlrt_marshallIn(SFc2_solver_simulinkInstanceStruct
  *chartInstance, const mxArray *c2_u, const emlrtMsgIdentifier *c2_parentId,
  c2_szHckfuHwXXVCwxC2kXrI2C *c2_b_y);
static real_T c2_c_emlrt_marshallIn(SFc2_solver_simulinkInstanceStruct
  *chartInstance, const mxArray *c2_u, const emlrtMsgIdentifier *c2_parentId);
static void c2_d_emlrt_marshallIn(SFc2_solver_simulinkInstanceStruct
  *chartInstance, const mxArray *c2_u, const emlrtMsgIdentifier *c2_parentId,
  c2_sv2voxIt8FKzjMlUEnyXmnC *c2_b_y);
static c2_sof5eNcdcizKAe6jjm9xhtG c2_e_emlrt_marshallIn
  (SFc2_solver_simulinkInstanceStruct *chartInstance, const mxArray *c2_u, const
   emlrtMsgIdentifier *c2_parentId);
static void c2_f_emlrt_marshallIn(SFc2_solver_simulinkInstanceStruct
  *chartInstance, const mxArray *c2_u, const emlrtMsgIdentifier *c2_parentId,
  char_T c2_b_y[9]);
static c2_syx7FYmGk3I6e3U1RUwUAWD c2_g_emlrt_marshallIn
  (SFc2_solver_simulinkInstanceStruct *chartInstance, const mxArray *c2_u, const
   emlrtMsgIdentifier *c2_parentId);
static c2_sqGsSc4djWypCV90lhUGiL c2_h_emlrt_marshallIn
  (SFc2_solver_simulinkInstanceStruct *chartInstance, const mxArray *c2_u, const
   emlrtMsgIdentifier *c2_parentId);
static c2_su2bTaGhbTlLQVXsqMBjEuH c2_i_emlrt_marshallIn
  (SFc2_solver_simulinkInstanceStruct *chartInstance, const mxArray *c2_u, const
   emlrtMsgIdentifier *c2_parentId);
static c2_sNlKjRKFHwjFKC34GeUJivG c2_j_emlrt_marshallIn
  (SFc2_solver_simulinkInstanceStruct *chartInstance, const mxArray *c2_u, const
   emlrtMsgIdentifier *c2_parentId);
static c2_sKS3cuSxR3HIcTIZZkbRPm c2_k_emlrt_marshallIn
  (SFc2_solver_simulinkInstanceStruct *chartInstance, const mxArray *c2_u, const
   emlrtMsgIdentifier *c2_parentId);
static c2_s88seaxzVTYYZIzTGqmP4AD c2_l_emlrt_marshallIn
  (SFc2_solver_simulinkInstanceStruct *chartInstance, const mxArray *c2_u, const
   emlrtMsgIdentifier *c2_parentId);
static c2_sfpFxKVMN8elArLoV353vAE c2_m_emlrt_marshallIn
  (SFc2_solver_simulinkInstanceStruct *chartInstance, const mxArray *c2_u, const
   emlrtMsgIdentifier *c2_parentId);
static const mxArray *c2_sf_marshallOut(void *chartInstanceVoid, void *c2_inData);
static void c2_n_emlrt_marshallIn(SFc2_solver_simulinkInstanceStruct
  *chartInstance, const mxArray *c2_b_statedot, const char_T *c2_identifier,
  real_T c2_b_y[6]);
static void c2_o_emlrt_marshallIn(SFc2_solver_simulinkInstanceStruct
  *chartInstance, const mxArray *c2_u, const emlrtMsgIdentifier *c2_parentId,
  real_T c2_b_y[6]);
static void c2_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c2_mxArrayInData, const char_T *c2_varName, void *c2_outData);
static const mxArray *c2_b_sf_marshallOut(void *chartInstanceVoid, void
  *c2_inData);
static const mxArray *c2_c_sf_marshallOut(void *chartInstanceVoid, void
  *c2_inData);
static const mxArray *c2_d_sf_marshallOut(void *chartInstanceVoid, void
  *c2_inData);
static void c2_b_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c2_mxArrayInData, const char_T *c2_varName, void *c2_outData);
static const mxArray *c2_e_sf_marshallOut(void *chartInstanceVoid, void
  *c2_inData);
static void c2_c_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c2_mxArrayInData, const char_T *c2_varName, void *c2_outData);
static const mxArray *c2_f_sf_marshallOut(void *chartInstanceVoid, void
  *c2_inData);
static void c2_d_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c2_mxArrayInData, const char_T *c2_varName, void *c2_outData);
static const mxArray *c2_g_sf_marshallOut(void *chartInstanceVoid, void
  *c2_inData);
static void c2_p_emlrt_marshallIn(SFc2_solver_simulinkInstanceStruct
  *chartInstance, const mxArray *c2_u, const emlrtMsgIdentifier *c2_parentId,
  real_T c2_b_y[7]);
static void c2_e_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c2_mxArrayInData, const char_T *c2_varName, void *c2_outData);
static const mxArray *c2_h_sf_marshallOut(void *chartInstanceVoid, void
  *c2_inData);
static void c2_q_emlrt_marshallIn(SFc2_solver_simulinkInstanceStruct
  *chartInstance, const mxArray *c2_u, const emlrtMsgIdentifier *c2_parentId,
  real_T c2_b_y[8]);
static void c2_f_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c2_mxArrayInData, const char_T *c2_varName, void *c2_outData);
static void c2_r_emlrt_marshallIn(SFc2_solver_simulinkInstanceStruct
  *chartInstance, const mxArray *c2_u, const emlrtMsgIdentifier *c2_parentId,
  real_T c2_b_y[17507]);
static void c2_g_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c2_mxArrayInData, const char_T *c2_varName, void *c2_outData);
static const mxArray *c2_i_sf_marshallOut(void *chartInstanceVoid, void
  *c2_inData);
static void c2_s_emlrt_marshallIn(SFc2_solver_simulinkInstanceStruct
  *chartInstance, const mxArray *c2_u, const emlrtMsgIdentifier *c2_parentId,
  real_T c2_b_y[3]);
static void c2_h_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c2_mxArrayInData, const char_T *c2_varName, void *c2_outData);
static void c2_t_emlrt_marshallIn(SFc2_solver_simulinkInstanceStruct
  *chartInstance, const mxArray *c2_u, const emlrtMsgIdentifier *c2_parentId,
  real_T c2_b_y[1256]);
static void c2_i_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c2_mxArrayInData, const char_T *c2_varName, void *c2_outData);
static void c2_error(SFc2_solver_simulinkInstanceStruct *chartInstance);
static real_T c2_sind(SFc2_solver_simulinkInstanceStruct *chartInstance, real_T
                      c2_x);
static real_T c2_cosd(SFc2_solver_simulinkInstanceStruct *chartInstance, real_T
                      c2_x);
static real_T c2_norm(SFc2_solver_simulinkInstanceStruct *chartInstance, real_T
                      c2_x[2]);
static void c2_b_error(SFc2_solver_simulinkInstanceStruct *chartInstance);
static void c2_c_error(SFc2_solver_simulinkInstanceStruct *chartInstance);
static const mxArray *c2_j_sf_marshallOut(void *chartInstanceVoid, void
  *c2_inData);
static int32_T c2_u_emlrt_marshallIn(SFc2_solver_simulinkInstanceStruct
  *chartInstance, const mxArray *c2_u, const emlrtMsgIdentifier *c2_parentId);
static void c2_j_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c2_mxArrayInData, const char_T *c2_varName, void *c2_outData);
static uint8_T c2_v_emlrt_marshallIn(SFc2_solver_simulinkInstanceStruct
  *chartInstance, const mxArray *c2_b_is_active_c2_solver_simulink, const char_T
  *c2_identifier);
static uint8_T c2_w_emlrt_marshallIn(SFc2_solver_simulinkInstanceStruct
  *chartInstance, const mxArray *c2_u, const emlrtMsgIdentifier *c2_parentId);
static void c2_b_sind(SFc2_solver_simulinkInstanceStruct *chartInstance, real_T *
                      c2_x);
static void c2_b_cosd(SFc2_solver_simulinkInstanceStruct *chartInstance, real_T *
                      c2_x);
static void init_dsm_address_info(SFc2_solver_simulinkInstanceStruct
  *chartInstance);
static void init_simulink_io_address(SFc2_solver_simulinkInstanceStruct
  *chartInstance);

/* Function Definitions */
static void initialize_c2_solver_simulink(SFc2_solver_simulinkInstanceStruct
  *chartInstance)
{
  if (sf_is_first_init_cond(chartInstance->S)) {
    initSimStructsc2_solver_simulink(chartInstance);
    chart_debug_initialize_data_addresses(chartInstance->S);
  }

  chartInstance->c2_sfEvent = CALL_EVENT;
  _sfTime_ = sf_get_time(chartInstance->S);
  chartInstance->c2_is_active_c2_solver_simulink = 0U;
}

static void initialize_params_c2_solver_simulink
  (SFc2_solver_simulinkInstanceStruct *chartInstance)
{
  (void)chartInstance;
}

static void enable_c2_solver_simulink(SFc2_solver_simulinkInstanceStruct
  *chartInstance)
{
  _sfTime_ = sf_get_time(chartInstance->S);
}

static void disable_c2_solver_simulink(SFc2_solver_simulinkInstanceStruct
  *chartInstance)
{
  _sfTime_ = sf_get_time(chartInstance->S);
}

static void c2_update_debugger_state_c2_solver_simulink
  (SFc2_solver_simulinkInstanceStruct *chartInstance)
{
  (void)chartInstance;
}

static const mxArray *get_sim_state_c2_solver_simulink
  (SFc2_solver_simulinkInstanceStruct *chartInstance)
{
  const mxArray *c2_st;
  const mxArray *c2_b_y = NULL;
  const mxArray *c2_c_y = NULL;
  uint8_T c2_hoistedGlobal;
  const mxArray *c2_d_y = NULL;
  c2_st = NULL;
  c2_st = NULL;
  c2_b_y = NULL;
  sf_mex_assign(&c2_b_y, sf_mex_createcellmatrix(2, 1), false);
  c2_c_y = NULL;
  sf_mex_assign(&c2_c_y, sf_mex_create("y", *chartInstance->c2_statedot, 0, 0U,
    1U, 0U, 1, 6), false);
  sf_mex_setcell(c2_b_y, 0, c2_c_y);
  c2_hoistedGlobal = chartInstance->c2_is_active_c2_solver_simulink;
  c2_d_y = NULL;
  sf_mex_assign(&c2_d_y, sf_mex_create("y", &c2_hoistedGlobal, 3, 0U, 0U, 0U, 0),
                false);
  sf_mex_setcell(c2_b_y, 1, c2_d_y);
  sf_mex_assign(&c2_st, c2_b_y, false);
  return c2_st;
}

static void set_sim_state_c2_solver_simulink(SFc2_solver_simulinkInstanceStruct *
  chartInstance, const mxArray *c2_st)
{
  const mxArray *c2_u;
  real_T c2_dv0[6];
  int32_T c2_i0;
  chartInstance->c2_doneDoubleBufferReInit = true;
  c2_u = sf_mex_dup(c2_st);
  c2_n_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getcell("statedot",
    c2_u, 0)), "statedot", c2_dv0);
  for (c2_i0 = 0; c2_i0 < 6; c2_i0++) {
    (*chartInstance->c2_statedot)[c2_i0] = c2_dv0[c2_i0];
  }

  chartInstance->c2_is_active_c2_solver_simulink = c2_v_emlrt_marshallIn
    (chartInstance, sf_mex_dup(sf_mex_getcell("is_active_c2_solver_simulink",
       c2_u, 1)), "is_active_c2_solver_simulink");
  sf_mex_destroy(&c2_u);
  c2_update_debugger_state_c2_solver_simulink(chartInstance);
  sf_mex_destroy(&c2_st);
}

static void finalize_c2_solver_simulink(SFc2_solver_simulinkInstanceStruct
  *chartInstance)
{
  (void)chartInstance;
}

static void sf_gateway_c2_solver_simulink(SFc2_solver_simulinkInstanceStruct
  *chartInstance)
{
  int32_T c2_i1;
  int32_T c2_i2;
  int32_T c2_i3;
  int32_T c2_i4;
  int32_T c2_i5;
  _SFD_SYMBOL_SCOPE_PUSH(0U, 0U);
  _sfTime_ = sf_get_time(chartInstance->S);
  _SFD_CC_CALL(CHART_ENTER_SFUNCTION_TAG, 0U, chartInstance->c2_sfEvent);
  for (c2_i1 = 0; c2_i1 < 17507; c2_i1++) {
    _SFD_DATA_RANGE_CHECK((*chartInstance->c2_c_aerodata)[c2_i1], 7U, 1U, 0U,
                          chartInstance->c2_sfEvent, false);
  }

  _SFD_DATA_RANGE_CHECK(*chartInstance->c2_wind, 6U, 1U, 0U,
                        chartInstance->c2_sfEvent, false);
  for (c2_i2 = 0; c2_i2 < 1256; c2_i2++) {
    _SFD_DATA_RANGE_CHECK((*chartInstance->c2_mass)[c2_i2], 5U, 1U, 0U,
                          chartInstance->c2_sfEvent, false);
  }

  for (c2_i3 = 0; c2_i3 < 1256; c2_i3++) {
    _SFD_DATA_RANGE_CHECK((*chartInstance->c2_T)[c2_i3], 4U, 1U, 0U,
                          chartInstance->c2_sfEvent, false);
  }

  _SFD_DATA_RANGE_CHECK(*chartInstance->c2_rocketOD, 3U, 1U, 0U,
                        chartInstance->c2_sfEvent, false);
  for (c2_i4 = 0; c2_i4 < 6; c2_i4++) {
    _SFD_DATA_RANGE_CHECK((*chartInstance->c2_state)[c2_i4], 2U, 1U, 0U,
                          chartInstance->c2_sfEvent, false);
  }

  _SFD_DATA_RANGE_CHECK(*chartInstance->c2_t_step, 1U, 1U, 0U,
                        chartInstance->c2_sfEvent, false);
  _SFD_DATA_RANGE_CHECK(*chartInstance->c2_t, 0U, 1U, 0U,
                        chartInstance->c2_sfEvent, false);
  chartInstance->c2_sfEvent = CALL_EVENT;
  c2_chartstep_c2_solver_simulink(chartInstance);
  _SFD_SYMBOL_SCOPE_POP();
  _SFD_CHECK_FOR_STATE_INCONSISTENCY(_solver_simulinkMachineNumber_,
    chartInstance->chartNumber, chartInstance->instanceNumber);
  for (c2_i5 = 0; c2_i5 < 6; c2_i5++) {
    _SFD_DATA_RANGE_CHECK((*chartInstance->c2_statedot)[c2_i5], 8U, 1U, 0U,
                          chartInstance->c2_sfEvent, false);
  }
}

static void mdl_start_c2_solver_simulink(SFc2_solver_simulinkInstanceStruct
  *chartInstance)
{
  (void)chartInstance;
}

static void c2_chartstep_c2_solver_simulink(SFc2_solver_simulinkInstanceStruct
  *chartInstance)
{
  real_T c2_hoistedGlobal;
  real_T c2_b_hoistedGlobal;
  real_T c2_c_hoistedGlobal;
  real_T c2_d_hoistedGlobal;
  real_T c2_b_t;
  real_T c2_b_t_step;
  int32_T c2_i6;
  real_T c2_b_rocketOD;
  real_T c2_b_state[6];
  int32_T c2_i7;
  int32_T c2_i8;
  real_T c2_b_T[1256];
  real_T c2_b_wind;
  real_T c2_b_mass[1256];
  int32_T c2_i9;
  uint32_T c2_debug_family_var_map[26];
  c2_szHckfuHwXXVCwxC2kXrI2C c2_workspace;
  c2_sv2voxIt8FKzjMlUEnyXmnC c2_rocket;
  real_T c2_CP;
  real_T c2_CM;
  real_T c2_k;
  real_T c2_tol;
  real_T c2_delta;
  real_T c2_current_mass;
  real_T c2_I;
  real_T c2_f_x;
  real_T c2_f_y;
  real_T c2_moment;
  real_T c2_a_x;
  real_T c2_a_y;
  real_T c2_alpha;
  real_T c2_nargin = 8.0;
  real_T c2_nargout = 1.0;
  real_T c2_b_statedot[6];
  const mxArray *c2_b_y = NULL;
  c2_szHckfuHwXXVCwxC2kXrI2C c2_r0;
  int32_T c2_i10;
  int32_T c2_i11;
  real_T c2_a;
  real_T c2_b_a;
  real_T c2_x;
  real_T c2_c_a;
  real_T c2_c_y;
  real_T c2_b_x;
  boolean_T c2_p;
  real_T c2_c_x;
  real_T c2_d_x;
  real_T c2_A;
  real_T c2_e_x;
  real_T c2_g_x;
  real_T c2_c_t;
  real_T c2_c_t_step;
  int32_T c2_i12;
  int32_T c2_i13;
  real_T c2_r[3];
  int32_T c2_i14;
  real_T c2_u[3];
  real_T c2_b_current_mass;
  real_T c2_c_T[1256];
  real_T c2_c_wind;
  int32_T c2_i15;
  c2_sv2voxIt8FKzjMlUEnyXmnC c2_b_rocket;
  real_T c2_b_CP;
  real_T c2_b_CM;
  uint32_T c2_b_debug_family_var_map[33];
  real_T c2_theta;
  real_T c2_b_k;
  real_T c2_b_tol;
  real_T c2_b_delta;
  real_T c2_thrust;
  real_T c2_Tx;
  real_T c2_Ty;
  real_T c2_Fdrag;
  real_T c2_Flift;
  real_T c2_Lx;
  real_T c2_Ly;
  real_T c2_Dx;
  real_T c2_Dy;
  real_T c2_G;
  real_T c2_r_earth;
  real_T c2_gravity;
  real_T c2_site_elevation;
  real_T c2_b_nargin = 11.0;
  real_T c2_b_nargout = 3.0;
  real_T c2_b_f_x;
  real_T c2_b_f_y;
  real_T c2_b_moment;
  int32_T c2_i16;
  real_T c2_h_x;
  int32_T c2_i17;
  real_T c2_b_r[3];
  real_T c2_i_x;
  real_T c2_j_x;
  int32_T c2_i18;
  real_T c2_b_u[3];
  c2_sv2voxIt8FKzjMlUEnyXmnC c2_c_rocket;
  real_T c2_d_aerodata[17507];
  real_T c2_b_Fdrag;
  real_T c2_b_Flift;
  real_T c2_d0;
  real_T c2_d1;
  real_T c2_d2;
  real_T c2_d3;
  real_T c2_d4;
  real_T c2_d5;
  int32_T c2_i19;
  real_T c2_d_a;
  real_T c2_c_r[2];
  real_T c2_e_a;
  real_T c2_k_x;
  real_T c2_f_a;
  real_T c2_d_y;
  boolean_T c2_b_p;
  real_T c2_B;
  real_T c2_e_y;
  real_T c2_g_y;
  real_T c2_h_y;
  real_T c2_d6;
  real_T c2_d7;
  real_T c2_b_A;
  real_T c2_b_B;
  real_T c2_l_x;
  real_T c2_i_y;
  real_T c2_m_x;
  real_T c2_j_y;
  real_T c2_c_A;
  real_T c2_c_B;
  real_T c2_n_x;
  real_T c2_k_y;
  real_T c2_o_x;
  real_T c2_l_y;
  real_T c2_d_A;
  real_T c2_d_B;
  real_T c2_p_x;
  real_T c2_m_y;
  real_T c2_q_x;
  real_T c2_n_y;
  int32_T c2_i20;
  int32_T exitg1;
  int32_T exitg2;
  _SFD_CC_CALL(CHART_ENTER_DURING_FUNCTION_TAG, 0U, chartInstance->c2_sfEvent);
  c2_hoistedGlobal = *chartInstance->c2_t;
  c2_b_hoistedGlobal = *chartInstance->c2_t_step;
  c2_c_hoistedGlobal = *chartInstance->c2_rocketOD;
  c2_d_hoistedGlobal = *chartInstance->c2_wind;
  c2_b_t = c2_hoistedGlobal;
  c2_b_t_step = c2_b_hoistedGlobal;
  for (c2_i6 = 0; c2_i6 < 6; c2_i6++) {
    c2_b_state[c2_i6] = (*chartInstance->c2_state)[c2_i6];
  }

  c2_b_rocketOD = c2_c_hoistedGlobal;
  for (c2_i7 = 0; c2_i7 < 1256; c2_i7++) {
    c2_b_T[c2_i7] = (*chartInstance->c2_T)[c2_i7];
  }

  for (c2_i8 = 0; c2_i8 < 1256; c2_i8++) {
    c2_b_mass[c2_i8] = (*chartInstance->c2_mass)[c2_i8];
  }

  c2_b_wind = c2_d_hoistedGlobal;
  for (c2_i9 = 0; c2_i9 < 17507; c2_i9++) {
    chartInstance->c2_aerodata[c2_i9] = (*chartInstance->c2_c_aerodata)[c2_i9];
  }

  _SFD_SYMBOL_SCOPE_PUSH_EML(0U, 26U, 26U, c2_debug_family_names,
    c2_debug_family_var_map);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_workspace, 0U, c2_f_sf_marshallOut,
    c2_d_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_rocket, 1U, c2_e_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_CP, 2U, c2_c_sf_marshallOut,
    c2_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_CM, 3U, c2_c_sf_marshallOut,
    c2_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_k, 4U, c2_c_sf_marshallOut,
    c2_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_tol, 5U, c2_c_sf_marshallOut,
    c2_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_delta, 6U, c2_c_sf_marshallOut,
    c2_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_current_mass, 7U, c2_c_sf_marshallOut,
    c2_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_I, 8U, c2_c_sf_marshallOut,
    c2_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_f_x, 9U, c2_c_sf_marshallOut,
    c2_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_f_y, 10U, c2_c_sf_marshallOut,
    c2_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_moment, 11U, c2_c_sf_marshallOut,
    c2_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_a_x, 12U, c2_c_sf_marshallOut,
    c2_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_a_y, 13U, c2_c_sf_marshallOut,
    c2_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_alpha, 14U, c2_c_sf_marshallOut,
    c2_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_nargin, 15U, c2_c_sf_marshallOut,
    c2_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_nargout, 16U, c2_c_sf_marshallOut,
    c2_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c2_b_t, 17U, c2_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c2_b_t_step, 18U, c2_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(c2_b_state, 19U, c2_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c2_b_rocketOD, 20U, c2_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(c2_b_T, 21U, c2_d_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(c2_b_mass, 22U, c2_d_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c2_b_wind, 23U, c2_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(chartInstance->c2_aerodata, 24U, c2_b_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c2_b_statedot, 25U, c2_sf_marshallOut,
    c2_sf_marshallIn);
  CV_EML_FCN(0, 0);
  _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 3);
  c2_b_y = NULL;
  sf_mex_assign(&c2_b_y, sf_mex_create("y", "import2simulink.mat", 15, 0U, 0U,
    0U, 2, 1, strlen("import2simulink.mat")), false);
  c2_emlrt_marshallIn(chartInstance, sf_mex_call_debug
                      (sfGlobalDebugInstanceStruct, "load", 1U, 1U, 14, c2_b_y),
                      "load", &c2_r0);
  c2_workspace = c2_r0;
  _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 4);
  c2_rocket = c2_workspace.rocket;
  _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 5);
  c2_CP = c2_workspace.CP;
  _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 6);
  c2_CM = c2_workspace.CM;
  _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 10);
  for (c2_i10 = 0; c2_i10 < 6; c2_i10++) {
    c2_b_statedot[c2_i10] = 0.0;
  }

  _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 11);
  for (c2_i11 = 0; c2_i11 < 3; c2_i11++) {
    c2_b_statedot[c2_i11] = c2_b_state[c2_i11 + 3];
  }

  _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 14);
  if (CV_EML_IF(0, 1, 0, CV_RELATIONAL_EVAL(4U, 0U, 0, c2_b_t, c2_b_mass[627],
        -1, 3U, c2_b_t <= c2_b_mass[627]))) {
    _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 15);
    c2_k = 1.0;
    _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 15);
    c2_tol = c2_b_t_step;
    _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 16);
    do {
      exitg2 = 0;
      CV_EML_WHILE(0, 1, 0, true);
      _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 17);
      c2_b_x = c2_b_mass[sf_eml_array_bounds_check(sfGlobalDebugInstanceStruct,
        chartInstance->S, 1U, 473, 1, MAX_uint32_T, (int32_T)sf_integer_check
        (chartInstance->S, 1U, 473U, 1U, c2_k), 1, 628) - 1] - c2_b_t;
      c2_c_x = c2_b_x;
      c2_d_x = c2_c_x;
      c2_delta = muDoubleScalarAbs(c2_d_x);
      _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 18);
      if (CV_EML_IF(0, 1, 1, CV_RELATIONAL_EVAL(4U, 0U, 1, c2_delta, c2_tol, -1,
            2U, c2_delta < c2_tol))) {
        exitg2 = 1;
      } else {
        _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 19);
        c2_k++;
        _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 16);
        _SF_MEX_LISTEN_FOR_CTRL_C(chartInstance->S);
      }
    } while (exitg2 == 0);

    _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 18);
    _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 21);
    c2_current_mass = c2_b_mass[sf_eml_array_bounds_check
      (sfGlobalDebugInstanceStruct, chartInstance->S, 1U, 566, 1, MAX_uint32_T,
       (int32_T)sf_integer_check(chartInstance->S, 1U, 566U, 1U, c2_k), 1, 628)
      + 627];
  } else {
    _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 23);
    c2_current_mass = c2_b_mass[1255];
  }

  _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 26);
  c2_a = c2_b_rocketOD;
  c2_b_a = c2_a;
  c2_x = c2_b_a;
  c2_c_a = c2_x;
  c2_c_y = c2_c_a * c2_c_a;
  c2_p = false;
  if (c2_p) {
    c2_error(chartInstance);
  }

  c2_A = 0.5 * c2_current_mass * c2_c_y;
  c2_e_x = c2_A;
  c2_g_x = c2_e_x;
  c2_I = c2_g_x / 4.0;
  _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 29);
  c2_c_t = c2_b_t;
  c2_c_t_step = c2_b_t_step;
  for (c2_i12 = 0; c2_i12 < 3; c2_i12++) {
    c2_r[c2_i12] = c2_b_state[c2_i12];
  }

  for (c2_i13 = 0; c2_i13 < 3; c2_i13++) {
    c2_u[c2_i13] = c2_b_state[c2_i13 + 3];
  }

  for (c2_i14 = 0; c2_i14 < 1256; c2_i14++) {
    c2_c_T[c2_i14] = c2_b_T[c2_i14];
  }

  c2_b_current_mass = c2_current_mass;
  c2_c_wind = c2_b_wind;
  for (c2_i15 = 0; c2_i15 < 17507; c2_i15++) {
    chartInstance->c2_b_aerodata[c2_i15] = chartInstance->c2_aerodata[c2_i15];
  }

  c2_b_rocket = c2_rocket;
  c2_b_CP = c2_CP;
  c2_b_CM = c2_CM;
  _SFD_SYMBOL_SCOPE_PUSH_EML(0U, 33U, 33U, c2_d_debug_family_names,
    c2_b_debug_family_var_map);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_theta, 0U, c2_c_sf_marshallOut,
    c2_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_b_k, 1U, c2_c_sf_marshallOut,
    c2_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_b_tol, 2U, c2_c_sf_marshallOut,
    c2_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_b_delta, 3U, c2_c_sf_marshallOut,
    c2_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_thrust, 4U, c2_c_sf_marshallOut,
    c2_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_Tx, 5U, c2_c_sf_marshallOut,
    c2_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_Ty, 6U, c2_c_sf_marshallOut,
    c2_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_Fdrag, 7U, c2_c_sf_marshallOut,
    c2_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_Flift, 8U, c2_c_sf_marshallOut,
    c2_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_Lx, 9U, c2_c_sf_marshallOut,
    c2_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_Ly, 10U, c2_c_sf_marshallOut,
    c2_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_Dx, 11U, c2_c_sf_marshallOut,
    c2_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_Dy, 12U, c2_c_sf_marshallOut,
    c2_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c2_G, 13U, c2_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_r_earth, 14U, c2_c_sf_marshallOut,
    c2_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_gravity, 15U, c2_c_sf_marshallOut,
    c2_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_site_elevation, 16U,
    c2_c_sf_marshallOut, c2_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_b_nargin, 17U, c2_c_sf_marshallOut,
    c2_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_b_nargout, 18U, c2_c_sf_marshallOut,
    c2_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_c_t, 19U, c2_c_sf_marshallOut,
    c2_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_c_t_step, 20U, c2_c_sf_marshallOut,
    c2_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c2_r, 21U, c2_i_sf_marshallOut,
    c2_h_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c2_u, 22U, c2_i_sf_marshallOut,
    c2_h_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c2_c_T, 23U, c2_d_sf_marshallOut,
    c2_i_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_b_current_mass, 24U,
    c2_c_sf_marshallOut, c2_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_c_wind, 25U, c2_c_sf_marshallOut,
    c2_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(chartInstance->c2_b_aerodata, 26U,
    c2_b_sf_marshallOut, c2_g_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_b_rocket, 27U, c2_e_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_b_CP, 28U, c2_c_sf_marshallOut,
    c2_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_b_CM, 29U, c2_c_sf_marshallOut,
    c2_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_b_f_x, 30U, c2_c_sf_marshallOut,
    c2_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_b_f_y, 31U, c2_c_sf_marshallOut,
    c2_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_b_moment, 32U, c2_c_sf_marshallOut,
    c2_b_sf_marshallIn);
  CV_SCRIPT_FCN(0, 0);
  _SFD_SCRIPT_CALL(0U, chartInstance->c2_sfEvent, 15);
  c2_theta = c2_r[2];
  _SFD_SCRIPT_CALL(0U, chartInstance->c2_sfEvent, 20);
  if (CV_SCRIPT_IF(0, 0, CV_RELATIONAL_EVAL(14U, 0U, 0, c2_c_t, c2_c_T[627], -1,
        2U, c2_c_t < c2_c_T[627]))) {
    _SFD_SCRIPT_CALL(0U, chartInstance->c2_sfEvent, 21);
    c2_b_k = 1.0;
    _SFD_SCRIPT_CALL(0U, chartInstance->c2_sfEvent, 21);
    c2_b_tol = c2_c_t_step;
    _SFD_SCRIPT_CALL(0U, chartInstance->c2_sfEvent, 22);
    do {
      exitg1 = 0;
      CV_SCRIPT_WHILE(0, 0, true);
      _SFD_SCRIPT_CALL(0U, chartInstance->c2_sfEvent, 23);
      c2_h_x = c2_c_T[sf_eml_array_bounds_check(sfGlobalDebugInstanceStruct,
        chartInstance->S, 1U, 732, 118, MAX_uint32_T, (int32_T)sf_integer_check
        (chartInstance->S, 1U, 732U, 118U, c2_b_k), 1, 628) - 1] - c2_c_t;
      c2_i_x = c2_h_x;
      c2_j_x = c2_i_x;
      c2_b_delta = muDoubleScalarAbs(c2_j_x);
      _SFD_SCRIPT_CALL(0U, chartInstance->c2_sfEvent, 24);
      if (CV_SCRIPT_IF(0, 1, CV_RELATIONAL_EVAL(14U, 0U, 1, c2_b_delta, c2_b_tol,
            -1, 2U, c2_b_delta < c2_b_tol))) {
        exitg1 = 1;
      } else {
        _SFD_SCRIPT_CALL(0U, chartInstance->c2_sfEvent, 25);
        c2_b_k++;
        _SFD_SCRIPT_CALL(0U, chartInstance->c2_sfEvent, 22);
        _SF_MEX_LISTEN_FOR_CTRL_C(chartInstance->S);
      }
    } while (exitg1 == 0);

    _SFD_SCRIPT_CALL(0U, chartInstance->c2_sfEvent, 24);
    _SFD_SCRIPT_CALL(0U, chartInstance->c2_sfEvent, 27);
    c2_thrust = c2_c_T[sf_eml_array_bounds_check(sfGlobalDebugInstanceStruct,
      chartInstance->S, 1U, 732, 118, MAX_uint32_T, (int32_T)sf_integer_check
      (chartInstance->S, 1U, 732U, 118U, c2_b_k), 1, 628) + 627];
    _SFD_SCRIPT_CALL(0U, chartInstance->c2_sfEvent, 28);
    c2_d0 = c2_theta;
    c2_b_sind(chartInstance, &c2_d0);
    c2_Tx = c2_d0 * c2_thrust;
    _SFD_SCRIPT_CALL(0U, chartInstance->c2_sfEvent, 29);
    c2_d2 = c2_theta;
    c2_b_cosd(chartInstance, &c2_d2);
    c2_Ty = c2_d2 * c2_thrust;
  } else {
    _SFD_SCRIPT_CALL(0U, chartInstance->c2_sfEvent, 31);
    c2_Tx = 0.0;
    _SFD_SCRIPT_CALL(0U, chartInstance->c2_sfEvent, 32);
    c2_Ty = 0.0;
  }

  _SFD_SCRIPT_CALL(0U, chartInstance->c2_sfEvent, 36);
  for (c2_i16 = 0; c2_i16 < 3; c2_i16++) {
    c2_b_r[c2_i16] = c2_r[c2_i16];
  }

  for (c2_i17 = 0; c2_i17 < 3; c2_i17++) {
    c2_b_u[c2_i17] = c2_u[c2_i17];
  }

  for (c2_i18 = 0; c2_i18 < 17507; c2_i18++) {
    c2_d_aerodata[c2_i18] = chartInstance->c2_b_aerodata[c2_i18];
  }

  c2_c_rocket = c2_b_rocket;
  c2_aerodynamics(chartInstance, c2_b_r, c2_b_u, c2_c_wind, c2_d_aerodata,
                  &c2_c_rocket, &c2_b_Fdrag, &c2_b_Flift);
  c2_Fdrag = c2_b_Fdrag;
  c2_Flift = c2_b_Flift;
  _SFD_SCRIPT_CALL(0U, chartInstance->c2_sfEvent, 39);
  c2_d1 = c2_theta;
  c2_b_cosd(chartInstance, &c2_d1);
  c2_Lx = c2_d1 * c2_Flift;
  _SFD_SCRIPT_CALL(0U, chartInstance->c2_sfEvent, 40);
  c2_d3 = c2_theta;
  c2_b_sind(chartInstance, &c2_d3);
  c2_Ly = c2_d3 * c2_Flift;
  _SFD_SCRIPT_CALL(0U, chartInstance->c2_sfEvent, 43);
  c2_d4 = c2_theta;
  c2_b_sind(chartInstance, &c2_d4);
  c2_Dx = c2_d4 * c2_Fdrag;
  _SFD_SCRIPT_CALL(0U, chartInstance->c2_sfEvent, 44);
  c2_d5 = c2_theta;
  c2_b_cosd(chartInstance, &c2_d5);
  c2_Dy = c2_d5 * c2_Fdrag;
  _SFD_SCRIPT_CALL(0U, chartInstance->c2_sfEvent, 49);
  c2_G = 3.986E+14;
  _SFD_SCRIPT_CALL(0U, chartInstance->c2_sfEvent, 49);
  c2_r_earth = 6.378E+6;
  _SFD_SCRIPT_CALL(0U, chartInstance->c2_sfEvent, 50);
  for (c2_i19 = 0; c2_i19 < 2; c2_i19++) {
    c2_c_r[c2_i19] = c2_r[c2_i19];
  }

  c2_d_a = c2_r_earth + c2_norm(chartInstance, c2_c_r);
  c2_e_a = c2_d_a;
  c2_k_x = c2_e_a;
  c2_f_a = c2_k_x;
  c2_d_y = c2_f_a * c2_f_a;
  c2_b_p = false;
  if (c2_b_p) {
    c2_error(chartInstance);
  }

  c2_B = c2_d_y;
  c2_e_y = c2_B;
  c2_g_y = c2_e_y;
  c2_h_y = 3.986E+14 / c2_g_y;
  c2_gravity = c2_h_y * c2_b_current_mass;
  _SFD_SCRIPT_CALL(0U, chartInstance->c2_sfEvent, 52);
  c2_site_elevation = 1293.0;
  _SFD_SCRIPT_CALL(0U, chartInstance->c2_sfEvent, 53);
  if (CV_SCRIPT_IF(0, 2, CV_RELATIONAL_EVAL(14U, 0U, 2, c2_r[1],
        c2_site_elevation, -1, 3U, c2_r[1] <= c2_site_elevation))) {
    _SFD_SCRIPT_CALL(0U, chartInstance->c2_sfEvent, 54);
    c2_gravity = 0.0;
  }

  _SFD_SCRIPT_CALL(0U, chartInstance->c2_sfEvent, 58);
  if (CV_SCRIPT_IF(0, 3, CV_RELATIONAL_EVAL(14U, 0U, 3, c2_u[1], 0.0, -1, 5U,
        c2_u[1] >= 0.0))) {
    _SFD_SCRIPT_CALL(0U, chartInstance->c2_sfEvent, 59);
    c2_b_f_x = (c2_Tx + c2_Lx) - c2_Dx;
    _SFD_SCRIPT_CALL(0U, chartInstance->c2_sfEvent, 60);
    c2_b_f_y = ((c2_Ty + c2_Ly) - c2_Dy) - c2_gravity;
    _SFD_SCRIPT_CALL(0U, chartInstance->c2_sfEvent, 61);
    c2_d6 = c2_theta;
    c2_b_cosd(chartInstance, &c2_d6);
    c2_d7 = c2_theta;
    c2_b_sind(chartInstance, &c2_d7);
    c2_b_moment = (c2_Lx - c2_Dx) * c2_d6 * (c2_b_CP - c2_b_CM) + (c2_Dy + c2_Ly)
      * c2_d7 * (c2_b_CP - c2_b_CM);
  } else {
    _SFD_SCRIPT_CALL(0U, chartInstance->c2_sfEvent, 63);
    c2_b_f_x = c2_Dx;
    _SFD_SCRIPT_CALL(0U, chartInstance->c2_sfEvent, 64);
    c2_b_f_y = c2_Dy - c2_gravity;
    _SFD_SCRIPT_CALL(0U, chartInstance->c2_sfEvent, 65);
    c2_b_moment = 0.0;
  }

  _SFD_SCRIPT_CALL(0U, chartInstance->c2_sfEvent, -65);
  _SFD_SYMBOL_SCOPE_POP();
  c2_f_x = c2_b_f_x;
  c2_f_y = c2_b_f_y;
  c2_moment = c2_b_moment;
  _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 33);
  c2_b_A = c2_f_x;
  c2_b_B = c2_current_mass;
  c2_l_x = c2_b_A;
  c2_i_y = c2_b_B;
  c2_m_x = c2_l_x;
  c2_j_y = c2_i_y;
  c2_a_x = c2_m_x / c2_j_y;
  _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 34);
  c2_c_A = c2_f_y;
  c2_c_B = c2_current_mass;
  c2_n_x = c2_c_A;
  c2_k_y = c2_c_B;
  c2_o_x = c2_n_x;
  c2_l_y = c2_k_y;
  c2_a_y = c2_o_x / c2_l_y;
  _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 35);
  c2_d_A = c2_moment;
  c2_d_B = c2_I;
  c2_p_x = c2_d_A;
  c2_m_y = c2_d_B;
  c2_q_x = c2_p_x;
  c2_n_y = c2_m_y;
  c2_alpha = c2_q_x / c2_n_y;
  _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 36);
  c2_b_statedot[3] = c2_a_x;
  _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 37);
  c2_b_statedot[4] = c2_a_y;
  _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 38);
  c2_b_statedot[5] = c2_alpha;
  _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, -38);
  _SFD_SYMBOL_SCOPE_POP();
  for (c2_i20 = 0; c2_i20 < 6; c2_i20++) {
    (*chartInstance->c2_statedot)[c2_i20] = c2_b_statedot[c2_i20];
  }

  _SFD_CC_CALL(EXIT_OUT_OF_FUNCTION_TAG, 0U, chartInstance->c2_sfEvent);
}

static void initSimStructsc2_solver_simulink(SFc2_solver_simulinkInstanceStruct *
  chartInstance)
{
  (void)chartInstance;
}

static void c2_aerodynamics(SFc2_solver_simulinkInstanceStruct *chartInstance,
  real_T c2_r[3], real_T c2_u[3], real_T c2_b_wind, real_T c2_d_aerodata[17507],
  c2_sv2voxIt8FKzjMlUEnyXmnC *c2_rocket, real_T *c2_Fdrag, real_T *c2_Flift)
{
  uint32_T c2_debug_family_var_map[30];
  real_T c2_umag;
  real_T c2_phi;
  real_T c2_resultant;
  real_T c2_lambda;
  real_T c2_aoa;
  real_T c2_rho;
  real_T c2_local_c;
  real_T c2_mu;
  real_T c2_mach;
  real_T c2_k;
  real_T c2_tol;
  real_T c2_delta;
  real_T c2_ft2m;
  real_T c2_rocket_length;
  real_T c2_Re_rocket_y;
  real_T c2_rail_length;
  real_T c2_launch_site;
  real_T c2_weight;
  real_T c2_CD;
  real_T c2_CL;
  real_T c2_S;
  real_T c2_nargin = 6.0;
  real_T c2_nargout = 2.0;
  int32_T c2_i21;
  real_T c2_b_u[2];
  real_T c2_a;
  real_T c2_b_a;
  real_T c2_x;
  real_T c2_c_a;
  real_T c2_b_y;
  boolean_T c2_p;
  real_T c2_d_a;
  real_T c2_e_a;
  real_T c2_b_x;
  real_T c2_f_a;
  real_T c2_c_y;
  boolean_T c2_b_p;
  real_T c2_d8;
  real_T c2_d9;
  real_T c2_c_x;
  real_T c2_d_y;
  real_T c2_d_x;
  real_T c2_e_y;
  real_T c2_z;
  real_T c2_e_x;
  real_T c2_f_x;
  boolean_T c2_b0;
  boolean_T c2_b1;
  boolean_T c2_c_p;
  boolean_T c2_d_p;
  real_T c2_g_x;
  real_T c2_h_x;
  real_T c2_i_x;
  real_T c2_j_x;
  real_T c2_unusedU0;
  real_T c2_unusedU1;
  real_T c2_b_rho;
  real_T c2_b_local_c;
  real_T c2_A;
  real_T c2_B;
  real_T c2_k_x;
  real_T c2_f_y;
  real_T c2_l_x;
  real_T c2_g_y;
  real_T c2_m_x;
  real_T c2_n_x;
  real_T c2_o_x;
  real_T c2_b_A;
  real_T c2_p_x;
  real_T c2_q_x;
  real_T c2_r_x;
  real_T c2_s_x;
  real_T c2_t_x;
  real_T c2_h_y;
  real_T c2_u_x;
  real_T c2_v_x;
  real_T c2_w_x;
  real_T c2_i_y;
  real_T c2_x_x;
  real_T c2_c_A;
  real_T c2_y_x;
  real_T c2_ab_x;
  real_T c2_bb_x;
  real_T c2_cb_x;
  real_T c2_j_y;
  real_T c2_db_x;
  real_T c2_eb_x;
  real_T c2_fb_x;
  real_T c2_k_y;
  real_T c2_d_A;
  real_T c2_g_a;
  real_T c2_gb_x;
  real_T c2_h_a;
  real_T c2_hb_x;
  real_T c2_ib_x;
  real_T c2_i_a;
  const mxArray *c2_l_y = NULL;
  real_T c2_m_y;
  static char_T c2_cv0[11] = { 'y', 'o', ',', ' ', 'a', 'o', 'a', ' ', '>', ' ',
    '4' };

  boolean_T c2_e_p;
  real_T c2_jb_x;
  real_T c2_kb_x;
  real_T c2_d10;
  real_T c2_j_a;
  real_T c2_k_a;
  real_T c2_lb_x;
  real_T c2_l_a;
  real_T c2_n_y;
  boolean_T c2_f_p;
  real_T c2_d11;
  real_T c2_m_a;
  real_T c2_n_a;
  real_T c2_mb_x;
  real_T c2_o_a;
  real_T c2_o_y;
  boolean_T c2_g_p;
  int32_T exitg1;
  _SFD_SYMBOL_SCOPE_PUSH_EML(0U, 30U, 30U, c2_c_debug_family_names,
    c2_debug_family_var_map);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_umag, 0U, c2_c_sf_marshallOut,
    c2_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_phi, 1U, c2_c_sf_marshallOut,
    c2_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_resultant, 2U, c2_c_sf_marshallOut,
    c2_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_lambda, 3U, c2_c_sf_marshallOut,
    c2_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_aoa, 4U, c2_c_sf_marshallOut,
    c2_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_rho, 5U, c2_c_sf_marshallOut,
    c2_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_local_c, 6U, c2_c_sf_marshallOut,
    c2_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c2_mu, 7U, c2_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_mach, 8U, c2_c_sf_marshallOut,
    c2_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_k, 9U, c2_c_sf_marshallOut,
    c2_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_tol, 10U, c2_c_sf_marshallOut,
    c2_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_delta, 11U, c2_c_sf_marshallOut,
    c2_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_ft2m, 12U, c2_c_sf_marshallOut,
    c2_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_rocket_length, 13U,
    c2_c_sf_marshallOut, c2_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_Re_rocket_y, 14U, c2_c_sf_marshallOut,
    c2_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_rail_length, 15U, c2_c_sf_marshallOut,
    c2_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_launch_site, 16U, c2_c_sf_marshallOut,
    c2_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_weight, 17U, c2_c_sf_marshallOut,
    c2_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_CD, 18U, c2_c_sf_marshallOut,
    c2_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_CL, 19U, c2_c_sf_marshallOut,
    c2_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_S, 20U, c2_c_sf_marshallOut,
    c2_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_nargin, 21U, c2_c_sf_marshallOut,
    c2_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_nargout, 22U, c2_c_sf_marshallOut,
    c2_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c2_r, 23U, c2_i_sf_marshallOut,
    c2_h_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c2_u, 24U, c2_i_sf_marshallOut,
    c2_h_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_b_wind, 25U, c2_c_sf_marshallOut,
    c2_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c2_d_aerodata, 26U, c2_b_sf_marshallOut,
    c2_g_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c2_rocket, 27U, c2_e_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c2_Fdrag, 28U, c2_c_sf_marshallOut,
    c2_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c2_Flift, 29U, c2_c_sf_marshallOut,
    c2_b_sf_marshallIn);
  CV_SCRIPT_FCN(1, 0);
  _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 12);
  for (c2_i21 = 0; c2_i21 < 2; c2_i21++) {
    c2_b_u[c2_i21] = c2_u[c2_i21];
  }

  c2_umag = c2_norm(chartInstance, c2_b_u);
  _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 13);
  c2_phi = 90.0 - c2_r[2];
  _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 14);
  c2_a = c2_umag;
  c2_b_a = c2_a;
  c2_x = c2_b_a;
  c2_c_a = c2_x;
  c2_b_y = c2_c_a * c2_c_a;
  c2_p = false;
  if (c2_p) {
    c2_error(chartInstance);
  }

  c2_d_a = c2_b_wind;
  c2_e_a = c2_d_a;
  c2_b_x = c2_e_a;
  c2_f_a = c2_b_x;
  c2_c_y = c2_f_a * c2_f_a;
  c2_b_p = false;
  if (c2_b_p) {
    c2_error(chartInstance);
  }

  c2_d8 = c2_phi;
  c2_b_cosd(chartInstance, &c2_d8);
  c2_resultant = (c2_b_y + c2_c_y) - 2.0 * c2_umag * c2_b_wind * c2_d8;
  _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 15);
  c2_d9 = c2_phi;
  c2_b_sind(chartInstance, &c2_d9);
  c2_c_x = c2_b_wind * c2_d9;
  c2_d_y = c2_resultant;
  c2_d_x = c2_c_x;
  c2_e_y = c2_d_y;
  c2_z = c2_d_x / c2_e_y;
  c2_e_x = c2_z;
  c2_lambda = c2_e_x;
  c2_f_x = c2_lambda;
  c2_b0 = (c2_f_x < -1.0);
  c2_b1 = (c2_f_x > 1.0);
  c2_c_p = (c2_b0 || c2_b1);
  c2_d_p = c2_c_p;
  if (c2_d_p) {
    c2_b_error(chartInstance);
  }

  c2_g_x = c2_lambda;
  c2_lambda = c2_g_x;
  c2_h_x = c2_lambda;
  c2_lambda = c2_h_x;
  c2_i_x = c2_lambda;
  c2_j_x = c2_i_x;
  c2_j_x = muDoubleScalarAsin(c2_j_x);
  c2_lambda = 57.295779513082323 * c2_j_x;
  _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 16);
  c2_aoa = (180.0 - c2_lambda) - c2_phi;
  _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 19);
  c2_getAtmoConditions(chartInstance, c2_r[1], &c2_unusedU0, &c2_unusedU1,
                       &c2_b_rho, &c2_b_local_c);
  c2_rho = c2_b_rho;
  c2_local_c = c2_b_local_c;
  c2_mu = 1.846E-5;
  _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 22);
  c2_A = c2_umag;
  c2_B = c2_local_c;
  c2_k_x = c2_A;
  c2_f_y = c2_B;
  c2_l_x = c2_k_x;
  c2_g_y = c2_f_y;
  c2_mach = c2_l_x / c2_g_y;
  _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 23);
  c2_k = 1.0;
  _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 23);
  c2_tol = 0.01;
  _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 24);
  do {
    exitg1 = 0;
    CV_SCRIPT_WHILE(1, 0, true);
    _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 25);
    c2_m_x = c2_mach - c2_d_aerodata[sf_eml_array_bounds_check
      (sfGlobalDebugInstanceStruct, chartInstance->S, 0U, 0, 0, MAX_uint32_T,
       (int32_T)sf_integer_check(chartInstance->S, 0U, 0U, 0U, c2_k), 1, 2501) -
      1];
    c2_n_x = c2_m_x;
    c2_o_x = c2_n_x;
    c2_delta = muDoubleScalarAbs(c2_o_x);
    _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 26);
    if (CV_SCRIPT_IF(1, 0, CV_RELATIONAL_EVAL(14U, 1U, 0, c2_delta, c2_tol, -1,
          2U, c2_delta < c2_tol))) {
      exitg1 = 1;
    } else {
      _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 27);
      c2_k++;
      _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 24);
      _SF_MEX_LISTEN_FOR_CTRL_C(chartInstance->S);
    }
  } while (exitg1 == 0);

  _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 26);
  _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 30);
  c2_ft2m = 0.3048;
  _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 31);
  c2_rocket_length = 11.0 * c2_ft2m;
  _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 32);
  c2_b_A = c2_rho * c2_umag * c2_rocket_length;
  c2_p_x = c2_b_A;
  c2_q_x = c2_p_x;
  c2_Re_rocket_y = c2_q_x / 1.846E-5;
  _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 38);
  c2_rail_length = 20.0;
  _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 39);
  c2_launch_site = 1293.0;
  _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 41);
  if (CV_SCRIPT_IF(1, 1, CV_RELATIONAL_EVAL(14U, 1U, 1, c2_u[1], 0.0, -1, 5U,
        c2_u[1] >= 0.0))) {
    _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 43);
    if (CV_SCRIPT_IF(1, 2, CV_RELATIONAL_EVAL(14U, 1U, 2, c2_r[1],
          c2_rail_length + c2_launch_site, -1, 3U, c2_r[1] <= c2_rail_length +
          c2_launch_site))) {
      _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 44);
      c2_aoa = 0.0;
    }

    _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 49);
    c2_r_x = c2_aoa;
    c2_s_x = c2_r_x;
    c2_t_x = c2_s_x;
    c2_h_y = muDoubleScalarAbs(c2_t_x);
    c2_u_x = c2_aoa;
    c2_v_x = c2_u_x;
    c2_w_x = c2_v_x;
    c2_i_y = muDoubleScalarAbs(c2_w_x);
    if (CV_SCRIPT_IF(1, 3, CV_RELATIONAL_EVAL(14U, 1U, 3, c2_h_y, 2.0, -1, 3U,
          c2_i_y <= 2.0))) {
      _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 50);
      c2_c_A = c2_aoa;
      c2_ab_x = c2_c_A;
      c2_cb_x = c2_ab_x;
      c2_weight = c2_cb_x / 2.0;
      _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 51);
      c2_CD = c2_weight * c2_d_aerodata[sf_eml_array_bounds_check
        (sfGlobalDebugInstanceStruct, chartInstance->S, 0U, 0, 0, MAX_uint32_T,
         (int32_T)sf_integer_check(chartInstance->S, 0U, 0U, 0U, c2_k), 1, 2501)
        + 5001] + (1.0 - c2_weight) * c2_d_aerodata[sf_eml_array_bounds_check
        (sfGlobalDebugInstanceStruct, chartInstance->S, 0U, 0, 0, MAX_uint32_T,
         (int32_T)sf_integer_check(chartInstance->S, 0U, 0U, 0U, c2_k), 1, 2501)
        + 2500];
      _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 52);
      c2_CL = c2_weight * c2_d_aerodata[sf_eml_array_bounds_check
        (sfGlobalDebugInstanceStruct, chartInstance->S, 0U, 0, 0, MAX_uint32_T,
         (int32_T)sf_integer_check(chartInstance->S, 0U, 0U, 0U, c2_k), 1, 2501)
        + 12504] + (1.0 - c2_weight) * c2_d_aerodata[sf_eml_array_bounds_check
        (sfGlobalDebugInstanceStruct, chartInstance->S, 0U, 0, 0, MAX_uint32_T,
         (int32_T)sf_integer_check(chartInstance->S, 0U, 0U, 0U, c2_k), 1, 2501)
        + 10003];
    } else {
      _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 53);
      c2_x_x = c2_aoa;
      c2_y_x = c2_x_x;
      c2_bb_x = c2_y_x;
      c2_j_y = muDoubleScalarAbs(c2_bb_x);
      c2_db_x = c2_aoa;
      c2_eb_x = c2_db_x;
      c2_fb_x = c2_eb_x;
      c2_k_y = muDoubleScalarAbs(c2_fb_x);
      if (CV_SCRIPT_IF(1, 4, CV_RELATIONAL_EVAL(14U, 1U, 4, c2_j_y, 4.0, -1, 3U,
            c2_k_y <= 4.0))) {
        _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 54);
        c2_d_A = c2_aoa - 2.0;
        c2_gb_x = c2_d_A;
        c2_hb_x = c2_gb_x;
        c2_weight = c2_hb_x / 2.0;
        _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 55);
        c2_CD = c2_weight * c2_d_aerodata[sf_eml_array_bounds_check
          (sfGlobalDebugInstanceStruct, chartInstance->S, 0U, 0, 0, MAX_uint32_T,
           (int32_T)sf_integer_check(chartInstance->S, 0U, 0U, 0U, c2_k), 1,
           2501) + 7502] + (1.0 - c2_weight) *
          c2_d_aerodata[sf_eml_array_bounds_check(sfGlobalDebugInstanceStruct,
          chartInstance->S, 0U, 0, 0, MAX_uint32_T, (int32_T)sf_integer_check
          (chartInstance->S, 0U, 0U, 0U, c2_k), 1, 2501) + 5001];
        _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 56);
        c2_CL = c2_weight * c2_d_aerodata[sf_eml_array_bounds_check
          (sfGlobalDebugInstanceStruct, chartInstance->S, 0U, 0, 0, MAX_uint32_T,
           (int32_T)sf_integer_check(chartInstance->S, 0U, 0U, 0U, c2_k), 1,
           2501) + 15005] + (1.0 - c2_weight) *
          c2_d_aerodata[sf_eml_array_bounds_check(sfGlobalDebugInstanceStruct,
          chartInstance->S, 0U, 0, 0, MAX_uint32_T, (int32_T)sf_integer_check
          (chartInstance->S, 0U, 0U, 0U, c2_k), 1, 2501) + 12504];
      } else {
        _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 58);
        c2_CD = 1.0E+8;
        _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 59);
        c2_CL = 1.0E+8;
        _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 60);
        c2_l_y = NULL;
        sf_mex_assign(&c2_l_y, sf_mex_create("y", c2_cv0, 10, 0U, 1U, 0U, 2, 1,
          11), false);
        sf_mex_call_debug(sfGlobalDebugInstanceStruct, "disp", 0U, 1U, 14,
                          c2_l_y);
      }
    }

    _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 63);
    c2_g_a = c2_rocket->OD;
    c2_h_a = c2_g_a;
    c2_ib_x = c2_h_a;
    c2_i_a = c2_ib_x;
    c2_m_y = c2_i_a * c2_i_a;
    c2_e_p = false;
    if (c2_e_p) {
      c2_error(chartInstance);
    }

    c2_jb_x = 3.1415926535897931 * c2_m_y;
    c2_kb_x = c2_jb_x;
    c2_S = c2_kb_x / 4.0;
    _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 66);
    c2_d10 = c2_aoa;
    c2_b_cosd(chartInstance, &c2_d10);
    c2_j_a = c2_d10 * c2_umag;
    c2_k_a = c2_j_a;
    c2_lb_x = c2_k_a;
    c2_l_a = c2_lb_x;
    c2_n_y = c2_l_a * c2_l_a;
    c2_f_p = false;
    if (c2_f_p) {
      c2_error(chartInstance);
    }

    *c2_Fdrag = c2_CD * c2_S * 0.5 * c2_rho * c2_n_y;
    _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 67);
    c2_d11 = c2_aoa;
    c2_b_sind(chartInstance, &c2_d11);
    c2_m_a = c2_d11 * c2_umag;
    c2_n_a = c2_m_a;
    c2_mb_x = c2_n_a;
    c2_o_a = c2_mb_x;
    c2_o_y = c2_o_a * c2_o_a;
    c2_g_p = false;
    if (c2_g_p) {
      c2_error(chartInstance);
    }

    *c2_Flift = c2_CL * c2_S * 0.5 * c2_rho * c2_o_y;
  } else {
    _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 69);
    if (CV_SCRIPT_COND(1, 0, CV_RELATIONAL_EVAL(14U, 1U, 5, c2_u[1],
          c2_rocket->drogue.deploy_u, -1, 2U, c2_u[1] <
          c2_rocket->drogue.deploy_u)) && CV_SCRIPT_COND(1, 1,
         CV_RELATIONAL_EVAL(14U, 1U, 6, c2_r[1], c2_rocket->main.deploy_h, -1,
                            4U, c2_r[1] > c2_rocket->main.deploy_h))) {
      CV_SCRIPT_MCDC(1, 0, true);
      CV_SCRIPT_IF(1, 5, true);
      _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 70);
      *c2_Fdrag = 20.0;
      _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 71);
      *c2_Flift = 0.0;
    } else {
      CV_SCRIPT_MCDC(1, 0, false);
      CV_SCRIPT_IF(1, 5, false);
      _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 73);
      *c2_Fdrag = 100.0;
      _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 74);
      *c2_Flift = 0.0;
    }
  }

  _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, -74);
  _SFD_SYMBOL_SCOPE_POP();
}

static void c2_getAtmoConditions(SFc2_solver_simulinkInstanceStruct
  *chartInstance, real_T c2_h, real_T *c2_b_T, real_T *c2_P, real_T *c2_rho,
  real_T *c2_a)
{
  uint32_T c2_debug_family_var_map[19];
  real_T c2_mu;
  real_T c2_Tlaunch;
  real_T c2_T0[8];
  real_T c2_Trate[7];
  real_T c2_hpos[7];
  real_T c2_g;
  real_T c2_avogadro;
  real_T c2_k_boltz;
  real_T c2_m_air;
  real_T c2_R_air;
  real_T c2_P_sl;
  real_T c2_k;
  real_T c2_nargin = 1.0;
  real_T c2_nargout = 5.0;
  int32_T c2_i22;
  static real_T c2_dv1[7] = { -0.0065, 0.0, 0.003, 0.0, -0.0045, 0.0, 0.004 };

  int32_T c2_i23;
  static real_T c2_dv2[7] = { 11000.0, 25000.0, 47000.0, 53000.0, 79000.0,
    90000.0, 105000.0 };

  real_T c2_x;
  real_T c2_b_y;
  real_T c2_b_x;
  real_T c2_c_y;
  real_T c2_z;
  real_T c2_c_x;
  real_T c2_d_x;
  real_T c2_d_y;
  real_T c2_e_y;
  real_T c2_b_z;
  real_T c2_b_a;
  real_T c2_c_a;
  real_T c2_e_x;
  real_T c2_d_a;
  real_T c2_f_y;
  real_T c2_e_a;
  boolean_T c2_p;
  real_T c2_f_x;
  real_T c2_g_x;
  boolean_T c2_b_p;
  boolean_T c2_c_p;
  real_T c2_h_x;
  real_T c2_i_x;
  _SFD_SYMBOL_SCOPE_PUSH_EML(0U, 19U, 19U, c2_b_debug_family_names,
    c2_debug_family_var_map);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_mu, 0U, c2_c_sf_marshallOut,
    c2_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_Tlaunch, 1U, c2_c_sf_marshallOut,
    c2_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c2_T0, 2U, c2_h_sf_marshallOut,
    c2_f_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c2_Trate, 3U, c2_g_sf_marshallOut,
    c2_e_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c2_hpos, 4U, c2_g_sf_marshallOut,
    c2_e_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_g, 5U, c2_c_sf_marshallOut,
    c2_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c2_avogadro, 6U, c2_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_k_boltz, 7U, c2_c_sf_marshallOut,
    c2_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_m_air, 8U, c2_c_sf_marshallOut,
    c2_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_R_air, 9U, c2_c_sf_marshallOut,
    c2_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_P_sl, 10U, c2_c_sf_marshallOut,
    c2_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_k, 11U, c2_c_sf_marshallOut,
    c2_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_nargin, 12U, c2_c_sf_marshallOut,
    c2_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_nargout, 13U, c2_c_sf_marshallOut,
    c2_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_h, 14U, c2_c_sf_marshallOut,
    c2_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c2_b_T, 15U, c2_c_sf_marshallOut,
    c2_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c2_P, 16U, c2_c_sf_marshallOut,
    c2_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c2_rho, 17U, c2_c_sf_marshallOut,
    c2_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c2_a, 18U, c2_c_sf_marshallOut,
    c2_b_sf_marshallIn);
  CV_SCRIPT_FCN(2, 0);
  _SFD_SCRIPT_CALL(2U, chartInstance->c2_sfEvent, 10);
  c2_Tlaunch = 288.16;
  _SFD_SCRIPT_CALL(2U, chartInstance->c2_sfEvent, 11);
  c2_T0[0] = c2_Tlaunch;
  c2_T0[1] = 216.66;
  c2_T0[2] = 216.66;
  c2_T0[3] = 282.66;
  c2_T0[4] = 282.66;
  c2_T0[5] = 165.66;
  c2_T0[6] = 165.66;
  c2_T0[7] = 225.66;
  _SFD_SCRIPT_CALL(2U, chartInstance->c2_sfEvent, 12);
  for (c2_i22 = 0; c2_i22 < 7; c2_i22++) {
    c2_Trate[c2_i22] = c2_dv1[c2_i22];
  }

  _SFD_SCRIPT_CALL(2U, chartInstance->c2_sfEvent, 13);
  for (c2_i23 = 0; c2_i23 < 7; c2_i23++) {
    c2_hpos[c2_i23] = c2_dv2[c2_i23];
  }

  _SFD_SCRIPT_CALL(2U, chartInstance->c2_sfEvent, 15);
  if (CV_SCRIPT_IF(2, 0, CV_RELATIONAL_EVAL(14U, 2U, 0, c2_h, c2_hpos[0], -1, 3U,
        c2_h <= c2_hpos[0]))) {
    _SFD_SCRIPT_CALL(2U, chartInstance->c2_sfEvent, 16);
    *c2_b_T = c2_T0[0] + c2_Trate[0] * (c2_hpos[0] - c2_h);
  } else {
    _SFD_SCRIPT_CALL(2U, chartInstance->c2_sfEvent, 17);
    if (CV_SCRIPT_IF(2, 1, CV_RELATIONAL_EVAL(14U, 2U, 1, c2_h, c2_hpos[1], -1,
          3U, c2_h <= c2_hpos[1]))) {
      _SFD_SCRIPT_CALL(2U, chartInstance->c2_sfEvent, 18);
      *c2_b_T = c2_T0[1] + c2_Trate[1] * (c2_hpos[1] - c2_h);
    } else {
      _SFD_SCRIPT_CALL(2U, chartInstance->c2_sfEvent, 19);
      if (CV_SCRIPT_IF(2, 2, CV_RELATIONAL_EVAL(14U, 2U, 2, c2_h, c2_hpos[2], -1,
            3U, c2_h <= c2_hpos[2]))) {
        _SFD_SCRIPT_CALL(2U, chartInstance->c2_sfEvent, 20);
        *c2_b_T = c2_T0[2] + c2_Trate[2] * (c2_hpos[2] - c2_h);
      } else {
        _SFD_SCRIPT_CALL(2U, chartInstance->c2_sfEvent, 21);
        if (CV_SCRIPT_IF(2, 3, CV_RELATIONAL_EVAL(14U, 2U, 3, c2_h, c2_hpos[3],
              -1, 3U, c2_h <= c2_hpos[3]))) {
          _SFD_SCRIPT_CALL(2U, chartInstance->c2_sfEvent, 22);
          *c2_b_T = c2_T0[3] + c2_Trate[3] * (c2_hpos[3] - c2_h);
        } else {
          _SFD_SCRIPT_CALL(2U, chartInstance->c2_sfEvent, 23);
          if (CV_SCRIPT_IF(2, 4, CV_RELATIONAL_EVAL(14U, 2U, 4, c2_h, c2_hpos[4],
                -1, 3U, c2_h <= c2_hpos[4]))) {
            _SFD_SCRIPT_CALL(2U, chartInstance->c2_sfEvent, 24);
            *c2_b_T = c2_T0[4] + c2_Trate[4] * (c2_hpos[4] - c2_h);
          } else {
            _SFD_SCRIPT_CALL(2U, chartInstance->c2_sfEvent, 25);
            if (CV_SCRIPT_IF(2, 5, CV_RELATIONAL_EVAL(14U, 2U, 5, c2_h, c2_hpos
                  [5], -1, 3U, c2_h <= c2_hpos[5]))) {
              _SFD_SCRIPT_CALL(2U, chartInstance->c2_sfEvent, 26);
              *c2_b_T = c2_T0[5] + c2_Trate[5] * (c2_hpos[5] - c2_h);
            } else {
              _SFD_SCRIPT_CALL(2U, chartInstance->c2_sfEvent, 27);
              if (CV_SCRIPT_IF(2, 6, CV_RELATIONAL_EVAL(14U, 2U, 6, c2_h,
                    c2_hpos[6], -1, 3U, c2_h <= c2_hpos[6]))) {
                _SFD_SCRIPT_CALL(2U, chartInstance->c2_sfEvent, 28);
                *c2_b_T = c2_T0[6] + c2_Trate[6] * (c2_hpos[6] - c2_h);
              } else {
                _SFD_SCRIPT_CALL(2U, chartInstance->c2_sfEvent, 30);
                *c2_b_T = 5.0E+10;
              }
            }
          }
        }
      }
    }
  }

  _SFD_SCRIPT_CALL(2U, chartInstance->c2_sfEvent, 34);
  c2_mu = 1.846E-5;
  _SFD_SCRIPT_CALL(2U, chartInstance->c2_sfEvent, 37);
  c2_g = 9.81;
  _SFD_SCRIPT_CALL(2U, chartInstance->c2_sfEvent, 38);
  c2_avogadro = 6.0221409E+23;
  _SFD_SCRIPT_CALL(2U, chartInstance->c2_sfEvent, 39);
  c2_k_boltz = 1.38064852E-23;
  _SFD_SCRIPT_CALL(2U, chartInstance->c2_sfEvent, 40);
  c2_m_air = 4.8155631828541245E-26;
  _SFD_SCRIPT_CALL(2U, chartInstance->c2_sfEvent, 41);
  c2_R_air = 287.0;
  _SFD_SCRIPT_CALL(2U, chartInstance->c2_sfEvent, 42);
  c2_P_sl = 101300.0;
  _SFD_SCRIPT_CALL(2U, chartInstance->c2_sfEvent, 43);
  c2_k = 1.4;
  _SFD_SCRIPT_CALL(2U, chartInstance->c2_sfEvent, 44);
  c2_x = -c2_m_air * c2_g * c2_h;
  c2_b_y = c2_k_boltz * *c2_b_T;
  c2_b_x = c2_x;
  c2_c_y = c2_b_y;
  c2_z = c2_b_x / c2_c_y;
  c2_c_x = c2_z;
  c2_d_x = c2_c_x;
  c2_d_x = muDoubleScalarExp(c2_d_x);
  *c2_P = c2_P_sl * c2_d_x;
  _SFD_SCRIPT_CALL(2U, chartInstance->c2_sfEvent, 47);
  c2_d_y = 288.15 - 0.0065 * c2_h;
  c2_e_y = c2_d_y;
  c2_b_z = 288.15 / c2_e_y;
  c2_b_a = c2_b_z;
  c2_c_a = c2_b_a;
  c2_e_x = c2_c_a;
  c2_d_a = c2_e_x;
  c2_f_y = muDoubleScalarPower(c2_d_a, 6.2558761132785179);
  c2_e_a = c2_c_a;
  c2_p = false;
  if (c2_e_a < 0.0) {
    c2_p = true;
  }

  if (c2_p) {
    c2_error(chartInstance);
  }

  *c2_rho = 1.225 * c2_f_y;
  _SFD_SCRIPT_CALL(2U, chartInstance->c2_sfEvent, 51);
  c2_f_x = c2_k * c2_R_air * *c2_b_T;
  *c2_a = c2_f_x;
  c2_g_x = *c2_a;
  c2_b_p = (c2_g_x < 0.0);
  c2_c_p = c2_b_p;
  if (c2_c_p) {
    c2_c_error(chartInstance);
  }

  c2_h_x = *c2_a;
  *c2_a = c2_h_x;
  c2_i_x = *c2_a;
  *c2_a = c2_i_x;
  *c2_a = muDoubleScalarSqrt(*c2_a);
  _SFD_SCRIPT_CALL(2U, chartInstance->c2_sfEvent, -51);
  _SFD_SYMBOL_SCOPE_POP();
}

static void init_script_number_translation(uint32_T c2_machineNumber, uint32_T
  c2_chartNumber, uint32_T c2_instanceNumber)
{
  (void)c2_machineNumber;
  _SFD_SCRIPT_TRANSLATION(c2_chartNumber, c2_instanceNumber, 0U,
    sf_debug_get_script_id(
    "C:\\Users\\ME123\\Github\\rockets-irec\\Trajectory_Sims\\Matlab Solver\\forces.m"));
  _SFD_SCRIPT_TRANSLATION(c2_chartNumber, c2_instanceNumber, 1U,
    sf_debug_get_script_id(
    "C:\\Users\\ME123\\Github\\rockets-irec\\Trajectory_Sims\\Matlab Solver\\aerodynamics.m"));
  _SFD_SCRIPT_TRANSLATION(c2_chartNumber, c2_instanceNumber, 2U,
    sf_debug_get_script_id(
    "C:\\Users\\ME123\\Github\\rockets-irec\\Trajectory_Sims\\Matlab Solver\\getAtmoConditions.m"));
}

static void c2_emlrt_marshallIn(SFc2_solver_simulinkInstanceStruct
  *chartInstance, const mxArray *c2_load, const char_T *c2_identifier,
  c2_szHckfuHwXXVCwxC2kXrI2C *c2_b_y)
{
  emlrtMsgIdentifier c2_thisId;
  c2_thisId.fIdentifier = c2_identifier;
  c2_thisId.fParent = NULL;
  c2_thisId.bParentIsCell = false;
  c2_b_emlrt_marshallIn(chartInstance, sf_mex_dup(c2_load), &c2_thisId, c2_b_y);
  sf_mex_destroy(&c2_load);
}

static void c2_b_emlrt_marshallIn(SFc2_solver_simulinkInstanceStruct
  *chartInstance, const mxArray *c2_u, const emlrtMsgIdentifier *c2_parentId,
  c2_szHckfuHwXXVCwxC2kXrI2C *c2_b_y)
{
  emlrtMsgIdentifier c2_thisId;
  static const char * c2_fieldNames[3] = { "CM", "CP", "rocket" };

  c2_thisId.fParent = c2_parentId;
  c2_thisId.bParentIsCell = false;
  sf_mex_check_struct(c2_parentId, c2_u, 3, c2_fieldNames, 0U, NULL);
  c2_thisId.fIdentifier = "CM";
  c2_b_y->CM = c2_c_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getfield
    (c2_u, "CM", "CM", 0)), &c2_thisId);
  c2_thisId.fIdentifier = "CP";
  c2_b_y->CP = c2_c_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getfield
    (c2_u, "CP", "CP", 0)), &c2_thisId);
  c2_thisId.fIdentifier = "rocket";
  c2_d_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getfield(c2_u, "rocket",
    "rocket", 0)), &c2_thisId, &c2_b_y->rocket);
  sf_mex_destroy(&c2_u);
}

static real_T c2_c_emlrt_marshallIn(SFc2_solver_simulinkInstanceStruct
  *chartInstance, const mxArray *c2_u, const emlrtMsgIdentifier *c2_parentId)
{
  real_T c2_b_y;
  real_T c2_d12;
  (void)chartInstance;
  sf_mex_import(c2_parentId, sf_mex_dup(c2_u), &c2_d12, 1, 0, 0U, 0, 0U, 0);
  c2_b_y = c2_d12;
  sf_mex_destroy(&c2_u);
  return c2_b_y;
}

static void c2_d_emlrt_marshallIn(SFc2_solver_simulinkInstanceStruct
  *chartInstance, const mxArray *c2_u, const emlrtMsgIdentifier *c2_parentId,
  c2_sv2voxIt8FKzjMlUEnyXmnC *c2_b_y)
{
  emlrtMsgIdentifier c2_thisId;
  static const char * c2_fieldNames[12] = { "OD", "drymass", "wetmass", "nose",
    "shoulder", "fore", "aft", "fin", "l", "drogue", "main", "payload" };

  c2_thisId.fParent = c2_parentId;
  c2_thisId.bParentIsCell = false;
  sf_mex_check_struct(c2_parentId, c2_u, 12, c2_fieldNames, 0U, NULL);
  c2_thisId.fIdentifier = "OD";
  c2_b_y->OD = c2_c_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getfield
    (c2_u, "OD", "OD", 0)), &c2_thisId);
  c2_thisId.fIdentifier = "drymass";
  c2_b_y->drymass = c2_c_emlrt_marshallIn(chartInstance, sf_mex_dup
    (sf_mex_getfield(c2_u, "drymass", "drymass", 0)), &c2_thisId);
  c2_thisId.fIdentifier = "wetmass";
  c2_b_y->wetmass = c2_c_emlrt_marshallIn(chartInstance, sf_mex_dup
    (sf_mex_getfield(c2_u, "wetmass", "wetmass", 0)), &c2_thisId);
  c2_thisId.fIdentifier = "nose";
  c2_b_y->nose = c2_e_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getfield
    (c2_u, "nose", "nose", 0)), &c2_thisId);
  c2_thisId.fIdentifier = "shoulder";
  c2_b_y->shoulder = c2_g_emlrt_marshallIn(chartInstance, sf_mex_dup
    (sf_mex_getfield(c2_u, "shoulder", "shoulder", 0)), &c2_thisId);
  c2_thisId.fIdentifier = "fore";
  c2_b_y->fore = c2_h_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getfield
    (c2_u, "fore", "fore", 0)), &c2_thisId);
  c2_thisId.fIdentifier = "aft";
  c2_b_y->aft = c2_i_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getfield
    (c2_u, "aft", "aft", 0)), &c2_thisId);
  c2_thisId.fIdentifier = "fin";
  c2_b_y->fin = c2_j_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getfield
    (c2_u, "fin", "fin", 0)), &c2_thisId);
  c2_thisId.fIdentifier = "l";
  c2_b_y->l = c2_c_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getfield
    (c2_u, "l", "l", 0)), &c2_thisId);
  c2_thisId.fIdentifier = "drogue";
  c2_b_y->drogue = c2_k_emlrt_marshallIn(chartInstance, sf_mex_dup
    (sf_mex_getfield(c2_u, "drogue", "drogue", 0)), &c2_thisId);
  c2_thisId.fIdentifier = "main";
  c2_b_y->main = c2_l_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getfield
    (c2_u, "main", "main", 0)), &c2_thisId);
  c2_thisId.fIdentifier = "payload";
  c2_b_y->payload = c2_m_emlrt_marshallIn(chartInstance, sf_mex_dup
    (sf_mex_getfield(c2_u, "payload", "payload", 0)), &c2_thisId);
  sf_mex_destroy(&c2_u);
}

static c2_sof5eNcdcizKAe6jjm9xhtG c2_e_emlrt_marshallIn
  (SFc2_solver_simulinkInstanceStruct *chartInstance, const mxArray *c2_u, const
   emlrtMsgIdentifier *c2_parentId)
{
  c2_sof5eNcdcizKAe6jjm9xhtG c2_b_y;
  emlrtMsgIdentifier c2_thisId;
  static const char * c2_fieldNames[4] = { "wetmass", "drymass", "type", "l" };

  c2_thisId.fParent = c2_parentId;
  c2_thisId.bParentIsCell = false;
  sf_mex_check_struct(c2_parentId, c2_u, 4, c2_fieldNames, 0U, NULL);
  c2_thisId.fIdentifier = "wetmass";
  c2_b_y.wetmass = c2_c_emlrt_marshallIn(chartInstance, sf_mex_dup
    (sf_mex_getfield(c2_u, "wetmass", "wetmass", 0)), &c2_thisId);
  c2_thisId.fIdentifier = "drymass";
  c2_b_y.drymass = c2_c_emlrt_marshallIn(chartInstance, sf_mex_dup
    (sf_mex_getfield(c2_u, "drymass", "drymass", 0)), &c2_thisId);
  c2_thisId.fIdentifier = "type";
  c2_f_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getfield(c2_u, "type",
    "type", 0)), &c2_thisId, c2_b_y.type);
  c2_thisId.fIdentifier = "l";
  c2_b_y.l = c2_c_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getfield
    (c2_u, "l", "l", 0)), &c2_thisId);
  sf_mex_destroy(&c2_u);
  return c2_b_y;
}

static void c2_f_emlrt_marshallIn(SFc2_solver_simulinkInstanceStruct
  *chartInstance, const mxArray *c2_u, const emlrtMsgIdentifier *c2_parentId,
  char_T c2_b_y[9])
{
  char_T c2_cv1[9];
  int32_T c2_i24;
  (void)chartInstance;
  sf_mex_import(c2_parentId, sf_mex_dup(c2_u), c2_cv1, 1, 10, 0U, 1, 0U, 2, 1, 9);
  for (c2_i24 = 0; c2_i24 < 9; c2_i24++) {
    c2_b_y[c2_i24] = c2_cv1[c2_i24];
  }

  sf_mex_destroy(&c2_u);
}

static c2_syx7FYmGk3I6e3U1RUwUAWD c2_g_emlrt_marshallIn
  (SFc2_solver_simulinkInstanceStruct *chartInstance, const mxArray *c2_u, const
   emlrtMsgIdentifier *c2_parentId)
{
  c2_syx7FYmGk3I6e3U1RUwUAWD c2_b_y;
  emlrtMsgIdentifier c2_thisId;
  static const char * c2_fieldNames[1] = { "l" };

  c2_thisId.fParent = c2_parentId;
  c2_thisId.bParentIsCell = false;
  sf_mex_check_struct(c2_parentId, c2_u, 1, c2_fieldNames, 0U, NULL);
  c2_thisId.fIdentifier = "l";
  c2_b_y.l = c2_c_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getfield
    (c2_u, "l", "l", 0)), &c2_thisId);
  sf_mex_destroy(&c2_u);
  return c2_b_y;
}

static c2_sqGsSc4djWypCV90lhUGiL c2_h_emlrt_marshallIn
  (SFc2_solver_simulinkInstanceStruct *chartInstance, const mxArray *c2_u, const
   emlrtMsgIdentifier *c2_parentId)
{
  c2_sqGsSc4djWypCV90lhUGiL c2_b_y;
  emlrtMsgIdentifier c2_thisId;
  static const char * c2_fieldNames[2] = { "l", "S" };

  c2_thisId.fParent = c2_parentId;
  c2_thisId.bParentIsCell = false;
  sf_mex_check_struct(c2_parentId, c2_u, 2, c2_fieldNames, 0U, NULL);
  c2_thisId.fIdentifier = "l";
  c2_b_y.l = c2_c_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getfield
    (c2_u, "l", "l", 0)), &c2_thisId);
  c2_thisId.fIdentifier = "S";
  c2_b_y.S = c2_c_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getfield
    (c2_u, "S", "S", 0)), &c2_thisId);
  sf_mex_destroy(&c2_u);
  return c2_b_y;
}

static c2_su2bTaGhbTlLQVXsqMBjEuH c2_i_emlrt_marshallIn
  (SFc2_solver_simulinkInstanceStruct *chartInstance, const mxArray *c2_u, const
   emlrtMsgIdentifier *c2_parentId)
{
  c2_su2bTaGhbTlLQVXsqMBjEuH c2_b_y;
  emlrtMsgIdentifier c2_thisId;
  static const char * c2_fieldNames[3] = { "l", "wetmass", "drymass" };

  c2_thisId.fParent = c2_parentId;
  c2_thisId.bParentIsCell = false;
  sf_mex_check_struct(c2_parentId, c2_u, 3, c2_fieldNames, 0U, NULL);
  c2_thisId.fIdentifier = "l";
  c2_b_y.l = c2_c_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getfield
    (c2_u, "l", "l", 0)), &c2_thisId);
  c2_thisId.fIdentifier = "wetmass";
  c2_b_y.wetmass = c2_c_emlrt_marshallIn(chartInstance, sf_mex_dup
    (sf_mex_getfield(c2_u, "wetmass", "wetmass", 0)), &c2_thisId);
  c2_thisId.fIdentifier = "drymass";
  c2_b_y.drymass = c2_c_emlrt_marshallIn(chartInstance, sf_mex_dup
    (sf_mex_getfield(c2_u, "drymass", "drymass", 0)), &c2_thisId);
  sf_mex_destroy(&c2_u);
  return c2_b_y;
}

static c2_sNlKjRKFHwjFKC34GeUJivG c2_j_emlrt_marshallIn
  (SFc2_solver_simulinkInstanceStruct *chartInstance, const mxArray *c2_u, const
   emlrtMsgIdentifier *c2_parentId)
{
  c2_sNlKjRKFHwjFKC34GeUJivG c2_b_y;
  emlrtMsgIdentifier c2_thisId;
  static const char * c2_fieldNames[8] = { "t", "span", "rootchord", "tipchord",
    "sweep", "S", "AR", "num" };

  c2_thisId.fParent = c2_parentId;
  c2_thisId.bParentIsCell = false;
  sf_mex_check_struct(c2_parentId, c2_u, 8, c2_fieldNames, 0U, NULL);
  c2_thisId.fIdentifier = "t";
  c2_b_y.t = c2_c_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getfield
    (c2_u, "t", "t", 0)), &c2_thisId);
  c2_thisId.fIdentifier = "span";
  c2_b_y.span = c2_c_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getfield
    (c2_u, "span", "span", 0)), &c2_thisId);
  c2_thisId.fIdentifier = "rootchord";
  c2_b_y.rootchord = c2_c_emlrt_marshallIn(chartInstance, sf_mex_dup
    (sf_mex_getfield(c2_u, "rootchord", "rootchord", 0)), &c2_thisId);
  c2_thisId.fIdentifier = "tipchord";
  c2_b_y.tipchord = c2_c_emlrt_marshallIn(chartInstance, sf_mex_dup
    (sf_mex_getfield(c2_u, "tipchord", "tipchord", 0)), &c2_thisId);
  c2_thisId.fIdentifier = "sweep";
  c2_b_y.sweep = c2_c_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getfield
    (c2_u, "sweep", "sweep", 0)), &c2_thisId);
  c2_thisId.fIdentifier = "S";
  c2_b_y.S = c2_c_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getfield
    (c2_u, "S", "S", 0)), &c2_thisId);
  c2_thisId.fIdentifier = "AR";
  c2_b_y.AR = c2_c_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getfield
    (c2_u, "AR", "AR", 0)), &c2_thisId);
  c2_thisId.fIdentifier = "num";
  c2_b_y.num = c2_c_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getfield
    (c2_u, "num", "num", 0)), &c2_thisId);
  sf_mex_destroy(&c2_u);
  return c2_b_y;
}

static c2_sKS3cuSxR3HIcTIZZkbRPm c2_k_emlrt_marshallIn
  (SFc2_solver_simulinkInstanceStruct *chartInstance, const mxArray *c2_u, const
   emlrtMsgIdentifier *c2_parentId)
{
  c2_sKS3cuSxR3HIcTIZZkbRPm c2_b_y;
  emlrtMsgIdentifier c2_thisId;
  static const char * c2_fieldNames[4] = { "d", "S", "Cd", "deploy_u" };

  c2_thisId.fParent = c2_parentId;
  c2_thisId.bParentIsCell = false;
  sf_mex_check_struct(c2_parentId, c2_u, 4, c2_fieldNames, 0U, NULL);
  c2_thisId.fIdentifier = "d";
  c2_b_y.d = c2_c_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getfield
    (c2_u, "d", "d", 0)), &c2_thisId);
  c2_thisId.fIdentifier = "S";
  c2_b_y.S = c2_c_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getfield
    (c2_u, "S", "S", 0)), &c2_thisId);
  c2_thisId.fIdentifier = "Cd";
  c2_b_y.Cd = c2_c_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getfield
    (c2_u, "Cd", "Cd", 0)), &c2_thisId);
  c2_thisId.fIdentifier = "deploy_u";
  c2_b_y.deploy_u = c2_c_emlrt_marshallIn(chartInstance, sf_mex_dup
    (sf_mex_getfield(c2_u, "deploy_u", "deploy_u", 0)), &c2_thisId);
  sf_mex_destroy(&c2_u);
  return c2_b_y;
}

static c2_s88seaxzVTYYZIzTGqmP4AD c2_l_emlrt_marshallIn
  (SFc2_solver_simulinkInstanceStruct *chartInstance, const mxArray *c2_u, const
   emlrtMsgIdentifier *c2_parentId)
{
  c2_s88seaxzVTYYZIzTGqmP4AD c2_b_y;
  emlrtMsgIdentifier c2_thisId;
  static const char * c2_fieldNames[4] = { "d", "S", "Cd", "deploy_h" };

  c2_thisId.fParent = c2_parentId;
  c2_thisId.bParentIsCell = false;
  sf_mex_check_struct(c2_parentId, c2_u, 4, c2_fieldNames, 0U, NULL);
  c2_thisId.fIdentifier = "d";
  c2_b_y.d = c2_c_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getfield
    (c2_u, "d", "d", 0)), &c2_thisId);
  c2_thisId.fIdentifier = "S";
  c2_b_y.S = c2_c_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getfield
    (c2_u, "S", "S", 0)), &c2_thisId);
  c2_thisId.fIdentifier = "Cd";
  c2_b_y.Cd = c2_c_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getfield
    (c2_u, "Cd", "Cd", 0)), &c2_thisId);
  c2_thisId.fIdentifier = "deploy_h";
  c2_b_y.deploy_h = c2_c_emlrt_marshallIn(chartInstance, sf_mex_dup
    (sf_mex_getfield(c2_u, "deploy_h", "deploy_h", 0)), &c2_thisId);
  sf_mex_destroy(&c2_u);
  return c2_b_y;
}

static c2_sfpFxKVMN8elArLoV353vAE c2_m_emlrt_marshallIn
  (SFc2_solver_simulinkInstanceStruct *chartInstance, const mxArray *c2_u, const
   emlrtMsgIdentifier *c2_parentId)
{
  c2_sfpFxKVMN8elArLoV353vAE c2_b_y;
  emlrtMsgIdentifier c2_thisId;
  static const char * c2_fieldNames[2] = { "chute_d", "chute_Cd" };

  c2_thisId.fParent = c2_parentId;
  c2_thisId.bParentIsCell = false;
  sf_mex_check_struct(c2_parentId, c2_u, 2, c2_fieldNames, 0U, NULL);
  c2_thisId.fIdentifier = "chute_d";
  c2_b_y.chute_d = c2_c_emlrt_marshallIn(chartInstance, sf_mex_dup
    (sf_mex_getfield(c2_u, "chute_d", "chute_d", 0)), &c2_thisId);
  c2_thisId.fIdentifier = "chute_Cd";
  c2_b_y.chute_Cd = c2_c_emlrt_marshallIn(chartInstance, sf_mex_dup
    (sf_mex_getfield(c2_u, "chute_Cd", "chute_Cd", 0)), &c2_thisId);
  sf_mex_destroy(&c2_u);
  return c2_b_y;
}

static const mxArray *c2_sf_marshallOut(void *chartInstanceVoid, void *c2_inData)
{
  const mxArray *c2_mxArrayOutData;
  int32_T c2_i25;
  const mxArray *c2_b_y = NULL;
  real_T c2_u[6];
  SFc2_solver_simulinkInstanceStruct *chartInstance;
  chartInstance = (SFc2_solver_simulinkInstanceStruct *)chartInstanceVoid;
  c2_mxArrayOutData = NULL;
  c2_mxArrayOutData = NULL;
  for (c2_i25 = 0; c2_i25 < 6; c2_i25++) {
    c2_u[c2_i25] = (*(real_T (*)[6])c2_inData)[c2_i25];
  }

  c2_b_y = NULL;
  sf_mex_assign(&c2_b_y, sf_mex_create("y", c2_u, 0, 0U, 1U, 0U, 1, 6), false);
  sf_mex_assign(&c2_mxArrayOutData, c2_b_y, false);
  return c2_mxArrayOutData;
}

static void c2_n_emlrt_marshallIn(SFc2_solver_simulinkInstanceStruct
  *chartInstance, const mxArray *c2_b_statedot, const char_T *c2_identifier,
  real_T c2_b_y[6])
{
  emlrtMsgIdentifier c2_thisId;
  c2_thisId.fIdentifier = c2_identifier;
  c2_thisId.fParent = NULL;
  c2_thisId.bParentIsCell = false;
  c2_o_emlrt_marshallIn(chartInstance, sf_mex_dup(c2_b_statedot), &c2_thisId,
                        c2_b_y);
  sf_mex_destroy(&c2_b_statedot);
}

static void c2_o_emlrt_marshallIn(SFc2_solver_simulinkInstanceStruct
  *chartInstance, const mxArray *c2_u, const emlrtMsgIdentifier *c2_parentId,
  real_T c2_b_y[6])
{
  real_T c2_dv3[6];
  int32_T c2_i26;
  (void)chartInstance;
  sf_mex_import(c2_parentId, sf_mex_dup(c2_u), c2_dv3, 1, 0, 0U, 1, 0U, 1, 6);
  for (c2_i26 = 0; c2_i26 < 6; c2_i26++) {
    c2_b_y[c2_i26] = c2_dv3[c2_i26];
  }

  sf_mex_destroy(&c2_u);
}

static void c2_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c2_mxArrayInData, const char_T *c2_varName, void *c2_outData)
{
  const mxArray *c2_b_statedot;
  const char_T *c2_identifier;
  emlrtMsgIdentifier c2_thisId;
  real_T c2_b_y[6];
  int32_T c2_i27;
  SFc2_solver_simulinkInstanceStruct *chartInstance;
  chartInstance = (SFc2_solver_simulinkInstanceStruct *)chartInstanceVoid;
  c2_b_statedot = sf_mex_dup(c2_mxArrayInData);
  c2_identifier = c2_varName;
  c2_thisId.fIdentifier = c2_identifier;
  c2_thisId.fParent = NULL;
  c2_thisId.bParentIsCell = false;
  c2_o_emlrt_marshallIn(chartInstance, sf_mex_dup(c2_b_statedot), &c2_thisId,
                        c2_b_y);
  sf_mex_destroy(&c2_b_statedot);
  for (c2_i27 = 0; c2_i27 < 6; c2_i27++) {
    (*(real_T (*)[6])c2_outData)[c2_i27] = c2_b_y[c2_i27];
  }

  sf_mex_destroy(&c2_mxArrayInData);
}

static const mxArray *c2_b_sf_marshallOut(void *chartInstanceVoid, void
  *c2_inData)
{
  const mxArray *c2_mxArrayOutData;
  int32_T c2_i28;
  int32_T c2_i29;
  const mxArray *c2_b_y = NULL;
  int32_T c2_i30;
  real_T c2_u[17507];
  SFc2_solver_simulinkInstanceStruct *chartInstance;
  chartInstance = (SFc2_solver_simulinkInstanceStruct *)chartInstanceVoid;
  c2_mxArrayOutData = NULL;
  c2_mxArrayOutData = NULL;
  c2_i28 = 0;
  for (c2_i29 = 0; c2_i29 < 7; c2_i29++) {
    for (c2_i30 = 0; c2_i30 < 2501; c2_i30++) {
      c2_u[c2_i30 + c2_i28] = (*(real_T (*)[17507])c2_inData)[c2_i30 + c2_i28];
    }

    c2_i28 += 2501;
  }

  c2_b_y = NULL;
  sf_mex_assign(&c2_b_y, sf_mex_create("y", c2_u, 0, 0U, 1U, 0U, 2, 2501, 7),
                false);
  sf_mex_assign(&c2_mxArrayOutData, c2_b_y, false);
  return c2_mxArrayOutData;
}

static const mxArray *c2_c_sf_marshallOut(void *chartInstanceVoid, void
  *c2_inData)
{
  const mxArray *c2_mxArrayOutData;
  real_T c2_u;
  const mxArray *c2_b_y = NULL;
  SFc2_solver_simulinkInstanceStruct *chartInstance;
  chartInstance = (SFc2_solver_simulinkInstanceStruct *)chartInstanceVoid;
  c2_mxArrayOutData = NULL;
  c2_mxArrayOutData = NULL;
  c2_u = *(real_T *)c2_inData;
  c2_b_y = NULL;
  sf_mex_assign(&c2_b_y, sf_mex_create("y", &c2_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_assign(&c2_mxArrayOutData, c2_b_y, false);
  return c2_mxArrayOutData;
}

static const mxArray *c2_d_sf_marshallOut(void *chartInstanceVoid, void
  *c2_inData)
{
  const mxArray *c2_mxArrayOutData;
  int32_T c2_i31;
  int32_T c2_i32;
  const mxArray *c2_b_y = NULL;
  int32_T c2_i33;
  real_T c2_u[1256];
  SFc2_solver_simulinkInstanceStruct *chartInstance;
  chartInstance = (SFc2_solver_simulinkInstanceStruct *)chartInstanceVoid;
  c2_mxArrayOutData = NULL;
  c2_mxArrayOutData = NULL;
  c2_i31 = 0;
  for (c2_i32 = 0; c2_i32 < 2; c2_i32++) {
    for (c2_i33 = 0; c2_i33 < 628; c2_i33++) {
      c2_u[c2_i33 + c2_i31] = (*(real_T (*)[1256])c2_inData)[c2_i33 + c2_i31];
    }

    c2_i31 += 628;
  }

  c2_b_y = NULL;
  sf_mex_assign(&c2_b_y, sf_mex_create("y", c2_u, 0, 0U, 1U, 0U, 2, 628, 2),
                false);
  sf_mex_assign(&c2_mxArrayOutData, c2_b_y, false);
  return c2_mxArrayOutData;
}

static void c2_b_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c2_mxArrayInData, const char_T *c2_varName, void *c2_outData)
{
  const mxArray *c2_nargout;
  const char_T *c2_identifier;
  emlrtMsgIdentifier c2_thisId;
  real_T c2_b_y;
  SFc2_solver_simulinkInstanceStruct *chartInstance;
  chartInstance = (SFc2_solver_simulinkInstanceStruct *)chartInstanceVoid;
  c2_nargout = sf_mex_dup(c2_mxArrayInData);
  c2_identifier = c2_varName;
  c2_thisId.fIdentifier = c2_identifier;
  c2_thisId.fParent = NULL;
  c2_thisId.bParentIsCell = false;
  c2_b_y = c2_c_emlrt_marshallIn(chartInstance, sf_mex_dup(c2_nargout),
    &c2_thisId);
  sf_mex_destroy(&c2_nargout);
  *(real_T *)c2_outData = c2_b_y;
  sf_mex_destroy(&c2_mxArrayInData);
}

static const mxArray *c2_e_sf_marshallOut(void *chartInstanceVoid, void
  *c2_inData)
{
  const mxArray *c2_mxArrayOutData;
  c2_sv2voxIt8FKzjMlUEnyXmnC c2_u;
  const mxArray *c2_b_y = NULL;
  real_T c2_b_u;
  const mxArray *c2_c_y = NULL;
  real_T c2_c_u;
  const mxArray *c2_d_y = NULL;
  real_T c2_d_u;
  const mxArray *c2_e_y = NULL;
  c2_sof5eNcdcizKAe6jjm9xhtG c2_e_u;
  const mxArray *c2_f_y = NULL;
  real_T c2_f_u;
  const mxArray *c2_g_y = NULL;
  real_T c2_g_u;
  const mxArray *c2_h_y = NULL;
  int32_T c2_i34;
  const mxArray *c2_i_y = NULL;
  char_T c2_h_u[9];
  real_T c2_i_u;
  const mxArray *c2_j_y = NULL;
  c2_syx7FYmGk3I6e3U1RUwUAWD c2_j_u;
  const mxArray *c2_k_y = NULL;
  real_T c2_k_u;
  const mxArray *c2_l_y = NULL;
  c2_sqGsSc4djWypCV90lhUGiL c2_l_u;
  const mxArray *c2_m_y = NULL;
  real_T c2_m_u;
  const mxArray *c2_n_y = NULL;
  real_T c2_n_u;
  const mxArray *c2_o_y = NULL;
  c2_su2bTaGhbTlLQVXsqMBjEuH c2_o_u;
  const mxArray *c2_p_y = NULL;
  real_T c2_p_u;
  const mxArray *c2_q_y = NULL;
  real_T c2_q_u;
  const mxArray *c2_r_y = NULL;
  real_T c2_r_u;
  const mxArray *c2_s_y = NULL;
  c2_sNlKjRKFHwjFKC34GeUJivG c2_s_u;
  const mxArray *c2_t_y = NULL;
  real_T c2_t_u;
  const mxArray *c2_u_y = NULL;
  real_T c2_u_u;
  const mxArray *c2_v_y = NULL;
  real_T c2_v_u;
  const mxArray *c2_w_y = NULL;
  real_T c2_w_u;
  const mxArray *c2_x_y = NULL;
  real_T c2_x_u;
  const mxArray *c2_y_y = NULL;
  real_T c2_y_u;
  const mxArray *c2_ab_y = NULL;
  real_T c2_ab_u;
  const mxArray *c2_bb_y = NULL;
  real_T c2_bb_u;
  const mxArray *c2_cb_y = NULL;
  real_T c2_cb_u;
  const mxArray *c2_db_y = NULL;
  c2_sKS3cuSxR3HIcTIZZkbRPm c2_db_u;
  const mxArray *c2_eb_y = NULL;
  real_T c2_eb_u;
  const mxArray *c2_fb_y = NULL;
  real_T c2_fb_u;
  const mxArray *c2_gb_y = NULL;
  real_T c2_gb_u;
  const mxArray *c2_hb_y = NULL;
  real_T c2_hb_u;
  const mxArray *c2_ib_y = NULL;
  c2_s88seaxzVTYYZIzTGqmP4AD c2_ib_u;
  const mxArray *c2_jb_y = NULL;
  real_T c2_jb_u;
  const mxArray *c2_kb_y = NULL;
  real_T c2_kb_u;
  const mxArray *c2_lb_y = NULL;
  real_T c2_lb_u;
  const mxArray *c2_mb_y = NULL;
  real_T c2_mb_u;
  const mxArray *c2_nb_y = NULL;
  c2_sfpFxKVMN8elArLoV353vAE c2_nb_u;
  const mxArray *c2_ob_y = NULL;
  real_T c2_ob_u;
  const mxArray *c2_pb_y = NULL;
  real_T c2_pb_u;
  const mxArray *c2_qb_y = NULL;
  SFc2_solver_simulinkInstanceStruct *chartInstance;
  chartInstance = (SFc2_solver_simulinkInstanceStruct *)chartInstanceVoid;
  c2_mxArrayOutData = NULL;
  c2_mxArrayOutData = NULL;
  c2_u = *(c2_sv2voxIt8FKzjMlUEnyXmnC *)c2_inData;
  c2_b_y = NULL;
  sf_mex_assign(&c2_b_y, sf_mex_createstruct("structure", 2, 1, 1), false);
  c2_b_u = c2_u.OD;
  c2_c_y = NULL;
  sf_mex_assign(&c2_c_y, sf_mex_create("y", &c2_b_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_b_y, c2_c_y, "OD", "OD", 0);
  c2_c_u = c2_u.drymass;
  c2_d_y = NULL;
  sf_mex_assign(&c2_d_y, sf_mex_create("y", &c2_c_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_b_y, c2_d_y, "drymass", "drymass", 0);
  c2_d_u = c2_u.wetmass;
  c2_e_y = NULL;
  sf_mex_assign(&c2_e_y, sf_mex_create("y", &c2_d_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_b_y, c2_e_y, "wetmass", "wetmass", 0);
  c2_e_u = c2_u.nose;
  c2_f_y = NULL;
  sf_mex_assign(&c2_f_y, sf_mex_createstruct("structure", 2, 1, 1), false);
  c2_f_u = c2_e_u.wetmass;
  c2_g_y = NULL;
  sf_mex_assign(&c2_g_y, sf_mex_create("y", &c2_f_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_f_y, c2_g_y, "wetmass", "wetmass", 0);
  c2_g_u = c2_e_u.drymass;
  c2_h_y = NULL;
  sf_mex_assign(&c2_h_y, sf_mex_create("y", &c2_g_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_f_y, c2_h_y, "drymass", "drymass", 0);
  for (c2_i34 = 0; c2_i34 < 9; c2_i34++) {
    c2_h_u[c2_i34] = c2_e_u.type[c2_i34];
  }

  c2_i_y = NULL;
  sf_mex_assign(&c2_i_y, sf_mex_create("y", c2_h_u, 10, 0U, 1U, 0U, 2, 1, 9),
                false);
  sf_mex_addfield(c2_f_y, c2_i_y, "type", "type", 0);
  c2_i_u = c2_e_u.l;
  c2_j_y = NULL;
  sf_mex_assign(&c2_j_y, sf_mex_create("y", &c2_i_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_f_y, c2_j_y, "l", "l", 0);
  sf_mex_addfield(c2_b_y, c2_f_y, "nose", "nose", 0);
  c2_j_u = c2_u.shoulder;
  c2_k_y = NULL;
  sf_mex_assign(&c2_k_y, sf_mex_createstruct("structure", 2, 1, 1), false);
  c2_k_u = c2_j_u.l;
  c2_l_y = NULL;
  sf_mex_assign(&c2_l_y, sf_mex_create("y", &c2_k_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_k_y, c2_l_y, "l", "l", 0);
  sf_mex_addfield(c2_b_y, c2_k_y, "shoulder", "shoulder", 0);
  c2_l_u = c2_u.fore;
  c2_m_y = NULL;
  sf_mex_assign(&c2_m_y, sf_mex_createstruct("structure", 2, 1, 1), false);
  c2_m_u = c2_l_u.l;
  c2_n_y = NULL;
  sf_mex_assign(&c2_n_y, sf_mex_create("y", &c2_m_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_m_y, c2_n_y, "l", "l", 0);
  c2_n_u = c2_l_u.S;
  c2_o_y = NULL;
  sf_mex_assign(&c2_o_y, sf_mex_create("y", &c2_n_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_m_y, c2_o_y, "S", "S", 0);
  sf_mex_addfield(c2_b_y, c2_m_y, "fore", "fore", 0);
  c2_o_u = c2_u.aft;
  c2_p_y = NULL;
  sf_mex_assign(&c2_p_y, sf_mex_createstruct("structure", 2, 1, 1), false);
  c2_p_u = c2_o_u.l;
  c2_q_y = NULL;
  sf_mex_assign(&c2_q_y, sf_mex_create("y", &c2_p_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_p_y, c2_q_y, "l", "l", 0);
  c2_q_u = c2_o_u.wetmass;
  c2_r_y = NULL;
  sf_mex_assign(&c2_r_y, sf_mex_create("y", &c2_q_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_p_y, c2_r_y, "wetmass", "wetmass", 0);
  c2_r_u = c2_o_u.drymass;
  c2_s_y = NULL;
  sf_mex_assign(&c2_s_y, sf_mex_create("y", &c2_r_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_p_y, c2_s_y, "drymass", "drymass", 0);
  sf_mex_addfield(c2_b_y, c2_p_y, "aft", "aft", 0);
  c2_s_u = c2_u.fin;
  c2_t_y = NULL;
  sf_mex_assign(&c2_t_y, sf_mex_createstruct("structure", 2, 1, 1), false);
  c2_t_u = c2_s_u.t;
  c2_u_y = NULL;
  sf_mex_assign(&c2_u_y, sf_mex_create("y", &c2_t_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_t_y, c2_u_y, "t", "t", 0);
  c2_u_u = c2_s_u.span;
  c2_v_y = NULL;
  sf_mex_assign(&c2_v_y, sf_mex_create("y", &c2_u_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_t_y, c2_v_y, "span", "span", 0);
  c2_v_u = c2_s_u.rootchord;
  c2_w_y = NULL;
  sf_mex_assign(&c2_w_y, sf_mex_create("y", &c2_v_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_t_y, c2_w_y, "rootchord", "rootchord", 0);
  c2_w_u = c2_s_u.tipchord;
  c2_x_y = NULL;
  sf_mex_assign(&c2_x_y, sf_mex_create("y", &c2_w_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_t_y, c2_x_y, "tipchord", "tipchord", 0);
  c2_x_u = c2_s_u.sweep;
  c2_y_y = NULL;
  sf_mex_assign(&c2_y_y, sf_mex_create("y", &c2_x_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_t_y, c2_y_y, "sweep", "sweep", 0);
  c2_y_u = c2_s_u.S;
  c2_ab_y = NULL;
  sf_mex_assign(&c2_ab_y, sf_mex_create("y", &c2_y_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_t_y, c2_ab_y, "S", "S", 0);
  c2_ab_u = c2_s_u.AR;
  c2_bb_y = NULL;
  sf_mex_assign(&c2_bb_y, sf_mex_create("y", &c2_ab_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_t_y, c2_bb_y, "AR", "AR", 0);
  c2_bb_u = c2_s_u.num;
  c2_cb_y = NULL;
  sf_mex_assign(&c2_cb_y, sf_mex_create("y", &c2_bb_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_t_y, c2_cb_y, "num", "num", 0);
  sf_mex_addfield(c2_b_y, c2_t_y, "fin", "fin", 0);
  c2_cb_u = c2_u.l;
  c2_db_y = NULL;
  sf_mex_assign(&c2_db_y, sf_mex_create("y", &c2_cb_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_b_y, c2_db_y, "l", "l", 0);
  c2_db_u = c2_u.drogue;
  c2_eb_y = NULL;
  sf_mex_assign(&c2_eb_y, sf_mex_createstruct("structure", 2, 1, 1), false);
  c2_eb_u = c2_db_u.d;
  c2_fb_y = NULL;
  sf_mex_assign(&c2_fb_y, sf_mex_create("y", &c2_eb_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_eb_y, c2_fb_y, "d", "d", 0);
  c2_fb_u = c2_db_u.S;
  c2_gb_y = NULL;
  sf_mex_assign(&c2_gb_y, sf_mex_create("y", &c2_fb_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_eb_y, c2_gb_y, "S", "S", 0);
  c2_gb_u = c2_db_u.Cd;
  c2_hb_y = NULL;
  sf_mex_assign(&c2_hb_y, sf_mex_create("y", &c2_gb_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_eb_y, c2_hb_y, "Cd", "Cd", 0);
  c2_hb_u = c2_db_u.deploy_u;
  c2_ib_y = NULL;
  sf_mex_assign(&c2_ib_y, sf_mex_create("y", &c2_hb_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_eb_y, c2_ib_y, "deploy_u", "deploy_u", 0);
  sf_mex_addfield(c2_b_y, c2_eb_y, "drogue", "drogue", 0);
  c2_ib_u = c2_u.main;
  c2_jb_y = NULL;
  sf_mex_assign(&c2_jb_y, sf_mex_createstruct("structure", 2, 1, 1), false);
  c2_jb_u = c2_ib_u.d;
  c2_kb_y = NULL;
  sf_mex_assign(&c2_kb_y, sf_mex_create("y", &c2_jb_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_jb_y, c2_kb_y, "d", "d", 0);
  c2_kb_u = c2_ib_u.S;
  c2_lb_y = NULL;
  sf_mex_assign(&c2_lb_y, sf_mex_create("y", &c2_kb_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_jb_y, c2_lb_y, "S", "S", 0);
  c2_lb_u = c2_ib_u.Cd;
  c2_mb_y = NULL;
  sf_mex_assign(&c2_mb_y, sf_mex_create("y", &c2_lb_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_jb_y, c2_mb_y, "Cd", "Cd", 0);
  c2_mb_u = c2_ib_u.deploy_h;
  c2_nb_y = NULL;
  sf_mex_assign(&c2_nb_y, sf_mex_create("y", &c2_mb_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_jb_y, c2_nb_y, "deploy_h", "deploy_h", 0);
  sf_mex_addfield(c2_b_y, c2_jb_y, "main", "main", 0);
  c2_nb_u = c2_u.payload;
  c2_ob_y = NULL;
  sf_mex_assign(&c2_ob_y, sf_mex_createstruct("structure", 2, 1, 1), false);
  c2_ob_u = c2_nb_u.chute_d;
  c2_pb_y = NULL;
  sf_mex_assign(&c2_pb_y, sf_mex_create("y", &c2_ob_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_ob_y, c2_pb_y, "chute_d", "chute_d", 0);
  c2_pb_u = c2_nb_u.chute_Cd;
  c2_qb_y = NULL;
  sf_mex_assign(&c2_qb_y, sf_mex_create("y", &c2_pb_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_ob_y, c2_qb_y, "chute_Cd", "chute_Cd", 0);
  sf_mex_addfield(c2_b_y, c2_ob_y, "payload", "payload", 0);
  sf_mex_assign(&c2_mxArrayOutData, c2_b_y, false);
  return c2_mxArrayOutData;
}

static void c2_c_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c2_mxArrayInData, const char_T *c2_varName, void *c2_outData)
{
  const mxArray *c2_rocket;
  const char_T *c2_identifier;
  emlrtMsgIdentifier c2_thisId;
  c2_sv2voxIt8FKzjMlUEnyXmnC c2_b_y;
  SFc2_solver_simulinkInstanceStruct *chartInstance;
  chartInstance = (SFc2_solver_simulinkInstanceStruct *)chartInstanceVoid;
  c2_rocket = sf_mex_dup(c2_mxArrayInData);
  c2_identifier = c2_varName;
  c2_thisId.fIdentifier = c2_identifier;
  c2_thisId.fParent = NULL;
  c2_thisId.bParentIsCell = false;
  c2_d_emlrt_marshallIn(chartInstance, sf_mex_dup(c2_rocket), &c2_thisId,
                        &c2_b_y);
  sf_mex_destroy(&c2_rocket);
  *(c2_sv2voxIt8FKzjMlUEnyXmnC *)c2_outData = c2_b_y;
  sf_mex_destroy(&c2_mxArrayInData);
}

static const mxArray *c2_f_sf_marshallOut(void *chartInstanceVoid, void
  *c2_inData)
{
  const mxArray *c2_mxArrayOutData;
  c2_szHckfuHwXXVCwxC2kXrI2C c2_u;
  const mxArray *c2_b_y = NULL;
  real_T c2_b_u;
  const mxArray *c2_c_y = NULL;
  real_T c2_c_u;
  const mxArray *c2_d_y = NULL;
  c2_sv2voxIt8FKzjMlUEnyXmnC c2_d_u;
  const mxArray *c2_e_y = NULL;
  real_T c2_e_u;
  const mxArray *c2_f_y = NULL;
  real_T c2_f_u;
  const mxArray *c2_g_y = NULL;
  real_T c2_g_u;
  const mxArray *c2_h_y = NULL;
  c2_sof5eNcdcizKAe6jjm9xhtG c2_h_u;
  const mxArray *c2_i_y = NULL;
  real_T c2_i_u;
  const mxArray *c2_j_y = NULL;
  real_T c2_j_u;
  const mxArray *c2_k_y = NULL;
  int32_T c2_i35;
  const mxArray *c2_l_y = NULL;
  char_T c2_k_u[9];
  real_T c2_l_u;
  const mxArray *c2_m_y = NULL;
  c2_syx7FYmGk3I6e3U1RUwUAWD c2_m_u;
  const mxArray *c2_n_y = NULL;
  real_T c2_n_u;
  const mxArray *c2_o_y = NULL;
  c2_sqGsSc4djWypCV90lhUGiL c2_o_u;
  const mxArray *c2_p_y = NULL;
  real_T c2_p_u;
  const mxArray *c2_q_y = NULL;
  real_T c2_q_u;
  const mxArray *c2_r_y = NULL;
  c2_su2bTaGhbTlLQVXsqMBjEuH c2_r_u;
  const mxArray *c2_s_y = NULL;
  real_T c2_s_u;
  const mxArray *c2_t_y = NULL;
  real_T c2_t_u;
  const mxArray *c2_u_y = NULL;
  real_T c2_u_u;
  const mxArray *c2_v_y = NULL;
  c2_sNlKjRKFHwjFKC34GeUJivG c2_v_u;
  const mxArray *c2_w_y = NULL;
  real_T c2_w_u;
  const mxArray *c2_x_y = NULL;
  real_T c2_x_u;
  const mxArray *c2_y_y = NULL;
  real_T c2_y_u;
  const mxArray *c2_ab_y = NULL;
  real_T c2_ab_u;
  const mxArray *c2_bb_y = NULL;
  real_T c2_bb_u;
  const mxArray *c2_cb_y = NULL;
  real_T c2_cb_u;
  const mxArray *c2_db_y = NULL;
  real_T c2_db_u;
  const mxArray *c2_eb_y = NULL;
  real_T c2_eb_u;
  const mxArray *c2_fb_y = NULL;
  real_T c2_fb_u;
  const mxArray *c2_gb_y = NULL;
  c2_sKS3cuSxR3HIcTIZZkbRPm c2_gb_u;
  const mxArray *c2_hb_y = NULL;
  real_T c2_hb_u;
  const mxArray *c2_ib_y = NULL;
  real_T c2_ib_u;
  const mxArray *c2_jb_y = NULL;
  real_T c2_jb_u;
  const mxArray *c2_kb_y = NULL;
  real_T c2_kb_u;
  const mxArray *c2_lb_y = NULL;
  c2_s88seaxzVTYYZIzTGqmP4AD c2_lb_u;
  const mxArray *c2_mb_y = NULL;
  real_T c2_mb_u;
  const mxArray *c2_nb_y = NULL;
  real_T c2_nb_u;
  const mxArray *c2_ob_y = NULL;
  real_T c2_ob_u;
  const mxArray *c2_pb_y = NULL;
  real_T c2_pb_u;
  const mxArray *c2_qb_y = NULL;
  c2_sfpFxKVMN8elArLoV353vAE c2_qb_u;
  const mxArray *c2_rb_y = NULL;
  real_T c2_rb_u;
  const mxArray *c2_sb_y = NULL;
  real_T c2_sb_u;
  const mxArray *c2_tb_y = NULL;
  SFc2_solver_simulinkInstanceStruct *chartInstance;
  chartInstance = (SFc2_solver_simulinkInstanceStruct *)chartInstanceVoid;
  c2_mxArrayOutData = NULL;
  c2_mxArrayOutData = NULL;
  c2_u = *(c2_szHckfuHwXXVCwxC2kXrI2C *)c2_inData;
  c2_b_y = NULL;
  sf_mex_assign(&c2_b_y, sf_mex_createstruct("structure", 2, 1, 1), false);
  c2_b_u = c2_u.CM;
  c2_c_y = NULL;
  sf_mex_assign(&c2_c_y, sf_mex_create("y", &c2_b_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_b_y, c2_c_y, "CM", "CM", 0);
  c2_c_u = c2_u.CP;
  c2_d_y = NULL;
  sf_mex_assign(&c2_d_y, sf_mex_create("y", &c2_c_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_b_y, c2_d_y, "CP", "CP", 0);
  c2_d_u = c2_u.rocket;
  c2_e_y = NULL;
  sf_mex_assign(&c2_e_y, sf_mex_createstruct("structure", 2, 1, 1), false);
  c2_e_u = c2_d_u.OD;
  c2_f_y = NULL;
  sf_mex_assign(&c2_f_y, sf_mex_create("y", &c2_e_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_e_y, c2_f_y, "OD", "OD", 0);
  c2_f_u = c2_d_u.drymass;
  c2_g_y = NULL;
  sf_mex_assign(&c2_g_y, sf_mex_create("y", &c2_f_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_e_y, c2_g_y, "drymass", "drymass", 0);
  c2_g_u = c2_d_u.wetmass;
  c2_h_y = NULL;
  sf_mex_assign(&c2_h_y, sf_mex_create("y", &c2_g_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_e_y, c2_h_y, "wetmass", "wetmass", 0);
  c2_h_u = c2_d_u.nose;
  c2_i_y = NULL;
  sf_mex_assign(&c2_i_y, sf_mex_createstruct("structure", 2, 1, 1), false);
  c2_i_u = c2_h_u.wetmass;
  c2_j_y = NULL;
  sf_mex_assign(&c2_j_y, sf_mex_create("y", &c2_i_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_i_y, c2_j_y, "wetmass", "wetmass", 0);
  c2_j_u = c2_h_u.drymass;
  c2_k_y = NULL;
  sf_mex_assign(&c2_k_y, sf_mex_create("y", &c2_j_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_i_y, c2_k_y, "drymass", "drymass", 0);
  for (c2_i35 = 0; c2_i35 < 9; c2_i35++) {
    c2_k_u[c2_i35] = c2_h_u.type[c2_i35];
  }

  c2_l_y = NULL;
  sf_mex_assign(&c2_l_y, sf_mex_create("y", c2_k_u, 10, 0U, 1U, 0U, 2, 1, 9),
                false);
  sf_mex_addfield(c2_i_y, c2_l_y, "type", "type", 0);
  c2_l_u = c2_h_u.l;
  c2_m_y = NULL;
  sf_mex_assign(&c2_m_y, sf_mex_create("y", &c2_l_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_i_y, c2_m_y, "l", "l", 0);
  sf_mex_addfield(c2_e_y, c2_i_y, "nose", "nose", 0);
  c2_m_u = c2_d_u.shoulder;
  c2_n_y = NULL;
  sf_mex_assign(&c2_n_y, sf_mex_createstruct("structure", 2, 1, 1), false);
  c2_n_u = c2_m_u.l;
  c2_o_y = NULL;
  sf_mex_assign(&c2_o_y, sf_mex_create("y", &c2_n_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_n_y, c2_o_y, "l", "l", 0);
  sf_mex_addfield(c2_e_y, c2_n_y, "shoulder", "shoulder", 0);
  c2_o_u = c2_d_u.fore;
  c2_p_y = NULL;
  sf_mex_assign(&c2_p_y, sf_mex_createstruct("structure", 2, 1, 1), false);
  c2_p_u = c2_o_u.l;
  c2_q_y = NULL;
  sf_mex_assign(&c2_q_y, sf_mex_create("y", &c2_p_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_p_y, c2_q_y, "l", "l", 0);
  c2_q_u = c2_o_u.S;
  c2_r_y = NULL;
  sf_mex_assign(&c2_r_y, sf_mex_create("y", &c2_q_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_p_y, c2_r_y, "S", "S", 0);
  sf_mex_addfield(c2_e_y, c2_p_y, "fore", "fore", 0);
  c2_r_u = c2_d_u.aft;
  c2_s_y = NULL;
  sf_mex_assign(&c2_s_y, sf_mex_createstruct("structure", 2, 1, 1), false);
  c2_s_u = c2_r_u.l;
  c2_t_y = NULL;
  sf_mex_assign(&c2_t_y, sf_mex_create("y", &c2_s_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_s_y, c2_t_y, "l", "l", 0);
  c2_t_u = c2_r_u.wetmass;
  c2_u_y = NULL;
  sf_mex_assign(&c2_u_y, sf_mex_create("y", &c2_t_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_s_y, c2_u_y, "wetmass", "wetmass", 0);
  c2_u_u = c2_r_u.drymass;
  c2_v_y = NULL;
  sf_mex_assign(&c2_v_y, sf_mex_create("y", &c2_u_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_s_y, c2_v_y, "drymass", "drymass", 0);
  sf_mex_addfield(c2_e_y, c2_s_y, "aft", "aft", 0);
  c2_v_u = c2_d_u.fin;
  c2_w_y = NULL;
  sf_mex_assign(&c2_w_y, sf_mex_createstruct("structure", 2, 1, 1), false);
  c2_w_u = c2_v_u.t;
  c2_x_y = NULL;
  sf_mex_assign(&c2_x_y, sf_mex_create("y", &c2_w_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_w_y, c2_x_y, "t", "t", 0);
  c2_x_u = c2_v_u.span;
  c2_y_y = NULL;
  sf_mex_assign(&c2_y_y, sf_mex_create("y", &c2_x_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_w_y, c2_y_y, "span", "span", 0);
  c2_y_u = c2_v_u.rootchord;
  c2_ab_y = NULL;
  sf_mex_assign(&c2_ab_y, sf_mex_create("y", &c2_y_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_w_y, c2_ab_y, "rootchord", "rootchord", 0);
  c2_ab_u = c2_v_u.tipchord;
  c2_bb_y = NULL;
  sf_mex_assign(&c2_bb_y, sf_mex_create("y", &c2_ab_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_w_y, c2_bb_y, "tipchord", "tipchord", 0);
  c2_bb_u = c2_v_u.sweep;
  c2_cb_y = NULL;
  sf_mex_assign(&c2_cb_y, sf_mex_create("y", &c2_bb_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_w_y, c2_cb_y, "sweep", "sweep", 0);
  c2_cb_u = c2_v_u.S;
  c2_db_y = NULL;
  sf_mex_assign(&c2_db_y, sf_mex_create("y", &c2_cb_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_w_y, c2_db_y, "S", "S", 0);
  c2_db_u = c2_v_u.AR;
  c2_eb_y = NULL;
  sf_mex_assign(&c2_eb_y, sf_mex_create("y", &c2_db_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_w_y, c2_eb_y, "AR", "AR", 0);
  c2_eb_u = c2_v_u.num;
  c2_fb_y = NULL;
  sf_mex_assign(&c2_fb_y, sf_mex_create("y", &c2_eb_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_w_y, c2_fb_y, "num", "num", 0);
  sf_mex_addfield(c2_e_y, c2_w_y, "fin", "fin", 0);
  c2_fb_u = c2_d_u.l;
  c2_gb_y = NULL;
  sf_mex_assign(&c2_gb_y, sf_mex_create("y", &c2_fb_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_e_y, c2_gb_y, "l", "l", 0);
  c2_gb_u = c2_d_u.drogue;
  c2_hb_y = NULL;
  sf_mex_assign(&c2_hb_y, sf_mex_createstruct("structure", 2, 1, 1), false);
  c2_hb_u = c2_gb_u.d;
  c2_ib_y = NULL;
  sf_mex_assign(&c2_ib_y, sf_mex_create("y", &c2_hb_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_hb_y, c2_ib_y, "d", "d", 0);
  c2_ib_u = c2_gb_u.S;
  c2_jb_y = NULL;
  sf_mex_assign(&c2_jb_y, sf_mex_create("y", &c2_ib_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_hb_y, c2_jb_y, "S", "S", 0);
  c2_jb_u = c2_gb_u.Cd;
  c2_kb_y = NULL;
  sf_mex_assign(&c2_kb_y, sf_mex_create("y", &c2_jb_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_hb_y, c2_kb_y, "Cd", "Cd", 0);
  c2_kb_u = c2_gb_u.deploy_u;
  c2_lb_y = NULL;
  sf_mex_assign(&c2_lb_y, sf_mex_create("y", &c2_kb_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_hb_y, c2_lb_y, "deploy_u", "deploy_u", 0);
  sf_mex_addfield(c2_e_y, c2_hb_y, "drogue", "drogue", 0);
  c2_lb_u = c2_d_u.main;
  c2_mb_y = NULL;
  sf_mex_assign(&c2_mb_y, sf_mex_createstruct("structure", 2, 1, 1), false);
  c2_mb_u = c2_lb_u.d;
  c2_nb_y = NULL;
  sf_mex_assign(&c2_nb_y, sf_mex_create("y", &c2_mb_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_mb_y, c2_nb_y, "d", "d", 0);
  c2_nb_u = c2_lb_u.S;
  c2_ob_y = NULL;
  sf_mex_assign(&c2_ob_y, sf_mex_create("y", &c2_nb_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_mb_y, c2_ob_y, "S", "S", 0);
  c2_ob_u = c2_lb_u.Cd;
  c2_pb_y = NULL;
  sf_mex_assign(&c2_pb_y, sf_mex_create("y", &c2_ob_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_mb_y, c2_pb_y, "Cd", "Cd", 0);
  c2_pb_u = c2_lb_u.deploy_h;
  c2_qb_y = NULL;
  sf_mex_assign(&c2_qb_y, sf_mex_create("y", &c2_pb_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_mb_y, c2_qb_y, "deploy_h", "deploy_h", 0);
  sf_mex_addfield(c2_e_y, c2_mb_y, "main", "main", 0);
  c2_qb_u = c2_d_u.payload;
  c2_rb_y = NULL;
  sf_mex_assign(&c2_rb_y, sf_mex_createstruct("structure", 2, 1, 1), false);
  c2_rb_u = c2_qb_u.chute_d;
  c2_sb_y = NULL;
  sf_mex_assign(&c2_sb_y, sf_mex_create("y", &c2_rb_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_rb_y, c2_sb_y, "chute_d", "chute_d", 0);
  c2_sb_u = c2_qb_u.chute_Cd;
  c2_tb_y = NULL;
  sf_mex_assign(&c2_tb_y, sf_mex_create("y", &c2_sb_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_rb_y, c2_tb_y, "chute_Cd", "chute_Cd", 0);
  sf_mex_addfield(c2_e_y, c2_rb_y, "payload", "payload", 0);
  sf_mex_addfield(c2_b_y, c2_e_y, "rocket", "rocket", 0);
  sf_mex_assign(&c2_mxArrayOutData, c2_b_y, false);
  return c2_mxArrayOutData;
}

static void c2_d_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c2_mxArrayInData, const char_T *c2_varName, void *c2_outData)
{
  const mxArray *c2_load;
  const char_T *c2_identifier;
  emlrtMsgIdentifier c2_thisId;
  c2_szHckfuHwXXVCwxC2kXrI2C c2_b_y;
  SFc2_solver_simulinkInstanceStruct *chartInstance;
  chartInstance = (SFc2_solver_simulinkInstanceStruct *)chartInstanceVoid;
  c2_load = sf_mex_dup(c2_mxArrayInData);
  c2_identifier = c2_varName;
  c2_thisId.fIdentifier = c2_identifier;
  c2_thisId.fParent = NULL;
  c2_thisId.bParentIsCell = false;
  c2_b_emlrt_marshallIn(chartInstance, sf_mex_dup(c2_load), &c2_thisId, &c2_b_y);
  sf_mex_destroy(&c2_load);
  *(c2_szHckfuHwXXVCwxC2kXrI2C *)c2_outData = c2_b_y;
  sf_mex_destroy(&c2_mxArrayInData);
}

static const mxArray *c2_g_sf_marshallOut(void *chartInstanceVoid, void
  *c2_inData)
{
  const mxArray *c2_mxArrayOutData;
  int32_T c2_i36;
  const mxArray *c2_b_y = NULL;
  real_T c2_u[7];
  SFc2_solver_simulinkInstanceStruct *chartInstance;
  chartInstance = (SFc2_solver_simulinkInstanceStruct *)chartInstanceVoid;
  c2_mxArrayOutData = NULL;
  c2_mxArrayOutData = NULL;
  for (c2_i36 = 0; c2_i36 < 7; c2_i36++) {
    c2_u[c2_i36] = (*(real_T (*)[7])c2_inData)[c2_i36];
  }

  c2_b_y = NULL;
  sf_mex_assign(&c2_b_y, sf_mex_create("y", c2_u, 0, 0U, 1U, 0U, 2, 1, 7), false);
  sf_mex_assign(&c2_mxArrayOutData, c2_b_y, false);
  return c2_mxArrayOutData;
}

static void c2_p_emlrt_marshallIn(SFc2_solver_simulinkInstanceStruct
  *chartInstance, const mxArray *c2_u, const emlrtMsgIdentifier *c2_parentId,
  real_T c2_b_y[7])
{
  real_T c2_dv4[7];
  int32_T c2_i37;
  (void)chartInstance;
  sf_mex_import(c2_parentId, sf_mex_dup(c2_u), c2_dv4, 1, 0, 0U, 1, 0U, 2, 1, 7);
  for (c2_i37 = 0; c2_i37 < 7; c2_i37++) {
    c2_b_y[c2_i37] = c2_dv4[c2_i37];
  }

  sf_mex_destroy(&c2_u);
}

static void c2_e_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c2_mxArrayInData, const char_T *c2_varName, void *c2_outData)
{
  const mxArray *c2_hpos;
  const char_T *c2_identifier;
  emlrtMsgIdentifier c2_thisId;
  real_T c2_b_y[7];
  int32_T c2_i38;
  SFc2_solver_simulinkInstanceStruct *chartInstance;
  chartInstance = (SFc2_solver_simulinkInstanceStruct *)chartInstanceVoid;
  c2_hpos = sf_mex_dup(c2_mxArrayInData);
  c2_identifier = c2_varName;
  c2_thisId.fIdentifier = c2_identifier;
  c2_thisId.fParent = NULL;
  c2_thisId.bParentIsCell = false;
  c2_p_emlrt_marshallIn(chartInstance, sf_mex_dup(c2_hpos), &c2_thisId, c2_b_y);
  sf_mex_destroy(&c2_hpos);
  for (c2_i38 = 0; c2_i38 < 7; c2_i38++) {
    (*(real_T (*)[7])c2_outData)[c2_i38] = c2_b_y[c2_i38];
  }

  sf_mex_destroy(&c2_mxArrayInData);
}

static const mxArray *c2_h_sf_marshallOut(void *chartInstanceVoid, void
  *c2_inData)
{
  const mxArray *c2_mxArrayOutData;
  int32_T c2_i39;
  const mxArray *c2_b_y = NULL;
  real_T c2_u[8];
  SFc2_solver_simulinkInstanceStruct *chartInstance;
  chartInstance = (SFc2_solver_simulinkInstanceStruct *)chartInstanceVoid;
  c2_mxArrayOutData = NULL;
  c2_mxArrayOutData = NULL;
  for (c2_i39 = 0; c2_i39 < 8; c2_i39++) {
    c2_u[c2_i39] = (*(real_T (*)[8])c2_inData)[c2_i39];
  }

  c2_b_y = NULL;
  sf_mex_assign(&c2_b_y, sf_mex_create("y", c2_u, 0, 0U, 1U, 0U, 2, 1, 8), false);
  sf_mex_assign(&c2_mxArrayOutData, c2_b_y, false);
  return c2_mxArrayOutData;
}

static void c2_q_emlrt_marshallIn(SFc2_solver_simulinkInstanceStruct
  *chartInstance, const mxArray *c2_u, const emlrtMsgIdentifier *c2_parentId,
  real_T c2_b_y[8])
{
  real_T c2_dv5[8];
  int32_T c2_i40;
  (void)chartInstance;
  sf_mex_import(c2_parentId, sf_mex_dup(c2_u), c2_dv5, 1, 0, 0U, 1, 0U, 2, 1, 8);
  for (c2_i40 = 0; c2_i40 < 8; c2_i40++) {
    c2_b_y[c2_i40] = c2_dv5[c2_i40];
  }

  sf_mex_destroy(&c2_u);
}

static void c2_f_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c2_mxArrayInData, const char_T *c2_varName, void *c2_outData)
{
  const mxArray *c2_T0;
  const char_T *c2_identifier;
  emlrtMsgIdentifier c2_thisId;
  real_T c2_b_y[8];
  int32_T c2_i41;
  SFc2_solver_simulinkInstanceStruct *chartInstance;
  chartInstance = (SFc2_solver_simulinkInstanceStruct *)chartInstanceVoid;
  c2_T0 = sf_mex_dup(c2_mxArrayInData);
  c2_identifier = c2_varName;
  c2_thisId.fIdentifier = c2_identifier;
  c2_thisId.fParent = NULL;
  c2_thisId.bParentIsCell = false;
  c2_q_emlrt_marshallIn(chartInstance, sf_mex_dup(c2_T0), &c2_thisId, c2_b_y);
  sf_mex_destroy(&c2_T0);
  for (c2_i41 = 0; c2_i41 < 8; c2_i41++) {
    (*(real_T (*)[8])c2_outData)[c2_i41] = c2_b_y[c2_i41];
  }

  sf_mex_destroy(&c2_mxArrayInData);
}

static void c2_r_emlrt_marshallIn(SFc2_solver_simulinkInstanceStruct
  *chartInstance, const mxArray *c2_u, const emlrtMsgIdentifier *c2_parentId,
  real_T c2_b_y[17507])
{
  real_T c2_dv6[17507];
  int32_T c2_i42;
  (void)chartInstance;
  sf_mex_import(c2_parentId, sf_mex_dup(c2_u), c2_dv6, 1, 0, 0U, 1, 0U, 2, 2501,
                7);
  for (c2_i42 = 0; c2_i42 < 17507; c2_i42++) {
    c2_b_y[c2_i42] = c2_dv6[c2_i42];
  }

  sf_mex_destroy(&c2_u);
}

static void c2_g_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c2_mxArrayInData, const char_T *c2_varName, void *c2_outData)
{
  const mxArray *c2_d_aerodata;
  const char_T *c2_identifier;
  emlrtMsgIdentifier c2_thisId;
  int32_T c2_i43;
  int32_T c2_i44;
  int32_T c2_i45;
  SFc2_solver_simulinkInstanceStruct *chartInstance;
  chartInstance = (SFc2_solver_simulinkInstanceStruct *)chartInstanceVoid;
  c2_d_aerodata = sf_mex_dup(c2_mxArrayInData);
  c2_identifier = c2_varName;
  c2_thisId.fIdentifier = c2_identifier;
  c2_thisId.fParent = NULL;
  c2_thisId.bParentIsCell = false;
  c2_r_emlrt_marshallIn(chartInstance, sf_mex_dup(c2_d_aerodata), &c2_thisId,
                        chartInstance->c2_y);
  sf_mex_destroy(&c2_d_aerodata);
  c2_i43 = 0;
  for (c2_i44 = 0; c2_i44 < 7; c2_i44++) {
    for (c2_i45 = 0; c2_i45 < 2501; c2_i45++) {
      (*(real_T (*)[17507])c2_outData)[c2_i45 + c2_i43] = chartInstance->
        c2_y[c2_i45 + c2_i43];
    }

    c2_i43 += 2501;
  }

  sf_mex_destroy(&c2_mxArrayInData);
}

static const mxArray *c2_i_sf_marshallOut(void *chartInstanceVoid, void
  *c2_inData)
{
  const mxArray *c2_mxArrayOutData;
  int32_T c2_i46;
  const mxArray *c2_b_y = NULL;
  real_T c2_u[3];
  SFc2_solver_simulinkInstanceStruct *chartInstance;
  chartInstance = (SFc2_solver_simulinkInstanceStruct *)chartInstanceVoid;
  c2_mxArrayOutData = NULL;
  c2_mxArrayOutData = NULL;
  for (c2_i46 = 0; c2_i46 < 3; c2_i46++) {
    c2_u[c2_i46] = (*(real_T (*)[3])c2_inData)[c2_i46];
  }

  c2_b_y = NULL;
  sf_mex_assign(&c2_b_y, sf_mex_create("y", c2_u, 0, 0U, 1U, 0U, 1, 3), false);
  sf_mex_assign(&c2_mxArrayOutData, c2_b_y, false);
  return c2_mxArrayOutData;
}

static void c2_s_emlrt_marshallIn(SFc2_solver_simulinkInstanceStruct
  *chartInstance, const mxArray *c2_u, const emlrtMsgIdentifier *c2_parentId,
  real_T c2_b_y[3])
{
  real_T c2_dv7[3];
  int32_T c2_i47;
  (void)chartInstance;
  sf_mex_import(c2_parentId, sf_mex_dup(c2_u), c2_dv7, 1, 0, 0U, 1, 0U, 1, 3);
  for (c2_i47 = 0; c2_i47 < 3; c2_i47++) {
    c2_b_y[c2_i47] = c2_dv7[c2_i47];
  }

  sf_mex_destroy(&c2_u);
}

static void c2_h_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c2_mxArrayInData, const char_T *c2_varName, void *c2_outData)
{
  const mxArray *c2_u;
  const char_T *c2_identifier;
  emlrtMsgIdentifier c2_thisId;
  real_T c2_b_y[3];
  int32_T c2_i48;
  SFc2_solver_simulinkInstanceStruct *chartInstance;
  chartInstance = (SFc2_solver_simulinkInstanceStruct *)chartInstanceVoid;
  c2_u = sf_mex_dup(c2_mxArrayInData);
  c2_identifier = c2_varName;
  c2_thisId.fIdentifier = c2_identifier;
  c2_thisId.fParent = NULL;
  c2_thisId.bParentIsCell = false;
  c2_s_emlrt_marshallIn(chartInstance, sf_mex_dup(c2_u), &c2_thisId, c2_b_y);
  sf_mex_destroy(&c2_u);
  for (c2_i48 = 0; c2_i48 < 3; c2_i48++) {
    (*(real_T (*)[3])c2_outData)[c2_i48] = c2_b_y[c2_i48];
  }

  sf_mex_destroy(&c2_mxArrayInData);
}

static void c2_t_emlrt_marshallIn(SFc2_solver_simulinkInstanceStruct
  *chartInstance, const mxArray *c2_u, const emlrtMsgIdentifier *c2_parentId,
  real_T c2_b_y[1256])
{
  real_T c2_dv8[1256];
  int32_T c2_i49;
  (void)chartInstance;
  sf_mex_import(c2_parentId, sf_mex_dup(c2_u), c2_dv8, 1, 0, 0U, 1, 0U, 2, 628,
                2);
  for (c2_i49 = 0; c2_i49 < 1256; c2_i49++) {
    c2_b_y[c2_i49] = c2_dv8[c2_i49];
  }

  sf_mex_destroy(&c2_u);
}

static void c2_i_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c2_mxArrayInData, const char_T *c2_varName, void *c2_outData)
{
  const mxArray *c2_b_T;
  const char_T *c2_identifier;
  emlrtMsgIdentifier c2_thisId;
  real_T c2_b_y[1256];
  int32_T c2_i50;
  int32_T c2_i51;
  int32_T c2_i52;
  SFc2_solver_simulinkInstanceStruct *chartInstance;
  chartInstance = (SFc2_solver_simulinkInstanceStruct *)chartInstanceVoid;
  c2_b_T = sf_mex_dup(c2_mxArrayInData);
  c2_identifier = c2_varName;
  c2_thisId.fIdentifier = c2_identifier;
  c2_thisId.fParent = NULL;
  c2_thisId.bParentIsCell = false;
  c2_t_emlrt_marshallIn(chartInstance, sf_mex_dup(c2_b_T), &c2_thisId, c2_b_y);
  sf_mex_destroy(&c2_b_T);
  c2_i50 = 0;
  for (c2_i51 = 0; c2_i51 < 2; c2_i51++) {
    for (c2_i52 = 0; c2_i52 < 628; c2_i52++) {
      (*(real_T (*)[1256])c2_outData)[c2_i52 + c2_i50] = c2_b_y[c2_i52 + c2_i50];
    }

    c2_i50 += 628;
  }

  sf_mex_destroy(&c2_mxArrayInData);
}

const mxArray *sf_c2_solver_simulink_get_eml_resolved_functions_info(void)
{
  const mxArray *c2_nameCaptureInfo = NULL;
  const char * c2_data[5] = {
    "789ce555cb4ec240149dfa8a1a35bad0852b976eb420a8605c8808a811622c861863ccd00e30d076703a18d9b174e7ffb872e71ff83dced816c74aac015f8937"
    "b9999ede9edbfb6c817290075c66b8d6260118e3e738d721e0caa88715aec3dee9de1f01d31ebee3aa139ba11be61a6d6821e08b412c6c439b15db4d04287288",
    "798d8c174b059ba8882d744424b08f39b0b292a90b84495ca76b486f682d0bd09ad37d0d3065f0924f04bce63312c8c7173f9f598997f880a778369927708550"
    "1d39fdf31d465b3a73f99d10fe51802ff079e622bda59e3a883a6a3e135d8ba939cc6aadb24a89de40cc59c114e96a91c23ad219a1ed4b0d5bfc49c84c585ed2",
    "444fa8eae6b06af5aac3588f3814298e896e5d1f165399a754fffcde7df80dfe7e481fe6037d105833ae227bc76634c9aee32503e2c36c41dbf4fc2d4bfe941e"
    "fe8074f6f37c2724de9f9a9b9d9038a60271080c1125469b7f38b0eef87e06dda3db10fe49807ff245f5907371ab32c83c76eeb7ffc93e2d80b7fd1078bd50d9",
    "a866eb51588707cdc3f6693c46aa9b7b3fb34f7f6d7ec2ea37178847e02a6229669134b10dcc30b1fd7f64bffb659056d9449fab4f29c02f7d517ddee5c48b34"
    "c07c3e26fef17e25582ebf1e236737b9423369d6238dbc198def7eff7e3d038936ff87", ""
  };

  c2_nameCaptureInfo = NULL;
  emlrtNameCaptureMxArrayR2016a(c2_data, 2680U, &c2_nameCaptureInfo);
  return c2_nameCaptureInfo;
}

static void c2_error(SFc2_solver_simulinkInstanceStruct *chartInstance)
{
  const mxArray *c2_b_y = NULL;
  static char_T c2_cv2[31] = { 'C', 'o', 'd', 'e', 'r', ':', 't', 'o', 'o', 'l',
    'b', 'o', 'x', ':', 'p', 'o', 'w', 'e', 'r', '_', 'd', 'o', 'm', 'a', 'i',
    'n', 'E', 'r', 'r', 'o', 'r' };

  (void)chartInstance;
  c2_b_y = NULL;
  sf_mex_assign(&c2_b_y, sf_mex_create("y", c2_cv2, 10, 0U, 1U, 0U, 2, 1, 31),
                false);
  sf_mex_call_debug(sfGlobalDebugInstanceStruct, "error", 0U, 1U, 14,
                    sf_mex_call_debug(sfGlobalDebugInstanceStruct, "message", 1U,
    1U, 14, c2_b_y));
}

static real_T c2_sind(SFc2_solver_simulinkInstanceStruct *chartInstance, real_T
                      c2_x)
{
  real_T c2_b_x;
  c2_b_x = c2_x;
  c2_b_sind(chartInstance, &c2_b_x);
  return c2_b_x;
}

static real_T c2_cosd(SFc2_solver_simulinkInstanceStruct *chartInstance, real_T
                      c2_x)
{
  real_T c2_b_x;
  c2_b_x = c2_x;
  c2_b_cosd(chartInstance, &c2_b_x);
  return c2_b_x;
}

static real_T c2_norm(SFc2_solver_simulinkInstanceStruct *chartInstance, real_T
                      c2_x[2])
{
  real_T c2_b_y;
  real_T c2_scale;
  int32_T c2_k;
  real_T c2_b_x;
  real_T c2_c_x;
  real_T c2_d_x;
  real_T c2_absxk;
  real_T c2_b_t;
  (void)chartInstance;
  c2_b_y = 0.0;
  c2_scale = 2.2250738585072014E-308;
  for (c2_k = 0; c2_k + 1 < 3; c2_k++) {
    c2_b_x = c2_x[c2_k];
    c2_c_x = c2_b_x;
    c2_d_x = c2_c_x;
    c2_absxk = muDoubleScalarAbs(c2_d_x);
    if (c2_absxk > c2_scale) {
      c2_b_t = c2_scale / c2_absxk;
      c2_b_y = 1.0 + c2_b_y * c2_b_t * c2_b_t;
      c2_scale = c2_absxk;
    } else {
      c2_b_t = c2_absxk / c2_scale;
      c2_b_y += c2_b_t * c2_b_t;
    }
  }

  return c2_scale * muDoubleScalarSqrt(c2_b_y);
}

static void c2_b_error(SFc2_solver_simulinkInstanceStruct *chartInstance)
{
  const mxArray *c2_b_y = NULL;
  static char_T c2_cv3[30] = { 'C', 'o', 'd', 'e', 'r', ':', 't', 'o', 'o', 'l',
    'b', 'o', 'x', ':', 'E', 'l', 'F', 'u', 'n', 'D', 'o', 'm', 'a', 'i', 'n',
    'E', 'r', 'r', 'o', 'r' };

  const mxArray *c2_c_y = NULL;
  static char_T c2_cv4[5] = { 'a', 's', 'i', 'n', 'd' };

  (void)chartInstance;
  c2_b_y = NULL;
  sf_mex_assign(&c2_b_y, sf_mex_create("y", c2_cv3, 10, 0U, 1U, 0U, 2, 1, 30),
                false);
  c2_c_y = NULL;
  sf_mex_assign(&c2_c_y, sf_mex_create("y", c2_cv4, 10, 0U, 1U, 0U, 2, 1, 5),
                false);
  sf_mex_call_debug(sfGlobalDebugInstanceStruct, "error", 0U, 1U, 14,
                    sf_mex_call_debug(sfGlobalDebugInstanceStruct, "message", 1U,
    2U, 14, c2_b_y, 14, c2_c_y));
}

static void c2_c_error(SFc2_solver_simulinkInstanceStruct *chartInstance)
{
  const mxArray *c2_b_y = NULL;
  static char_T c2_cv5[30] = { 'C', 'o', 'd', 'e', 'r', ':', 't', 'o', 'o', 'l',
    'b', 'o', 'x', ':', 'E', 'l', 'F', 'u', 'n', 'D', 'o', 'm', 'a', 'i', 'n',
    'E', 'r', 'r', 'o', 'r' };

  const mxArray *c2_c_y = NULL;
  static char_T c2_cv6[4] = { 's', 'q', 'r', 't' };

  (void)chartInstance;
  c2_b_y = NULL;
  sf_mex_assign(&c2_b_y, sf_mex_create("y", c2_cv5, 10, 0U, 1U, 0U, 2, 1, 30),
                false);
  c2_c_y = NULL;
  sf_mex_assign(&c2_c_y, sf_mex_create("y", c2_cv6, 10, 0U, 1U, 0U, 2, 1, 4),
                false);
  sf_mex_call_debug(sfGlobalDebugInstanceStruct, "error", 0U, 1U, 14,
                    sf_mex_call_debug(sfGlobalDebugInstanceStruct, "message", 1U,
    2U, 14, c2_b_y, 14, c2_c_y));
}

static const mxArray *c2_j_sf_marshallOut(void *chartInstanceVoid, void
  *c2_inData)
{
  const mxArray *c2_mxArrayOutData;
  int32_T c2_u;
  const mxArray *c2_b_y = NULL;
  SFc2_solver_simulinkInstanceStruct *chartInstance;
  chartInstance = (SFc2_solver_simulinkInstanceStruct *)chartInstanceVoid;
  c2_mxArrayOutData = NULL;
  c2_mxArrayOutData = NULL;
  c2_u = *(int32_T *)c2_inData;
  c2_b_y = NULL;
  sf_mex_assign(&c2_b_y, sf_mex_create("y", &c2_u, 6, 0U, 0U, 0U, 0), false);
  sf_mex_assign(&c2_mxArrayOutData, c2_b_y, false);
  return c2_mxArrayOutData;
}

static int32_T c2_u_emlrt_marshallIn(SFc2_solver_simulinkInstanceStruct
  *chartInstance, const mxArray *c2_u, const emlrtMsgIdentifier *c2_parentId)
{
  int32_T c2_b_y;
  int32_T c2_i53;
  (void)chartInstance;
  sf_mex_import(c2_parentId, sf_mex_dup(c2_u), &c2_i53, 1, 6, 0U, 0, 0U, 0);
  c2_b_y = c2_i53;
  sf_mex_destroy(&c2_u);
  return c2_b_y;
}

static void c2_j_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c2_mxArrayInData, const char_T *c2_varName, void *c2_outData)
{
  const mxArray *c2_b_sfEvent;
  const char_T *c2_identifier;
  emlrtMsgIdentifier c2_thisId;
  int32_T c2_b_y;
  SFc2_solver_simulinkInstanceStruct *chartInstance;
  chartInstance = (SFc2_solver_simulinkInstanceStruct *)chartInstanceVoid;
  c2_b_sfEvent = sf_mex_dup(c2_mxArrayInData);
  c2_identifier = c2_varName;
  c2_thisId.fIdentifier = c2_identifier;
  c2_thisId.fParent = NULL;
  c2_thisId.bParentIsCell = false;
  c2_b_y = c2_u_emlrt_marshallIn(chartInstance, sf_mex_dup(c2_b_sfEvent),
    &c2_thisId);
  sf_mex_destroy(&c2_b_sfEvent);
  *(int32_T *)c2_outData = c2_b_y;
  sf_mex_destroy(&c2_mxArrayInData);
}

static uint8_T c2_v_emlrt_marshallIn(SFc2_solver_simulinkInstanceStruct
  *chartInstance, const mxArray *c2_b_is_active_c2_solver_simulink, const char_T
  *c2_identifier)
{
  uint8_T c2_b_y;
  emlrtMsgIdentifier c2_thisId;
  c2_thisId.fIdentifier = c2_identifier;
  c2_thisId.fParent = NULL;
  c2_thisId.bParentIsCell = false;
  c2_b_y = c2_w_emlrt_marshallIn(chartInstance, sf_mex_dup
    (c2_b_is_active_c2_solver_simulink), &c2_thisId);
  sf_mex_destroy(&c2_b_is_active_c2_solver_simulink);
  return c2_b_y;
}

static uint8_T c2_w_emlrt_marshallIn(SFc2_solver_simulinkInstanceStruct
  *chartInstance, const mxArray *c2_u, const emlrtMsgIdentifier *c2_parentId)
{
  uint8_T c2_b_y;
  uint8_T c2_u0;
  (void)chartInstance;
  sf_mex_import(c2_parentId, sf_mex_dup(c2_u), &c2_u0, 1, 3, 0U, 0, 0U, 0);
  c2_b_y = c2_u0;
  sf_mex_destroy(&c2_u);
  return c2_b_y;
}

static void c2_b_sind(SFc2_solver_simulinkInstanceStruct *chartInstance, real_T *
                      c2_x)
{
  real_T c2_b_x;
  real_T c2_c_x;
  real_T c2_d_x;
  boolean_T c2_b;
  boolean_T c2_b2;
  real_T c2_e_x;
  boolean_T c2_b_b;
  boolean_T c2_b3;
  boolean_T c2_c_b;
  real_T c2_f_x;
  real_T c2_g_x;
  real_T c2_h_x;
  real_T c2_i_x;
  real_T c2_j_x;
  real_T c2_k_x;
  real_T c2_l_x;
  real_T c2_absx;
  int8_T c2_n;
  real_T c2_m_x;
  real_T c2_n_x;
  real_T c2_o_x;
  (void)chartInstance;
  c2_b_x = *c2_x;
  c2_c_x = c2_b_x;
  c2_d_x = c2_c_x;
  c2_b = muDoubleScalarIsInf(c2_d_x);
  c2_b2 = !c2_b;
  c2_e_x = c2_c_x;
  c2_b_b = muDoubleScalarIsNaN(c2_e_x);
  c2_b3 = !c2_b_b;
  c2_c_b = (c2_b2 && c2_b3);
  if (!c2_c_b) {
    c2_g_x = rtNaN;
  } else {
    c2_f_x = c2_b_x;
    c2_h_x = c2_f_x;
    c2_i_x = c2_h_x;
    c2_j_x = c2_i_x;
    c2_k_x = c2_j_x;
    c2_h_x = muDoubleScalarRem(c2_k_x, 360.0);
    c2_l_x = c2_h_x;
    c2_absx = muDoubleScalarAbs(c2_l_x);
    if (c2_absx > 180.0) {
      if (c2_h_x > 0.0) {
        c2_h_x -= 360.0;
      } else {
        c2_h_x += 360.0;
      }

      c2_m_x = c2_h_x;
      c2_n_x = c2_m_x;
      c2_o_x = c2_n_x;
      c2_absx = muDoubleScalarAbs(c2_o_x);
    }

    if (c2_absx <= 45.0) {
      c2_h_x *= 0.017453292519943295;
      c2_n = 0;
    } else if (c2_absx <= 135.0) {
      if (c2_h_x > 0.0) {
        c2_h_x = 0.017453292519943295 * (c2_h_x - 90.0);
        c2_n = 1;
      } else {
        c2_h_x = 0.017453292519943295 * (c2_h_x + 90.0);
        c2_n = -1;
      }
    } else if (c2_h_x > 0.0) {
      c2_h_x = 0.017453292519943295 * (c2_h_x - 180.0);
      c2_n = 2;
    } else {
      c2_h_x = 0.017453292519943295 * (c2_h_x + 180.0);
      c2_n = -2;
    }

    if ((real_T)c2_n == 0.0) {
      c2_g_x = muDoubleScalarSin(c2_h_x);
    } else if ((real_T)c2_n == 1.0) {
      c2_g_x = muDoubleScalarCos(c2_h_x);
    } else if ((real_T)c2_n == -1.0) {
      c2_g_x = -muDoubleScalarCos(c2_h_x);
    } else {
      c2_g_x = -muDoubleScalarSin(c2_h_x);
    }
  }

  *c2_x = c2_g_x;
}

static void c2_b_cosd(SFc2_solver_simulinkInstanceStruct *chartInstance, real_T *
                      c2_x)
{
  real_T c2_b_x;
  real_T c2_c_x;
  real_T c2_d_x;
  boolean_T c2_b;
  boolean_T c2_b4;
  real_T c2_e_x;
  boolean_T c2_b_b;
  boolean_T c2_b5;
  boolean_T c2_c_b;
  real_T c2_f_x;
  real_T c2_g_x;
  real_T c2_h_x;
  real_T c2_i_x;
  real_T c2_j_x;
  real_T c2_k_x;
  real_T c2_absx;
  int8_T c2_n;
  real_T c2_l_x;
  real_T c2_m_x;
  real_T c2_n_x;
  (void)chartInstance;
  c2_b_x = *c2_x;
  c2_c_x = c2_b_x;
  c2_d_x = c2_c_x;
  c2_b = muDoubleScalarIsInf(c2_d_x);
  c2_b4 = !c2_b;
  c2_e_x = c2_c_x;
  c2_b_b = muDoubleScalarIsNaN(c2_e_x);
  c2_b5 = !c2_b_b;
  c2_c_b = (c2_b4 && c2_b5);
  if (!c2_c_b) {
    *c2_x = rtNaN;
  } else {
    c2_f_x = c2_b_x;
    c2_g_x = c2_f_x;
    c2_h_x = c2_g_x;
    c2_i_x = c2_h_x;
    c2_j_x = c2_i_x;
    c2_g_x = muDoubleScalarRem(c2_j_x, 360.0);
    c2_k_x = c2_g_x;
    c2_absx = muDoubleScalarAbs(c2_k_x);
    if (c2_absx > 180.0) {
      if (c2_g_x > 0.0) {
        c2_g_x -= 360.0;
      } else {
        c2_g_x += 360.0;
      }

      c2_l_x = c2_g_x;
      c2_m_x = c2_l_x;
      c2_n_x = c2_m_x;
      c2_absx = muDoubleScalarAbs(c2_n_x);
    }

    if (c2_absx <= 45.0) {
      c2_g_x *= 0.017453292519943295;
      c2_n = 0;
    } else if (c2_absx <= 135.0) {
      if (c2_g_x > 0.0) {
        c2_g_x = 0.017453292519943295 * (c2_g_x - 90.0);
        c2_n = 1;
      } else {
        c2_g_x = 0.017453292519943295 * (c2_g_x + 90.0);
        c2_n = -1;
      }
    } else if (c2_g_x > 0.0) {
      c2_g_x = 0.017453292519943295 * (c2_g_x - 180.0);
      c2_n = 2;
    } else {
      c2_g_x = 0.017453292519943295 * (c2_g_x + 180.0);
      c2_n = -2;
    }

    if ((real_T)c2_n == 0.0) {
      *c2_x = muDoubleScalarCos(c2_g_x);
    } else if ((real_T)c2_n == 1.0) {
      *c2_x = -muDoubleScalarSin(c2_g_x);
    } else if ((real_T)c2_n == -1.0) {
      *c2_x = muDoubleScalarSin(c2_g_x);
    } else {
      *c2_x = -muDoubleScalarCos(c2_g_x);
    }
  }
}

static void init_dsm_address_info(SFc2_solver_simulinkInstanceStruct
  *chartInstance)
{
  (void)chartInstance;
}

static void init_simulink_io_address(SFc2_solver_simulinkInstanceStruct
  *chartInstance)
{
  chartInstance->c2_t = (real_T *)ssGetInputPortSignal_wrapper(chartInstance->S,
    0);
  chartInstance->c2_t_step = (real_T *)ssGetInputPortSignal_wrapper
    (chartInstance->S, 1);
  chartInstance->c2_statedot = (real_T (*)[6])ssGetOutputPortSignal_wrapper
    (chartInstance->S, 1);
  chartInstance->c2_state = (real_T (*)[6])ssGetInputPortSignal_wrapper
    (chartInstance->S, 2);
  chartInstance->c2_rocketOD = (real_T *)ssGetInputPortSignal_wrapper
    (chartInstance->S, 3);
  chartInstance->c2_T = (real_T (*)[1256])ssGetInputPortSignal_wrapper
    (chartInstance->S, 4);
  chartInstance->c2_mass = (real_T (*)[1256])ssGetInputPortSignal_wrapper
    (chartInstance->S, 5);
  chartInstance->c2_wind = (real_T *)ssGetInputPortSignal_wrapper
    (chartInstance->S, 6);
  chartInstance->c2_c_aerodata = (real_T (*)[17507])ssGetInputPortSignal_wrapper
    (chartInstance->S, 7);
}

/* SFunction Glue Code */
#ifdef utFree
#undef utFree
#endif

#ifdef utMalloc
#undef utMalloc
#endif

#ifdef __cplusplus

extern "C" void *utMalloc(size_t size);
extern "C" void utFree(void*);

#else

extern void *utMalloc(size_t size);
extern void utFree(void*);

#endif

void sf_c2_solver_simulink_get_check_sum(mxArray *plhs[])
{
  ((real_T *)mxGetPr((plhs[0])))[0] = (real_T)(3188091824U);
  ((real_T *)mxGetPr((plhs[0])))[1] = (real_T)(2296906022U);
  ((real_T *)mxGetPr((plhs[0])))[2] = (real_T)(2627482510U);
  ((real_T *)mxGetPr((plhs[0])))[3] = (real_T)(2309335490U);
}

mxArray* sf_c2_solver_simulink_get_post_codegen_info(void);
mxArray *sf_c2_solver_simulink_get_autoinheritance_info(void)
{
  const char *autoinheritanceFields[] = { "checksum", "inputs", "parameters",
    "outputs", "locals", "postCodegenInfo" };

  mxArray *mxAutoinheritanceInfo = mxCreateStructMatrix(1, 1, sizeof
    (autoinheritanceFields)/sizeof(autoinheritanceFields[0]),
    autoinheritanceFields);

  {
    mxArray *mxChecksum = mxCreateString("FB5OPtMGxRZT3TlTy2E4cG");
    mxSetField(mxAutoinheritanceInfo,0,"checksum",mxChecksum);
  }

  {
    const char *dataFields[] = { "size", "type", "complexity" };

    mxArray *mxData = mxCreateStructMatrix(1,8,3,dataFields);

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,0,mxREAL);
      double *pr = mxGetPr(mxSize);
      mxSetField(mxData,0,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt", "isFixedPointType" };

      mxArray *mxType = mxCreateStructMatrix(1,1,sizeof(typeFields)/sizeof
        (typeFields[0]),typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxType,0,"isFixedPointType",mxCreateDoubleScalar(0));
      mxSetField(mxData,0,"type",mxType);
    }

    mxSetField(mxData,0,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,0,mxREAL);
      double *pr = mxGetPr(mxSize);
      mxSetField(mxData,1,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt", "isFixedPointType" };

      mxArray *mxType = mxCreateStructMatrix(1,1,sizeof(typeFields)/sizeof
        (typeFields[0]),typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxType,0,"isFixedPointType",mxCreateDoubleScalar(0));
      mxSetField(mxData,1,"type",mxType);
    }

    mxSetField(mxData,1,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,1,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(6);
      mxSetField(mxData,2,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt", "isFixedPointType" };

      mxArray *mxType = mxCreateStructMatrix(1,1,sizeof(typeFields)/sizeof
        (typeFields[0]),typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxType,0,"isFixedPointType",mxCreateDoubleScalar(0));
      mxSetField(mxData,2,"type",mxType);
    }

    mxSetField(mxData,2,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,0,mxREAL);
      double *pr = mxGetPr(mxSize);
      mxSetField(mxData,3,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt", "isFixedPointType" };

      mxArray *mxType = mxCreateStructMatrix(1,1,sizeof(typeFields)/sizeof
        (typeFields[0]),typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxType,0,"isFixedPointType",mxCreateDoubleScalar(0));
      mxSetField(mxData,3,"type",mxType);
    }

    mxSetField(mxData,3,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(628);
      pr[1] = (double)(2);
      mxSetField(mxData,4,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt", "isFixedPointType" };

      mxArray *mxType = mxCreateStructMatrix(1,1,sizeof(typeFields)/sizeof
        (typeFields[0]),typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxType,0,"isFixedPointType",mxCreateDoubleScalar(0));
      mxSetField(mxData,4,"type",mxType);
    }

    mxSetField(mxData,4,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(628);
      pr[1] = (double)(2);
      mxSetField(mxData,5,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt", "isFixedPointType" };

      mxArray *mxType = mxCreateStructMatrix(1,1,sizeof(typeFields)/sizeof
        (typeFields[0]),typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxType,0,"isFixedPointType",mxCreateDoubleScalar(0));
      mxSetField(mxData,5,"type",mxType);
    }

    mxSetField(mxData,5,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,0,mxREAL);
      double *pr = mxGetPr(mxSize);
      mxSetField(mxData,6,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt", "isFixedPointType" };

      mxArray *mxType = mxCreateStructMatrix(1,1,sizeof(typeFields)/sizeof
        (typeFields[0]),typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxType,0,"isFixedPointType",mxCreateDoubleScalar(0));
      mxSetField(mxData,6,"type",mxType);
    }

    mxSetField(mxData,6,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(2501);
      pr[1] = (double)(7);
      mxSetField(mxData,7,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt", "isFixedPointType" };

      mxArray *mxType = mxCreateStructMatrix(1,1,sizeof(typeFields)/sizeof
        (typeFields[0]),typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxType,0,"isFixedPointType",mxCreateDoubleScalar(0));
      mxSetField(mxData,7,"type",mxType);
    }

    mxSetField(mxData,7,"complexity",mxCreateDoubleScalar(0));
    mxSetField(mxAutoinheritanceInfo,0,"inputs",mxData);
  }

  {
    mxSetField(mxAutoinheritanceInfo,0,"parameters",mxCreateDoubleMatrix(0,0,
                mxREAL));
  }

  {
    const char *dataFields[] = { "size", "type", "complexity" };

    mxArray *mxData = mxCreateStructMatrix(1,1,3,dataFields);

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,1,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(6);
      mxSetField(mxData,0,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt", "isFixedPointType" };

      mxArray *mxType = mxCreateStructMatrix(1,1,sizeof(typeFields)/sizeof
        (typeFields[0]),typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxType,0,"isFixedPointType",mxCreateDoubleScalar(0));
      mxSetField(mxData,0,"type",mxType);
    }

    mxSetField(mxData,0,"complexity",mxCreateDoubleScalar(0));
    mxSetField(mxAutoinheritanceInfo,0,"outputs",mxData);
  }

  {
    mxSetField(mxAutoinheritanceInfo,0,"locals",mxCreateDoubleMatrix(0,0,mxREAL));
  }

  {
    mxArray* mxPostCodegenInfo = sf_c2_solver_simulink_get_post_codegen_info();
    mxSetField(mxAutoinheritanceInfo,0,"postCodegenInfo",mxPostCodegenInfo);
  }

  return(mxAutoinheritanceInfo);
}

mxArray *sf_c2_solver_simulink_third_party_uses_info(void)
{
  mxArray * mxcell3p = mxCreateCellMatrix(1,0);
  return(mxcell3p);
}

mxArray *sf_c2_solver_simulink_jit_fallback_info(void)
{
  const char *infoFields[] = { "fallbackType", "fallbackReason",
    "hiddenFallbackType", "hiddenFallbackReason", "incompatibleSymbol" };

  mxArray *mxInfo = mxCreateStructMatrix(1, 1, 5, infoFields);
  mxArray *fallbackType = mxCreateString("pre");
  mxArray *fallbackReason = mxCreateString("hasBreakpoints");
  mxArray *hiddenFallbackType = mxCreateString("none");
  mxArray *hiddenFallbackReason = mxCreateString("");
  mxArray *incompatibleSymbol = mxCreateString("");
  mxSetField(mxInfo, 0, infoFields[0], fallbackType);
  mxSetField(mxInfo, 0, infoFields[1], fallbackReason);
  mxSetField(mxInfo, 0, infoFields[2], hiddenFallbackType);
  mxSetField(mxInfo, 0, infoFields[3], hiddenFallbackReason);
  mxSetField(mxInfo, 0, infoFields[4], incompatibleSymbol);
  return mxInfo;
}

mxArray *sf_c2_solver_simulink_updateBuildInfo_args_info(void)
{
  mxArray *mxBIArgs = mxCreateCellMatrix(1,0);
  return mxBIArgs;
}

mxArray* sf_c2_solver_simulink_get_post_codegen_info(void)
{
  const char* fieldNames[] = { "exportedFunctionsUsedByThisChart",
    "exportedFunctionsChecksum" };

  mwSize dims[2] = { 1, 1 };

  mxArray* mxPostCodegenInfo = mxCreateStructArray(2, dims, sizeof(fieldNames)/
    sizeof(fieldNames[0]), fieldNames);

  {
    mxArray* mxExportedFunctionsChecksum = mxCreateString("");
    mwSize exp_dims[2] = { 0, 1 };

    mxArray* mxExportedFunctionsUsedByThisChart = mxCreateCellArray(2, exp_dims);
    mxSetField(mxPostCodegenInfo, 0, "exportedFunctionsUsedByThisChart",
               mxExportedFunctionsUsedByThisChart);
    mxSetField(mxPostCodegenInfo, 0, "exportedFunctionsChecksum",
               mxExportedFunctionsChecksum);
  }

  return mxPostCodegenInfo;
}

static const mxArray *sf_get_sim_state_info_c2_solver_simulink(void)
{
  const char *infoFields[] = { "chartChecksum", "varInfo" };

  mxArray *mxInfo = mxCreateStructMatrix(1, 1, 2, infoFields);
  const char *infoEncStr[] = {
    "100 S1x2'type','srcId','name','auxInfo'{{M[1],M[5],T\"statedot\",},{M[8],M[0],T\"is_active_c2_solver_simulink\",}}"
  };

  mxArray *mxVarInfo = sf_mex_decode_encoded_mx_struct_array(infoEncStr, 2, 10);
  mxArray *mxChecksum = mxCreateDoubleMatrix(1, 4, mxREAL);
  sf_c2_solver_simulink_get_check_sum(&mxChecksum);
  mxSetField(mxInfo, 0, infoFields[0], mxChecksum);
  mxSetField(mxInfo, 0, infoFields[1], mxVarInfo);
  return mxInfo;
}

static void chart_debug_initialization(SimStruct *S, unsigned int
  fullDebuggerInitialization)
{
  if (!sim_mode_is_rtw_gen(S)) {
    SFc2_solver_simulinkInstanceStruct *chartInstance =
      (SFc2_solver_simulinkInstanceStruct *)sf_get_chart_instance_ptr(S);
    if (ssIsFirstInitCond(S) && fullDebuggerInitialization==1) {
      /* do this only if simulation is starting */
      {
        unsigned int chartAlreadyPresent;
        chartAlreadyPresent = sf_debug_initialize_chart
          (sfGlobalDebugInstanceStruct,
           _solver_simulinkMachineNumber_,
           2,
           1,
           1,
           0,
           9,
           0,
           0,
           0,
           0,
           3,
           &chartInstance->chartNumber,
           &chartInstance->instanceNumber,
           (void *)S);

        /* Each instance must initialize its own list of scripts */
        init_script_number_translation(_solver_simulinkMachineNumber_,
          chartInstance->chartNumber,chartInstance->instanceNumber);
        if (chartAlreadyPresent==0) {
          /* this is the first instance */
          sf_debug_set_chart_disable_implicit_casting
            (sfGlobalDebugInstanceStruct,_solver_simulinkMachineNumber_,
             chartInstance->chartNumber,1);
          sf_debug_set_chart_event_thresholds(sfGlobalDebugInstanceStruct,
            _solver_simulinkMachineNumber_,
            chartInstance->chartNumber,
            0,
            0,
            0);
          _SFD_SET_DATA_PROPS(0,1,1,0,"t");
          _SFD_SET_DATA_PROPS(1,1,1,0,"t_step");
          _SFD_SET_DATA_PROPS(2,1,1,0,"state");
          _SFD_SET_DATA_PROPS(3,1,1,0,"rocketOD");
          _SFD_SET_DATA_PROPS(4,1,1,0,"T");
          _SFD_SET_DATA_PROPS(5,1,1,0,"mass");
          _SFD_SET_DATA_PROPS(6,1,1,0,"wind");
          _SFD_SET_DATA_PROPS(7,1,1,0,"aerodata");
          _SFD_SET_DATA_PROPS(8,2,0,1,"statedot");
          _SFD_STATE_INFO(0,0,2);
          _SFD_CH_SUBSTATE_COUNT(0);
          _SFD_CH_SUBSTATE_DECOMP(0);
        }

        _SFD_CV_INIT_CHART(0,0,0,0);

        {
          _SFD_CV_INIT_STATE(0,0,0,0,0,0,NULL,NULL);
        }

        _SFD_CV_INIT_TRANS(0,0,NULL,NULL,0,NULL);

        /* Initialization of MATLAB Function Model Coverage */
        _SFD_CV_INIT_EML(0,1,1,0,2,0,0,0,0,1,0,0);
        _SFD_CV_INIT_EML_FCN(0,0,"eML_blk_kernel",0,-1,1002);
        _SFD_CV_INIT_EML_IF(0,1,0,389,409,572,612);
        _SFD_CV_INIT_EML_IF(0,1,1,490,504,439,541);
        _SFD_CV_INIT_EML_WHILE(0,1,0,439,447,541);
        _SFD_CV_INIT_EML_RELATIONAL(0,1,0,392,409,-1,3);
        _SFD_CV_INIT_EML_RELATIONAL(0,1,1,493,504,-1,2);
        _SFD_CV_INIT_SCRIPT(0,1,0,4,0,0,0,0,1,0,0);
        _SFD_CV_INIT_SCRIPT_FCN(0,0,"forces",394,-1,1686);
        _SFD_CV_INIT_SCRIPT_IF(0,0,650,665,874,906);
        _SFD_CV_INIT_SCRIPT_IF(0,1,743,757,695,794);
        _SFD_CV_INIT_SCRIPT_IF(0,2,1353,1378,-1,1399);
        _SFD_CV_INIT_SCRIPT_IF(0,3,1457,1469,1605,1681);
        _SFD_CV_INIT_SCRIPT_WHILE(0,0,695,703,794);
        _SFD_CV_INIT_SCRIPT_RELATIONAL(0,0,653,665,-1,2);
        _SFD_CV_INIT_SCRIPT_RELATIONAL(0,1,746,757,-1,2);
        _SFD_CV_INIT_SCRIPT_RELATIONAL(0,2,1356,1378,-1,3);
        _SFD_CV_INIT_SCRIPT_RELATIONAL(0,3,1460,1469,-1,5);
        _SFD_CV_INIT_SCRIPT(1,1,0,6,0,0,0,0,1,2,1);
        _SFD_CV_INIT_SCRIPT_FCN(1,0,"aerodynamics",106,-1,2147);
        _SFD_CV_INIT_SCRIPT_IF(1,0,785,799,735,831);
        _SFD_CV_INIT_SCRIPT_IF(1,1,1140,1152,2003,2142);
        _SFD_CV_INIT_SCRIPT_IF(1,2,1162,1196,-1,1221);
        _SFD_CV_INIT_SCRIPT_IF(1,3,1342,1358,1519,1796);
        _SFD_CV_INIT_SCRIPT_IF(1,4,1519,1539,1704,1796);
        _SFD_CV_INIT_SCRIPT_IF(1,5,2003,2070,2102,2142);
        _SFD_CV_INIT_SCRIPT_WHILE(1,0,735,743,831);

        {
          static int condStart[] = { 2010, 2043 };

          static int condEnd[] = { 2039, 2070 };

          static int pfixExpr[] = { 0, 1, -3 };

          _SFD_CV_INIT_SCRIPT_MCDC(1,0,2010,2070,2,0,&(condStart[0]),&(condEnd[0]),
            3,&(pfixExpr[0]));
        }

        _SFD_CV_INIT_SCRIPT_RELATIONAL(1,0,788,799,-1,2);
        _SFD_CV_INIT_SCRIPT_RELATIONAL(1,1,1143,1152,-1,5);
        _SFD_CV_INIT_SCRIPT_RELATIONAL(1,2,1165,1196,-1,3);
        _SFD_CV_INIT_SCRIPT_RELATIONAL(1,3,1345,1358,-1,3);
        _SFD_CV_INIT_SCRIPT_RELATIONAL(1,4,1526,1539,-1,3);
        _SFD_CV_INIT_SCRIPT_RELATIONAL(1,5,2010,2039,-1,2);
        _SFD_CV_INIT_SCRIPT_RELATIONAL(1,6,2043,2070,-1,4);
        _SFD_CV_INIT_SCRIPT(2,1,0,7,0,0,0,0,0,0,0);
        _SFD_CV_INIT_SCRIPT_FCN(2,0,"getAtmoConditions",212,-1,1550);
        _SFD_CV_INIT_SCRIPT_IF(2,0,513,528,567,945);
        _SFD_CV_INIT_SCRIPT_IF(2,1,567,586,625,945);
        _SFD_CV_INIT_SCRIPT_IF(2,2,625,644,683,945);
        _SFD_CV_INIT_SCRIPT_IF(2,3,683,702,741,945);
        _SFD_CV_INIT_SCRIPT_IF(2,4,741,760,799,945);
        _SFD_CV_INIT_SCRIPT_IF(2,5,799,818,857,945);
        _SFD_CV_INIT_SCRIPT_IF(2,6,857,876,915,945);
        _SFD_CV_INIT_SCRIPT_RELATIONAL(2,0,516,528,-1,3);
        _SFD_CV_INIT_SCRIPT_RELATIONAL(2,1,574,586,-1,3);
        _SFD_CV_INIT_SCRIPT_RELATIONAL(2,2,632,644,-1,3);
        _SFD_CV_INIT_SCRIPT_RELATIONAL(2,3,690,702,-1,3);
        _SFD_CV_INIT_SCRIPT_RELATIONAL(2,4,748,760,-1,3);
        _SFD_CV_INIT_SCRIPT_RELATIONAL(2,5,806,818,-1,3);
        _SFD_CV_INIT_SCRIPT_RELATIONAL(2,6,864,876,-1,3);
        _SFD_SET_DATA_COMPILED_PROPS(0,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c2_c_sf_marshallOut,(MexInFcnForType)NULL);
        _SFD_SET_DATA_COMPILED_PROPS(1,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c2_c_sf_marshallOut,(MexInFcnForType)NULL);

        {
          unsigned int dimVector[1];
          dimVector[0]= 6U;
          _SFD_SET_DATA_COMPILED_PROPS(2,SF_DOUBLE,1,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c2_sf_marshallOut,(MexInFcnForType)NULL);
        }

        _SFD_SET_DATA_COMPILED_PROPS(3,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c2_c_sf_marshallOut,(MexInFcnForType)NULL);

        {
          unsigned int dimVector[2];
          dimVector[0]= 628U;
          dimVector[1]= 2U;
          _SFD_SET_DATA_COMPILED_PROPS(4,SF_DOUBLE,2,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c2_d_sf_marshallOut,(MexInFcnForType)NULL);
        }

        {
          unsigned int dimVector[2];
          dimVector[0]= 628U;
          dimVector[1]= 2U;
          _SFD_SET_DATA_COMPILED_PROPS(5,SF_DOUBLE,2,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c2_d_sf_marshallOut,(MexInFcnForType)NULL);
        }

        _SFD_SET_DATA_COMPILED_PROPS(6,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c2_c_sf_marshallOut,(MexInFcnForType)NULL);

        {
          unsigned int dimVector[2];
          dimVector[0]= 2501U;
          dimVector[1]= 7U;
          _SFD_SET_DATA_COMPILED_PROPS(7,SF_DOUBLE,2,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c2_b_sf_marshallOut,(MexInFcnForType)NULL);
        }

        {
          unsigned int dimVector[1];
          dimVector[0]= 6U;
          _SFD_SET_DATA_COMPILED_PROPS(8,SF_DOUBLE,1,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c2_sf_marshallOut,(MexInFcnForType)
            c2_sf_marshallIn);
        }
      }
    } else {
      sf_debug_reset_current_state_configuration(sfGlobalDebugInstanceStruct,
        _solver_simulinkMachineNumber_,chartInstance->chartNumber,
        chartInstance->instanceNumber);
    }
  }
}

static void chart_debug_initialize_data_addresses(SimStruct *S)
{
  if (!sim_mode_is_rtw_gen(S)) {
    SFc2_solver_simulinkInstanceStruct *chartInstance =
      (SFc2_solver_simulinkInstanceStruct *)sf_get_chart_instance_ptr(S);
    if (ssIsFirstInitCond(S)) {
      /* do this only if simulation is starting and after we know the addresses of all data */
      {
        _SFD_SET_DATA_VALUE_PTR(0U, chartInstance->c2_t);
        _SFD_SET_DATA_VALUE_PTR(1U, chartInstance->c2_t_step);
        _SFD_SET_DATA_VALUE_PTR(8U, *chartInstance->c2_statedot);
        _SFD_SET_DATA_VALUE_PTR(2U, *chartInstance->c2_state);
        _SFD_SET_DATA_VALUE_PTR(3U, chartInstance->c2_rocketOD);
        _SFD_SET_DATA_VALUE_PTR(4U, *chartInstance->c2_T);
        _SFD_SET_DATA_VALUE_PTR(5U, *chartInstance->c2_mass);
        _SFD_SET_DATA_VALUE_PTR(6U, chartInstance->c2_wind);
        _SFD_SET_DATA_VALUE_PTR(7U, *chartInstance->c2_c_aerodata);
      }
    }
  }
}

static const char* sf_get_instance_specialization(void)
{
  return "sU7IXcf24vdGiZeZbT4VPZH";
}

static void sf_opaque_initialize_c2_solver_simulink(void *chartInstanceVar)
{
  chart_debug_initialization(((SFc2_solver_simulinkInstanceStruct*)
    chartInstanceVar)->S,0);
  initialize_params_c2_solver_simulink((SFc2_solver_simulinkInstanceStruct*)
    chartInstanceVar);
  initialize_c2_solver_simulink((SFc2_solver_simulinkInstanceStruct*)
    chartInstanceVar);
}

static void sf_opaque_enable_c2_solver_simulink(void *chartInstanceVar)
{
  enable_c2_solver_simulink((SFc2_solver_simulinkInstanceStruct*)
    chartInstanceVar);
}

static void sf_opaque_disable_c2_solver_simulink(void *chartInstanceVar)
{
  disable_c2_solver_simulink((SFc2_solver_simulinkInstanceStruct*)
    chartInstanceVar);
}

static void sf_opaque_gateway_c2_solver_simulink(void *chartInstanceVar)
{
  sf_gateway_c2_solver_simulink((SFc2_solver_simulinkInstanceStruct*)
    chartInstanceVar);
}

static const mxArray* sf_opaque_get_sim_state_c2_solver_simulink(SimStruct* S)
{
  return get_sim_state_c2_solver_simulink((SFc2_solver_simulinkInstanceStruct *)
    sf_get_chart_instance_ptr(S));     /* raw sim ctx */
}

static void sf_opaque_set_sim_state_c2_solver_simulink(SimStruct* S, const
  mxArray *st)
{
  set_sim_state_c2_solver_simulink((SFc2_solver_simulinkInstanceStruct*)
    sf_get_chart_instance_ptr(S), st);
}

static void sf_opaque_terminate_c2_solver_simulink(void *chartInstanceVar)
{
  if (chartInstanceVar!=NULL) {
    SimStruct *S = ((SFc2_solver_simulinkInstanceStruct*) chartInstanceVar)->S;
    if (sim_mode_is_rtw_gen(S) || sim_mode_is_external(S)) {
      sf_clear_rtw_identifier(S);
      unload_solver_simulink_optimization_info();
    }

    finalize_c2_solver_simulink((SFc2_solver_simulinkInstanceStruct*)
      chartInstanceVar);
    utFree(chartInstanceVar);
    if (ssGetUserData(S)!= NULL) {
      sf_free_ChartRunTimeInfo(S);
    }

    ssSetUserData(S,NULL);
  }
}

static void sf_opaque_init_subchart_simstructs(void *chartInstanceVar)
{
  initSimStructsc2_solver_simulink((SFc2_solver_simulinkInstanceStruct*)
    chartInstanceVar);
}

extern unsigned int sf_machine_global_initializer_called(void);
static void mdlProcessParameters_c2_solver_simulink(SimStruct *S)
{
  int i;
  for (i=0;i<ssGetNumRunTimeParams(S);i++) {
    if (ssGetSFcnParamTunable(S,i)) {
      ssUpdateDlgParamAsRunTimeParam(S,i);
    }
  }

  if (sf_machine_global_initializer_called()) {
    initialize_params_c2_solver_simulink((SFc2_solver_simulinkInstanceStruct*)
      sf_get_chart_instance_ptr(S));
  }
}

static void mdlSetWorkWidths_c2_solver_simulink(SimStruct *S)
{
  /* Set overwritable ports for inplace optimization */
  ssSetStatesModifiedOnlyInUpdate(S, 1);
  ssMdlUpdateIsEmpty(S, 1);
  if (sim_mode_is_rtw_gen(S) || sim_mode_is_external(S)) {
    mxArray *infoStruct = load_solver_simulink_optimization_info
      (sim_mode_is_rtw_gen(S), sim_mode_is_modelref_sim(S), sim_mode_is_external
       (S));
    int_T chartIsInlinable =
      (int_T)sf_is_chart_inlinable(sf_get_instance_specialization(),infoStruct,2);
    ssSetStateflowIsInlinable(S,chartIsInlinable);
    ssSetRTWCG(S,1);
    ssSetEnableFcnIsTrivial(S,1);
    ssSetDisableFcnIsTrivial(S,1);
    ssSetNotMultipleInlinable(S,sf_rtw_info_uint_prop
      (sf_get_instance_specialization(),infoStruct,2,
       "gatewayCannotBeInlinedMultipleTimes"));
    sf_set_chart_accesses_machine_info(S, sf_get_instance_specialization(),
      infoStruct, 2);
    sf_update_buildInfo(S, sf_get_instance_specialization(),infoStruct,2);
    if (chartIsInlinable) {
      ssSetInputPortOptimOpts(S, 0, SS_REUSABLE_AND_LOCAL);
      ssSetInputPortOptimOpts(S, 1, SS_REUSABLE_AND_LOCAL);
      ssSetInputPortOptimOpts(S, 2, SS_REUSABLE_AND_LOCAL);
      ssSetInputPortOptimOpts(S, 3, SS_REUSABLE_AND_LOCAL);
      ssSetInputPortOptimOpts(S, 4, SS_REUSABLE_AND_LOCAL);
      ssSetInputPortOptimOpts(S, 5, SS_REUSABLE_AND_LOCAL);
      ssSetInputPortOptimOpts(S, 6, SS_REUSABLE_AND_LOCAL);
      ssSetInputPortOptimOpts(S, 7, SS_REUSABLE_AND_LOCAL);
      sf_mark_chart_expressionable_inputs(S,sf_get_instance_specialization(),
        infoStruct,2,8);
      sf_mark_chart_reusable_outputs(S,sf_get_instance_specialization(),
        infoStruct,2,1);
    }

    {
      unsigned int outPortIdx;
      for (outPortIdx=1; outPortIdx<=1; ++outPortIdx) {
        ssSetOutputPortOptimizeInIR(S, outPortIdx, 1U);
      }
    }

    {
      unsigned int inPortIdx;
      for (inPortIdx=0; inPortIdx < 8; ++inPortIdx) {
        ssSetInputPortOptimizeInIR(S, inPortIdx, 1U);
      }
    }

    sf_set_rtw_dwork_info(S,sf_get_instance_specialization(),infoStruct,2);
    sf_register_codegen_names_for_scoped_functions_defined_by_chart(S);
    ssSetHasSubFunctions(S,!(chartIsInlinable));
  } else {
  }

  ssSetOptions(S,ssGetOptions(S)|SS_OPTION_WORKS_WITH_CODE_REUSE);
  ssSetChecksum0(S,(158728526U));
  ssSetChecksum1(S,(152571399U));
  ssSetChecksum2(S,(268177138U));
  ssSetChecksum3(S,(3751163718U));
  ssSetmdlDerivatives(S, NULL);
  ssSetExplicitFCSSCtrl(S,1);
  ssSetStateSemanticsClassicAndSynchronous(S, true);
  ssSupportsMultipleExecInstances(S,1);
}

static void mdlRTW_c2_solver_simulink(SimStruct *S)
{
  if (sim_mode_is_rtw_gen(S)) {
    ssWriteRTWStrParam(S, "StateflowChartType", "Embedded MATLAB");
  }
}

static void mdlStart_c2_solver_simulink(SimStruct *S)
{
  SFc2_solver_simulinkInstanceStruct *chartInstance;
  chartInstance = (SFc2_solver_simulinkInstanceStruct *)utMalloc(sizeof
    (SFc2_solver_simulinkInstanceStruct));
  if (chartInstance==NULL) {
    sf_mex_error_message("Could not allocate memory for chart instance.");
  }

  memset(chartInstance, 0, sizeof(SFc2_solver_simulinkInstanceStruct));
  chartInstance->chartInfo.chartInstance = chartInstance;
  chartInstance->chartInfo.isEMLChart = 1;
  chartInstance->chartInfo.chartInitialized = 0;
  chartInstance->chartInfo.sFunctionGateway =
    sf_opaque_gateway_c2_solver_simulink;
  chartInstance->chartInfo.initializeChart =
    sf_opaque_initialize_c2_solver_simulink;
  chartInstance->chartInfo.terminateChart =
    sf_opaque_terminate_c2_solver_simulink;
  chartInstance->chartInfo.enableChart = sf_opaque_enable_c2_solver_simulink;
  chartInstance->chartInfo.disableChart = sf_opaque_disable_c2_solver_simulink;
  chartInstance->chartInfo.getSimState =
    sf_opaque_get_sim_state_c2_solver_simulink;
  chartInstance->chartInfo.setSimState =
    sf_opaque_set_sim_state_c2_solver_simulink;
  chartInstance->chartInfo.getSimStateInfo =
    sf_get_sim_state_info_c2_solver_simulink;
  chartInstance->chartInfo.zeroCrossings = NULL;
  chartInstance->chartInfo.outputs = NULL;
  chartInstance->chartInfo.derivatives = NULL;
  chartInstance->chartInfo.mdlRTW = mdlRTW_c2_solver_simulink;
  chartInstance->chartInfo.mdlStart = mdlStart_c2_solver_simulink;
  chartInstance->chartInfo.mdlSetWorkWidths =
    mdlSetWorkWidths_c2_solver_simulink;
  chartInstance->chartInfo.callGetHoverDataForMsg = NULL;
  chartInstance->chartInfo.extModeExec = NULL;
  chartInstance->chartInfo.restoreLastMajorStepConfiguration = NULL;
  chartInstance->chartInfo.restoreBeforeLastMajorStepConfiguration = NULL;
  chartInstance->chartInfo.storeCurrentConfiguration = NULL;
  chartInstance->chartInfo.callAtomicSubchartUserFcn = NULL;
  chartInstance->chartInfo.callAtomicSubchartAutoFcn = NULL;
  chartInstance->chartInfo.debugInstance = sfGlobalDebugInstanceStruct;
  chartInstance->S = S;
  sf_init_ChartRunTimeInfo(S, &(chartInstance->chartInfo), false, 0);
  init_dsm_address_info(chartInstance);
  init_simulink_io_address(chartInstance);
  if (!sim_mode_is_rtw_gen(S)) {
  }

  chart_debug_initialization(S,1);
  mdl_start_c2_solver_simulink(chartInstance);
}

void c2_solver_simulink_method_dispatcher(SimStruct *S, int_T method, void *data)
{
  switch (method) {
   case SS_CALL_MDL_START:
    mdlStart_c2_solver_simulink(S);
    break;

   case SS_CALL_MDL_SET_WORK_WIDTHS:
    mdlSetWorkWidths_c2_solver_simulink(S);
    break;

   case SS_CALL_MDL_PROCESS_PARAMETERS:
    mdlProcessParameters_c2_solver_simulink(S);
    break;

   default:
    /* Unhandled method */
    sf_mex_error_message("Stateflow Internal Error:\n"
                         "Error calling c2_solver_simulink_method_dispatcher.\n"
                         "Can't handle method %d.\n", method);
    break;
  }
}
