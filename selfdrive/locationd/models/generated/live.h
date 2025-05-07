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
void live_H(double *in_vec, double *out_3441997376683687169);
void live_err_fun(double *nom_x, double *delta_x, double *out_7746620232378633521);
void live_inv_err_fun(double *nom_x, double *true_x, double *out_5251594479722443109);
void live_H_mod_fun(double *state, double *out_2945448643041196896);
void live_f_fun(double *state, double dt, double *out_5213749310311564333);
void live_F_fun(double *state, double dt, double *out_1457805266542172642);
void live_h_4(double *state, double *unused, double *out_2365370829773282893);
void live_H_4(double *state, double *unused, double *out_3715254290388302116);
void live_h_9(double *state, double *unused, double *out_7847272953359835304);
void live_H_9(double *state, double *unused, double *out_3474064643758711471);
void live_h_10(double *state, double *unused, double *out_136598297755644021);
void live_H_10(double *state, double *unused, double *out_2052203864134325295);
void live_h_12(double *state, double *unused, double *out_5068293052119121946);
void live_H_12(double *state, double *unused, double *out_1304202117643659679);
void live_h_35(double *state, double *unused, double *out_5865483788478182384);
void live_H_35(double *state, double *unused, double *out_4049765149968673388);
void live_h_32(double *state, double *unused, double *out_1934888001531324053);
void live_H_32(double *state, double *unused, double *out_6833423314387090981);
void live_h_13(double *state, double *unused, double *out_920254234475228033);
void live_H_13(double *state, double *unused, double *out_175300199588608485);
void live_h_14(double *state, double *unused, double *out_7847272953359835304);
void live_H_14(double *state, double *unused, double *out_3474064643758711471);
void live_h_33(double *state, double *unused, double *out_9075474917242752719);
void live_H_33(double *state, double *unused, double *out_7200322154607530992);
void live_predict(double *in_x, double *in_P, double *in_Q, double dt);
}