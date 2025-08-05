#pragma once
#include "rednose/helpers/ekf.h"
extern "C" {
void live_update_4(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_9(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_10(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_12(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_35(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_32(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_13(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_14(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_33(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_H(double *in_vec, double *out_7667749778584838038);
void live_err_fun(double *nom_x, double *delta_x, double *out_3187320990701143640);
void live_inv_err_fun(double *nom_x, double *true_x, double *out_6784353246696417741);
void live_H_mod_fun(double *state, double *out_3086261338921078215);
void live_f_fun(double *state, double dt, double *out_4288300052430626530);
void live_F_fun(double *state, double dt, double *out_3524237092915001189);
void live_h_4(double *state, double *unused, double *out_21114561061629424);
void live_H_4(double *state, double *unused, double *out_8732160023376179490);
void live_h_9(double *state, double *unused, double *out_8365083262638039901);
void live_H_9(double *state, double *unused, double *out_8490970376746588845);
void live_h_10(double *state, double *unused, double *out_458063441325728286);
void live_H_10(double *state, double *unused, double *out_1503645237955922824);
void live_h_12(double *state, double *unused, double *out_7727589096421401572);
void live_H_12(double *state, double *unused, double *out_8111060998328585823);
void live_h_35(double *state, double *unused, double *out_590992386516909103);
void live_H_35(double *state, double *unused, double *out_5365497966003572114);
void live_h_32(double *state, double *unused, double *out_4035080666024460913);
void live_H_32(double *state, double *unused, double *out_435806406204040444);
void live_h_13(double *state, double *unused, double *out_5596924643374363611);
void live_H_13(double *state, double *unused, double *out_3215587821166133670);
void live_h_14(double *state, double *unused, double *out_8365083262638039901);
void live_H_14(double *state, double *unused, double *out_8490970376746588845);
void live_h_33(double *state, double *unused, double *out_7891362578043253873);
void live_H_33(double *state, double *unused, double *out_2214940961364714510);
void live_predict(double *in_x, double *in_P, double *in_Q, double dt);
}