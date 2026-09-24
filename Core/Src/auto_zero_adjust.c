
#include "auto_zero_adjust.h"

double factor_value = 1.0;

void auto_zero_adjust(unn_std_var_typdef *opt_std_vars)
{
	float factor;
	float factor2=0, factor3=0;
	//float factor1=0, factor4=0;
	float avrg_factor = 0;
                   //(sys_info.curr_rgbc_vars.curr_red_rcv
	factor = (float)sys_info.curr_rgbc_vars.curr_red_rcv / (float)opt_std_vars->stan_0_red;
//	factor1 = factor;
	opt_std_vars->stan_0_red = sys_info.curr_rgbc_vars.curr_red_rcv;
	for (uint8_t i = 1; i < NOS_STD; i++)
	{
		opt_std_vars->strd_vars[i][0] =
			(uint16_t)(factor * (float)opt_std_vars->strd_vars[i][0]);

	}

	factor = (float)sys_info.curr_rgbc_vars.curr_green_rcv / (float)opt_std_vars->stan_0_green;
	factor2 = factor;
	opt_std_vars->stan_0_green = sys_info.curr_rgbc_vars.curr_green_rcv;
	for (uint8_t i = 1; i < NOS_STD; i++)
	{
		opt_std_vars->strd_vars[i][1] =
			(uint16_t)(factor * (float)opt_std_vars->strd_vars[i][1]);
	}

	factor = (float)sys_info.curr_rgbc_vars.curr_blue_rcv / (float)opt_std_vars->stan_0_blue;
	factor3 = factor;
	opt_std_vars->stan_0_blue = sys_info.curr_rgbc_vars.curr_blue_rcv;
	for (uint8_t i = 1; i < NOS_STD; i++)
	{
		opt_std_vars->strd_vars[i][2] =
			(uint16_t)(factor * (float)opt_std_vars->strd_vars[i][2]);
	}

	factor = (float)sys_info.curr_rgbc_vars.curr_clear_rcv / (float)opt_std_vars->stan_0_clear;
	//factor4 = factor;
	opt_std_vars->stan_0_clear = sys_info.curr_rgbc_vars.curr_clear_rcv;
	for (uint8_t i = 1; i < NOS_STD; i++)
	{
		opt_std_vars->strd_vars[i][3] =
			(uint16_t)(factor * (float)opt_std_vars->strd_vars[i][3]);
	}
//avrg_factor = (factor1 + factor2 + factor3 + factor4) / 4;
	avrg_factor = (factor2 + factor3) / 2;

factor_value = (avrg_factor);
	
}
