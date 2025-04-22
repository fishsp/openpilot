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
void live_H(double *in_vec, double *out_815136856927738723);
void live_err_fun(double *nom_x, double *delta_x, double *out_388680556077542228);
void live_inv_err_fun(double *nom_x, double *true_x, double *out_8127066149059297);
void live_H_mod_fun(double *state, double *out_8609420937836662126);
void live_f_fun(double *state, double dt, double *out_7512307863762782601);
void live_F_fun(double *state, double dt, double *out_7309556550418904427);
void live_h_4(double *state, double *unused, double *out_947555635192105910);
void live_H_4(double *state, double *unused, double *out_8914354488603990125);
void live_h_9(double *state, double *unused, double *out_1805485837177463263);
void live_H_9(double *state, double *unused, double *out_2245170649841114021);
void live_h_10(double *state, double *unused, double *out_8966290198033134090);
void live_H_10(double *state, double *unused, double *out_4056687223578356085);
void live_h_12(double *state, double *unused, double *out_7241157233247121385);
void live_H_12(double *state, double *unused, double *out_2533096111561257129);
void live_h_35(double *state, double *unused, double *out_7421521615084351375);
void live_H_35(double *state, double *unused, double *out_5278659143886270838);
void live_h_32(double *state, double *unused, double *out_6025893554957820428);
void live_H_32(double *state, double *unused, double *out_937654368153163950);
void live_h_13(double *state, double *unused, double *out_5915898362560157987);
void live_H_13(double *state, double *unused, double *out_2716787748394487761);
void live_h_14(double *state, double *unused, double *out_1805485837177463263);
void live_H_14(double *state, double *unused, double *out_2245170649841114021);
void live_h_33(double *state, double *unused, double *out_5993090950278094991);
void live_H_33(double *state, double *unused, double *out_8429216148525128442);
void live_predict(double *in_x, double *in_P, double *in_Q, double dt);
}