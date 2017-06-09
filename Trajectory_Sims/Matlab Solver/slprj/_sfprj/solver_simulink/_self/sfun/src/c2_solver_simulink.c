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
static const char * c2_debug_family_names[34] = { "workspace", "rocket", "k",
  "tol", "delta", "current_mass", "I", "f_x", "f_y", "moment", "drag", "lift",
  "CD", "CL", "aoa", "W", "CP", "a_x", "a_y", "alpha", "nargin", "nargout", "t",
  "state", "t_step", "T", "mass", "wind", "aerodata", "CM", "site_elevation",
  "T0", "forceout", "statedot" };

static const char * c2_b_debug_family_names[21] = { "mu", "T0", "Trate", "hpos",
  "g", "avogadro", "k_boltz", "m_air", "R_air", "P_sl", "k", "rho_sl", "nargin",
  "nargout", "h", "h0", "Tlaunch", "T", "P", "rho", "a" };

static const char * c2_c_debug_family_names[36] = { "ux", "uy", "umag", "phi",
  "resultant", "lambda", "rho", "local_c", "mu", "mach", "k", "tol", "delta",
  "ft2m", "rocket_length", "Re_rocket_y", "rail_length", "launch_site", "Sdrag",
  "Slift", "weight", "nargin", "nargout", "r", "u", "wind", "aerodata", "rocket",
  "site_elevation", "T0", "Fdrag", "Flift", "CD", "CL", "aoa", "CP" };

static const char * c2_d_debug_family_names[39] = { "ux", "theta", "k", "tol",
  "delta", "thrust", "Tx", "Ty", "Lx", "Ly", "Dx", "Dy", "G", "r_earth", "dir",
  "nargin", "nargout", "t", "t_step", "r", "u", "T", "current_mass", "wind",
  "aerodata", "rocket", "CM", "site_elevation", "T0", "f_x", "f_y", "moment",
  "Fdrag", "Flift", "CD", "CL", "aoa", "gravity", "CP" };

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
  real_T c2_r[3], real_T c2_b_u[3], real_T c2_b_wind, real_T c2_e_aerodata[25010],
  c2_sv2voxIt8FKzjMlUEnyXmnC *c2_rocket, real_T c2_b_site_elevation, real_T
  c2_b_T0, real_T *c2_Fdrag, real_T *c2_Flift, real_T *c2_CD, real_T *c2_CL,
  real_T *c2_aoa, real_T *c2_CP);
static void c2_getAtmoConditions(SFc2_solver_simulinkInstanceStruct
  *chartInstance, real_T c2_h, real_T c2_h0, real_T c2_Tlaunch, real_T *c2_b_T,
  real_T *c2_P, real_T *c2_rho, real_T *c2_a);
static void init_script_number_translation(uint32_T c2_machineNumber, uint32_T
  c2_chartNumber, uint32_T c2_instanceNumber);
static void c2_emlrt_marshallIn(SFc2_solver_simulinkInstanceStruct
  *chartInstance, const mxArray *c2_load, const char_T *c2_identifier,
  c2_sOKxARu9Dkm6wShDKKOBVjE *c2_b_y);
static void c2_b_emlrt_marshallIn(SFc2_solver_simulinkInstanceStruct
  *chartInstance, const mxArray *c2_b_u, const emlrtMsgIdentifier *c2_parentId,
  c2_sOKxARu9Dkm6wShDKKOBVjE *c2_b_y);
static void c2_c_emlrt_marshallIn(SFc2_solver_simulinkInstanceStruct
  *chartInstance, const mxArray *c2_b_u, const emlrtMsgIdentifier *c2_parentId,
  c2_sv2voxIt8FKzjMlUEnyXmnC *c2_b_y);
static real_T c2_d_emlrt_marshallIn(SFc2_solver_simulinkInstanceStruct
  *chartInstance, const mxArray *c2_b_u, const emlrtMsgIdentifier *c2_parentId);
static c2_sof5eNcdcizKAe6jjm9xhtG c2_e_emlrt_marshallIn
  (SFc2_solver_simulinkInstanceStruct *chartInstance, const mxArray *c2_b_u,
   const emlrtMsgIdentifier *c2_parentId);
static void c2_f_emlrt_marshallIn(SFc2_solver_simulinkInstanceStruct
  *chartInstance, const mxArray *c2_b_u, const emlrtMsgIdentifier *c2_parentId,
  char_T c2_b_y[9]);
static c2_syx7FYmGk3I6e3U1RUwUAWD c2_g_emlrt_marshallIn
  (SFc2_solver_simulinkInstanceStruct *chartInstance, const mxArray *c2_b_u,
   const emlrtMsgIdentifier *c2_parentId);
static c2_sqGsSc4djWypCV90lhUGiL c2_h_emlrt_marshallIn
  (SFc2_solver_simulinkInstanceStruct *chartInstance, const mxArray *c2_b_u,
   const emlrtMsgIdentifier *c2_parentId);
static c2_su2bTaGhbTlLQVXsqMBjEuH c2_i_emlrt_marshallIn
  (SFc2_solver_simulinkInstanceStruct *chartInstance, const mxArray *c2_b_u,
   const emlrtMsgIdentifier *c2_parentId);
static c2_sNlKjRKFHwjFKC34GeUJivG c2_j_emlrt_marshallIn
  (SFc2_solver_simulinkInstanceStruct *chartInstance, const mxArray *c2_b_u,
   const emlrtMsgIdentifier *c2_parentId);
static c2_sKS3cuSxR3HIcTIZZkbRPm c2_k_emlrt_marshallIn
  (SFc2_solver_simulinkInstanceStruct *chartInstance, const mxArray *c2_b_u,
   const emlrtMsgIdentifier *c2_parentId);
static c2_s88seaxzVTYYZIzTGqmP4AD c2_l_emlrt_marshallIn
  (SFc2_solver_simulinkInstanceStruct *chartInstance, const mxArray *c2_b_u,
   const emlrtMsgIdentifier *c2_parentId);
static c2_sfpFxKVMN8elArLoV353vAE c2_m_emlrt_marshallIn
  (SFc2_solver_simulinkInstanceStruct *chartInstance, const mxArray *c2_b_u,
   const emlrtMsgIdentifier *c2_parentId);
static const mxArray *c2_sf_marshallOut(void *chartInstanceVoid, void *c2_inData);
static void c2_n_emlrt_marshallIn(SFc2_solver_simulinkInstanceStruct
  *chartInstance, const mxArray *c2_b_statedot, const char_T *c2_identifier,
  real_T c2_b_y[6]);
static void c2_o_emlrt_marshallIn(SFc2_solver_simulinkInstanceStruct
  *chartInstance, const mxArray *c2_b_u, const emlrtMsgIdentifier *c2_parentId,
  real_T c2_b_y[6]);
static void c2_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c2_mxArrayInData, const char_T *c2_varName, void *c2_outData);
static const mxArray *c2_b_sf_marshallOut(void *chartInstanceVoid, void
  *c2_inData);
static void c2_p_emlrt_marshallIn(SFc2_solver_simulinkInstanceStruct
  *chartInstance, const mxArray *c2_b_forceout, const char_T *c2_identifier,
  real_T c2_b_y[10]);
static void c2_q_emlrt_marshallIn(SFc2_solver_simulinkInstanceStruct
  *chartInstance, const mxArray *c2_b_u, const emlrtMsgIdentifier *c2_parentId,
  real_T c2_b_y[10]);
static void c2_b_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c2_mxArrayInData, const char_T *c2_varName, void *c2_outData);
static const mxArray *c2_c_sf_marshallOut(void *chartInstanceVoid, void
  *c2_inData);
static const mxArray *c2_d_sf_marshallOut(void *chartInstanceVoid, void
  *c2_inData);
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
static void c2_e_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c2_mxArrayInData, const char_T *c2_varName, void *c2_outData);
static const mxArray *c2_h_sf_marshallOut(void *chartInstanceVoid, void
  *c2_inData);
static void c2_r_emlrt_marshallIn(SFc2_solver_simulinkInstanceStruct
  *chartInstance, const mxArray *c2_b_u, const emlrtMsgIdentifier *c2_parentId,
  real_T c2_b_y[7]);
static void c2_f_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c2_mxArrayInData, const char_T *c2_varName, void *c2_outData);
static const mxArray *c2_i_sf_marshallOut(void *chartInstanceVoid, void
  *c2_inData);
static void c2_s_emlrt_marshallIn(SFc2_solver_simulinkInstanceStruct
  *chartInstance, const mxArray *c2_b_u, const emlrtMsgIdentifier *c2_parentId,
  real_T c2_b_y[8]);
static void c2_g_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c2_mxArrayInData, const char_T *c2_varName, void *c2_outData);
static void c2_t_emlrt_marshallIn(SFc2_solver_simulinkInstanceStruct
  *chartInstance, const mxArray *c2_b_u, const emlrtMsgIdentifier *c2_parentId,
  real_T c2_b_y[25010]);
static void c2_h_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c2_mxArrayInData, const char_T *c2_varName, void *c2_outData);
static const mxArray *c2_j_sf_marshallOut(void *chartInstanceVoid, void
  *c2_inData);
static void c2_u_emlrt_marshallIn(SFc2_solver_simulinkInstanceStruct
  *chartInstance, const mxArray *c2_b_u, const emlrtMsgIdentifier *c2_parentId,
  real_T c2_b_y[3]);
static void c2_i_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c2_mxArrayInData, const char_T *c2_varName, void *c2_outData);
static void c2_v_emlrt_marshallIn(SFc2_solver_simulinkInstanceStruct
  *chartInstance, const mxArray *c2_b_u, const emlrtMsgIdentifier *c2_parentId,
  real_T c2_b_y[1256]);
static void c2_j_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c2_mxArrayInData, const char_T *c2_varName, void *c2_outData);
static void c2_error(SFc2_solver_simulinkInstanceStruct *chartInstance);
static real_T c2_sind(SFc2_solver_simulinkInstanceStruct *chartInstance, real_T
                      c2_x);
static real_T c2_cosd(SFc2_solver_simulinkInstanceStruct *chartInstance, real_T
                      c2_x);
static real_T c2_norm(SFc2_solver_simulinkInstanceStruct *chartInstance, real_T
                      c2_x[2]);
static real_T c2_asind(SFc2_solver_simulinkInstanceStruct *chartInstance, real_T
  c2_x);
static void c2_b_error(SFc2_solver_simulinkInstanceStruct *chartInstance);
static void c2_c_error(SFc2_solver_simulinkInstanceStruct *chartInstance);
static const mxArray *c2_k_sf_marshallOut(void *chartInstanceVoid, void
  *c2_inData);
static int32_T c2_w_emlrt_marshallIn(SFc2_solver_simulinkInstanceStruct
  *chartInstance, const mxArray *c2_b_u, const emlrtMsgIdentifier *c2_parentId);
static void c2_k_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c2_mxArrayInData, const char_T *c2_varName, void *c2_outData);
static uint8_T c2_x_emlrt_marshallIn(SFc2_solver_simulinkInstanceStruct
  *chartInstance, const mxArray *c2_b_is_active_c2_solver_simulink, const char_T
  *c2_identifier);
static uint8_T c2_y_emlrt_marshallIn(SFc2_solver_simulinkInstanceStruct
  *chartInstance, const mxArray *c2_b_u, const emlrtMsgIdentifier *c2_parentId);
static void c2_b_sind(SFc2_solver_simulinkInstanceStruct *chartInstance, real_T *
                      c2_x);
static void c2_b_cosd(SFc2_solver_simulinkInstanceStruct *chartInstance, real_T *
                      c2_x);
static void c2_b_asind(SFc2_solver_simulinkInstanceStruct *chartInstance, real_T
  *c2_x);
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
  const mxArray *c2_d_y = NULL;
  uint8_T c2_hoistedGlobal;
  const mxArray *c2_e_y = NULL;
  c2_st = NULL;
  c2_st = NULL;
  c2_b_y = NULL;
  sf_mex_assign(&c2_b_y, sf_mex_createcellmatrix(3, 1), false);
  c2_c_y = NULL;
  sf_mex_assign(&c2_c_y, sf_mex_create("y", *chartInstance->c2_forceout, 0, 0U,
    1U, 0U, 2, 1, 10), false);
  sf_mex_setcell(c2_b_y, 0, c2_c_y);
  c2_d_y = NULL;
  sf_mex_assign(&c2_d_y, sf_mex_create("y", *chartInstance->c2_statedot, 0, 0U,
    1U, 0U, 1, 6), false);
  sf_mex_setcell(c2_b_y, 1, c2_d_y);
  c2_hoistedGlobal = chartInstance->c2_is_active_c2_solver_simulink;
  c2_e_y = NULL;
  sf_mex_assign(&c2_e_y, sf_mex_create("y", &c2_hoistedGlobal, 3, 0U, 0U, 0U, 0),
                false);
  sf_mex_setcell(c2_b_y, 2, c2_e_y);
  sf_mex_assign(&c2_st, c2_b_y, false);
  return c2_st;
}

static void set_sim_state_c2_solver_simulink(SFc2_solver_simulinkInstanceStruct *
  chartInstance, const mxArray *c2_st)
{
  const mxArray *c2_b_u;
  real_T c2_dv0[10];
  int32_T c2_i0;
  real_T c2_dv1[6];
  int32_T c2_i1;
  chartInstance->c2_doneDoubleBufferReInit = true;
  c2_b_u = sf_mex_dup(c2_st);
  c2_p_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getcell("forceout",
    c2_b_u, 0)), "forceout", c2_dv0);
  for (c2_i0 = 0; c2_i0 < 10; c2_i0++) {
    (*chartInstance->c2_forceout)[c2_i0] = c2_dv0[c2_i0];
  }

  c2_n_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getcell("statedot",
    c2_b_u, 1)), "statedot", c2_dv1);
  for (c2_i1 = 0; c2_i1 < 6; c2_i1++) {
    (*chartInstance->c2_statedot)[c2_i1] = c2_dv1[c2_i1];
  }

  chartInstance->c2_is_active_c2_solver_simulink = c2_x_emlrt_marshallIn
    (chartInstance, sf_mex_dup(sf_mex_getcell("is_active_c2_solver_simulink",
       c2_b_u, 2)), "is_active_c2_solver_simulink");
  sf_mex_destroy(&c2_b_u);
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
  int32_T c2_i2;
  int32_T c2_i3;
  int32_T c2_i4;
  int32_T c2_i5;
  int32_T c2_i6;
  int32_T c2_i7;
  _SFD_SYMBOL_SCOPE_PUSH(0U, 0U);
  _sfTime_ = sf_get_time(chartInstance->S);
  _SFD_CC_CALL(CHART_ENTER_SFUNCTION_TAG, 0U, chartInstance->c2_sfEvent);
  _SFD_DATA_RANGE_CHECK(*chartInstance->c2_T0, 9U, 1U, 0U,
                        chartInstance->c2_sfEvent, false);
  _SFD_DATA_RANGE_CHECK(*chartInstance->c2_site_elevation, 8U, 1U, 0U,
                        chartInstance->c2_sfEvent, false);
  _SFD_DATA_RANGE_CHECK(*chartInstance->c2_CM, 7U, 1U, 0U,
                        chartInstance->c2_sfEvent, false);
  for (c2_i2 = 0; c2_i2 < 25010; c2_i2++) {
    _SFD_DATA_RANGE_CHECK((*chartInstance->c2_d_aerodata)[c2_i2], 6U, 1U, 0U,
                          chartInstance->c2_sfEvent, false);
  }

  _SFD_DATA_RANGE_CHECK(*chartInstance->c2_wind, 5U, 1U, 0U,
                        chartInstance->c2_sfEvent, false);
  for (c2_i3 = 0; c2_i3 < 1256; c2_i3++) {
    _SFD_DATA_RANGE_CHECK((*chartInstance->c2_mass)[c2_i3], 4U, 1U, 0U,
                          chartInstance->c2_sfEvent, false);
  }

  for (c2_i4 = 0; c2_i4 < 1256; c2_i4++) {
    _SFD_DATA_RANGE_CHECK((*chartInstance->c2_T)[c2_i4], 3U, 1U, 0U,
                          chartInstance->c2_sfEvent, false);
  }

  _SFD_DATA_RANGE_CHECK(*chartInstance->c2_t_step, 2U, 1U, 0U,
                        chartInstance->c2_sfEvent, false);
  for (c2_i5 = 0; c2_i5 < 6; c2_i5++) {
    _SFD_DATA_RANGE_CHECK((*chartInstance->c2_state)[c2_i5], 1U, 1U, 0U,
                          chartInstance->c2_sfEvent, false);
  }

  _SFD_DATA_RANGE_CHECK(*chartInstance->c2_t, 0U, 1U, 0U,
                        chartInstance->c2_sfEvent, false);
  chartInstance->c2_sfEvent = CALL_EVENT;
  c2_chartstep_c2_solver_simulink(chartInstance);
  _SFD_SYMBOL_SCOPE_POP();
  _SFD_CHECK_FOR_STATE_INCONSISTENCY(_solver_simulinkMachineNumber_,
    chartInstance->chartNumber, chartInstance->instanceNumber);
  for (c2_i6 = 0; c2_i6 < 10; c2_i6++) {
    _SFD_DATA_RANGE_CHECK((*chartInstance->c2_forceout)[c2_i6], 10U, 1U, 0U,
                          chartInstance->c2_sfEvent, false);
  }

  for (c2_i7 = 0; c2_i7 < 6; c2_i7++) {
    _SFD_DATA_RANGE_CHECK((*chartInstance->c2_statedot)[c2_i7], 11U, 1U, 0U,
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
  real_T c2_e_hoistedGlobal;
  real_T c2_f_hoistedGlobal;
  real_T c2_b_t;
  int32_T c2_i8;
  real_T c2_b_t_step;
  real_T c2_b_state[6];
  int32_T c2_i9;
  int32_T c2_i10;
  real_T c2_b_T[1256];
  real_T c2_b_wind;
  real_T c2_b_mass[1256];
  int32_T c2_i11;
  real_T c2_b_CM;
  real_T c2_b_site_elevation;
  real_T c2_b_T0;
  uint32_T c2_debug_family_var_map[34];
  c2_sOKxARu9Dkm6wShDKKOBVjE c2_workspace;
  c2_sv2voxIt8FKzjMlUEnyXmnC c2_rocket;
  real_T c2_k;
  real_T c2_tol;
  real_T c2_delta;
  real_T c2_current_mass;
  real_T c2_I;
  real_T c2_f_x;
  real_T c2_f_y;
  real_T c2_moment;
  real_T c2_drag;
  real_T c2_lift;
  real_T c2_CD;
  real_T c2_CL;
  real_T c2_aoa;
  real_T c2_W;
  real_T c2_CP;
  real_T c2_a_x;
  real_T c2_a_y;
  real_T c2_alpha;
  real_T c2_nargin = 10.0;
  real_T c2_nargout = 2.0;
  real_T c2_b_forceout[10];
  real_T c2_b_statedot[6];
  const mxArray *c2_b_y = NULL;
  c2_sOKxARu9Dkm6wShDKKOBVjE c2_r0;
  int32_T c2_i12;
  int32_T c2_i13;
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
  int32_T c2_i14;
  int32_T c2_i15;
  real_T c2_r[3];
  int32_T c2_i16;
  real_T c2_b_u[3];
  real_T c2_b_current_mass;
  real_T c2_c_T[1256];
  real_T c2_c_wind;
  int32_T c2_i17;
  c2_sv2voxIt8FKzjMlUEnyXmnC c2_b_rocket;
  real_T c2_c_CM;
  real_T c2_c_site_elevation;
  real_T c2_c_T0;
  uint32_T c2_b_debug_family_var_map[39];
  real_T c2_ux;
  real_T c2_theta;
  real_T c2_b_k;
  real_T c2_b_tol;
  real_T c2_b_delta;
  real_T c2_thrust;
  real_T c2_Tx;
  real_T c2_Ty;
  real_T c2_Lx;
  real_T c2_Ly;
  real_T c2_Dx;
  real_T c2_Dy;
  real_T c2_G;
  real_T c2_r_earth;
  real_T c2_dir;
  real_T c2_b_nargin = 12.0;
  real_T c2_b_nargout = 10.0;
  real_T c2_b_f_x;
  real_T c2_b_f_y;
  real_T c2_b_moment;
  real_T c2_b_drag;
  real_T c2_b_lift;
  real_T c2_b_CD;
  real_T c2_b_CL;
  real_T c2_b_aoa;
  real_T c2_b_W;
  real_T c2_b_CP;
  int32_T c2_i18;
  real_T c2_h_x;
  int32_T c2_i19;
  real_T c2_b_r[3];
  real_T c2_i_x;
  real_T c2_j_x;
  int32_T c2_i20;
  real_T c2_c_u[3];
  c2_sv2voxIt8FKzjMlUEnyXmnC c2_c_rocket;
  real_T c2_Fdrag;
  real_T c2_Flift;
  real_T c2_c_CD;
  real_T c2_c_CL;
  real_T c2_c_aoa;
  real_T c2_c_CP;
  real_T c2_d0;
  real_T c2_d1;
  real_T c2_d2;
  real_T c2_d3;
  real_T c2_d4;
  real_T c2_d5;
  int32_T c2_i21;
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
  int32_T c2_i22;
  int32_T c2_i23;
  int32_T exitg1;
  int32_T exitg2;
  _SFD_CC_CALL(CHART_ENTER_DURING_FUNCTION_TAG, 0U, chartInstance->c2_sfEvent);
  c2_hoistedGlobal = *chartInstance->c2_t;
  c2_b_hoistedGlobal = *chartInstance->c2_t_step;
  c2_c_hoistedGlobal = *chartInstance->c2_wind;
  c2_d_hoistedGlobal = *chartInstance->c2_CM;
  c2_e_hoistedGlobal = *chartInstance->c2_site_elevation;
  c2_f_hoistedGlobal = *chartInstance->c2_T0;
  c2_b_t = c2_hoistedGlobal;
  for (c2_i8 = 0; c2_i8 < 6; c2_i8++) {
    c2_b_state[c2_i8] = (*chartInstance->c2_state)[c2_i8];
  }

  c2_b_t_step = c2_b_hoistedGlobal;
  for (c2_i9 = 0; c2_i9 < 1256; c2_i9++) {
    c2_b_T[c2_i9] = (*chartInstance->c2_T)[c2_i9];
  }

  for (c2_i10 = 0; c2_i10 < 1256; c2_i10++) {
    c2_b_mass[c2_i10] = (*chartInstance->c2_mass)[c2_i10];
  }

  c2_b_wind = c2_c_hoistedGlobal;
  for (c2_i11 = 0; c2_i11 < 25010; c2_i11++) {
    chartInstance->c2_aerodata[c2_i11] = (*chartInstance->c2_d_aerodata)[c2_i11];
  }

  c2_b_CM = c2_d_hoistedGlobal;
  c2_b_site_elevation = c2_e_hoistedGlobal;
  c2_b_T0 = c2_f_hoistedGlobal;
  _SFD_SYMBOL_SCOPE_PUSH_EML(0U, 34U, 34U, c2_debug_family_names,
    c2_debug_family_var_map);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_workspace, 0U, c2_g_sf_marshallOut,
    c2_e_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_rocket, 1U, c2_f_sf_marshallOut,
    c2_d_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_k, 2U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_tol, 3U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_delta, 4U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_current_mass, 5U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_I, 6U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_f_x, 7U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_f_y, 8U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_moment, 9U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_drag, 10U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_lift, 11U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_CD, 12U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_CL, 13U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_aoa, 14U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_W, 15U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_CP, 16U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_a_x, 17U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_a_y, 18U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_alpha, 19U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_nargin, 20U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_nargout, 21U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c2_b_t, 22U, c2_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(c2_b_state, 23U, c2_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c2_b_t_step, 24U, c2_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(c2_b_T, 25U, c2_e_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(c2_b_mass, 26U, c2_e_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c2_b_wind, 27U, c2_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(chartInstance->c2_aerodata, 28U, c2_d_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c2_b_CM, 29U, c2_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c2_b_site_elevation, 30U, c2_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c2_b_T0, 31U, c2_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c2_b_forceout, 32U, c2_b_sf_marshallOut,
    c2_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c2_b_statedot, 33U, c2_sf_marshallOut,
    c2_sf_marshallIn);
  CV_EML_FCN(0, 0);
  _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 5);
  c2_b_y = NULL;
  sf_mex_assign(&c2_b_y, sf_mex_create("y", "import2simulink.mat", 15, 0U, 0U,
    0U, 2, 1, strlen("import2simulink.mat")), false);
  c2_emlrt_marshallIn(chartInstance, sf_mex_call_debug
                      (sfGlobalDebugInstanceStruct, "load", 1U, 1U, 14, c2_b_y),
                      "load", &c2_r0);
  c2_workspace = c2_r0;
  _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 6);
  c2_rocket = c2_workspace.rocket;
  _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 9);
  for (c2_i12 = 0; c2_i12 < 6; c2_i12++) {
    c2_b_statedot[c2_i12] = 0.0;
  }

  _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 10);
  for (c2_i13 = 0; c2_i13 < 3; c2_i13++) {
    c2_b_statedot[c2_i13] = c2_b_state[c2_i13 + 3];
  }

  _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 13);
  if (CV_EML_IF(0, 1, 0, CV_RELATIONAL_EVAL(4U, 0U, 0, c2_b_t, c2_b_mass[627],
        -1, 3U, c2_b_t <= c2_b_mass[627]))) {
    _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 14);
    c2_k = 1.0;
    _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 14);
    c2_tol = c2_b_t_step;
    _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 15);
    do {
      exitg2 = 0;
      CV_EML_WHILE(0, 1, 0, true);
      _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 16);
      c2_b_x = c2_b_mass[sf_eml_array_bounds_check(sfGlobalDebugInstanceStruct,
        chartInstance->S, 1U, 508, 1, MAX_uint32_T, (int32_T)sf_integer_check
        (chartInstance->S, 1U, 508U, 1U, c2_k), 1, 628) - 1] - c2_b_t;
      c2_c_x = c2_b_x;
      c2_d_x = c2_c_x;
      c2_delta = muDoubleScalarAbs(c2_d_x);
      _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 17);
      if (CV_EML_IF(0, 1, 1, CV_RELATIONAL_EVAL(4U, 0U, 1, c2_delta, c2_tol, -1,
            2U, c2_delta < c2_tol))) {
        exitg2 = 1;
      } else {
        _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 18);
        c2_k++;
        _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 15);
        _SF_MEX_LISTEN_FOR_CTRL_C(chartInstance->S);
      }
    } while (exitg2 == 0);

    _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 17);
    _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 20);
    c2_current_mass = c2_b_mass[sf_eml_array_bounds_check
      (sfGlobalDebugInstanceStruct, chartInstance->S, 1U, 601, 1, MAX_uint32_T,
       (int32_T)sf_integer_check(chartInstance->S, 1U, 601U, 1U, c2_k), 1, 628)
      + 627];
  } else {
    _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 22);
    c2_current_mass = c2_b_mass[1255];
  }

  _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 25);
  c2_a = c2_rocket.OD;
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
  _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 28);
  c2_c_t = c2_b_t;
  c2_c_t_step = c2_b_t_step;
  for (c2_i14 = 0; c2_i14 < 3; c2_i14++) {
    c2_r[c2_i14] = c2_b_state[c2_i14];
  }

  for (c2_i15 = 0; c2_i15 < 3; c2_i15++) {
    c2_b_u[c2_i15] = c2_b_state[c2_i15 + 3];
  }

  for (c2_i16 = 0; c2_i16 < 1256; c2_i16++) {
    c2_c_T[c2_i16] = c2_b_T[c2_i16];
  }

  c2_b_current_mass = c2_current_mass;
  c2_c_wind = c2_b_wind;
  for (c2_i17 = 0; c2_i17 < 25010; c2_i17++) {
    chartInstance->c2_b_aerodata[c2_i17] = chartInstance->c2_aerodata[c2_i17];
  }

  c2_b_rocket = c2_rocket;
  c2_c_CM = c2_b_CM;
  c2_c_site_elevation = c2_b_site_elevation;
  c2_c_T0 = c2_b_T0;
  _SFD_SYMBOL_SCOPE_PUSH_EML(0U, 39U, 39U, c2_d_debug_family_names,
    c2_b_debug_family_var_map);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_ux, 0U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_theta, 1U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_b_k, 2U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_b_tol, 3U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_b_delta, 4U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_thrust, 5U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_Tx, 6U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_Ty, 7U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_Lx, 8U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_Ly, 9U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_Dx, 10U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_Dy, 11U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c2_G, 12U, c2_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_r_earth, 13U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_dir, 14U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_b_nargin, 15U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_b_nargout, 16U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_c_t, 17U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_c_t_step, 18U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c2_r, 19U, c2_j_sf_marshallOut,
    c2_i_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c2_b_u, 20U, c2_j_sf_marshallOut,
    c2_i_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c2_c_T, 21U, c2_e_sf_marshallOut,
    c2_j_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_b_current_mass, 22U,
    c2_c_sf_marshallOut, c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_c_wind, 23U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(chartInstance->c2_b_aerodata, 24U,
    c2_d_sf_marshallOut, c2_h_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_b_rocket, 25U, c2_f_sf_marshallOut,
    c2_d_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_c_CM, 26U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_c_site_elevation, 27U,
    c2_c_sf_marshallOut, c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_c_T0, 28U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_b_f_x, 29U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_b_f_y, 30U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_b_moment, 31U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_b_drag, 32U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_b_lift, 33U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_b_CD, 34U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_b_CL, 35U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_b_aoa, 36U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_b_W, 37U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_b_CP, 38U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  CV_SCRIPT_FCN(0, 0);
  _SFD_SCRIPT_CALL(0U, chartInstance->c2_sfEvent, 17);
  c2_ux = c2_b_u[0];
  _SFD_SCRIPT_CALL(0U, chartInstance->c2_sfEvent, 18);
  c2_theta = c2_r[2];
  _SFD_SCRIPT_CALL(0U, chartInstance->c2_sfEvent, 23);
  if (CV_SCRIPT_IF(0, 0, CV_RELATIONAL_EVAL(14U, 0U, 0, c2_c_t, c2_c_T[627], -1,
        2U, c2_c_t < c2_c_T[627]))) {
    _SFD_SCRIPT_CALL(0U, chartInstance->c2_sfEvent, 24);
    c2_b_k = 1.0;
    _SFD_SCRIPT_CALL(0U, chartInstance->c2_sfEvent, 24);
    c2_b_tol = c2_c_t_step;
    _SFD_SCRIPT_CALL(0U, chartInstance->c2_sfEvent, 25);
    do {
      exitg1 = 0;
      CV_SCRIPT_WHILE(0, 0, true);
      _SFD_SCRIPT_CALL(0U, chartInstance->c2_sfEvent, 26);
      c2_h_x = c2_c_T[sf_eml_array_bounds_check(sfGlobalDebugInstanceStruct,
        chartInstance->S, 1U, 768, 176, MAX_uint32_T, (int32_T)sf_integer_check
        (chartInstance->S, 1U, 768U, 176U, c2_b_k), 1, 628) - 1] - c2_c_t;
      c2_i_x = c2_h_x;
      c2_j_x = c2_i_x;
      c2_b_delta = muDoubleScalarAbs(c2_j_x);
      _SFD_SCRIPT_CALL(0U, chartInstance->c2_sfEvent, 27);
      if (CV_SCRIPT_IF(0, 1, CV_RELATIONAL_EVAL(14U, 0U, 1, c2_b_delta, c2_b_tol,
            -1, 2U, c2_b_delta < c2_b_tol))) {
        exitg1 = 1;
      } else {
        _SFD_SCRIPT_CALL(0U, chartInstance->c2_sfEvent, 28);
        c2_b_k++;
        _SFD_SCRIPT_CALL(0U, chartInstance->c2_sfEvent, 25);
        _SF_MEX_LISTEN_FOR_CTRL_C(chartInstance->S);
      }
    } while (exitg1 == 0);

    _SFD_SCRIPT_CALL(0U, chartInstance->c2_sfEvent, 27);
    _SFD_SCRIPT_CALL(0U, chartInstance->c2_sfEvent, 30);
    c2_thrust = c2_c_T[sf_eml_array_bounds_check(sfGlobalDebugInstanceStruct,
      chartInstance->S, 1U, 768, 176, MAX_uint32_T, (int32_T)sf_integer_check
      (chartInstance->S, 1U, 768U, 176U, c2_b_k), 1, 628) + 627];
    _SFD_SCRIPT_CALL(0U, chartInstance->c2_sfEvent, 31);
    c2_d0 = c2_theta;
    c2_b_sind(chartInstance, &c2_d0);
    c2_Tx = c2_d0 * c2_thrust;
    _SFD_SCRIPT_CALL(0U, chartInstance->c2_sfEvent, 32);
    c2_d1 = c2_theta;
    c2_b_cosd(chartInstance, &c2_d1);
    c2_Ty = c2_d1 * c2_thrust;
  } else {
    _SFD_SCRIPT_CALL(0U, chartInstance->c2_sfEvent, 34);
    c2_Tx = 0.0;
    _SFD_SCRIPT_CALL(0U, chartInstance->c2_sfEvent, 35);
    c2_Ty = 0.0;
  }

  _SFD_SCRIPT_CALL(0U, chartInstance->c2_sfEvent, 39);
  for (c2_i18 = 0; c2_i18 < 3; c2_i18++) {
    c2_b_r[c2_i18] = c2_r[c2_i18];
  }

  for (c2_i19 = 0; c2_i19 < 3; c2_i19++) {
    c2_c_u[c2_i19] = c2_b_u[c2_i19];
  }

  for (c2_i20 = 0; c2_i20 < 25010; c2_i20++) {
    chartInstance->c2_c_aerodata[c2_i20] = chartInstance->c2_b_aerodata[c2_i20];
  }

  c2_c_rocket = c2_b_rocket;
  c2_aerodynamics(chartInstance, c2_b_r, c2_c_u, c2_c_wind,
                  chartInstance->c2_c_aerodata, &c2_c_rocket,
                  c2_c_site_elevation, c2_c_T0, &c2_Fdrag, &c2_Flift, &c2_c_CD,
                  &c2_c_CL, &c2_c_aoa, &c2_c_CP);
  c2_b_drag = c2_Fdrag;
  c2_b_lift = c2_Flift;
  c2_b_CD = c2_c_CD;
  c2_b_CL = c2_c_CL;
  c2_b_aoa = c2_c_aoa;
  c2_b_CP = c2_c_CP;
  _SFD_SCRIPT_CALL(0U, chartInstance->c2_sfEvent, 43);
  c2_d2 = c2_theta;
  c2_b_cosd(chartInstance, &c2_d2);
  c2_Lx = c2_d2 * c2_b_lift;
  _SFD_SCRIPT_CALL(0U, chartInstance->c2_sfEvent, 44);
  c2_d3 = c2_theta;
  c2_b_sind(chartInstance, &c2_d3);
  c2_Ly = c2_d3 * c2_b_lift;
  _SFD_SCRIPT_CALL(0U, chartInstance->c2_sfEvent, 47);
  c2_d4 = c2_theta;
  c2_b_sind(chartInstance, &c2_d4);
  c2_Dx = c2_d4 * c2_b_drag;
  _SFD_SCRIPT_CALL(0U, chartInstance->c2_sfEvent, 48);
  c2_d5 = c2_theta;
  c2_b_cosd(chartInstance, &c2_d5);
  c2_Dy = c2_d5 * c2_b_drag;
  _SFD_SCRIPT_CALL(0U, chartInstance->c2_sfEvent, 53);
  c2_G = 3.986E+14;
  _SFD_SCRIPT_CALL(0U, chartInstance->c2_sfEvent, 53);
  c2_r_earth = 6.378E+6;
  _SFD_SCRIPT_CALL(0U, chartInstance->c2_sfEvent, 54);
  for (c2_i21 = 0; c2_i21 < 2; c2_i21++) {
    c2_c_r[c2_i21] = c2_r[c2_i21];
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
  c2_b_W = c2_h_y * c2_b_current_mass;
  _SFD_SCRIPT_CALL(0U, chartInstance->c2_sfEvent, 57);
  if (CV_SCRIPT_IF(0, 2, CV_RELATIONAL_EVAL(14U, 0U, 2, c2_r[1],
        c2_c_site_elevation, -1, 0U, c2_r[1] == c2_c_site_elevation))) {
    _SFD_SCRIPT_CALL(0U, chartInstance->c2_sfEvent, 58);
    c2_b_W = 0.0;
  }

  _SFD_SCRIPT_CALL(0U, chartInstance->c2_sfEvent, 61);
  if (CV_SCRIPT_IF(0, 3, CV_RELATIONAL_EVAL(14U, 0U, 3, c2_c_wind, c2_ux, -1, 5U,
        c2_c_wind >= c2_ux))) {
    _SFD_SCRIPT_CALL(0U, chartInstance->c2_sfEvent, 61);
    c2_dir = 1.0;
  } else {
    _SFD_SCRIPT_CALL(0U, chartInstance->c2_sfEvent, 61);
    c2_dir = -1.0;
  }

  _SFD_SCRIPT_CALL(0U, chartInstance->c2_sfEvent, 64);
  if (CV_SCRIPT_IF(0, 4, CV_RELATIONAL_EVAL(14U, 0U, 4, c2_b_u[1], 0.0, -1, 5U,
        c2_b_u[1] >= 0.0))) {
    _SFD_SCRIPT_CALL(0U, chartInstance->c2_sfEvent, 65);
    c2_b_f_x = (c2_Tx + c2_Lx) + c2_dir * c2_Dx;
    _SFD_SCRIPT_CALL(0U, chartInstance->c2_sfEvent, 66);
    c2_b_f_y = ((c2_Ty + c2_Ly) - c2_Dy) - c2_b_W;
    _SFD_SCRIPT_CALL(0U, chartInstance->c2_sfEvent, 67);
    c2_d6 = c2_theta;
    c2_b_cosd(chartInstance, &c2_d6);
    c2_d7 = c2_theta;
    c2_b_sind(chartInstance, &c2_d7);
    c2_b_moment = (c2_Lx - c2_Dx) * c2_d6 * (c2_b_CP - c2_c_CM) + (c2_Dy + c2_Ly)
      * c2_d7 * (c2_b_CP - c2_c_CM);
  } else {
    _SFD_SCRIPT_CALL(0U, chartInstance->c2_sfEvent, 71);
    c2_b_f_x = c2_Dx + c2_dir * c2_Lx;
    _SFD_SCRIPT_CALL(0U, chartInstance->c2_sfEvent, 72);
    c2_b_f_y = (c2_Ly + c2_Dy) - c2_b_W;
    _SFD_SCRIPT_CALL(0U, chartInstance->c2_sfEvent, 73);
    c2_b_moment = 0.0;
  }

  _SFD_SCRIPT_CALL(0U, chartInstance->c2_sfEvent, -73);
  _SFD_SYMBOL_SCOPE_POP();
  c2_f_x = c2_b_f_x;
  c2_f_y = c2_b_f_y;
  c2_moment = c2_b_moment;
  c2_drag = c2_b_drag;
  c2_lift = c2_b_lift;
  c2_CD = c2_b_CD;
  c2_CL = c2_b_CL;
  c2_aoa = c2_b_aoa;
  c2_W = c2_b_W;
  c2_CP = c2_b_CP;
  _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 31);
  c2_b_forceout[0] = c2_f_x;
  c2_b_forceout[1] = c2_f_y;
  c2_b_forceout[2] = c2_moment;
  c2_b_forceout[3] = c2_drag;
  c2_b_forceout[4] = c2_lift;
  c2_b_forceout[5] = c2_CD;
  c2_b_forceout[6] = c2_CL;
  c2_b_forceout[7] = c2_aoa;
  c2_b_forceout[8] = c2_W;
  c2_b_forceout[9] = c2_CP;
  _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 34);
  c2_b_A = c2_f_x;
  c2_b_B = c2_current_mass;
  c2_l_x = c2_b_A;
  c2_i_y = c2_b_B;
  c2_m_x = c2_l_x;
  c2_j_y = c2_i_y;
  c2_a_x = c2_m_x / c2_j_y;
  _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 35);
  c2_c_A = c2_f_y;
  c2_c_B = c2_current_mass;
  c2_n_x = c2_c_A;
  c2_k_y = c2_c_B;
  c2_o_x = c2_n_x;
  c2_l_y = c2_k_y;
  c2_a_y = c2_o_x / c2_l_y;
  _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 36);
  c2_d_A = c2_moment;
  c2_d_B = c2_I;
  c2_p_x = c2_d_A;
  c2_m_y = c2_d_B;
  c2_q_x = c2_p_x;
  c2_n_y = c2_m_y;
  c2_alpha = c2_q_x / c2_n_y;
  _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 37);
  c2_b_statedot[3] = c2_a_x;
  _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 38);
  c2_b_statedot[4] = c2_a_y;
  _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, 39);
  c2_b_statedot[5] = c2_alpha;
  _SFD_EML_CALL(0U, chartInstance->c2_sfEvent, -39);
  _SFD_SYMBOL_SCOPE_POP();
  for (c2_i22 = 0; c2_i22 < 10; c2_i22++) {
    (*chartInstance->c2_forceout)[c2_i22] = c2_b_forceout[c2_i22];
  }

  for (c2_i23 = 0; c2_i23 < 6; c2_i23++) {
    (*chartInstance->c2_statedot)[c2_i23] = c2_b_statedot[c2_i23];
  }

  _SFD_CC_CALL(EXIT_OUT_OF_FUNCTION_TAG, 0U, chartInstance->c2_sfEvent);
}

static void initSimStructsc2_solver_simulink(SFc2_solver_simulinkInstanceStruct *
  chartInstance)
{
  (void)chartInstance;
}

static void c2_aerodynamics(SFc2_solver_simulinkInstanceStruct *chartInstance,
  real_T c2_r[3], real_T c2_b_u[3], real_T c2_b_wind, real_T c2_e_aerodata[25010],
  c2_sv2voxIt8FKzjMlUEnyXmnC *c2_rocket, real_T c2_b_site_elevation, real_T
  c2_b_T0, real_T *c2_Fdrag, real_T *c2_Flift, real_T *c2_CD, real_T *c2_CL,
  real_T *c2_aoa, real_T *c2_CP)
{
  uint32_T c2_debug_family_var_map[36];
  real_T c2_ux;
  real_T c2_uy;
  real_T c2_umag;
  real_T c2_phi;
  real_T c2_resultant;
  real_T c2_lambda;
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
  real_T c2_Sdrag;
  real_T c2_Slift;
  real_T c2_weight;
  real_T c2_nargin = 7.0;
  real_T c2_nargout = 6.0;
  int32_T c2_i24;
  real_T c2_c_u[2];
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
  real_T c2_d10;
  real_T c2_unusedU0;
  real_T c2_unusedU1;
  real_T c2_b_rho;
  real_T c2_b_local_c;
  real_T c2_A;
  real_T c2_B;
  real_T c2_e_x;
  real_T c2_f_y;
  real_T c2_f_x;
  real_T c2_g_y;
  real_T c2_g_x;
  real_T c2_h_x;
  real_T c2_i_x;
  real_T c2_b_A;
  real_T c2_j_x;
  real_T c2_k_x;
  real_T c2_g_a;
  real_T c2_h_a;
  real_T c2_l_x;
  real_T c2_i_a;
  real_T c2_h_y;
  boolean_T c2_c_p;
  real_T c2_m_x;
  real_T c2_n_x;
  real_T c2_o_x;
  real_T c2_p_x;
  real_T c2_q_x;
  real_T c2_i_y;
  real_T c2_r_x;
  real_T c2_s_x;
  real_T c2_t_x;
  real_T c2_j_y;
  real_T c2_j_a;
  real_T c2_k_a;
  real_T c2_l_a;
  real_T c2_u_x;
  real_T c2_v_x;
  real_T c2_c_A;
  real_T c2_m_a;
  real_T c2_n_a;
  real_T c2_w_x;
  real_T c2_x_x;
  real_T c2_y_x;
  real_T c2_k_y;
  real_T c2_ab_x;
  real_T c2_bb_x;
  real_T c2_o_a;
  boolean_T c2_d_p;
  real_T c2_l_y;
  real_T c2_m_y;
  real_T c2_cb_x;
  boolean_T c2_e_p;
  real_T c2_db_x;
  real_T c2_eb_x;
  real_T c2_n_y;
  real_T c2_p_a;
  real_T c2_q_a;
  real_T c2_r_a;
  real_T c2_fb_x;
  real_T c2_d_A;
  real_T c2_s_a;
  real_T c2_t_a;
  real_T c2_gb_x;
  real_T c2_hb_x;
  real_T c2_o_y;
  real_T c2_ib_x;
  real_T c2_d11;
  real_T c2_u_a;
  boolean_T c2_f_p;
  real_T c2_p_y;
  real_T c2_v_a;
  boolean_T c2_g_p;
  real_T c2_w_a;
  const mxArray *c2_q_y = NULL;
  real_T c2_jb_x;
  static char_T c2_cv0[11] = { 'y', 'o', ',', ' ', 'a', 'o', 'a', ' ', '>', ' ',
    '4' };

  real_T c2_x_a;
  real_T c2_r_y;
  boolean_T c2_h_p;
  real_T c2_d12;
  real_T c2_y_a;
  real_T c2_ab_a;
  real_T c2_kb_x;
  real_T c2_bb_a;
  real_T c2_s_y;
  boolean_T c2_i_p;
  int32_T exitg1;
  _SFD_SYMBOL_SCOPE_PUSH_EML(0U, 36U, 36U, c2_c_debug_family_names,
    c2_debug_family_var_map);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_ux, 0U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_uy, 1U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_umag, 2U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_phi, 3U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_resultant, 4U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_lambda, 5U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_rho, 6U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_local_c, 7U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c2_mu, 8U, c2_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_mach, 9U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_k, 10U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_tol, 11U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_delta, 12U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_ft2m, 13U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_rocket_length, 14U,
    c2_c_sf_marshallOut, c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_Re_rocket_y, 15U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_rail_length, 16U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_launch_site, 17U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_Sdrag, 18U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_Slift, 19U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_weight, 20U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_nargin, 21U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_nargout, 22U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c2_r, 23U, c2_j_sf_marshallOut,
    c2_i_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c2_b_u, 24U, c2_j_sf_marshallOut,
    c2_i_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_b_wind, 25U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c2_e_aerodata, 26U, c2_d_sf_marshallOut,
    c2_h_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c2_rocket, 27U, c2_f_sf_marshallOut,
    c2_d_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_b_site_elevation, 28U,
    c2_c_sf_marshallOut, c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_b_T0, 29U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c2_Fdrag, 30U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c2_Flift, 31U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c2_CD, 32U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c2_CL, 33U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c2_aoa, 34U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c2_CP, 35U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  CV_SCRIPT_FCN(1, 0);
  _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 11);
  c2_ux = c2_b_u[0];
  _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 12);
  c2_uy = c2_b_u[1];
  _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 13);
  for (c2_i24 = 0; c2_i24 < 2; c2_i24++) {
    c2_c_u[c2_i24] = c2_b_u[c2_i24];
  }

  c2_umag = c2_norm(chartInstance, c2_c_u);
  _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 14);
  c2_phi = 90.0 - c2_r[2];
  _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 15);
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
  _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 16);
  c2_d9 = c2_phi;
  c2_b_sind(chartInstance, &c2_d9);
  c2_c_x = c2_b_wind * c2_d9;
  c2_d_y = c2_resultant;
  c2_d_x = c2_c_x;
  c2_e_y = c2_d_y;
  c2_z = c2_d_x / c2_e_y;
  c2_d10 = c2_z;
  c2_b_asind(chartInstance, &c2_d10);
  c2_lambda = c2_d10;
  _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 17);
  *c2_aoa = (180.0 - c2_lambda) - c2_phi;
  _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 20);
  c2_getAtmoConditions(chartInstance, c2_r[1], c2_b_site_elevation, c2_b_T0,
                       &c2_unusedU0, &c2_unusedU1, &c2_b_rho, &c2_b_local_c);
  c2_rho = c2_b_rho;
  c2_local_c = c2_b_local_c;
  c2_mu = 1.846E-5;
  _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 23);
  c2_A = c2_umag;
  c2_B = c2_local_c;
  c2_e_x = c2_A;
  c2_f_y = c2_B;
  c2_f_x = c2_e_x;
  c2_g_y = c2_f_y;
  c2_mach = c2_f_x / c2_g_y;
  _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 24);
  c2_k = 1.0;
  _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 24);
  c2_tol = 0.01;
  _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 25);
  do {
    exitg1 = 0;
    CV_SCRIPT_WHILE(1, 0, true);
    _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 26);
    c2_g_x = c2_mach - c2_e_aerodata[sf_eml_array_bounds_check
      (sfGlobalDebugInstanceStruct, chartInstance->S, 0U, 0, 0, MAX_uint32_T,
       (int32_T)sf_integer_check(chartInstance->S, 0U, 0U, 0U, c2_k), 1, 2501) -
      1];
    c2_h_x = c2_g_x;
    c2_i_x = c2_h_x;
    c2_delta = muDoubleScalarAbs(c2_i_x);
    _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 27);
    if (CV_SCRIPT_IF(1, 0, CV_RELATIONAL_EVAL(14U, 1U, 0, c2_delta, c2_tol, -1,
          2U, c2_delta < c2_tol))) {
      exitg1 = 1;
    } else {
      _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 28);
      c2_k++;
      _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 25);
      _SF_MEX_LISTEN_FOR_CTRL_C(chartInstance->S);
    }
  } while (exitg1 == 0);

  _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 27);
  _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 32);
  c2_ft2m = 0.3048;
  _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 33);
  c2_rocket_length = 11.0 * c2_ft2m;
  _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 34);
  c2_b_A = c2_rho * c2_umag * c2_rocket_length;
  c2_j_x = c2_b_A;
  c2_k_x = c2_j_x;
  c2_Re_rocket_y = c2_k_x / 1.846E-5;
  _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 39);
  c2_rail_length = 5.4864;
  _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 40);
  c2_launch_site = 1293.0;
  _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 41);
  *c2_aoa = 0.0;
  _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 43);
  c2_g_a = c2_rocket->OD;
  c2_h_a = c2_g_a;
  c2_l_x = c2_h_a;
  c2_i_a = c2_l_x;
  c2_h_y = c2_i_a * c2_i_a;
  c2_c_p = false;
  if (c2_c_p) {
    c2_error(chartInstance);
  }

  c2_m_x = 3.1415926535897931 * c2_h_y;
  c2_n_x = c2_m_x;
  c2_Sdrag = c2_n_x / 4.0;
  _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 44);
  c2_Slift = c2_rocket->l * c2_rocket->OD;
  _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 46);
  if (CV_SCRIPT_IF(1, 1, CV_RELATIONAL_EVAL(14U, 1U, 1, c2_b_u[1], 0.0, -1, 5U,
        c2_b_u[1] >= 0.0))) {
    _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 48);
    if (CV_SCRIPT_IF(1, 2, CV_RELATIONAL_EVAL(14U, 1U, 2, c2_r[1],
          c2_rail_length + c2_launch_site, -1, 3U, c2_r[1] <= c2_rail_length +
          c2_launch_site))) {
      _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 49);
      *c2_aoa = 0.0;
    }

    _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 54);
    c2_o_x = *c2_aoa;
    c2_p_x = c2_o_x;
    c2_q_x = c2_p_x;
    c2_i_y = muDoubleScalarAbs(c2_q_x);
    c2_r_x = *c2_aoa;
    c2_s_x = c2_r_x;
    c2_t_x = c2_s_x;
    c2_j_y = muDoubleScalarAbs(c2_t_x);
    if (CV_SCRIPT_IF(1, 3, CV_RELATIONAL_EVAL(14U, 1U, 3, c2_i_y, 2.0, -1, 3U,
          c2_j_y <= 2.0))) {
      _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 55);
      c2_c_A = *c2_aoa;
      c2_x_x = c2_c_A;
      c2_bb_x = c2_x_x;
      c2_weight = c2_bb_x / 2.0;
      _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 56);
      *c2_CD = c2_weight * c2_e_aerodata[sf_eml_array_bounds_check
        (sfGlobalDebugInstanceStruct, chartInstance->S, 0U, 0, 0, MAX_uint32_T,
         (int32_T)sf_integer_check(chartInstance->S, 0U, 0U, 0U, c2_k), 1, 2501)
        + 5001] + (1.0 - c2_weight) * c2_e_aerodata[sf_eml_array_bounds_check
        (sfGlobalDebugInstanceStruct, chartInstance->S, 0U, 0, 0, MAX_uint32_T,
         (int32_T)sf_integer_check(chartInstance->S, 0U, 0U, 0U, c2_k), 1, 2501)
        + 2500];
      _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 57);
      *c2_CL = c2_weight * c2_e_aerodata[sf_eml_array_bounds_check
        (sfGlobalDebugInstanceStruct, chartInstance->S, 0U, 0, 0, MAX_uint32_T,
         (int32_T)sf_integer_check(chartInstance->S, 0U, 0U, 0U, c2_k), 1, 2501)
        + 12504] + (1.0 - c2_weight) * c2_e_aerodata[sf_eml_array_bounds_check
        (sfGlobalDebugInstanceStruct, chartInstance->S, 0U, 0, 0, MAX_uint32_T,
         (int32_T)sf_integer_check(chartInstance->S, 0U, 0U, 0U, c2_k), 1, 2501)
        + 10003];
      _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 58);
      *c2_CP = c2_weight * c2_e_aerodata[sf_eml_array_bounds_check
        (sfGlobalDebugInstanceStruct, chartInstance->S, 0U, 0, 0, MAX_uint32_T,
         (int32_T)sf_integer_check(chartInstance->S, 0U, 0U, 0U, c2_k), 1, 2501)
        + 20007] + (1.0 - c2_weight) * c2_e_aerodata[sf_eml_array_bounds_check
        (sfGlobalDebugInstanceStruct, chartInstance->S, 0U, 0, 0, MAX_uint32_T,
         (int32_T)sf_integer_check(chartInstance->S, 0U, 0U, 0U, c2_k), 1, 2501)
        + 17506];
    } else {
      _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 59);
      c2_v_x = *c2_aoa;
      c2_w_x = c2_v_x;
      c2_ab_x = c2_w_x;
      c2_l_y = muDoubleScalarAbs(c2_ab_x);
      c2_cb_x = *c2_aoa;
      c2_db_x = c2_cb_x;
      c2_eb_x = c2_db_x;
      c2_n_y = muDoubleScalarAbs(c2_eb_x);
      if (CV_SCRIPT_IF(1, 4, CV_RELATIONAL_EVAL(14U, 1U, 4, c2_l_y, 4.0, -1, 3U,
            c2_n_y <= 4.0))) {
        _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 60);
        c2_d_A = *c2_aoa - 2.0;
        c2_gb_x = c2_d_A;
        c2_ib_x = c2_gb_x;
        c2_weight = c2_ib_x / 2.0;
        _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 61);
        *c2_CD = c2_weight * c2_e_aerodata[sf_eml_array_bounds_check
          (sfGlobalDebugInstanceStruct, chartInstance->S, 0U, 0, 0, MAX_uint32_T,
           (int32_T)sf_integer_check(chartInstance->S, 0U, 0U, 0U, c2_k), 1,
           2501) + 7502] + (1.0 - c2_weight) *
          c2_e_aerodata[sf_eml_array_bounds_check(sfGlobalDebugInstanceStruct,
          chartInstance->S, 0U, 0, 0, MAX_uint32_T, (int32_T)sf_integer_check
          (chartInstance->S, 0U, 0U, 0U, c2_k), 1, 2501) + 5001];
        _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 62);
        *c2_CL = c2_weight * c2_e_aerodata[sf_eml_array_bounds_check
          (sfGlobalDebugInstanceStruct, chartInstance->S, 0U, 0, 0, MAX_uint32_T,
           (int32_T)sf_integer_check(chartInstance->S, 0U, 0U, 0U, c2_k), 1,
           2501) + 15005] + (1.0 - c2_weight) *
          c2_e_aerodata[sf_eml_array_bounds_check(sfGlobalDebugInstanceStruct,
          chartInstance->S, 0U, 0, 0, MAX_uint32_T, (int32_T)sf_integer_check
          (chartInstance->S, 0U, 0U, 0U, c2_k), 1, 2501) + 12504];
        _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 63);
        *c2_CP = c2_weight * c2_e_aerodata[sf_eml_array_bounds_check
          (sfGlobalDebugInstanceStruct, chartInstance->S, 0U, 0, 0, MAX_uint32_T,
           (int32_T)sf_integer_check(chartInstance->S, 0U, 0U, 0U, c2_k), 1,
           2501) + 22508] + (1.0 - c2_weight) *
          c2_e_aerodata[sf_eml_array_bounds_check(sfGlobalDebugInstanceStruct,
          chartInstance->S, 0U, 0, 0, MAX_uint32_T, (int32_T)sf_integer_check
          (chartInstance->S, 0U, 0U, 0U, c2_k), 1, 2501) + 20007];
      } else {
        _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 65);
        *c2_CD = 1.0E+10;
        _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 66);
        *c2_CL = 1.0E+10;
        _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 67);
        *c2_CP = 1.0E+10;
        _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 68);
        c2_q_y = NULL;
        sf_mex_assign(&c2_q_y, sf_mex_create("y", c2_cv0, 10, 0U, 1U, 0U, 2, 1,
          11), false);
        sf_mex_call_debug(sfGlobalDebugInstanceStruct, "disp", 0U, 1U, 14,
                          c2_q_y);
      }
    }

    _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 71);
    c2_d11 = *c2_aoa;
    c2_b_cosd(chartInstance, &c2_d11);
    c2_v_a = c2_d11 * c2_umag;
    c2_w_a = c2_v_a;
    c2_jb_x = c2_w_a;
    c2_x_a = c2_jb_x;
    c2_r_y = c2_x_a * c2_x_a;
    c2_h_p = false;
    if (c2_h_p) {
      c2_error(chartInstance);
    }

    *c2_Fdrag = *c2_CD * c2_Sdrag * 0.5 * c2_rho * c2_r_y;
    _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 72);
    c2_d12 = *c2_aoa;
    c2_b_sind(chartInstance, &c2_d12);
    c2_y_a = c2_d12 * c2_umag;
    c2_ab_a = c2_y_a;
    c2_kb_x = c2_ab_a;
    c2_bb_a = c2_kb_x;
    c2_s_y = c2_bb_a * c2_bb_a;
    c2_i_p = false;
    if (c2_i_p) {
      c2_error(chartInstance);
    }

    *c2_Flift = *c2_CL * c2_Slift * 0.5 * c2_rho * c2_s_y;
  } else {
    _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 74);
    if (CV_SCRIPT_COND(1, 0, CV_RELATIONAL_EVAL(14U, 1U, 5, c2_b_u[1],
          c2_rocket->drogue.deploy_u, -1, 2U, c2_b_u[1] <
          c2_rocket->drogue.deploy_u)) && CV_SCRIPT_COND(1, 1,
         CV_RELATIONAL_EVAL(14U, 1U, 6, c2_r[1], c2_rocket->main.deploy_h, -1,
                            4U, c2_r[1] > c2_rocket->main.deploy_h))) {
      CV_SCRIPT_MCDC(1, 0, true);
      CV_SCRIPT_IF(1, 5, true);
      _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 75);
      *c2_CD = 0.0;
      _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 76);
      *c2_CL = 1.0;
      _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 77);
      *c2_CP = c2_e_aerodata[17507];
      _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 78);
      c2_j_a = c2_uy;
      c2_k_a = c2_j_a;
      c2_u_x = c2_k_a;
      c2_n_a = c2_u_x;
      c2_k_y = c2_n_a * c2_n_a;
      c2_d_p = false;
      if (c2_d_p) {
        c2_error(chartInstance);
      }

      *c2_Fdrag = c2_rocket->drogue.Cd * c2_rocket->drogue.S * 0.5 * c2_rho *
        c2_k_y;
      _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 79);
      c2_p_a = c2_ux - c2_b_wind;
      c2_q_a = c2_p_a;
      c2_fb_x = c2_q_a;
      c2_t_a = c2_fb_x;
      c2_o_y = c2_t_a * c2_t_a;
      c2_f_p = false;
      if (c2_f_p) {
        c2_error(chartInstance);
      }

      *c2_Flift = *c2_CL * c2_Slift * 0.5 * c2_rho * c2_o_y;
    } else {
      CV_SCRIPT_MCDC(1, 0, false);
      CV_SCRIPT_IF(1, 5, false);
      _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 81);
      if (CV_SCRIPT_IF(1, 6, CV_RELATIONAL_EVAL(14U, 1U, 7, c2_r[1],
            c2_rocket->main.deploy_h, -1, 2U, c2_r[1] < c2_rocket->main.deploy_h)))
      {
        _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 82);
        *c2_CD = 0.0;
        _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 83);
        *c2_CL = 1.0;
        _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 84);
        *c2_CP = c2_e_aerodata[17507];
        _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 85);
        c2_l_a = c2_uy;
        c2_m_a = c2_l_a;
        c2_y_x = c2_m_a;
        c2_o_a = c2_y_x;
        c2_m_y = c2_o_a * c2_o_a;
        c2_e_p = false;
        if (c2_e_p) {
          c2_error(chartInstance);
        }

        *c2_Fdrag = c2_rocket->main.Cd * c2_rocket->main.S * 0.5 * c2_rho *
          c2_m_y;
        _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 86);
        c2_r_a = c2_ux - c2_b_wind;
        c2_s_a = c2_r_a;
        c2_hb_x = c2_s_a;
        c2_u_a = c2_hb_x;
        c2_p_y = c2_u_a * c2_u_a;
        c2_g_p = false;
        if (c2_g_p) {
          c2_error(chartInstance);
        }

        *c2_Flift = *c2_CL * c2_Slift * 0.5 * c2_rho * c2_p_y;
      } else {
        _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 88);
        *c2_CD = 0.0;
        _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 89);
        *c2_CL = 0.0;
        _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 90);
        *c2_CP = c2_e_aerodata[17507];
        _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 91);
        *c2_Fdrag = 0.0;
        _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, 92);
        *c2_Flift = 0.0;
      }
    }
  }

  _SFD_SCRIPT_CALL(1U, chartInstance->c2_sfEvent, -92);
  _SFD_SYMBOL_SCOPE_POP();
}

static void c2_getAtmoConditions(SFc2_solver_simulinkInstanceStruct
  *chartInstance, real_T c2_h, real_T c2_h0, real_T c2_Tlaunch, real_T *c2_b_T,
  real_T *c2_P, real_T *c2_rho, real_T *c2_a)
{
  uint32_T c2_debug_family_var_map[21];
  real_T c2_mu;
  real_T c2_b_T0[8];
  real_T c2_Trate[7];
  real_T c2_hpos[7];
  real_T c2_g;
  real_T c2_avogadro;
  real_T c2_k_boltz;
  real_T c2_m_air;
  real_T c2_R_air;
  real_T c2_P_sl;
  real_T c2_k;
  real_T c2_rho_sl;
  real_T c2_nargin = 3.0;
  real_T c2_nargout = 5.0;
  int32_T c2_i25;
  static real_T c2_dv2[7] = { -0.0065, 0.0, 0.003, 0.0, -0.0045, 0.0, 0.004 };

  int32_T c2_i26;
  static real_T c2_dv3[7] = { 11000.0, 25000.0, 47000.0, 53000.0, 79000.0,
    90000.0, 105000.0 };

  real_T c2_x;
  real_T c2_b_y;
  real_T c2_b_x;
  real_T c2_c_y;
  real_T c2_z;
  real_T c2_c_x;
  real_T c2_d_x;
  real_T c2_B;
  real_T c2_d_y;
  real_T c2_e_y;
  real_T c2_f_y;
  real_T c2_e_x;
  real_T c2_f_x;
  real_T c2_g_x;
  real_T c2_h_x;
  boolean_T c2_p;
  boolean_T c2_b_p;
  real_T c2_i_x;
  real_T c2_j_x;
  _SFD_SYMBOL_SCOPE_PUSH_EML(0U, 21U, 21U, c2_b_debug_family_names,
    c2_debug_family_var_map);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_mu, 0U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c2_b_T0, 1U, c2_i_sf_marshallOut,
    c2_g_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c2_Trate, 2U, c2_h_sf_marshallOut,
    c2_f_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c2_hpos, 3U, c2_h_sf_marshallOut,
    c2_f_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c2_g, 4U, c2_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c2_avogadro, 5U, c2_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_k_boltz, 6U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_m_air, 7U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_R_air, 8U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_P_sl, 9U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_k, 10U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_rho_sl, 11U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_nargin, 12U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_nargout, 13U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_h, 14U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_h0, 15U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c2_Tlaunch, 16U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c2_b_T, 17U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c2_P, 18U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c2_rho, 19U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c2_a, 20U, c2_c_sf_marshallOut,
    c2_c_sf_marshallIn);
  CV_SCRIPT_FCN(2, 0);
  _SFD_SCRIPT_CALL(2U, chartInstance->c2_sfEvent, 10);
  c2_h += c2_h0;
  _SFD_SCRIPT_CALL(2U, chartInstance->c2_sfEvent, 11);
  c2_b_T0[0] = c2_Tlaunch;
  c2_b_T0[1] = 216.66;
  c2_b_T0[2] = 216.66;
  c2_b_T0[3] = 282.66;
  c2_b_T0[4] = 282.66;
  c2_b_T0[5] = 165.66;
  c2_b_T0[6] = 165.66;
  c2_b_T0[7] = 225.66;
  _SFD_SCRIPT_CALL(2U, chartInstance->c2_sfEvent, 12);
  for (c2_i25 = 0; c2_i25 < 7; c2_i25++) {
    c2_Trate[c2_i25] = c2_dv2[c2_i25];
  }

  _SFD_SCRIPT_CALL(2U, chartInstance->c2_sfEvent, 13);
  for (c2_i26 = 0; c2_i26 < 7; c2_i26++) {
    c2_hpos[c2_i26] = c2_dv3[c2_i26];
  }

  _SFD_SCRIPT_CALL(2U, chartInstance->c2_sfEvent, 15);
  if (CV_SCRIPT_IF(2, 0, CV_RELATIONAL_EVAL(14U, 2U, 0, c2_h, c2_hpos[0], -1, 3U,
        c2_h <= c2_hpos[0]))) {
    _SFD_SCRIPT_CALL(2U, chartInstance->c2_sfEvent, 16);
    *c2_b_T = c2_b_T0[0] + c2_Trate[0] * (c2_hpos[0] - c2_h);
  } else {
    _SFD_SCRIPT_CALL(2U, chartInstance->c2_sfEvent, 17);
    if (CV_SCRIPT_IF(2, 1, CV_RELATIONAL_EVAL(14U, 2U, 1, c2_h, c2_hpos[1], -1,
          3U, c2_h <= c2_hpos[1]))) {
      _SFD_SCRIPT_CALL(2U, chartInstance->c2_sfEvent, 18);
      *c2_b_T = c2_b_T0[1] + c2_Trate[1] * (c2_hpos[1] - c2_h);
    } else {
      _SFD_SCRIPT_CALL(2U, chartInstance->c2_sfEvent, 19);
      if (CV_SCRIPT_IF(2, 2, CV_RELATIONAL_EVAL(14U, 2U, 2, c2_h, c2_hpos[2], -1,
            3U, c2_h <= c2_hpos[2]))) {
        _SFD_SCRIPT_CALL(2U, chartInstance->c2_sfEvent, 20);
        *c2_b_T = c2_b_T0[2] + c2_Trate[2] * (c2_hpos[2] - c2_h);
      } else {
        _SFD_SCRIPT_CALL(2U, chartInstance->c2_sfEvent, 21);
        if (CV_SCRIPT_IF(2, 3, CV_RELATIONAL_EVAL(14U, 2U, 3, c2_h, c2_hpos[3],
              -1, 3U, c2_h <= c2_hpos[3]))) {
          _SFD_SCRIPT_CALL(2U, chartInstance->c2_sfEvent, 22);
          *c2_b_T = c2_b_T0[3] + c2_Trate[3] * (c2_hpos[3] - c2_h);
        } else {
          _SFD_SCRIPT_CALL(2U, chartInstance->c2_sfEvent, 23);
          if (CV_SCRIPT_IF(2, 4, CV_RELATIONAL_EVAL(14U, 2U, 4, c2_h, c2_hpos[4],
                -1, 3U, c2_h <= c2_hpos[4]))) {
            _SFD_SCRIPT_CALL(2U, chartInstance->c2_sfEvent, 24);
            *c2_b_T = c2_b_T0[4] + c2_Trate[4] * (c2_hpos[4] - c2_h);
          } else {
            _SFD_SCRIPT_CALL(2U, chartInstance->c2_sfEvent, 25);
            if (CV_SCRIPT_IF(2, 5, CV_RELATIONAL_EVAL(14U, 2U, 5, c2_h, c2_hpos
                  [5], -1, 3U, c2_h <= c2_hpos[5]))) {
              _SFD_SCRIPT_CALL(2U, chartInstance->c2_sfEvent, 26);
              *c2_b_T = c2_b_T0[5] + c2_Trate[5] * (c2_hpos[5] - c2_h);
            } else {
              _SFD_SCRIPT_CALL(2U, chartInstance->c2_sfEvent, 27);
              if (CV_SCRIPT_IF(2, 6, CV_RELATIONAL_EVAL(14U, 2U, 6, c2_h,
                    c2_hpos[6], -1, 3U, c2_h <= c2_hpos[6]))) {
                _SFD_SCRIPT_CALL(2U, chartInstance->c2_sfEvent, 28);
                *c2_b_T = c2_b_T0[6] + c2_Trate[6] * (c2_hpos[6] - c2_h);
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
  c2_rho_sl = 1.225;
  _SFD_SCRIPT_CALL(2U, chartInstance->c2_sfEvent, 48);
  c2_B = c2_R_air * *c2_b_T;
  c2_d_y = c2_B;
  c2_e_y = c2_d_y;
  c2_f_y = 9.81 / c2_e_y;
  c2_e_x = -c2_f_y * c2_h;
  c2_f_x = c2_e_x;
  c2_f_x = muDoubleScalarExp(c2_f_x);
  *c2_rho = c2_rho_sl * c2_f_x;
  _SFD_SCRIPT_CALL(2U, chartInstance->c2_sfEvent, 51);
  c2_g_x = c2_k * c2_R_air * *c2_b_T;
  *c2_a = c2_g_x;
  c2_h_x = *c2_a;
  c2_p = (c2_h_x < 0.0);
  c2_b_p = c2_p;
  if (c2_b_p) {
    c2_c_error(chartInstance);
  }

  c2_i_x = *c2_a;
  *c2_a = c2_i_x;
  c2_j_x = *c2_a;
  *c2_a = c2_j_x;
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
  c2_sOKxARu9Dkm6wShDKKOBVjE *c2_b_y)
{
  emlrtMsgIdentifier c2_thisId;
  c2_thisId.fIdentifier = c2_identifier;
  c2_thisId.fParent = NULL;
  c2_thisId.bParentIsCell = false;
  c2_b_emlrt_marshallIn(chartInstance, sf_mex_dup(c2_load), &c2_thisId, c2_b_y);
  sf_mex_destroy(&c2_load);
}

static void c2_b_emlrt_marshallIn(SFc2_solver_simulinkInstanceStruct
  *chartInstance, const mxArray *c2_b_u, const emlrtMsgIdentifier *c2_parentId,
  c2_sOKxARu9Dkm6wShDKKOBVjE *c2_b_y)
{
  emlrtMsgIdentifier c2_thisId;
  static const char * c2_fieldNames[1] = { "rocket" };

  c2_thisId.fParent = c2_parentId;
  c2_thisId.bParentIsCell = false;
  sf_mex_check_struct(c2_parentId, c2_b_u, 1, c2_fieldNames, 0U, NULL);
  c2_thisId.fIdentifier = "rocket";
  c2_c_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getfield(c2_b_u,
    "rocket", "rocket", 0)), &c2_thisId, &c2_b_y->rocket);
  sf_mex_destroy(&c2_b_u);
}

static void c2_c_emlrt_marshallIn(SFc2_solver_simulinkInstanceStruct
  *chartInstance, const mxArray *c2_b_u, const emlrtMsgIdentifier *c2_parentId,
  c2_sv2voxIt8FKzjMlUEnyXmnC *c2_b_y)
{
  emlrtMsgIdentifier c2_thisId;
  static const char * c2_fieldNames[12] = { "OD", "drymass", "wetmass", "nose",
    "shoulder", "fore", "aft", "fin", "l", "drogue", "main", "payload" };

  c2_thisId.fParent = c2_parentId;
  c2_thisId.bParentIsCell = false;
  sf_mex_check_struct(c2_parentId, c2_b_u, 12, c2_fieldNames, 0U, NULL);
  c2_thisId.fIdentifier = "OD";
  c2_b_y->OD = c2_d_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getfield
    (c2_b_u, "OD", "OD", 0)), &c2_thisId);
  c2_thisId.fIdentifier = "drymass";
  c2_b_y->drymass = c2_d_emlrt_marshallIn(chartInstance, sf_mex_dup
    (sf_mex_getfield(c2_b_u, "drymass", "drymass", 0)), &c2_thisId);
  c2_thisId.fIdentifier = "wetmass";
  c2_b_y->wetmass = c2_d_emlrt_marshallIn(chartInstance, sf_mex_dup
    (sf_mex_getfield(c2_b_u, "wetmass", "wetmass", 0)), &c2_thisId);
  c2_thisId.fIdentifier = "nose";
  c2_b_y->nose = c2_e_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getfield
    (c2_b_u, "nose", "nose", 0)), &c2_thisId);
  c2_thisId.fIdentifier = "shoulder";
  c2_b_y->shoulder = c2_g_emlrt_marshallIn(chartInstance, sf_mex_dup
    (sf_mex_getfield(c2_b_u, "shoulder", "shoulder", 0)), &c2_thisId);
  c2_thisId.fIdentifier = "fore";
  c2_b_y->fore = c2_h_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getfield
    (c2_b_u, "fore", "fore", 0)), &c2_thisId);
  c2_thisId.fIdentifier = "aft";
  c2_b_y->aft = c2_i_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getfield
    (c2_b_u, "aft", "aft", 0)), &c2_thisId);
  c2_thisId.fIdentifier = "fin";
  c2_b_y->fin = c2_j_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getfield
    (c2_b_u, "fin", "fin", 0)), &c2_thisId);
  c2_thisId.fIdentifier = "l";
  c2_b_y->l = c2_d_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getfield
    (c2_b_u, "l", "l", 0)), &c2_thisId);
  c2_thisId.fIdentifier = "drogue";
  c2_b_y->drogue = c2_k_emlrt_marshallIn(chartInstance, sf_mex_dup
    (sf_mex_getfield(c2_b_u, "drogue", "drogue", 0)), &c2_thisId);
  c2_thisId.fIdentifier = "main";
  c2_b_y->main = c2_l_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getfield
    (c2_b_u, "main", "main", 0)), &c2_thisId);
  c2_thisId.fIdentifier = "payload";
  c2_b_y->payload = c2_m_emlrt_marshallIn(chartInstance, sf_mex_dup
    (sf_mex_getfield(c2_b_u, "payload", "payload", 0)), &c2_thisId);
  sf_mex_destroy(&c2_b_u);
}

static real_T c2_d_emlrt_marshallIn(SFc2_solver_simulinkInstanceStruct
  *chartInstance, const mxArray *c2_b_u, const emlrtMsgIdentifier *c2_parentId)
{
  real_T c2_b_y;
  real_T c2_d13;
  (void)chartInstance;
  sf_mex_import(c2_parentId, sf_mex_dup(c2_b_u), &c2_d13, 1, 0, 0U, 0, 0U, 0);
  c2_b_y = c2_d13;
  sf_mex_destroy(&c2_b_u);
  return c2_b_y;
}

static c2_sof5eNcdcizKAe6jjm9xhtG c2_e_emlrt_marshallIn
  (SFc2_solver_simulinkInstanceStruct *chartInstance, const mxArray *c2_b_u,
   const emlrtMsgIdentifier *c2_parentId)
{
  c2_sof5eNcdcizKAe6jjm9xhtG c2_b_y;
  emlrtMsgIdentifier c2_thisId;
  static const char * c2_fieldNames[4] = { "wetmass", "drymass", "type", "l" };

  c2_thisId.fParent = c2_parentId;
  c2_thisId.bParentIsCell = false;
  sf_mex_check_struct(c2_parentId, c2_b_u, 4, c2_fieldNames, 0U, NULL);
  c2_thisId.fIdentifier = "wetmass";
  c2_b_y.wetmass = c2_d_emlrt_marshallIn(chartInstance, sf_mex_dup
    (sf_mex_getfield(c2_b_u, "wetmass", "wetmass", 0)), &c2_thisId);
  c2_thisId.fIdentifier = "drymass";
  c2_b_y.drymass = c2_d_emlrt_marshallIn(chartInstance, sf_mex_dup
    (sf_mex_getfield(c2_b_u, "drymass", "drymass", 0)), &c2_thisId);
  c2_thisId.fIdentifier = "type";
  c2_f_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getfield(c2_b_u, "type",
    "type", 0)), &c2_thisId, c2_b_y.type);
  c2_thisId.fIdentifier = "l";
  c2_b_y.l = c2_d_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getfield
    (c2_b_u, "l", "l", 0)), &c2_thisId);
  sf_mex_destroy(&c2_b_u);
  return c2_b_y;
}

static void c2_f_emlrt_marshallIn(SFc2_solver_simulinkInstanceStruct
  *chartInstance, const mxArray *c2_b_u, const emlrtMsgIdentifier *c2_parentId,
  char_T c2_b_y[9])
{
  char_T c2_cv1[9];
  int32_T c2_i27;
  (void)chartInstance;
  sf_mex_import(c2_parentId, sf_mex_dup(c2_b_u), c2_cv1, 1, 10, 0U, 1, 0U, 2, 1,
                9);
  for (c2_i27 = 0; c2_i27 < 9; c2_i27++) {
    c2_b_y[c2_i27] = c2_cv1[c2_i27];
  }

  sf_mex_destroy(&c2_b_u);
}

static c2_syx7FYmGk3I6e3U1RUwUAWD c2_g_emlrt_marshallIn
  (SFc2_solver_simulinkInstanceStruct *chartInstance, const mxArray *c2_b_u,
   const emlrtMsgIdentifier *c2_parentId)
{
  c2_syx7FYmGk3I6e3U1RUwUAWD c2_b_y;
  emlrtMsgIdentifier c2_thisId;
  static const char * c2_fieldNames[1] = { "l" };

  c2_thisId.fParent = c2_parentId;
  c2_thisId.bParentIsCell = false;
  sf_mex_check_struct(c2_parentId, c2_b_u, 1, c2_fieldNames, 0U, NULL);
  c2_thisId.fIdentifier = "l";
  c2_b_y.l = c2_d_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getfield
    (c2_b_u, "l", "l", 0)), &c2_thisId);
  sf_mex_destroy(&c2_b_u);
  return c2_b_y;
}

static c2_sqGsSc4djWypCV90lhUGiL c2_h_emlrt_marshallIn
  (SFc2_solver_simulinkInstanceStruct *chartInstance, const mxArray *c2_b_u,
   const emlrtMsgIdentifier *c2_parentId)
{
  c2_sqGsSc4djWypCV90lhUGiL c2_b_y;
  emlrtMsgIdentifier c2_thisId;
  static const char * c2_fieldNames[2] = { "l", "S" };

  c2_thisId.fParent = c2_parentId;
  c2_thisId.bParentIsCell = false;
  sf_mex_check_struct(c2_parentId, c2_b_u, 2, c2_fieldNames, 0U, NULL);
  c2_thisId.fIdentifier = "l";
  c2_b_y.l = c2_d_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getfield
    (c2_b_u, "l", "l", 0)), &c2_thisId);
  c2_thisId.fIdentifier = "S";
  c2_b_y.S = c2_d_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getfield
    (c2_b_u, "S", "S", 0)), &c2_thisId);
  sf_mex_destroy(&c2_b_u);
  return c2_b_y;
}

static c2_su2bTaGhbTlLQVXsqMBjEuH c2_i_emlrt_marshallIn
  (SFc2_solver_simulinkInstanceStruct *chartInstance, const mxArray *c2_b_u,
   const emlrtMsgIdentifier *c2_parentId)
{
  c2_su2bTaGhbTlLQVXsqMBjEuH c2_b_y;
  emlrtMsgIdentifier c2_thisId;
  static const char * c2_fieldNames[3] = { "l", "wetmass", "drymass" };

  c2_thisId.fParent = c2_parentId;
  c2_thisId.bParentIsCell = false;
  sf_mex_check_struct(c2_parentId, c2_b_u, 3, c2_fieldNames, 0U, NULL);
  c2_thisId.fIdentifier = "l";
  c2_b_y.l = c2_d_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getfield
    (c2_b_u, "l", "l", 0)), &c2_thisId);
  c2_thisId.fIdentifier = "wetmass";
  c2_b_y.wetmass = c2_d_emlrt_marshallIn(chartInstance, sf_mex_dup
    (sf_mex_getfield(c2_b_u, "wetmass", "wetmass", 0)), &c2_thisId);
  c2_thisId.fIdentifier = "drymass";
  c2_b_y.drymass = c2_d_emlrt_marshallIn(chartInstance, sf_mex_dup
    (sf_mex_getfield(c2_b_u, "drymass", "drymass", 0)), &c2_thisId);
  sf_mex_destroy(&c2_b_u);
  return c2_b_y;
}

static c2_sNlKjRKFHwjFKC34GeUJivG c2_j_emlrt_marshallIn
  (SFc2_solver_simulinkInstanceStruct *chartInstance, const mxArray *c2_b_u,
   const emlrtMsgIdentifier *c2_parentId)
{
  c2_sNlKjRKFHwjFKC34GeUJivG c2_b_y;
  emlrtMsgIdentifier c2_thisId;
  static const char * c2_fieldNames[8] = { "t", "span", "rootchord", "tipchord",
    "sweep", "S", "AR", "num" };

  c2_thisId.fParent = c2_parentId;
  c2_thisId.bParentIsCell = false;
  sf_mex_check_struct(c2_parentId, c2_b_u, 8, c2_fieldNames, 0U, NULL);
  c2_thisId.fIdentifier = "t";
  c2_b_y.t = c2_d_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getfield
    (c2_b_u, "t", "t", 0)), &c2_thisId);
  c2_thisId.fIdentifier = "span";
  c2_b_y.span = c2_d_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getfield
    (c2_b_u, "span", "span", 0)), &c2_thisId);
  c2_thisId.fIdentifier = "rootchord";
  c2_b_y.rootchord = c2_d_emlrt_marshallIn(chartInstance, sf_mex_dup
    (sf_mex_getfield(c2_b_u, "rootchord", "rootchord", 0)), &c2_thisId);
  c2_thisId.fIdentifier = "tipchord";
  c2_b_y.tipchord = c2_d_emlrt_marshallIn(chartInstance, sf_mex_dup
    (sf_mex_getfield(c2_b_u, "tipchord", "tipchord", 0)), &c2_thisId);
  c2_thisId.fIdentifier = "sweep";
  c2_b_y.sweep = c2_d_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getfield
    (c2_b_u, "sweep", "sweep", 0)), &c2_thisId);
  c2_thisId.fIdentifier = "S";
  c2_b_y.S = c2_d_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getfield
    (c2_b_u, "S", "S", 0)), &c2_thisId);
  c2_thisId.fIdentifier = "AR";
  c2_b_y.AR = c2_d_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getfield
    (c2_b_u, "AR", "AR", 0)), &c2_thisId);
  c2_thisId.fIdentifier = "num";
  c2_b_y.num = c2_d_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getfield
    (c2_b_u, "num", "num", 0)), &c2_thisId);
  sf_mex_destroy(&c2_b_u);
  return c2_b_y;
}

static c2_sKS3cuSxR3HIcTIZZkbRPm c2_k_emlrt_marshallIn
  (SFc2_solver_simulinkInstanceStruct *chartInstance, const mxArray *c2_b_u,
   const emlrtMsgIdentifier *c2_parentId)
{
  c2_sKS3cuSxR3HIcTIZZkbRPm c2_b_y;
  emlrtMsgIdentifier c2_thisId;
  static const char * c2_fieldNames[4] = { "d", "S", "Cd", "deploy_u" };

  c2_thisId.fParent = c2_parentId;
  c2_thisId.bParentIsCell = false;
  sf_mex_check_struct(c2_parentId, c2_b_u, 4, c2_fieldNames, 0U, NULL);
  c2_thisId.fIdentifier = "d";
  c2_b_y.d = c2_d_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getfield
    (c2_b_u, "d", "d", 0)), &c2_thisId);
  c2_thisId.fIdentifier = "S";
  c2_b_y.S = c2_d_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getfield
    (c2_b_u, "S", "S", 0)), &c2_thisId);
  c2_thisId.fIdentifier = "Cd";
  c2_b_y.Cd = c2_d_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getfield
    (c2_b_u, "Cd", "Cd", 0)), &c2_thisId);
  c2_thisId.fIdentifier = "deploy_u";
  c2_b_y.deploy_u = c2_d_emlrt_marshallIn(chartInstance, sf_mex_dup
    (sf_mex_getfield(c2_b_u, "deploy_u", "deploy_u", 0)), &c2_thisId);
  sf_mex_destroy(&c2_b_u);
  return c2_b_y;
}

static c2_s88seaxzVTYYZIzTGqmP4AD c2_l_emlrt_marshallIn
  (SFc2_solver_simulinkInstanceStruct *chartInstance, const mxArray *c2_b_u,
   const emlrtMsgIdentifier *c2_parentId)
{
  c2_s88seaxzVTYYZIzTGqmP4AD c2_b_y;
  emlrtMsgIdentifier c2_thisId;
  static const char * c2_fieldNames[4] = { "d", "S", "Cd", "deploy_h" };

  c2_thisId.fParent = c2_parentId;
  c2_thisId.bParentIsCell = false;
  sf_mex_check_struct(c2_parentId, c2_b_u, 4, c2_fieldNames, 0U, NULL);
  c2_thisId.fIdentifier = "d";
  c2_b_y.d = c2_d_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getfield
    (c2_b_u, "d", "d", 0)), &c2_thisId);
  c2_thisId.fIdentifier = "S";
  c2_b_y.S = c2_d_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getfield
    (c2_b_u, "S", "S", 0)), &c2_thisId);
  c2_thisId.fIdentifier = "Cd";
  c2_b_y.Cd = c2_d_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getfield
    (c2_b_u, "Cd", "Cd", 0)), &c2_thisId);
  c2_thisId.fIdentifier = "deploy_h";
  c2_b_y.deploy_h = c2_d_emlrt_marshallIn(chartInstance, sf_mex_dup
    (sf_mex_getfield(c2_b_u, "deploy_h", "deploy_h", 0)), &c2_thisId);
  sf_mex_destroy(&c2_b_u);
  return c2_b_y;
}

static c2_sfpFxKVMN8elArLoV353vAE c2_m_emlrt_marshallIn
  (SFc2_solver_simulinkInstanceStruct *chartInstance, const mxArray *c2_b_u,
   const emlrtMsgIdentifier *c2_parentId)
{
  c2_sfpFxKVMN8elArLoV353vAE c2_b_y;
  emlrtMsgIdentifier c2_thisId;
  static const char * c2_fieldNames[2] = { "chute_d", "chute_Cd" };

  c2_thisId.fParent = c2_parentId;
  c2_thisId.bParentIsCell = false;
  sf_mex_check_struct(c2_parentId, c2_b_u, 2, c2_fieldNames, 0U, NULL);
  c2_thisId.fIdentifier = "chute_d";
  c2_b_y.chute_d = c2_d_emlrt_marshallIn(chartInstance, sf_mex_dup
    (sf_mex_getfield(c2_b_u, "chute_d", "chute_d", 0)), &c2_thisId);
  c2_thisId.fIdentifier = "chute_Cd";
  c2_b_y.chute_Cd = c2_d_emlrt_marshallIn(chartInstance, sf_mex_dup
    (sf_mex_getfield(c2_b_u, "chute_Cd", "chute_Cd", 0)), &c2_thisId);
  sf_mex_destroy(&c2_b_u);
  return c2_b_y;
}

static const mxArray *c2_sf_marshallOut(void *chartInstanceVoid, void *c2_inData)
{
  const mxArray *c2_mxArrayOutData;
  int32_T c2_i28;
  const mxArray *c2_b_y = NULL;
  real_T c2_b_u[6];
  SFc2_solver_simulinkInstanceStruct *chartInstance;
  chartInstance = (SFc2_solver_simulinkInstanceStruct *)chartInstanceVoid;
  c2_mxArrayOutData = NULL;
  c2_mxArrayOutData = NULL;
  for (c2_i28 = 0; c2_i28 < 6; c2_i28++) {
    c2_b_u[c2_i28] = (*(real_T (*)[6])c2_inData)[c2_i28];
  }

  c2_b_y = NULL;
  sf_mex_assign(&c2_b_y, sf_mex_create("y", c2_b_u, 0, 0U, 1U, 0U, 1, 6), false);
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
  *chartInstance, const mxArray *c2_b_u, const emlrtMsgIdentifier *c2_parentId,
  real_T c2_b_y[6])
{
  real_T c2_dv4[6];
  int32_T c2_i29;
  (void)chartInstance;
  sf_mex_import(c2_parentId, sf_mex_dup(c2_b_u), c2_dv4, 1, 0, 0U, 1, 0U, 1, 6);
  for (c2_i29 = 0; c2_i29 < 6; c2_i29++) {
    c2_b_y[c2_i29] = c2_dv4[c2_i29];
  }

  sf_mex_destroy(&c2_b_u);
}

static void c2_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c2_mxArrayInData, const char_T *c2_varName, void *c2_outData)
{
  const mxArray *c2_b_statedot;
  const char_T *c2_identifier;
  emlrtMsgIdentifier c2_thisId;
  real_T c2_b_y[6];
  int32_T c2_i30;
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
  for (c2_i30 = 0; c2_i30 < 6; c2_i30++) {
    (*(real_T (*)[6])c2_outData)[c2_i30] = c2_b_y[c2_i30];
  }

  sf_mex_destroy(&c2_mxArrayInData);
}

static const mxArray *c2_b_sf_marshallOut(void *chartInstanceVoid, void
  *c2_inData)
{
  const mxArray *c2_mxArrayOutData;
  int32_T c2_i31;
  const mxArray *c2_b_y = NULL;
  real_T c2_b_u[10];
  SFc2_solver_simulinkInstanceStruct *chartInstance;
  chartInstance = (SFc2_solver_simulinkInstanceStruct *)chartInstanceVoid;
  c2_mxArrayOutData = NULL;
  c2_mxArrayOutData = NULL;
  for (c2_i31 = 0; c2_i31 < 10; c2_i31++) {
    c2_b_u[c2_i31] = (*(real_T (*)[10])c2_inData)[c2_i31];
  }

  c2_b_y = NULL;
  sf_mex_assign(&c2_b_y, sf_mex_create("y", c2_b_u, 0, 0U, 1U, 0U, 2, 1, 10),
                false);
  sf_mex_assign(&c2_mxArrayOutData, c2_b_y, false);
  return c2_mxArrayOutData;
}

static void c2_p_emlrt_marshallIn(SFc2_solver_simulinkInstanceStruct
  *chartInstance, const mxArray *c2_b_forceout, const char_T *c2_identifier,
  real_T c2_b_y[10])
{
  emlrtMsgIdentifier c2_thisId;
  c2_thisId.fIdentifier = c2_identifier;
  c2_thisId.fParent = NULL;
  c2_thisId.bParentIsCell = false;
  c2_q_emlrt_marshallIn(chartInstance, sf_mex_dup(c2_b_forceout), &c2_thisId,
                        c2_b_y);
  sf_mex_destroy(&c2_b_forceout);
}

static void c2_q_emlrt_marshallIn(SFc2_solver_simulinkInstanceStruct
  *chartInstance, const mxArray *c2_b_u, const emlrtMsgIdentifier *c2_parentId,
  real_T c2_b_y[10])
{
  real_T c2_dv5[10];
  int32_T c2_i32;
  (void)chartInstance;
  sf_mex_import(c2_parentId, sf_mex_dup(c2_b_u), c2_dv5, 1, 0, 0U, 1, 0U, 2, 1,
                10);
  for (c2_i32 = 0; c2_i32 < 10; c2_i32++) {
    c2_b_y[c2_i32] = c2_dv5[c2_i32];
  }

  sf_mex_destroy(&c2_b_u);
}

static void c2_b_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c2_mxArrayInData, const char_T *c2_varName, void *c2_outData)
{
  const mxArray *c2_b_forceout;
  const char_T *c2_identifier;
  emlrtMsgIdentifier c2_thisId;
  real_T c2_b_y[10];
  int32_T c2_i33;
  SFc2_solver_simulinkInstanceStruct *chartInstance;
  chartInstance = (SFc2_solver_simulinkInstanceStruct *)chartInstanceVoid;
  c2_b_forceout = sf_mex_dup(c2_mxArrayInData);
  c2_identifier = c2_varName;
  c2_thisId.fIdentifier = c2_identifier;
  c2_thisId.fParent = NULL;
  c2_thisId.bParentIsCell = false;
  c2_q_emlrt_marshallIn(chartInstance, sf_mex_dup(c2_b_forceout), &c2_thisId,
                        c2_b_y);
  sf_mex_destroy(&c2_b_forceout);
  for (c2_i33 = 0; c2_i33 < 10; c2_i33++) {
    (*(real_T (*)[10])c2_outData)[c2_i33] = c2_b_y[c2_i33];
  }

  sf_mex_destroy(&c2_mxArrayInData);
}

static const mxArray *c2_c_sf_marshallOut(void *chartInstanceVoid, void
  *c2_inData)
{
  const mxArray *c2_mxArrayOutData;
  real_T c2_b_u;
  const mxArray *c2_b_y = NULL;
  SFc2_solver_simulinkInstanceStruct *chartInstance;
  chartInstance = (SFc2_solver_simulinkInstanceStruct *)chartInstanceVoid;
  c2_mxArrayOutData = NULL;
  c2_mxArrayOutData = NULL;
  c2_b_u = *(real_T *)c2_inData;
  c2_b_y = NULL;
  sf_mex_assign(&c2_b_y, sf_mex_create("y", &c2_b_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_assign(&c2_mxArrayOutData, c2_b_y, false);
  return c2_mxArrayOutData;
}

static const mxArray *c2_d_sf_marshallOut(void *chartInstanceVoid, void
  *c2_inData)
{
  const mxArray *c2_mxArrayOutData;
  int32_T c2_i34;
  int32_T c2_i35;
  const mxArray *c2_b_y = NULL;
  int32_T c2_i36;
  SFc2_solver_simulinkInstanceStruct *chartInstance;
  chartInstance = (SFc2_solver_simulinkInstanceStruct *)chartInstanceVoid;
  c2_mxArrayOutData = NULL;
  c2_mxArrayOutData = NULL;
  c2_i34 = 0;
  for (c2_i35 = 0; c2_i35 < 10; c2_i35++) {
    for (c2_i36 = 0; c2_i36 < 2501; c2_i36++) {
      chartInstance->c2_u[c2_i36 + c2_i34] = (*(real_T (*)[25010])c2_inData)
        [c2_i36 + c2_i34];
    }

    c2_i34 += 2501;
  }

  c2_b_y = NULL;
  sf_mex_assign(&c2_b_y, sf_mex_create("y", chartInstance->c2_u, 0, 0U, 1U, 0U,
    2, 2501, 10), false);
  sf_mex_assign(&c2_mxArrayOutData, c2_b_y, false);
  return c2_mxArrayOutData;
}

static const mxArray *c2_e_sf_marshallOut(void *chartInstanceVoid, void
  *c2_inData)
{
  const mxArray *c2_mxArrayOutData;
  int32_T c2_i37;
  int32_T c2_i38;
  const mxArray *c2_b_y = NULL;
  int32_T c2_i39;
  real_T c2_b_u[1256];
  SFc2_solver_simulinkInstanceStruct *chartInstance;
  chartInstance = (SFc2_solver_simulinkInstanceStruct *)chartInstanceVoid;
  c2_mxArrayOutData = NULL;
  c2_mxArrayOutData = NULL;
  c2_i37 = 0;
  for (c2_i38 = 0; c2_i38 < 2; c2_i38++) {
    for (c2_i39 = 0; c2_i39 < 628; c2_i39++) {
      c2_b_u[c2_i39 + c2_i37] = (*(real_T (*)[1256])c2_inData)[c2_i39 + c2_i37];
    }

    c2_i37 += 628;
  }

  c2_b_y = NULL;
  sf_mex_assign(&c2_b_y, sf_mex_create("y", c2_b_u, 0, 0U, 1U, 0U, 2, 628, 2),
                false);
  sf_mex_assign(&c2_mxArrayOutData, c2_b_y, false);
  return c2_mxArrayOutData;
}

static void c2_c_sf_marshallIn(void *chartInstanceVoid, const mxArray
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
  c2_b_y = c2_d_emlrt_marshallIn(chartInstance, sf_mex_dup(c2_nargout),
    &c2_thisId);
  sf_mex_destroy(&c2_nargout);
  *(real_T *)c2_outData = c2_b_y;
  sf_mex_destroy(&c2_mxArrayInData);
}

static const mxArray *c2_f_sf_marshallOut(void *chartInstanceVoid, void
  *c2_inData)
{
  const mxArray *c2_mxArrayOutData;
  c2_sv2voxIt8FKzjMlUEnyXmnC c2_b_u;
  const mxArray *c2_b_y = NULL;
  real_T c2_c_u;
  const mxArray *c2_c_y = NULL;
  real_T c2_d_u;
  const mxArray *c2_d_y = NULL;
  real_T c2_e_u;
  const mxArray *c2_e_y = NULL;
  c2_sof5eNcdcizKAe6jjm9xhtG c2_f_u;
  const mxArray *c2_f_y = NULL;
  real_T c2_g_u;
  const mxArray *c2_g_y = NULL;
  real_T c2_h_u;
  const mxArray *c2_h_y = NULL;
  int32_T c2_i40;
  const mxArray *c2_i_y = NULL;
  char_T c2_i_u[9];
  real_T c2_j_u;
  const mxArray *c2_j_y = NULL;
  c2_syx7FYmGk3I6e3U1RUwUAWD c2_k_u;
  const mxArray *c2_k_y = NULL;
  real_T c2_l_u;
  const mxArray *c2_l_y = NULL;
  c2_sqGsSc4djWypCV90lhUGiL c2_m_u;
  const mxArray *c2_m_y = NULL;
  real_T c2_n_u;
  const mxArray *c2_n_y = NULL;
  real_T c2_o_u;
  const mxArray *c2_o_y = NULL;
  c2_su2bTaGhbTlLQVXsqMBjEuH c2_p_u;
  const mxArray *c2_p_y = NULL;
  real_T c2_q_u;
  const mxArray *c2_q_y = NULL;
  real_T c2_r_u;
  const mxArray *c2_r_y = NULL;
  real_T c2_s_u;
  const mxArray *c2_s_y = NULL;
  c2_sNlKjRKFHwjFKC34GeUJivG c2_t_u;
  const mxArray *c2_t_y = NULL;
  real_T c2_u_u;
  const mxArray *c2_u_y = NULL;
  real_T c2_v_u;
  const mxArray *c2_v_y = NULL;
  real_T c2_w_u;
  const mxArray *c2_w_y = NULL;
  real_T c2_x_u;
  const mxArray *c2_x_y = NULL;
  real_T c2_y_u;
  const mxArray *c2_y_y = NULL;
  real_T c2_ab_u;
  const mxArray *c2_ab_y = NULL;
  real_T c2_bb_u;
  const mxArray *c2_bb_y = NULL;
  real_T c2_cb_u;
  const mxArray *c2_cb_y = NULL;
  real_T c2_db_u;
  const mxArray *c2_db_y = NULL;
  c2_sKS3cuSxR3HIcTIZZkbRPm c2_eb_u;
  const mxArray *c2_eb_y = NULL;
  real_T c2_fb_u;
  const mxArray *c2_fb_y = NULL;
  real_T c2_gb_u;
  const mxArray *c2_gb_y = NULL;
  real_T c2_hb_u;
  const mxArray *c2_hb_y = NULL;
  real_T c2_ib_u;
  const mxArray *c2_ib_y = NULL;
  c2_s88seaxzVTYYZIzTGqmP4AD c2_jb_u;
  const mxArray *c2_jb_y = NULL;
  real_T c2_kb_u;
  const mxArray *c2_kb_y = NULL;
  real_T c2_lb_u;
  const mxArray *c2_lb_y = NULL;
  real_T c2_mb_u;
  const mxArray *c2_mb_y = NULL;
  real_T c2_nb_u;
  const mxArray *c2_nb_y = NULL;
  c2_sfpFxKVMN8elArLoV353vAE c2_ob_u;
  const mxArray *c2_ob_y = NULL;
  real_T c2_pb_u;
  const mxArray *c2_pb_y = NULL;
  real_T c2_qb_u;
  const mxArray *c2_qb_y = NULL;
  SFc2_solver_simulinkInstanceStruct *chartInstance;
  chartInstance = (SFc2_solver_simulinkInstanceStruct *)chartInstanceVoid;
  c2_mxArrayOutData = NULL;
  c2_mxArrayOutData = NULL;
  c2_b_u = *(c2_sv2voxIt8FKzjMlUEnyXmnC *)c2_inData;
  c2_b_y = NULL;
  sf_mex_assign(&c2_b_y, sf_mex_createstruct("structure", 2, 1, 1), false);
  c2_c_u = c2_b_u.OD;
  c2_c_y = NULL;
  sf_mex_assign(&c2_c_y, sf_mex_create("y", &c2_c_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_b_y, c2_c_y, "OD", "OD", 0);
  c2_d_u = c2_b_u.drymass;
  c2_d_y = NULL;
  sf_mex_assign(&c2_d_y, sf_mex_create("y", &c2_d_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_b_y, c2_d_y, "drymass", "drymass", 0);
  c2_e_u = c2_b_u.wetmass;
  c2_e_y = NULL;
  sf_mex_assign(&c2_e_y, sf_mex_create("y", &c2_e_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_b_y, c2_e_y, "wetmass", "wetmass", 0);
  c2_f_u = c2_b_u.nose;
  c2_f_y = NULL;
  sf_mex_assign(&c2_f_y, sf_mex_createstruct("structure", 2, 1, 1), false);
  c2_g_u = c2_f_u.wetmass;
  c2_g_y = NULL;
  sf_mex_assign(&c2_g_y, sf_mex_create("y", &c2_g_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_f_y, c2_g_y, "wetmass", "wetmass", 0);
  c2_h_u = c2_f_u.drymass;
  c2_h_y = NULL;
  sf_mex_assign(&c2_h_y, sf_mex_create("y", &c2_h_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_f_y, c2_h_y, "drymass", "drymass", 0);
  for (c2_i40 = 0; c2_i40 < 9; c2_i40++) {
    c2_i_u[c2_i40] = c2_f_u.type[c2_i40];
  }

  c2_i_y = NULL;
  sf_mex_assign(&c2_i_y, sf_mex_create("y", c2_i_u, 10, 0U, 1U, 0U, 2, 1, 9),
                false);
  sf_mex_addfield(c2_f_y, c2_i_y, "type", "type", 0);
  c2_j_u = c2_f_u.l;
  c2_j_y = NULL;
  sf_mex_assign(&c2_j_y, sf_mex_create("y", &c2_j_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_f_y, c2_j_y, "l", "l", 0);
  sf_mex_addfield(c2_b_y, c2_f_y, "nose", "nose", 0);
  c2_k_u = c2_b_u.shoulder;
  c2_k_y = NULL;
  sf_mex_assign(&c2_k_y, sf_mex_createstruct("structure", 2, 1, 1), false);
  c2_l_u = c2_k_u.l;
  c2_l_y = NULL;
  sf_mex_assign(&c2_l_y, sf_mex_create("y", &c2_l_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_k_y, c2_l_y, "l", "l", 0);
  sf_mex_addfield(c2_b_y, c2_k_y, "shoulder", "shoulder", 0);
  c2_m_u = c2_b_u.fore;
  c2_m_y = NULL;
  sf_mex_assign(&c2_m_y, sf_mex_createstruct("structure", 2, 1, 1), false);
  c2_n_u = c2_m_u.l;
  c2_n_y = NULL;
  sf_mex_assign(&c2_n_y, sf_mex_create("y", &c2_n_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_m_y, c2_n_y, "l", "l", 0);
  c2_o_u = c2_m_u.S;
  c2_o_y = NULL;
  sf_mex_assign(&c2_o_y, sf_mex_create("y", &c2_o_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_m_y, c2_o_y, "S", "S", 0);
  sf_mex_addfield(c2_b_y, c2_m_y, "fore", "fore", 0);
  c2_p_u = c2_b_u.aft;
  c2_p_y = NULL;
  sf_mex_assign(&c2_p_y, sf_mex_createstruct("structure", 2, 1, 1), false);
  c2_q_u = c2_p_u.l;
  c2_q_y = NULL;
  sf_mex_assign(&c2_q_y, sf_mex_create("y", &c2_q_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_p_y, c2_q_y, "l", "l", 0);
  c2_r_u = c2_p_u.wetmass;
  c2_r_y = NULL;
  sf_mex_assign(&c2_r_y, sf_mex_create("y", &c2_r_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_p_y, c2_r_y, "wetmass", "wetmass", 0);
  c2_s_u = c2_p_u.drymass;
  c2_s_y = NULL;
  sf_mex_assign(&c2_s_y, sf_mex_create("y", &c2_s_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_p_y, c2_s_y, "drymass", "drymass", 0);
  sf_mex_addfield(c2_b_y, c2_p_y, "aft", "aft", 0);
  c2_t_u = c2_b_u.fin;
  c2_t_y = NULL;
  sf_mex_assign(&c2_t_y, sf_mex_createstruct("structure", 2, 1, 1), false);
  c2_u_u = c2_t_u.t;
  c2_u_y = NULL;
  sf_mex_assign(&c2_u_y, sf_mex_create("y", &c2_u_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_t_y, c2_u_y, "t", "t", 0);
  c2_v_u = c2_t_u.span;
  c2_v_y = NULL;
  sf_mex_assign(&c2_v_y, sf_mex_create("y", &c2_v_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_t_y, c2_v_y, "span", "span", 0);
  c2_w_u = c2_t_u.rootchord;
  c2_w_y = NULL;
  sf_mex_assign(&c2_w_y, sf_mex_create("y", &c2_w_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_t_y, c2_w_y, "rootchord", "rootchord", 0);
  c2_x_u = c2_t_u.tipchord;
  c2_x_y = NULL;
  sf_mex_assign(&c2_x_y, sf_mex_create("y", &c2_x_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_t_y, c2_x_y, "tipchord", "tipchord", 0);
  c2_y_u = c2_t_u.sweep;
  c2_y_y = NULL;
  sf_mex_assign(&c2_y_y, sf_mex_create("y", &c2_y_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_t_y, c2_y_y, "sweep", "sweep", 0);
  c2_ab_u = c2_t_u.S;
  c2_ab_y = NULL;
  sf_mex_assign(&c2_ab_y, sf_mex_create("y", &c2_ab_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_t_y, c2_ab_y, "S", "S", 0);
  c2_bb_u = c2_t_u.AR;
  c2_bb_y = NULL;
  sf_mex_assign(&c2_bb_y, sf_mex_create("y", &c2_bb_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_t_y, c2_bb_y, "AR", "AR", 0);
  c2_cb_u = c2_t_u.num;
  c2_cb_y = NULL;
  sf_mex_assign(&c2_cb_y, sf_mex_create("y", &c2_cb_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_t_y, c2_cb_y, "num", "num", 0);
  sf_mex_addfield(c2_b_y, c2_t_y, "fin", "fin", 0);
  c2_db_u = c2_b_u.l;
  c2_db_y = NULL;
  sf_mex_assign(&c2_db_y, sf_mex_create("y", &c2_db_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_b_y, c2_db_y, "l", "l", 0);
  c2_eb_u = c2_b_u.drogue;
  c2_eb_y = NULL;
  sf_mex_assign(&c2_eb_y, sf_mex_createstruct("structure", 2, 1, 1), false);
  c2_fb_u = c2_eb_u.d;
  c2_fb_y = NULL;
  sf_mex_assign(&c2_fb_y, sf_mex_create("y", &c2_fb_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_eb_y, c2_fb_y, "d", "d", 0);
  c2_gb_u = c2_eb_u.S;
  c2_gb_y = NULL;
  sf_mex_assign(&c2_gb_y, sf_mex_create("y", &c2_gb_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_eb_y, c2_gb_y, "S", "S", 0);
  c2_hb_u = c2_eb_u.Cd;
  c2_hb_y = NULL;
  sf_mex_assign(&c2_hb_y, sf_mex_create("y", &c2_hb_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_eb_y, c2_hb_y, "Cd", "Cd", 0);
  c2_ib_u = c2_eb_u.deploy_u;
  c2_ib_y = NULL;
  sf_mex_assign(&c2_ib_y, sf_mex_create("y", &c2_ib_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_eb_y, c2_ib_y, "deploy_u", "deploy_u", 0);
  sf_mex_addfield(c2_b_y, c2_eb_y, "drogue", "drogue", 0);
  c2_jb_u = c2_b_u.main;
  c2_jb_y = NULL;
  sf_mex_assign(&c2_jb_y, sf_mex_createstruct("structure", 2, 1, 1), false);
  c2_kb_u = c2_jb_u.d;
  c2_kb_y = NULL;
  sf_mex_assign(&c2_kb_y, sf_mex_create("y", &c2_kb_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_jb_y, c2_kb_y, "d", "d", 0);
  c2_lb_u = c2_jb_u.S;
  c2_lb_y = NULL;
  sf_mex_assign(&c2_lb_y, sf_mex_create("y", &c2_lb_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_jb_y, c2_lb_y, "S", "S", 0);
  c2_mb_u = c2_jb_u.Cd;
  c2_mb_y = NULL;
  sf_mex_assign(&c2_mb_y, sf_mex_create("y", &c2_mb_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_jb_y, c2_mb_y, "Cd", "Cd", 0);
  c2_nb_u = c2_jb_u.deploy_h;
  c2_nb_y = NULL;
  sf_mex_assign(&c2_nb_y, sf_mex_create("y", &c2_nb_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_jb_y, c2_nb_y, "deploy_h", "deploy_h", 0);
  sf_mex_addfield(c2_b_y, c2_jb_y, "main", "main", 0);
  c2_ob_u = c2_b_u.payload;
  c2_ob_y = NULL;
  sf_mex_assign(&c2_ob_y, sf_mex_createstruct("structure", 2, 1, 1), false);
  c2_pb_u = c2_ob_u.chute_d;
  c2_pb_y = NULL;
  sf_mex_assign(&c2_pb_y, sf_mex_create("y", &c2_pb_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_ob_y, c2_pb_y, "chute_d", "chute_d", 0);
  c2_qb_u = c2_ob_u.chute_Cd;
  c2_qb_y = NULL;
  sf_mex_assign(&c2_qb_y, sf_mex_create("y", &c2_qb_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_ob_y, c2_qb_y, "chute_Cd", "chute_Cd", 0);
  sf_mex_addfield(c2_b_y, c2_ob_y, "payload", "payload", 0);
  sf_mex_assign(&c2_mxArrayOutData, c2_b_y, false);
  return c2_mxArrayOutData;
}

static void c2_d_sf_marshallIn(void *chartInstanceVoid, const mxArray
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
  c2_c_emlrt_marshallIn(chartInstance, sf_mex_dup(c2_rocket), &c2_thisId,
                        &c2_b_y);
  sf_mex_destroy(&c2_rocket);
  *(c2_sv2voxIt8FKzjMlUEnyXmnC *)c2_outData = c2_b_y;
  sf_mex_destroy(&c2_mxArrayInData);
}

static const mxArray *c2_g_sf_marshallOut(void *chartInstanceVoid, void
  *c2_inData)
{
  const mxArray *c2_mxArrayOutData;
  c2_sOKxARu9Dkm6wShDKKOBVjE c2_b_u;
  const mxArray *c2_b_y = NULL;
  c2_sv2voxIt8FKzjMlUEnyXmnC c2_c_u;
  const mxArray *c2_c_y = NULL;
  real_T c2_d_u;
  const mxArray *c2_d_y = NULL;
  real_T c2_e_u;
  const mxArray *c2_e_y = NULL;
  real_T c2_f_u;
  const mxArray *c2_f_y = NULL;
  c2_sof5eNcdcizKAe6jjm9xhtG c2_g_u;
  const mxArray *c2_g_y = NULL;
  real_T c2_h_u;
  const mxArray *c2_h_y = NULL;
  real_T c2_i_u;
  const mxArray *c2_i_y = NULL;
  int32_T c2_i41;
  const mxArray *c2_j_y = NULL;
  char_T c2_j_u[9];
  real_T c2_k_u;
  const mxArray *c2_k_y = NULL;
  c2_syx7FYmGk3I6e3U1RUwUAWD c2_l_u;
  const mxArray *c2_l_y = NULL;
  real_T c2_m_u;
  const mxArray *c2_m_y = NULL;
  c2_sqGsSc4djWypCV90lhUGiL c2_n_u;
  const mxArray *c2_n_y = NULL;
  real_T c2_o_u;
  const mxArray *c2_o_y = NULL;
  real_T c2_p_u;
  const mxArray *c2_p_y = NULL;
  c2_su2bTaGhbTlLQVXsqMBjEuH c2_q_u;
  const mxArray *c2_q_y = NULL;
  real_T c2_r_u;
  const mxArray *c2_r_y = NULL;
  real_T c2_s_u;
  const mxArray *c2_s_y = NULL;
  real_T c2_t_u;
  const mxArray *c2_t_y = NULL;
  c2_sNlKjRKFHwjFKC34GeUJivG c2_u_u;
  const mxArray *c2_u_y = NULL;
  real_T c2_v_u;
  const mxArray *c2_v_y = NULL;
  real_T c2_w_u;
  const mxArray *c2_w_y = NULL;
  real_T c2_x_u;
  const mxArray *c2_x_y = NULL;
  real_T c2_y_u;
  const mxArray *c2_y_y = NULL;
  real_T c2_ab_u;
  const mxArray *c2_ab_y = NULL;
  real_T c2_bb_u;
  const mxArray *c2_bb_y = NULL;
  real_T c2_cb_u;
  const mxArray *c2_cb_y = NULL;
  real_T c2_db_u;
  const mxArray *c2_db_y = NULL;
  real_T c2_eb_u;
  const mxArray *c2_eb_y = NULL;
  c2_sKS3cuSxR3HIcTIZZkbRPm c2_fb_u;
  const mxArray *c2_fb_y = NULL;
  real_T c2_gb_u;
  const mxArray *c2_gb_y = NULL;
  real_T c2_hb_u;
  const mxArray *c2_hb_y = NULL;
  real_T c2_ib_u;
  const mxArray *c2_ib_y = NULL;
  real_T c2_jb_u;
  const mxArray *c2_jb_y = NULL;
  c2_s88seaxzVTYYZIzTGqmP4AD c2_kb_u;
  const mxArray *c2_kb_y = NULL;
  real_T c2_lb_u;
  const mxArray *c2_lb_y = NULL;
  real_T c2_mb_u;
  const mxArray *c2_mb_y = NULL;
  real_T c2_nb_u;
  const mxArray *c2_nb_y = NULL;
  real_T c2_ob_u;
  const mxArray *c2_ob_y = NULL;
  c2_sfpFxKVMN8elArLoV353vAE c2_pb_u;
  const mxArray *c2_pb_y = NULL;
  real_T c2_qb_u;
  const mxArray *c2_qb_y = NULL;
  real_T c2_rb_u;
  const mxArray *c2_rb_y = NULL;
  SFc2_solver_simulinkInstanceStruct *chartInstance;
  chartInstance = (SFc2_solver_simulinkInstanceStruct *)chartInstanceVoid;
  c2_mxArrayOutData = NULL;
  c2_mxArrayOutData = NULL;
  c2_b_u = *(c2_sOKxARu9Dkm6wShDKKOBVjE *)c2_inData;
  c2_b_y = NULL;
  sf_mex_assign(&c2_b_y, sf_mex_createstruct("structure", 2, 1, 1), false);
  c2_c_u = c2_b_u.rocket;
  c2_c_y = NULL;
  sf_mex_assign(&c2_c_y, sf_mex_createstruct("structure", 2, 1, 1), false);
  c2_d_u = c2_c_u.OD;
  c2_d_y = NULL;
  sf_mex_assign(&c2_d_y, sf_mex_create("y", &c2_d_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_c_y, c2_d_y, "OD", "OD", 0);
  c2_e_u = c2_c_u.drymass;
  c2_e_y = NULL;
  sf_mex_assign(&c2_e_y, sf_mex_create("y", &c2_e_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_c_y, c2_e_y, "drymass", "drymass", 0);
  c2_f_u = c2_c_u.wetmass;
  c2_f_y = NULL;
  sf_mex_assign(&c2_f_y, sf_mex_create("y", &c2_f_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_c_y, c2_f_y, "wetmass", "wetmass", 0);
  c2_g_u = c2_c_u.nose;
  c2_g_y = NULL;
  sf_mex_assign(&c2_g_y, sf_mex_createstruct("structure", 2, 1, 1), false);
  c2_h_u = c2_g_u.wetmass;
  c2_h_y = NULL;
  sf_mex_assign(&c2_h_y, sf_mex_create("y", &c2_h_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_g_y, c2_h_y, "wetmass", "wetmass", 0);
  c2_i_u = c2_g_u.drymass;
  c2_i_y = NULL;
  sf_mex_assign(&c2_i_y, sf_mex_create("y", &c2_i_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_g_y, c2_i_y, "drymass", "drymass", 0);
  for (c2_i41 = 0; c2_i41 < 9; c2_i41++) {
    c2_j_u[c2_i41] = c2_g_u.type[c2_i41];
  }

  c2_j_y = NULL;
  sf_mex_assign(&c2_j_y, sf_mex_create("y", c2_j_u, 10, 0U, 1U, 0U, 2, 1, 9),
                false);
  sf_mex_addfield(c2_g_y, c2_j_y, "type", "type", 0);
  c2_k_u = c2_g_u.l;
  c2_k_y = NULL;
  sf_mex_assign(&c2_k_y, sf_mex_create("y", &c2_k_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_g_y, c2_k_y, "l", "l", 0);
  sf_mex_addfield(c2_c_y, c2_g_y, "nose", "nose", 0);
  c2_l_u = c2_c_u.shoulder;
  c2_l_y = NULL;
  sf_mex_assign(&c2_l_y, sf_mex_createstruct("structure", 2, 1, 1), false);
  c2_m_u = c2_l_u.l;
  c2_m_y = NULL;
  sf_mex_assign(&c2_m_y, sf_mex_create("y", &c2_m_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_l_y, c2_m_y, "l", "l", 0);
  sf_mex_addfield(c2_c_y, c2_l_y, "shoulder", "shoulder", 0);
  c2_n_u = c2_c_u.fore;
  c2_n_y = NULL;
  sf_mex_assign(&c2_n_y, sf_mex_createstruct("structure", 2, 1, 1), false);
  c2_o_u = c2_n_u.l;
  c2_o_y = NULL;
  sf_mex_assign(&c2_o_y, sf_mex_create("y", &c2_o_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_n_y, c2_o_y, "l", "l", 0);
  c2_p_u = c2_n_u.S;
  c2_p_y = NULL;
  sf_mex_assign(&c2_p_y, sf_mex_create("y", &c2_p_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_n_y, c2_p_y, "S", "S", 0);
  sf_mex_addfield(c2_c_y, c2_n_y, "fore", "fore", 0);
  c2_q_u = c2_c_u.aft;
  c2_q_y = NULL;
  sf_mex_assign(&c2_q_y, sf_mex_createstruct("structure", 2, 1, 1), false);
  c2_r_u = c2_q_u.l;
  c2_r_y = NULL;
  sf_mex_assign(&c2_r_y, sf_mex_create("y", &c2_r_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_q_y, c2_r_y, "l", "l", 0);
  c2_s_u = c2_q_u.wetmass;
  c2_s_y = NULL;
  sf_mex_assign(&c2_s_y, sf_mex_create("y", &c2_s_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_q_y, c2_s_y, "wetmass", "wetmass", 0);
  c2_t_u = c2_q_u.drymass;
  c2_t_y = NULL;
  sf_mex_assign(&c2_t_y, sf_mex_create("y", &c2_t_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_q_y, c2_t_y, "drymass", "drymass", 0);
  sf_mex_addfield(c2_c_y, c2_q_y, "aft", "aft", 0);
  c2_u_u = c2_c_u.fin;
  c2_u_y = NULL;
  sf_mex_assign(&c2_u_y, sf_mex_createstruct("structure", 2, 1, 1), false);
  c2_v_u = c2_u_u.t;
  c2_v_y = NULL;
  sf_mex_assign(&c2_v_y, sf_mex_create("y", &c2_v_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_u_y, c2_v_y, "t", "t", 0);
  c2_w_u = c2_u_u.span;
  c2_w_y = NULL;
  sf_mex_assign(&c2_w_y, sf_mex_create("y", &c2_w_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_u_y, c2_w_y, "span", "span", 0);
  c2_x_u = c2_u_u.rootchord;
  c2_x_y = NULL;
  sf_mex_assign(&c2_x_y, sf_mex_create("y", &c2_x_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_u_y, c2_x_y, "rootchord", "rootchord", 0);
  c2_y_u = c2_u_u.tipchord;
  c2_y_y = NULL;
  sf_mex_assign(&c2_y_y, sf_mex_create("y", &c2_y_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_u_y, c2_y_y, "tipchord", "tipchord", 0);
  c2_ab_u = c2_u_u.sweep;
  c2_ab_y = NULL;
  sf_mex_assign(&c2_ab_y, sf_mex_create("y", &c2_ab_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_u_y, c2_ab_y, "sweep", "sweep", 0);
  c2_bb_u = c2_u_u.S;
  c2_bb_y = NULL;
  sf_mex_assign(&c2_bb_y, sf_mex_create("y", &c2_bb_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_u_y, c2_bb_y, "S", "S", 0);
  c2_cb_u = c2_u_u.AR;
  c2_cb_y = NULL;
  sf_mex_assign(&c2_cb_y, sf_mex_create("y", &c2_cb_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_u_y, c2_cb_y, "AR", "AR", 0);
  c2_db_u = c2_u_u.num;
  c2_db_y = NULL;
  sf_mex_assign(&c2_db_y, sf_mex_create("y", &c2_db_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_u_y, c2_db_y, "num", "num", 0);
  sf_mex_addfield(c2_c_y, c2_u_y, "fin", "fin", 0);
  c2_eb_u = c2_c_u.l;
  c2_eb_y = NULL;
  sf_mex_assign(&c2_eb_y, sf_mex_create("y", &c2_eb_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_c_y, c2_eb_y, "l", "l", 0);
  c2_fb_u = c2_c_u.drogue;
  c2_fb_y = NULL;
  sf_mex_assign(&c2_fb_y, sf_mex_createstruct("structure", 2, 1, 1), false);
  c2_gb_u = c2_fb_u.d;
  c2_gb_y = NULL;
  sf_mex_assign(&c2_gb_y, sf_mex_create("y", &c2_gb_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_fb_y, c2_gb_y, "d", "d", 0);
  c2_hb_u = c2_fb_u.S;
  c2_hb_y = NULL;
  sf_mex_assign(&c2_hb_y, sf_mex_create("y", &c2_hb_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_fb_y, c2_hb_y, "S", "S", 0);
  c2_ib_u = c2_fb_u.Cd;
  c2_ib_y = NULL;
  sf_mex_assign(&c2_ib_y, sf_mex_create("y", &c2_ib_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_fb_y, c2_ib_y, "Cd", "Cd", 0);
  c2_jb_u = c2_fb_u.deploy_u;
  c2_jb_y = NULL;
  sf_mex_assign(&c2_jb_y, sf_mex_create("y", &c2_jb_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_fb_y, c2_jb_y, "deploy_u", "deploy_u", 0);
  sf_mex_addfield(c2_c_y, c2_fb_y, "drogue", "drogue", 0);
  c2_kb_u = c2_c_u.main;
  c2_kb_y = NULL;
  sf_mex_assign(&c2_kb_y, sf_mex_createstruct("structure", 2, 1, 1), false);
  c2_lb_u = c2_kb_u.d;
  c2_lb_y = NULL;
  sf_mex_assign(&c2_lb_y, sf_mex_create("y", &c2_lb_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_kb_y, c2_lb_y, "d", "d", 0);
  c2_mb_u = c2_kb_u.S;
  c2_mb_y = NULL;
  sf_mex_assign(&c2_mb_y, sf_mex_create("y", &c2_mb_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_kb_y, c2_mb_y, "S", "S", 0);
  c2_nb_u = c2_kb_u.Cd;
  c2_nb_y = NULL;
  sf_mex_assign(&c2_nb_y, sf_mex_create("y", &c2_nb_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_kb_y, c2_nb_y, "Cd", "Cd", 0);
  c2_ob_u = c2_kb_u.deploy_h;
  c2_ob_y = NULL;
  sf_mex_assign(&c2_ob_y, sf_mex_create("y", &c2_ob_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_kb_y, c2_ob_y, "deploy_h", "deploy_h", 0);
  sf_mex_addfield(c2_c_y, c2_kb_y, "main", "main", 0);
  c2_pb_u = c2_c_u.payload;
  c2_pb_y = NULL;
  sf_mex_assign(&c2_pb_y, sf_mex_createstruct("structure", 2, 1, 1), false);
  c2_qb_u = c2_pb_u.chute_d;
  c2_qb_y = NULL;
  sf_mex_assign(&c2_qb_y, sf_mex_create("y", &c2_qb_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_pb_y, c2_qb_y, "chute_d", "chute_d", 0);
  c2_rb_u = c2_pb_u.chute_Cd;
  c2_rb_y = NULL;
  sf_mex_assign(&c2_rb_y, sf_mex_create("y", &c2_rb_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c2_pb_y, c2_rb_y, "chute_Cd", "chute_Cd", 0);
  sf_mex_addfield(c2_c_y, c2_pb_y, "payload", "payload", 0);
  sf_mex_addfield(c2_b_y, c2_c_y, "rocket", "rocket", 0);
  sf_mex_assign(&c2_mxArrayOutData, c2_b_y, false);
  return c2_mxArrayOutData;
}

static void c2_e_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c2_mxArrayInData, const char_T *c2_varName, void *c2_outData)
{
  const mxArray *c2_load;
  const char_T *c2_identifier;
  emlrtMsgIdentifier c2_thisId;
  c2_sOKxARu9Dkm6wShDKKOBVjE c2_b_y;
  SFc2_solver_simulinkInstanceStruct *chartInstance;
  chartInstance = (SFc2_solver_simulinkInstanceStruct *)chartInstanceVoid;
  c2_load = sf_mex_dup(c2_mxArrayInData);
  c2_identifier = c2_varName;
  c2_thisId.fIdentifier = c2_identifier;
  c2_thisId.fParent = NULL;
  c2_thisId.bParentIsCell = false;
  c2_b_emlrt_marshallIn(chartInstance, sf_mex_dup(c2_load), &c2_thisId, &c2_b_y);
  sf_mex_destroy(&c2_load);
  *(c2_sOKxARu9Dkm6wShDKKOBVjE *)c2_outData = c2_b_y;
  sf_mex_destroy(&c2_mxArrayInData);
}

static const mxArray *c2_h_sf_marshallOut(void *chartInstanceVoid, void
  *c2_inData)
{
  const mxArray *c2_mxArrayOutData;
  int32_T c2_i42;
  const mxArray *c2_b_y = NULL;
  real_T c2_b_u[7];
  SFc2_solver_simulinkInstanceStruct *chartInstance;
  chartInstance = (SFc2_solver_simulinkInstanceStruct *)chartInstanceVoid;
  c2_mxArrayOutData = NULL;
  c2_mxArrayOutData = NULL;
  for (c2_i42 = 0; c2_i42 < 7; c2_i42++) {
    c2_b_u[c2_i42] = (*(real_T (*)[7])c2_inData)[c2_i42];
  }

  c2_b_y = NULL;
  sf_mex_assign(&c2_b_y, sf_mex_create("y", c2_b_u, 0, 0U, 1U, 0U, 2, 1, 7),
                false);
  sf_mex_assign(&c2_mxArrayOutData, c2_b_y, false);
  return c2_mxArrayOutData;
}

static void c2_r_emlrt_marshallIn(SFc2_solver_simulinkInstanceStruct
  *chartInstance, const mxArray *c2_b_u, const emlrtMsgIdentifier *c2_parentId,
  real_T c2_b_y[7])
{
  real_T c2_dv6[7];
  int32_T c2_i43;
  (void)chartInstance;
  sf_mex_import(c2_parentId, sf_mex_dup(c2_b_u), c2_dv6, 1, 0, 0U, 1, 0U, 2, 1,
                7);
  for (c2_i43 = 0; c2_i43 < 7; c2_i43++) {
    c2_b_y[c2_i43] = c2_dv6[c2_i43];
  }

  sf_mex_destroy(&c2_b_u);
}

static void c2_f_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c2_mxArrayInData, const char_T *c2_varName, void *c2_outData)
{
  const mxArray *c2_hpos;
  const char_T *c2_identifier;
  emlrtMsgIdentifier c2_thisId;
  real_T c2_b_y[7];
  int32_T c2_i44;
  SFc2_solver_simulinkInstanceStruct *chartInstance;
  chartInstance = (SFc2_solver_simulinkInstanceStruct *)chartInstanceVoid;
  c2_hpos = sf_mex_dup(c2_mxArrayInData);
  c2_identifier = c2_varName;
  c2_thisId.fIdentifier = c2_identifier;
  c2_thisId.fParent = NULL;
  c2_thisId.bParentIsCell = false;
  c2_r_emlrt_marshallIn(chartInstance, sf_mex_dup(c2_hpos), &c2_thisId, c2_b_y);
  sf_mex_destroy(&c2_hpos);
  for (c2_i44 = 0; c2_i44 < 7; c2_i44++) {
    (*(real_T (*)[7])c2_outData)[c2_i44] = c2_b_y[c2_i44];
  }

  sf_mex_destroy(&c2_mxArrayInData);
}

static const mxArray *c2_i_sf_marshallOut(void *chartInstanceVoid, void
  *c2_inData)
{
  const mxArray *c2_mxArrayOutData;
  int32_T c2_i45;
  const mxArray *c2_b_y = NULL;
  real_T c2_b_u[8];
  SFc2_solver_simulinkInstanceStruct *chartInstance;
  chartInstance = (SFc2_solver_simulinkInstanceStruct *)chartInstanceVoid;
  c2_mxArrayOutData = NULL;
  c2_mxArrayOutData = NULL;
  for (c2_i45 = 0; c2_i45 < 8; c2_i45++) {
    c2_b_u[c2_i45] = (*(real_T (*)[8])c2_inData)[c2_i45];
  }

  c2_b_y = NULL;
  sf_mex_assign(&c2_b_y, sf_mex_create("y", c2_b_u, 0, 0U, 1U, 0U, 2, 1, 8),
                false);
  sf_mex_assign(&c2_mxArrayOutData, c2_b_y, false);
  return c2_mxArrayOutData;
}

static void c2_s_emlrt_marshallIn(SFc2_solver_simulinkInstanceStruct
  *chartInstance, const mxArray *c2_b_u, const emlrtMsgIdentifier *c2_parentId,
  real_T c2_b_y[8])
{
  real_T c2_dv7[8];
  int32_T c2_i46;
  (void)chartInstance;
  sf_mex_import(c2_parentId, sf_mex_dup(c2_b_u), c2_dv7, 1, 0, 0U, 1, 0U, 2, 1,
                8);
  for (c2_i46 = 0; c2_i46 < 8; c2_i46++) {
    c2_b_y[c2_i46] = c2_dv7[c2_i46];
  }

  sf_mex_destroy(&c2_b_u);
}

static void c2_g_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c2_mxArrayInData, const char_T *c2_varName, void *c2_outData)
{
  const mxArray *c2_b_T0;
  const char_T *c2_identifier;
  emlrtMsgIdentifier c2_thisId;
  real_T c2_b_y[8];
  int32_T c2_i47;
  SFc2_solver_simulinkInstanceStruct *chartInstance;
  chartInstance = (SFc2_solver_simulinkInstanceStruct *)chartInstanceVoid;
  c2_b_T0 = sf_mex_dup(c2_mxArrayInData);
  c2_identifier = c2_varName;
  c2_thisId.fIdentifier = c2_identifier;
  c2_thisId.fParent = NULL;
  c2_thisId.bParentIsCell = false;
  c2_s_emlrt_marshallIn(chartInstance, sf_mex_dup(c2_b_T0), &c2_thisId, c2_b_y);
  sf_mex_destroy(&c2_b_T0);
  for (c2_i47 = 0; c2_i47 < 8; c2_i47++) {
    (*(real_T (*)[8])c2_outData)[c2_i47] = c2_b_y[c2_i47];
  }

  sf_mex_destroy(&c2_mxArrayInData);
}

static void c2_t_emlrt_marshallIn(SFc2_solver_simulinkInstanceStruct
  *chartInstance, const mxArray *c2_b_u, const emlrtMsgIdentifier *c2_parentId,
  real_T c2_b_y[25010])
{
  int32_T c2_i48;
  sf_mex_import(c2_parentId, sf_mex_dup(c2_b_u), chartInstance->c2_dv8, 1, 0, 0U,
                1, 0U, 2, 2501, 10);
  for (c2_i48 = 0; c2_i48 < 25010; c2_i48++) {
    c2_b_y[c2_i48] = chartInstance->c2_dv8[c2_i48];
  }

  sf_mex_destroy(&c2_b_u);
}

static void c2_h_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c2_mxArrayInData, const char_T *c2_varName, void *c2_outData)
{
  const mxArray *c2_e_aerodata;
  const char_T *c2_identifier;
  emlrtMsgIdentifier c2_thisId;
  int32_T c2_i49;
  int32_T c2_i50;
  int32_T c2_i51;
  SFc2_solver_simulinkInstanceStruct *chartInstance;
  chartInstance = (SFc2_solver_simulinkInstanceStruct *)chartInstanceVoid;
  c2_e_aerodata = sf_mex_dup(c2_mxArrayInData);
  c2_identifier = c2_varName;
  c2_thisId.fIdentifier = c2_identifier;
  c2_thisId.fParent = NULL;
  c2_thisId.bParentIsCell = false;
  c2_t_emlrt_marshallIn(chartInstance, sf_mex_dup(c2_e_aerodata), &c2_thisId,
                        chartInstance->c2_y);
  sf_mex_destroy(&c2_e_aerodata);
  c2_i49 = 0;
  for (c2_i50 = 0; c2_i50 < 10; c2_i50++) {
    for (c2_i51 = 0; c2_i51 < 2501; c2_i51++) {
      (*(real_T (*)[25010])c2_outData)[c2_i51 + c2_i49] = chartInstance->
        c2_y[c2_i51 + c2_i49];
    }

    c2_i49 += 2501;
  }

  sf_mex_destroy(&c2_mxArrayInData);
}

static const mxArray *c2_j_sf_marshallOut(void *chartInstanceVoid, void
  *c2_inData)
{
  const mxArray *c2_mxArrayOutData;
  int32_T c2_i52;
  const mxArray *c2_b_y = NULL;
  real_T c2_b_u[3];
  SFc2_solver_simulinkInstanceStruct *chartInstance;
  chartInstance = (SFc2_solver_simulinkInstanceStruct *)chartInstanceVoid;
  c2_mxArrayOutData = NULL;
  c2_mxArrayOutData = NULL;
  for (c2_i52 = 0; c2_i52 < 3; c2_i52++) {
    c2_b_u[c2_i52] = (*(real_T (*)[3])c2_inData)[c2_i52];
  }

  c2_b_y = NULL;
  sf_mex_assign(&c2_b_y, sf_mex_create("y", c2_b_u, 0, 0U, 1U, 0U, 1, 3), false);
  sf_mex_assign(&c2_mxArrayOutData, c2_b_y, false);
  return c2_mxArrayOutData;
}

static void c2_u_emlrt_marshallIn(SFc2_solver_simulinkInstanceStruct
  *chartInstance, const mxArray *c2_b_u, const emlrtMsgIdentifier *c2_parentId,
  real_T c2_b_y[3])
{
  real_T c2_dv9[3];
  int32_T c2_i53;
  (void)chartInstance;
  sf_mex_import(c2_parentId, sf_mex_dup(c2_b_u), c2_dv9, 1, 0, 0U, 1, 0U, 1, 3);
  for (c2_i53 = 0; c2_i53 < 3; c2_i53++) {
    c2_b_y[c2_i53] = c2_dv9[c2_i53];
  }

  sf_mex_destroy(&c2_b_u);
}

static void c2_i_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c2_mxArrayInData, const char_T *c2_varName, void *c2_outData)
{
  const mxArray *c2_b_u;
  const char_T *c2_identifier;
  emlrtMsgIdentifier c2_thisId;
  real_T c2_b_y[3];
  int32_T c2_i54;
  SFc2_solver_simulinkInstanceStruct *chartInstance;
  chartInstance = (SFc2_solver_simulinkInstanceStruct *)chartInstanceVoid;
  c2_b_u = sf_mex_dup(c2_mxArrayInData);
  c2_identifier = c2_varName;
  c2_thisId.fIdentifier = c2_identifier;
  c2_thisId.fParent = NULL;
  c2_thisId.bParentIsCell = false;
  c2_u_emlrt_marshallIn(chartInstance, sf_mex_dup(c2_b_u), &c2_thisId, c2_b_y);
  sf_mex_destroy(&c2_b_u);
  for (c2_i54 = 0; c2_i54 < 3; c2_i54++) {
    (*(real_T (*)[3])c2_outData)[c2_i54] = c2_b_y[c2_i54];
  }

  sf_mex_destroy(&c2_mxArrayInData);
}

static void c2_v_emlrt_marshallIn(SFc2_solver_simulinkInstanceStruct
  *chartInstance, const mxArray *c2_b_u, const emlrtMsgIdentifier *c2_parentId,
  real_T c2_b_y[1256])
{
  real_T c2_dv10[1256];
  int32_T c2_i55;
  (void)chartInstance;
  sf_mex_import(c2_parentId, sf_mex_dup(c2_b_u), c2_dv10, 1, 0, 0U, 1, 0U, 2,
                628, 2);
  for (c2_i55 = 0; c2_i55 < 1256; c2_i55++) {
    c2_b_y[c2_i55] = c2_dv10[c2_i55];
  }

  sf_mex_destroy(&c2_b_u);
}

static void c2_j_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c2_mxArrayInData, const char_T *c2_varName, void *c2_outData)
{
  const mxArray *c2_b_T;
  const char_T *c2_identifier;
  emlrtMsgIdentifier c2_thisId;
  real_T c2_b_y[1256];
  int32_T c2_i56;
  int32_T c2_i57;
  int32_T c2_i58;
  SFc2_solver_simulinkInstanceStruct *chartInstance;
  chartInstance = (SFc2_solver_simulinkInstanceStruct *)chartInstanceVoid;
  c2_b_T = sf_mex_dup(c2_mxArrayInData);
  c2_identifier = c2_varName;
  c2_thisId.fIdentifier = c2_identifier;
  c2_thisId.fParent = NULL;
  c2_thisId.bParentIsCell = false;
  c2_v_emlrt_marshallIn(chartInstance, sf_mex_dup(c2_b_T), &c2_thisId, c2_b_y);
  sf_mex_destroy(&c2_b_T);
  c2_i56 = 0;
  for (c2_i57 = 0; c2_i57 < 2; c2_i57++) {
    for (c2_i58 = 0; c2_i58 < 628; c2_i58++) {
      (*(real_T (*)[1256])c2_outData)[c2_i58 + c2_i56] = c2_b_y[c2_i58 + c2_i56];
    }

    c2_i56 += 628;
  }

  sf_mex_destroy(&c2_mxArrayInData);
}

const mxArray *sf_c2_solver_simulink_get_eml_resolved_functions_info(void)
{
  const mxArray *c2_nameCaptureInfo = NULL;
  const char * c2_data[5] = {
    "789ce555c16ed340105dd312b5082ab870e6d84b7109a94a39a55889d39250923809525555ce7a82b7f57adbf5ba4d72eab13df14b88039fc0f7b08ee3b03211"
    "464949913ad268fd3c7ee399b73b36d2f66a48da9a74f7114239b9ae487f80627b38c69af4a5f11adf5f464fc6f88b74cc7c017d11077d9b024acc6194f8b62f",
    "acc119200e01f32ec019457ac4038b50a83205548804b4ac8426200a45d7860bf8b41952c4dd60f21ae4a960d4cf26fad5cf72aa9fc4927e9e2abc377fe069e3"
    "98ca8b708f710cc1ecfc40f0108b987f95c1afa6f8113e2c1d196ff556003cd06ba557f9d7ba49841b7675cef02988608370c0bac5ed13c082f1c1719350f9a4",
    "2d3cbbfba219ed09d7e31e5ed2693ae4a6d4a12975ac4e742dde7c377feccece9fbe0f77c1af64ecc3f3d43e44d810050bb6da4323ac7f6c59e54f3ef40f3e18"
    "71be75259f36251f52d6599ebfcaa87751e7a69851c7e3541d11b6813367203f1c0407499e79e7e83a83df48f11bb7a487da4bacca3ce7f1ab799fe7a9068dfd",
    "7397b2bd70d3cc0fcaf556b5b3537ab79879fadfce4f967ecf52f544f833885d4199c17c8708c2fce41f39eb7c392cec7af077fa7452fcce2de9f35b4f52a439"
    "e6ebdb7d9eafe67b6e192715b3603bb850185aed6de7f2a0fcefe7eb27f62702a7", "" };

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

static real_T c2_asind(SFc2_solver_simulinkInstanceStruct *chartInstance, real_T
  c2_x)
{
  real_T c2_b_x;
  c2_b_x = c2_x;
  c2_b_asind(chartInstance, &c2_b_x);
  return c2_b_x;
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

static const mxArray *c2_k_sf_marshallOut(void *chartInstanceVoid, void
  *c2_inData)
{
  const mxArray *c2_mxArrayOutData;
  int32_T c2_b_u;
  const mxArray *c2_b_y = NULL;
  SFc2_solver_simulinkInstanceStruct *chartInstance;
  chartInstance = (SFc2_solver_simulinkInstanceStruct *)chartInstanceVoid;
  c2_mxArrayOutData = NULL;
  c2_mxArrayOutData = NULL;
  c2_b_u = *(int32_T *)c2_inData;
  c2_b_y = NULL;
  sf_mex_assign(&c2_b_y, sf_mex_create("y", &c2_b_u, 6, 0U, 0U, 0U, 0), false);
  sf_mex_assign(&c2_mxArrayOutData, c2_b_y, false);
  return c2_mxArrayOutData;
}

static int32_T c2_w_emlrt_marshallIn(SFc2_solver_simulinkInstanceStruct
  *chartInstance, const mxArray *c2_b_u, const emlrtMsgIdentifier *c2_parentId)
{
  int32_T c2_b_y;
  int32_T c2_i59;
  (void)chartInstance;
  sf_mex_import(c2_parentId, sf_mex_dup(c2_b_u), &c2_i59, 1, 6, 0U, 0, 0U, 0);
  c2_b_y = c2_i59;
  sf_mex_destroy(&c2_b_u);
  return c2_b_y;
}

static void c2_k_sf_marshallIn(void *chartInstanceVoid, const mxArray
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
  c2_b_y = c2_w_emlrt_marshallIn(chartInstance, sf_mex_dup(c2_b_sfEvent),
    &c2_thisId);
  sf_mex_destroy(&c2_b_sfEvent);
  *(int32_T *)c2_outData = c2_b_y;
  sf_mex_destroy(&c2_mxArrayInData);
}

static uint8_T c2_x_emlrt_marshallIn(SFc2_solver_simulinkInstanceStruct
  *chartInstance, const mxArray *c2_b_is_active_c2_solver_simulink, const char_T
  *c2_identifier)
{
  uint8_T c2_b_y;
  emlrtMsgIdentifier c2_thisId;
  c2_thisId.fIdentifier = c2_identifier;
  c2_thisId.fParent = NULL;
  c2_thisId.bParentIsCell = false;
  c2_b_y = c2_y_emlrt_marshallIn(chartInstance, sf_mex_dup
    (c2_b_is_active_c2_solver_simulink), &c2_thisId);
  sf_mex_destroy(&c2_b_is_active_c2_solver_simulink);
  return c2_b_y;
}

static uint8_T c2_y_emlrt_marshallIn(SFc2_solver_simulinkInstanceStruct
  *chartInstance, const mxArray *c2_b_u, const emlrtMsgIdentifier *c2_parentId)
{
  uint8_T c2_b_y;
  uint8_T c2_u0;
  (void)chartInstance;
  sf_mex_import(c2_parentId, sf_mex_dup(c2_b_u), &c2_u0, 1, 3, 0U, 0, 0U, 0);
  c2_b_y = c2_u0;
  sf_mex_destroy(&c2_b_u);
  return c2_b_y;
}

static void c2_b_sind(SFc2_solver_simulinkInstanceStruct *chartInstance, real_T *
                      c2_x)
{
  real_T c2_b_x;
  real_T c2_c_x;
  real_T c2_d_x;
  boolean_T c2_b;
  boolean_T c2_b0;
  real_T c2_e_x;
  boolean_T c2_b_b;
  boolean_T c2_b1;
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
  c2_b0 = !c2_b;
  c2_e_x = c2_c_x;
  c2_b_b = muDoubleScalarIsNaN(c2_e_x);
  c2_b1 = !c2_b_b;
  c2_c_b = (c2_b0 && c2_b1);
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
  c2_b2 = !c2_b;
  c2_e_x = c2_c_x;
  c2_b_b = muDoubleScalarIsNaN(c2_e_x);
  c2_b3 = !c2_b_b;
  c2_c_b = (c2_b2 && c2_b3);
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

static void c2_b_asind(SFc2_solver_simulinkInstanceStruct *chartInstance, real_T
  *c2_x)
{
  real_T c2_b_x;
  boolean_T c2_b4;
  boolean_T c2_b5;
  boolean_T c2_p;
  boolean_T c2_b_p;
  real_T c2_c_x;
  real_T c2_d_x;
  c2_b_x = *c2_x;
  c2_b4 = (c2_b_x < -1.0);
  c2_b5 = (c2_b_x > 1.0);
  c2_p = (c2_b4 || c2_b5);
  c2_b_p = c2_p;
  if (c2_b_p) {
    c2_b_error(chartInstance);
  }

  c2_c_x = *c2_x;
  c2_d_x = c2_c_x;
  c2_d_x = muDoubleScalarAsin(c2_d_x);
  *c2_x = 57.295779513082323 * c2_d_x;
}

static void init_dsm_address_info(SFc2_solver_simulinkInstanceStruct
  *chartInstance)
{
  (void)chartInstance;
}

static void init_simulink_io_address(SFc2_solver_simulinkInstanceStruct
  *chartInstance)
{
  chartInstance->c2_forceout = (real_T (*)[10])ssGetOutputPortSignal_wrapper
    (chartInstance->S, 1);
  chartInstance->c2_t = (real_T *)ssGetInputPortSignal_wrapper(chartInstance->S,
    0);
  chartInstance->c2_statedot = (real_T (*)[6])ssGetOutputPortSignal_wrapper
    (chartInstance->S, 2);
  chartInstance->c2_state = (real_T (*)[6])ssGetInputPortSignal_wrapper
    (chartInstance->S, 1);
  chartInstance->c2_t_step = (real_T *)ssGetInputPortSignal_wrapper
    (chartInstance->S, 2);
  chartInstance->c2_T = (real_T (*)[1256])ssGetInputPortSignal_wrapper
    (chartInstance->S, 3);
  chartInstance->c2_mass = (real_T (*)[1256])ssGetInputPortSignal_wrapper
    (chartInstance->S, 4);
  chartInstance->c2_wind = (real_T *)ssGetInputPortSignal_wrapper
    (chartInstance->S, 5);
  chartInstance->c2_d_aerodata = (real_T (*)[25010])ssGetInputPortSignal_wrapper
    (chartInstance->S, 6);
  chartInstance->c2_CM = (real_T *)ssGetInputPortSignal_wrapper(chartInstance->S,
    7);
  chartInstance->c2_site_elevation = (real_T *)ssGetInputPortSignal_wrapper
    (chartInstance->S, 8);
  chartInstance->c2_T0 = (real_T *)ssGetInputPortSignal_wrapper(chartInstance->S,
    9);
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
  ((real_T *)mxGetPr((plhs[0])))[0] = (real_T)(3045320160U);
  ((real_T *)mxGetPr((plhs[0])))[1] = (real_T)(1811755107U);
  ((real_T *)mxGetPr((plhs[0])))[2] = (real_T)(2825798743U);
  ((real_T *)mxGetPr((plhs[0])))[3] = (real_T)(1349332254U);
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
    mxArray *mxChecksum = mxCreateString("mOqO5WH1tOeuwVuJTAfjaB");
    mxSetField(mxAutoinheritanceInfo,0,"checksum",mxChecksum);
  }

  {
    const char *dataFields[] = { "size", "type", "complexity" };

    mxArray *mxData = mxCreateStructMatrix(1,10,3,dataFields);

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
      mxArray *mxSize = mxCreateDoubleMatrix(1,1,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(6);
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
      mxArray *mxSize = mxCreateDoubleMatrix(1,0,mxREAL);
      double *pr = mxGetPr(mxSize);
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
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(628);
      pr[1] = (double)(2);
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
      mxArray *mxSize = mxCreateDoubleMatrix(1,0,mxREAL);
      double *pr = mxGetPr(mxSize);
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
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(2501);
      pr[1] = (double)(10);
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
      mxArray *mxSize = mxCreateDoubleMatrix(1,0,mxREAL);
      double *pr = mxGetPr(mxSize);
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

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,0,mxREAL);
      double *pr = mxGetPr(mxSize);
      mxSetField(mxData,8,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt", "isFixedPointType" };

      mxArray *mxType = mxCreateStructMatrix(1,1,sizeof(typeFields)/sizeof
        (typeFields[0]),typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxType,0,"isFixedPointType",mxCreateDoubleScalar(0));
      mxSetField(mxData,8,"type",mxType);
    }

    mxSetField(mxData,8,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,0,mxREAL);
      double *pr = mxGetPr(mxSize);
      mxSetField(mxData,9,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt", "isFixedPointType" };

      mxArray *mxType = mxCreateStructMatrix(1,1,sizeof(typeFields)/sizeof
        (typeFields[0]),typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxType,0,"isFixedPointType",mxCreateDoubleScalar(0));
      mxSetField(mxData,9,"type",mxType);
    }

    mxSetField(mxData,9,"complexity",mxCreateDoubleScalar(0));
    mxSetField(mxAutoinheritanceInfo,0,"inputs",mxData);
  }

  {
    mxSetField(mxAutoinheritanceInfo,0,"parameters",mxCreateDoubleMatrix(0,0,
                mxREAL));
  }

  {
    const char *dataFields[] = { "size", "type", "complexity" };

    mxArray *mxData = mxCreateStructMatrix(1,2,3,dataFields);

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(10);
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
      mxArray *mxSize = mxCreateDoubleMatrix(1,1,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(6);
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
    "100 S1x3'type','srcId','name','auxInfo'{{M[1],M[20],T\"forceout\",},{M[1],M[5],T\"statedot\",},{M[8],M[0],T\"is_active_c2_solver_simulink\",}}"
  };

  mxArray *mxVarInfo = sf_mex_decode_encoded_mx_struct_array(infoEncStr, 3, 10);
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
           12,
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
          _SFD_SET_DATA_PROPS(1,1,1,0,"state");
          _SFD_SET_DATA_PROPS(2,1,1,0,"t_step");
          _SFD_SET_DATA_PROPS(3,1,1,0,"T");
          _SFD_SET_DATA_PROPS(4,1,1,0,"mass");
          _SFD_SET_DATA_PROPS(5,1,1,0,"wind");
          _SFD_SET_DATA_PROPS(6,1,1,0,"aerodata");
          _SFD_SET_DATA_PROPS(7,1,1,0,"CM");
          _SFD_SET_DATA_PROPS(8,1,1,0,"site_elevation");
          _SFD_SET_DATA_PROPS(9,1,1,0,"T0");
          _SFD_SET_DATA_PROPS(10,2,0,1,"forceout");
          _SFD_SET_DATA_PROPS(11,2,0,1,"statedot");
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
        _SFD_CV_INIT_EML_FCN(0,0,"eML_blk_kernel",0,-1,1159);
        _SFD_CV_INIT_EML_IF(0,1,0,424,444,607,647);
        _SFD_CV_INIT_EML_IF(0,1,1,525,539,474,576);
        _SFD_CV_INIT_EML_WHILE(0,1,0,474,482,576);
        _SFD_CV_INIT_EML_RELATIONAL(0,1,0,427,444,-1,3);
        _SFD_CV_INIT_EML_RELATIONAL(0,1,1,528,539,-1,2);
        _SFD_CV_INIT_SCRIPT(0,1,0,5,0,0,0,0,1,0,0);
        _SFD_CV_INIT_SCRIPT_FCN(0,0,"forces",394,-1,2058);
        _SFD_CV_INIT_SCRIPT_IF(0,0,733,748,957,989);
        _SFD_CV_INIT_SCRIPT_IF(0,1,826,840,778,877);
        _SFD_CV_INIT_SCRIPT_IF(0,2,1482,1507,-1,1528);
        _SFD_CV_INIT_SCRIPT_IF(0,3,1530,1543,1554,1572);
        _SFD_CV_INIT_SCRIPT_IF(0,4,1657,1669,1864,1985);
        _SFD_CV_INIT_SCRIPT_WHILE(0,0,778,786,877);
        _SFD_CV_INIT_SCRIPT_RELATIONAL(0,0,736,748,-1,2);
        _SFD_CV_INIT_SCRIPT_RELATIONAL(0,1,829,840,-1,2);
        _SFD_CV_INIT_SCRIPT_RELATIONAL(0,2,1485,1507,-1,0);
        _SFD_CV_INIT_SCRIPT_RELATIONAL(0,3,1533,1543,-1,5);
        _SFD_CV_INIT_SCRIPT_RELATIONAL(0,4,1660,1669,-1,5);
        _SFD_CV_INIT_SCRIPT(1,1,0,7,0,0,0,0,1,2,1);
        _SFD_CV_INIT_SCRIPT_FCN(1,0,"aerodynamics",106,-1,2737);
        _SFD_CV_INIT_SCRIPT_IF(1,0,792,806,742,838);
        _SFD_CV_INIT_SCRIPT_IF(1,1,1247,1259,2223,2732);
        _SFD_CV_INIT_SCRIPT_IF(1,2,1269,1303,-1,1328);
        _SFD_CV_INIT_SCRIPT_IF(1,3,1464,1480,1707,2082);
        _SFD_CV_INIT_SCRIPT_IF(1,4,1707,1727,1961,2082);
        _SFD_CV_INIT_SCRIPT_IF(1,5,2223,2290,2456,2732);
        _SFD_CV_INIT_SCRIPT_IF(1,6,2456,2490,2646,2732);
        _SFD_CV_INIT_SCRIPT_WHILE(1,0,742,750,838);

        {
          static int condStart[] = { 2230, 2263 };

          static int condEnd[] = { 2259, 2290 };

          static int pfixExpr[] = { 0, 1, -3 };

          _SFD_CV_INIT_SCRIPT_MCDC(1,0,2230,2290,2,0,&(condStart[0]),&(condEnd[0]),
            3,&(pfixExpr[0]));
        }

        _SFD_CV_INIT_SCRIPT_RELATIONAL(1,0,795,806,-1,2);
        _SFD_CV_INIT_SCRIPT_RELATIONAL(1,1,1250,1259,-1,5);
        _SFD_CV_INIT_SCRIPT_RELATIONAL(1,2,1272,1303,-1,3);
        _SFD_CV_INIT_SCRIPT_RELATIONAL(1,3,1467,1480,-1,3);
        _SFD_CV_INIT_SCRIPT_RELATIONAL(1,4,1714,1727,-1,3);
        _SFD_CV_INIT_SCRIPT_RELATIONAL(1,5,2230,2259,-1,2);
        _SFD_CV_INIT_SCRIPT_RELATIONAL(1,6,2263,2290,-1,4);
        _SFD_CV_INIT_SCRIPT_RELATIONAL(1,7,2463,2490,-1,2);
        _SFD_CV_INIT_SCRIPT(2,1,0,7,0,0,0,0,0,0,0);
        _SFD_CV_INIT_SCRIPT_FCN(2,0,"getAtmoConditions",212,-1,1494);
        _SFD_CV_INIT_SCRIPT_IF(2,0,497,512,551,929);
        _SFD_CV_INIT_SCRIPT_IF(2,1,551,570,609,929);
        _SFD_CV_INIT_SCRIPT_IF(2,2,609,628,667,929);
        _SFD_CV_INIT_SCRIPT_IF(2,3,667,686,725,929);
        _SFD_CV_INIT_SCRIPT_IF(2,4,725,744,783,929);
        _SFD_CV_INIT_SCRIPT_IF(2,5,783,802,841,929);
        _SFD_CV_INIT_SCRIPT_IF(2,6,841,860,899,929);
        _SFD_CV_INIT_SCRIPT_RELATIONAL(2,0,500,512,-1,3);
        _SFD_CV_INIT_SCRIPT_RELATIONAL(2,1,558,570,-1,3);
        _SFD_CV_INIT_SCRIPT_RELATIONAL(2,2,616,628,-1,3);
        _SFD_CV_INIT_SCRIPT_RELATIONAL(2,3,674,686,-1,3);
        _SFD_CV_INIT_SCRIPT_RELATIONAL(2,4,732,744,-1,3);
        _SFD_CV_INIT_SCRIPT_RELATIONAL(2,5,790,802,-1,3);
        _SFD_CV_INIT_SCRIPT_RELATIONAL(2,6,848,860,-1,3);
        _SFD_SET_DATA_COMPILED_PROPS(0,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c2_c_sf_marshallOut,(MexInFcnForType)NULL);

        {
          unsigned int dimVector[1];
          dimVector[0]= 6U;
          _SFD_SET_DATA_COMPILED_PROPS(1,SF_DOUBLE,1,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c2_sf_marshallOut,(MexInFcnForType)NULL);
        }

        _SFD_SET_DATA_COMPILED_PROPS(2,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c2_c_sf_marshallOut,(MexInFcnForType)NULL);

        {
          unsigned int dimVector[2];
          dimVector[0]= 628U;
          dimVector[1]= 2U;
          _SFD_SET_DATA_COMPILED_PROPS(3,SF_DOUBLE,2,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c2_e_sf_marshallOut,(MexInFcnForType)NULL);
        }

        {
          unsigned int dimVector[2];
          dimVector[0]= 628U;
          dimVector[1]= 2U;
          _SFD_SET_DATA_COMPILED_PROPS(4,SF_DOUBLE,2,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c2_e_sf_marshallOut,(MexInFcnForType)NULL);
        }

        _SFD_SET_DATA_COMPILED_PROPS(5,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c2_c_sf_marshallOut,(MexInFcnForType)NULL);

        {
          unsigned int dimVector[2];
          dimVector[0]= 2501U;
          dimVector[1]= 10U;
          _SFD_SET_DATA_COMPILED_PROPS(6,SF_DOUBLE,2,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c2_d_sf_marshallOut,(MexInFcnForType)NULL);
        }

        _SFD_SET_DATA_COMPILED_PROPS(7,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c2_c_sf_marshallOut,(MexInFcnForType)NULL);
        _SFD_SET_DATA_COMPILED_PROPS(8,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c2_c_sf_marshallOut,(MexInFcnForType)NULL);
        _SFD_SET_DATA_COMPILED_PROPS(9,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c2_c_sf_marshallOut,(MexInFcnForType)NULL);

        {
          unsigned int dimVector[2];
          dimVector[0]= 1U;
          dimVector[1]= 10U;
          _SFD_SET_DATA_COMPILED_PROPS(10,SF_DOUBLE,2,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c2_b_sf_marshallOut,(MexInFcnForType)
            c2_b_sf_marshallIn);
        }

        {
          unsigned int dimVector[1];
          dimVector[0]= 6U;
          _SFD_SET_DATA_COMPILED_PROPS(11,SF_DOUBLE,1,&(dimVector[0]),0,0,0,0.0,
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
        _SFD_SET_DATA_VALUE_PTR(10U, *chartInstance->c2_forceout);
        _SFD_SET_DATA_VALUE_PTR(0U, chartInstance->c2_t);
        _SFD_SET_DATA_VALUE_PTR(11U, *chartInstance->c2_statedot);
        _SFD_SET_DATA_VALUE_PTR(1U, *chartInstance->c2_state);
        _SFD_SET_DATA_VALUE_PTR(2U, chartInstance->c2_t_step);
        _SFD_SET_DATA_VALUE_PTR(3U, *chartInstance->c2_T);
        _SFD_SET_DATA_VALUE_PTR(4U, *chartInstance->c2_mass);
        _SFD_SET_DATA_VALUE_PTR(5U, chartInstance->c2_wind);
        _SFD_SET_DATA_VALUE_PTR(6U, *chartInstance->c2_d_aerodata);
        _SFD_SET_DATA_VALUE_PTR(7U, chartInstance->c2_CM);
        _SFD_SET_DATA_VALUE_PTR(8U, chartInstance->c2_site_elevation);
        _SFD_SET_DATA_VALUE_PTR(9U, chartInstance->c2_T0);
      }
    }
  }
}

static const char* sf_get_instance_specialization(void)
{
  return "s6HijgpaX33YP9aqLvBgQBF";
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
      ssSetInputPortOptimOpts(S, 8, SS_REUSABLE_AND_LOCAL);
      ssSetInputPortOptimOpts(S, 9, SS_REUSABLE_AND_LOCAL);
      sf_mark_chart_expressionable_inputs(S,sf_get_instance_specialization(),
        infoStruct,2,10);
      sf_mark_chart_reusable_outputs(S,sf_get_instance_specialization(),
        infoStruct,2,2);
    }

    {
      unsigned int outPortIdx;
      for (outPortIdx=1; outPortIdx<=2; ++outPortIdx) {
        ssSetOutputPortOptimizeInIR(S, outPortIdx, 1U);
      }
    }

    {
      unsigned int inPortIdx;
      for (inPortIdx=0; inPortIdx < 10; ++inPortIdx) {
        ssSetInputPortOptimizeInIR(S, inPortIdx, 1U);
      }
    }

    sf_set_rtw_dwork_info(S,sf_get_instance_specialization(),infoStruct,2);
    sf_register_codegen_names_for_scoped_functions_defined_by_chart(S);
    ssSetHasSubFunctions(S,!(chartIsInlinable));
  } else {
  }

  ssSetOptions(S,ssGetOptions(S)|SS_OPTION_WORKS_WITH_CODE_REUSE);
  ssSetChecksum0(S,(2354657649U));
  ssSetChecksum1(S,(1480646934U));
  ssSetChecksum2(S,(3480948660U));
  ssSetChecksum3(S,(874912624U));
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
