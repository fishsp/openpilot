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
void err_fun(double *nom_x, double *delta_x, double *out_7219539594883989435) {
   out_7219539594883989435[0] = delta_x[0] + nom_x[0];
   out_7219539594883989435[1] = delta_x[1] + nom_x[1];
   out_7219539594883989435[2] = delta_x[2] + nom_x[2];
   out_7219539594883989435[3] = delta_x[3] + nom_x[3];
   out_7219539594883989435[4] = delta_x[4] + nom_x[4];
   out_7219539594883989435[5] = delta_x[5] + nom_x[5];
   out_7219539594883989435[6] = delta_x[6] + nom_x[6];
   out_7219539594883989435[7] = delta_x[7] + nom_x[7];
   out_7219539594883989435[8] = delta_x[8] + nom_x[8];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_3875430566376996335) {
   out_3875430566376996335[0] = -nom_x[0] + true_x[0];
   out_3875430566376996335[1] = -nom_x[1] + true_x[1];
   out_3875430566376996335[2] = -nom_x[2] + true_x[2];
   out_3875430566376996335[3] = -nom_x[3] + true_x[3];
   out_3875430566376996335[4] = -nom_x[4] + true_x[4];
   out_3875430566376996335[5] = -nom_x[5] + true_x[5];
   out_3875430566376996335[6] = -nom_x[6] + true_x[6];
   out_3875430566376996335[7] = -nom_x[7] + true_x[7];
   out_3875430566376996335[8] = -nom_x[8] + true_x[8];
}
void H_mod_fun(double *state, double *out_8856109895292523094) {
   out_8856109895292523094[0] = 1.0;
   out_8856109895292523094[1] = 0;
   out_8856109895292523094[2] = 0;
   out_8856109895292523094[3] = 0;
   out_8856109895292523094[4] = 0;
   out_8856109895292523094[5] = 0;
   out_8856109895292523094[6] = 0;
   out_8856109895292523094[7] = 0;
   out_8856109895292523094[8] = 0;
   out_8856109895292523094[9] = 0;
   out_8856109895292523094[10] = 1.0;
   out_8856109895292523094[11] = 0;
   out_8856109895292523094[12] = 0;
   out_8856109895292523094[13] = 0;
   out_8856109895292523094[14] = 0;
   out_8856109895292523094[15] = 0;
   out_8856109895292523094[16] = 0;
   out_8856109895292523094[17] = 0;
   out_8856109895292523094[18] = 0;
   out_8856109895292523094[19] = 0;
   out_8856109895292523094[20] = 1.0;
   out_8856109895292523094[21] = 0;
   out_8856109895292523094[22] = 0;
   out_8856109895292523094[23] = 0;
   out_8856109895292523094[24] = 0;
   out_8856109895292523094[25] = 0;
   out_8856109895292523094[26] = 0;
   out_8856109895292523094[27] = 0;
   out_8856109895292523094[28] = 0;
   out_8856109895292523094[29] = 0;
   out_8856109895292523094[30] = 1.0;
   out_8856109895292523094[31] = 0;
   out_8856109895292523094[32] = 0;
   out_8856109895292523094[33] = 0;
   out_8856109895292523094[34] = 0;
   out_8856109895292523094[35] = 0;
   out_8856109895292523094[36] = 0;
   out_8856109895292523094[37] = 0;
   out_8856109895292523094[38] = 0;
   out_8856109895292523094[39] = 0;
   out_8856109895292523094[40] = 1.0;
   out_8856109895292523094[41] = 0;
   out_8856109895292523094[42] = 0;
   out_8856109895292523094[43] = 0;
   out_8856109895292523094[44] = 0;
   out_8856109895292523094[45] = 0;
   out_8856109895292523094[46] = 0;
   out_8856109895292523094[47] = 0;
   out_8856109895292523094[48] = 0;
   out_8856109895292523094[49] = 0;
   out_8856109895292523094[50] = 1.0;
   out_8856109895292523094[51] = 0;
   out_8856109895292523094[52] = 0;
   out_8856109895292523094[53] = 0;
   out_8856109895292523094[54] = 0;
   out_8856109895292523094[55] = 0;
   out_8856109895292523094[56] = 0;
   out_8856109895292523094[57] = 0;
   out_8856109895292523094[58] = 0;
   out_8856109895292523094[59] = 0;
   out_8856109895292523094[60] = 1.0;
   out_8856109895292523094[61] = 0;
   out_8856109895292523094[62] = 0;
   out_8856109895292523094[63] = 0;
   out_8856109895292523094[64] = 0;
   out_8856109895292523094[65] = 0;
   out_8856109895292523094[66] = 0;
   out_8856109895292523094[67] = 0;
   out_8856109895292523094[68] = 0;
   out_8856109895292523094[69] = 0;
   out_8856109895292523094[70] = 1.0;
   out_8856109895292523094[71] = 0;
   out_8856109895292523094[72] = 0;
   out_8856109895292523094[73] = 0;
   out_8856109895292523094[74] = 0;
   out_8856109895292523094[75] = 0;
   out_8856109895292523094[76] = 0;
   out_8856109895292523094[77] = 0;
   out_8856109895292523094[78] = 0;
   out_8856109895292523094[79] = 0;
   out_8856109895292523094[80] = 1.0;
}
void f_fun(double *state, double dt, double *out_762284416938852500) {
   out_762284416938852500[0] = state[0];
   out_762284416938852500[1] = state[1];
   out_762284416938852500[2] = state[2];
   out_762284416938852500[3] = state[3];
   out_762284416938852500[4] = state[4];
   out_762284416938852500[5] = dt*((-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]))*state[6] - 9.8000000000000007*state[8] + stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*state[1]) + (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*state[4])) + state[5];
   out_762284416938852500[6] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*state[4])) + state[6];
   out_762284416938852500[7] = state[7];
   out_762284416938852500[8] = state[8];
}
void F_fun(double *state, double dt, double *out_6360313542418096019) {
   out_6360313542418096019[0] = 1;
   out_6360313542418096019[1] = 0;
   out_6360313542418096019[2] = 0;
   out_6360313542418096019[3] = 0;
   out_6360313542418096019[4] = 0;
   out_6360313542418096019[5] = 0;
   out_6360313542418096019[6] = 0;
   out_6360313542418096019[7] = 0;
   out_6360313542418096019[8] = 0;
   out_6360313542418096019[9] = 0;
   out_6360313542418096019[10] = 1;
   out_6360313542418096019[11] = 0;
   out_6360313542418096019[12] = 0;
   out_6360313542418096019[13] = 0;
   out_6360313542418096019[14] = 0;
   out_6360313542418096019[15] = 0;
   out_6360313542418096019[16] = 0;
   out_6360313542418096019[17] = 0;
   out_6360313542418096019[18] = 0;
   out_6360313542418096019[19] = 0;
   out_6360313542418096019[20] = 1;
   out_6360313542418096019[21] = 0;
   out_6360313542418096019[22] = 0;
   out_6360313542418096019[23] = 0;
   out_6360313542418096019[24] = 0;
   out_6360313542418096019[25] = 0;
   out_6360313542418096019[26] = 0;
   out_6360313542418096019[27] = 0;
   out_6360313542418096019[28] = 0;
   out_6360313542418096019[29] = 0;
   out_6360313542418096019[30] = 1;
   out_6360313542418096019[31] = 0;
   out_6360313542418096019[32] = 0;
   out_6360313542418096019[33] = 0;
   out_6360313542418096019[34] = 0;
   out_6360313542418096019[35] = 0;
   out_6360313542418096019[36] = 0;
   out_6360313542418096019[37] = 0;
   out_6360313542418096019[38] = 0;
   out_6360313542418096019[39] = 0;
   out_6360313542418096019[40] = 1;
   out_6360313542418096019[41] = 0;
   out_6360313542418096019[42] = 0;
   out_6360313542418096019[43] = 0;
   out_6360313542418096019[44] = 0;
   out_6360313542418096019[45] = dt*(stiffness_front*(-state[2] - state[3] + state[7])/(mass*state[1]) + (-stiffness_front - stiffness_rear)*state[5]/(mass*state[4]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[6]/(mass*state[4]));
   out_6360313542418096019[46] = -dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*pow(state[1], 2));
   out_6360313542418096019[47] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_6360313542418096019[48] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_6360313542418096019[49] = dt*((-1 - (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*pow(state[4], 2)))*state[6] - (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*pow(state[4], 2)));
   out_6360313542418096019[50] = dt*(-stiffness_front*state[0] - stiffness_rear*state[0])/(mass*state[4]) + 1;
   out_6360313542418096019[51] = dt*(-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]));
   out_6360313542418096019[52] = dt*stiffness_front*state[0]/(mass*state[1]);
   out_6360313542418096019[53] = -9.8000000000000007*dt;
   out_6360313542418096019[54] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front - pow(center_to_rear, 2)*stiffness_rear)*state[6]/(rotational_inertia*state[4]));
   out_6360313542418096019[55] = -center_to_front*dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*pow(state[1], 2));
   out_6360313542418096019[56] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_6360313542418096019[57] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_6360313542418096019[58] = dt*(-(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*pow(state[4], 2)) - (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*pow(state[4], 2)));
   out_6360313542418096019[59] = dt*(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(rotational_inertia*state[4]);
   out_6360313542418096019[60] = dt*(-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])/(rotational_inertia*state[4]) + 1;
   out_6360313542418096019[61] = center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_6360313542418096019[62] = 0;
   out_6360313542418096019[63] = 0;
   out_6360313542418096019[64] = 0;
   out_6360313542418096019[65] = 0;
   out_6360313542418096019[66] = 0;
   out_6360313542418096019[67] = 0;
   out_6360313542418096019[68] = 0;
   out_6360313542418096019[69] = 0;
   out_6360313542418096019[70] = 1;
   out_6360313542418096019[71] = 0;
   out_6360313542418096019[72] = 0;
   out_6360313542418096019[73] = 0;
   out_6360313542418096019[74] = 0;
   out_6360313542418096019[75] = 0;
   out_6360313542418096019[76] = 0;
   out_6360313542418096019[77] = 0;
   out_6360313542418096019[78] = 0;
   out_6360313542418096019[79] = 0;
   out_6360313542418096019[80] = 1;
}
void h_25(double *state, double *unused, double *out_813385390617363116) {
   out_813385390617363116[0] = state[6];
}
void H_25(double *state, double *unused, double *out_2998985903263687316) {
   out_2998985903263687316[0] = 0;
   out_2998985903263687316[1] = 0;
   out_2998985903263687316[2] = 0;
   out_2998985903263687316[3] = 0;
   out_2998985903263687316[4] = 0;
   out_2998985903263687316[5] = 0;
   out_2998985903263687316[6] = 1;
   out_2998985903263687316[7] = 0;
   out_2998985903263687316[8] = 0;
}
void h_24(double *state, double *unused, double *out_4621117448125363727) {
   out_4621117448125363727[0] = state[4];
   out_4621117448125363727[1] = state[5];
}
void H_24(double *state, double *unused, double *out_2274508379649325278) {
   out_2274508379649325278[0] = 0;
   out_2274508379649325278[1] = 0;
   out_2274508379649325278[2] = 0;
   out_2274508379649325278[3] = 0;
   out_2274508379649325278[4] = 1;
   out_2274508379649325278[5] = 0;
   out_2274508379649325278[6] = 0;
   out_2274508379649325278[7] = 0;
   out_2274508379649325278[8] = 0;
   out_2274508379649325278[9] = 0;
   out_2274508379649325278[10] = 0;
   out_2274508379649325278[11] = 0;
   out_2274508379649325278[12] = 0;
   out_2274508379649325278[13] = 0;
   out_2274508379649325278[14] = 1;
   out_2274508379649325278[15] = 0;
   out_2274508379649325278[16] = 0;
   out_2274508379649325278[17] = 0;
}
void h_30(double *state, double *unused, double *out_7067887260136686069) {
   out_7067887260136686069[0] = state[4];
}
void H_30(double *state, double *unused, double *out_7526682233391295514) {
   out_7526682233391295514[0] = 0;
   out_7526682233391295514[1] = 0;
   out_7526682233391295514[2] = 0;
   out_7526682233391295514[3] = 0;
   out_7526682233391295514[4] = 1;
   out_7526682233391295514[5] = 0;
   out_7526682233391295514[6] = 0;
   out_7526682233391295514[7] = 0;
   out_7526682233391295514[8] = 0;
}
void h_26(double *state, double *unused, double *out_8011397113038348504) {
   out_8011397113038348504[0] = state[7];
}
void H_26(double *state, double *unused, double *out_6740489222137743540) {
   out_6740489222137743540[0] = 0;
   out_6740489222137743540[1] = 0;
   out_6740489222137743540[2] = 0;
   out_6740489222137743540[3] = 0;
   out_6740489222137743540[4] = 0;
   out_6740489222137743540[5] = 0;
   out_6740489222137743540[6] = 0;
   out_6740489222137743540[7] = 1;
   out_6740489222137743540[8] = 0;
}
void h_27(double *state, double *unused, double *out_7212426908031212262) {
   out_7212426908031212262[0] = state[3];
}
void H_27(double *state, double *unused, double *out_8745298528517831191) {
   out_8745298528517831191[0] = 0;
   out_8745298528517831191[1] = 0;
   out_8745298528517831191[2] = 0;
   out_8745298528517831191[3] = 1;
   out_8745298528517831191[4] = 0;
   out_8745298528517831191[5] = 0;
   out_8745298528517831191[6] = 0;
   out_8745298528517831191[7] = 0;
   out_8745298528517831191[8] = 0;
}
void h_29(double *state, double *unused, double *out_3574977379777189912) {
   out_3574977379777189912[0] = state[1];
}
void H_29(double *state, double *unused, double *out_7016450889076903330) {
   out_7016450889076903330[0] = 0;
   out_7016450889076903330[1] = 1;
   out_7016450889076903330[2] = 0;
   out_7016450889076903330[3] = 0;
   out_7016450889076903330[4] = 0;
   out_7016450889076903330[5] = 0;
   out_7016450889076903330[6] = 0;
   out_7016450889076903330[7] = 0;
   out_7016450889076903330[8] = 0;
}
void h_28(double *state, double *unused, double *out_2210753640370366617) {
   out_2210753640370366617[0] = state[0];
}
void H_28(double *state, double *unused, double *out_5052820617511577079) {
   out_5052820617511577079[0] = 1;
   out_5052820617511577079[1] = 0;
   out_5052820617511577079[2] = 0;
   out_5052820617511577079[3] = 0;
   out_5052820617511577079[4] = 0;
   out_5052820617511577079[5] = 0;
   out_5052820617511577079[6] = 0;
   out_5052820617511577079[7] = 0;
   out_5052820617511577079[8] = 0;
}
void h_31(double *state, double *unused, double *out_4450834918871385466) {
   out_4450834918871385466[0] = state[8];
}
void H_31(double *state, double *unused, double *out_7366697324371095016) {
   out_7366697324371095016[0] = 0;
   out_7366697324371095016[1] = 0;
   out_7366697324371095016[2] = 0;
   out_7366697324371095016[3] = 0;
   out_7366697324371095016[4] = 0;
   out_7366697324371095016[5] = 0;
   out_7366697324371095016[6] = 0;
   out_7366697324371095016[7] = 0;
   out_7366697324371095016[8] = 1;
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
void car_err_fun(double *nom_x, double *delta_x, double *out_7219539594883989435) {
  err_fun(nom_x, delta_x, out_7219539594883989435);
}
void car_inv_err_fun(double *nom_x, double *true_x, double *out_3875430566376996335) {
  inv_err_fun(nom_x, true_x, out_3875430566376996335);
}
void car_H_mod_fun(double *state, double *out_8856109895292523094) {
  H_mod_fun(state, out_8856109895292523094);
}
void car_f_fun(double *state, double dt, double *out_762284416938852500) {
  f_fun(state,  dt, out_762284416938852500);
}
void car_F_fun(double *state, double dt, double *out_6360313542418096019) {
  F_fun(state,  dt, out_6360313542418096019);
}
void car_h_25(double *state, double *unused, double *out_813385390617363116) {
  h_25(state, unused, out_813385390617363116);
}
void car_H_25(double *state, double *unused, double *out_2998985903263687316) {
  H_25(state, unused, out_2998985903263687316);
}
void car_h_24(double *state, double *unused, double *out_4621117448125363727) {
  h_24(state, unused, out_4621117448125363727);
}
void car_H_24(double *state, double *unused, double *out_2274508379649325278) {
  H_24(state, unused, out_2274508379649325278);
}
void car_h_30(double *state, double *unused, double *out_7067887260136686069) {
  h_30(state, unused, out_7067887260136686069);
}
void car_H_30(double *state, double *unused, double *out_7526682233391295514) {
  H_30(state, unused, out_7526682233391295514);
}
void car_h_26(double *state, double *unused, double *out_8011397113038348504) {
  h_26(state, unused, out_8011397113038348504);
}
void car_H_26(double *state, double *unused, double *out_6740489222137743540) {
  H_26(state, unused, out_6740489222137743540);
}
void car_h_27(double *state, double *unused, double *out_7212426908031212262) {
  h_27(state, unused, out_7212426908031212262);
}
void car_H_27(double *state, double *unused, double *out_8745298528517831191) {
  H_27(state, unused, out_8745298528517831191);
}
void car_h_29(double *state, double *unused, double *out_3574977379777189912) {
  h_29(state, unused, out_3574977379777189912);
}
void car_H_29(double *state, double *unused, double *out_7016450889076903330) {
  H_29(state, unused, out_7016450889076903330);
}
void car_h_28(double *state, double *unused, double *out_2210753640370366617) {
  h_28(state, unused, out_2210753640370366617);
}
void car_H_28(double *state, double *unused, double *out_5052820617511577079) {
  H_28(state, unused, out_5052820617511577079);
}
void car_h_31(double *state, double *unused, double *out_4450834918871385466) {
  h_31(state, unused, out_4450834918871385466);
}
void car_H_31(double *state, double *unused, double *out_7366697324371095016) {
  H_31(state, unused, out_7366697324371095016);
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
