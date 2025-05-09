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
void live_H(double *in_vec, double *out_3960662652636581650);
void live_err_fun(double *nom_x, double *delta_x, double *out_2034880371853639070);
void live_inv_err_fun(double *nom_x, double *true_x, double *out_7388604203758754888);
void live_H_mod_fun(double *state, double *out_3600610583816990225);
void live_f_fun(double *state, double dt, double *out_6759008510624374542);
void live_F_fun(double *state, double dt, double *out_8929491357872735178);
void live_h_4(double *state, double *unused, double *out_3866614507224277709);
void live_H_4(double *state, double *unused, double *out_9089864803322904416);
void live_h_9(double *state, double *unused, double *out_6990588499902101356);
void live_H_9(double *state, double *unused, double *out_5199711534031869717);
void live_h_10(double *state, double *unused, double *out_4916505082398751015);
void live_H_10(double *state, double *unused, double *out_1990326578245677539);
void live_h_12(double *state, double *unused, double *out_6723805956476414841);
void live_H_12(double *state, double *unused, double *out_8468765778275310749);
void live_h_35(double *state, double *unused, double *out_8632690586666191156);
void live_H_35(double *state, double *unused, double *out_5723202745950297040);
void live_h_32(double *state, double *unused, double *out_3538794038118438407);
void live_H_32(double *state, double *unused, double *out_5592037961009556571);
void live_h_13(double *state, double *unused, double *out_2796926804560068075);
void live_H_13(double *state, double *unused, double *out_8468522362125606715);
void live_h_14(double *state, double *unused, double *out_6990588499902101356);
void live_H_14(double *state, double *unused, double *out_5199711534031869717);
void live_h_33(double *state, double *unused, double *out_7158366233241733492);
void live_H_33(double *state, double *unused, double *out_2572645741311439436);
void live_predict(double *in_x, double *in_P, double *in_Q, double dt);
}