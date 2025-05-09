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
void err_fun(double *nom_x, double *delta_x, double *out_899789458318900093) {
   out_899789458318900093[0] = delta_x[0] + nom_x[0];
   out_899789458318900093[1] = delta_x[1] + nom_x[1];
   out_899789458318900093[2] = delta_x[2] + nom_x[2];
   out_899789458318900093[3] = delta_x[3] + nom_x[3];
   out_899789458318900093[4] = delta_x[4] + nom_x[4];
   out_899789458318900093[5] = delta_x[5] + nom_x[5];
   out_899789458318900093[6] = delta_x[6] + nom_x[6];
   out_899789458318900093[7] = delta_x[7] + nom_x[7];
   out_899789458318900093[8] = delta_x[8] + nom_x[8];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_3551667316528293959) {
   out_3551667316528293959[0] = -nom_x[0] + true_x[0];
   out_3551667316528293959[1] = -nom_x[1] + true_x[1];
   out_3551667316528293959[2] = -nom_x[2] + true_x[2];
   out_3551667316528293959[3] = -nom_x[3] + true_x[3];
   out_3551667316528293959[4] = -nom_x[4] + true_x[4];
   out_3551667316528293959[5] = -nom_x[5] + true_x[5];
   out_3551667316528293959[6] = -nom_x[6] + true_x[6];
   out_3551667316528293959[7] = -nom_x[7] + true_x[7];
   out_3551667316528293959[8] = -nom_x[8] + true_x[8];
}
void H_mod_fun(double *state, double *out_6957613803753124388) {
   out_6957613803753124388[0] = 1.0;
   out_6957613803753124388[1] = 0;
   out_6957613803753124388[2] = 0;
   out_6957613803753124388[3] = 0;
   out_6957613803753124388[4] = 0;
   out_6957613803753124388[5] = 0;
   out_6957613803753124388[6] = 0;
   out_6957613803753124388[7] = 0;
   out_6957613803753124388[8] = 0;
   out_6957613803753124388[9] = 0;
   out_6957613803753124388[10] = 1.0;
   out_6957613803753124388[11] = 0;
   out_6957613803753124388[12] = 0;
   out_6957613803753124388[13] = 0;
   out_6957613803753124388[14] = 0;
   out_6957613803753124388[15] = 0;
   out_6957613803753124388[16] = 0;
   out_6957613803753124388[17] = 0;
   out_6957613803753124388[18] = 0;
   out_6957613803753124388[19] = 0;
   out_6957613803753124388[20] = 1.0;
   out_6957613803753124388[21] = 0;
   out_6957613803753124388[22] = 0;
   out_6957613803753124388[23] = 0;
   out_6957613803753124388[24] = 0;
   out_6957613803753124388[25] = 0;
   out_6957613803753124388[26] = 0;
   out_6957613803753124388[27] = 0;
   out_6957613803753124388[28] = 0;
   out_6957613803753124388[29] = 0;
   out_6957613803753124388[30] = 1.0;
   out_6957613803753124388[31] = 0;
   out_6957613803753124388[32] = 0;
   out_6957613803753124388[33] = 0;
   out_6957613803753124388[34] = 0;
   out_6957613803753124388[35] = 0;
   out_6957613803753124388[36] = 0;
   out_6957613803753124388[37] = 0;
   out_6957613803753124388[38] = 0;
   out_6957613803753124388[39] = 0;
   out_6957613803753124388[40] = 1.0;
   out_6957613803753124388[41] = 0;
   out_6957613803753124388[42] = 0;
   out_6957613803753124388[43] = 0;
   out_6957613803753124388[44] = 0;
   out_6957613803753124388[45] = 0;
   out_6957613803753124388[46] = 0;
   out_6957613803753124388[47] = 0;
   out_6957613803753124388[48] = 0;
   out_6957613803753124388[49] = 0;
   out_6957613803753124388[50] = 1.0;
   out_6957613803753124388[51] = 0;
   out_6957613803753124388[52] = 0;
   out_6957613803753124388[53] = 0;
   out_6957613803753124388[54] = 0;
   out_6957613803753124388[55] = 0;
   out_6957613803753124388[56] = 0;
   out_6957613803753124388[57] = 0;
   out_6957613803753124388[58] = 0;
   out_6957613803753124388[59] = 0;
   out_6957613803753124388[60] = 1.0;
   out_6957613803753124388[61] = 0;
   out_6957613803753124388[62] = 0;
   out_6957613803753124388[63] = 0;
   out_6957613803753124388[64] = 0;
   out_6957613803753124388[65] = 0;
   out_6957613803753124388[66] = 0;
   out_6957613803753124388[67] = 0;
   out_6957613803753124388[68] = 0;
   out_6957613803753124388[69] = 0;
   out_6957613803753124388[70] = 1.0;
   out_6957613803753124388[71] = 0;
   out_6957613803753124388[72] = 0;
   out_6957613803753124388[73] = 0;
   out_6957613803753124388[74] = 0;
   out_6957613803753124388[75] = 0;
   out_6957613803753124388[76] = 0;
   out_6957613803753124388[77] = 0;
   out_6957613803753124388[78] = 0;
   out_6957613803753124388[79] = 0;
   out_6957613803753124388[80] = 1.0;
}
void f_fun(double *state, double dt, double *out_2871800089490512517) {
   out_2871800089490512517[0] = state[0];
   out_2871800089490512517[1] = state[1];
   out_2871800089490512517[2] = state[2];
   out_2871800089490512517[3] = state[3];
   out_2871800089490512517[4] = state[4];
   out_2871800089490512517[5] = dt*((-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]))*state[6] - 9.8000000000000007*state[8] + stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*state[1]) + (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*state[4])) + state[5];
   out_2871800089490512517[6] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*state[4])) + state[6];
   out_2871800089490512517[7] = state[7];
   out_2871800089490512517[8] = state[8];
}
void F_fun(double *state, double dt, double *out_1047028032418201950) {
   out_1047028032418201950[0] = 1;
   out_1047028032418201950[1] = 0;
   out_1047028032418201950[2] = 0;
   out_1047028032418201950[3] = 0;
   out_1047028032418201950[4] = 0;
   out_1047028032418201950[5] = 0;
   out_1047028032418201950[6] = 0;
   out_1047028032418201950[7] = 0;
   out_1047028032418201950[8] = 0;
   out_1047028032418201950[9] = 0;
   out_1047028032418201950[10] = 1;
   out_1047028032418201950[11] = 0;
   out_1047028032418201950[12] = 0;
   out_1047028032418201950[13] = 0;
   out_1047028032418201950[14] = 0;
   out_1047028032418201950[15] = 0;
   out_1047028032418201950[16] = 0;
   out_1047028032418201950[17] = 0;
   out_1047028032418201950[18] = 0;
   out_1047028032418201950[19] = 0;
   out_1047028032418201950[20] = 1;
   out_1047028032418201950[21] = 0;
   out_1047028032418201950[22] = 0;
   out_1047028032418201950[23] = 0;
   out_1047028032418201950[24] = 0;
   out_1047028032418201950[25] = 0;
   out_1047028032418201950[26] = 0;
   out_1047028032418201950[27] = 0;
   out_1047028032418201950[28] = 0;
   out_1047028032418201950[29] = 0;
   out_1047028032418201950[30] = 1;
   out_1047028032418201950[31] = 0;
   out_1047028032418201950[32] = 0;
   out_1047028032418201950[33] = 0;
   out_1047028032418201950[34] = 0;
   out_1047028032418201950[35] = 0;
   out_1047028032418201950[36] = 0;
   out_1047028032418201950[37] = 0;
   out_1047028032418201950[38] = 0;
   out_1047028032418201950[39] = 0;
   out_1047028032418201950[40] = 1;
   out_1047028032418201950[41] = 0;
   out_1047028032418201950[42] = 0;
   out_1047028032418201950[43] = 0;
   out_1047028032418201950[44] = 0;
   out_1047028032418201950[45] = dt*(stiffness_front*(-state[2] - state[3] + state[7])/(mass*state[1]) + (-stiffness_front - stiffness_rear)*state[5]/(mass*state[4]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[6]/(mass*state[4]));
   out_1047028032418201950[46] = -dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*pow(state[1], 2));
   out_1047028032418201950[47] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_1047028032418201950[48] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_1047028032418201950[49] = dt*((-1 - (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*pow(state[4], 2)))*state[6] - (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*pow(state[4], 2)));
   out_1047028032418201950[50] = dt*(-stiffness_front*state[0] - stiffness_rear*state[0])/(mass*state[4]) + 1;
   out_1047028032418201950[51] = dt*(-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]));
   out_1047028032418201950[52] = dt*stiffness_front*state[0]/(mass*state[1]);
   out_1047028032418201950[53] = -9.8000000000000007*dt;
   out_1047028032418201950[54] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front - pow(center_to_rear, 2)*stiffness_rear)*state[6]/(rotational_inertia*state[4]));
   out_1047028032418201950[55] = -center_to_front*dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*pow(state[1], 2));
   out_1047028032418201950[56] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_1047028032418201950[57] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_1047028032418201950[58] = dt*(-(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*pow(state[4], 2)) - (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*pow(state[4], 2)));
   out_1047028032418201950[59] = dt*(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(rotational_inertia*state[4]);
   out_1047028032418201950[60] = dt*(-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])/(rotational_inertia*state[4]) + 1;
   out_1047028032418201950[61] = center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_1047028032418201950[62] = 0;
   out_1047028032418201950[63] = 0;
   out_1047028032418201950[64] = 0;
   out_1047028032418201950[65] = 0;
   out_1047028032418201950[66] = 0;
   out_1047028032418201950[67] = 0;
   out_1047028032418201950[68] = 0;
   out_1047028032418201950[69] = 0;
   out_1047028032418201950[70] = 1;
   out_1047028032418201950[71] = 0;
   out_1047028032418201950[72] = 0;
   out_1047028032418201950[73] = 0;
   out_1047028032418201950[74] = 0;
   out_1047028032418201950[75] = 0;
   out_1047028032418201950[76] = 0;
   out_1047028032418201950[77] = 0;
   out_1047028032418201950[78] = 0;
   out_1047028032418201950[79] = 0;
   out_1047028032418201950[80] = 1;
}
void h_25(double *state, double *unused, double *out_9049864551823437647) {
   out_9049864551823437647[0] = state[6];
}
void H_25(double *state, double *unused, double *out_242198979470376582) {
   out_242198979470376582[0] = 0;
   out_242198979470376582[1] = 0;
   out_242198979470376582[2] = 0;
   out_242198979470376582[3] = 0;
   out_242198979470376582[4] = 0;
   out_242198979470376582[5] = 0;
   out_242198979470376582[6] = 1;
   out_242198979470376582[7] = 0;
   out_242198979470376582[8] = 0;
}
void h_24(double *state, double *unused, double *out_6397558077660116904) {
   out_6397558077660116904[0] = state[4];
   out_6397558077660116904[1] = state[5];
}
void H_24(double *state, double *unused, double *out_6412006478637809175) {
   out_6412006478637809175[0] = 0;
   out_6412006478637809175[1] = 0;
   out_6412006478637809175[2] = 0;
   out_6412006478637809175[3] = 0;
   out_6412006478637809175[4] = 1;
   out_6412006478637809175[5] = 0;
   out_6412006478637809175[6] = 0;
   out_6412006478637809175[7] = 0;
   out_6412006478637809175[8] = 0;
   out_6412006478637809175[9] = 0;
   out_6412006478637809175[10] = 0;
   out_6412006478637809175[11] = 0;
   out_6412006478637809175[12] = 0;
   out_6412006478637809175[13] = 0;
   out_6412006478637809175[14] = 1;
   out_6412006478637809175[15] = 0;
   out_6412006478637809175[16] = 0;
   out_6412006478637809175[17] = 0;
}
void h_30(double *state, double *unused, double *out_2462036625347142484) {
   out_2462036625347142484[0] = state[4];
}
void H_30(double *state, double *unused, double *out_4769895309597984780) {
   out_4769895309597984780[0] = 0;
   out_4769895309597984780[1] = 0;
   out_4769895309597984780[2] = 0;
   out_4769895309597984780[3] = 0;
   out_4769895309597984780[4] = 1;
   out_4769895309597984780[5] = 0;
   out_4769895309597984780[6] = 0;
   out_4769895309597984780[7] = 0;
   out_4769895309597984780[8] = 0;
}
void h_26(double *state, double *unused, double *out_142368695461555167) {
   out_142368695461555167[0] = state[7];
}
void H_26(double *state, double *unused, double *out_3983702298344432806) {
   out_3983702298344432806[0] = 0;
   out_3983702298344432806[1] = 0;
   out_3983702298344432806[2] = 0;
   out_3983702298344432806[3] = 0;
   out_3983702298344432806[4] = 0;
   out_3983702298344432806[5] = 0;
   out_3983702298344432806[6] = 0;
   out_3983702298344432806[7] = 1;
   out_3983702298344432806[8] = 0;
}
void h_27(double *state, double *unused, double *out_3413955682579972216) {
   out_3413955682579972216[0] = state[3];
}
void H_27(double *state, double *unused, double *out_2546301238414041563) {
   out_2546301238414041563[0] = 0;
   out_2546301238414041563[1] = 0;
   out_2546301238414041563[2] = 0;
   out_2546301238414041563[3] = 1;
   out_2546301238414041563[4] = 0;
   out_2546301238414041563[5] = 0;
   out_2546301238414041563[6] = 0;
   out_2546301238414041563[7] = 0;
   out_2546301238414041563[8] = 0;
}
void h_29(double *state, double *unused, double *out_7424416623028435716) {
   out_7424416623028435716[0] = state[1];
}
void H_29(double *state, double *unused, double *out_4259663965283592596) {
   out_4259663965283592596[0] = 0;
   out_4259663965283592596[1] = 1;
   out_4259663965283592596[2] = 0;
   out_4259663965283592596[3] = 0;
   out_4259663965283592596[4] = 0;
   out_4259663965283592596[5] = 0;
   out_4259663965283592596[6] = 0;
   out_4259663965283592596[7] = 0;
   out_4259663965283592596[8] = 0;
}
void h_28(double *state, double *unused, double *out_9056685553660929051) {
   out_9056685553660929051[0] = state[0];
}
void H_28(double *state, double *unused, double *out_9104681091356428446) {
   out_9104681091356428446[0] = 1;
   out_9104681091356428446[1] = 0;
   out_9104681091356428446[2] = 0;
   out_9104681091356428446[3] = 0;
   out_9104681091356428446[4] = 0;
   out_9104681091356428446[5] = 0;
   out_9104681091356428446[6] = 0;
   out_9104681091356428446[7] = 0;
   out_9104681091356428446[8] = 0;
}
void h_31(double *state, double *unused, double *out_5261590185666782321) {
   out_5261590185666782321[0] = state[8];
}
void H_31(double *state, double *unused, double *out_4609910400577784282) {
   out_4609910400577784282[0] = 0;
   out_4609910400577784282[1] = 0;
   out_4609910400577784282[2] = 0;
   out_4609910400577784282[3] = 0;
   out_4609910400577784282[4] = 0;
   out_4609910400577784282[5] = 0;
   out_4609910400577784282[6] = 0;
   out_4609910400577784282[7] = 0;
   out_4609910400577784282[8] = 1;
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
void car_err_fun(double *nom_x, double *delta_x, double *out_899789458318900093) {
  err_fun(nom_x, delta_x, out_899789458318900093);
}
void car_inv_err_fun(double *nom_x, double *true_x, double *out_3551667316528293959) {
  inv_err_fun(nom_x, true_x, out_3551667316528293959);
}
void car_H_mod_fun(double *state, double *out_6957613803753124388) {
  H_mod_fun(state, out_6957613803753124388);
}
void car_f_fun(double *state, double dt, double *out_2871800089490512517) {
  f_fun(state,  dt, out_2871800089490512517);
}
void car_F_fun(double *state, double dt, double *out_1047028032418201950) {
  F_fun(state,  dt, out_1047028032418201950);
}
void car_h_25(double *state, double *unused, double *out_9049864551823437647) {
  h_25(state, unused, out_9049864551823437647);
}
void car_H_25(double *state, double *unused, double *out_242198979470376582) {
  H_25(state, unused, out_242198979470376582);
}
void car_h_24(double *state, double *unused, double *out_6397558077660116904) {
  h_24(state, unused, out_6397558077660116904);
}
void car_H_24(double *state, double *unused, double *out_6412006478637809175) {
  H_24(state, unused, out_6412006478637809175);
}
void car_h_30(double *state, double *unused, double *out_2462036625347142484) {
  h_30(state, unused, out_2462036625347142484);
}
void car_H_30(double *state, double *unused, double *out_4769895309597984780) {
  H_30(state, unused, out_4769895309597984780);
}
void car_h_26(double *state, double *unused, double *out_142368695461555167) {
  h_26(state, unused, out_142368695461555167);
}
void car_H_26(double *state, double *unused, double *out_3983702298344432806) {
  H_26(state, unused, out_3983702298344432806);
}
void car_h_27(double *state, double *unused, double *out_3413955682579972216) {
  h_27(state, unused, out_3413955682579972216);
}
void car_H_27(double *state, double *unused, double *out_2546301238414041563) {
  H_27(state, unused, out_2546301238414041563);
}
void car_h_29(double *state, double *unused, double *out_7424416623028435716) {
  h_29(state, unused, out_7424416623028435716);
}
void car_H_29(double *state, double *unused, double *out_4259663965283592596) {
  H_29(state, unused, out_4259663965283592596);
}
void car_h_28(double *state, double *unused, double *out_9056685553660929051) {
  h_28(state, unused, out_9056685553660929051);
}
void car_H_28(double *state, double *unused, double *out_9104681091356428446) {
  H_28(state, unused, out_9104681091356428446);
}
void car_h_31(double *state, double *unused, double *out_5261590185666782321) {
  h_31(state, unused, out_5261590185666782321);
}
void car_H_31(double *state, double *unused, double *out_4609910400577784282) {
  H_31(state, unused, out_4609910400577784282);
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
