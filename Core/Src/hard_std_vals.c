/*
 * Std_hard_vals.c
 *
 *  Created on: 08-Sept-2026
 *      Author: DKD
 */

/* Includes ------------------------------------------------------------------*/
#include "hard_std_vals.h"
#include "my_struct.h"

/* Private define ------------------------------------------------------------*/
#ifndef ARC_CAL
#define ARC_CAL
#endif

#ifndef TM_MULTIPLIER
#define TM_MULTIPLIER 20
#endif

/* USER CODE BEGIN 1 */

#ifdef ARC_CAL
void sys_var_init(void)
{
	// system chemical quantity info
	switch (save_sys_info.bk_var.curr_sys_add)
	{
	case MAGNESIUM:
		sys_info.chem_1a_qty = 2000; // 20.00ml
		sys_info.chem_1b_qty = 0;
		sys_info.chem_2c_qty = 200;
		sys_info.chem_3d_qty = 0;
		sys_info.chem_4e_qty = 0;
		sys_info.chem_5f_qty = 0;
		sys_info.wtr_g_qty = 0;		 // for water quantity in grams
		sys_info.led_pwm_red = 0xff; // pwm value
		sys_info.led_pwm_green = 0xff;
		sys_info.led_pwm_blue = 0xff;
		sys_info.val_cal_x = 1; // green//0 - red, 1 - green, 2- blue, 3 - Clear.
		sys_info.val_cal_y = 2; // blue//0 - red, 1 - green, 2- blue, 3 - Clear.
		sys_info.act_stan_vals[0] = 0;
		sys_info.act_stan_vals[1] = 0;
		sys_info.act_stan_vals[2] = 0;
		sys_info.act_stan_vals[3] = 0;
		sys_info.act_stan_vals[4] = 0;
		sys_info.act_stan_vals[5] = 0;
		sys_info.act_stan_vals[6] = 0;
		sys_info.act_stan_vals[7] = 0;
		break;
	case IRON:
		sys_info.chem_1a_qty = 200; /// 2.00ml
		sys_info.chem_1b_qty = 0;
		sys_info.chem_2c_qty = 200;
		sys_info.chem_3d_qty = 200;
		sys_info.chem_4e_qty = 0;
		sys_info.chem_5f_qty = 0;
		sys_info.wtr_g_qty = 0;
		sys_info.led_pwm_red = 0xff;
		sys_info.led_pwm_green = 0xff;
		sys_info.led_pwm_blue = 0xff;
		sys_info.val_cal_x = 1; // green
		sys_info.val_cal_y = 2; // blue
		sys_info.act_stan_vals[0] = 0;
		sys_info.act_stan_vals[1] = 0;
		sys_info.act_stan_vals[2] = 0;
		sys_info.act_stan_vals[3] = 0;
		sys_info.act_stan_vals[4] = 0;
		sys_info.act_stan_vals[5] = 0;
		sys_info.act_stan_vals[6] = 0;
		sys_info.act_stan_vals[7] = 0;
		break;
	case COPPER:
		sys_info.chem_1a_qty = 300;
		sys_info.chem_1b_qty = 0;
		sys_info.chem_2c_qty = 150;
		sys_info.chem_3d_qty = 0;
		sys_info.chem_4e_qty = 0;
		sys_info.chem_5f_qty = 0;
		sys_info.wtr_g_qty = 0;
		sys_info.led_pwm_red = 0xff;
		sys_info.led_pwm_green = 0xff;
		sys_info.led_pwm_blue = 0xff;
		sys_info.val_cal_x = 1; // green
		sys_info.val_cal_y = 2; // blue
		sys_info.act_stan_vals[0] = 0.00;
		sys_info.act_stan_vals[1] = 0.00; // duplicate blank
		sys_info.act_stan_vals[2] = 0.20;
		sys_info.act_stan_vals[3] = 0.40;
		sys_info.act_stan_vals[4] = 0.60;
		sys_info.act_stan_vals[5] = 0.80;
		sys_info.act_stan_vals[6] = 1.00;
		sys_info.act_stan_vals[7] = 2.00;
		break;
	case ZINC:
		sys_info.chem_1a_qty = 2000;
		sys_info.chem_1b_qty = 0;
		sys_info.chem_2c_qty = 200;
		sys_info.chem_3d_qty = 300;
		sys_info.chem_4e_qty = 0;
		sys_info.chem_5f_qty = 0;
		sys_info.wtr_g_qty = 0;
		sys_info.led_pwm_red = 0xff;
		sys_info.led_pwm_green = 0xff;
		sys_info.led_pwm_blue = 0xff;
		sys_info.val_cal_x = 1; // green
		sys_info.val_cal_y = 2; // blue
		sys_info.act_stan_vals[0] = 0;
		sys_info.act_stan_vals[1] = 0;
		sys_info.act_stan_vals[2] = 0;
		sys_info.act_stan_vals[3] = 0;
		sys_info.act_stan_vals[4] = 0;
		sys_info.act_stan_vals[5] = 0;
		sys_info.act_stan_vals[6] = 0;
		sys_info.act_stan_vals[7] = 0;
		
		break;
	case BORON:
		sys_info.chem_1a_qty = 2000;
		sys_info.chem_1b_qty = 0;
		sys_info.chem_2c_qty = 200;
		sys_info.chem_3d_qty = 0;
		sys_info.chem_4e_qty = 0;
		sys_info.chem_5f_qty = 0;
		sys_info.wtr_g_qty = 0;
		sys_info.led_pwm_red = 0xff;
		sys_info.led_pwm_green = 0xff;
		sys_info.led_pwm_blue = 0xff;
		sys_info.val_cal_x = 1; // green
		sys_info.val_cal_y = 2; // blue
		sys_info.act_stan_vals[0] = 0;
		sys_info.act_stan_vals[1] = 0;   // duplicate blank
		sys_info.act_stan_vals[2] = 0.5;
		sys_info.act_stan_vals[3] = 1;
		sys_info.act_stan_vals[4] = 1.5;
		sys_info.act_stan_vals[5] = 2;
		sys_info.act_stan_vals[6] = 2.5;
		sys_info.act_stan_vals[7] = 5;
		
		break;
	case SULPHUR:
		sys_info.chem_1a_qty = 1500;
		sys_info.chem_1b_qty = 0;
		sys_info.chem_2c_qty = 30;
		sys_info.chem_3d_qty = 0;
		sys_info.chem_4e_qty = 0;
		sys_info.chem_5f_qty = 0;
		sys_info.wtr_g_qty = 120;
		sys_info.led_pwm_red = 0xff;
		sys_info.led_pwm_green = 0xff;
		sys_info.led_pwm_blue = 0xff;
		sys_info.val_cal_x = 0;			  //
		sys_info.val_cal_y = 2;			  //
		sys_info.act_stan_vals[0] = 0;	  // Blank
		sys_info.act_stan_vals[1] = 0;    // Duplicate Blank
		sys_info.act_stan_vals[2] = 1.00; // 1 ppm
		sys_info.act_stan_vals[3] = 5;	  // 5 ppm
		sys_info.act_stan_vals[4] = 10;	  // 10 ppm
		sys_info.act_stan_vals[5] = 20;	  // 20 ppm
		sys_info.act_stan_vals[6] = 30;	  // 30 ppm
		sys_info.act_stan_vals[7] = 50;	  // 50 ppm
		break;
	case POTASSIUM:

		sys_info.chem_1a_qty = 500;   /* Reagent 1A quantity for potassium test */
		sys_info.chem_1b_qty = 100;  /* Reagent 1B quantity for potassium test */
		sys_info.chem_2c_qty = 100;  /* Reagent 2C quantity for potassium test */
		sys_info.chem_3d_qty = 55;   /* Reagent 3D quantity for potassium test */
		sys_info.chem_4e_qty = 100;  /* Reagent 4E quantity for potassium test */
		sys_info.chem_5f_qty = 0;    /* Reagent 5F quantity = 0 (not used for potassium) */
		sys_info.wtr_g_qty = 520;    /* Water/glass wash quantity for potassium test */
		sys_info.led_pwm_red = 0xff;   /* Red LED PWM = 0xFF (max brightness, 255) */
		sys_info.led_pwm_green = 0xff; /* Green LED PWM = 0xFF (max brightness, 255) */
		sys_info.led_pwm_blue = 0xff;  /* Blue LED PWM = 0xFF (max brightness, 255) */
		sys_info.val_cal_x = 1; // green   /* Calibration X-axis = 1 (Green channel) */
		sys_info.val_cal_y = 2; // blue    /* Calibration Y-axis = 2 (Blue channel) */
		sys_info.act_stan_vals[0] = 0;    /* Standard 0 concentration = 0 ppm (blank) --> category 1: Very Low */
		sys_info.act_stan_vals[1] = 0;    /* Standard 1 concentration = 2 ppm --> category 1: Very Low */
		sys_info.act_stan_vals[2] = 2;    /* Standard 2 concentration = 4 ppm --> category 1: Very Low */
		sys_info.act_stan_vals[3] = 4;    /* Standard 3 concentration = 6 ppm --> category 2: Low */
		sys_info.act_stan_vals[4] = 6;    /* Standard 4 concentration = 6 ppm (duplicate) --> category 2: Low */
		sys_info.act_stan_vals[5] = 8;    /* Standard 5 concentration = 8 ppm --> category 3: Medium */
		sys_info.act_stan_vals[6] = 10;    /* Standard 6 concentration = 8 ppm (duplicate) --> category 3: Medium */
		sys_info.act_stan_vals[7] = 20;   /* Standard 7 concentration = 10 ppm --> category 4: High */
		
		break;
	case PHOSPHORUS:
		sys_info.chem_1a_qty = 1000; // 10.00ml
		sys_info.chem_1b_qty = 0;
		sys_info.chem_2c_qty = 25;	// 0.25ml
		sys_info.chem_3d_qty = 200; // 2.00ml
		sys_info.chem_4e_qty = 100; // 1.00ml
		sys_info.chem_5f_qty = 0;
		sys_info.wtr_g_qty = 475;
		sys_info.led_pwm_red = 0xff;
		sys_info.led_pwm_green = 0xff;
		sys_info.led_pwm_blue = 0xff;
		sys_info.val_cal_x = 1; // green
		sys_info.val_cal_y = 2; // blue
		sys_info.act_stan_vals[0] = 0.00;
		sys_info.act_stan_vals[1] = 0.00; // duplicate blank
		sys_info.act_stan_vals[2] = 0.1;
		sys_info.act_stan_vals[3] = 0.2;
		sys_info.act_stan_vals[4] = 0.4;
		sys_info.act_stan_vals[5] = 0.6;
		sys_info.act_stan_vals[6] = 0.8;
		sys_info.act_stan_vals[7] = 1.0;
		break;
	case NITROGEN:
		sys_info.chem_1a_qty = 1000;
		sys_info.chem_1b_qty = 0;
		sys_info.chem_2c_qty = 100;
		sys_info.chem_3d_qty = 100;
		sys_info.chem_4e_qty = 0;
		sys_info.chem_5f_qty = 0;
		sys_info.wtr_g_qty = 580;
		sys_info.led_pwm_red = 0xff;
		sys_info.led_pwm_green = 0xff;
		sys_info.led_pwm_blue = 0xff;
		sys_info.val_cal_x = 1; // green
		sys_info.val_cal_y = 2; // blue
		sys_info.act_stan_vals[0] = 0;
		sys_info.act_stan_vals[1] = 0;  // duplicate blank
		sys_info.act_stan_vals[2] = 2;
		sys_info.act_stan_vals[3] = 4;
		sys_info.act_stan_vals[4] = 6;
		sys_info.act_stan_vals[5] = 8;
		sys_info.act_stan_vals[6] = 10;
		sys_info.act_stan_vals[7] = 10;
		break;
	case MASTER_SYS:
	case ORGANIC_CARBON:
		sys_info.chem_1a_qty = 1000; // 10.00ml
		sys_info.chem_1b_qty = 0;
		sys_info.chem_2c_qty = 1000; // 10.00ml
		sys_info.chem_3d_qty = 0;
		sys_info.chem_4e_qty = 0;
		sys_info.chem_5f_qty = 0;
		sys_info.wtr_g_qty = 0;
		sys_info.led_pwm_red = 0xff;
		sys_info.led_pwm_green = 0xff;
		sys_info.led_pwm_blue = 0xff;
		sys_info.val_cal_x = 0; // red
		sys_info.val_cal_y = 2; // blue
		sys_info.act_stan_vals[0] = 00;
		sys_info.act_stan_vals[1] = 00;  // duplicate blank
		sys_info.act_stan_vals[2] = 50;  // 0.50%
		sys_info.act_stan_vals[3] = 100; // 1.00%
		sys_info.act_stan_vals[4] = 150; // 1.50%
		sys_info.act_stan_vals[5] = 200; // 2.00%
		sys_info.act_stan_vals[6] = 250; // 2.50%
		sys_info.act_stan_vals[7] = 300; // 3.00%
		break;
	}
	sys_info.wtr_f_wsh = 1300;
	sys_info.wtr_f_wsh_in_tm = sys_info.wtr_f_wsh * TM_MULTIPLIER;
	sys_info.drain_tm = (1000 * 55);	   // 55 sec
	sys_info.drain_mix_tm = (1000 * 10);   // 10 sec
	sys_info.tak_data_skp_tm = (10 * 25);  // solution rest time//25 sec
	sys_info.auto_zero_skp_tm = (10 * 25); // solution rest time//25 sec
	// sys_info.auto_set_skp_tm = (10*25);//solution rest time//25 sec

	for (uint8_t i = 0; i < NOS_STD; i++)
	{
		for (uint8_t j = 0; j < 4; j++)
		{
			sys_info.opt_std_vars.strd_vars[i][j] = save_sys_info.bk_var.hrd_std_vars.strd_vars[i][j];
			sys_info.opt_std_vars2.strd_vars[i][j] = save_sys_info.bk_var.hrd_std_vars2.strd_vars[i][j];
		}
	}

	sys_info.Stat_L.warm_up_stat = 0;
	sys_info.Stat_L.drain_wsh_stat = 0; // 1
}
#endif

#ifdef ARC_CAL
void init_hrd_strd(void)
{
switch(save_sys_info.bk_var.curr_sys_add)
	{
	case MAGNESIUM:
		//alt func = 0;  												//alt func = 1;
       save_sys_info.bk_var.hrd_std_vars.stan_0_red = 18270;        save_sys_info.bk_var.hrd_std_vars2.stan_0_red = 18270;
       save_sys_info.bk_var.hrd_std_vars.stan_0_green = 14522;      save_sys_info.bk_var.hrd_std_vars2.stan_0_green = 14522;
       save_sys_info.bk_var.hrd_std_vars.stan_0_blue = 16275;       save_sys_info.bk_var.hrd_std_vars2.stan_0_blue = 16275;
       save_sys_info.bk_var.hrd_std_vars.stan_0_clear = 51293;      save_sys_info.bk_var.hrd_std_vars2.stan_0_clear = 51293;

       save_sys_info.bk_var.hrd_std_vars.stan_1_red = 16130;        save_sys_info.bk_var.hrd_std_vars2.stan_1_red = 16130;
       save_sys_info.bk_var.hrd_std_vars.stan_1_green = 13343;      save_sys_info.bk_var.hrd_std_vars2.stan_1_green = 13343;
       save_sys_info.bk_var.hrd_std_vars.stan_1_blue = 15134;       save_sys_info.bk_var.hrd_std_vars2.stan_1_blue = 15134;
       save_sys_info.bk_var.hrd_std_vars.stan_1_clear = 46676;      save_sys_info.bk_var.hrd_std_vars2.stan_1_clear = 46676;

       save_sys_info.bk_var.hrd_std_vars.stan_2_red = 14000;        save_sys_info.bk_var.hrd_std_vars2.stan_2_red = 14000;
       save_sys_info.bk_var.hrd_std_vars.stan_2_green = 12129;      save_sys_info.bk_var.hrd_std_vars2.stan_2_green = 12129;
       save_sys_info.bk_var.hrd_std_vars.stan_2_blue = 13953;       save_sys_info.bk_var.hrd_std_vars2.stan_2_blue = 13953;
       save_sys_info.bk_var.hrd_std_vars.stan_2_clear = 41985;      save_sys_info.bk_var.hrd_std_vars2.stan_2_clear = 41985;

       save_sys_info.bk_var.hrd_std_vars.stan_3_red = 10423;        save_sys_info.bk_var.hrd_std_vars2.stan_3_red = 10423;
       save_sys_info.bk_var.hrd_std_vars.stan_3_green = 9970;       save_sys_info.bk_var.hrd_std_vars2.stan_3_green = 9970;
       save_sys_info.bk_var.hrd_std_vars.stan_3_blue = 11836;       save_sys_info.bk_var.hrd_std_vars2.stan_3_blue = 11836;
       save_sys_info.bk_var.hrd_std_vars.stan_3_clear = 33830;      save_sys_info.bk_var.hrd_std_vars2.stan_3_clear = 33830;

       save_sys_info.bk_var.hrd_std_vars.stan_4_red = 7956;         save_sys_info.bk_var.hrd_std_vars2.stan_4_red = 7956;
       save_sys_info.bk_var.hrd_std_vars.stan_4_green = 8365;       save_sys_info.bk_var.hrd_std_vars2.stan_4_green = 8365;
       save_sys_info.bk_var.hrd_std_vars.stan_4_blue = 10230;       save_sys_info.bk_var.hrd_std_vars2.stan_4_blue = 10230;
       save_sys_info.bk_var.hrd_std_vars.stan_4_clear = 27924;      save_sys_info.bk_var.hrd_std_vars2.stan_4_clear = 27924;

       save_sys_info.bk_var.hrd_std_vars.stan_5_red = 5811;         save_sys_info.bk_var.hrd_std_vars2.stan_5_red = 5811;
       save_sys_info.bk_var.hrd_std_vars.stan_5_green = 6847;       save_sys_info.bk_var.hrd_std_vars2.stan_5_green = 6847;
       save_sys_info.bk_var.hrd_std_vars.stan_5_blue = 8673;        save_sys_info.bk_var.hrd_std_vars2.stan_5_blue = 8673;
       save_sys_info.bk_var.hrd_std_vars.stan_5_clear = 22489;      save_sys_info.bk_var.hrd_std_vars2.stan_5_clear = 22489;

       save_sys_info.bk_var.hrd_std_vars.stan_6_red = 4425;        save_sys_info.bk_var.hrd_std_vars2.stan_6_red = 4425;
       save_sys_info.bk_var.hrd_std_vars.stan_6_green = 5785;      save_sys_info.bk_var.hrd_std_vars2.stan_6_green = 5785;
       save_sys_info.bk_var.hrd_std_vars.stan_6_blue = 7557;       save_sys_info.bk_var.hrd_std_vars2.stan_6_blue = 7557;
       save_sys_info.bk_var.hrd_std_vars.stan_6_clear = 18768;     save_sys_info.bk_var.hrd_std_vars2.stan_6_clear = 18768;

       save_sys_info.bk_var.hrd_std_vars.stan_7_red = 4425;        save_sys_info.bk_var.hrd_std_vars2.stan_7_red = 4425;
       save_sys_info.bk_var.hrd_std_vars.stan_7_green = 5785;      save_sys_info.bk_var.hrd_std_vars2.stan_7_green = 5785;
       save_sys_info.bk_var.hrd_std_vars.stan_7_blue = 7557;       save_sys_info.bk_var.hrd_std_vars2.stan_7_blue = 7557;
       save_sys_info.bk_var.hrd_std_vars.stan_7_clear = 18768;     save_sys_info.bk_var.hrd_std_vars2.stan_7_clear = 18768;
		break;
	case IRON:
       save_sys_info.bk_var.hrd_std_vars.stan_0_red = 18270;        save_sys_info.bk_var.hrd_std_vars2.stan_0_red = 18270;
       save_sys_info.bk_var.hrd_std_vars.stan_0_green = 14522;      save_sys_info.bk_var.hrd_std_vars2.stan_0_green = 14522;
       save_sys_info.bk_var.hrd_std_vars.stan_0_blue = 16275;       save_sys_info.bk_var.hrd_std_vars2.stan_0_blue = 16275;
       save_sys_info.bk_var.hrd_std_vars.stan_0_clear = 51293;      save_sys_info.bk_var.hrd_std_vars2.stan_0_clear = 51293;

       save_sys_info.bk_var.hrd_std_vars.stan_1_red = 16130;        save_sys_info.bk_var.hrd_std_vars2.stan_1_red = 16130;
       save_sys_info.bk_var.hrd_std_vars.stan_1_green = 13343;      save_sys_info.bk_var.hrd_std_vars2.stan_1_green = 13343;
       save_sys_info.bk_var.hrd_std_vars.stan_1_blue = 15134;       save_sys_info.bk_var.hrd_std_vars2.stan_1_blue = 15134;
       save_sys_info.bk_var.hrd_std_vars.stan_1_clear = 46676;      save_sys_info.bk_var.hrd_std_vars2.stan_1_clear = 46676;

       save_sys_info.bk_var.hrd_std_vars.stan_2_red = 14000;        save_sys_info.bk_var.hrd_std_vars2.stan_2_red = 14000;
       save_sys_info.bk_var.hrd_std_vars.stan_2_green = 12129;      save_sys_info.bk_var.hrd_std_vars2.stan_2_green = 12129;
       save_sys_info.bk_var.hrd_std_vars.stan_2_blue = 13953;       save_sys_info.bk_var.hrd_std_vars2.stan_2_blue = 13953;
       save_sys_info.bk_var.hrd_std_vars.stan_2_clear = 41985;      save_sys_info.bk_var.hrd_std_vars2.stan_2_clear = 41985;

       save_sys_info.bk_var.hrd_std_vars.stan_3_red = 10423;        save_sys_info.bk_var.hrd_std_vars2.stan_3_red = 10423;
       save_sys_info.bk_var.hrd_std_vars.stan_3_green = 9970;       save_sys_info.bk_var.hrd_std_vars2.stan_3_green = 9970;
       save_sys_info.bk_var.hrd_std_vars.stan_3_blue = 11836;       save_sys_info.bk_var.hrd_std_vars2.stan_3_blue = 11836;
       save_sys_info.bk_var.hrd_std_vars.stan_3_clear = 33830;      save_sys_info.bk_var.hrd_std_vars2.stan_3_clear = 33830;

       save_sys_info.bk_var.hrd_std_vars.stan_4_red = 7956;         save_sys_info.bk_var.hrd_std_vars2.stan_4_red = 7956;
       save_sys_info.bk_var.hrd_std_vars.stan_4_green = 8365;       save_sys_info.bk_var.hrd_std_vars2.stan_4_green = 8365;
       save_sys_info.bk_var.hrd_std_vars.stan_4_blue = 10230;       save_sys_info.bk_var.hrd_std_vars2.stan_4_blue = 10230;
       save_sys_info.bk_var.hrd_std_vars.stan_4_clear = 27924;      save_sys_info.bk_var.hrd_std_vars2.stan_4_clear = 27924;

       save_sys_info.bk_var.hrd_std_vars.stan_5_red = 5811;         save_sys_info.bk_var.hrd_std_vars2.stan_5_red = 5811;
       save_sys_info.bk_var.hrd_std_vars.stan_5_green = 6847;       save_sys_info.bk_var.hrd_std_vars2.stan_5_green = 6847;
       save_sys_info.bk_var.hrd_std_vars.stan_5_blue = 8673;        save_sys_info.bk_var.hrd_std_vars2.stan_5_blue = 8673;
       save_sys_info.bk_var.hrd_std_vars.stan_5_clear = 22489;      save_sys_info.bk_var.hrd_std_vars2.stan_5_clear = 22489;

       save_sys_info.bk_var.hrd_std_vars.stan_6_red = 4425;        save_sys_info.bk_var.hrd_std_vars2.stan_6_red = 4425;
       save_sys_info.bk_var.hrd_std_vars.stan_6_green = 5785;      save_sys_info.bk_var.hrd_std_vars2.stan_6_green = 5785;
       save_sys_info.bk_var.hrd_std_vars.stan_6_blue = 7557;       save_sys_info.bk_var.hrd_std_vars2.stan_6_blue = 7557;
       save_sys_info.bk_var.hrd_std_vars.stan_6_clear = 18768;     save_sys_info.bk_var.hrd_std_vars2.stan_6_clear = 18768;

       save_sys_info.bk_var.hrd_std_vars.stan_7_red = 4425;        save_sys_info.bk_var.hrd_std_vars2.stan_7_red = 4425;
       save_sys_info.bk_var.hrd_std_vars.stan_7_green = 5785;      save_sys_info.bk_var.hrd_std_vars2.stan_7_green = 5785;
       save_sys_info.bk_var.hrd_std_vars.stan_7_blue = 7557;       save_sys_info.bk_var.hrd_std_vars2.stan_7_blue = 7557;
       save_sys_info.bk_var.hrd_std_vars.stan_7_clear = 18768;     save_sys_info.bk_var.hrd_std_vars2.stan_7_clear = 18768;
		break;
	case COPPER:
       /* stan_0 - Blank (0.00 ppm) */
       save_sys_info.bk_var.hrd_std_vars.stan_0_red = 14975;        save_sys_info.bk_var.hrd_std_vars2.stan_0_red = 14975;
       save_sys_info.bk_var.hrd_std_vars.stan_0_green = 17375;      save_sys_info.bk_var.hrd_std_vars2.stan_0_green = 17375;
       save_sys_info.bk_var.hrd_std_vars.stan_0_blue = 18161;       save_sys_info.bk_var.hrd_std_vars2.stan_0_blue = 18161;
       save_sys_info.bk_var.hrd_std_vars.stan_0_clear = 50225;      save_sys_info.bk_var.hrd_std_vars2.stan_0_clear = 50225;

       /* stan_1 - Blank duplicate (same as stan_0) */
       save_sys_info.bk_var.hrd_std_vars.stan_1_red = 14975;        save_sys_info.bk_var.hrd_std_vars2.stan_1_red = 14975;
       save_sys_info.bk_var.hrd_std_vars.stan_1_green = 17375;      save_sys_info.bk_var.hrd_std_vars2.stan_1_green = 17375;
       save_sys_info.bk_var.hrd_std_vars.stan_1_blue = 18161;       save_sys_info.bk_var.hrd_std_vars2.stan_1_blue = 18161;
       save_sys_info.bk_var.hrd_std_vars.stan_1_clear = 50225;      save_sys_info.bk_var.hrd_std_vars2.stan_1_clear = 50225;

       /* stan_2 - 0.20 ppm */
       save_sys_info.bk_var.hrd_std_vars.stan_2_red = 14845;        save_sys_info.bk_var.hrd_std_vars2.stan_2_red = 14845;
       save_sys_info.bk_var.hrd_std_vars.stan_2_green = 16693;      save_sys_info.bk_var.hrd_std_vars2.stan_2_green = 16693;
       save_sys_info.bk_var.hrd_std_vars.stan_2_blue = 17079;       save_sys_info.bk_var.hrd_std_vars2.stan_2_blue = 17079;
       save_sys_info.bk_var.hrd_std_vars.stan_2_clear = 48375;      save_sys_info.bk_var.hrd_std_vars2.stan_2_clear = 48375;

       /* stan_3 - 0.40 ppm */
       save_sys_info.bk_var.hrd_std_vars.stan_3_red = 14797;        save_sys_info.bk_var.hrd_std_vars2.stan_3_red = 14797;
       save_sys_info.bk_var.hrd_std_vars.stan_3_green = 16292;      save_sys_info.bk_var.hrd_std_vars2.stan_3_green = 16292;
       save_sys_info.bk_var.hrd_std_vars.stan_3_blue = 16388;       save_sys_info.bk_var.hrd_std_vars2.stan_3_blue = 16388;
       save_sys_info.bk_var.hrd_std_vars.stan_3_clear = 47270;      save_sys_info.bk_var.hrd_std_vars2.stan_3_clear = 47270;

       /* stan_4 - 0.60 ppm */
       save_sys_info.bk_var.hrd_std_vars.stan_4_red = 14735;        save_sys_info.bk_var.hrd_std_vars2.stan_4_red = 14735;
       save_sys_info.bk_var.hrd_std_vars.stan_4_green = 15849;      save_sys_info.bk_var.hrd_std_vars2.stan_4_green = 15849;
       save_sys_info.bk_var.hrd_std_vars.stan_4_blue = 15710;       save_sys_info.bk_var.hrd_std_vars2.stan_4_blue = 15710;
       save_sys_info.bk_var.hrd_std_vars.stan_4_clear = 46126;      save_sys_info.bk_var.hrd_std_vars2.stan_4_clear = 46126;

       /* stan_5 - 0.80 ppm */
       save_sys_info.bk_var.hrd_std_vars.stan_5_red = 14323;        save_sys_info.bk_var.hrd_std_vars2.stan_5_red = 14323;
       save_sys_info.bk_var.hrd_std_vars.stan_5_green = 14716;      save_sys_info.bk_var.hrd_std_vars2.stan_5_green = 14716;
       save_sys_info.bk_var.hrd_std_vars.stan_5_blue = 14194;       save_sys_info.bk_var.hrd_std_vars2.stan_5_blue = 14194;
       save_sys_info.bk_var.hrd_std_vars.stan_5_clear = 43148;      save_sys_info.bk_var.hrd_std_vars2.stan_5_clear = 43148;

       /* stan_6 - 1.00 ppm */
       save_sys_info.bk_var.hrd_std_vars.stan_6_red = 14369;        save_sys_info.bk_var.hrd_std_vars2.stan_6_red = 14369;
       save_sys_info.bk_var.hrd_std_vars.stan_6_green = 14552;      save_sys_info.bk_var.hrd_std_vars2.stan_6_green = 14552;
       save_sys_info.bk_var.hrd_std_vars.stan_6_blue = 13818;       save_sys_info.bk_var.hrd_std_vars2.stan_6_blue = 13818;
       save_sys_info.bk_var.hrd_std_vars.stan_6_clear = 42671;      save_sys_info.bk_var.hrd_std_vars2.stan_6_clear = 42671;

       /* stan_7 - 2.00 ppm */
       save_sys_info.bk_var.hrd_std_vars.stan_7_red = 13382;        save_sys_info.bk_var.hrd_std_vars2.stan_7_red = 13382;
       save_sys_info.bk_var.hrd_std_vars.stan_7_green = 11511;      save_sys_info.bk_var.hrd_std_vars2.stan_7_green = 11511;
       save_sys_info.bk_var.hrd_std_vars.stan_7_blue = 9846;        save_sys_info.bk_var.hrd_std_vars2.stan_7_blue = 9846;
       save_sys_info.bk_var.hrd_std_vars.stan_7_clear = 34849;      save_sys_info.bk_var.hrd_std_vars2.stan_7_clear = 34849;
		break;
	case ZINC:
       save_sys_info.bk_var.hrd_std_vars.stan_0_red = 18270;        save_sys_info.bk_var.hrd_std_vars2.stan_0_red = 18270;
       save_sys_info.bk_var.hrd_std_vars.stan_0_green = 14522;      save_sys_info.bk_var.hrd_std_vars2.stan_0_green = 14522;
       save_sys_info.bk_var.hrd_std_vars.stan_0_blue = 16275;       save_sys_info.bk_var.hrd_std_vars2.stan_0_blue = 16275;
       save_sys_info.bk_var.hrd_std_vars.stan_0_clear = 51293;      save_sys_info.bk_var.hrd_std_vars2.stan_0_clear = 51293;

       save_sys_info.bk_var.hrd_std_vars.stan_1_red = 16130;        save_sys_info.bk_var.hrd_std_vars2.stan_1_red = 16130;
       save_sys_info.bk_var.hrd_std_vars.stan_1_green = 13343;      save_sys_info.bk_var.hrd_std_vars2.stan_1_green = 13343;
       save_sys_info.bk_var.hrd_std_vars.stan_1_blue = 15134;       save_sys_info.bk_var.hrd_std_vars2.stan_1_blue = 15134;
       save_sys_info.bk_var.hrd_std_vars.stan_1_clear = 46676;      save_sys_info.bk_var.hrd_std_vars2.stan_1_clear = 46676;

       save_sys_info.bk_var.hrd_std_vars.stan_2_red = 14000;        save_sys_info.bk_var.hrd_std_vars2.stan_2_red = 14000;
       save_sys_info.bk_var.hrd_std_vars.stan_2_green = 12129;      save_sys_info.bk_var.hrd_std_vars2.stan_2_green = 12129;
       save_sys_info.bk_var.hrd_std_vars.stan_2_blue = 13953;       save_sys_info.bk_var.hrd_std_vars2.stan_2_blue = 13953;
       save_sys_info.bk_var.hrd_std_vars.stan_2_clear = 41985;      save_sys_info.bk_var.hrd_std_vars2.stan_2_clear = 41985;

       save_sys_info.bk_var.hrd_std_vars.stan_3_red = 10423;        save_sys_info.bk_var.hrd_std_vars2.stan_3_red = 10423;
       save_sys_info.bk_var.hrd_std_vars.stan_3_green = 9970;       save_sys_info.bk_var.hrd_std_vars2.stan_3_green = 9970;
       save_sys_info.bk_var.hrd_std_vars.stan_3_blue = 11836;       save_sys_info.bk_var.hrd_std_vars2.stan_3_blue = 11836;
       save_sys_info.bk_var.hrd_std_vars.stan_3_clear = 33830;      save_sys_info.bk_var.hrd_std_vars2.stan_3_clear = 33830;

       save_sys_info.bk_var.hrd_std_vars.stan_4_red = 7956;         save_sys_info.bk_var.hrd_std_vars2.stan_4_red = 7956;
       save_sys_info.bk_var.hrd_std_vars.stan_4_green = 8365;       save_sys_info.bk_var.hrd_std_vars2.stan_4_green = 8365;
       save_sys_info.bk_var.hrd_std_vars.stan_4_blue = 10230;       save_sys_info.bk_var.hrd_std_vars2.stan_4_blue = 10230;
       save_sys_info.bk_var.hrd_std_vars.stan_4_clear = 27924;      save_sys_info.bk_var.hrd_std_vars2.stan_4_clear = 27924;

       save_sys_info.bk_var.hrd_std_vars.stan_5_red = 5811;         save_sys_info.bk_var.hrd_std_vars2.stan_5_red = 5811;
       save_sys_info.bk_var.hrd_std_vars.stan_5_green = 6847;       save_sys_info.bk_var.hrd_std_vars2.stan_5_green = 6847;
       save_sys_info.bk_var.hrd_std_vars.stan_5_blue = 8673;        save_sys_info.bk_var.hrd_std_vars2.stan_5_blue = 8673;
       save_sys_info.bk_var.hrd_std_vars.stan_5_clear = 22489;      save_sys_info.bk_var.hrd_std_vars2.stan_5_clear = 22489;

       save_sys_info.bk_var.hrd_std_vars.stan_6_red = 4425;        save_sys_info.bk_var.hrd_std_vars2.stan_6_red = 4425;
       save_sys_info.bk_var.hrd_std_vars.stan_6_green = 5785;      save_sys_info.bk_var.hrd_std_vars2.stan_6_green = 5785;
       save_sys_info.bk_var.hrd_std_vars.stan_6_blue = 7557;       save_sys_info.bk_var.hrd_std_vars2.stan_6_blue = 7557;
       save_sys_info.bk_var.hrd_std_vars.stan_6_clear = 18768;     save_sys_info.bk_var.hrd_std_vars2.stan_6_clear = 18768;

       save_sys_info.bk_var.hrd_std_vars.stan_7_red = 4425;        save_sys_info.bk_var.hrd_std_vars2.stan_7_red = 4425;
       save_sys_info.bk_var.hrd_std_vars.stan_7_green = 5785;      save_sys_info.bk_var.hrd_std_vars2.stan_7_green = 5785;
       save_sys_info.bk_var.hrd_std_vars.stan_7_blue = 7557;       save_sys_info.bk_var.hrd_std_vars2.stan_7_blue = 7557;
       save_sys_info.bk_var.hrd_std_vars.stan_7_clear = 18768;     save_sys_info.bk_var.hrd_std_vars2.stan_7_clear = 18768;
		break;
	case BORON:
       /* stan_0 - Blank (0.0 ppm) */
       save_sys_info.bk_var.hrd_std_vars.stan_0_red = 18900;        save_sys_info.bk_var.hrd_std_vars2.stan_0_red = 18900;
       save_sys_info.bk_var.hrd_std_vars.stan_0_green = 16598;      save_sys_info.bk_var.hrd_std_vars2.stan_0_green = 16598;
       save_sys_info.bk_var.hrd_std_vars.stan_0_blue = 18503;       save_sys_info.bk_var.hrd_std_vars2.stan_0_blue = 18503;
       save_sys_info.bk_var.hrd_std_vars.stan_0_clear = 51241;      save_sys_info.bk_var.hrd_std_vars2.stan_0_clear = 51241;

       /* stan_1 - Blank duplicate (same as stan_0) */
       save_sys_info.bk_var.hrd_std_vars.stan_1_red = 18900;        save_sys_info.bk_var.hrd_std_vars2.stan_1_red = 18900;
       save_sys_info.bk_var.hrd_std_vars.stan_1_green = 16598;      save_sys_info.bk_var.hrd_std_vars2.stan_1_green = 16598;
       save_sys_info.bk_var.hrd_std_vars.stan_1_blue = 18503;       save_sys_info.bk_var.hrd_std_vars2.stan_1_blue = 18503;
       save_sys_info.bk_var.hrd_std_vars.stan_1_clear = 51241;      save_sys_info.bk_var.hrd_std_vars2.stan_1_clear = 51241;

       /* stan_2 - 0.5 ppm */
       save_sys_info.bk_var.hrd_std_vars.stan_2_red = 19036;        save_sys_info.bk_var.hrd_std_vars2.stan_2_red = 19036;
       save_sys_info.bk_var.hrd_std_vars.stan_2_green = 16651;      save_sys_info.bk_var.hrd_std_vars2.stan_2_green = 16651;
       save_sys_info.bk_var.hrd_std_vars.stan_2_blue = 18116;       save_sys_info.bk_var.hrd_std_vars2.stan_2_blue = 18116;
       save_sys_info.bk_var.hrd_std_vars.stan_2_clear = 51078;      save_sys_info.bk_var.hrd_std_vars2.stan_2_clear = 51078;

       /* stan_3 - 1.0 ppm */
       save_sys_info.bk_var.hrd_std_vars.stan_3_red = 19060;        save_sys_info.bk_var.hrd_std_vars2.stan_3_red = 19060;
       save_sys_info.bk_var.hrd_std_vars.stan_3_green = 16472;      save_sys_info.bk_var.hrd_std_vars2.stan_3_green = 16472;
       save_sys_info.bk_var.hrd_std_vars.stan_3_blue = 17564;       save_sys_info.bk_var.hrd_std_vars2.stan_3_blue = 17564;
       save_sys_info.bk_var.hrd_std_vars.stan_3_clear = 50418;      save_sys_info.bk_var.hrd_std_vars2.stan_3_clear = 50418;

       /* stan_4 - 1.5 ppm */
       save_sys_info.bk_var.hrd_std_vars.stan_4_red = 19038;        save_sys_info.bk_var.hrd_std_vars2.stan_4_red = 19038;
       save_sys_info.bk_var.hrd_std_vars.stan_4_green = 16294;      save_sys_info.bk_var.hrd_std_vars2.stan_4_green = 16294;
       save_sys_info.bk_var.hrd_std_vars.stan_4_blue = 17023;       save_sys_info.bk_var.hrd_std_vars2.stan_4_blue = 17023;
       save_sys_info.bk_var.hrd_std_vars.stan_4_clear = 49724;      save_sys_info.bk_var.hrd_std_vars2.stan_4_clear = 49724;

       /* stan_5 - 2.0 ppm */
       save_sys_info.bk_var.hrd_std_vars.stan_5_red = 19055;        save_sys_info.bk_var.hrd_std_vars2.stan_5_red = 19055;
       save_sys_info.bk_var.hrd_std_vars.stan_5_green = 16129;      save_sys_info.bk_var.hrd_std_vars2.stan_5_green = 16129;
       save_sys_info.bk_var.hrd_std_vars.stan_5_blue = 16493;       save_sys_info.bk_var.hrd_std_vars2.stan_5_blue = 16493;
       save_sys_info.bk_var.hrd_std_vars.stan_5_clear = 49088;      save_sys_info.bk_var.hrd_std_vars2.stan_5_clear = 49088;

       /* stan_6 - 2.5 ppm */
       save_sys_info.bk_var.hrd_std_vars.stan_6_red = 18993;        save_sys_info.bk_var.hrd_std_vars2.stan_6_red = 18993;
       save_sys_info.bk_var.hrd_std_vars.stan_6_green = 15818;      save_sys_info.bk_var.hrd_std_vars2.stan_6_green = 15818;
       save_sys_info.bk_var.hrd_std_vars.stan_6_blue = 15774;       save_sys_info.bk_var.hrd_std_vars2.stan_6_blue = 15774;
       save_sys_info.bk_var.hrd_std_vars.stan_6_clear = 48057;      save_sys_info.bk_var.hrd_std_vars2.stan_6_clear = 48057;

       /* stan_7 - 5.0 ppm */
       save_sys_info.bk_var.hrd_std_vars.stan_7_red = 18936;        save_sys_info.bk_var.hrd_std_vars2.stan_7_red = 18936;
       save_sys_info.bk_var.hrd_std_vars.stan_7_green = 15186;      save_sys_info.bk_var.hrd_std_vars2.stan_7_green = 15186;
       save_sys_info.bk_var.hrd_std_vars.stan_7_blue = 14101;       save_sys_info.bk_var.hrd_std_vars2.stan_7_blue = 14101;
       save_sys_info.bk_var.hrd_std_vars.stan_7_clear = 45861;      save_sys_info.bk_var.hrd_std_vars2.stan_7_clear = 45861;
		break;
	case SULPHUR://15-05-2026
       /* stan_0 - Blank (0 ppm) */
       save_sys_info.bk_var.hrd_std_vars.stan_0_red = 18994;        save_sys_info.bk_var.hrd_std_vars2.stan_0_red = 18994;
       save_sys_info.bk_var.hrd_std_vars.stan_0_green = 19019;      save_sys_info.bk_var.hrd_std_vars2.stan_0_green = 19019;
       save_sys_info.bk_var.hrd_std_vars.stan_0_blue = 21154;       save_sys_info.bk_var.hrd_std_vars2.stan_0_blue = 21154;
       save_sys_info.bk_var.hrd_std_vars.stan_0_clear = 53306;      save_sys_info.bk_var.hrd_std_vars2.stan_0_clear = 53306;

       /* stan_1 - Blank duplicate (same as stan_0) */
       save_sys_info.bk_var.hrd_std_vars.stan_1_red = 18994;        save_sys_info.bk_var.hrd_std_vars2.stan_1_red = 18994;
       save_sys_info.bk_var.hrd_std_vars.stan_1_green = 19019;      save_sys_info.bk_var.hrd_std_vars2.stan_1_green = 19019;
       save_sys_info.bk_var.hrd_std_vars.stan_1_blue = 21154;       save_sys_info.bk_var.hrd_std_vars2.stan_1_blue = 21154;
       save_sys_info.bk_var.hrd_std_vars.stan_1_clear = 53306;      save_sys_info.bk_var.hrd_std_vars2.stan_1_clear = 53306;

       /* stan_2 - 1.00 ppm */
       save_sys_info.bk_var.hrd_std_vars.stan_2_red = 18702;        save_sys_info.bk_var.hrd_std_vars2.stan_2_red = 18702;
       save_sys_info.bk_var.hrd_std_vars.stan_2_green = 18626;      save_sys_info.bk_var.hrd_std_vars2.stan_2_green = 18626;
       save_sys_info.bk_var.hrd_std_vars.stan_2_blue = 20685;       save_sys_info.bk_var.hrd_std_vars2.stan_2_blue = 20685;
       save_sys_info.bk_var.hrd_std_vars.stan_2_clear = 52283;      save_sys_info.bk_var.hrd_std_vars2.stan_2_clear = 52283;

       /* stan_3 - 5 ppm */
       save_sys_info.bk_var.hrd_std_vars.stan_3_red = 16999;        save_sys_info.bk_var.hrd_std_vars2.stan_3_red = 16999;
       save_sys_info.bk_var.hrd_std_vars.stan_3_green = 16769;      save_sys_info.bk_var.hrd_std_vars2.stan_3_green = 16769;
       save_sys_info.bk_var.hrd_std_vars.stan_3_blue = 18524;       save_sys_info.bk_var.hrd_std_vars2.stan_3_blue = 18524;
       save_sys_info.bk_var.hrd_std_vars.stan_3_clear = 47202;      save_sys_info.bk_var.hrd_std_vars2.stan_3_clear = 47202;

       /* stan_4 - 10 ppm */
       save_sys_info.bk_var.hrd_std_vars.stan_4_red = 15441;        save_sys_info.bk_var.hrd_std_vars2.stan_4_red = 15441;
       save_sys_info.bk_var.hrd_std_vars.stan_4_green = 15215;      save_sys_info.bk_var.hrd_std_vars2.stan_4_green = 15215;
       save_sys_info.bk_var.hrd_std_vars.stan_4_blue = 16753;       save_sys_info.bk_var.hrd_std_vars2.stan_4_blue = 16753;
       save_sys_info.bk_var.hrd_std_vars.stan_4_clear = 42886;      save_sys_info.bk_var.hrd_std_vars2.stan_4_clear = 42886;

       /* stan_5 - 20 ppm */
       save_sys_info.bk_var.hrd_std_vars.stan_5_red = 13027;        save_sys_info.bk_var.hrd_std_vars2.stan_5_red = 13027;
       save_sys_info.bk_var.hrd_std_vars.stan_5_green = 12591;      save_sys_info.bk_var.hrd_std_vars2.stan_5_green = 12591;
       save_sys_info.bk_var.hrd_std_vars.stan_5_blue = 13724;       save_sys_info.bk_var.hrd_std_vars2.stan_5_blue = 13724;
       save_sys_info.bk_var.hrd_std_vars.stan_5_clear = 35669;      save_sys_info.bk_var.hrd_std_vars2.stan_5_clear = 35669;

       /* stan_6 - 30 ppm */
       save_sys_info.bk_var.hrd_std_vars.stan_6_red = 11156;        save_sys_info.bk_var.hrd_std_vars2.stan_6_red = 11156;
       save_sys_info.bk_var.hrd_std_vars.stan_6_green = 10652;      save_sys_info.bk_var.hrd_std_vars2.stan_6_green = 10652;
       save_sys_info.bk_var.hrd_std_vars.stan_6_blue = 11524;       save_sys_info.bk_var.hrd_std_vars2.stan_6_blue = 11524;
       save_sys_info.bk_var.hrd_std_vars.stan_6_clear = 30284;      save_sys_info.bk_var.hrd_std_vars2.stan_6_clear = 30284;

       /* stan_7 - 50 ppm */
       save_sys_info.bk_var.hrd_std_vars.stan_7_red = 8092;         save_sys_info.bk_var.hrd_std_vars2.stan_7_red = 8092;
       save_sys_info.bk_var.hrd_std_vars.stan_7_green = 7519;       save_sys_info.bk_var.hrd_std_vars2.stan_7_green = 7519;
       save_sys_info.bk_var.hrd_std_vars.stan_7_blue = 8029;        save_sys_info.bk_var.hrd_std_vars2.stan_7_blue = 8029;
       save_sys_info.bk_var.hrd_std_vars.stan_7_clear = 21533;      save_sys_info.bk_var.hrd_std_vars2.stan_7_clear = 21533;
       break;
	case POTASSIUM:  //standard on date
              save_sys_info.bk_var.hrd_std_vars.stan_0_red = 19509;        save_sys_info.bk_var.hrd_std_vars2.stan_0_red = 19509;     /* stan_0: 0 ppm (blank) - Red channel */
		save_sys_info.bk_var.hrd_std_vars.stan_0_green = 19674;      save_sys_info.bk_var.hrd_std_vars2.stan_0_green = 19674;   /* stan_0: 0 ppm (blank) - Green channel */
		save_sys_info.bk_var.hrd_std_vars.stan_0_blue = 21081;       save_sys_info.bk_var.hrd_std_vars2.stan_0_blue = 21081;    /* stan_0: 0 ppm (blank) - Blue channel */
		save_sys_info.bk_var.hrd_std_vars.stan_0_clear = 55231;      save_sys_info.bk_var.hrd_std_vars2.stan_0_clear = 55231;   /* stan_0: 0 ppm (blank) - Clear channel */

		save_sys_info.bk_var.hrd_std_vars.stan_1_red = 19509;        save_sys_info.bk_var.hrd_std_vars2.stan_1_red = 19509;     /* stan_1: 0 ppm (repeat of stan_0) - Red channel */
		save_sys_info.bk_var.hrd_std_vars.stan_1_green = 19674;      save_sys_info.bk_var.hrd_std_vars2.stan_1_green = 19674;   /* stan_1: 0 ppm (repeat of stan_0) - Green channel */
		save_sys_info.bk_var.hrd_std_vars.stan_1_blue = 21081;       save_sys_info.bk_var.hrd_std_vars2.stan_1_blue = 21081;    /* stan_1: 0 ppm (repeat of stan_0) - Blue channel */
		save_sys_info.bk_var.hrd_std_vars.stan_1_clear = 55231;      save_sys_info.bk_var.hrd_std_vars2.stan_1_clear = 55231;   /* stan_1: 0 ppm (repeat of stan_0) - Clear channel */

		save_sys_info.bk_var.hrd_std_vars.stan_2_red = 18489;        save_sys_info.bk_var.hrd_std_vars2.stan_2_red = 18489;     /* stan_2: 2 ppm - Red channel */
		save_sys_info.bk_var.hrd_std_vars.stan_2_green = 18664;      save_sys_info.bk_var.hrd_std_vars2.stan_2_green = 18664;   /* stan_2: 2 ppm - Green channel */
		save_sys_info.bk_var.hrd_std_vars.stan_2_blue = 19997;       save_sys_info.bk_var.hrd_std_vars2.stan_2_blue = 19997;    /* stan_2: 2 ppm - Blue channel */
		save_sys_info.bk_var.hrd_std_vars.stan_2_clear = 52411;      save_sys_info.bk_var.hrd_std_vars2.stan_2_clear = 52411;   /* stan_2: 2 ppm - Clear channel */

		save_sys_info.bk_var.hrd_std_vars.stan_3_red = 14879;        save_sys_info.bk_var.hrd_std_vars2.stan_3_red = 14879;     /* stan_3: 4 ppm - Red channel */
		save_sys_info.bk_var.hrd_std_vars.stan_3_green = 15148;      save_sys_info.bk_var.hrd_std_vars2.stan_3_green = 15148;   /* stan_3: 4 ppm - Green channel */
		save_sys_info.bk_var.hrd_std_vars.stan_3_blue = 16205;       save_sys_info.bk_var.hrd_std_vars2.stan_3_blue = 16205;    /* stan_3: 4 ppm - Blue channel */
		save_sys_info.bk_var.hrd_std_vars.stan_3_clear = 42431;      save_sys_info.bk_var.hrd_std_vars2.stan_3_clear = 42431;   /* stan_3: 4 ppm - Clear channel */

		save_sys_info.bk_var.hrd_std_vars.stan_4_red = 12434;        save_sys_info.bk_var.hrd_std_vars2.stan_4_red = 12434;     /* stan_4: 6 ppm - Red channel */
		save_sys_info.bk_var.hrd_std_vars.stan_4_green = 12713;      save_sys_info.bk_var.hrd_std_vars2.stan_4_green = 12713;   /* stan_4: 6 ppm - Green channel */
		save_sys_info.bk_var.hrd_std_vars.stan_4_blue = 13558;       save_sys_info.bk_var.hrd_std_vars2.stan_4_blue = 13558;    /* stan_4: 6 ppm - Blue channel */
		save_sys_info.bk_var.hrd_std_vars.stan_4_clear = 35561;      save_sys_info.bk_var.hrd_std_vars2.stan_4_clear = 35561;   /* stan_4: 6 ppm - Clear channel */

		save_sys_info.bk_var.hrd_std_vars.stan_5_red = 10470;        save_sys_info.bk_var.hrd_std_vars2.stan_5_red = 10470;     /* stan_5: 8 ppm - Red channel */
		save_sys_info.bk_var.hrd_std_vars.stan_5_green = 10735;      save_sys_info.bk_var.hrd_std_vars2.stan_5_green = 10735;   /* stan_5: 8 ppm - Green channel */
		save_sys_info.bk_var.hrd_std_vars.stan_5_blue = 11418;       save_sys_info.bk_var.hrd_std_vars2.stan_5_blue = 11418;    /* stan_5: 8 ppm - Blue channel */
		save_sys_info.bk_var.hrd_std_vars.stan_5_clear = 30004;      save_sys_info.bk_var.hrd_std_vars2.stan_5_clear = 30004;   /* stan_5: 8 ppm - Clear channel */

		save_sys_info.bk_var.hrd_std_vars.stan_6_red = 9211;         save_sys_info.bk_var.hrd_std_vars2.stan_6_red = 9211;      /* stan_6: 10 ppm - Red channel */
		save_sys_info.bk_var.hrd_std_vars.stan_6_green = 9457;       save_sys_info.bk_var.hrd_std_vars2.stan_6_green = 9457;    /* stan_6: 10 ppm - Green channel */
		save_sys_info.bk_var.hrd_std_vars.stan_6_blue = 10033;       save_sys_info.bk_var.hrd_std_vars2.stan_6_blue = 10033;    /* stan_6: 10 ppm - Blue channel */
		save_sys_info.bk_var.hrd_std_vars.stan_6_clear = 26414;      save_sys_info.bk_var.hrd_std_vars2.stan_6_clear = 26414;   /* stan_6: 10 ppm - Clear channel */

		save_sys_info.bk_var.hrd_std_vars.stan_7_red = 5346;         save_sys_info.bk_var.hrd_std_vars2.stan_7_red = 5346;      /* stan_7: 20 ppm - Red channel */
		save_sys_info.bk_var.hrd_std_vars.stan_7_green = 5534;       save_sys_info.bk_var.hrd_std_vars2.stan_7_green = 5534;    /* stan_7: 20 ppm - Green channel */
		save_sys_info.bk_var.hrd_std_vars.stan_7_blue = 5824;        save_sys_info.bk_var.hrd_std_vars2.stan_7_blue = 5824;     /* stan_7: 20 ppm - Blue channel */
		save_sys_info.bk_var.hrd_std_vars.stan_7_clear = 15398;      save_sys_info.bk_var.hrd_std_vars2.stan_7_clear = 15398;   /* stan_7: 20 ppm - Clear channel */
       
	break;
	case PHOSPHORUS://29-04-2026 alkline							//19-05-2026 acidic
       /* stan_0 - Blank (0.00 ppm) */
       save_sys_info.bk_var.hrd_std_vars.stan_0_red   = 16604;  save_sys_info.bk_var.hrd_std_vars2.stan_0_red   = 16713;
       save_sys_info.bk_var.hrd_std_vars.stan_0_green = 15462;  save_sys_info.bk_var.hrd_std_vars2.stan_0_green = 15570;
       save_sys_info.bk_var.hrd_std_vars.stan_0_blue  = 18440;  save_sys_info.bk_var.hrd_std_vars2.stan_0_blue  = 18772;
       save_sys_info.bk_var.hrd_std_vars.stan_0_clear = 47486;  save_sys_info.bk_var.hrd_std_vars2.stan_0_clear = 47914;

       /* stan_1 - Blank duplicate (same as stan_0) */
       save_sys_info.bk_var.hrd_std_vars.stan_1_red   = 16604;  save_sys_info.bk_var.hrd_std_vars2.stan_1_red   = 16713;
       save_sys_info.bk_var.hrd_std_vars.stan_1_green = 15462;  save_sys_info.bk_var.hrd_std_vars2.stan_1_green = 15570;
       save_sys_info.bk_var.hrd_std_vars.stan_1_blue  = 18440;  save_sys_info.bk_var.hrd_std_vars2.stan_1_blue  = 18772;
       save_sys_info.bk_var.hrd_std_vars.stan_1_clear = 47486;  save_sys_info.bk_var.hrd_std_vars2.stan_1_clear = 47914;

       /* stan_2 - 0.10 ppm */
       save_sys_info.bk_var.hrd_std_vars.stan_2_red   = 14995;  save_sys_info.bk_var.hrd_std_vars2.stan_2_red   = 15262;
       save_sys_info.bk_var.hrd_std_vars.stan_2_green = 14440;  save_sys_info.bk_var.hrd_std_vars2.stan_2_green = 14729;
       save_sys_info.bk_var.hrd_std_vars.stan_2_blue  = 17352;  save_sys_info.bk_var.hrd_std_vars2.stan_2_blue  = 17901;
       save_sys_info.bk_var.hrd_std_vars.stan_2_clear = 43996;  save_sys_info.bk_var.hrd_std_vars2.stan_2_clear = 44955;

       /* stan_3 - 0.20 ppm */
       save_sys_info.bk_var.hrd_std_vars.stan_3_red   = 13285;  save_sys_info.bk_var.hrd_std_vars2.stan_3_red   = 13632;
       save_sys_info.bk_var.hrd_std_vars.stan_3_green = 13287;  save_sys_info.bk_var.hrd_std_vars2.stan_3_green = 13667;
       save_sys_info.bk_var.hrd_std_vars.stan_3_blue  = 16187;  save_sys_info.bk_var.hrd_std_vars2.stan_3_blue  = 16770;
       save_sys_info.bk_var.hrd_std_vars.stan_3_clear = 40162;  save_sys_info.bk_var.hrd_std_vars2.stan_3_clear = 41371;

       /* stan_4 - 0.40 ppm */
       save_sys_info.bk_var.hrd_std_vars.stan_4_red   = 10378;  save_sys_info.bk_var.hrd_std_vars2.stan_4_red   = 10755;
       save_sys_info.bk_var.hrd_std_vars.stan_4_green = 11221;  save_sys_info.bk_var.hrd_std_vars2.stan_4_green = 11699;
       save_sys_info.bk_var.hrd_std_vars.stan_4_blue  = 13916;  save_sys_info.bk_var.hrd_std_vars2.stan_4_blue  = 14661;
       save_sys_info.bk_var.hrd_std_vars.stan_4_clear = 33405;  save_sys_info.bk_var.hrd_std_vars2.stan_4_clear = 34847;

       /* stan_5 - 0.60 ppm */
       save_sys_info.bk_var.hrd_std_vars.stan_5_red   = 8011;   save_sys_info.bk_var.hrd_std_vars2.stan_5_red   = 8207;
       save_sys_info.bk_var.hrd_std_vars.stan_5_green = 9411;   save_sys_info.bk_var.hrd_std_vars2.stan_5_green = 9766;
       save_sys_info.bk_var.hrd_std_vars.stan_5_blue  = 11962;  save_sys_info.bk_var.hrd_std_vars2.stan_5_blue  = 12556;
       save_sys_info.bk_var.hrd_std_vars.stan_5_clear = 27648;  save_sys_info.bk_var.hrd_std_vars2.stan_5_clear = 28672;

       /* stan_6 - 0.80 ppm */
       save_sys_info.bk_var.hrd_std_vars.stan_6_red   = 6181;   save_sys_info.bk_var.hrd_std_vars2.stan_6_red   = 6370;
       save_sys_info.bk_var.hrd_std_vars.stan_6_green = 7914;   save_sys_info.bk_var.hrd_std_vars2.stan_6_green = 8293;
       save_sys_info.bk_var.hrd_std_vars.stan_6_blue  = 10319;  save_sys_info.bk_var.hrd_std_vars2.stan_6_blue  = 10927;
       save_sys_info.bk_var.hrd_std_vars.stan_6_clear = 22971;  save_sys_info.bk_var.hrd_std_vars2.stan_6_clear = 24034;

       /* stan_7 - 1.00 ppm */
       save_sys_info.bk_var.hrd_std_vars.stan_7_red   = 4516;   save_sys_info.bk_var.hrd_std_vars2.stan_7_red   = 4979;
       save_sys_info.bk_var.hrd_std_vars.stan_7_green = 6480;   save_sys_info.bk_var.hrd_std_vars2.stan_7_green = 7100;
       save_sys_info.bk_var.hrd_std_vars.stan_7_blue  = 8728;   save_sys_info.bk_var.hrd_std_vars2.stan_7_blue  = 9585;
       save_sys_info.bk_var.hrd_std_vars.stan_7_clear = 18561;  save_sys_info.bk_var.hrd_std_vars2.stan_7_clear = 20341;
	break;
	case NITROGEN: // 25-05-2026
        /* stan_0 - Blank (0 ppm) */
        save_sys_info.bk_var.hrd_std_vars.stan_0_red = 17073;        save_sys_info.bk_var.hrd_std_vars2.stan_0_red = 17073;
        save_sys_info.bk_var.hrd_std_vars.stan_0_green = 16682;      save_sys_info.bk_var.hrd_std_vars2.stan_0_green = 16682;
        save_sys_info.bk_var.hrd_std_vars.stan_0_blue = 17398;       save_sys_info.bk_var.hrd_std_vars2.stan_0_blue = 17398;
        save_sys_info.bk_var.hrd_std_vars.stan_0_clear = 48515;      save_sys_info.bk_var.hrd_std_vars2.stan_0_clear = 48515;

        /* stan_1 - Blank duplicate (same as stan_0) */
        save_sys_info.bk_var.hrd_std_vars.stan_1_red = 17073;        save_sys_info.bk_var.hrd_std_vars2.stan_1_red = 17073;
        save_sys_info.bk_var.hrd_std_vars.stan_1_green = 16682;      save_sys_info.bk_var.hrd_std_vars2.stan_1_green = 16682;
        save_sys_info.bk_var.hrd_std_vars.stan_1_blue = 17398;       save_sys_info.bk_var.hrd_std_vars2.stan_1_blue = 17398;
        save_sys_info.bk_var.hrd_std_vars.stan_1_clear = 48515;      save_sys_info.bk_var.hrd_std_vars2.stan_1_clear = 48515;

        /* stan_2 - 2 ppm */
        save_sys_info.bk_var.hrd_std_vars.stan_2_red = 16314;        save_sys_info.bk_var.hrd_std_vars2.stan_2_red = 16314;
        save_sys_info.bk_var.hrd_std_vars.stan_2_green = 13538;      save_sys_info.bk_var.hrd_std_vars2.stan_2_green = 13538;
        save_sys_info.bk_var.hrd_std_vars.stan_2_blue = 12862;       save_sys_info.bk_var.hrd_std_vars2.stan_2_blue = 12862;
        save_sys_info.bk_var.hrd_std_vars.stan_2_clear = 40536;      save_sys_info.bk_var.hrd_std_vars2.stan_2_clear = 40536;

        /* stan_3 - 4 ppm */
        save_sys_info.bk_var.hrd_std_vars.stan_3_red = 16101;        save_sys_info.bk_var.hrd_std_vars2.stan_3_red = 16101;
        save_sys_info.bk_var.hrd_std_vars.stan_3_green = 11230;      save_sys_info.bk_var.hrd_std_vars2.stan_3_green = 11230;
        save_sys_info.bk_var.hrd_std_vars.stan_3_blue = 9825;       save_sys_info.bk_var.hrd_std_vars2.stan_3_blue = 9825;
        save_sys_info.bk_var.hrd_std_vars.stan_3_clear = 35260;      save_sys_info.bk_var.hrd_std_vars2.stan_3_clear = 35260;

        /* stan_4 - 6 ppm */
        save_sys_info.bk_var.hrd_std_vars.stan_4_red = 15347;        save_sys_info.bk_var.hrd_std_vars2.stan_4_red = 15347;
        save_sys_info.bk_var.hrd_std_vars.stan_4_green = 9500;       save_sys_info.bk_var.hrd_std_vars2.stan_4_green = 9500;
        save_sys_info.bk_var.hrd_std_vars.stan_4_blue = 7923;       save_sys_info.bk_var.hrd_std_vars2.stan_4_blue = 7923;
        save_sys_info.bk_var.hrd_std_vars.stan_4_clear = 31112;      save_sys_info.bk_var.hrd_std_vars2.stan_4_clear = 31112;

        /* stan_5 - 8 ppm */
        save_sys_info.bk_var.hrd_std_vars.stan_5_red = 14752;        save_sys_info.bk_var.hrd_std_vars2.stan_5_red = 14752;
        save_sys_info.bk_var.hrd_std_vars.stan_5_green = 8199;       save_sys_info.bk_var.hrd_std_vars2.stan_5_green = 8199;
        save_sys_info.bk_var.hrd_std_vars.stan_5_blue = 6557;       save_sys_info.bk_var.hrd_std_vars2.stan_5_blue = 6557;
        save_sys_info.bk_var.hrd_std_vars.stan_5_clear = 28013;      save_sys_info.bk_var.hrd_std_vars2.stan_5_clear = 28013;

        /* stan_6 - 10 ppm */
        save_sys_info.bk_var.hrd_std_vars.stan_6_red = 13030;        save_sys_info.bk_var.hrd_std_vars2.stan_6_red = 13030;
        save_sys_info.bk_var.hrd_std_vars.stan_6_green = 6087;       save_sys_info.bk_var.hrd_std_vars2.stan_6_green = 6087;
        save_sys_info.bk_var.hrd_std_vars.stan_6_blue = 4636;        save_sys_info.bk_var.hrd_std_vars2.stan_6_blue = 4636;
        save_sys_info.bk_var.hrd_std_vars.stan_6_clear = 22535;      save_sys_info.bk_var.hrd_std_vars2.stan_6_clear = 22535;

        /* stan_7 - 10 ppm duplicate */
        save_sys_info.bk_var.hrd_std_vars.stan_7_red = 13030;        save_sys_info.bk_var.hrd_std_vars2.stan_7_red = 13030;
        save_sys_info.bk_var.hrd_std_vars.stan_7_green = 6087;       save_sys_info.bk_var.hrd_std_vars2.stan_7_green = 6087;
        save_sys_info.bk_var.hrd_std_vars.stan_7_blue = 4636;        save_sys_info.bk_var.hrd_std_vars2.stan_7_blue = 4636;
        save_sys_info.bk_var.hrd_std_vars.stan_7_clear = 22535;      save_sys_info.bk_var.hrd_std_vars2.stan_7_clear = 22535;
        break;
	case MASTER_SYS:
	case ORGANIC_CARBON:
       /* stan_0 - Blank (0.00%) */
       save_sys_info.bk_var.hrd_std_vars.stan_0_red = 17392;        save_sys_info.bk_var.hrd_std_vars2.stan_0_red = 17392;
       save_sys_info.bk_var.hrd_std_vars.stan_0_green = 3923;       save_sys_info.bk_var.hrd_std_vars2.stan_0_green = 3923;
       save_sys_info.bk_var.hrd_std_vars.stan_0_blue = 2377;        save_sys_info.bk_var.hrd_std_vars2.stan_0_blue = 2377;
       save_sys_info.bk_var.hrd_std_vars.stan_0_clear = 22557;      save_sys_info.bk_var.hrd_std_vars2.stan_0_clear = 22557;

       /* stan_1 - Blank duplicate (same as stan_0) */
       save_sys_info.bk_var.hrd_std_vars.stan_1_red = 17392;        save_sys_info.bk_var.hrd_std_vars2.stan_1_red = 17392;
       save_sys_info.bk_var.hrd_std_vars.stan_1_green = 3923;       save_sys_info.bk_var.hrd_std_vars2.stan_1_green = 3923;
       save_sys_info.bk_var.hrd_std_vars.stan_1_blue = 2377;        save_sys_info.bk_var.hrd_std_vars2.stan_1_blue = 2377;
       save_sys_info.bk_var.hrd_std_vars.stan_1_clear = 22557;      save_sys_info.bk_var.hrd_std_vars2.stan_1_clear = 22557;

       /* stan_2 - 0.50% */
       save_sys_info.bk_var.hrd_std_vars.stan_2_red = 15377;        save_sys_info.bk_var.hrd_std_vars2.stan_2_red = 15377;
       save_sys_info.bk_var.hrd_std_vars.stan_2_green = 3811;       save_sys_info.bk_var.hrd_std_vars2.stan_2_green = 3811;
       save_sys_info.bk_var.hrd_std_vars.stan_2_blue = 2187;        save_sys_info.bk_var.hrd_std_vars2.stan_2_blue = 2187;
       save_sys_info.bk_var.hrd_std_vars.stan_2_clear = 20359;      save_sys_info.bk_var.hrd_std_vars2.stan_2_clear = 20359;

       /* stan_3 - 1.00% */
       save_sys_info.bk_var.hrd_std_vars.stan_3_red = 13602;        save_sys_info.bk_var.hrd_std_vars2.stan_3_red = 13602;
       save_sys_info.bk_var.hrd_std_vars.stan_3_green = 3603;       save_sys_info.bk_var.hrd_std_vars2.stan_3_green = 3603;
       save_sys_info.bk_var.hrd_std_vars.stan_3_blue = 1990;        save_sys_info.bk_var.hrd_std_vars2.stan_3_blue = 1990;
       save_sys_info.bk_var.hrd_std_vars.stan_3_clear = 18297;      save_sys_info.bk_var.hrd_std_vars2.stan_3_clear = 18297;

       /* stan_4 - 1.50% */
       save_sys_info.bk_var.hrd_std_vars.stan_4_red = 12171;        save_sys_info.bk_var.hrd_std_vars2.stan_4_red = 12171;
       save_sys_info.bk_var.hrd_std_vars.stan_4_green = 3668;       save_sys_info.bk_var.hrd_std_vars2.stan_4_green = 3668;
       save_sys_info.bk_var.hrd_std_vars.stan_4_blue = 1900;        save_sys_info.bk_var.hrd_std_vars2.stan_4_blue = 1900;
       save_sys_info.bk_var.hrd_std_vars.stan_4_clear = 16911;      save_sys_info.bk_var.hrd_std_vars2.stan_4_clear = 16911;

       /* stan_5 - 2.00% */
       save_sys_info.bk_var.hrd_std_vars.stan_5_red = 11292;        save_sys_info.bk_var.hrd_std_vars2.stan_5_red = 11292;
       save_sys_info.bk_var.hrd_std_vars.stan_5_green = 3864;       save_sys_info.bk_var.hrd_std_vars2.stan_5_green = 3864;
       save_sys_info.bk_var.hrd_std_vars.stan_5_blue = 1898;        save_sys_info.bk_var.hrd_std_vars2.stan_5_blue = 1898;
       save_sys_info.bk_var.hrd_std_vars.stan_5_clear = 16263;      save_sys_info.bk_var.hrd_std_vars2.stan_5_clear = 16263;

       /* stan_6 - 2.50% */
       save_sys_info.bk_var.hrd_std_vars.stan_6_red = 10173;        save_sys_info.bk_var.hrd_std_vars2.stan_6_red = 10173;
       save_sys_info.bk_var.hrd_std_vars.stan_6_green = 3641;       save_sys_info.bk_var.hrd_std_vars2.stan_6_green = 3641;
       save_sys_info.bk_var.hrd_std_vars.stan_6_blue = 1752;        save_sys_info.bk_var.hrd_std_vars2.stan_6_blue = 1752;
       save_sys_info.bk_var.hrd_std_vars.stan_6_clear = 14847;      save_sys_info.bk_var.hrd_std_vars2.stan_6_clear = 14847;

       /* stan_7 - 3.00% */
       save_sys_info.bk_var.hrd_std_vars.stan_7_red = 9693;         save_sys_info.bk_var.hrd_std_vars2.stan_7_red = 9693;
       save_sys_info.bk_var.hrd_std_vars.stan_7_green = 3653;       save_sys_info.bk_var.hrd_std_vars2.stan_7_green = 3653;
       save_sys_info.bk_var.hrd_std_vars.stan_7_blue = 1726;        save_sys_info.bk_var.hrd_std_vars2.stan_7_blue = 1726;
       save_sys_info.bk_var.hrd_std_vars.stan_7_clear = 14375;      save_sys_info.bk_var.hrd_std_vars2.stan_7_clear = 14375;
		break;
	}
}
#endif
