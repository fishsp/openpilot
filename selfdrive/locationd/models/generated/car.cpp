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
 *                       Code generated with SymPy 1.12                       *
 *                                                                            *
 *              See http://www.sympy.org/ for more information.               *
 *                                                                            *
 *                         This file is part of 'ekf'                         *
 ******************************************************************************/
void err_fun(double *nom_x, double *delta_x, double *out_4743726722773991975) {
   out_4743726722773991975[0] = delta_x[0] + nom_x[0];
   out_4743726722773991975[1] = delta_x[1] + nom_x[1];
   out_4743726722773991975[2] = delta_x[2] + nom_x[2];
   out_4743726722773991975[3] = delta_x[3] + nom_x[3];
   out_4743726722773991975[4] = delta_x[4] + nom_x[4];
   out_4743726722773991975[5] = delta_x[5] + nom_x[5];
   out_4743726722773991975[6] = delta_x[6] + nom_x[6];
   out_4743726722773991975[7] = delta_x[7] + nom_x[7];
   out_4743726722773991975[8] = delta_x[8] + nom_x[8];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_2841966670875871855) {
   out_2841966670875871855[0] = -nom_x[0] + true_x[0];
   out_2841966670875871855[1] = -nom_x[1] + true_x[1];
   out_2841966670875871855[2] = -nom_x[2] + true_x[2];
   out_2841966670875871855[3] = -nom_x[3] + true_x[3];
   out_2841966670875871855[4] = -nom_x[4] + true_x[4];
   out_2841966670875871855[5] = -nom_x[5] + true_x[5];
   out_2841966670875871855[6] = -nom_x[6] + true_x[6];
   out_2841966670875871855[7] = -nom_x[7] + true_x[7];
   out_2841966670875871855[8] = -nom_x[8] + true_x[8];
}
void H_mod_fun(double *state, double *out_7340213240266504600) {
   out_7340213240266504600[0] = 1.0;
   out_7340213240266504600[1] = 0;
   out_7340213240266504600[2] = 0;
   out_7340213240266504600[3] = 0;
   out_7340213240266504600[4] = 0;
   out_7340213240266504600[5] = 0;
   out_7340213240266504600[6] = 0;
   out_7340213240266504600[7] = 0;
   out_7340213240266504600[8] = 0;
   out_7340213240266504600[9] = 0;
   out_7340213240266504600[10] = 1.0;
   out_7340213240266504600[11] = 0;
   out_7340213240266504600[12] = 0;
   out_7340213240266504600[13] = 0;
   out_7340213240266504600[14] = 0;
   out_7340213240266504600[15] = 0;
   out_7340213240266504600[16] = 0;
   out_7340213240266504600[17] = 0;
   out_7340213240266504600[18] = 0;
   out_7340213240266504600[19] = 0;
   out_7340213240266504600[20] = 1.0;
   out_7340213240266504600[21] = 0;
   out_7340213240266504600[22] = 0;
   out_7340213240266504600[23] = 0;
   out_7340213240266504600[24] = 0;
   out_7340213240266504600[25] = 0;
   out_7340213240266504600[26] = 0;
   out_7340213240266504600[27] = 0;
   out_7340213240266504600[28] = 0;
   out_7340213240266504600[29] = 0;
   out_7340213240266504600[30] = 1.0;
   out_7340213240266504600[31] = 0;
   out_7340213240266504600[32] = 0;
   out_7340213240266504600[33] = 0;
   out_7340213240266504600[34] = 0;
   out_7340213240266504600[35] = 0;
   out_7340213240266504600[36] = 0;
   out_7340213240266504600[37] = 0;
   out_7340213240266504600[38] = 0;
   out_7340213240266504600[39] = 0;
   out_7340213240266504600[40] = 1.0;
   out_7340213240266504600[41] = 0;
   out_7340213240266504600[42] = 0;
   out_7340213240266504600[43] = 0;
   out_7340213240266504600[44] = 0;
   out_7340213240266504600[45] = 0;
   out_7340213240266504600[46] = 0;
   out_7340213240266504600[47] = 0;
   out_7340213240266504600[48] = 0;
   out_7340213240266504600[49] = 0;
   out_7340213240266504600[50] = 1.0;
   out_7340213240266504600[51] = 0;
   out_7340213240266504600[52] = 0;
   out_7340213240266504600[53] = 0;
   out_7340213240266504600[54] = 0;
   out_7340213240266504600[55] = 0;
   out_7340213240266504600[56] = 0;
   out_7340213240266504600[57] = 0;
   out_7340213240266504600[58] = 0;
   out_7340213240266504600[59] = 0;
   out_7340213240266504600[60] = 1.0;
   out_7340213240266504600[61] = 0;
   out_7340213240266504600[62] = 0;
   out_7340213240266504600[63] = 0;
   out_7340213240266504600[64] = 0;
   out_7340213240266504600[65] = 0;
   out_7340213240266504600[66] = 0;
   out_7340213240266504600[67] = 0;
   out_7340213240266504600[68] = 0;
   out_7340213240266504600[69] = 0;
   out_7340213240266504600[70] = 1.0;
   out_7340213240266504600[71] = 0;
   out_7340213240266504600[72] = 0;
   out_7340213240266504600[73] = 0;
   out_7340213240266504600[74] = 0;
   out_7340213240266504600[75] = 0;
   out_7340213240266504600[76] = 0;
   out_7340213240266504600[77] = 0;
   out_7340213240266504600[78] = 0;
   out_7340213240266504600[79] = 0;
   out_7340213240266504600[80] = 1.0;
}
void f_fun(double *state, double dt, double *out_3713690376114026963) {
   out_3713690376114026963[0] = state[0];
   out_3713690376114026963[1] = state[1];
   out_3713690376114026963[2] = state[2];
   out_3713690376114026963[3] = state[3];
   out_3713690376114026963[4] = state[4];
   out_3713690376114026963[5] = dt*((-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]))*state[6] - 9.8000000000000007*state[8] + stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*state[1]) + (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*state[4])) + state[5];
   out_3713690376114026963[6] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*state[4])) + state[6];
   out_3713690376114026963[7] = state[7];
   out_3713690376114026963[8] = state[8];
}
void F_fun(double *state, double dt, double *out_4741690943052406499) {
   out_4741690943052406499[0] = 1;
   out_4741690943052406499[1] = 0;
   out_4741690943052406499[2] = 0;
   out_4741690943052406499[3] = 0;
   out_4741690943052406499[4] = 0;
   out_4741690943052406499[5] = 0;
   out_4741690943052406499[6] = 0;
   out_4741690943052406499[7] = 0;
   out_4741690943052406499[8] = 0;
   out_4741690943052406499[9] = 0;
   out_4741690943052406499[10] = 1;
   out_4741690943052406499[11] = 0;
   out_4741690943052406499[12] = 0;
   out_4741690943052406499[13] = 0;
   out_4741690943052406499[14] = 0;
   out_4741690943052406499[15] = 0;
   out_4741690943052406499[16] = 0;
   out_4741690943052406499[17] = 0;
   out_4741690943052406499[18] = 0;
   out_4741690943052406499[19] = 0;
   out_4741690943052406499[20] = 1;
   out_4741690943052406499[21] = 0;
   out_4741690943052406499[22] = 0;
   out_4741690943052406499[23] = 0;
   out_4741690943052406499[24] = 0;
   out_4741690943052406499[25] = 0;
   out_4741690943052406499[26] = 0;
   out_4741690943052406499[27] = 0;
   out_4741690943052406499[28] = 0;
   out_4741690943052406499[29] = 0;
   out_4741690943052406499[30] = 1;
   out_4741690943052406499[31] = 0;
   out_4741690943052406499[32] = 0;
   out_4741690943052406499[33] = 0;
   out_4741690943052406499[34] = 0;
   out_4741690943052406499[35] = 0;
   out_4741690943052406499[36] = 0;
   out_4741690943052406499[37] = 0;
   out_4741690943052406499[38] = 0;
   out_4741690943052406499[39] = 0;
   out_4741690943052406499[40] = 1;
   out_4741690943052406499[41] = 0;
   out_4741690943052406499[42] = 0;
   out_4741690943052406499[43] = 0;
   out_4741690943052406499[44] = 0;
   out_4741690943052406499[45] = dt*(stiffness_front*(-state[2] - state[3] + state[7])/(mass*state[1]) + (-stiffness_front - stiffness_rear)*state[5]/(mass*state[4]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[6]/(mass*state[4]));
   out_4741690943052406499[46] = -dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*pow(state[1], 2));
   out_4741690943052406499[47] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_4741690943052406499[48] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_4741690943052406499[49] = dt*((-1 - (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*pow(state[4], 2)))*state[6] - (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*pow(state[4], 2)));
   out_4741690943052406499[50] = dt*(-stiffness_front*state[0] - stiffness_rear*state[0])/(mass*state[4]) + 1;
   out_4741690943052406499[51] = dt*(-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]));
   out_4741690943052406499[52] = dt*stiffness_front*state[0]/(mass*state[1]);
   out_4741690943052406499[53] = -9.8000000000000007*dt;
   out_4741690943052406499[54] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front - pow(center_to_rear, 2)*stiffness_rear)*state[6]/(rotational_inertia*state[4]));
   out_4741690943052406499[55] = -center_to_front*dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*pow(state[1], 2));
   out_4741690943052406499[56] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_4741690943052406499[57] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_4741690943052406499[58] = dt*(-(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*pow(state[4], 2)) - (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*pow(state[4], 2)));
   out_4741690943052406499[59] = dt*(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(rotational_inertia*state[4]);
   out_4741690943052406499[60] = dt*(-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])/(rotational_inertia*state[4]) + 1;
   out_4741690943052406499[61] = center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_4741690943052406499[62] = 0;
   out_4741690943052406499[63] = 0;
   out_4741690943052406499[64] = 0;
   out_4741690943052406499[65] = 0;
   out_4741690943052406499[66] = 0;
   out_4741690943052406499[67] = 0;
   out_4741690943052406499[68] = 0;
   out_4741690943052406499[69] = 0;
   out_4741690943052406499[70] = 1;
   out_4741690943052406499[71] = 0;
   out_4741690943052406499[72] = 0;
   out_4741690943052406499[73] = 0;
   out_4741690943052406499[74] = 0;
   out_4741690943052406499[75] = 0;
   out_4741690943052406499[76] = 0;
   out_4741690943052406499[77] = 0;
   out_4741690943052406499[78] = 0;
   out_4741690943052406499[79] = 0;
   out_4741690943052406499[80] = 1;
}
void h_25(double *state, double *unused, double *out_8600294221188020871) {
   out_8600294221188020871[0] = state[6];
}
void H_25(double *state, double *unused, double *out_2073788871405630401) {
   out_2073788871405630401[0] = 0;
   out_2073788871405630401[1] = 0;
   out_2073788871405630401[2] = 0;
   out_2073788871405630401[3] = 0;
   out_2073788871405630401[4] = 0;
   out_2073788871405630401[5] = 0;
   out_2073788871405630401[6] = 1;
   out_2073788871405630401[7] = 0;
   out_2073788871405630401[8] = 0;
}
void h_24(double *state, double *unused, double *out_8767666115400015860) {
   out_8767666115400015860[0] = state[4];
   out_8767666115400015860[1] = state[5];
}
void H_24(double *state, double *unused, double *out_4501782935185887700) {
   out_4501782935185887700[0] = 0;
   out_4501782935185887700[1] = 0;
   out_4501782935185887700[2] = 0;
   out_4501782935185887700[3] = 0;
   out_4501782935185887700[4] = 1;
   out_4501782935185887700[5] = 0;
   out_4501782935185887700[6] = 0;
   out_4501782935185887700[7] = 0;
   out_4501782935185887700[8] = 0;
   out_4501782935185887700[9] = 0;
   out_4501782935185887700[10] = 0;
   out_4501782935185887700[11] = 0;
   out_4501782935185887700[12] = 0;
   out_4501782935185887700[13] = 0;
   out_4501782935185887700[14] = 1;
   out_4501782935185887700[15] = 0;
   out_4501782935185887700[16] = 0;
   out_4501782935185887700[17] = 0;
}
void h_30(double *state, double *unused, double *out_3482855874219714884) {
   out_3482855874219714884[0] = state[4];
}
void H_30(double *state, double *unused, double *out_4842901470085986354) {
   out_4842901470085986354[0] = 0;
   out_4842901470085986354[1] = 0;
   out_4842901470085986354[2] = 0;
   out_4842901470085986354[3] = 0;
   out_4842901470085986354[4] = 1;
   out_4842901470085986354[5] = 0;
   out_4842901470085986354[6] = 0;
   out_4842901470085986354[7] = 0;
   out_4842901470085986354[8] = 0;
}
void h_26(double *state, double *unused, double *out_4086171758896192401) {
   out_4086171758896192401[0] = state[7];
}
void H_26(double *state, double *unused, double *out_5815292190279686625) {
   out_5815292190279686625[0] = 0;
   out_5815292190279686625[1] = 0;
   out_5815292190279686625[2] = 0;
   out_5815292190279686625[3] = 0;
   out_5815292190279686625[4] = 0;
   out_5815292190279686625[5] = 0;
   out_5815292190279686625[6] = 0;
   out_5815292190279686625[7] = 1;
   out_5815292190279686625[8] = 0;
}
void h_27(double *state, double *unused, double *out_7432550420111879134) {
   out_7432550420111879134[0] = state[3];
}
void H_27(double *state, double *unused, double *out_2668138158285561443) {
   out_2668138158285561443[0] = 0;
   out_2668138158285561443[1] = 0;
   out_2668138158285561443[2] = 0;
   out_2668138158285561443[3] = 1;
   out_2668138158285561443[4] = 0;
   out_2668138158285561443[5] = 0;
   out_2668138158285561443[6] = 0;
   out_2668138158285561443[7] = 0;
   out_2668138158285561443[8] = 0;
}
void h_29(double *state, double *unused, double *out_1207327056606662750) {
   out_1207327056606662750[0] = state[1];
}
void H_29(double *state, double *unused, double *out_5353132814400378538) {
   out_5353132814400378538[0] = 0;
   out_5353132814400378538[1] = 1;
   out_5353132814400378538[2] = 0;
   out_5353132814400378538[3] = 0;
   out_5353132814400378538[4] = 0;
   out_5353132814400378538[5] = 0;
   out_5353132814400378538[6] = 0;
   out_5353132814400378538[7] = 0;
   out_5353132814400378538[8] = 0;
}
void h_28(double *state, double *unused, double *out_6012520385936826837) {
   out_6012520385936826837[0] = state[0];
}
void H_28(double *state, double *unused, double *out_270733797330847964) {
   out_270733797330847964[0] = 1;
   out_270733797330847964[1] = 0;
   out_270733797330847964[2] = 0;
   out_270733797330847964[3] = 0;
   out_270733797330847964[4] = 0;
   out_270733797330847964[5] = 0;
   out_270733797330847964[6] = 0;
   out_270733797330847964[7] = 0;
   out_270733797330847964[8] = 0;
}
void h_31(double *state, double *unused, double *out_8757480889539150016) {
   out_8757480889539150016[0] = state[8];
}
void H_31(double *state, double *unused, double *out_2043142909528669973) {
   out_2043142909528669973[0] = 0;
   out_2043142909528669973[1] = 0;
   out_2043142909528669973[2] = 0;
   out_2043142909528669973[3] = 0;
   out_2043142909528669973[4] = 0;
   out_2043142909528669973[5] = 0;
   out_2043142909528669973[6] = 0;
   out_2043142909528669973[7] = 0;
   out_2043142909528669973[8] = 1;
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
void car_err_fun(double *nom_x, double *delta_x, double *out_4743726722773991975) {
  err_fun(nom_x, delta_x, out_4743726722773991975);
}
void car_inv_err_fun(double *nom_x, double *true_x, double *out_2841966670875871855) {
  inv_err_fun(nom_x, true_x, out_2841966670875871855);
}
void car_H_mod_fun(double *state, double *out_7340213240266504600) {
  H_mod_fun(state, out_7340213240266504600);
}
void car_f_fun(double *state, double dt, double *out_3713690376114026963) {
  f_fun(state,  dt, out_3713690376114026963);
}
void car_F_fun(double *state, double dt, double *out_4741690943052406499) {
  F_fun(state,  dt, out_4741690943052406499);
}
void car_h_25(double *state, double *unused, double *out_8600294221188020871) {
  h_25(state, unused, out_8600294221188020871);
}
void car_H_25(double *state, double *unused, double *out_2073788871405630401) {
  H_25(state, unused, out_2073788871405630401);
}
void car_h_24(double *state, double *unused, double *out_8767666115400015860) {
  h_24(state, unused, out_8767666115400015860);
}
void car_H_24(double *state, double *unused, double *out_4501782935185887700) {
  H_24(state, unused, out_4501782935185887700);
}
void car_h_30(double *state, double *unused, double *out_3482855874219714884) {
  h_30(state, unused, out_3482855874219714884);
}
void car_H_30(double *state, double *unused, double *out_4842901470085986354) {
  H_30(state, unused, out_4842901470085986354);
}
void car_h_26(double *state, double *unused, double *out_4086171758896192401) {
  h_26(state, unused, out_4086171758896192401);
}
void car_H_26(double *state, double *unused, double *out_5815292190279686625) {
  H_26(state, unused, out_5815292190279686625);
}
void car_h_27(double *state, double *unused, double *out_7432550420111879134) {
  h_27(state, unused, out_7432550420111879134);
}
void car_H_27(double *state, double *unused, double *out_2668138158285561443) {
  H_27(state, unused, out_2668138158285561443);
}
void car_h_29(double *state, double *unused, double *out_1207327056606662750) {
  h_29(state, unused, out_1207327056606662750);
}
void car_H_29(double *state, double *unused, double *out_5353132814400378538) {
  H_29(state, unused, out_5353132814400378538);
}
void car_h_28(double *state, double *unused, double *out_6012520385936826837) {
  h_28(state, unused, out_6012520385936826837);
}
void car_H_28(double *state, double *unused, double *out_270733797330847964) {
  H_28(state, unused, out_270733797330847964);
}
void car_h_31(double *state, double *unused, double *out_8757480889539150016) {
  h_31(state, unused, out_8757480889539150016);
}
void car_H_31(double *state, double *unused, double *out_2043142909528669973) {
  H_31(state, unused, out_2043142909528669973);
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
