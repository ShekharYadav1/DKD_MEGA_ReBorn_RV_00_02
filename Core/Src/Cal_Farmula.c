
#include "Cal_Formula.h"
#include <math.h>

void PHOS_value_calculation2(unn_std_var_typdef *opt_std_vars)
{
	// sys_info.curr_rgbc_vars.curr_red_rcv = 9901;
	// sys_info.curr_rgbc_vars.curr_green_rcv = 10660;
	// sys_info.curr_rgbc_vars.curr_blue_rcv = 13163;
	// sys_info.curr_rgbc_vars.curr_clear_rcv = 3172;

		sys_info.curr_absrb_val = log10( ((double)opt_std_vars->strd_vars[0][sys_info.val_cal_y] * (double)sys_info.curr_rgbc_vars.curr_rgbc_var[sys_info.val_cal_x] )
				/ ((double)opt_std_vars->strd_vars[0][sys_info.val_cal_x] * (double)sys_info.curr_rgbc_vars.curr_rgbc_var[sys_info.val_cal_y]));

		for(uint8_t i = 0; i < NOS_STD; i++)
		{
			sys_info.std_absrb_val[i] = log10( ((double)opt_std_vars->strd_vars[0][sys_info.val_cal_y] * (double)opt_std_vars->strd_vars[i][sys_info.val_cal_x] )
				/ ((double)opt_std_vars->strd_vars[0][sys_info.val_cal_x] * (double)opt_std_vars->strd_vars[i][sys_info.val_cal_y]));
		}
		//log10( ($G$8 * J12)/($F$8 *K12))
	if(factor_value < 1)
		{
			sys_info.curr_absrb_val = (sys_info.curr_absrb_val *factor_value);
		}
	else if(factor_value > 1)
	{
		sys_info.curr_absrb_val = (sys_info.curr_absrb_val/factor_value);
	}

		if(sys_info.curr_absrb_val <= sys_info.std_absrb_val[0])
		{
			sys_info.curr_ResVal = 0;
			sys_info.curr_Result_cat = 1;
		}
		else if (sys_info.curr_absrb_val <= sys_info.std_absrb_val[1])
		{
			sys_info.curr_ResVal = ((sys_info.curr_absrb_val / sys_info.std_absrb_val[1])
				* sys_info.act_stan_vals[1]);
			sys_info.curr_Result_cat = 1;           //////////////VERRY LOW
		}
		else if (sys_info.curr_absrb_val <= sys_info.std_absrb_val[2])
		{
			sys_info.curr_ResVal = ( ( (sys_info.curr_absrb_val - sys_info.std_absrb_val[1]) / (sys_info.std_absrb_val[2] - sys_info.std_absrb_val[1]) )
				* (sys_info.act_stan_vals[2] - sys_info.act_stan_vals[1]) + sys_info.act_stan_vals[1]);
			sys_info.curr_Result_cat = 1;           //////////////VERRY LOW
		}
		else if (sys_info.curr_absrb_val <= sys_info.std_absrb_val[3])
		{
			sys_info.curr_ResVal = ( ( (sys_info.curr_absrb_val - sys_info.std_absrb_val[2]) / (sys_info.std_absrb_val[3] - sys_info.std_absrb_val[2]) )
				* (sys_info.act_stan_vals[3] - sys_info.act_stan_vals[2]) + sys_info.act_stan_vals[2]);
			sys_info.curr_Result_cat = 2;           //////////////LOW
		}
		else if (sys_info.curr_absrb_val <= sys_info.std_absrb_val[4])
		{
			sys_info.curr_ResVal = ( ( (sys_info.curr_absrb_val - sys_info.std_absrb_val[3]) / (sys_info.std_absrb_val[4] - sys_info.std_absrb_val[3]) )
				* (sys_info.act_stan_vals[4] - sys_info.act_stan_vals[3]) + sys_info.act_stan_vals[3]);
			sys_info.curr_Result_cat = 2;           //////////////LOW
		}
		else if (sys_info.curr_absrb_val <= sys_info.std_absrb_val[5])
		{
			sys_info.curr_ResVal = ( ( (sys_info.curr_absrb_val - sys_info.std_absrb_val[4]) / (sys_info.std_absrb_val[5] - sys_info.std_absrb_val[4]) )
				* (sys_info.act_stan_vals[5] - sys_info.act_stan_vals[4]) + sys_info.act_stan_vals[4]);
			sys_info.curr_Result_cat = 3;           //////////////Medium
		}
		else if (sys_info.curr_absrb_val <= sys_info.std_absrb_val[6])
		{
			sys_info.curr_ResVal = ( ( (sys_info.curr_absrb_val - sys_info.std_absrb_val[5]) / (sys_info.std_absrb_val[6] - sys_info.std_absrb_val[5]) )
				* (sys_info.act_stan_vals[6] - sys_info.act_stan_vals[5]) + sys_info.act_stan_vals[5]);
			sys_info.curr_Result_cat = 3;           //////////////Medium
		}
		else if (sys_info.curr_absrb_val <= sys_info.std_absrb_val[7])
		{
			sys_info.curr_ResVal = ( ( (sys_info.curr_absrb_val - sys_info.std_absrb_val[6]) / (sys_info.std_absrb_val[7] - sys_info.std_absrb_val[6]) )
				* (sys_info.act_stan_vals[7] - sys_info.act_stan_vals[6]) + sys_info.act_stan_vals[6]);
			sys_info.curr_Result_cat = 4;           //////////////High
		}
		else if (sys_info.curr_absrb_val <= sys_info.std_absrb_val[8])
		{
			sys_info.curr_ResVal = ( ( (sys_info.curr_absrb_val - sys_info.std_absrb_val[7]) / (sys_info.std_absrb_val[8] - sys_info.std_absrb_val[7]) )
				* (sys_info.act_stan_vals[8] - sys_info.act_stan_vals[7]) + sys_info.act_stan_vals[7]);
			sys_info.curr_Result_cat = 4;           //////////////High
		}
		else if (sys_info.curr_absrb_val <= sys_info.std_absrb_val[9])
		{
			sys_info.curr_ResVal = ( ( (sys_info.curr_absrb_val - sys_info.std_absrb_val[8]) / (sys_info.std_absrb_val[9] - sys_info.std_absrb_val[8]) )
				* (sys_info.act_stan_vals[9] - sys_info.act_stan_vals[8]) + sys_info.act_stan_vals[8]);
			sys_info.curr_Result_cat = 5;           //////////////Verry High
		}
		else if (sys_info.curr_absrb_val <= sys_info.std_absrb_val[10])
		{
			sys_info.curr_ResVal = ( ( (sys_info.curr_absrb_val - sys_info.std_absrb_val[9]) / (sys_info.std_absrb_val[10] - sys_info.std_absrb_val[9]) )
				* (sys_info.act_stan_vals[10] - sys_info.act_stan_vals[9]) + sys_info.act_stan_vals[9]);
			sys_info.curr_Result_cat = 5;           //////////////Verry High
		}
		else if (sys_info.curr_absrb_val > sys_info.std_absrb_val[10])
		{
			sys_info.curr_ResVal = (sys_info.act_stan_vals[10]/sys_info.std_absrb_val[10])* (sys_info.curr_absrb_val);
			
			if(sys_info.curr_ResVal > sys_info.act_stan_vals[10])
			 {
				 sys_info.curr_ResVal = sys_info.act_stan_vals[10];
			 }
			sys_info.curr_Result_cat = 5;           //////////////Verry High
			//sys_info.curr_ResVal = sys_info.act_stan_vals[10];
			//sys_info.curr_Result_cat = 5;           //////////////Verry High
		}

}

void SUL_value_calculation1(unn_std_var_typdef *opt_std_vars)
{
	// uint32_t sum_x, sum_y, sum_xy, sum_xx;
	sys_info.sum_x = 0;
	sys_info.sum_y = 0;
	sys_info.sum_xy = 0;
	sys_info.sum_xx = 0;
	uint8_t i;
	for (i = 0; i < NOS_STD; i++)
	{
		sys_info.sum_x += opt_std_vars->strd_vars[i][sys_info.val_cal_x];
		sys_info.sum_y += opt_std_vars->strd_vars[i][sys_info.val_cal_y];
		sys_info.sum_xy += opt_std_vars->strd_vars[i][sys_info.val_cal_x] * opt_std_vars->strd_vars[i][sys_info.val_cal_y];
		sys_info.sum_xx += opt_std_vars->strd_vars[i][sys_info.val_cal_x] * opt_std_vars->strd_vars[i][sys_info.val_cal_x];
	}
	sys_info.mslp = (((double)NOS_STD * (double)sys_info.sum_xy) - ((double)sys_info.sum_x * (double)sys_info.sum_y)) / (((double)NOS_STD * (double)sys_info.sum_xx) - ((double)sys_info.sum_x * (double)sys_info.sum_x));

	sys_info.cshft = ((double)sys_info.sum_y - (sys_info.mslp * (double)sys_info.sum_x)) / NOS_STD;

	for (uint8_t i = 0; i < NOS_STD; i++)
	{
		sys_info.ref_stan_x[i] = ((sys_info.mslp * ((double)opt_std_vars->strd_vars[i][sys_info.val_cal_y] - sys_info.cshft)) + (double)opt_std_vars->strd_vars[i][sys_info.val_cal_x]) / ((sys_info.mslp * sys_info.mslp) + 1);
		sys_info.ref_stan_y[i] = (sys_info.mslp * sys_info.ref_stan_x[i]) + sys_info.cshft;
		sys_info.ref_stan_dist[i] = sqrt(((sys_info.ref_stan_x[0] - sys_info.ref_stan_x[i]) * (sys_info.ref_stan_x[0] - sys_info.ref_stan_x[i])) + ((sys_info.ref_stan_y[0] - sys_info.ref_stan_y[i]) * (sys_info.ref_stan_y[0] - sys_info.ref_stan_y[i])));
	}

	// sys_info.curr_red_rcv = 16174;
	// sys_info.curr_green_rcv = 12952;
	// sys_info.curr_blue_rcv = 14442;
	// sys_info.curr_clear_rcv = 45558;
	sys_info.curr_x = ((sys_info.mslp * ((double)sys_info.curr_rgbc_vars.curr_rgbc_var[sys_info.val_cal_y] - sys_info.cshft)) + (double)sys_info.curr_rgbc_vars.curr_rgbc_var[sys_info.val_cal_x]) / ((sys_info.mslp * sys_info.mslp) + 1);
	sys_info.curr_y = (sys_info.mslp * sys_info.curr_x) + sys_info.cshft;
	sys_info.curr_dist = sqrt(((sys_info.ref_stan_x[0] - sys_info.curr_x) * (sys_info.ref_stan_x[0] - sys_info.curr_x)) + ((sys_info.ref_stan_y[0] - sys_info.curr_y) * (sys_info.ref_stan_y[0] - sys_info.curr_y)));

	if (sys_info.curr_dist <= sys_info.ref_stan_dist[0])
	{
		sys_info.curr_ResVal = 0;
		sys_info.curr_Result_cat = 1;
	}
	else if (sys_info.curr_dist <= sys_info.ref_stan_dist[1])
	{
		sys_info.curr_ResVal = ((sys_info.curr_dist / sys_info.ref_stan_dist[1]) * sys_info.act_stan_vals[1]);
		sys_info.curr_Result_cat = 1; //////////////VERRY LOW
	}
	else if (sys_info.curr_dist <= sys_info.ref_stan_dist[2])
	{
		sys_info.curr_ResVal = (((sys_info.curr_dist - sys_info.ref_stan_dist[1]) / (sys_info.ref_stan_dist[2] - sys_info.ref_stan_dist[1])) * (sys_info.act_stan_vals[2] - sys_info.act_stan_vals[1]) + sys_info.act_stan_vals[1]);
		sys_info.curr_Result_cat = 1; //////////////VERRY LOW
	}
	else if (sys_info.curr_dist <= sys_info.ref_stan_dist[3])
	{
		sys_info.curr_ResVal = (((sys_info.curr_dist - sys_info.ref_stan_dist[2]) / (sys_info.ref_stan_dist[3] - sys_info.ref_stan_dist[2])) * (sys_info.act_stan_vals[3] - sys_info.act_stan_vals[2]) + sys_info.act_stan_vals[2]);
		sys_info.curr_Result_cat = 2; //////////////LOW
	}
	else if (sys_info.curr_dist <= sys_info.ref_stan_dist[4])
	{
		sys_info.curr_ResVal = (((sys_info.curr_dist - sys_info.ref_stan_dist[3]) / (sys_info.ref_stan_dist[4] - sys_info.ref_stan_dist[3])) * (sys_info.act_stan_vals[4] - sys_info.act_stan_vals[3]) + sys_info.act_stan_vals[3]);
		sys_info.curr_Result_cat = 2; //////////////LOW
	}
	else if (sys_info.curr_dist <= sys_info.ref_stan_dist[5])
	{
		sys_info.curr_ResVal = (((sys_info.curr_dist - sys_info.ref_stan_dist[4]) / (sys_info.ref_stan_dist[5] - sys_info.ref_stan_dist[4])) * (sys_info.act_stan_vals[5] - sys_info.act_stan_vals[4]) + sys_info.act_stan_vals[4]);
		sys_info.curr_Result_cat = 3; //////////////Medium
	}
	else if (sys_info.curr_dist <= sys_info.ref_stan_dist[6])
	{
		sys_info.curr_ResVal = (((sys_info.curr_dist - sys_info.ref_stan_dist[5]) / (sys_info.ref_stan_dist[6] - sys_info.ref_stan_dist[5])) * (sys_info.act_stan_vals[6] - sys_info.act_stan_vals[5]) + sys_info.act_stan_vals[5]);
		sys_info.curr_Result_cat = 3; //////////////Medium
	}
	else if (sys_info.curr_dist <= sys_info.ref_stan_dist[7])
	{
		sys_info.curr_ResVal = (((sys_info.curr_dist - sys_info.ref_stan_dist[6]) / (sys_info.ref_stan_dist[7] - sys_info.ref_stan_dist[6])) * (sys_info.act_stan_vals[7] - sys_info.act_stan_vals[6]) + sys_info.act_stan_vals[6]);
		sys_info.curr_Result_cat = 4; //////////////High
	}
	else if (sys_info.curr_dist <= sys_info.ref_stan_dist[8])
	{
		sys_info.curr_ResVal = (((sys_info.curr_dist - sys_info.ref_stan_dist[7]) / (sys_info.ref_stan_dist[8] - sys_info.ref_stan_dist[7])) * (sys_info.act_stan_vals[8] - sys_info.act_stan_vals[7]) + sys_info.act_stan_vals[7]);
		sys_info.curr_Result_cat = 4; //////////////High
	}
	else if (sys_info.curr_dist <= sys_info.ref_stan_dist[9])
	{
		sys_info.curr_ResVal = (((sys_info.curr_dist - sys_info.ref_stan_dist[8]) / (sys_info.ref_stan_dist[9] - sys_info.ref_stan_dist[8])) * (sys_info.act_stan_vals[9] - sys_info.act_stan_vals[8]) + sys_info.act_stan_vals[8]);
		sys_info.curr_Result_cat = 5; //////////////Verry High
	}
	else if (sys_info.curr_dist <= sys_info.ref_stan_dist[10])
	{
		sys_info.curr_ResVal = (((sys_info.curr_dist - sys_info.ref_stan_dist[9]) / (sys_info.ref_stan_dist[10] - sys_info.ref_stan_dist[9])) * (sys_info.act_stan_vals[10] - sys_info.act_stan_vals[9]) + sys_info.act_stan_vals[9]);
		sys_info.curr_Result_cat = 5; //////////////Verry High
	}
	else if (sys_info.curr_dist > sys_info.ref_stan_dist[10])
	{
		sys_info.curr_ResVal = ((sys_info.act_stan_vals[10] / sys_info.ref_stan_dist[10]) * (sys_info.curr_dist - sys_info.ref_stan_dist[10])) + sys_info.act_stan_vals[10];
		sys_info.curr_Result_cat = 5; //////////////Verry High
									  // sys_info.curr_ResVal = sys_info.act_stan_vals[10];
		// sys_info.curr_Result_cat = 5;           //////////////Verry High
	}
}

void NIT_interpolation_calculation3(unn_std_var_typdef *opt_std_vars){
	double absrb1, absrb2;

	for (uint8_t i = 0; i < NOS_STD; i++)
	{
		absrb1 = log10((double)opt_std_vars->stan_0_green / (double)opt_std_vars->strd_vars[i][1]);
		absrb2 = log10((double)opt_std_vars->stan_0_blue  / (double)opt_std_vars->strd_vars[i][2]);

		sys_info.std_absrb_val[i] = (absrb1 + absrb2) / 2.00;
	}

	// Step 2: Current sample ka absorbance (same formula, consistent scale)
	absrb1 = log10((double)opt_std_vars->stan_0_green / (double)sys_info.curr_rgbc_vars.curr_green_rcv);
	absrb2 = log10((double)opt_std_vars->stan_0_blue  / (double)sys_info.curr_rgbc_vars.curr_blue_rcv);

	sys_info.curr_absrb_val = (absrb1 + absrb2) / 2.00;

//	// Step 3: factor correction
	if (factor_value < 1)
	{
		sys_info.curr_absrb_val = ((sys_info.curr_absrb_val / factor_value))/2;
	}
	else if (factor_value > 1)
	{
		sys_info.curr_absrb_val = ((sys_info.curr_absrb_val * factor_value))/2;
	}

	// Step 4: interpolation (aapka original explicit ladder, as-is)
	if (sys_info.curr_absrb_val <= sys_info.std_absrb_val[0])
	{
		sys_info.curr_ResVal = 10;
		sys_info.curr_Result_cat = 1;
	}
	else if (sys_info.curr_absrb_val <= sys_info.std_absrb_val[1])
	{
		sys_info.curr_ResVal = ((sys_info.curr_absrb_val / sys_info.std_absrb_val[1])
			* sys_info.act_stan_vals[1]);
		sys_info.curr_Result_cat = 1;           //////////////VERRY LOW
	}
	else if (sys_info.curr_absrb_val <= sys_info.std_absrb_val[2])
	{
		sys_info.curr_ResVal = ( ( (sys_info.curr_absrb_val - sys_info.std_absrb_val[1]) / (sys_info.std_absrb_val[2] - sys_info.std_absrb_val[1]) )
			* (sys_info.act_stan_vals[2] - sys_info.act_stan_vals[1]) + sys_info.act_stan_vals[1]);
		sys_info.curr_Result_cat = 1;           //////////////VERRY LOW
	}
	else if (sys_info.curr_absrb_val <= sys_info.std_absrb_val[3])
	{
		sys_info.curr_ResVal = ( ( (sys_info.curr_absrb_val - sys_info.std_absrb_val[2]) / (sys_info.std_absrb_val[3] - sys_info.std_absrb_val[2]) )
			* (sys_info.act_stan_vals[3] - sys_info.act_stan_vals[2]) + sys_info.act_stan_vals[2]);
		sys_info.curr_Result_cat = 2;           //////////////LOW
	}
	else if (sys_info.curr_absrb_val <= sys_info.std_absrb_val[4])
	{
		sys_info.curr_ResVal = ( ( (sys_info.curr_absrb_val - sys_info.std_absrb_val[3]) / (sys_info.std_absrb_val[4] - sys_info.std_absrb_val[3]) )
			* (sys_info.act_stan_vals[4] - sys_info.act_stan_vals[3]) + sys_info.act_stan_vals[3]);
		sys_info.curr_Result_cat = 2;           //////////////LOW
	}
	else if (sys_info.curr_absrb_val <= sys_info.std_absrb_val[5])
	{
		sys_info.curr_ResVal = ( ( (sys_info.curr_absrb_val - sys_info.std_absrb_val[4]) / (sys_info.std_absrb_val[5] - sys_info.std_absrb_val[4]) )
			* (sys_info.act_stan_vals[5] - sys_info.act_stan_vals[4]) + sys_info.act_stan_vals[4]);
		sys_info.curr_Result_cat = 3;           //////////////Medium
	}
	else if (sys_info.curr_absrb_val <= sys_info.std_absrb_val[6])
	{
		sys_info.curr_ResVal = ( ( (sys_info.curr_absrb_val - sys_info.std_absrb_val[5]) / (sys_info.std_absrb_val[6] - sys_info.std_absrb_val[5]) )
			* (sys_info.act_stan_vals[6] - sys_info.act_stan_vals[5]) + sys_info.act_stan_vals[5]);
		sys_info.curr_Result_cat = 3;           //////////////Medium
	}
	else if (sys_info.curr_absrb_val <= sys_info.std_absrb_val[7])
	{
		sys_info.curr_ResVal = ( ( (sys_info.curr_absrb_val - sys_info.std_absrb_val[6]) / (sys_info.std_absrb_val[7] - sys_info.std_absrb_val[6]) )
			* (sys_info.act_stan_vals[7] - sys_info.act_stan_vals[6]) + sys_info.act_stan_vals[6]);
		sys_info.curr_Result_cat = 4;           //////////////High
	}
	else if (sys_info.curr_absrb_val <= sys_info.std_absrb_val[8])
	{
		sys_info.curr_ResVal = ( ( (sys_info.curr_absrb_val - sys_info.std_absrb_val[7]) / (sys_info.std_absrb_val[8] - sys_info.std_absrb_val[7]) )
			* (sys_info.act_stan_vals[8] - sys_info.act_stan_vals[7]) + sys_info.act_stan_vals[7]);
		sys_info.curr_Result_cat = 4;           //////////////High
	}
	else if (sys_info.curr_absrb_val <= sys_info.std_absrb_val[9])
	{
		sys_info.curr_ResVal = ( ( (sys_info.curr_absrb_val - sys_info.std_absrb_val[8]) / (sys_info.std_absrb_val[9] - sys_info.std_absrb_val[8]) )
			* (sys_info.act_stan_vals[9] - sys_info.act_stan_vals[8]) + sys_info.act_stan_vals[8]);
		sys_info.curr_Result_cat = 5;           //////////////Verry High
	}
	else if (sys_info.curr_absrb_val <= sys_info.std_absrb_val[10])
	{
		sys_info.curr_ResVal = ( ( (sys_info.curr_absrb_val - sys_info.std_absrb_val[9]) / (sys_info.std_absrb_val[10] - sys_info.std_absrb_val[9]) )
			* (sys_info.act_stan_vals[10] - sys_info.act_stan_vals[9]) + sys_info.act_stan_vals[9]);
		sys_info.curr_Result_cat = 5;           //////////////Verry High
	}
	else // sys_info.curr_absrb_val > sys_info.std_absrb_val[10]
	{
		sys_info.curr_ResVal = (sys_info.act_stan_vals[10] / sys_info.std_absrb_val[10]) * (sys_info.curr_absrb_val);

		if (sys_info.curr_ResVal > sys_info.act_stan_vals[10])
		{
			sys_info.curr_ResVal = sys_info.act_stan_vals[10];
		}
		sys_info.curr_Result_cat = 5;           //////////////Verry High
	}

}

void NIT_value_calculation3(unn_std_var_typdef *opt_std_vars)
{
	double  absrb1, absrb2,avg_absrb;
	double std_absrb_val_mul_sum, std_absrb_sqr_sum;
	std_absrb_val_mul_sum = 0;
	std_absrb_sqr_sum = 0;
	sys_info.std_multplr = 0;
	for (uint8_t i = 0; i < NOS_STD; i++)
	{
		absrb1 = log10((double)opt_std_vars->stan_0_green / (double)opt_std_vars->strd_vars[i][1]);
		absrb2 = log10((double)opt_std_vars->stan_0_blue / (double)opt_std_vars->strd_vars[i][2]);
	
		 avg_absrb = (absrb1 + absrb2) / 2.00;

		std_absrb_val_mul_sum = std_absrb_val_mul_sum + (avg_absrb * (double)sys_info.act_stan_vals[i]);
		std_absrb_sqr_sum = std_absrb_sqr_sum + (avg_absrb * avg_absrb);
	}
	sys_info.std_multplr = (std_absrb_val_mul_sum / std_absrb_sqr_sum); // constant factor

	absrb1 = log10((double)opt_std_vars->stan_0_green / (double)sys_info.curr_rgbc_vars.curr_green_rcv);
	absrb2 = log10((double)opt_std_vars->stan_0_blue / (double)sys_info.curr_rgbc_vars.curr_blue_rcv);

	 avg_absrb = (absrb1 + absrb2) / 2.00;

	sys_info.curr_ResVal = (avg_absrb * sys_info.std_multplr)/2;

	if (sys_info.curr_ResVal <= sys_info.act_stan_vals[0])
	{
		sys_info.curr_ResVal = sys_info.act_stan_vals[0];
		sys_info.curr_Result_cat = 1;
	}
	else if (sys_info.curr_ResVal <= sys_info.act_stan_vals[1])
	{
		sys_info.curr_Result_cat = 1; //////////////VERRY LOW
	}
	else if (sys_info.curr_ResVal <= sys_info.act_stan_vals[2])
	{
		sys_info.curr_Result_cat = 1; //////////////VERRY LOW
	}
	else if (sys_info.curr_ResVal <= sys_info.act_stan_vals[3])
	{
		sys_info.curr_Result_cat = 2; //////////////LOW
	}
	else if (sys_info.curr_ResVal <= sys_info.act_stan_vals[4])
	{
		sys_info.curr_Result_cat = 2; //////////////LOW
	}
	else if (sys_info.curr_ResVal <= sys_info.act_stan_vals[5])
	{
		sys_info.curr_Result_cat = 3; //////////////Medium
	}
	else if (sys_info.curr_ResVal <= sys_info.act_stan_vals[6])
	{
		sys_info.curr_Result_cat = 3; //////////////Medium
	}
	else if (sys_info.curr_ResVal <= sys_info.act_stan_vals[7])
	{
		sys_info.curr_Result_cat = 4; //////////////High
	}
	else if (sys_info.curr_ResVal <= sys_info.act_stan_vals[8])
	{
		sys_info.curr_Result_cat = 4; //////////////High
	}
	else if (sys_info.curr_ResVal <= sys_info.act_stan_vals[9])
	{
		sys_info.curr_Result_cat = 5; //////////////Verry High
	}
	else if (sys_info.curr_ResVal <= sys_info.act_stan_vals[10])
	{
		sys_info.curr_Result_cat = 5; //////////////Verry High
	}
	else if (sys_info.curr_ResVal > sys_info.act_stan_vals[10])
	{
		sys_info.curr_Result_cat = 5; //////////////Verry High
									  // sys_info.curr_ResVal = sys_info.act_stan_vals[10];
		// sys_info.curr_Result_cat = 5;           //////////////Verry High
	}


}

void POT_value_calculation5(unn_std_var_typdef *opt_std_vars)
{

	double absrb0, absrb1, absrb2, absrb3, avg_absrb;
	double std_absrb_val_mul_sum, std_absrb_sqr_sum;
	std_absrb_val_mul_sum = 0;
	std_absrb_sqr_sum = 0;
	sys_info.std_multplr = 0;
	for (uint8_t i = 0; i < NOS_STD; i++)
	{ // opt_std_vars->stan_0_red
		absrb0 = log10((double)opt_std_vars->stan_0_red / (double)opt_std_vars->strd_vars[i][0]);
		absrb1 = log10((double)opt_std_vars->stan_0_green / (double)opt_std_vars->strd_vars[i][1]);
		absrb2 = log10((double)opt_std_vars->stan_0_blue / (double)opt_std_vars->strd_vars[i][2]);
		absrb3 = log10((double)opt_std_vars->stan_0_clear / (double)opt_std_vars->strd_vars[i][3]);
		avg_absrb = (absrb0 + absrb1 + absrb2 + absrb3) / 4.00;
		// sys_info.std_absrb_val_mul[i] = (avg_absrb * (double)sys_info.act_stan_vals[i]);
		// sys_info.std_absrb_sqr[i] = (avg_absrb * avg_absrb);
		std_absrb_val_mul_sum = std_absrb_val_mul_sum + (avg_absrb * (double)sys_info.act_stan_vals[i]);
		std_absrb_sqr_sum = std_absrb_sqr_sum + (avg_absrb * avg_absrb);
	}
	sys_info.std_multplr = ((std_absrb_val_mul_sum / std_absrb_sqr_sum)); // constant factor

	absrb0 = log10((double)opt_std_vars->stan_0_red / (double)sys_info.curr_rgbc_vars.curr_red_rcv);
	absrb1 = log10((double)opt_std_vars->stan_0_green / (double)sys_info.curr_rgbc_vars.curr_green_rcv);
	absrb2 = log10((double)opt_std_vars->stan_0_blue / (double)sys_info.curr_rgbc_vars.curr_blue_rcv);
	absrb3 = log10((double)opt_std_vars->stan_0_clear / (double)sys_info.curr_rgbc_vars.curr_clear_rcv);
	avg_absrb = (absrb0 + absrb1 + absrb2 + absrb3) / 4;

	if (factor_value > 1)
	{
		sys_info.curr_ResVal = ((avg_absrb * sys_info.std_multplr) * factor_value);
	}
	else if (factor_value == 1)
	{
		sys_info.curr_ResVal = ((avg_absrb * sys_info.std_multplr)) - factor_value;
	}
	else if (factor_value < 1)
	{
		sys_info.curr_ResVal = ((avg_absrb * sys_info.std_multplr) * factor_value);
	}

	if (sys_info.curr_ResVal <= sys_info.act_stan_vals[0])
	{
		sys_info.curr_ResVal = sys_info.act_stan_vals[0];
		sys_info.curr_Result_cat = 1;
	}

	else if (sys_info.curr_ResVal <= sys_info.act_stan_vals[1])
	{
		sys_info.curr_Result_cat = 1; //////////////VERRY LOW
	}
	else if (sys_info.curr_ResVal <= sys_info.act_stan_vals[2])
	{
		sys_info.curr_Result_cat = 1; //////////////VERRY LOW
	}
	else if (sys_info.curr_ResVal <= sys_info.act_stan_vals[3])
	{
		sys_info.curr_Result_cat = 2; //////////////LOW
	}
	else if (sys_info.curr_ResVal <= sys_info.act_stan_vals[4])
	{
		sys_info.curr_Result_cat = 2; //////////////LOW
	}
	else if (sys_info.curr_ResVal <= sys_info.act_stan_vals[5])
	{
		sys_info.curr_Result_cat = 3; //////////////Medium
	}
	else if (sys_info.curr_ResVal <= sys_info.act_stan_vals[6])
	{
		sys_info.curr_Result_cat = 3; //////////////Medium
	}
	else if (sys_info.curr_ResVal <= sys_info.act_stan_vals[7])
	{
		sys_info.curr_Result_cat = 4; //////////////High
	}
	else if (sys_info.curr_ResVal <= sys_info.act_stan_vals[8])
	{
		sys_info.curr_Result_cat = 4; //////////////High
	}
	else if (sys_info.curr_ResVal <= sys_info.act_stan_vals[9])
	{
		sys_info.curr_Result_cat = 5; //////////////Verry High
	}
	else if (sys_info.curr_ResVal <= sys_info.act_stan_vals[10])
	{
		sys_info.curr_Result_cat = 5; //////////////Verry High
	}
	else if (sys_info.curr_ResVal > sys_info.act_stan_vals[10])
	{
		sys_info.curr_Result_cat = 5; //////////////Verry High
									  // sys_info.curr_ResVal = sys_info.act_stan_vals[10];
									  // sys_info.curr_Result_cat = 5;           //////////////Verry High
	}
}
void OC_value_calculation6(unn_std_var_typdef *opt_std_vars)
{

double  absrb0=0 ,absrb3=0;

	for (uint8_t i = 0; i < NOS_STD; i++)
	{
		  absrb0 = log10((double)opt_std_vars->stan_0_red / (double)opt_std_vars->strd_vars[i][0]);
//          absrb3 = log10((double)opt_std_vars->stan_0_clear / (double)opt_std_vars->strd_vars[i][3]);
		sys_info.std_absrb_val[i] = absrb0;
	}

	// Step 2: Current sample ka absorbance (same formula, consistent scale)
	   absrb0 = log10((double)opt_std_vars->stan_0_red / (double)sys_info.curr_rgbc_vars.curr_red_rcv);
//	   absrb3 = log10((double)opt_std_vars->stan_0_clear / (double)sys_info.curr_rgbc_vars.curr_clear_rcv);

	sys_info.curr_absrb_val = absrb0;

//	// Step 3: factor correction
	//   if (factor_value < 1)
	//   {
	//   	sys_info.curr_absrb_val = ((sys_info.curr_absrb_val / factor_value));
	//   }
	//   else if (factor_value > 1)
	//   {
	//   	sys_info.curr_absrb_val = ((sys_info.curr_absrb_val / factor_value));
	//   }

	if (sys_info.curr_absrb_val <= sys_info.std_absrb_val[0])
	{
		sys_info.curr_ResVal = 0;
		sys_info.curr_Result_cat = 1;
	}                                  //
	else if (sys_info.curr_absrb_val <= sys_info.std_absrb_val[1])
	{                                                              
		sys_info.curr_ResVal = ((sys_info.curr_absrb_val / sys_info.std_absrb_val[1])
			* sys_info.act_stan_vals[1]);
		// sys_info.curr_ResVal = (sys_info.curr_ResVal/factor_value);
		sys_info.curr_Result_cat = 1;           //////////////VERRY LOW
	}
	else if (sys_info.curr_absrb_val <= sys_info.std_absrb_val[2])
	{
		sys_info.curr_ResVal = ( ( (sys_info.curr_absrb_val - sys_info.std_absrb_val[1]) / (sys_info.std_absrb_val[2] - sys_info.std_absrb_val[1]) )
			* (sys_info.act_stan_vals[2] - sys_info.act_stan_vals[1]) + sys_info.act_stan_vals[1]);
		sys_info.curr_Result_cat = 1;           //////////////VERRY LOW
	}
	else if (sys_info.curr_absrb_val <= sys_info.std_absrb_val[3])
	{
		sys_info.curr_ResVal = ( ( (sys_info.curr_absrb_val - sys_info.std_absrb_val[2]) / (sys_info.std_absrb_val[3] - sys_info.std_absrb_val[2]) )
			* (sys_info.act_stan_vals[3] - sys_info.act_stan_vals[2]) + sys_info.act_stan_vals[2]);
		sys_info.curr_Result_cat = 2;           //////////////LOW
	}
	else if (sys_info.curr_absrb_val <= sys_info.std_absrb_val[4]) 
	{
		sys_info.curr_ResVal = ( ( (sys_info.curr_absrb_val - sys_info.std_absrb_val[3]) / (sys_info.std_absrb_val[4] - sys_info.std_absrb_val[3]) )
			* (sys_info.act_stan_vals[4] - sys_info.act_stan_vals[3]) + sys_info.act_stan_vals[3]);
		sys_info.curr_Result_cat = 2;           //////////////LOW
	}
	else if (sys_info.curr_absrb_val <= sys_info.std_absrb_val[5])
	{
		sys_info.curr_ResVal = ( ( (sys_info.curr_absrb_val - sys_info.std_absrb_val[4]) / (sys_info.std_absrb_val[5] - sys_info.std_absrb_val[4]) )
			* (sys_info.act_stan_vals[5] - sys_info.act_stan_vals[4]) + sys_info.act_stan_vals[4]);
		sys_info.curr_Result_cat = 3;           //////////////Medium
	}
	else if (sys_info.curr_absrb_val <= sys_info.std_absrb_val[6])
	{
		sys_info.curr_ResVal = ( ( (sys_info.curr_absrb_val - sys_info.std_absrb_val[5]) / (sys_info.std_absrb_val[6] - sys_info.std_absrb_val[5]) )
			* (sys_info.act_stan_vals[6] - sys_info.act_stan_vals[5]) + sys_info.act_stan_vals[5]);
		sys_info.curr_Result_cat = 3;           //////////////Medium
	}
	else if (sys_info.curr_absrb_val <= sys_info.std_absrb_val[7])
	{
		sys_info.curr_ResVal = ( ( (sys_info.curr_absrb_val - sys_info.std_absrb_val[6]) / (sys_info.std_absrb_val[7] - sys_info.std_absrb_val[6]) )
			* (sys_info.act_stan_vals[7] - sys_info.act_stan_vals[6]) + sys_info.act_stan_vals[6]);
		sys_info.curr_Result_cat = 4;           //////////////High
	}
	else if (sys_info.curr_absrb_val <= sys_info.std_absrb_val[8])
	{
		sys_info.curr_ResVal = ( ( (sys_info.curr_absrb_val - sys_info.std_absrb_val[7]) / (sys_info.std_absrb_val[8] - sys_info.std_absrb_val[7]) )
			* (sys_info.act_stan_vals[8] - sys_info.act_stan_vals[7]) + sys_info.act_stan_vals[7]);
		sys_info.curr_Result_cat = 4;           //////////////High
	}
	else if (sys_info.curr_absrb_val <= sys_info.std_absrb_val[9])
	{
		sys_info.curr_ResVal = ( ( (sys_info.curr_absrb_val - sys_info.std_absrb_val[8]) / (sys_info.std_absrb_val[9] - sys_info.std_absrb_val[8]) )
			* (sys_info.act_stan_vals[9] - sys_info.act_stan_vals[8]) + sys_info.act_stan_vals[8]);
		sys_info.curr_Result_cat = 5;           //////////////Verry High
	}
	else if (sys_info.curr_absrb_val <= sys_info.std_absrb_val[10])
	{
		sys_info.curr_ResVal = ( ( (sys_info.curr_absrb_val - sys_info.std_absrb_val[9]) / (sys_info.std_absrb_val[10] - sys_info.std_absrb_val[9]) )
			* (sys_info.act_stan_vals[10] - sys_info.act_stan_vals[9]) + sys_info.act_stan_vals[9]);
		sys_info.curr_Result_cat = 5;           //////////////Verry High
	}
	else // sys_info.curr_absrb_val > sys_info.std_absrb_val[10]
	{
		sys_info.curr_ResVal = (sys_info.act_stan_vals[10] / sys_info.std_absrb_val[10]) * (sys_info.curr_absrb_val);

		if (sys_info.curr_ResVal > sys_info.act_stan_vals[10])
		{
			sys_info.curr_ResVal = sys_info.act_stan_vals[10];
		}
		sys_info.curr_Result_cat = 5;           //////////////Verry High
	}

}
