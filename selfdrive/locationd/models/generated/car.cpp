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
void err_fun(double *nom_x, double *delta_x, double *out_7447692140094294189) {
   out_7447692140094294189[0] = delta_x[0] + nom_x[0];
   out_7447692140094294189[1] = delta_x[1] + nom_x[1];
   out_7447692140094294189[2] = delta_x[2] + nom_x[2];
   out_7447692140094294189[3] = delta_x[3] + nom_x[3];
   out_7447692140094294189[4] = delta_x[4] + nom_x[4];
   out_7447692140094294189[5] = delta_x[5] + nom_x[5];
   out_7447692140094294189[6] = delta_x[6] + nom_x[6];
   out_7447692140094294189[7] = delta_x[7] + nom_x[7];
   out_7447692140094294189[8] = delta_x[8] + nom_x[8];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_190421467442803104) {
   out_190421467442803104[0] = -nom_x[0] + true_x[0];
   out_190421467442803104[1] = -nom_x[1] + true_x[1];
   out_190421467442803104[2] = -nom_x[2] + true_x[2];
   out_190421467442803104[3] = -nom_x[3] + true_x[3];
   out_190421467442803104[4] = -nom_x[4] + true_x[4];
   out_190421467442803104[5] = -nom_x[5] + true_x[5];
   out_190421467442803104[6] = -nom_x[6] + true_x[6];
   out_190421467442803104[7] = -nom_x[7] + true_x[7];
   out_190421467442803104[8] = -nom_x[8] + true_x[8];
}
void H_mod_fun(double *state, double *out_6314959913246026197) {
   out_6314959913246026197[0] = 1.0;
   out_6314959913246026197[1] = 0;
   out_6314959913246026197[2] = 0;
   out_6314959913246026197[3] = 0;
   out_6314959913246026197[4] = 0;
   out_6314959913246026197[5] = 0;
   out_6314959913246026197[6] = 0;
   out_6314959913246026197[7] = 0;
   out_6314959913246026197[8] = 0;
   out_6314959913246026197[9] = 0;
   out_6314959913246026197[10] = 1.0;
   out_6314959913246026197[11] = 0;
   out_6314959913246026197[12] = 0;
   out_6314959913246026197[13] = 0;
   out_6314959913246026197[14] = 0;
   out_6314959913246026197[15] = 0;
   out_6314959913246026197[16] = 0;
   out_6314959913246026197[17] = 0;
   out_6314959913246026197[18] = 0;
   out_6314959913246026197[19] = 0;
   out_6314959913246026197[20] = 1.0;
   out_6314959913246026197[21] = 0;
   out_6314959913246026197[22] = 0;
   out_6314959913246026197[23] = 0;
   out_6314959913246026197[24] = 0;
   out_6314959913246026197[25] = 0;
   out_6314959913246026197[26] = 0;
   out_6314959913246026197[27] = 0;
   out_6314959913246026197[28] = 0;
   out_6314959913246026197[29] = 0;
   out_6314959913246026197[30] = 1.0;
   out_6314959913246026197[31] = 0;
   out_6314959913246026197[32] = 0;
   out_6314959913246026197[33] = 0;
   out_6314959913246026197[34] = 0;
   out_6314959913246026197[35] = 0;
   out_6314959913246026197[36] = 0;
   out_6314959913246026197[37] = 0;
   out_6314959913246026197[38] = 0;
   out_6314959913246026197[39] = 0;
   out_6314959913246026197[40] = 1.0;
   out_6314959913246026197[41] = 0;
   out_6314959913246026197[42] = 0;
   out_6314959913246026197[43] = 0;
   out_6314959913246026197[44] = 0;
   out_6314959913246026197[45] = 0;
   out_6314959913246026197[46] = 0;
   out_6314959913246026197[47] = 0;
   out_6314959913246026197[48] = 0;
   out_6314959913246026197[49] = 0;
   out_6314959913246026197[50] = 1.0;
   out_6314959913246026197[51] = 0;
   out_6314959913246026197[52] = 0;
   out_6314959913246026197[53] = 0;
   out_6314959913246026197[54] = 0;
   out_6314959913246026197[55] = 0;
   out_6314959913246026197[56] = 0;
   out_6314959913246026197[57] = 0;
   out_6314959913246026197[58] = 0;
   out_6314959913246026197[59] = 0;
   out_6314959913246026197[60] = 1.0;
   out_6314959913246026197[61] = 0;
   out_6314959913246026197[62] = 0;
   out_6314959913246026197[63] = 0;
   out_6314959913246026197[64] = 0;
   out_6314959913246026197[65] = 0;
   out_6314959913246026197[66] = 0;
   out_6314959913246026197[67] = 0;
   out_6314959913246026197[68] = 0;
   out_6314959913246026197[69] = 0;
   out_6314959913246026197[70] = 1.0;
   out_6314959913246026197[71] = 0;
   out_6314959913246026197[72] = 0;
   out_6314959913246026197[73] = 0;
   out_6314959913246026197[74] = 0;
   out_6314959913246026197[75] = 0;
   out_6314959913246026197[76] = 0;
   out_6314959913246026197[77] = 0;
   out_6314959913246026197[78] = 0;
   out_6314959913246026197[79] = 0;
   out_6314959913246026197[80] = 1.0;
}
void f_fun(double *state, double dt, double *out_3201321820069077199) {
   out_3201321820069077199[0] = state[0];
   out_3201321820069077199[1] = state[1];
   out_3201321820069077199[2] = state[2];
   out_3201321820069077199[3] = state[3];
   out_3201321820069077199[4] = state[4];
   out_3201321820069077199[5] = dt*((-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]))*state[6] - 9.8000000000000007*state[8] + stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*state[1]) + (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*state[4])) + state[5];
   out_3201321820069077199[6] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*state[4])) + state[6];
   out_3201321820069077199[7] = state[7];
   out_3201321820069077199[8] = state[8];
}
void F_fun(double *state, double dt, double *out_3009618409394531093) {
   out_3009618409394531093[0] = 1;
   out_3009618409394531093[1] = 0;
   out_3009618409394531093[2] = 0;
   out_3009618409394531093[3] = 0;
   out_3009618409394531093[4] = 0;
   out_3009618409394531093[5] = 0;
   out_3009618409394531093[6] = 0;
   out_3009618409394531093[7] = 0;
   out_3009618409394531093[8] = 0;
   out_3009618409394531093[9] = 0;
   out_3009618409394531093[10] = 1;
   out_3009618409394531093[11] = 0;
   out_3009618409394531093[12] = 0;
   out_3009618409394531093[13] = 0;
   out_3009618409394531093[14] = 0;
   out_3009618409394531093[15] = 0;
   out_3009618409394531093[16] = 0;
   out_3009618409394531093[17] = 0;
   out_3009618409394531093[18] = 0;
   out_3009618409394531093[19] = 0;
   out_3009618409394531093[20] = 1;
   out_3009618409394531093[21] = 0;
   out_3009618409394531093[22] = 0;
   out_3009618409394531093[23] = 0;
   out_3009618409394531093[24] = 0;
   out_3009618409394531093[25] = 0;
   out_3009618409394531093[26] = 0;
   out_3009618409394531093[27] = 0;
   out_3009618409394531093[28] = 0;
   out_3009618409394531093[29] = 0;
   out_3009618409394531093[30] = 1;
   out_3009618409394531093[31] = 0;
   out_3009618409394531093[32] = 0;
   out_3009618409394531093[33] = 0;
   out_3009618409394531093[34] = 0;
   out_3009618409394531093[35] = 0;
   out_3009618409394531093[36] = 0;
   out_3009618409394531093[37] = 0;
   out_3009618409394531093[38] = 0;
   out_3009618409394531093[39] = 0;
   out_3009618409394531093[40] = 1;
   out_3009618409394531093[41] = 0;
   out_3009618409394531093[42] = 0;
   out_3009618409394531093[43] = 0;
   out_3009618409394531093[44] = 0;
   out_3009618409394531093[45] = dt*(stiffness_front*(-state[2] - state[3] + state[7])/(mass*state[1]) + (-stiffness_front - stiffness_rear)*state[5]/(mass*state[4]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[6]/(mass*state[4]));
   out_3009618409394531093[46] = -dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*pow(state[1], 2));
   out_3009618409394531093[47] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_3009618409394531093[48] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_3009618409394531093[49] = dt*((-1 - (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*pow(state[4], 2)))*state[6] - (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*pow(state[4], 2)));
   out_3009618409394531093[50] = dt*(-stiffness_front*state[0] - stiffness_rear*state[0])/(mass*state[4]) + 1;
   out_3009618409394531093[51] = dt*(-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]));
   out_3009618409394531093[52] = dt*stiffness_front*state[0]/(mass*state[1]);
   out_3009618409394531093[53] = -9.8000000000000007*dt;
   out_3009618409394531093[54] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front - pow(center_to_rear, 2)*stiffness_rear)*state[6]/(rotational_inertia*state[4]));
   out_3009618409394531093[55] = -center_to_front*dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*pow(state[1], 2));
   out_3009618409394531093[56] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_3009618409394531093[57] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_3009618409394531093[58] = dt*(-(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*pow(state[4], 2)) - (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*pow(state[4], 2)));
   out_3009618409394531093[59] = dt*(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(rotational_inertia*state[4]);
   out_3009618409394531093[60] = dt*(-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])/(rotational_inertia*state[4]) + 1;
   out_3009618409394531093[61] = center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_3009618409394531093[62] = 0;
   out_3009618409394531093[63] = 0;
   out_3009618409394531093[64] = 0;
   out_3009618409394531093[65] = 0;
   out_3009618409394531093[66] = 0;
   out_3009618409394531093[67] = 0;
   out_3009618409394531093[68] = 0;
   out_3009618409394531093[69] = 0;
   out_3009618409394531093[70] = 1;
   out_3009618409394531093[71] = 0;
   out_3009618409394531093[72] = 0;
   out_3009618409394531093[73] = 0;
   out_3009618409394531093[74] = 0;
   out_3009618409394531093[75] = 0;
   out_3009618409394531093[76] = 0;
   out_3009618409394531093[77] = 0;
   out_3009618409394531093[78] = 0;
   out_3009618409394531093[79] = 0;
   out_3009618409394531093[80] = 1;
}
void h_25(double *state, double *unused, double *out_787725499327780767) {
   out_787725499327780767[0] = state[6];
}
void H_25(double *state, double *unused, double *out_8114437426866176332) {
   out_8114437426866176332[0] = 0;
   out_8114437426866176332[1] = 0;
   out_8114437426866176332[2] = 0;
   out_8114437426866176332[3] = 0;
   out_8114437426866176332[4] = 0;
   out_8114437426866176332[5] = 0;
   out_8114437426866176332[6] = 1;
   out_8114437426866176332[7] = 0;
   out_8114437426866176332[8] = 0;
}
void h_24(double *state, double *unused, double *out_4758358035022413509) {
   out_4758358035022413509[0] = state[4];
   out_4758358035022413509[1] = state[5];
}
void H_24(double *state, double *unused, double *out_6701409178827767495) {
   out_6701409178827767495[0] = 0;
   out_6701409178827767495[1] = 0;
   out_6701409178827767495[2] = 0;
   out_6701409178827767495[3] = 0;
   out_6701409178827767495[4] = 1;
   out_6701409178827767495[5] = 0;
   out_6701409178827767495[6] = 0;
   out_6701409178827767495[7] = 0;
   out_6701409178827767495[8] = 0;
   out_6701409178827767495[9] = 0;
   out_6701409178827767495[10] = 0;
   out_6701409178827767495[11] = 0;
   out_6701409178827767495[12] = 0;
   out_6701409178827767495[13] = 0;
   out_6701409178827767495[14] = 1;
   out_6701409178827767495[15] = 0;
   out_6701409178827767495[16] = 0;
   out_6701409178827767495[17] = 0;
}
void h_30(double *state, double *unused, double *out_2208667425506130393) {
   out_2208667425506130393[0] = state[4];
}
void H_30(double *state, double *unused, double *out_5596104468358927705) {
   out_5596104468358927705[0] = 0;
   out_5596104468358927705[1] = 0;
   out_5596104468358927705[2] = 0;
   out_5596104468358927705[3] = 0;
   out_5596104468358927705[4] = 1;
   out_5596104468358927705[5] = 0;
   out_5596104468358927705[6] = 0;
   out_5596104468358927705[7] = 0;
   out_5596104468358927705[8] = 0;
}
void h_26(double *state, double *unused, double *out_3899115260241811353) {
   out_3899115260241811353[0] = state[7];
}
void H_26(double *state, double *unused, double *out_6590803327969319060) {
   out_6590803327969319060[0] = 0;
   out_6590803327969319060[1] = 0;
   out_6590803327969319060[2] = 0;
   out_6590803327969319060[3] = 0;
   out_6590803327969319060[4] = 0;
   out_6590803327969319060[5] = 0;
   out_6590803327969319060[6] = 0;
   out_6590803327969319060[7] = 1;
   out_6590803327969319060[8] = 0;
}
void h_27(double *state, double *unused, double *out_7811758850780339592) {
   out_7811758850780339592[0] = state[3];
}
void H_27(double *state, double *unused, double *out_7770867780159352616) {
   out_7770867780159352616[0] = 0;
   out_7770867780159352616[1] = 0;
   out_7770867780159352616[2] = 0;
   out_7770867780159352616[3] = 1;
   out_7770867780159352616[4] = 0;
   out_7770867780159352616[5] = 0;
   out_7770867780159352616[6] = 0;
   out_7770867780159352616[7] = 0;
   out_7770867780159352616[8] = 0;
}
void h_29(double *state, double *unused, double *out_3549317488487607563) {
   out_3549317488487607563[0] = state[1];
}
void H_29(double *state, double *unused, double *out_5085873124044535521) {
   out_5085873124044535521[0] = 0;
   out_5085873124044535521[1] = 1;
   out_5085873124044535521[2] = 0;
   out_5085873124044535521[3] = 0;
   out_5085873124044535521[4] = 0;
   out_5085873124044535521[5] = 0;
   out_5085873124044535521[6] = 0;
   out_5085873124044535521[7] = 0;
   out_5085873124044535521[8] = 0;
}
void h_28(double *state, double *unused, double *out_8639308683631296411) {
   out_8639308683631296411[0] = state[0];
}
void H_28(double *state, double *unused, double *out_8278471932595485521) {
   out_8278471932595485521[0] = 1;
   out_8278471932595485521[1] = 0;
   out_8278471932595485521[2] = 0;
   out_8278471932595485521[3] = 0;
   out_8278471932595485521[4] = 0;
   out_8278471932595485521[5] = 0;
   out_8278471932595485521[6] = 0;
   out_8278471932595485521[7] = 0;
   out_8278471932595485521[8] = 0;
}
void h_31(double *state, double *unused, double *out_827363945134528403) {
   out_827363945134528403[0] = state[8];
}
void H_31(double *state, double *unused, double *out_5964595225735967584) {
   out_5964595225735967584[0] = 0;
   out_5964595225735967584[1] = 0;
   out_5964595225735967584[2] = 0;
   out_5964595225735967584[3] = 0;
   out_5964595225735967584[4] = 0;
   out_5964595225735967584[5] = 0;
   out_5964595225735967584[6] = 0;
   out_5964595225735967584[7] = 0;
   out_5964595225735967584[8] = 1;
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
void car_err_fun(double *nom_x, double *delta_x, double *out_7447692140094294189) {
  err_fun(nom_x, delta_x, out_7447692140094294189);
}
void car_inv_err_fun(double *nom_x, double *true_x, double *out_190421467442803104) {
  inv_err_fun(nom_x, true_x, out_190421467442803104);
}
void car_H_mod_fun(double *state, double *out_6314959913246026197) {
  H_mod_fun(state, out_6314959913246026197);
}
void car_f_fun(double *state, double dt, double *out_3201321820069077199) {
  f_fun(state,  dt, out_3201321820069077199);
}
void car_F_fun(double *state, double dt, double *out_3009618409394531093) {
  F_fun(state,  dt, out_3009618409394531093);
}
void car_h_25(double *state, double *unused, double *out_787725499327780767) {
  h_25(state, unused, out_787725499327780767);
}
void car_H_25(double *state, double *unused, double *out_8114437426866176332) {
  H_25(state, unused, out_8114437426866176332);
}
void car_h_24(double *state, double *unused, double *out_4758358035022413509) {
  h_24(state, unused, out_4758358035022413509);
}
void car_H_24(double *state, double *unused, double *out_6701409178827767495) {
  H_24(state, unused, out_6701409178827767495);
}
void car_h_30(double *state, double *unused, double *out_2208667425506130393) {
  h_30(state, unused, out_2208667425506130393);
}
void car_H_30(double *state, double *unused, double *out_5596104468358927705) {
  H_30(state, unused, out_5596104468358927705);
}
void car_h_26(double *state, double *unused, double *out_3899115260241811353) {
  h_26(state, unused, out_3899115260241811353);
}
void car_H_26(double *state, double *unused, double *out_6590803327969319060) {
  H_26(state, unused, out_6590803327969319060);
}
void car_h_27(double *state, double *unused, double *out_7811758850780339592) {
  h_27(state, unused, out_7811758850780339592);
}
void car_H_27(double *state, double *unused, double *out_7770867780159352616) {
  H_27(state, unused, out_7770867780159352616);
}
void car_h_29(double *state, double *unused, double *out_3549317488487607563) {
  h_29(state, unused, out_3549317488487607563);
}
void car_H_29(double *state, double *unused, double *out_5085873124044535521) {
  H_29(state, unused, out_5085873124044535521);
}
void car_h_28(double *state, double *unused, double *out_8639308683631296411) {
  h_28(state, unused, out_8639308683631296411);
}
void car_H_28(double *state, double *unused, double *out_8278471932595485521) {
  H_28(state, unused, out_8278471932595485521);
}
void car_h_31(double *state, double *unused, double *out_827363945134528403) {
  h_31(state, unused, out_827363945134528403);
}
void car_H_31(double *state, double *unused, double *out_5964595225735967584) {
  H_31(state, unused, out_5964595225735967584);
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
