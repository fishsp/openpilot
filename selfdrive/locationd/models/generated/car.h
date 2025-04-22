#pragma once
#include "rednose/helpers/ekf.h"
extern "C" {
void car_update_25(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_24(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_30(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_26(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_27(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_29(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_28(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_31(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_err_fun(double *nom_x, double *delta_x, double *out_7219539594883989435);
void car_inv_err_fun(double *nom_x, double *true_x, double *out_3875430566376996335);
void car_H_mod_fun(double *state, double *out_8856109895292523094);
void car_f_fun(double *state, double dt, double *out_762284416938852500);
void car_F_fun(double *state, double dt, double *out_6360313542418096019);
void car_h_25(double *state, double *unused, double *out_813385390617363116);
void car_H_25(double *state, double *unused, double *out_2998985903263687316);
void car_h_24(double *state, double *unused, double *out_4621117448125363727);
void car_H_24(double *state, double *unused, double *out_2274508379649325278);
void car_h_30(double *state, double *unused, double *out_7067887260136686069);
void car_H_30(double *state, double *unused, double *out_7526682233391295514);
void car_h_26(double *state, double *unused, double *out_8011397113038348504);
void car_H_26(double *state, double *unused, double *out_6740489222137743540);
void car_h_27(double *state, double *unused, double *out_7212426908031212262);
void car_H_27(double *state, double *unused, double *out_8745298528517831191);
void car_h_29(double *state, double *unused, double *out_3574977379777189912);
void car_H_29(double *state, double *unused, double *out_7016450889076903330);
void car_h_28(double *state, double *unused, double *out_2210753640370366617);
void car_H_28(double *state, double *unused, double *out_5052820617511577079);
void car_h_31(double *state, double *unused, double *out_4450834918871385466);
void car_H_31(double *state, double *unused, double *out_7366697324371095016);
void car_predict(double *in_x, double *in_P, double *in_Q, double dt);
void car_set_mass(double x);
void car_set_rotational_inertia(double x);
void car_set_center_to_front(double x);
void car_set_center_to_rear(double x);
void car_set_stiffness_front(double x);
void car_set_stiffness_rear(double x);
}