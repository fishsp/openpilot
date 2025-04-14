#include "car.h"

namespace {
#define DIM 9
#define EDIM 9
#define MEDIM 9
typedef void (*Hfun)(double *, double *, double *);

double mass;

void set_mass(double x){ mass = x;}

double rotational_inertia;

void set_rotational_inertia(double x){ rotational_inertia = x;}

double center_to_front;

void set_center_to_front(double x){ center_to_front = x;}

double center_to_rear;

void set_center_to_rear(double x){ center_to_rear = x;}

double stiffness_front;

void set_stiffness_front(double x){ stiffness_front = x;}

double stiffness_rear;

void set_stiffness_rear(double x){ stiffness_rear = x;}
const static double MAHA_THRESH_25 = 3.8414588206941227;
const static double MAHA_THRESH_24 = 5.991464547107981;
const static double MAHA_THRESH_30 = 3.8414588206941227;
const static double MAHA_THRESH_26 = 3.8414588206941227;
const static double MAHA_THRESH_27 = 3.8414588206941227;
const static double MAHA_THRESH_29 = 3.8414588206941227;
const static double MAHA_THRESH_28 = 3.8414588206941227;
const static double MAHA_THRESH_31 = 3.8414588206941227;

/******************************************************************************
 *                      Code generated with SymPy 1.13.2                      *
 *                                                                            *
 *              See http://www.sympy.org/ for more information.               *
 *                                                                            *
 *                         This file is part of 'ekf'                         *
 ******************************************************************************/
void err_fun(double *nom_x, double *delta_x, double *out_1157861628203609812) {
   out_1157861628203609812[0] = delta_x[0] + nom_x[0];
   out_1157861628203609812[1] = delta_x[1] + nom_x[1];
   out_1157861628203609812[2] = delta_x[2] + nom_x[2];
   out_1157861628203609812[3] = delta_x[3] + nom_x[3];
   out_1157861628203609812[4] = delta_x[4] + nom_x[4];
   out_1157861628203609812[5] = delta_x[5] + nom_x[5];
   out_1157861628203609812[6] = delta_x[6] + nom_x[6];
   out_1157861628203609812[7] = delta_x[7] + nom_x[7];
   out_1157861628203609812[8] = delta_x[8] + nom_x[8];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_3048943521048617947) {
   out_3048943521048617947[0] = -nom_x[0] + true_x[0];
   out_3048943521048617947[1] = -nom_x[1] + true_x[1];
   out_3048943521048617947[2] = -nom_x[2] + true_x[2];
   out_3048943521048617947[3] = -nom_x[3] + true_x[3];
   out_3048943521048617947[4] = -nom_x[4] + true_x[4];
   out_3048943521048617947[5] = -nom_x[5] + true_x[5];
   out_3048943521048617947[6] = -nom_x[6] + true_x[6];
   out_3048943521048617947[7] = -nom_x[7] + true_x[7];
   out_3048943521048617947[8] = -nom_x[8] + true_x[8];
}
void H_mod_fun(double *state, double *out_6436924980861323347) {
   out_6436924980861323347[0] = 1.0;
   out_6436924980861323347[1] = 0.0;
   out_6436924980861323347[2] = 0.0;
   out_6436924980861323347[3] = 0.0;
   out_6436924980861323347[4] = 0.0;
   out_6436924980861323347[5] = 0.0;
   out_6436924980861323347[6] = 0.0;
   out_6436924980861323347[7] = 0.0;
   out_6436924980861323347[8] = 0.0;
   out_6436924980861323347[9] = 0.0;
   out_6436924980861323347[10] = 1.0;
   out_6436924980861323347[11] = 0.0;
   out_6436924980861323347[12] = 0.0;
   out_6436924980861323347[13] = 0.0;
   out_6436924980861323347[14] = 0.0;
   out_6436924980861323347[15] = 0.0;
   out_6436924980861323347[16] = 0.0;
   out_6436924980861323347[17] = 0.0;
   out_6436924980861323347[18] = 0.0;
   out_6436924980861323347[19] = 0.0;
   out_6436924980861323347[20] = 1.0;
   out_6436924980861323347[21] = 0.0;
   out_6436924980861323347[22] = 0.0;
   out_6436924980861323347[23] = 0.0;
   out_6436924980861323347[24] = 0.0;
   out_6436924980861323347[25] = 0.0;
   out_6436924980861323347[26] = 0.0;
   out_6436924980861323347[27] = 0.0;
   out_6436924980861323347[28] = 0.0;
   out_6436924980861323347[29] = 0.0;
   out_6436924980861323347[30] = 1.0;
   out_6436924980861323347[31] = 0.0;
   out_6436924980861323347[32] = 0.0;
   out_6436924980861323347[33] = 0.0;
   out_6436924980861323347[34] = 0.0;
   out_6436924980861323347[35] = 0.0;
   out_6436924980861323347[36] = 0.0;
   out_6436924980861323347[37] = 0.0;
   out_6436924980861323347[38] = 0.0;
   out_6436924980861323347[39] = 0.0;
   out_6436924980861323347[40] = 1.0;
   out_6436924980861323347[41] = 0.0;
   out_6436924980861323347[42] = 0.0;
   out_6436924980861323347[43] = 0.0;
   out_6436924980861323347[44] = 0.0;
   out_6436924980861323347[45] = 0.0;
   out_6436924980861323347[46] = 0.0;
   out_6436924980861323347[47] = 0.0;
   out_6436924980861323347[48] = 0.0;
   out_6436924980861323347[49] = 0.0;
   out_6436924980861323347[50] = 1.0;
   out_6436924980861323347[51] = 0.0;
   out_6436924980861323347[52] = 0.0;
   out_6436924980861323347[53] = 0.0;
   out_6436924980861323347[54] = 0.0;
   out_6436924980861323347[55] = 0.0;
   out_6436924980861323347[56] = 0.0;
   out_6436924980861323347[57] = 0.0;
   out_6436924980861323347[58] = 0.0;
   out_6436924980861323347[59] = 0.0;
   out_6436924980861323347[60] = 1.0;
   out_6436924980861323347[61] = 0.0;
   out_6436924980861323347[62] = 0.0;
   out_6436924980861323347[63] = 0.0;
   out_6436924980861323347[64] = 0.0;
   out_6436924980861323347[65] = 0.0;
   out_6436924980861323347[66] = 0.0;
   out_6436924980861323347[67] = 0.0;
   out_6436924980861323347[68] = 0.0;
   out_6436924980861323347[69] = 0.0;
   out_6436924980861323347[70] = 1.0;
   out_6436924980861323347[71] = 0.0;
   out_6436924980861323347[72] = 0.0;
   out_6436924980861323347[73] = 0.0;
   out_6436924980861323347[74] = 0.0;
   out_6436924980861323347[75] = 0.0;
   out_6436924980861323347[76] = 0.0;
   out_6436924980861323347[77] = 0.0;
   out_6436924980861323347[78] = 0.0;
   out_6436924980861323347[79] = 0.0;
   out_6436924980861323347[80] = 1.0;
}
void f_fun(double *state, double dt, double *out_8302405370356002085) {
   out_8302405370356002085[0] = state[0];
   out_8302405370356002085[1] = state[1];
   out_8302405370356002085[2] = state[2];
   out_8302405370356002085[3] = state[3];
   out_8302405370356002085[4] = state[4];
   out_8302405370356002085[5] = dt*((-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]))*state[6] - 9.8000000000000007*state[8] + stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*state[1]) + (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*state[4])) + state[5];
   out_8302405370356002085[6] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*state[4])) + state[6];
   out_8302405370356002085[7] = state[7];
   out_8302405370356002085[8] = state[8];
}
void F_fun(double *state, double dt, double *out_7970830068534808110) {
   out_7970830068534808110[0] = 1;
   out_7970830068534808110[1] = 0;
   out_7970830068534808110[2] = 0;
   out_7970830068534808110[3] = 0;
   out_7970830068534808110[4] = 0;
   out_7970830068534808110[5] = 0;
   out_7970830068534808110[6] = 0;
   out_7970830068534808110[7] = 0;
   out_7970830068534808110[8] = 0;
   out_7970830068534808110[9] = 0;
   out_7970830068534808110[10] = 1;
   out_7970830068534808110[11] = 0;
   out_7970830068534808110[12] = 0;
   out_7970830068534808110[13] = 0;
   out_7970830068534808110[14] = 0;
   out_7970830068534808110[15] = 0;
   out_7970830068534808110[16] = 0;
   out_7970830068534808110[17] = 0;
   out_7970830068534808110[18] = 0;
   out_7970830068534808110[19] = 0;
   out_7970830068534808110[20] = 1;
   out_7970830068534808110[21] = 0;
   out_7970830068534808110[22] = 0;
   out_7970830068534808110[23] = 0;
   out_7970830068534808110[24] = 0;
   out_7970830068534808110[25] = 0;
   out_7970830068534808110[26] = 0;
   out_7970830068534808110[27] = 0;
   out_7970830068534808110[28] = 0;
   out_7970830068534808110[29] = 0;
   out_7970830068534808110[30] = 1;
   out_7970830068534808110[31] = 0;
   out_7970830068534808110[32] = 0;
   out_7970830068534808110[33] = 0;
   out_7970830068534808110[34] = 0;
   out_7970830068534808110[35] = 0;
   out_7970830068534808110[36] = 0;
   out_7970830068534808110[37] = 0;
   out_7970830068534808110[38] = 0;
   out_7970830068534808110[39] = 0;
   out_7970830068534808110[40] = 1;
   out_7970830068534808110[41] = 0;
   out_7970830068534808110[42] = 0;
   out_7970830068534808110[43] = 0;
   out_7970830068534808110[44] = 0;
   out_7970830068534808110[45] = dt*(stiffness_front*(-state[2] - state[3] + state[7])/(mass*state[1]) + (-stiffness_front - stiffness_rear)*state[5]/(mass*state[4]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[6]/(mass*state[4]));
   out_7970830068534808110[46] = -dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*pow(state[1], 2));
   out_7970830068534808110[47] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_7970830068534808110[48] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_7970830068534808110[49] = dt*((-1 - (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*pow(state[4], 2)))*state[6] - (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*pow(state[4], 2)));
   out_7970830068534808110[50] = dt*(-stiffness_front*state[0] - stiffness_rear*state[0])/(mass*state[4]) + 1;
   out_7970830068534808110[51] = dt*(-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]));
   out_7970830068534808110[52] = dt*stiffness_front*state[0]/(mass*state[1]);
   out_7970830068534808110[53] = -9.8000000000000007*dt;
   out_7970830068534808110[54] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front - pow(center_to_rear, 2)*stiffness_rear)*state[6]/(rotational_inertia*state[4]));
   out_7970830068534808110[55] = -center_to_front*dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*pow(state[1], 2));
   out_7970830068534808110[56] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_7970830068534808110[57] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_7970830068534808110[58] = dt*(-(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*pow(state[4], 2)) - (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*pow(state[4], 2)));
   out_7970830068534808110[59] = dt*(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(rotational_inertia*state[4]);
   out_7970830068534808110[60] = dt*(-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])/(rotational_inertia*state[4]) + 1;
   out_7970830068534808110[61] = center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_7970830068534808110[62] = 0;
   out_7970830068534808110[63] = 0;
   out_7970830068534808110[64] = 0;
   out_7970830068534808110[65] = 0;
   out_7970830068534808110[66] = 0;
   out_7970830068534808110[67] = 0;
   out_7970830068534808110[68] = 0;
   out_7970830068534808110[69] = 0;
   out_7970830068534808110[70] = 1;
   out_7970830068534808110[71] = 0;
   out_7970830068534808110[72] = 0;
   out_7970830068534808110[73] = 0;
   out_7970830068534808110[74] = 0;
   out_7970830068534808110[75] = 0;
   out_7970830068534808110[76] = 0;
   out_7970830068534808110[77] = 0;
   out_7970830068534808110[78] = 0;
   out_7970830068534808110[79] = 0;
   out_7970830068534808110[80] = 1;
}
void h_25(double *state, double *unused, double *out_7195447474562559623) {
   out_7195447474562559623[0] = state[6];
}
void H_25(double *state, double *unused, double *out_7214988249920359160) {
   out_7214988249920359160[0] = 0;
   out_7214988249920359160[1] = 0;
   out_7214988249920359160[2] = 0;
   out_7214988249920359160[3] = 0;
   out_7214988249920359160[4] = 0;
   out_7214988249920359160[5] = 0;
   out_7214988249920359160[6] = 1;
   out_7214988249920359160[7] = 0;
   out_7214988249920359160[8] = 0;
}
void h_24(double *state, double *unused, double *out_5845787550217648453) {
   out_5845787550217648453[0] = state[4];
   out_5845787550217648453[1] = state[5];
}
void H_24(double *state, double *unused, double *out_1929878460030518002) {
   out_1929878460030518002[0] = 0;
   out_1929878460030518002[1] = 0;
   out_1929878460030518002[2] = 0;
   out_1929878460030518002[3] = 0;
   out_1929878460030518002[4] = 1;
   out_1929878460030518002[5] = 0;
   out_1929878460030518002[6] = 0;
   out_1929878460030518002[7] = 0;
   out_1929878460030518002[8] = 0;
   out_1929878460030518002[9] = 0;
   out_1929878460030518002[10] = 0;
   out_1929878460030518002[11] = 0;
   out_1929878460030518002[12] = 0;
   out_1929878460030518002[13] = 0;
   out_1929878460030518002[14] = 1;
   out_1929878460030518002[15] = 0;
   out_1929878460030518002[16] = 0;
   out_1929878460030518002[17] = 0;
}
void h_30(double *state, double *unused, double *out_2387185174253970807) {
   out_2387185174253970807[0] = state[4];
}
void H_30(double *state, double *unused, double *out_8713422865281943829) {
   out_8713422865281943829[0] = 0;
   out_8713422865281943829[1] = 0;
   out_8713422865281943829[2] = 0;
   out_8713422865281943829[3] = 0;
   out_8713422865281943829[4] = 1;
   out_8713422865281943829[5] = 0;
   out_8713422865281943829[6] = 0;
   out_8713422865281943829[7] = 0;
   out_8713422865281943829[8] = 0;
}
void h_26(double *state, double *unused, double *out_2614488093406874838) {
   out_2614488093406874838[0] = state[7];
}
void H_26(double *state, double *unused, double *out_7927229854028391855) {
   out_7927229854028391855[0] = 0;
   out_7927229854028391855[1] = 0;
   out_7927229854028391855[2] = 0;
   out_7927229854028391855[3] = 0;
   out_7927229854028391855[4] = 0;
   out_7927229854028391855[5] = 0;
   out_7927229854028391855[6] = 0;
   out_7927229854028391855[7] = 1;
   out_7927229854028391855[8] = 0;
}
void h_27(double *state, double *unused, double *out_6437597329401740813) {
   out_6437597329401740813[0] = state[3];
}
void H_27(double *state, double *unused, double *out_7558557896627182876) {
   out_7558557896627182876[0] = 0;
   out_7558557896627182876[1] = 0;
   out_7558557896627182876[2] = 0;
   out_7558557896627182876[3] = 1;
   out_7558557896627182876[4] = 0;
   out_7558557896627182876[5] = 0;
   out_7558557896627182876[6] = 0;
   out_7558557896627182876[7] = 0;
   out_7558557896627182876[8] = 0;
}
void h_29(double *state, double *unused, double *out_1443675321352308372) {
   out_1443675321352308372[0] = state[1];
}
void H_29(double *state, double *unused, double *out_8203191520967551645) {
   out_8203191520967551645[0] = 0;
   out_8203191520967551645[1] = 1;
   out_8203191520967551645[2] = 0;
   out_8203191520967551645[3] = 0;
   out_8203191520967551645[4] = 0;
   out_8203191520967551645[5] = 0;
   out_8203191520967551645[6] = 0;
   out_8203191520967551645[7] = 0;
   out_8203191520967551645[8] = 0;
}
void h_28(double *state, double *unused, double *out_1435924061740895168) {
   out_1435924061740895168[0] = state[0];
}
void H_28(double *state, double *unused, double *out_5161153535672469397) {
   out_5161153535672469397[0] = 1;
   out_5161153535672469397[1] = 0;
   out_5161153535672469397[2] = 0;
   out_5161153535672469397[3] = 0;
   out_5161153535672469397[4] = 0;
   out_5161153535672469397[5] = 0;
   out_5161153535672469397[6] = 0;
   out_5161153535672469397[7] = 0;
   out_5161153535672469397[8] = 0;
}
void h_31(double *state, double *unused, double *out_4059747486291480197) {
   out_4059747486291480197[0] = state[8];
}
void H_31(double *state, double *unused, double *out_2847276828812951460) {
   out_2847276828812951460[0] = 0;
   out_2847276828812951460[1] = 0;
   out_2847276828812951460[2] = 0;
   out_2847276828812951460[3] = 0;
   out_2847276828812951460[4] = 0;
   out_2847276828812951460[5] = 0;
   out_2847276828812951460[6] = 0;
   out_2847276828812951460[7] = 0;
   out_2847276828812951460[8] = 1;
}
#include <eigen3/Eigen/Dense>
#include <iostream>

typedef Eigen::Matrix<double, DIM, DIM, Eigen::RowMajor> DDM;
typedef Eigen::Matrix<double, EDIM, EDIM, Eigen::RowMajor> EEM;
typedef Eigen::Matrix<double, DIM, EDIM, Eigen::RowMajor> DEM;

void predict(double *in_x, double *in_P, double *in_Q, double dt) {
  typedef Eigen::Matrix<double, MEDIM, MEDIM, Eigen::RowMajor> RRM;

  double nx[DIM] = {0};
  double in_F[EDIM*EDIM] = {0};

  // functions from sympy
  f_fun(in_x, dt, nx);
  F_fun(in_x, dt, in_F);


  EEM F(in_F);
  EEM P(in_P);
  EEM Q(in_Q);

  RRM F_main = F.topLeftCorner(MEDIM, MEDIM);
  P.topLeftCorner(MEDIM, MEDIM) = (F_main * P.topLeftCorner(MEDIM, MEDIM)) * F_main.transpose();
  P.topRightCorner(MEDIM, EDIM - MEDIM) = F_main * P.topRightCorner(MEDIM, EDIM - MEDIM);
  P.bottomLeftCorner(EDIM - MEDIM, MEDIM) = P.bottomLeftCorner(EDIM - MEDIM, MEDIM) * F_main.transpose();

  P = P + dt*Q;

  // copy out state
  memcpy(in_x, nx, DIM * sizeof(double));
  memcpy(in_P, P.data(), EDIM * EDIM * sizeof(double));
}

// note: extra_args dim only correct when null space projecting
// otherwise 1
template <int ZDIM, int EADIM, bool MAHA_TEST>
void update(double *in_x, double *in_P, Hfun h_fun, Hfun H_fun, Hfun Hea_fun, double *in_z, double *in_R, double *in_ea, double MAHA_THRESHOLD) {
  typedef Eigen::Matrix<double, ZDIM, ZDIM, Eigen::RowMajor> ZZM;
  typedef Eigen::Matrix<double, ZDIM, DIM, Eigen::RowMajor> ZDM;
  typedef Eigen::Matrix<double, Eigen::Dynamic, EDIM, Eigen::RowMajor> XEM;
  //typedef Eigen::Matrix<double, EDIM, ZDIM, Eigen::RowMajor> EZM;
  typedef Eigen::Matrix<double, Eigen::Dynamic, 1> X1M;
  typedef Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> XXM;

  double in_hx[ZDIM] = {0};
  double in_H[ZDIM * DIM] = {0};
  double in_H_mod[EDIM * DIM] = {0};
  double delta_x[EDIM] = {0};
  double x_new[DIM] = {0};


  // state x, P
  Eigen::Matrix<double, ZDIM, 1> z(in_z);
  EEM P(in_P);
  ZZM pre_R(in_R);

  // functions from sympy
  h_fun(in_x, in_ea, in_hx);
  H_fun(in_x, in_ea, in_H);
  ZDM pre_H(in_H);

  // get y (y = z - hx)
  Eigen::Matrix<double, ZDIM, 1> pre_y(in_hx); pre_y = z - pre_y;
  X1M y; XXM H; XXM R;
  if (Hea_fun){
    typedef Eigen::Matrix<double, ZDIM, EADIM, Eigen::RowMajor> ZAM;
    double in_Hea[ZDIM * EADIM] = {0};
    Hea_fun(in_x, in_ea, in_Hea);
    ZAM Hea(in_Hea);
    XXM A = Hea.transpose().fullPivLu().kernel();


    y = A.transpose() * pre_y;
    H = A.transpose() * pre_H;
    R = A.transpose() * pre_R * A;
  } else {
    y = pre_y;
    H = pre_H;
    R = pre_R;
  }
  // get modified H
  H_mod_fun(in_x, in_H_mod);
  DEM H_mod(in_H_mod);
  XEM H_err = H * H_mod;

  // Do mahalobis distance test
  if (MAHA_TEST){
    XXM a = (H_err * P * H_err.transpose() + R).inverse();
    double maha_dist = y.transpose() * a * y;
    if (maha_dist > MAHA_THRESHOLD){
      R = 1.0e16 * R;
    }
  }

  // Outlier resilient weighting
  double weight = 1;//(1.5)/(1 + y.squaredNorm()/R.sum());

  // kalman gains and I_KH
  XXM S = ((H_err * P) * H_err.transpose()) + R/weight;
  XEM KT = S.fullPivLu().solve(H_err * P.transpose());
  //EZM K = KT.transpose(); TODO: WHY DOES THIS NOT COMPILE?
  //EZM K = S.fullPivLu().solve(H_err * P.transpose()).transpose();
  //std::cout << "Here is the matrix rot:\n" << K << std::endl;
  EEM I_KH = Eigen::Matrix<double, EDIM, EDIM>::Identity() - (KT.transpose() * H_err);

  // update state by injecting dx
  Eigen::Matrix<double, EDIM, 1> dx(delta_x);
  dx  = (KT.transpose() * y);
  memcpy(delta_x, dx.data(), EDIM * sizeof(double));
  err_fun(in_x, delta_x, x_new);
  Eigen::Matrix<double, DIM, 1> x(x_new);

  // update cov
  P = ((I_KH * P) * I_KH.transpose()) + ((KT.transpose() * R) * KT);

  // copy out state
  memcpy(in_x, x.data(), DIM * sizeof(double));
  memcpy(in_P, P.data(), EDIM * EDIM * sizeof(double));
  memcpy(in_z, y.data(), y.rows() * sizeof(double));
}




}
extern "C" {

void car_update_25(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_25, H_25, NULL, in_z, in_R, in_ea, MAHA_THRESH_25);
}
void car_update_24(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<2, 3, 0>(in_x, in_P, h_24, H_24, NULL, in_z, in_R, in_ea, MAHA_THRESH_24);
}
void car_update_30(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_30, H_30, NULL, in_z, in_R, in_ea, MAHA_THRESH_30);
}
void car_update_26(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_26, H_26, NULL, in_z, in_R, in_ea, MAHA_THRESH_26);
}
void car_update_27(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_27, H_27, NULL, in_z, in_R, in_ea, MAHA_THRESH_27);
}
void car_update_29(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_29, H_29, NULL, in_z, in_R, in_ea, MAHA_THRESH_29);
}
void car_update_28(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_28, H_28, NULL, in_z, in_R, in_ea, MAHA_THRESH_28);
}
void car_update_31(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_31, H_31, NULL, in_z, in_R, in_ea, MAHA_THRESH_31);
}
void car_err_fun(double *nom_x, double *delta_x, double *out_1157861628203609812) {
  err_fun(nom_x, delta_x, out_1157861628203609812);
}
void car_inv_err_fun(double *nom_x, double *true_x, double *out_3048943521048617947) {
  inv_err_fun(nom_x, true_x, out_3048943521048617947);
}
void car_H_mod_fun(double *state, double *out_6436924980861323347) {
  H_mod_fun(state, out_6436924980861323347);
}
void car_f_fun(double *state, double dt, double *out_8302405370356002085) {
  f_fun(state,  dt, out_8302405370356002085);
}
void car_F_fun(double *state, double dt, double *out_7970830068534808110) {
  F_fun(state,  dt, out_7970830068534808110);
}
void car_h_25(double *state, double *unused, double *out_7195447474562559623) {
  h_25(state, unused, out_7195447474562559623);
}
void car_H_25(double *state, double *unused, double *out_7214988249920359160) {
  H_25(state, unused, out_7214988249920359160);
}
void car_h_24(double *state, double *unused, double *out_5845787550217648453) {
  h_24(state, unused, out_5845787550217648453);
}
void car_H_24(double *state, double *unused, double *out_1929878460030518002) {
  H_24(state, unused, out_1929878460030518002);
}
void car_h_30(double *state, double *unused, double *out_2387185174253970807) {
  h_30(state, unused, out_2387185174253970807);
}
void car_H_30(double *state, double *unused, double *out_8713422865281943829) {
  H_30(state, unused, out_8713422865281943829);
}
void car_h_26(double *state, double *unused, double *out_2614488093406874838) {
  h_26(state, unused, out_2614488093406874838);
}
void car_H_26(double *state, double *unused, double *out_7927229854028391855) {
  H_26(state, unused, out_7927229854028391855);
}
void car_h_27(double *state, double *unused, double *out_6437597329401740813) {
  h_27(state, unused, out_6437597329401740813);
}
void car_H_27(double *state, double *unused, double *out_7558557896627182876) {
  H_27(state, unused, out_7558557896627182876);
}
void car_h_29(double *state, double *unused, double *out_1443675321352308372) {
  h_29(state, unused, out_1443675321352308372);
}
void car_H_29(double *state, double *unused, double *out_8203191520967551645) {
  H_29(state, unused, out_8203191520967551645);
}
void car_h_28(double *state, double *unused, double *out_1435924061740895168) {
  h_28(state, unused, out_1435924061740895168);
}
void car_H_28(double *state, double *unused, double *out_5161153535672469397) {
  H_28(state, unused, out_5161153535672469397);
}
void car_h_31(double *state, double *unused, double *out_4059747486291480197) {
  h_31(state, unused, out_4059747486291480197);
}
void car_H_31(double *state, double *unused, double *out_2847276828812951460) {
  H_31(state, unused, out_2847276828812951460);
}
void car_predict(double *in_x, double *in_P, double *in_Q, double dt) {
  predict(in_x, in_P, in_Q, dt);
}
void car_set_mass(double x) {
  set_mass(x);
}
void car_set_rotational_inertia(double x) {
  set_rotational_inertia(x);
}
void car_set_center_to_front(double x) {
  set_center_to_front(x);
}
void car_set_center_to_rear(double x) {
  set_center_to_rear(x);
}
void car_set_stiffness_front(double x) {
  set_stiffness_front(x);
}
void car_set_stiffness_rear(double x) {
  set_stiffness_rear(x);
}
}

const EKF car = {
  .name = "car",
  .kinds = { 25, 24, 30, 26, 27, 29, 28, 31 },
  .feature_kinds = {  },
  .f_fun = car_f_fun,
  .F_fun = car_F_fun,
  .err_fun = car_err_fun,
  .inv_err_fun = car_inv_err_fun,
  .H_mod_fun = car_H_mod_fun,
  .predict = car_predict,
  .hs = {
    { 25, car_h_25 },
    { 24, car_h_24 },
    { 30, car_h_30 },
    { 26, car_h_26 },
    { 27, car_h_27 },
    { 29, car_h_29 },
    { 28, car_h_28 },
    { 31, car_h_31 },
  },
  .Hs = {
    { 25, car_H_25 },
    { 24, car_H_24 },
    { 30, car_H_30 },
    { 26, car_H_26 },
    { 27, car_H_27 },
    { 29, car_H_29 },
    { 28, car_H_28 },
    { 31, car_H_31 },
  },
  .updates = {
    { 25, car_update_25 },
    { 24, car_update_24 },
    { 30, car_update_30 },
    { 26, car_update_26 },
    { 27, car_update_27 },
    { 29, car_update_29 },
    { 28, car_update_28 },
    { 31, car_update_31 },
  },
  .Hes = {
  },
  .sets = {
    { "mass", car_set_mass },
    { "rotational_inertia", car_set_rotational_inertia },
    { "center_to_front", car_set_center_to_front },
    { "center_to_rear", car_set_center_to_rear },
    { "stiffness_front", car_set_stiffness_front },
    { "stiffness_rear", car_set_stiffness_rear },
  },
  .extra_routines = {
  },
};

ekf_lib_init(car)
