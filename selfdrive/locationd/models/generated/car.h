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
void car_err_fun(double *nom_x, double *delta_x, double *out_7447692140094294189);
void car_inv_err_fun(double *nom_x, double *true_x, double *out_190421467442803104);
void car_H_mod_fun(double *state, double *out_6314959913246026197);
void car_f_fun(double *state, double dt, double *out_3201321820069077199);
void car_F_fun(double *state, double dt, double *out_3009618409394531093);
void car_h_25(double *state, double *unused, double *out_787725499327780767);
void car_H_25(double *state, double *unused, double *out_8114437426866176332);
void car_h_24(double *state, double *unused, double *out_4758358035022413509);
void car_H_24(double *state, double *unused, double *out_6701409178827767495);
void car_h_30(double *state, double *unused, double *out_2208667425506130393);
void car_H_30(double *state, double *unused, double *out_5596104468358927705);
void car_h_26(double *state, double *unused, double *out_3899115260241811353);
void car_H_26(double *state, double *unused, double *out_6590803327969319060);
void car_h_27(double *state, double *unused, double *out_7811758850780339592);
void car_H_27(double *state, double *unused, double *out_7770867780159352616);
void car_h_29(double *state, double *unused, double *out_3549317488487607563);
void car_H_29(double *state, double *unused, double *out_5085873124044535521);
void car_h_28(double *state, double *unused, double *out_8639308683631296411);
void car_H_28(double *state, double *unused, double *out_8278471932595485521);
void car_h_31(double *state, double *unused, double *out_827363945134528403);
void car_H_31(double *state, double *unused, double *out_5964595225735967584);
void car_predict(double *in_x, double *in_P, double *in_Q, double dt);
void car_set_mass(double x);
void car_set_rotational_inertia(double x);
void car_set_center_to_front(double x);
void car_set_center_to_rear(double x);
void car_set_stiffness_front(double x);
void car_set_stiffness_rear(double x);
}