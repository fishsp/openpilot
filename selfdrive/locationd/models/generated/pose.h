#pragma once
#include "rednose/helpers/ekf.h"
extern "C" {
void pose_update_4(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_10(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_13(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_14(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_err_fun(double *nom_x, double *delta_x, double *out_1487504118503105920);
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_2323018677029669916);
void pose_H_mod_fun(double *state, double *out_4092507731700215880);
void pose_f_fun(double *state, double dt, double *out_7088713391271841939);
void pose_F_fun(double *state, double dt, double *out_8358144676595975828);
void pose_h_4(double *state, double *unused, double *out_3125340361276393037);
void pose_H_4(double *state, double *unused, double *out_3457766192013903683);
void pose_h_10(double *state, double *unused, double *out_5094701453959070614);
void pose_H_10(double *state, double *unused, double *out_7210162420988875382);
void pose_h_13(double *state, double *unused, double *out_3814849203139753026);
void pose_H_13(double *state, double *unused, double *out_4152865016302797246);
void pose_h_14(double *state, double *unused, double *out_9041680693720875183);
void pose_H_14(double *state, double *unused, double *out_6540554624309275979);
void pose_predict(double *in_x, double *in_P, double *in_Q, double dt);
}