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
void car_err_fun(double *nom_x, double *delta_x, double *out_5780593744637766078);
void car_inv_err_fun(double *nom_x, double *true_x, double *out_2456602474355631579);
void car_H_mod_fun(double *state, double *out_8242723593120472939);
void car_f_fun(double *state, double dt, double *out_4382144304731747207);
void car_F_fun(double *state, double dt, double *out_3593418831783737087);
void car_h_25(double *state, double *unused, double *out_3708183686637237416);
void car_H_25(double *state, double *unused, double *out_5606166668105046745);
void car_h_24(double *state, double *unused, double *out_5674420985845255762);
void car_H_24(double *state, double *unused, double *out_3433517069099547179);
void car_h_30(double *state, double *unused, double *out_3062651539713113520);
void car_H_30(double *state, double *unused, double *out_8124499626612295372);
void car_h_26(double *state, double *unused, double *out_7684837569169221834);
void car_H_26(double *state, double *unused, double *out_1864663349230990521);
void car_h_27(double *state, double *unused, double *out_8709856954298083061);
void car_H_27(double *state, double *unused, double *out_3302064409161381764);
void car_h_29(double *state, double *unused, double *out_6099437591157446205);
void car_H_29(double *state, double *unused, double *out_1588701682291830731);
void car_h_28(double *state, double *unused, double *out_272112217708757704);
void car_H_28(double *state, double *unused, double *out_3552331953857156982);
void car_h_31(double *state, double *unused, double *out_5158815079266120583);
void car_H_31(double *state, double *unused, double *out_1238455246997639045);
void car_predict(double *in_x, double *in_P, double *in_Q, double dt);
void car_set_mass(double x);
void car_set_rotational_inertia(double x);
void car_set_center_to_front(double x);
void car_set_center_to_rear(double x);
void car_set_stiffness_front(double x);
void car_set_stiffness_rear(double x);
}