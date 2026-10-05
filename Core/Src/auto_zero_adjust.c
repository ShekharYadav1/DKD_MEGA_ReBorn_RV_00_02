
#include "auto_zero_adjust.h"

double factor_value = 1.0;
//static uint8_t auto_zero_adjust_flag = 0;

// void auto_zero_reset() // Reset the auto_zero_adjust_flag to 0 when second time came command from gui
// {
// 	auto_zero_adjust_flag = 0;
//
// 		for (uint8_t i = 0; i < NOS_STD; i++)
// 	{
// 		for (uint8_t j = 0; j < 4; j++)
// 		{
// 			sys_info.opt_std_vars.strd_vars[i][j] = save_sys_info.bk_var.hrd_std_vars.strd_vars[i][j];
// 			sys_info.opt_std_vars2.strd_vars[i][j] = save_sys_info.bk_var.hrd_std_vars2.strd_vars[i][j];
// 		}
// 	}
// }


void auto_zero_adjust(unn_std_var_typdef *opt_std_vars)
{
	float factor;
//	float factor2=0, factor3=0;
	float factor1=0, factor4=0;
	float avrg_factor = 0;

//    if(auto_zero_adjust_flag == 1)
// 	{
//     auto_zero_reset();
// 	}
//
	factor = (float)sys_info.curr_rgbc_vars.curr_red_rcv / (float)opt_std_vars->stan_0_red;
	factor1 = factor;
	opt_std_vars->stan_0_red = sys_info.curr_rgbc_vars.curr_red_rcv;
	for (uint8_t i = 1; i < NOS_STD; i++)
	{
		opt_std_vars->strd_vars[i][0] =
			(uint16_t)(factor * (float)opt_std_vars->strd_vars[i][0]);

	}

	factor = (float)sys_info.curr_rgbc_vars.curr_green_rcv / (float)opt_std_vars->stan_0_green;
//	factor2 = factor;
	opt_std_vars->stan_0_green = sys_info.curr_rgbc_vars.curr_green_rcv;
	for (uint8_t i = 1; i < NOS_STD; i++)
	{
		opt_std_vars->strd_vars[i][1] =
			(uint16_t)(factor * (float)opt_std_vars->strd_vars[i][1]);
	}

	factor = (float)sys_info.curr_rgbc_vars.curr_blue_rcv / (float)opt_std_vars->stan_0_blue;
//	factor3 = factor;
	opt_std_vars->stan_0_blue = sys_info.curr_rgbc_vars.curr_blue_rcv;
	for (uint8_t i = 1; i < NOS_STD; i++)
	{
		opt_std_vars->strd_vars[i][2] =
			(uint16_t)(factor * (float)opt_std_vars->strd_vars[i][2]);
	}

	factor = (float)sys_info.curr_rgbc_vars.curr_clear_rcv / (float)opt_std_vars->stan_0_clear;
	factor4 = factor;
	opt_std_vars->stan_0_clear = sys_info.curr_rgbc_vars.curr_clear_rcv;
	for (uint8_t i = 1; i < NOS_STD; i++)
	{
		opt_std_vars->strd_vars[i][3] =
			(uint16_t)(factor * (float)opt_std_vars->strd_vars[i][3]);
	}
//avrg_factor = (factor1 + factor2 + factor3 + factor4) / 4;
	avrg_factor = (factor1);

	factor_value = avrg_factor; // average factor for all four channels
//factor_value = factor1+factor2+factor3+factor4/4; // average factor for all four channels
//auto_zero_adjust_flag = 1;
	
}
