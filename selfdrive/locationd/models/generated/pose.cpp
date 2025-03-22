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
void err_fun(double *nom_x, double *delta_x, double *out_1487504118503105920) {
   out_1487504118503105920[0] = delta_x[0] + nom_x[0];
   out_1487504118503105920[1] = delta_x[1] + nom_x[1];
   out_1487504118503105920[2] = delta_x[2] + nom_x[2];
   out_1487504118503105920[3] = delta_x[3] + nom_x[3];
   out_1487504118503105920[4] = delta_x[4] + nom_x[4];
   out_1487504118503105920[5] = delta_x[5] + nom_x[5];
   out_1487504118503105920[6] = delta_x[6] + nom_x[6];
   out_1487504118503105920[7] = delta_x[7] + nom_x[7];
   out_1487504118503105920[8] = delta_x[8] + nom_x[8];
   out_1487504118503105920[9] = delta_x[9] + nom_x[9];
   out_1487504118503105920[10] = delta_x[10] + nom_x[10];
   out_1487504118503105920[11] = delta_x[11] + nom_x[11];
   out_1487504118503105920[12] = delta_x[12] + nom_x[12];
   out_1487504118503105920[13] = delta_x[13] + nom_x[13];
   out_1487504118503105920[14] = delta_x[14] + nom_x[14];
   out_1487504118503105920[15] = delta_x[15] + nom_x[15];
   out_1487504118503105920[16] = delta_x[16] + nom_x[16];
   out_1487504118503105920[17] = delta_x[17] + nom_x[17];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_2323018677029669916) {
   out_2323018677029669916[0] = -nom_x[0] + true_x[0];
   out_2323018677029669916[1] = -nom_x[1] + true_x[1];
   out_2323018677029669916[2] = -nom_x[2] + true_x[2];
   out_2323018677029669916[3] = -nom_x[3] + true_x[3];
   out_2323018677029669916[4] = -nom_x[4] + true_x[4];
   out_2323018677029669916[5] = -nom_x[5] + true_x[5];
   out_2323018677029669916[6] = -nom_x[6] + true_x[6];
   out_2323018677029669916[7] = -nom_x[7] + true_x[7];
   out_2323018677029669916[8] = -nom_x[8] + true_x[8];
   out_2323018677029669916[9] = -nom_x[9] + true_x[9];
   out_2323018677029669916[10] = -nom_x[10] + true_x[10];
   out_2323018677029669916[11] = -nom_x[11] + true_x[11];
   out_2323018677029669916[12] = -nom_x[12] + true_x[12];
   out_2323018677029669916[13] = -nom_x[13] + true_x[13];
   out_2323018677029669916[14] = -nom_x[14] + true_x[14];
   out_2323018677029669916[15] = -nom_x[15] + true_x[15];
   out_2323018677029669916[16] = -nom_x[16] + true_x[16];
   out_2323018677029669916[17] = -nom_x[17] + true_x[17];
}
void H_mod_fun(double *state, double *out_4092507731700215880) {
   out_4092507731700215880[0] = 1.0;
   out_4092507731700215880[1] = 0.0;
   out_4092507731700215880[2] = 0.0;
   out_4092507731700215880[3] = 0.0;
   out_4092507731700215880[4] = 0.0;
   out_4092507731700215880[5] = 0.0;
   out_4092507731700215880[6] = 0.0;
   out_4092507731700215880[7] = 0.0;
   out_4092507731700215880[8] = 0.0;
   out_4092507731700215880[9] = 0.0;
   out_4092507731700215880[10] = 0.0;
   out_4092507731700215880[11] = 0.0;
   out_4092507731700215880[12] = 0.0;
   out_4092507731700215880[13] = 0.0;
   out_4092507731700215880[14] = 0.0;
   out_4092507731700215880[15] = 0.0;
   out_4092507731700215880[16] = 0.0;
   out_4092507731700215880[17] = 0.0;
   out_4092507731700215880[18] = 0.0;
   out_4092507731700215880[19] = 1.0;
   out_4092507731700215880[20] = 0.0;
   out_4092507731700215880[21] = 0.0;
   out_4092507731700215880[22] = 0.0;
   out_4092507731700215880[23] = 0.0;
   out_4092507731700215880[24] = 0.0;
   out_4092507731700215880[25] = 0.0;
   out_4092507731700215880[26] = 0.0;
   out_4092507731700215880[27] = 0.0;
   out_4092507731700215880[28] = 0.0;
   out_4092507731700215880[29] = 0.0;
   out_4092507731700215880[30] = 0.0;
   out_4092507731700215880[31] = 0.0;
   out_4092507731700215880[32] = 0.0;
   out_4092507731700215880[33] = 0.0;
   out_4092507731700215880[34] = 0.0;
   out_4092507731700215880[35] = 0.0;
   out_4092507731700215880[36] = 0.0;
   out_4092507731700215880[37] = 0.0;
   out_4092507731700215880[38] = 1.0;
   out_4092507731700215880[39] = 0.0;
   out_4092507731700215880[40] = 0.0;
   out_4092507731700215880[41] = 0.0;
   out_4092507731700215880[42] = 0.0;
   out_4092507731700215880[43] = 0.0;
   out_4092507731700215880[44] = 0.0;
   out_4092507731700215880[45] = 0.0;
   out_4092507731700215880[46] = 0.0;
   out_4092507731700215880[47] = 0.0;
   out_4092507731700215880[48] = 0.0;
   out_4092507731700215880[49] = 0.0;
   out_4092507731700215880[50] = 0.0;
   out_4092507731700215880[51] = 0.0;
   out_4092507731700215880[52] = 0.0;
   out_4092507731700215880[53] = 0.0;
   out_4092507731700215880[54] = 0.0;
   out_4092507731700215880[55] = 0.0;
   out_4092507731700215880[56] = 0.0;
   out_4092507731700215880[57] = 1.0;
   out_4092507731700215880[58] = 0.0;
   out_4092507731700215880[59] = 0.0;
   out_4092507731700215880[60] = 0.0;
   out_4092507731700215880[61] = 0.0;
   out_4092507731700215880[62] = 0.0;
   out_4092507731700215880[63] = 0.0;
   out_4092507731700215880[64] = 0.0;
   out_4092507731700215880[65] = 0.0;
   out_4092507731700215880[66] = 0.0;
   out_4092507731700215880[67] = 0.0;
   out_4092507731700215880[68] = 0.0;
   out_4092507731700215880[69] = 0.0;
   out_4092507731700215880[70] = 0.0;
   out_4092507731700215880[71] = 0.0;
   out_4092507731700215880[72] = 0.0;
   out_4092507731700215880[73] = 0.0;
   out_4092507731700215880[74] = 0.0;
   out_4092507731700215880[75] = 0.0;
   out_4092507731700215880[76] = 1.0;
   out_4092507731700215880[77] = 0.0;
   out_4092507731700215880[78] = 0.0;
   out_4092507731700215880[79] = 0.0;
   out_4092507731700215880[80] = 0.0;
   out_4092507731700215880[81] = 0.0;
   out_4092507731700215880[82] = 0.0;
   out_4092507731700215880[83] = 0.0;
   out_4092507731700215880[84] = 0.0;
   out_4092507731700215880[85] = 0.0;
   out_4092507731700215880[86] = 0.0;
   out_4092507731700215880[87] = 0.0;
   out_4092507731700215880[88] = 0.0;
   out_4092507731700215880[89] = 0.0;
   out_4092507731700215880[90] = 0.0;
   out_4092507731700215880[91] = 0.0;
   out_4092507731700215880[92] = 0.0;
   out_4092507731700215880[93] = 0.0;
   out_4092507731700215880[94] = 0.0;
   out_4092507731700215880[95] = 1.0;
   out_4092507731700215880[96] = 0.0;
   out_4092507731700215880[97] = 0.0;
   out_4092507731700215880[98] = 0.0;
   out_4092507731700215880[99] = 0.0;
   out_4092507731700215880[100] = 0.0;
   out_4092507731700215880[101] = 0.0;
   out_4092507731700215880[102] = 0.0;
   out_4092507731700215880[103] = 0.0;
   out_4092507731700215880[104] = 0.0;
   out_4092507731700215880[105] = 0.0;
   out_4092507731700215880[106] = 0.0;
   out_4092507731700215880[107] = 0.0;
   out_4092507731700215880[108] = 0.0;
   out_4092507731700215880[109] = 0.0;
   out_4092507731700215880[110] = 0.0;
   out_4092507731700215880[111] = 0.0;
   out_4092507731700215880[112] = 0.0;
   out_4092507731700215880[113] = 0.0;
   out_4092507731700215880[114] = 1.0;
   out_4092507731700215880[115] = 0.0;
   out_4092507731700215880[116] = 0.0;
   out_4092507731700215880[117] = 0.0;
   out_4092507731700215880[118] = 0.0;
   out_4092507731700215880[119] = 0.0;
   out_4092507731700215880[120] = 0.0;
   out_4092507731700215880[121] = 0.0;
   out_4092507731700215880[122] = 0.0;
   out_4092507731700215880[123] = 0.0;
   out_4092507731700215880[124] = 0.0;
   out_4092507731700215880[125] = 0.0;
   out_4092507731700215880[126] = 0.0;
   out_4092507731700215880[127] = 0.0;
   out_4092507731700215880[128] = 0.0;
   out_4092507731700215880[129] = 0.0;
   out_4092507731700215880[130] = 0.0;
   out_4092507731700215880[131] = 0.0;
   out_4092507731700215880[132] = 0.0;
   out_4092507731700215880[133] = 1.0;
   out_4092507731700215880[134] = 0.0;
   out_4092507731700215880[135] = 0.0;
   out_4092507731700215880[136] = 0.0;
   out_4092507731700215880[137] = 0.0;
   out_4092507731700215880[138] = 0.0;
   out_4092507731700215880[139] = 0.0;
   out_4092507731700215880[140] = 0.0;
   out_4092507731700215880[141] = 0.0;
   out_4092507731700215880[142] = 0.0;
   out_4092507731700215880[143] = 0.0;
   out_4092507731700215880[144] = 0.0;
   out_4092507731700215880[145] = 0.0;
   out_4092507731700215880[146] = 0.0;
   out_4092507731700215880[147] = 0.0;
   out_4092507731700215880[148] = 0.0;
   out_4092507731700215880[149] = 0.0;
   out_4092507731700215880[150] = 0.0;
   out_4092507731700215880[151] = 0.0;
   out_4092507731700215880[152] = 1.0;
   out_4092507731700215880[153] = 0.0;
   out_4092507731700215880[154] = 0.0;
   out_4092507731700215880[155] = 0.0;
   out_4092507731700215880[156] = 0.0;
   out_4092507731700215880[157] = 0.0;
   out_4092507731700215880[158] = 0.0;
   out_4092507731700215880[159] = 0.0;
   out_4092507731700215880[160] = 0.0;
   out_4092507731700215880[161] = 0.0;
   out_4092507731700215880[162] = 0.0;
   out_4092507731700215880[163] = 0.0;
   out_4092507731700215880[164] = 0.0;
   out_4092507731700215880[165] = 0.0;
   out_4092507731700215880[166] = 0.0;
   out_4092507731700215880[167] = 0.0;
   out_4092507731700215880[168] = 0.0;
   out_4092507731700215880[169] = 0.0;
   out_4092507731700215880[170] = 0.0;
   out_4092507731700215880[171] = 1.0;
   out_4092507731700215880[172] = 0.0;
   out_4092507731700215880[173] = 0.0;
   out_4092507731700215880[174] = 0.0;
   out_4092507731700215880[175] = 0.0;
   out_4092507731700215880[176] = 0.0;
   out_4092507731700215880[177] = 0.0;
   out_4092507731700215880[178] = 0.0;
   out_4092507731700215880[179] = 0.0;
   out_4092507731700215880[180] = 0.0;
   out_4092507731700215880[181] = 0.0;
   out_4092507731700215880[182] = 0.0;
   out_4092507731700215880[183] = 0.0;
   out_4092507731700215880[184] = 0.0;
   out_4092507731700215880[185] = 0.0;
   out_4092507731700215880[186] = 0.0;
   out_4092507731700215880[187] = 0.0;
   out_4092507731700215880[188] = 0.0;
   out_4092507731700215880[189] = 0.0;
   out_4092507731700215880[190] = 1.0;
   out_4092507731700215880[191] = 0.0;
   out_4092507731700215880[192] = 0.0;
   out_4092507731700215880[193] = 0.0;
   out_4092507731700215880[194] = 0.0;
   out_4092507731700215880[195] = 0.0;
   out_4092507731700215880[196] = 0.0;
   out_4092507731700215880[197] = 0.0;
   out_4092507731700215880[198] = 0.0;
   out_4092507731700215880[199] = 0.0;
   out_4092507731700215880[200] = 0.0;
   out_4092507731700215880[201] = 0.0;
   out_4092507731700215880[202] = 0.0;
   out_4092507731700215880[203] = 0.0;
   out_4092507731700215880[204] = 0.0;
   out_4092507731700215880[205] = 0.0;
   out_4092507731700215880[206] = 0.0;
   out_4092507731700215880[207] = 0.0;
   out_4092507731700215880[208] = 0.0;
   out_4092507731700215880[209] = 1.0;
   out_4092507731700215880[210] = 0.0;
   out_4092507731700215880[211] = 0.0;
   out_4092507731700215880[212] = 0.0;
   out_4092507731700215880[213] = 0.0;
   out_4092507731700215880[214] = 0.0;
   out_4092507731700215880[215] = 0.0;
   out_4092507731700215880[216] = 0.0;
   out_4092507731700215880[217] = 0.0;
   out_4092507731700215880[218] = 0.0;
   out_4092507731700215880[219] = 0.0;
   out_4092507731700215880[220] = 0.0;
   out_4092507731700215880[221] = 0.0;
   out_4092507731700215880[222] = 0.0;
   out_4092507731700215880[223] = 0.0;
   out_4092507731700215880[224] = 0.0;
   out_4092507731700215880[225] = 0.0;
   out_4092507731700215880[226] = 0.0;
   out_4092507731700215880[227] = 0.0;
   out_4092507731700215880[228] = 1.0;
   out_4092507731700215880[229] = 0.0;
   out_4092507731700215880[230] = 0.0;
   out_4092507731700215880[231] = 0.0;
   out_4092507731700215880[232] = 0.0;
   out_4092507731700215880[233] = 0.0;
   out_4092507731700215880[234] = 0.0;
   out_4092507731700215880[235] = 0.0;
   out_4092507731700215880[236] = 0.0;
   out_4092507731700215880[237] = 0.0;
   out_4092507731700215880[238] = 0.0;
   out_4092507731700215880[239] = 0.0;
   out_4092507731700215880[240] = 0.0;
   out_4092507731700215880[241] = 0.0;
   out_4092507731700215880[242] = 0.0;
   out_4092507731700215880[243] = 0.0;
   out_4092507731700215880[244] = 0.0;
   out_4092507731700215880[245] = 0.0;
   out_4092507731700215880[246] = 0.0;
   out_4092507731700215880[247] = 1.0;
   out_4092507731700215880[248] = 0.0;
   out_4092507731700215880[249] = 0.0;
   out_4092507731700215880[250] = 0.0;
   out_4092507731700215880[251] = 0.0;
   out_4092507731700215880[252] = 0.0;
   out_4092507731700215880[253] = 0.0;
   out_4092507731700215880[254] = 0.0;
   out_4092507731700215880[255] = 0.0;
   out_4092507731700215880[256] = 0.0;
   out_4092507731700215880[257] = 0.0;
   out_4092507731700215880[258] = 0.0;
   out_4092507731700215880[259] = 0.0;
   out_4092507731700215880[260] = 0.0;
   out_4092507731700215880[261] = 0.0;
   out_4092507731700215880[262] = 0.0;
   out_4092507731700215880[263] = 0.0;
   out_4092507731700215880[264] = 0.0;
   out_4092507731700215880[265] = 0.0;
   out_4092507731700215880[266] = 1.0;
   out_4092507731700215880[267] = 0.0;
   out_4092507731700215880[268] = 0.0;
   out_4092507731700215880[269] = 0.0;
   out_4092507731700215880[270] = 0.0;
   out_4092507731700215880[271] = 0.0;
   out_4092507731700215880[272] = 0.0;
   out_4092507731700215880[273] = 0.0;
   out_4092507731700215880[274] = 0.0;
   out_4092507731700215880[275] = 0.0;
   out_4092507731700215880[276] = 0.0;
   out_4092507731700215880[277] = 0.0;
   out_4092507731700215880[278] = 0.0;
   out_4092507731700215880[279] = 0.0;
   out_4092507731700215880[280] = 0.0;
   out_4092507731700215880[281] = 0.0;
   out_4092507731700215880[282] = 0.0;
   out_4092507731700215880[283] = 0.0;
   out_4092507731700215880[284] = 0.0;
   out_4092507731700215880[285] = 1.0;
   out_4092507731700215880[286] = 0.0;
   out_4092507731700215880[287] = 0.0;
   out_4092507731700215880[288] = 0.0;
   out_4092507731700215880[289] = 0.0;
   out_4092507731700215880[290] = 0.0;
   out_4092507731700215880[291] = 0.0;
   out_4092507731700215880[292] = 0.0;
   out_4092507731700215880[293] = 0.0;
   out_4092507731700215880[294] = 0.0;
   out_4092507731700215880[295] = 0.0;
   out_4092507731700215880[296] = 0.0;
   out_4092507731700215880[297] = 0.0;
   out_4092507731700215880[298] = 0.0;
   out_4092507731700215880[299] = 0.0;
   out_4092507731700215880[300] = 0.0;
   out_4092507731700215880[301] = 0.0;
   out_4092507731700215880[302] = 0.0;
   out_4092507731700215880[303] = 0.0;
   out_4092507731700215880[304] = 1.0;
   out_4092507731700215880[305] = 0.0;
   out_4092507731700215880[306] = 0.0;
   out_4092507731700215880[307] = 0.0;
   out_4092507731700215880[308] = 0.0;
   out_4092507731700215880[309] = 0.0;
   out_4092507731700215880[310] = 0.0;
   out_4092507731700215880[311] = 0.0;
   out_4092507731700215880[312] = 0.0;
   out_4092507731700215880[313] = 0.0;
   out_4092507731700215880[314] = 0.0;
   out_4092507731700215880[315] = 0.0;
   out_4092507731700215880[316] = 0.0;
   out_4092507731700215880[317] = 0.0;
   out_4092507731700215880[318] = 0.0;
   out_4092507731700215880[319] = 0.0;
   out_4092507731700215880[320] = 0.0;
   out_4092507731700215880[321] = 0.0;
   out_4092507731700215880[322] = 0.0;
   out_4092507731700215880[323] = 1.0;
}
void f_fun(double *state, double dt, double *out_7088713391271841939) {
   out_7088713391271841939[0] = atan2((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), -(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]));
   out_7088713391271841939[1] = asin(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]));
   out_7088713391271841939[2] = atan2(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), -(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]));
   out_7088713391271841939[3] = dt*state[12] + state[3];
   out_7088713391271841939[4] = dt*state[13] + state[4];
   out_7088713391271841939[5] = dt*state[14] + state[5];
   out_7088713391271841939[6] = state[6];
   out_7088713391271841939[7] = state[7];
   out_7088713391271841939[8] = state[8];
   out_7088713391271841939[9] = state[9];
   out_7088713391271841939[10] = state[10];
   out_7088713391271841939[11] = state[11];
   out_7088713391271841939[12] = state[12];
   out_7088713391271841939[13] = state[13];
   out_7088713391271841939[14] = state[14];
   out_7088713391271841939[15] = state[15];
   out_7088713391271841939[16] = state[16];
   out_7088713391271841939[17] = state[17];
}
void F_fun(double *state, double dt, double *out_8358144676595975828) {
   out_8358144676595975828[0] = ((-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*cos(state[0])*cos(state[1]) - sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*cos(state[0])*cos(state[1]) - sin(dt*state[6])*sin(state[0])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_8358144676595975828[1] = ((-sin(dt*state[6])*sin(dt*state[8]) - sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*cos(state[1]) - (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*sin(state[1]) - sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(state[0]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*sin(state[1]) + (-sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) + sin(dt*state[8])*cos(dt*state[6]))*cos(state[1]) - sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(state[0]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_8358144676595975828[2] = 0;
   out_8358144676595975828[3] = 0;
   out_8358144676595975828[4] = 0;
   out_8358144676595975828[5] = 0;
   out_8358144676595975828[6] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(dt*cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) - dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_8358144676595975828[7] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*sin(dt*state[7])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[6])*sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) - dt*sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[7])*cos(dt*state[6])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[8])*sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]) - dt*sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_8358144676595975828[8] = ((dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((dt*sin(dt*state[6])*sin(dt*state[8]) + dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_8358144676595975828[9] = 0;
   out_8358144676595975828[10] = 0;
   out_8358144676595975828[11] = 0;
   out_8358144676595975828[12] = 0;
   out_8358144676595975828[13] = 0;
   out_8358144676595975828[14] = 0;
   out_8358144676595975828[15] = 0;
   out_8358144676595975828[16] = 0;
   out_8358144676595975828[17] = 0;
   out_8358144676595975828[18] = (-sin(dt*state[7])*sin(state[0])*cos(state[1]) - sin(dt*state[8])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_8358144676595975828[19] = (-sin(dt*state[7])*sin(state[1])*cos(state[0]) + sin(dt*state[8])*sin(state[0])*sin(state[1])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_8358144676595975828[20] = 0;
   out_8358144676595975828[21] = 0;
   out_8358144676595975828[22] = 0;
   out_8358144676595975828[23] = 0;
   out_8358144676595975828[24] = 0;
   out_8358144676595975828[25] = (dt*sin(dt*state[7])*sin(dt*state[8])*sin(state[0])*cos(state[1]) - dt*sin(dt*state[7])*sin(state[1])*cos(dt*state[8]) + dt*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_8358144676595975828[26] = (-dt*sin(dt*state[8])*sin(state[1])*cos(dt*state[7]) - dt*sin(state[0])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_8358144676595975828[27] = 0;
   out_8358144676595975828[28] = 0;
   out_8358144676595975828[29] = 0;
   out_8358144676595975828[30] = 0;
   out_8358144676595975828[31] = 0;
   out_8358144676595975828[32] = 0;
   out_8358144676595975828[33] = 0;
   out_8358144676595975828[34] = 0;
   out_8358144676595975828[35] = 0;
   out_8358144676595975828[36] = ((sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_8358144676595975828[37] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-sin(dt*state[7])*sin(state[2])*cos(state[0])*cos(state[1]) + sin(dt*state[8])*sin(state[0])*sin(state[2])*cos(dt*state[7])*cos(state[1]) - sin(state[1])*sin(state[2])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(-sin(dt*state[7])*cos(state[0])*cos(state[1])*cos(state[2]) + sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1])*cos(state[2]) - sin(state[1])*cos(dt*state[7])*cos(dt*state[8])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_8358144676595975828[38] = ((-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (-sin(state[0])*sin(state[1])*sin(state[2]) - cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_8358144676595975828[39] = 0;
   out_8358144676595975828[40] = 0;
   out_8358144676595975828[41] = 0;
   out_8358144676595975828[42] = 0;
   out_8358144676595975828[43] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(dt*(sin(state[0])*cos(state[2]) - sin(state[1])*sin(state[2])*cos(state[0]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*sin(state[2])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(dt*(-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_8358144676595975828[44] = (dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*sin(state[2])*cos(dt*state[7])*cos(state[1]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + (dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[7])*cos(state[1])*cos(state[2]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_8358144676595975828[45] = 0;
   out_8358144676595975828[46] = 0;
   out_8358144676595975828[47] = 0;
   out_8358144676595975828[48] = 0;
   out_8358144676595975828[49] = 0;
   out_8358144676595975828[50] = 0;
   out_8358144676595975828[51] = 0;
   out_8358144676595975828[52] = 0;
   out_8358144676595975828[53] = 0;
   out_8358144676595975828[54] = 0;
   out_8358144676595975828[55] = 0;
   out_8358144676595975828[56] = 0;
   out_8358144676595975828[57] = 1;
   out_8358144676595975828[58] = 0;
   out_8358144676595975828[59] = 0;
   out_8358144676595975828[60] = 0;
   out_8358144676595975828[61] = 0;
   out_8358144676595975828[62] = 0;
   out_8358144676595975828[63] = 0;
   out_8358144676595975828[64] = 0;
   out_8358144676595975828[65] = 0;
   out_8358144676595975828[66] = dt;
   out_8358144676595975828[67] = 0;
   out_8358144676595975828[68] = 0;
   out_8358144676595975828[69] = 0;
   out_8358144676595975828[70] = 0;
   out_8358144676595975828[71] = 0;
   out_8358144676595975828[72] = 0;
   out_8358144676595975828[73] = 0;
   out_8358144676595975828[74] = 0;
   out_8358144676595975828[75] = 0;
   out_8358144676595975828[76] = 1;
   out_8358144676595975828[77] = 0;
   out_8358144676595975828[78] = 0;
   out_8358144676595975828[79] = 0;
   out_8358144676595975828[80] = 0;
   out_8358144676595975828[81] = 0;
   out_8358144676595975828[82] = 0;
   out_8358144676595975828[83] = 0;
   out_8358144676595975828[84] = 0;
   out_8358144676595975828[85] = dt;
   out_8358144676595975828[86] = 0;
   out_8358144676595975828[87] = 0;
   out_8358144676595975828[88] = 0;
   out_8358144676595975828[89] = 0;
   out_8358144676595975828[90] = 0;
   out_8358144676595975828[91] = 0;
   out_8358144676595975828[92] = 0;
   out_8358144676595975828[93] = 0;
   out_8358144676595975828[94] = 0;
   out_8358144676595975828[95] = 1;
   out_8358144676595975828[96] = 0;
   out_8358144676595975828[97] = 0;
   out_8358144676595975828[98] = 0;
   out_8358144676595975828[99] = 0;
   out_8358144676595975828[100] = 0;
   out_8358144676595975828[101] = 0;
   out_8358144676595975828[102] = 0;
   out_8358144676595975828[103] = 0;
   out_8358144676595975828[104] = dt;
   out_8358144676595975828[105] = 0;
   out_8358144676595975828[106] = 0;
   out_8358144676595975828[107] = 0;
   out_8358144676595975828[108] = 0;
   out_8358144676595975828[109] = 0;
   out_8358144676595975828[110] = 0;
   out_8358144676595975828[111] = 0;
   out_8358144676595975828[112] = 0;
   out_8358144676595975828[113] = 0;
   out_8358144676595975828[114] = 1;
   out_8358144676595975828[115] = 0;
   out_8358144676595975828[116] = 0;
   out_8358144676595975828[117] = 0;
   out_8358144676595975828[118] = 0;
   out_8358144676595975828[119] = 0;
   out_8358144676595975828[120] = 0;
   out_8358144676595975828[121] = 0;
   out_8358144676595975828[122] = 0;
   out_8358144676595975828[123] = 0;
   out_8358144676595975828[124] = 0;
   out_8358144676595975828[125] = 0;
   out_8358144676595975828[126] = 0;
   out_8358144676595975828[127] = 0;
   out_8358144676595975828[128] = 0;
   out_8358144676595975828[129] = 0;
   out_8358144676595975828[130] = 0;
   out_8358144676595975828[131] = 0;
   out_8358144676595975828[132] = 0;
   out_8358144676595975828[133] = 1;
   out_8358144676595975828[134] = 0;
   out_8358144676595975828[135] = 0;
   out_8358144676595975828[136] = 0;
   out_8358144676595975828[137] = 0;
   out_8358144676595975828[138] = 0;
   out_8358144676595975828[139] = 0;
   out_8358144676595975828[140] = 0;
   out_8358144676595975828[141] = 0;
   out_8358144676595975828[142] = 0;
   out_8358144676595975828[143] = 0;
   out_8358144676595975828[144] = 0;
   out_8358144676595975828[145] = 0;
   out_8358144676595975828[146] = 0;
   out_8358144676595975828[147] = 0;
   out_8358144676595975828[148] = 0;
   out_8358144676595975828[149] = 0;
   out_8358144676595975828[150] = 0;
   out_8358144676595975828[151] = 0;
   out_8358144676595975828[152] = 1;
   out_8358144676595975828[153] = 0;
   out_8358144676595975828[154] = 0;
   out_8358144676595975828[155] = 0;
   out_8358144676595975828[156] = 0;
   out_8358144676595975828[157] = 0;
   out_8358144676595975828[158] = 0;
   out_8358144676595975828[159] = 0;
   out_8358144676595975828[160] = 0;
   out_8358144676595975828[161] = 0;
   out_8358144676595975828[162] = 0;
   out_8358144676595975828[163] = 0;
   out_8358144676595975828[164] = 0;
   out_8358144676595975828[165] = 0;
   out_8358144676595975828[166] = 0;
   out_8358144676595975828[167] = 0;
   out_8358144676595975828[168] = 0;
   out_8358144676595975828[169] = 0;
   out_8358144676595975828[170] = 0;
   out_8358144676595975828[171] = 1;
   out_8358144676595975828[172] = 0;
   out_8358144676595975828[173] = 0;
   out_8358144676595975828[174] = 0;
   out_8358144676595975828[175] = 0;
   out_8358144676595975828[176] = 0;
   out_8358144676595975828[177] = 0;
   out_8358144676595975828[178] = 0;
   out_8358144676595975828[179] = 0;
   out_8358144676595975828[180] = 0;
   out_8358144676595975828[181] = 0;
   out_8358144676595975828[182] = 0;
   out_8358144676595975828[183] = 0;
   out_8358144676595975828[184] = 0;
   out_8358144676595975828[185] = 0;
   out_8358144676595975828[186] = 0;
   out_8358144676595975828[187] = 0;
   out_8358144676595975828[188] = 0;
   out_8358144676595975828[189] = 0;
   out_8358144676595975828[190] = 1;
   out_8358144676595975828[191] = 0;
   out_8358144676595975828[192] = 0;
   out_8358144676595975828[193] = 0;
   out_8358144676595975828[194] = 0;
   out_8358144676595975828[195] = 0;
   out_8358144676595975828[196] = 0;
   out_8358144676595975828[197] = 0;
   out_8358144676595975828[198] = 0;
   out_8358144676595975828[199] = 0;
   out_8358144676595975828[200] = 0;
   out_8358144676595975828[201] = 0;
   out_8358144676595975828[202] = 0;
   out_8358144676595975828[203] = 0;
   out_8358144676595975828[204] = 0;
   out_8358144676595975828[205] = 0;
   out_8358144676595975828[206] = 0;
   out_8358144676595975828[207] = 0;
   out_8358144676595975828[208] = 0;
   out_8358144676595975828[209] = 1;
   out_8358144676595975828[210] = 0;
   out_8358144676595975828[211] = 0;
   out_8358144676595975828[212] = 0;
   out_8358144676595975828[213] = 0;
   out_8358144676595975828[214] = 0;
   out_8358144676595975828[215] = 0;
   out_8358144676595975828[216] = 0;
   out_8358144676595975828[217] = 0;
   out_8358144676595975828[218] = 0;
   out_8358144676595975828[219] = 0;
   out_8358144676595975828[220] = 0;
   out_8358144676595975828[221] = 0;
   out_8358144676595975828[222] = 0;
   out_8358144676595975828[223] = 0;
   out_8358144676595975828[224] = 0;
   out_8358144676595975828[225] = 0;
   out_8358144676595975828[226] = 0;
   out_8358144676595975828[227] = 0;
   out_8358144676595975828[228] = 1;
   out_8358144676595975828[229] = 0;
   out_8358144676595975828[230] = 0;
   out_8358144676595975828[231] = 0;
   out_8358144676595975828[232] = 0;
   out_8358144676595975828[233] = 0;
   out_8358144676595975828[234] = 0;
   out_8358144676595975828[235] = 0;
   out_8358144676595975828[236] = 0;
   out_8358144676595975828[237] = 0;
   out_8358144676595975828[238] = 0;
   out_8358144676595975828[239] = 0;
   out_8358144676595975828[240] = 0;
   out_8358144676595975828[241] = 0;
   out_8358144676595975828[242] = 0;
   out_8358144676595975828[243] = 0;
   out_8358144676595975828[244] = 0;
   out_8358144676595975828[245] = 0;
   out_8358144676595975828[246] = 0;
   out_8358144676595975828[247] = 1;
   out_8358144676595975828[248] = 0;
   out_8358144676595975828[249] = 0;
   out_8358144676595975828[250] = 0;
   out_8358144676595975828[251] = 0;
   out_8358144676595975828[252] = 0;
   out_8358144676595975828[253] = 0;
   out_8358144676595975828[254] = 0;
   out_8358144676595975828[255] = 0;
   out_8358144676595975828[256] = 0;
   out_8358144676595975828[257] = 0;
   out_8358144676595975828[258] = 0;
   out_8358144676595975828[259] = 0;
   out_8358144676595975828[260] = 0;
   out_8358144676595975828[261] = 0;
   out_8358144676595975828[262] = 0;
   out_8358144676595975828[263] = 0;
   out_8358144676595975828[264] = 0;
   out_8358144676595975828[265] = 0;
   out_8358144676595975828[266] = 1;
   out_8358144676595975828[267] = 0;
   out_8358144676595975828[268] = 0;
   out_8358144676595975828[269] = 0;
   out_8358144676595975828[270] = 0;
   out_8358144676595975828[271] = 0;
   out_8358144676595975828[272] = 0;
   out_8358144676595975828[273] = 0;
   out_8358144676595975828[274] = 0;
   out_8358144676595975828[275] = 0;
   out_8358144676595975828[276] = 0;
   out_8358144676595975828[277] = 0;
   out_8358144676595975828[278] = 0;
   out_8358144676595975828[279] = 0;
   out_8358144676595975828[280] = 0;
   out_8358144676595975828[281] = 0;
   out_8358144676595975828[282] = 0;
   out_8358144676595975828[283] = 0;
   out_8358144676595975828[284] = 0;
   out_8358144676595975828[285] = 1;
   out_8358144676595975828[286] = 0;
   out_8358144676595975828[287] = 0;
   out_8358144676595975828[288] = 0;
   out_8358144676595975828[289] = 0;
   out_8358144676595975828[290] = 0;
   out_8358144676595975828[291] = 0;
   out_8358144676595975828[292] = 0;
   out_8358144676595975828[293] = 0;
   out_8358144676595975828[294] = 0;
   out_8358144676595975828[295] = 0;
   out_8358144676595975828[296] = 0;
   out_8358144676595975828[297] = 0;
   out_8358144676595975828[298] = 0;
   out_8358144676595975828[299] = 0;
   out_8358144676595975828[300] = 0;
   out_8358144676595975828[301] = 0;
   out_8358144676595975828[302] = 0;
   out_8358144676595975828[303] = 0;
   out_8358144676595975828[304] = 1;
   out_8358144676595975828[305] = 0;
   out_8358144676595975828[306] = 0;
   out_8358144676595975828[307] = 0;
   out_8358144676595975828[308] = 0;
   out_8358144676595975828[309] = 0;
   out_8358144676595975828[310] = 0;
   out_8358144676595975828[311] = 0;
   out_8358144676595975828[312] = 0;
   out_8358144676595975828[313] = 0;
   out_8358144676595975828[314] = 0;
   out_8358144676595975828[315] = 0;
   out_8358144676595975828[316] = 0;
   out_8358144676595975828[317] = 0;
   out_8358144676595975828[318] = 0;
   out_8358144676595975828[319] = 0;
   out_8358144676595975828[320] = 0;
   out_8358144676595975828[321] = 0;
   out_8358144676595975828[322] = 0;
   out_8358144676595975828[323] = 1;
}
void h_4(double *state, double *unused, double *out_3125340361276393037) {
   out_3125340361276393037[0] = state[6] + state[9];
   out_3125340361276393037[1] = state[7] + state[10];
   out_3125340361276393037[2] = state[8] + state[11];
}
void H_4(double *state, double *unused, double *out_3457766192013903683) {
   out_3457766192013903683[0] = 0;
   out_3457766192013903683[1] = 0;
   out_3457766192013903683[2] = 0;
   out_3457766192013903683[3] = 0;
   out_3457766192013903683[4] = 0;
   out_3457766192013903683[5] = 0;
   out_3457766192013903683[6] = 1;
   out_3457766192013903683[7] = 0;
   out_3457766192013903683[8] = 0;
   out_3457766192013903683[9] = 1;
   out_3457766192013903683[10] = 0;
   out_3457766192013903683[11] = 0;
   out_3457766192013903683[12] = 0;
   out_3457766192013903683[13] = 0;
   out_3457766192013903683[14] = 0;
   out_3457766192013903683[15] = 0;
   out_3457766192013903683[16] = 0;
   out_3457766192013903683[17] = 0;
   out_3457766192013903683[18] = 0;
   out_3457766192013903683[19] = 0;
   out_3457766192013903683[20] = 0;
   out_3457766192013903683[21] = 0;
   out_3457766192013903683[22] = 0;
   out_3457766192013903683[23] = 0;
   out_3457766192013903683[24] = 0;
   out_3457766192013903683[25] = 1;
   out_3457766192013903683[26] = 0;
   out_3457766192013903683[27] = 0;
   out_3457766192013903683[28] = 1;
   out_3457766192013903683[29] = 0;
   out_3457766192013903683[30] = 0;
   out_3457766192013903683[31] = 0;
   out_3457766192013903683[32] = 0;
   out_3457766192013903683[33] = 0;
   out_3457766192013903683[34] = 0;
   out_3457766192013903683[35] = 0;
   out_3457766192013903683[36] = 0;
   out_3457766192013903683[37] = 0;
   out_3457766192013903683[38] = 0;
   out_3457766192013903683[39] = 0;
   out_3457766192013903683[40] = 0;
   out_3457766192013903683[41] = 0;
   out_3457766192013903683[42] = 0;
   out_3457766192013903683[43] = 0;
   out_3457766192013903683[44] = 1;
   out_3457766192013903683[45] = 0;
   out_3457766192013903683[46] = 0;
   out_3457766192013903683[47] = 1;
   out_3457766192013903683[48] = 0;
   out_3457766192013903683[49] = 0;
   out_3457766192013903683[50] = 0;
   out_3457766192013903683[51] = 0;
   out_3457766192013903683[52] = 0;
   out_3457766192013903683[53] = 0;
}
void h_10(double *state, double *unused, double *out_5094701453959070614) {
   out_5094701453959070614[0] = 9.8100000000000005*sin(state[1]) - state[4]*state[8] + state[5]*state[7] + state[12] + state[15];
   out_5094701453959070614[1] = -9.8100000000000005*sin(state[0])*cos(state[1]) + state[3]*state[8] - state[5]*state[6] + state[13] + state[16];
   out_5094701453959070614[2] = -9.8100000000000005*cos(state[0])*cos(state[1]) - state[3]*state[7] + state[4]*state[6] + state[14] + state[17];
}
void H_10(double *state, double *unused, double *out_7210162420988875382) {
   out_7210162420988875382[0] = 0;
   out_7210162420988875382[1] = 9.8100000000000005*cos(state[1]);
   out_7210162420988875382[2] = 0;
   out_7210162420988875382[3] = 0;
   out_7210162420988875382[4] = -state[8];
   out_7210162420988875382[5] = state[7];
   out_7210162420988875382[6] = 0;
   out_7210162420988875382[7] = state[5];
   out_7210162420988875382[8] = -state[4];
   out_7210162420988875382[9] = 0;
   out_7210162420988875382[10] = 0;
   out_7210162420988875382[11] = 0;
   out_7210162420988875382[12] = 1;
   out_7210162420988875382[13] = 0;
   out_7210162420988875382[14] = 0;
   out_7210162420988875382[15] = 1;
   out_7210162420988875382[16] = 0;
   out_7210162420988875382[17] = 0;
   out_7210162420988875382[18] = -9.8100000000000005*cos(state[0])*cos(state[1]);
   out_7210162420988875382[19] = 9.8100000000000005*sin(state[0])*sin(state[1]);
   out_7210162420988875382[20] = 0;
   out_7210162420988875382[21] = state[8];
   out_7210162420988875382[22] = 0;
   out_7210162420988875382[23] = -state[6];
   out_7210162420988875382[24] = -state[5];
   out_7210162420988875382[25] = 0;
   out_7210162420988875382[26] = state[3];
   out_7210162420988875382[27] = 0;
   out_7210162420988875382[28] = 0;
   out_7210162420988875382[29] = 0;
   out_7210162420988875382[30] = 0;
   out_7210162420988875382[31] = 1;
   out_7210162420988875382[32] = 0;
   out_7210162420988875382[33] = 0;
   out_7210162420988875382[34] = 1;
   out_7210162420988875382[35] = 0;
   out_7210162420988875382[36] = 9.8100000000000005*sin(state[0])*cos(state[1]);
   out_7210162420988875382[37] = 9.8100000000000005*sin(state[1])*cos(state[0]);
   out_7210162420988875382[38] = 0;
   out_7210162420988875382[39] = -state[7];
   out_7210162420988875382[40] = state[6];
   out_7210162420988875382[41] = 0;
   out_7210162420988875382[42] = state[4];
   out_7210162420988875382[43] = -state[3];
   out_7210162420988875382[44] = 0;
   out_7210162420988875382[45] = 0;
   out_7210162420988875382[46] = 0;
   out_7210162420988875382[47] = 0;
   out_7210162420988875382[48] = 0;
   out_7210162420988875382[49] = 0;
   out_7210162420988875382[50] = 1;
   out_7210162420988875382[51] = 0;
   out_7210162420988875382[52] = 0;
   out_7210162420988875382[53] = 1;
}
void h_13(double *state, double *unused, double *out_3814849203139753026) {
   out_3814849203139753026[0] = state[3];
   out_3814849203139753026[1] = state[4];
   out_3814849203139753026[2] = state[5];
}
void H_13(double *state, double *unused, double *out_4152865016302797246) {
   out_4152865016302797246[0] = 0;
   out_4152865016302797246[1] = 0;
   out_4152865016302797246[2] = 0;
   out_4152865016302797246[3] = 1;
   out_4152865016302797246[4] = 0;
   out_4152865016302797246[5] = 0;
   out_4152865016302797246[6] = 0;
   out_4152865016302797246[7] = 0;
   out_4152865016302797246[8] = 0;
   out_4152865016302797246[9] = 0;
   out_4152865016302797246[10] = 0;
   out_4152865016302797246[11] = 0;
   out_4152865016302797246[12] = 0;
   out_4152865016302797246[13] = 0;
   out_4152865016302797246[14] = 0;
   out_4152865016302797246[15] = 0;
   out_4152865016302797246[16] = 0;
   out_4152865016302797246[17] = 0;
   out_4152865016302797246[18] = 0;
   out_4152865016302797246[19] = 0;
   out_4152865016302797246[20] = 0;
   out_4152865016302797246[21] = 0;
   out_4152865016302797246[22] = 1;
   out_4152865016302797246[23] = 0;
   out_4152865016302797246[24] = 0;
   out_4152865016302797246[25] = 0;
   out_4152865016302797246[26] = 0;
   out_4152865016302797246[27] = 0;
   out_4152865016302797246[28] = 0;
   out_4152865016302797246[29] = 0;
   out_4152865016302797246[30] = 0;
   out_4152865016302797246[31] = 0;
   out_4152865016302797246[32] = 0;
   out_4152865016302797246[33] = 0;
   out_4152865016302797246[34] = 0;
   out_4152865016302797246[35] = 0;
   out_4152865016302797246[36] = 0;
   out_4152865016302797246[37] = 0;
   out_4152865016302797246[38] = 0;
   out_4152865016302797246[39] = 0;
   out_4152865016302797246[40] = 0;
   out_4152865016302797246[41] = 1;
   out_4152865016302797246[42] = 0;
   out_4152865016302797246[43] = 0;
   out_4152865016302797246[44] = 0;
   out_4152865016302797246[45] = 0;
   out_4152865016302797246[46] = 0;
   out_4152865016302797246[47] = 0;
   out_4152865016302797246[48] = 0;
   out_4152865016302797246[49] = 0;
   out_4152865016302797246[50] = 0;
   out_4152865016302797246[51] = 0;
   out_4152865016302797246[52] = 0;
   out_4152865016302797246[53] = 0;
}
void h_14(double *state, double *unused, double *out_9041680693720875183) {
   out_9041680693720875183[0] = state[6];
   out_9041680693720875183[1] = state[7];
   out_9041680693720875183[2] = state[8];
}
void H_14(double *state, double *unused, double *out_6540554624309275979) {
   out_6540554624309275979[0] = 0;
   out_6540554624309275979[1] = 0;
   out_6540554624309275979[2] = 0;
   out_6540554624309275979[3] = 0;
   out_6540554624309275979[4] = 0;
   out_6540554624309275979[5] = 0;
   out_6540554624309275979[6] = 1;
   out_6540554624309275979[7] = 0;
   out_6540554624309275979[8] = 0;
   out_6540554624309275979[9] = 0;
   out_6540554624309275979[10] = 0;
   out_6540554624309275979[11] = 0;
   out_6540554624309275979[12] = 0;
   out_6540554624309275979[13] = 0;
   out_6540554624309275979[14] = 0;
   out_6540554624309275979[15] = 0;
   out_6540554624309275979[16] = 0;
   out_6540554624309275979[17] = 0;
   out_6540554624309275979[18] = 0;
   out_6540554624309275979[19] = 0;
   out_6540554624309275979[20] = 0;
   out_6540554624309275979[21] = 0;
   out_6540554624309275979[22] = 0;
   out_6540554624309275979[23] = 0;
   out_6540554624309275979[24] = 0;
   out_6540554624309275979[25] = 1;
   out_6540554624309275979[26] = 0;
   out_6540554624309275979[27] = 0;
   out_6540554624309275979[28] = 0;
   out_6540554624309275979[29] = 0;
   out_6540554624309275979[30] = 0;
   out_6540554624309275979[31] = 0;
   out_6540554624309275979[32] = 0;
   out_6540554624309275979[33] = 0;
   out_6540554624309275979[34] = 0;
   out_6540554624309275979[35] = 0;
   out_6540554624309275979[36] = 0;
   out_6540554624309275979[37] = 0;
   out_6540554624309275979[38] = 0;
   out_6540554624309275979[39] = 0;
   out_6540554624309275979[40] = 0;
   out_6540554624309275979[41] = 0;
   out_6540554624309275979[42] = 0;
   out_6540554624309275979[43] = 0;
   out_6540554624309275979[44] = 1;
   out_6540554624309275979[45] = 0;
   out_6540554624309275979[46] = 0;
   out_6540554624309275979[47] = 0;
   out_6540554624309275979[48] = 0;
   out_6540554624309275979[49] = 0;
   out_6540554624309275979[50] = 0;
   out_6540554624309275979[51] = 0;
   out_6540554624309275979[52] = 0;
   out_6540554624309275979[53] = 0;
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
void pose_err_fun(double *nom_x, double *delta_x, double *out_1487504118503105920) {
  err_fun(nom_x, delta_x, out_1487504118503105920);
}
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_2323018677029669916) {
  inv_err_fun(nom_x, true_x, out_2323018677029669916);
}
void pose_H_mod_fun(double *state, double *out_4092507731700215880) {
  H_mod_fun(state, out_4092507731700215880);
}
void pose_f_fun(double *state, double dt, double *out_7088713391271841939) {
  f_fun(state,  dt, out_7088713391271841939);
}
void pose_F_fun(double *state, double dt, double *out_8358144676595975828) {
  F_fun(state,  dt, out_8358144676595975828);
}
void pose_h_4(double *state, double *unused, double *out_3125340361276393037) {
  h_4(state, unused, out_3125340361276393037);
}
void pose_H_4(double *state, double *unused, double *out_3457766192013903683) {
  H_4(state, unused, out_3457766192013903683);
}
void pose_h_10(double *state, double *unused, double *out_5094701453959070614) {
  h_10(state, unused, out_5094701453959070614);
}
void pose_H_10(double *state, double *unused, double *out_7210162420988875382) {
  H_10(state, unused, out_7210162420988875382);
}
void pose_h_13(double *state, double *unused, double *out_3814849203139753026) {
  h_13(state, unused, out_3814849203139753026);
}
void pose_H_13(double *state, double *unused, double *out_4152865016302797246) {
  H_13(state, unused, out_4152865016302797246);
}
void pose_h_14(double *state, double *unused, double *out_9041680693720875183) {
  h_14(state, unused, out_9041680693720875183);
}
void pose_H_14(double *state, double *unused, double *out_6540554624309275979) {
  H_14(state, unused, out_6540554624309275979);
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
