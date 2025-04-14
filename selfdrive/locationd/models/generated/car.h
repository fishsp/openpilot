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
void car_err_fun(double *nom_x, double *delta_x, double *out_1157861628203609812);
void car_inv_err_fun(double *nom_x, double *true_x, double *out_3048943521048617947);
void car_H_mod_fun(double *state, double *out_6436924980861323347);
void car_f_fun(double *state, double dt, double *out_8302405370356002085);
void car_F_fun(double *state, double dt, double *out_7970830068534808110);
void car_h_25(double *state, double *unused, double *out_7195447474562559623);
void car_H_25(double *state, double *unused, double *out_7214988249920359160);
void car_h_24(double *state, double *unused, double *out_5845787550217648453);
void car_H_24(double *state, double *unused, double *out_1929878460030518002);
void car_h_30(double *state, double *unused, double *out_2387185174253970807);
void car_H_30(double *state, double *unused, double *out_8713422865281943829);
void car_h_26(double *state, double *unused, double *out_2614488093406874838);
void car_H_26(double *state, double *unused, double *out_7927229854028391855);
void car_h_27(double *state, double *unused, double *out_6437597329401740813);
void car_H_27(double *state, double *unused, double *out_7558557896627182876);
void car_h_29(double *state, double *unused, double *out_1443675321352308372);
void car_H_29(double *state, double *unused, double *out_8203191520967551645);
void car_h_28(double *state, double *unused, double *out_1435924061740895168);
void car_H_28(double *state, double *unused, double *out_5161153535672469397);
void car_h_31(double *state, double *unused, double *out_4059747486291480197);
void car_H_31(double *state, double *unused, double *out_2847276828812951460);
void car_predict(double *in_x, double *in_P, double *in_Q, double dt);
void car_set_mass(double x);
void car_set_rotational_inertia(double x);
void car_set_center_to_front(double x);
void car_set_center_to_rear(double x);
void car_set_stiffness_front(double x);
void car_set_stiffness_rear(double x);
}