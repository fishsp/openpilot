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
void car_err_fun(double *nom_x, double *delta_x, double *out_4743726722773991975);
void car_inv_err_fun(double *nom_x, double *true_x, double *out_2841966670875871855);
void car_H_mod_fun(double *state, double *out_7340213240266504600);
void car_f_fun(double *state, double dt, double *out_3713690376114026963);
void car_F_fun(double *state, double dt, double *out_4741690943052406499);
void car_h_25(double *state, double *unused, double *out_8600294221188020871);
void car_H_25(double *state, double *unused, double *out_2073788871405630401);
void car_h_24(double *state, double *unused, double *out_8767666115400015860);
void car_H_24(double *state, double *unused, double *out_4501782935185887700);
void car_h_30(double *state, double *unused, double *out_3482855874219714884);
void car_H_30(double *state, double *unused, double *out_4842901470085986354);
void car_h_26(double *state, double *unused, double *out_4086171758896192401);
void car_H_26(double *state, double *unused, double *out_5815292190279686625);
void car_h_27(double *state, double *unused, double *out_7432550420111879134);
void car_H_27(double *state, double *unused, double *out_2668138158285561443);
void car_h_29(double *state, double *unused, double *out_1207327056606662750);
void car_H_29(double *state, double *unused, double *out_5353132814400378538);
void car_h_28(double *state, double *unused, double *out_6012520385936826837);
void car_H_28(double *state, double *unused, double *out_270733797330847964);
void car_h_31(double *state, double *unused, double *out_8757480889539150016);
void car_H_31(double *state, double *unused, double *out_2043142909528669973);
void car_predict(double *in_x, double *in_P, double *in_Q, double dt);
void car_set_mass(double x);
void car_set_rotational_inertia(double x);
void car_set_center_to_front(double x);
void car_set_center_to_rear(double x);
void car_set_stiffness_front(double x);
void car_set_stiffness_rear(double x);
}