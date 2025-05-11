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
void live_H(double *in_vec, double *out_3137054973667800054);
void live_err_fun(double *nom_x, double *delta_x, double *out_2310867145260871250);
void live_inv_err_fun(double *nom_x, double *true_x, double *out_1181665963072530302);
void live_H_mod_fun(double *state, double *out_7445274670191397373);
void live_f_fun(double *state, double dt, double *out_8630929383789012076);
void live_F_fun(double *state, double dt, double *out_3948174244088473224);
void live_h_4(double *state, double *unused, double *out_9072310303296772361);
void live_H_4(double *state, double *unused, double *out_5834011543760882937);
void live_h_9(double *state, double *unused, double *out_5572675555180498553);
void live_H_9(double *state, double *unused, double *out_1453207391503564533);
void live_h_10(double *state, double *unused, double *out_5932252766934801408);
void live_H_10(double *state, double *unused, double *out_8183982984963755303);
void live_h_12(double *state, double *unused, double *out_6559616620029641687);
void live_H_12(double *state, double *unused, double *out_1833116769921567555);
void live_h_35(double *state, double *unused, double *out_3495388243969068146);
void live_H_35(double *state, double *unused, double *out_2467349486388275561);
void live_h_32(double *state, double *unused, double *out_8160293758621550020);
void live_H_32(double *state, double *unused, double *out_2336184701447535092);
void live_h_13(double *state, double *unused, double *out_7681618668293812691);
void live_H_13(double *state, double *unused, double *out_2121574486666204156);
void live_h_14(double *state, double *unused, double *out_5572675555180498553);
void live_H_14(double *state, double *unused, double *out_1453207391503564533);
void live_h_33(double *state, double *unused, double *out_1699402612391354373);
void live_H_33(double *state, double *unused, double *out_683207518250582043);
void live_predict(double *in_x, double *in_P, double *in_Q, double dt);
}