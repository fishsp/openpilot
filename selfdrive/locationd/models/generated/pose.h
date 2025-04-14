#pragma once
#include "rednose/helpers/ekf.h"
extern "C" {
void pose_update_4(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_10(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_13(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_14(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_err_fun(double *nom_x, double *delta_x, double *out_1937455278130310156);
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_2984526074350005589);
void pose_H_mod_fun(double *state, double *out_4209141269458760337);
void pose_f_fun(double *state, double dt, double *out_8130059171420837203);
void pose_F_fun(double *state, double dt, double *out_434290692210433210);
void pose_h_4(double *state, double *unused, double *out_7847268677212961006);
void pose_H_4(double *state, double *unused, double *out_2389442543552463476);
void pose_h_10(double *state, double *unused, double *out_7815524616711553503);
void pose_H_10(double *state, double *unused, double *out_5409770976600334064);
void pose_h_13(double *state, double *unused, double *out_1287863093787623213);
void pose_H_13(double *state, double *unused, double *out_8446670321840387211);
void pose_h_14(double *state, double *unused, double *out_8895375992006815785);
void pose_H_14(double *state, double *unused, double *out_693345888742908820);
void pose_predict(double *in_x, double *in_P, double *in_Q, double dt);
}