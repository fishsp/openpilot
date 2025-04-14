#include "pose.h"

namespace {
#define DIM 18
#define EDIM 18
#define MEDIM 18
typedef void (*Hfun)(double *, double *, double *);
const static double MAHA_THRESH_4 = 7.814727903251177;
const static double MAHA_THRESH_10 = 7.814727903251177;
const static double MAHA_THRESH_13 = 7.814727903251177;
const static double MAHA_THRESH_14 = 7.814727903251177;

/******************************************************************************
 *                      Code generated with SymPy 1.13.2                      *
 *                                                                            *
 *              See http://www.sympy.org/ for more information.               *
 *                                                                            *
 *                         This file is part of 'ekf'                         *
 ******************************************************************************/
void err_fun(double *nom_x, double *delta_x, double *out_1937455278130310156) {
   out_1937455278130310156[0] = delta_x[0] + nom_x[0];
   out_1937455278130310156[1] = delta_x[1] + nom_x[1];
   out_1937455278130310156[2] = delta_x[2] + nom_x[2];
   out_1937455278130310156[3] = delta_x[3] + nom_x[3];
   out_1937455278130310156[4] = delta_x[4] + nom_x[4];
   out_1937455278130310156[5] = delta_x[5] + nom_x[5];
   out_1937455278130310156[6] = delta_x[6] + nom_x[6];
   out_1937455278130310156[7] = delta_x[7] + nom_x[7];
   out_1937455278130310156[8] = delta_x[8] + nom_x[8];
   out_1937455278130310156[9] = delta_x[9] + nom_x[9];
   out_1937455278130310156[10] = delta_x[10] + nom_x[10];
   out_1937455278130310156[11] = delta_x[11] + nom_x[11];
   out_1937455278130310156[12] = delta_x[12] + nom_x[12];
   out_1937455278130310156[13] = delta_x[13] + nom_x[13];
   out_1937455278130310156[14] = delta_x[14] + nom_x[14];
   out_1937455278130310156[15] = delta_x[15] + nom_x[15];
   out_1937455278130310156[16] = delta_x[16] + nom_x[16];
   out_1937455278130310156[17] = delta_x[17] + nom_x[17];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_2984526074350005589) {
   out_2984526074350005589[0] = -nom_x[0] + true_x[0];
   out_2984526074350005589[1] = -nom_x[1] + true_x[1];
   out_2984526074350005589[2] = -nom_x[2] + true_x[2];
   out_2984526074350005589[3] = -nom_x[3] + true_x[3];
   out_2984526074350005589[4] = -nom_x[4] + true_x[4];
   out_2984526074350005589[5] = -nom_x[5] + true_x[5];
   out_2984526074350005589[6] = -nom_x[6] + true_x[6];
   out_2984526074350005589[7] = -nom_x[7] + true_x[7];
   out_2984526074350005589[8] = -nom_x[8] + true_x[8];
   out_2984526074350005589[9] = -nom_x[9] + true_x[9];
   out_2984526074350005589[10] = -nom_x[10] + true_x[10];
   out_2984526074350005589[11] = -nom_x[11] + true_x[11];
   out_2984526074350005589[12] = -nom_x[12] + true_x[12];
   out_2984526074350005589[13] = -nom_x[13] + true_x[13];
   out_2984526074350005589[14] = -nom_x[14] + true_x[14];
   out_2984526074350005589[15] = -nom_x[15] + true_x[15];
   out_2984526074350005589[16] = -nom_x[16] + true_x[16];
   out_2984526074350005589[17] = -nom_x[17] + true_x[17];
}
void H_mod_fun(double *state, double *out_4209141269458760337) {
   out_4209141269458760337[0] = 1.0;
   out_4209141269458760337[1] = 0.0;
   out_4209141269458760337[2] = 0.0;
   out_4209141269458760337[3] = 0.0;
   out_4209141269458760337[4] = 0.0;
   out_4209141269458760337[5] = 0.0;
   out_4209141269458760337[6] = 0.0;
   out_4209141269458760337[7] = 0.0;
   out_4209141269458760337[8] = 0.0;
   out_4209141269458760337[9] = 0.0;
   out_4209141269458760337[10] = 0.0;
   out_4209141269458760337[11] = 0.0;
   out_4209141269458760337[12] = 0.0;
   out_4209141269458760337[13] = 0.0;
   out_4209141269458760337[14] = 0.0;
   out_4209141269458760337[15] = 0.0;
   out_4209141269458760337[16] = 0.0;
   out_4209141269458760337[17] = 0.0;
   out_4209141269458760337[18] = 0.0;
   out_4209141269458760337[19] = 1.0;
   out_4209141269458760337[20] = 0.0;
   out_4209141269458760337[21] = 0.0;
   out_4209141269458760337[22] = 0.0;
   out_4209141269458760337[23] = 0.0;
   out_4209141269458760337[24] = 0.0;
   out_4209141269458760337[25] = 0.0;
   out_4209141269458760337[26] = 0.0;
   out_4209141269458760337[27] = 0.0;
   out_4209141269458760337[28] = 0.0;
   out_4209141269458760337[29] = 0.0;
   out_4209141269458760337[30] = 0.0;
   out_4209141269458760337[31] = 0.0;
   out_4209141269458760337[32] = 0.0;
   out_4209141269458760337[33] = 0.0;
   out_4209141269458760337[34] = 0.0;
   out_4209141269458760337[35] = 0.0;
   out_4209141269458760337[36] = 0.0;
   out_4209141269458760337[37] = 0.0;
   out_4209141269458760337[38] = 1.0;
   out_4209141269458760337[39] = 0.0;
   out_4209141269458760337[40] = 0.0;
   out_4209141269458760337[41] = 0.0;
   out_4209141269458760337[42] = 0.0;
   out_4209141269458760337[43] = 0.0;
   out_4209141269458760337[44] = 0.0;
   out_4209141269458760337[45] = 0.0;
   out_4209141269458760337[46] = 0.0;
   out_4209141269458760337[47] = 0.0;
   out_4209141269458760337[48] = 0.0;
   out_4209141269458760337[49] = 0.0;
   out_4209141269458760337[50] = 0.0;
   out_4209141269458760337[51] = 0.0;
   out_4209141269458760337[52] = 0.0;
   out_4209141269458760337[53] = 0.0;
   out_4209141269458760337[54] = 0.0;
   out_4209141269458760337[55] = 0.0;
   out_4209141269458760337[56] = 0.0;
   out_4209141269458760337[57] = 1.0;
   out_4209141269458760337[58] = 0.0;
   out_4209141269458760337[59] = 0.0;
   out_4209141269458760337[60] = 0.0;
   out_4209141269458760337[61] = 0.0;
   out_4209141269458760337[62] = 0.0;
   out_4209141269458760337[63] = 0.0;
   out_4209141269458760337[64] = 0.0;
   out_4209141269458760337[65] = 0.0;
   out_4209141269458760337[66] = 0.0;
   out_4209141269458760337[67] = 0.0;
   out_4209141269458760337[68] = 0.0;
   out_4209141269458760337[69] = 0.0;
   out_4209141269458760337[70] = 0.0;
   out_4209141269458760337[71] = 0.0;
   out_4209141269458760337[72] = 0.0;
   out_4209141269458760337[73] = 0.0;
   out_4209141269458760337[74] = 0.0;
   out_4209141269458760337[75] = 0.0;
   out_4209141269458760337[76] = 1.0;
   out_4209141269458760337[77] = 0.0;
   out_4209141269458760337[78] = 0.0;
   out_4209141269458760337[79] = 0.0;
   out_4209141269458760337[80] = 0.0;
   out_4209141269458760337[81] = 0.0;
   out_4209141269458760337[82] = 0.0;
   out_4209141269458760337[83] = 0.0;
   out_4209141269458760337[84] = 0.0;
   out_4209141269458760337[85] = 0.0;
   out_4209141269458760337[86] = 0.0;
   out_4209141269458760337[87] = 0.0;
   out_4209141269458760337[88] = 0.0;
   out_4209141269458760337[89] = 0.0;
   out_4209141269458760337[90] = 0.0;
   out_4209141269458760337[91] = 0.0;
   out_4209141269458760337[92] = 0.0;
   out_4209141269458760337[93] = 0.0;
   out_4209141269458760337[94] = 0.0;
   out_4209141269458760337[95] = 1.0;
   out_4209141269458760337[96] = 0.0;
   out_4209141269458760337[97] = 0.0;
   out_4209141269458760337[98] = 0.0;
   out_4209141269458760337[99] = 0.0;
   out_4209141269458760337[100] = 0.0;
   out_4209141269458760337[101] = 0.0;
   out_4209141269458760337[102] = 0.0;
   out_4209141269458760337[103] = 0.0;
   out_4209141269458760337[104] = 0.0;
   out_4209141269458760337[105] = 0.0;
   out_4209141269458760337[106] = 0.0;
   out_4209141269458760337[107] = 0.0;
   out_4209141269458760337[108] = 0.0;
   out_4209141269458760337[109] = 0.0;
   out_4209141269458760337[110] = 0.0;
   out_4209141269458760337[111] = 0.0;
   out_4209141269458760337[112] = 0.0;
   out_4209141269458760337[113] = 0.0;
   out_4209141269458760337[114] = 1.0;
   out_4209141269458760337[115] = 0.0;
   out_4209141269458760337[116] = 0.0;
   out_4209141269458760337[117] = 0.0;
   out_4209141269458760337[118] = 0.0;
   out_4209141269458760337[119] = 0.0;
   out_4209141269458760337[120] = 0.0;
   out_4209141269458760337[121] = 0.0;
   out_4209141269458760337[122] = 0.0;
   out_4209141269458760337[123] = 0.0;
   out_4209141269458760337[124] = 0.0;
   out_4209141269458760337[125] = 0.0;
   out_4209141269458760337[126] = 0.0;
   out_4209141269458760337[127] = 0.0;
   out_4209141269458760337[128] = 0.0;
   out_4209141269458760337[129] = 0.0;
   out_4209141269458760337[130] = 0.0;
   out_4209141269458760337[131] = 0.0;
   out_4209141269458760337[132] = 0.0;
   out_4209141269458760337[133] = 1.0;
   out_4209141269458760337[134] = 0.0;
   out_4209141269458760337[135] = 0.0;
   out_4209141269458760337[136] = 0.0;
   out_4209141269458760337[137] = 0.0;
   out_4209141269458760337[138] = 0.0;
   out_4209141269458760337[139] = 0.0;
   out_4209141269458760337[140] = 0.0;
   out_4209141269458760337[141] = 0.0;
   out_4209141269458760337[142] = 0.0;
   out_4209141269458760337[143] = 0.0;
   out_4209141269458760337[144] = 0.0;
   out_4209141269458760337[145] = 0.0;
   out_4209141269458760337[146] = 0.0;
   out_4209141269458760337[147] = 0.0;
   out_4209141269458760337[148] = 0.0;
   out_4209141269458760337[149] = 0.0;
   out_4209141269458760337[150] = 0.0;
   out_4209141269458760337[151] = 0.0;
   out_4209141269458760337[152] = 1.0;
   out_4209141269458760337[153] = 0.0;
   out_4209141269458760337[154] = 0.0;
   out_4209141269458760337[155] = 0.0;
   out_4209141269458760337[156] = 0.0;
   out_4209141269458760337[157] = 0.0;
   out_4209141269458760337[158] = 0.0;
   out_4209141269458760337[159] = 0.0;
   out_4209141269458760337[160] = 0.0;
   out_4209141269458760337[161] = 0.0;
   out_4209141269458760337[162] = 0.0;
   out_4209141269458760337[163] = 0.0;
   out_4209141269458760337[164] = 0.0;
   out_4209141269458760337[165] = 0.0;
   out_4209141269458760337[166] = 0.0;
   out_4209141269458760337[167] = 0.0;
   out_4209141269458760337[168] = 0.0;
   out_4209141269458760337[169] = 0.0;
   out_4209141269458760337[170] = 0.0;
   out_4209141269458760337[171] = 1.0;
   out_4209141269458760337[172] = 0.0;
   out_4209141269458760337[173] = 0.0;
   out_4209141269458760337[174] = 0.0;
   out_4209141269458760337[175] = 0.0;
   out_4209141269458760337[176] = 0.0;
   out_4209141269458760337[177] = 0.0;
   out_4209141269458760337[178] = 0.0;
   out_4209141269458760337[179] = 0.0;
   out_4209141269458760337[180] = 0.0;
   out_4209141269458760337[181] = 0.0;
   out_4209141269458760337[182] = 0.0;
   out_4209141269458760337[183] = 0.0;
   out_4209141269458760337[184] = 0.0;
   out_4209141269458760337[185] = 0.0;
   out_4209141269458760337[186] = 0.0;
   out_4209141269458760337[187] = 0.0;
   out_4209141269458760337[188] = 0.0;
   out_4209141269458760337[189] = 0.0;
   out_4209141269458760337[190] = 1.0;
   out_4209141269458760337[191] = 0.0;
   out_4209141269458760337[192] = 0.0;
   out_4209141269458760337[193] = 0.0;
   out_4209141269458760337[194] = 0.0;
   out_4209141269458760337[195] = 0.0;
   out_4209141269458760337[196] = 0.0;
   out_4209141269458760337[197] = 0.0;
   out_4209141269458760337[198] = 0.0;
   out_4209141269458760337[199] = 0.0;
   out_4209141269458760337[200] = 0.0;
   out_4209141269458760337[201] = 0.0;
   out_4209141269458760337[202] = 0.0;
   out_4209141269458760337[203] = 0.0;
   out_4209141269458760337[204] = 0.0;
   out_4209141269458760337[205] = 0.0;
   out_4209141269458760337[206] = 0.0;
   out_4209141269458760337[207] = 0.0;
   out_4209141269458760337[208] = 0.0;
   out_4209141269458760337[209] = 1.0;
   out_4209141269458760337[210] = 0.0;
   out_4209141269458760337[211] = 0.0;
   out_4209141269458760337[212] = 0.0;
   out_4209141269458760337[213] = 0.0;
   out_4209141269458760337[214] = 0.0;
   out_4209141269458760337[215] = 0.0;
   out_4209141269458760337[216] = 0.0;
   out_4209141269458760337[217] = 0.0;
   out_4209141269458760337[218] = 0.0;
   out_4209141269458760337[219] = 0.0;
   out_4209141269458760337[220] = 0.0;
   out_4209141269458760337[221] = 0.0;
   out_4209141269458760337[222] = 0.0;
   out_4209141269458760337[223] = 0.0;
   out_4209141269458760337[224] = 0.0;
   out_4209141269458760337[225] = 0.0;
   out_4209141269458760337[226] = 0.0;
   out_4209141269458760337[227] = 0.0;
   out_4209141269458760337[228] = 1.0;
   out_4209141269458760337[229] = 0.0;
   out_4209141269458760337[230] = 0.0;
   out_4209141269458760337[231] = 0.0;
   out_4209141269458760337[232] = 0.0;
   out_4209141269458760337[233] = 0.0;
   out_4209141269458760337[234] = 0.0;
   out_4209141269458760337[235] = 0.0;
   out_4209141269458760337[236] = 0.0;
   out_4209141269458760337[237] = 0.0;
   out_4209141269458760337[238] = 0.0;
   out_4209141269458760337[239] = 0.0;
   out_4209141269458760337[240] = 0.0;
   out_4209141269458760337[241] = 0.0;
   out_4209141269458760337[242] = 0.0;
   out_4209141269458760337[243] = 0.0;
   out_4209141269458760337[244] = 0.0;
   out_4209141269458760337[245] = 0.0;
   out_4209141269458760337[246] = 0.0;
   out_4209141269458760337[247] = 1.0;
   out_4209141269458760337[248] = 0.0;
   out_4209141269458760337[249] = 0.0;
   out_4209141269458760337[250] = 0.0;
   out_4209141269458760337[251] = 0.0;
   out_4209141269458760337[252] = 0.0;
   out_4209141269458760337[253] = 0.0;
   out_4209141269458760337[254] = 0.0;
   out_4209141269458760337[255] = 0.0;
   out_4209141269458760337[256] = 0.0;
   out_4209141269458760337[257] = 0.0;
   out_4209141269458760337[258] = 0.0;
   out_4209141269458760337[259] = 0.0;
   out_4209141269458760337[260] = 0.0;
   out_4209141269458760337[261] = 0.0;
   out_4209141269458760337[262] = 0.0;
   out_4209141269458760337[263] = 0.0;
   out_4209141269458760337[264] = 0.0;
   out_4209141269458760337[265] = 0.0;
   out_4209141269458760337[266] = 1.0;
   out_4209141269458760337[267] = 0.0;
   out_4209141269458760337[268] = 0.0;
   out_4209141269458760337[269] = 0.0;
   out_4209141269458760337[270] = 0.0;
   out_4209141269458760337[271] = 0.0;
   out_4209141269458760337[272] = 0.0;
   out_4209141269458760337[273] = 0.0;
   out_4209141269458760337[274] = 0.0;
   out_4209141269458760337[275] = 0.0;
   out_4209141269458760337[276] = 0.0;
   out_4209141269458760337[277] = 0.0;
   out_4209141269458760337[278] = 0.0;
   out_4209141269458760337[279] = 0.0;
   out_4209141269458760337[280] = 0.0;
   out_4209141269458760337[281] = 0.0;
   out_4209141269458760337[282] = 0.0;
   out_4209141269458760337[283] = 0.0;
   out_4209141269458760337[284] = 0.0;
   out_4209141269458760337[285] = 1.0;
   out_4209141269458760337[286] = 0.0;
   out_4209141269458760337[287] = 0.0;
   out_4209141269458760337[288] = 0.0;
   out_4209141269458760337[289] = 0.0;
   out_4209141269458760337[290] = 0.0;
   out_4209141269458760337[291] = 0.0;
   out_4209141269458760337[292] = 0.0;
   out_4209141269458760337[293] = 0.0;
   out_4209141269458760337[294] = 0.0;
   out_4209141269458760337[295] = 0.0;
   out_4209141269458760337[296] = 0.0;
   out_4209141269458760337[297] = 0.0;
   out_4209141269458760337[298] = 0.0;
   out_4209141269458760337[299] = 0.0;
   out_4209141269458760337[300] = 0.0;
   out_4209141269458760337[301] = 0.0;
   out_4209141269458760337[302] = 0.0;
   out_4209141269458760337[303] = 0.0;
   out_4209141269458760337[304] = 1.0;
   out_4209141269458760337[305] = 0.0;
   out_4209141269458760337[306] = 0.0;
   out_4209141269458760337[307] = 0.0;
   out_4209141269458760337[308] = 0.0;
   out_4209141269458760337[309] = 0.0;
   out_4209141269458760337[310] = 0.0;
   out_4209141269458760337[311] = 0.0;
   out_4209141269458760337[312] = 0.0;
   out_4209141269458760337[313] = 0.0;
   out_4209141269458760337[314] = 0.0;
   out_4209141269458760337[315] = 0.0;
   out_4209141269458760337[316] = 0.0;
   out_4209141269458760337[317] = 0.0;
   out_4209141269458760337[318] = 0.0;
   out_4209141269458760337[319] = 0.0;
   out_4209141269458760337[320] = 0.0;
   out_4209141269458760337[321] = 0.0;
   out_4209141269458760337[322] = 0.0;
   out_4209141269458760337[323] = 1.0;
}
void f_fun(double *state, double dt, double *out_8130059171420837203) {
   out_8130059171420837203[0] = atan2((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), -(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]));
   out_8130059171420837203[1] = asin(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]));
   out_8130059171420837203[2] = atan2(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), -(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]));
   out_8130059171420837203[3] = dt*state[12] + state[3];
   out_8130059171420837203[4] = dt*state[13] + state[4];
   out_8130059171420837203[5] = dt*state[14] + state[5];
   out_8130059171420837203[6] = state[6];
   out_8130059171420837203[7] = state[7];
   out_8130059171420837203[8] = state[8];
   out_8130059171420837203[9] = state[9];
   out_8130059171420837203[10] = state[10];
   out_8130059171420837203[11] = state[11];
   out_8130059171420837203[12] = state[12];
   out_8130059171420837203[13] = state[13];
   out_8130059171420837203[14] = state[14];
   out_8130059171420837203[15] = state[15];
   out_8130059171420837203[16] = state[16];
   out_8130059171420837203[17] = state[17];
}
void F_fun(double *state, double dt, double *out_434290692210433210) {
   out_434290692210433210[0] = ((-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*cos(state[0])*cos(state[1]) - sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*cos(state[0])*cos(state[1]) - sin(dt*state[6])*sin(state[0])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_434290692210433210[1] = ((-sin(dt*state[6])*sin(dt*state[8]) - sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*cos(state[1]) - (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*sin(state[1]) - sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(state[0]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*sin(state[1]) + (-sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) + sin(dt*state[8])*cos(dt*state[6]))*cos(state[1]) - sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(state[0]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_434290692210433210[2] = 0;
   out_434290692210433210[3] = 0;
   out_434290692210433210[4] = 0;
   out_434290692210433210[5] = 0;
   out_434290692210433210[6] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(dt*cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) - dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_434290692210433210[7] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*sin(dt*state[7])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[6])*sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) - dt*sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[7])*cos(dt*state[6])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[8])*sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]) - dt*sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_434290692210433210[8] = ((dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((dt*sin(dt*state[6])*sin(dt*state[8]) + dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_434290692210433210[9] = 0;
   out_434290692210433210[10] = 0;
   out_434290692210433210[11] = 0;
   out_434290692210433210[12] = 0;
   out_434290692210433210[13] = 0;
   out_434290692210433210[14] = 0;
   out_434290692210433210[15] = 0;
   out_434290692210433210[16] = 0;
   out_434290692210433210[17] = 0;
   out_434290692210433210[18] = (-sin(dt*state[7])*sin(state[0])*cos(state[1]) - sin(dt*state[8])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_434290692210433210[19] = (-sin(dt*state[7])*sin(state[1])*cos(state[0]) + sin(dt*state[8])*sin(state[0])*sin(state[1])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_434290692210433210[20] = 0;
   out_434290692210433210[21] = 0;
   out_434290692210433210[22] = 0;
   out_434290692210433210[23] = 0;
   out_434290692210433210[24] = 0;
   out_434290692210433210[25] = (dt*sin(dt*state[7])*sin(dt*state[8])*sin(state[0])*cos(state[1]) - dt*sin(dt*state[7])*sin(state[1])*cos(dt*state[8]) + dt*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_434290692210433210[26] = (-dt*sin(dt*state[8])*sin(state[1])*cos(dt*state[7]) - dt*sin(state[0])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_434290692210433210[27] = 0;
   out_434290692210433210[28] = 0;
   out_434290692210433210[29] = 0;
   out_434290692210433210[30] = 0;
   out_434290692210433210[31] = 0;
   out_434290692210433210[32] = 0;
   out_434290692210433210[33] = 0;
   out_434290692210433210[34] = 0;
   out_434290692210433210[35] = 0;
   out_434290692210433210[36] = ((sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_434290692210433210[37] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-sin(dt*state[7])*sin(state[2])*cos(state[0])*cos(state[1]) + sin(dt*state[8])*sin(state[0])*sin(state[2])*cos(dt*state[7])*cos(state[1]) - sin(state[1])*sin(state[2])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(-sin(dt*state[7])*cos(state[0])*cos(state[1])*cos(state[2]) + sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1])*cos(state[2]) - sin(state[1])*cos(dt*state[7])*cos(dt*state[8])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_434290692210433210[38] = ((-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (-sin(state[0])*sin(state[1])*sin(state[2]) - cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_434290692210433210[39] = 0;
   out_434290692210433210[40] = 0;
   out_434290692210433210[41] = 0;
   out_434290692210433210[42] = 0;
   out_434290692210433210[43] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(dt*(sin(state[0])*cos(state[2]) - sin(state[1])*sin(state[2])*cos(state[0]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*sin(state[2])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(dt*(-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_434290692210433210[44] = (dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*sin(state[2])*cos(dt*state[7])*cos(state[1]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + (dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[7])*cos(state[1])*cos(state[2]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_434290692210433210[45] = 0;
   out_434290692210433210[46] = 0;
   out_434290692210433210[47] = 0;
   out_434290692210433210[48] = 0;
   out_434290692210433210[49] = 0;
   out_434290692210433210[50] = 0;
   out_434290692210433210[51] = 0;
   out_434290692210433210[52] = 0;
   out_434290692210433210[53] = 0;
   out_434290692210433210[54] = 0;
   out_434290692210433210[55] = 0;
   out_434290692210433210[56] = 0;
   out_434290692210433210[57] = 1;
   out_434290692210433210[58] = 0;
   out_434290692210433210[59] = 0;
   out_434290692210433210[60] = 0;
   out_434290692210433210[61] = 0;
   out_434290692210433210[62] = 0;
   out_434290692210433210[63] = 0;
   out_434290692210433210[64] = 0;
   out_434290692210433210[65] = 0;
   out_434290692210433210[66] = dt;
   out_434290692210433210[67] = 0;
   out_434290692210433210[68] = 0;
   out_434290692210433210[69] = 0;
   out_434290692210433210[70] = 0;
   out_434290692210433210[71] = 0;
   out_434290692210433210[72] = 0;
   out_434290692210433210[73] = 0;
   out_434290692210433210[74] = 0;
   out_434290692210433210[75] = 0;
   out_434290692210433210[76] = 1;
   out_434290692210433210[77] = 0;
   out_434290692210433210[78] = 0;
   out_434290692210433210[79] = 0;
   out_434290692210433210[80] = 0;
   out_434290692210433210[81] = 0;
   out_434290692210433210[82] = 0;
   out_434290692210433210[83] = 0;
   out_434290692210433210[84] = 0;
   out_434290692210433210[85] = dt;
   out_434290692210433210[86] = 0;
   out_434290692210433210[87] = 0;
   out_434290692210433210[88] = 0;
   out_434290692210433210[89] = 0;
   out_434290692210433210[90] = 0;
   out_434290692210433210[91] = 0;
   out_434290692210433210[92] = 0;
   out_434290692210433210[93] = 0;
   out_434290692210433210[94] = 0;
   out_434290692210433210[95] = 1;
   out_434290692210433210[96] = 0;
   out_434290692210433210[97] = 0;
   out_434290692210433210[98] = 0;
   out_434290692210433210[99] = 0;
   out_434290692210433210[100] = 0;
   out_434290692210433210[101] = 0;
   out_434290692210433210[102] = 0;
   out_434290692210433210[103] = 0;
   out_434290692210433210[104] = dt;
   out_434290692210433210[105] = 0;
   out_434290692210433210[106] = 0;
   out_434290692210433210[107] = 0;
   out_434290692210433210[108] = 0;
   out_434290692210433210[109] = 0;
   out_434290692210433210[110] = 0;
   out_434290692210433210[111] = 0;
   out_434290692210433210[112] = 0;
   out_434290692210433210[113] = 0;
   out_434290692210433210[114] = 1;
   out_434290692210433210[115] = 0;
   out_434290692210433210[116] = 0;
   out_434290692210433210[117] = 0;
   out_434290692210433210[118] = 0;
   out_434290692210433210[119] = 0;
   out_434290692210433210[120] = 0;
   out_434290692210433210[121] = 0;
   out_434290692210433210[122] = 0;
   out_434290692210433210[123] = 0;
   out_434290692210433210[124] = 0;
   out_434290692210433210[125] = 0;
   out_434290692210433210[126] = 0;
   out_434290692210433210[127] = 0;
   out_434290692210433210[128] = 0;
   out_434290692210433210[129] = 0;
   out_434290692210433210[130] = 0;
   out_434290692210433210[131] = 0;
   out_434290692210433210[132] = 0;
   out_434290692210433210[133] = 1;
   out_434290692210433210[134] = 0;
   out_434290692210433210[135] = 0;
   out_434290692210433210[136] = 0;
   out_434290692210433210[137] = 0;
   out_434290692210433210[138] = 0;
   out_434290692210433210[139] = 0;
   out_434290692210433210[140] = 0;
   out_434290692210433210[141] = 0;
   out_434290692210433210[142] = 0;
   out_434290692210433210[143] = 0;
   out_434290692210433210[144] = 0;
   out_434290692210433210[145] = 0;
   out_434290692210433210[146] = 0;
   out_434290692210433210[147] = 0;
   out_434290692210433210[148] = 0;
   out_434290692210433210[149] = 0;
   out_434290692210433210[150] = 0;
   out_434290692210433210[151] = 0;
   out_434290692210433210[152] = 1;
   out_434290692210433210[153] = 0;
   out_434290692210433210[154] = 0;
   out_434290692210433210[155] = 0;
   out_434290692210433210[156] = 0;
   out_434290692210433210[157] = 0;
   out_434290692210433210[158] = 0;
   out_434290692210433210[159] = 0;
   out_434290692210433210[160] = 0;
   out_434290692210433210[161] = 0;
   out_434290692210433210[162] = 0;
   out_434290692210433210[163] = 0;
   out_434290692210433210[164] = 0;
   out_434290692210433210[165] = 0;
   out_434290692210433210[166] = 0;
   out_434290692210433210[167] = 0;
   out_434290692210433210[168] = 0;
   out_434290692210433210[169] = 0;
   out_434290692210433210[170] = 0;
   out_434290692210433210[171] = 1;
   out_434290692210433210[172] = 0;
   out_434290692210433210[173] = 0;
   out_434290692210433210[174] = 0;
   out_434290692210433210[175] = 0;
   out_434290692210433210[176] = 0;
   out_434290692210433210[177] = 0;
   out_434290692210433210[178] = 0;
   out_434290692210433210[179] = 0;
   out_434290692210433210[180] = 0;
   out_434290692210433210[181] = 0;
   out_434290692210433210[182] = 0;
   out_434290692210433210[183] = 0;
   out_434290692210433210[184] = 0;
   out_434290692210433210[185] = 0;
   out_434290692210433210[186] = 0;
   out_434290692210433210[187] = 0;
   out_434290692210433210[188] = 0;
   out_434290692210433210[189] = 0;
   out_434290692210433210[190] = 1;
   out_434290692210433210[191] = 0;
   out_434290692210433210[192] = 0;
   out_434290692210433210[193] = 0;
   out_434290692210433210[194] = 0;
   out_434290692210433210[195] = 0;
   out_434290692210433210[196] = 0;
   out_434290692210433210[197] = 0;
   out_434290692210433210[198] = 0;
   out_434290692210433210[199] = 0;
   out_434290692210433210[200] = 0;
   out_434290692210433210[201] = 0;
   out_434290692210433210[202] = 0;
   out_434290692210433210[203] = 0;
   out_434290692210433210[204] = 0;
   out_434290692210433210[205] = 0;
   out_434290692210433210[206] = 0;
   out_434290692210433210[207] = 0;
   out_434290692210433210[208] = 0;
   out_434290692210433210[209] = 1;
   out_434290692210433210[210] = 0;
   out_434290692210433210[211] = 0;
   out_434290692210433210[212] = 0;
   out_434290692210433210[213] = 0;
   out_434290692210433210[214] = 0;
   out_434290692210433210[215] = 0;
   out_434290692210433210[216] = 0;
   out_434290692210433210[217] = 0;
   out_434290692210433210[218] = 0;
   out_434290692210433210[219] = 0;
   out_434290692210433210[220] = 0;
   out_434290692210433210[221] = 0;
   out_434290692210433210[222] = 0;
   out_434290692210433210[223] = 0;
   out_434290692210433210[224] = 0;
   out_434290692210433210[225] = 0;
   out_434290692210433210[226] = 0;
   out_434290692210433210[227] = 0;
   out_434290692210433210[228] = 1;
   out_434290692210433210[229] = 0;
   out_434290692210433210[230] = 0;
   out_434290692210433210[231] = 0;
   out_434290692210433210[232] = 0;
   out_434290692210433210[233] = 0;
   out_434290692210433210[234] = 0;
   out_434290692210433210[235] = 0;
   out_434290692210433210[236] = 0;
   out_434290692210433210[237] = 0;
   out_434290692210433210[238] = 0;
   out_434290692210433210[239] = 0;
   out_434290692210433210[240] = 0;
   out_434290692210433210[241] = 0;
   out_434290692210433210[242] = 0;
   out_434290692210433210[243] = 0;
   out_434290692210433210[244] = 0;
   out_434290692210433210[245] = 0;
   out_434290692210433210[246] = 0;
   out_434290692210433210[247] = 1;
   out_434290692210433210[248] = 0;
   out_434290692210433210[249] = 0;
   out_434290692210433210[250] = 0;
   out_434290692210433210[251] = 0;
   out_434290692210433210[252] = 0;
   out_434290692210433210[253] = 0;
   out_434290692210433210[254] = 0;
   out_434290692210433210[255] = 0;
   out_434290692210433210[256] = 0;
   out_434290692210433210[257] = 0;
   out_434290692210433210[258] = 0;
   out_434290692210433210[259] = 0;
   out_434290692210433210[260] = 0;
   out_434290692210433210[261] = 0;
   out_434290692210433210[262] = 0;
   out_434290692210433210[263] = 0;
   out_434290692210433210[264] = 0;
   out_434290692210433210[265] = 0;
   out_434290692210433210[266] = 1;
   out_434290692210433210[267] = 0;
   out_434290692210433210[268] = 0;
   out_434290692210433210[269] = 0;
   out_434290692210433210[270] = 0;
   out_434290692210433210[271] = 0;
   out_434290692210433210[272] = 0;
   out_434290692210433210[273] = 0;
   out_434290692210433210[274] = 0;
   out_434290692210433210[275] = 0;
   out_434290692210433210[276] = 0;
   out_434290692210433210[277] = 0;
   out_434290692210433210[278] = 0;
   out_434290692210433210[279] = 0;
   out_434290692210433210[280] = 0;
   out_434290692210433210[281] = 0;
   out_434290692210433210[282] = 0;
   out_434290692210433210[283] = 0;
   out_434290692210433210[284] = 0;
   out_434290692210433210[285] = 1;
   out_434290692210433210[286] = 0;
   out_434290692210433210[287] = 0;
   out_434290692210433210[288] = 0;
   out_434290692210433210[289] = 0;
   out_434290692210433210[290] = 0;
   out_434290692210433210[291] = 0;
   out_434290692210433210[292] = 0;
   out_434290692210433210[293] = 0;
   out_434290692210433210[294] = 0;
   out_434290692210433210[295] = 0;
   out_434290692210433210[296] = 0;
   out_434290692210433210[297] = 0;
   out_434290692210433210[298] = 0;
   out_434290692210433210[299] = 0;
   out_434290692210433210[300] = 0;
   out_434290692210433210[301] = 0;
   out_434290692210433210[302] = 0;
   out_434290692210433210[303] = 0;
   out_434290692210433210[304] = 1;
   out_434290692210433210[305] = 0;
   out_434290692210433210[306] = 0;
   out_434290692210433210[307] = 0;
   out_434290692210433210[308] = 0;
   out_434290692210433210[309] = 0;
   out_434290692210433210[310] = 0;
   out_434290692210433210[311] = 0;
   out_434290692210433210[312] = 0;
   out_434290692210433210[313] = 0;
   out_434290692210433210[314] = 0;
   out_434290692210433210[315] = 0;
   out_434290692210433210[316] = 0;
   out_434290692210433210[317] = 0;
   out_434290692210433210[318] = 0;
   out_434290692210433210[319] = 0;
   out_434290692210433210[320] = 0;
   out_434290692210433210[321] = 0;
   out_434290692210433210[322] = 0;
   out_434290692210433210[323] = 1;
}
void h_4(double *state, double *unused, double *out_7847268677212961006) {
   out_7847268677212961006[0] = state[6] + state[9];
   out_7847268677212961006[1] = state[7] + state[10];
   out_7847268677212961006[2] = state[8] + state[11];
}
void H_4(double *state, double *unused, double *out_2389442543552463476) {
   out_2389442543552463476[0] = 0;
   out_2389442543552463476[1] = 0;
   out_2389442543552463476[2] = 0;
   out_2389442543552463476[3] = 0;
   out_2389442543552463476[4] = 0;
   out_2389442543552463476[5] = 0;
   out_2389442543552463476[6] = 1;
   out_2389442543552463476[7] = 0;
   out_2389442543552463476[8] = 0;
   out_2389442543552463476[9] = 1;
   out_2389442543552463476[10] = 0;
   out_2389442543552463476[11] = 0;
   out_2389442543552463476[12] = 0;
   out_2389442543552463476[13] = 0;
   out_2389442543552463476[14] = 0;
   out_2389442543552463476[15] = 0;
   out_2389442543552463476[16] = 0;
   out_2389442543552463476[17] = 0;
   out_2389442543552463476[18] = 0;
   out_2389442543552463476[19] = 0;
   out_2389442543552463476[20] = 0;
   out_2389442543552463476[21] = 0;
   out_2389442543552463476[22] = 0;
   out_2389442543552463476[23] = 0;
   out_2389442543552463476[24] = 0;
   out_2389442543552463476[25] = 1;
   out_2389442543552463476[26] = 0;
   out_2389442543552463476[27] = 0;
   out_2389442543552463476[28] = 1;
   out_2389442543552463476[29] = 0;
   out_2389442543552463476[30] = 0;
   out_2389442543552463476[31] = 0;
   out_2389442543552463476[32] = 0;
   out_2389442543552463476[33] = 0;
   out_2389442543552463476[34] = 0;
   out_2389442543552463476[35] = 0;
   out_2389442543552463476[36] = 0;
   out_2389442543552463476[37] = 0;
   out_2389442543552463476[38] = 0;
   out_2389442543552463476[39] = 0;
   out_2389442543552463476[40] = 0;
   out_2389442543552463476[41] = 0;
   out_2389442543552463476[42] = 0;
   out_2389442543552463476[43] = 0;
   out_2389442543552463476[44] = 1;
   out_2389442543552463476[45] = 0;
   out_2389442543552463476[46] = 0;
   out_2389442543552463476[47] = 1;
   out_2389442543552463476[48] = 0;
   out_2389442543552463476[49] = 0;
   out_2389442543552463476[50] = 0;
   out_2389442543552463476[51] = 0;
   out_2389442543552463476[52] = 0;
   out_2389442543552463476[53] = 0;
}
void h_10(double *state, double *unused, double *out_7815524616711553503) {
   out_7815524616711553503[0] = 9.8100000000000005*sin(state[1]) - state[4]*state[8] + state[5]*state[7] + state[12] + state[15];
   out_7815524616711553503[1] = -9.8100000000000005*sin(state[0])*cos(state[1]) + state[3]*state[8] - state[5]*state[6] + state[13] + state[16];
   out_7815524616711553503[2] = -9.8100000000000005*cos(state[0])*cos(state[1]) - state[3]*state[7] + state[4]*state[6] + state[14] + state[17];
}
void H_10(double *state, double *unused, double *out_5409770976600334064) {
   out_5409770976600334064[0] = 0;
   out_5409770976600334064[1] = 9.8100000000000005*cos(state[1]);
   out_5409770976600334064[2] = 0;
   out_5409770976600334064[3] = 0;
   out_5409770976600334064[4] = -state[8];
   out_5409770976600334064[5] = state[7];
   out_5409770976600334064[6] = 0;
   out_5409770976600334064[7] = state[5];
   out_5409770976600334064[8] = -state[4];
   out_5409770976600334064[9] = 0;
   out_5409770976600334064[10] = 0;
   out_5409770976600334064[11] = 0;
   out_5409770976600334064[12] = 1;
   out_5409770976600334064[13] = 0;
   out_5409770976600334064[14] = 0;
   out_5409770976600334064[15] = 1;
   out_5409770976600334064[16] = 0;
   out_5409770976600334064[17] = 0;
   out_5409770976600334064[18] = -9.8100000000000005*cos(state[0])*cos(state[1]);
   out_5409770976600334064[19] = 9.8100000000000005*sin(state[0])*sin(state[1]);
   out_5409770976600334064[20] = 0;
   out_5409770976600334064[21] = state[8];
   out_5409770976600334064[22] = 0;
   out_5409770976600334064[23] = -state[6];
   out_5409770976600334064[24] = -state[5];
   out_5409770976600334064[25] = 0;
   out_5409770976600334064[26] = state[3];
   out_5409770976600334064[27] = 0;
   out_5409770976600334064[28] = 0;
   out_5409770976600334064[29] = 0;
   out_5409770976600334064[30] = 0;
   out_5409770976600334064[31] = 1;
   out_5409770976600334064[32] = 0;
   out_5409770976600334064[33] = 0;
   out_5409770976600334064[34] = 1;
   out_5409770976600334064[35] = 0;
   out_5409770976600334064[36] = 9.8100000000000005*sin(state[0])*cos(state[1]);
   out_5409770976600334064[37] = 9.8100000000000005*sin(state[1])*cos(state[0]);
   out_5409770976600334064[38] = 0;
   out_5409770976600334064[39] = -state[7];
   out_5409770976600334064[40] = state[6];
   out_5409770976600334064[41] = 0;
   out_5409770976600334064[42] = state[4];
   out_5409770976600334064[43] = -state[3];
   out_5409770976600334064[44] = 0;
   out_5409770976600334064[45] = 0;
   out_5409770976600334064[46] = 0;
   out_5409770976600334064[47] = 0;
   out_5409770976600334064[48] = 0;
   out_5409770976600334064[49] = 0;
   out_5409770976600334064[50] = 1;
   out_5409770976600334064[51] = 0;
   out_5409770976600334064[52] = 0;
   out_5409770976600334064[53] = 1;
}
void h_13(double *state, double *unused, double *out_1287863093787623213) {
   out_1287863093787623213[0] = state[3];
   out_1287863093787623213[1] = state[4];
   out_1287863093787623213[2] = state[5];
}
void H_13(double *state, double *unused, double *out_8446670321840387211) {
   out_8446670321840387211[0] = 0;
   out_8446670321840387211[1] = 0;
   out_8446670321840387211[2] = 0;
   out_8446670321840387211[3] = 1;
   out_8446670321840387211[4] = 0;
   out_8446670321840387211[5] = 0;
   out_8446670321840387211[6] = 0;
   out_8446670321840387211[7] = 0;
   out_8446670321840387211[8] = 0;
   out_8446670321840387211[9] = 0;
   out_8446670321840387211[10] = 0;
   out_8446670321840387211[11] = 0;
   out_8446670321840387211[12] = 0;
   out_8446670321840387211[13] = 0;
   out_8446670321840387211[14] = 0;
   out_8446670321840387211[15] = 0;
   out_8446670321840387211[16] = 0;
   out_8446670321840387211[17] = 0;
   out_8446670321840387211[18] = 0;
   out_8446670321840387211[19] = 0;
   out_8446670321840387211[20] = 0;
   out_8446670321840387211[21] = 0;
   out_8446670321840387211[22] = 1;
   out_8446670321840387211[23] = 0;
   out_8446670321840387211[24] = 0;
   out_8446670321840387211[25] = 0;
   out_8446670321840387211[26] = 0;
   out_8446670321840387211[27] = 0;
   out_8446670321840387211[28] = 0;
   out_8446670321840387211[29] = 0;
   out_8446670321840387211[30] = 0;
   out_8446670321840387211[31] = 0;
   out_8446670321840387211[32] = 0;
   out_8446670321840387211[33] = 0;
   out_8446670321840387211[34] = 0;
   out_8446670321840387211[35] = 0;
   out_8446670321840387211[36] = 0;
   out_8446670321840387211[37] = 0;
   out_8446670321840387211[38] = 0;
   out_8446670321840387211[39] = 0;
   out_8446670321840387211[40] = 0;
   out_8446670321840387211[41] = 1;
   out_8446670321840387211[42] = 0;
   out_8446670321840387211[43] = 0;
   out_8446670321840387211[44] = 0;
   out_8446670321840387211[45] = 0;
   out_8446670321840387211[46] = 0;
   out_8446670321840387211[47] = 0;
   out_8446670321840387211[48] = 0;
   out_8446670321840387211[49] = 0;
   out_8446670321840387211[50] = 0;
   out_8446670321840387211[51] = 0;
   out_8446670321840387211[52] = 0;
   out_8446670321840387211[53] = 0;
}
void h_14(double *state, double *unused, double *out_8895375992006815785) {
   out_8895375992006815785[0] = state[6];
   out_8895375992006815785[1] = state[7];
   out_8895375992006815785[2] = state[8];
}
void H_14(double *state, double *unused, double *out_693345888742908820) {
   out_693345888742908820[0] = 0;
   out_693345888742908820[1] = 0;
   out_693345888742908820[2] = 0;
   out_693345888742908820[3] = 0;
   out_693345888742908820[4] = 0;
   out_693345888742908820[5] = 0;
   out_693345888742908820[6] = 1;
   out_693345888742908820[7] = 0;
   out_693345888742908820[8] = 0;
   out_693345888742908820[9] = 0;
   out_693345888742908820[10] = 0;
   out_693345888742908820[11] = 0;
   out_693345888742908820[12] = 0;
   out_693345888742908820[13] = 0;
   out_693345888742908820[14] = 0;
   out_693345888742908820[15] = 0;
   out_693345888742908820[16] = 0;
   out_693345888742908820[17] = 0;
   out_693345888742908820[18] = 0;
   out_693345888742908820[19] = 0;
   out_693345888742908820[20] = 0;
   out_693345888742908820[21] = 0;
   out_693345888742908820[22] = 0;
   out_693345888742908820[23] = 0;
   out_693345888742908820[24] = 0;
   out_693345888742908820[25] = 1;
   out_693345888742908820[26] = 0;
   out_693345888742908820[27] = 0;
   out_693345888742908820[28] = 0;
   out_693345888742908820[29] = 0;
   out_693345888742908820[30] = 0;
   out_693345888742908820[31] = 0;
   out_693345888742908820[32] = 0;
   out_693345888742908820[33] = 0;
   out_693345888742908820[34] = 0;
   out_693345888742908820[35] = 0;
   out_693345888742908820[36] = 0;
   out_693345888742908820[37] = 0;
   out_693345888742908820[38] = 0;
   out_693345888742908820[39] = 0;
   out_693345888742908820[40] = 0;
   out_693345888742908820[41] = 0;
   out_693345888742908820[42] = 0;
   out_693345888742908820[43] = 0;
   out_693345888742908820[44] = 1;
   out_693345888742908820[45] = 0;
   out_693345888742908820[46] = 0;
   out_693345888742908820[47] = 0;
   out_693345888742908820[48] = 0;
   out_693345888742908820[49] = 0;
   out_693345888742908820[50] = 0;
   out_693345888742908820[51] = 0;
   out_693345888742908820[52] = 0;
   out_693345888742908820[53] = 0;
}
#include <eigen3/Eigen/Dense>
#include <iostream>

typedef Eigen::Matrix<double, DIM, DIM, Eigen::RowMajor> DDM;
typedef Eigen::Matrix<double, EDIM, EDIM, Eigen::RowMajor> EEM;
typedef Eigen::Matrix<double, DIM, EDIM, Eigen::RowMajor> DEM;

void predict(double *in_x, double *in_P, double *in_Q, double dt) {
  typedef Eigen::Matrix<double, MEDIM, MEDIM, Eigen::RowMajor> RRM;

  double nx[DIM] = {0};
  double in_F[EDIM*EDIM] = {0};

  // functions from sympy
  f_fun(in_x, dt, nx);
  F_fun(in_x, dt, in_F);


  EEM F(in_F);
  EEM P(in_P);
  EEM Q(in_Q);

  RRM F_main = F.topLeftCorner(MEDIM, MEDIM);
  P.topLeftCorner(MEDIM, MEDIM) = (F_main * P.topLeftCorner(MEDIM, MEDIM)) * F_main.transpose();
  P.topRightCorner(MEDIM, EDIM - MEDIM) = F_main * P.topRightCorner(MEDIM, EDIM - MEDIM);
  P.bottomLeftCorner(EDIM - MEDIM, MEDIM) = P.bottomLeftCorner(EDIM - MEDIM, MEDIM) * F_main.transpose();

  P = P + dt*Q;

  // copy out state
  memcpy(in_x, nx, DIM * sizeof(double));
  memcpy(in_P, P.data(), EDIM * EDIM * sizeof(double));
}

// note: extra_args dim only correct when null space projecting
// otherwise 1
template <int ZDIM, int EADIM, bool MAHA_TEST>
void update(double *in_x, double *in_P, Hfun h_fun, Hfun H_fun, Hfun Hea_fun, double *in_z, double *in_R, double *in_ea, double MAHA_THRESHOLD) {
  typedef Eigen::Matrix<double, ZDIM, ZDIM, Eigen::RowMajor> ZZM;
  typedef Eigen::Matrix<double, ZDIM, DIM, Eigen::RowMajor> ZDM;
  typedef Eigen::Matrix<double, Eigen::Dynamic, EDIM, Eigen::RowMajor> XEM;
  //typedef Eigen::Matrix<double, EDIM, ZDIM, Eigen::RowMajor> EZM;
  typedef Eigen::Matrix<double, Eigen::Dynamic, 1> X1M;
  typedef Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> XXM;

  double in_hx[ZDIM] = {0};
  double in_H[ZDIM * DIM] = {0};
  double in_H_mod[EDIM * DIM] = {0};
  double delta_x[EDIM] = {0};
  double x_new[DIM] = {0};


  // state x, P
  Eigen::Matrix<double, ZDIM, 1> z(in_z);
  EEM P(in_P);
  ZZM pre_R(in_R);

  // functions from sympy
  h_fun(in_x, in_ea, in_hx);
  H_fun(in_x, in_ea, in_H);
  ZDM pre_H(in_H);

  // get y (y = z - hx)
  Eigen::Matrix<double, ZDIM, 1> pre_y(in_hx); pre_y = z - pre_y;
  X1M y; XXM H; XXM R;
  if (Hea_fun){
    typedef Eigen::Matrix<double, ZDIM, EADIM, Eigen::RowMajor> ZAM;
    double in_Hea[ZDIM * EADIM] = {0};
    Hea_fun(in_x, in_ea, in_Hea);
    ZAM Hea(in_Hea);
    XXM A = Hea.transpose().fullPivLu().kernel();


    y = A.transpose() * pre_y;
    H = A.transpose() * pre_H;
    R = A.transpose() * pre_R * A;
  } else {
    y = pre_y;
    H = pre_H;
    R = pre_R;
  }
  // get modified H
  H_mod_fun(in_x, in_H_mod);
  DEM H_mod(in_H_mod);
  XEM H_err = H * H_mod;

  // Do mahalobis distance test
  if (MAHA_TEST){
    XXM a = (H_err * P * H_err.transpose() + R).inverse();
    double maha_dist = y.transpose() * a * y;
    if (maha_dist > MAHA_THRESHOLD){
      R = 1.0e16 * R;
    }
  }

  // Outlier resilient weighting
  double weight = 1;//(1.5)/(1 + y.squaredNorm()/R.sum());

  // kalman gains and I_KH
  XXM S = ((H_err * P) * H_err.transpose()) + R/weight;
  XEM KT = S.fullPivLu().solve(H_err * P.transpose());
  //EZM K = KT.transpose(); TODO: WHY DOES THIS NOT COMPILE?
  //EZM K = S.fullPivLu().solve(H_err * P.transpose()).transpose();
  //std::cout << "Here is the matrix rot:\n" << K << std::endl;
  EEM I_KH = Eigen::Matrix<double, EDIM, EDIM>::Identity() - (KT.transpose() * H_err);

  // update state by injecting dx
  Eigen::Matrix<double, EDIM, 1> dx(delta_x);
  dx  = (KT.transpose() * y);
  memcpy(delta_x, dx.data(), EDIM * sizeof(double));
  err_fun(in_x, delta_x, x_new);
  Eigen::Matrix<double, DIM, 1> x(x_new);

  // update cov
  P = ((I_KH * P) * I_KH.transpose()) + ((KT.transpose() * R) * KT);

  // copy out state
  memcpy(in_x, x.data(), DIM * sizeof(double));
  memcpy(in_P, P.data(), EDIM * EDIM * sizeof(double));
  memcpy(in_z, y.data(), y.rows() * sizeof(double));
}




}
extern "C" {

void pose_update_4(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<3, 3, 0>(in_x, in_P, h_4, H_4, NULL, in_z, in_R, in_ea, MAHA_THRESH_4);
}
void pose_update_10(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<3, 3, 0>(in_x, in_P, h_10, H_10, NULL, in_z, in_R, in_ea, MAHA_THRESH_10);
}
void pose_update_13(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<3, 3, 0>(in_x, in_P, h_13, H_13, NULL, in_z, in_R, in_ea, MAHA_THRESH_13);
}
void pose_update_14(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<3, 3, 0>(in_x, in_P, h_14, H_14, NULL, in_z, in_R, in_ea, MAHA_THRESH_14);
}
void pose_err_fun(double *nom_x, double *delta_x, double *out_1937455278130310156) {
  err_fun(nom_x, delta_x, out_1937455278130310156);
}
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_2984526074350005589) {
  inv_err_fun(nom_x, true_x, out_2984526074350005589);
}
void pose_H_mod_fun(double *state, double *out_4209141269458760337) {
  H_mod_fun(state, out_4209141269458760337);
}
void pose_f_fun(double *state, double dt, double *out_8130059171420837203) {
  f_fun(state,  dt, out_8130059171420837203);
}
void pose_F_fun(double *state, double dt, double *out_434290692210433210) {
  F_fun(state,  dt, out_434290692210433210);
}
void pose_h_4(double *state, double *unused, double *out_7847268677212961006) {
  h_4(state, unused, out_7847268677212961006);
}
void pose_H_4(double *state, double *unused, double *out_2389442543552463476) {
  H_4(state, unused, out_2389442543552463476);
}
void pose_h_10(double *state, double *unused, double *out_7815524616711553503) {
  h_10(state, unused, out_7815524616711553503);
}
void pose_H_10(double *state, double *unused, double *out_5409770976600334064) {
  H_10(state, unused, out_5409770976600334064);
}
void pose_h_13(double *state, double *unused, double *out_1287863093787623213) {
  h_13(state, unused, out_1287863093787623213);
}
void pose_H_13(double *state, double *unused, double *out_8446670321840387211) {
  H_13(state, unused, out_8446670321840387211);
}
void pose_h_14(double *state, double *unused, double *out_8895375992006815785) {
  h_14(state, unused, out_8895375992006815785);
}
void pose_H_14(double *state, double *unused, double *out_693345888742908820) {
  H_14(state, unused, out_693345888742908820);
}
void pose_predict(double *in_x, double *in_P, double *in_Q, double dt) {
  predict(in_x, in_P, in_Q, dt);
}
}

const EKF pose = {
  .name = "pose",
  .kinds = { 4, 10, 13, 14 },
  .feature_kinds = {  },
  .f_fun = pose_f_fun,
  .F_fun = pose_F_fun,
  .err_fun = pose_err_fun,
  .inv_err_fun = pose_inv_err_fun,
  .H_mod_fun = pose_H_mod_fun,
  .predict = pose_predict,
  .hs = {
    { 4, pose_h_4 },
    { 10, pose_h_10 },
    { 13, pose_h_13 },
    { 14, pose_h_14 },
  },
  .Hs = {
    { 4, pose_H_4 },
    { 10, pose_H_10 },
    { 13, pose_H_13 },
    { 14, pose_H_14 },
  },
  .updates = {
    { 4, pose_update_4 },
    { 10, pose_update_10 },
    { 13, pose_update_13 },
    { 14, pose_update_14 },
  },
  .Hes = {
  },
  .sets = {
  },
  .extra_routines = {
  },
};

ekf_lib_init(pose)
