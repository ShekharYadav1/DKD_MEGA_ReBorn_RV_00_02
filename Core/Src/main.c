/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <math.h>
#include "my_struct.h"
//#include "Sen_AS7343.h"
#include "Sen_TCS34725.h"
#include "RGB_led.h"
#include "GUI_Comm.h"
#include "Pump.h"
#include "Memory.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define ARC_CAL
//#define OLD_CAL

#define TM_MULTIPLIER	20

#define FLASH_OVER_WRITE
#define MEM_ID1 0xAA
#define MEM_ID2 0x55
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
sys_var_typdef sys_info;
union_bk_var_typdef save_sys_info;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void sys_mem_validate(void);
void auto_zero_adjust(unn_std_var_typdef* opt_std_vars);
//void value_calculation2(void);
//void value_calculation(void);
void cal_result(void);
void auto_set_adjust(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();//motors
  MX_I2C1_Init();//sensor
  MX_TIM3_Init();//led pwm
  MX_USART2_UART_Init();//uart rs485
  /* USER CODE BEGIN 2 */
  //HAL_Delay(100);
  Flash_Read_Data(MEM_STRT_ADD , save_sys_info.bkdata, MEM_SIZE);
  sys_mem_validate();
  TCS3472X_init(&hi2c1);
  RGB_Led_init(&htim3);
  GUI_Comm_init(&huart2);
  pump1(0);pump2(0);pump3(0);pump4(0);pump5(0);pump6(0);pump7(0);pump8(0);motor1(0);
  //PUMP1OFF;PUMP2OFF;PUMP3OFF;PUMP4OFF;PUMP5OFF;PUMP6OFF;PUMP7OFF;
  //PUMP8OFF;MOTOR1OFF;//pump & motors
  set_Chnl_PWM( sys_info.led_pwm_red, sys_info.led_pwm_green, sys_info.led_pwm_blue);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  if( (sys_info.sys_f.mem_save == 1) && (sys_info.sys_f.comm_mode == RCV_MODE) )
	  {Flash_Write_Data(MEM_STRT_ADD, save_sys_info.bkdata, MEM_SIZE); sys_info.sys_f.mem_save = 0;}
	  TCS3472X_handler();
	  get_sen_stat() ? ( sys_info.Stat_L.sen_stat |= 1 ) : ( sys_info.Stat_L.sen_stat &= 0 );
	  if(sys_info.Stat_L.sen_stat)
	  {
		  get_mavg_rgbc_val(&sys_info.curr_rgbc_vars.curr_red_rcv, &sys_info.curr_rgbc_vars.curr_green_rcv, &sys_info.curr_rgbc_vars.curr_blue_rcv, &sys_info.curr_rgbc_vars.curr_clear_rcv);
		  if(!sys_info.Stat_L.warm_up_stat)
		  {
			  if(sys_info.Stat_M.tak_data_stat)
			  {
				  if(sys_info.tak_data_skp_run_tm >= sys_info.tak_data_skp_tm)
				  {
					  if( ( !sys_info.curr_ResVal) && (!sys_info.curr_Result_cat) )
					  {
						  cal_result();
					  }
					  //sys_info.Stat_L.drain_wsh_stat = 1;
					  sys_info.Stat_M.tak_data_stat = 0;
					  sys_info.Stat_L.alt_func = 0;
					  sys_info.tak_data_skp_run_tm = 0;
				  }
			  }
			  if(sys_info.Stat_L.auto_zero)
			  {
				  if(sys_info.auto_zero_skp_run_tm >= sys_info.auto_zero_skp_tm)
				  {
#ifdef ARC_CAL
					  if(sys_info.Stat_L.alt_func == 0)
					  {
						  auto_zero_adjust(&sys_info.opt_std_vars);
						  auto_zero_adjust(&sys_info.opt_std_vars2);
					  }
					  else if(sys_info.Stat_L.alt_func == 1)
					  {
						  auto_zero_adjust(&sys_info.opt_std_vars);
						  auto_zero_adjust(&sys_info.opt_std_vars2);
						  sys_info.Stat_L.alt_func = 0;
					  }
#endif
#ifdef OLD_CAL
					  auto_zero_adjust(&sys_info.opt_std_vars);
					  sys_info.Stat_L.alt_func = 0;
#endif
					  sys_info.Stat_L.auto_zero_save = 1;
					  sys_info.Stat_L.alt_func = 0;
					  sys_info.Stat_L.auto_zero = 0;
					  //sys_info.Stat_L.drain_wsh_stat = 1;
					  sys_info.auto_zero_skp_run_tm = 0;
					  sys_info.curr_ResVal = 0;
					  sys_info.curr_Result_cat = 0;
				  }
			  }
			  /*if(sys_info.Stat_M2.auto_set)
			  {
				  if(sys_info.auto_set_skp_run_tm >= sys_info.auto_set_skp_tm)
				  {
					  auto_set_adjust();
					  //sys_info.Stat_L.drain_wsh_stat = 1;
					  sys_info.Stat_M2.auto_set = 0;
					  sys_info.auto_set_skp_run_tm = 0;
				  }
			  }*/
		  }
		  else
		  {
			  sys_info.Stat_M.tak_data_stat = 0;
			  sys_info.Stat_L.auto_zero = 0;
		  }
	  }
	  GUI_Comm_handler();
	  pump_handler();

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void auto_set_adjust(void)
{
	float factor;
	factor = (float)sys_info.curr_rgbc_vars.curr_red_rcv/(float)sys_info.opt_std_vars.stan_4_red;
	sys_info.opt_std_vars.stan_4_red = sys_info.curr_rgbc_vars.curr_red_rcv;
	for(uint8_t i = 1; i < NOS_STD; i++)
	{
		if(i == 4)
		{continue;}
		else
		{
			sys_info.opt_std_vars.strd_vars[i][0] =
			(uint16_t)(factor * (float)sys_info.opt_std_vars.strd_vars[i][0]);
		}
	}

	factor = (float)sys_info.curr_rgbc_vars.curr_green_rcv/(float)sys_info.opt_std_vars.stan_4_green;
	sys_info.opt_std_vars.stan_4_green = sys_info.curr_rgbc_vars.curr_green_rcv;
	for(uint8_t i = 1; i < NOS_STD; i++)
	{
		if(i == 4)
		{continue;}
		else
		{
			sys_info.opt_std_vars.strd_vars[i][1] =
				(uint16_t)(factor * (float)sys_info.opt_std_vars.strd_vars[i][1]);
		}
	}


	factor = (float)sys_info.curr_rgbc_vars.curr_blue_rcv/(float)sys_info.opt_std_vars.stan_4_blue;
	sys_info.opt_std_vars.stan_4_blue = sys_info.curr_rgbc_vars.curr_blue_rcv;
	for(uint8_t i = 1; i < NOS_STD; i++)
	{
		if(i == 4)
		{continue;}
		else
		{
			sys_info.opt_std_vars.strd_vars[i][2] =
				(uint16_t)(factor * (float)sys_info.opt_std_vars.strd_vars[i][2]);
		}
	}


	factor = (float)sys_info.curr_rgbc_vars.curr_clear_rcv/(float)sys_info.opt_std_vars.stan_4_clear;
	sys_info.opt_std_vars.stan_4_clear = sys_info.curr_rgbc_vars.curr_clear_rcv;
	for(uint8_t i = 1; i < NOS_STD; i++)
	{
		if(i == 4)
		{continue;}
		else
		{
			sys_info.opt_std_vars.strd_vars[i][3] =
				(uint16_t)(factor * (float)sys_info.opt_std_vars.strd_vars[i][3]);
		}
	}
}

void value_calculation2(unn_std_var_typdef* opt_std_vars)
{
	//sys_info.curr_rgbc_vars.curr_red_rcv = 9901;
	//sys_info.curr_rgbc_vars.curr_green_rcv = 10660;
	//sys_info.curr_rgbc_vars.curr_blue_rcv = 13163;
	//sys_info.curr_rgbc_vars.curr_clear_rcv = 3172;

	sys_info.curr_absrb_val = log10( ((double)opt_std_vars->strd_vars[0][sys_info.val_cal_y] * (double)sys_info.curr_rgbc_vars.curr_rgbc_var[sys_info.val_cal_x] )
			/ ((double)opt_std_vars->strd_vars[0][sys_info.val_cal_x] * (double)sys_info.curr_rgbc_vars.curr_rgbc_var[sys_info.val_cal_y]));
	for(uint8_t i = 0; i < NOS_STD; i++)
	{
		sys_info.std_absrb_val[i] = log10( ((double)opt_std_vars->strd_vars[0][sys_info.val_cal_y] * (double)opt_std_vars->strd_vars[i][sys_info.val_cal_x] )
			/ ((double)opt_std_vars->strd_vars[0][sys_info.val_cal_x] * (double)opt_std_vars->strd_vars[i][sys_info.val_cal_y]));
	}
	//log10( ($G$8 * J12)/($F$8 *K12))

	if(sys_info.curr_absrb_val >= sys_info.std_absrb_val[0])
	{
		sys_info.curr_ResVal = 0;sys_info.curr_Result_cat = 1;
	}
	else if (sys_info.curr_absrb_val >= sys_info.std_absrb_val[1])
	{
		sys_info.curr_ResVal = ((sys_info.curr_absrb_val / sys_info.std_absrb_val[1])
			* sys_info.act_stan_vals[1]);
		sys_info.curr_Result_cat = 1;           //////////////VERRY LOW
	}
	else if (sys_info.curr_absrb_val >= sys_info.std_absrb_val[2])
	{
		sys_info.curr_ResVal = ( ( (sys_info.curr_absrb_val - sys_info.std_absrb_val[1]) / (sys_info.std_absrb_val[2] - sys_info.std_absrb_val[1]) )
			* (sys_info.act_stan_vals[2] - sys_info.act_stan_vals[1]) + sys_info.act_stan_vals[1]);
		sys_info.curr_Result_cat = 1;           //////////////VERRY LOW
	}
	else if (sys_info.curr_absrb_val >= sys_info.std_absrb_val[3])
	{
		sys_info.curr_ResVal = ( ( (sys_info.curr_absrb_val - sys_info.std_absrb_val[2]) / (sys_info.std_absrb_val[3] - sys_info.std_absrb_val[2]) )
			* (sys_info.act_stan_vals[3] - sys_info.act_stan_vals[2]) + sys_info.act_stan_vals[2]);
		sys_info.curr_Result_cat = 2;           //////////////LOW
	}
	else if (sys_info.curr_absrb_val >= sys_info.std_absrb_val[4])
	{
		sys_info.curr_ResVal = ( ( (sys_info.curr_absrb_val - sys_info.std_absrb_val[3]) / (sys_info.std_absrb_val[4] - sys_info.std_absrb_val[3]) )
			* (sys_info.act_stan_vals[4] - sys_info.act_stan_vals[3]) + sys_info.act_stan_vals[3]);
		sys_info.curr_Result_cat = 2;           //////////////LOW
	}
	else if (sys_info.curr_absrb_val >= sys_info.std_absrb_val[5])
	{
		sys_info.curr_ResVal = ( ( (sys_info.curr_absrb_val - sys_info.std_absrb_val[4]) / (sys_info.std_absrb_val[5] - sys_info.std_absrb_val[4]) )
			* (sys_info.act_stan_vals[5] - sys_info.act_stan_vals[4]) + sys_info.act_stan_vals[4]);
		sys_info.curr_Result_cat = 3;           //////////////Medium
	}
	else if (sys_info.curr_absrb_val >= sys_info.std_absrb_val[6])
	{
		sys_info.curr_ResVal = ( ( (sys_info.curr_absrb_val - sys_info.std_absrb_val[5]) / (sys_info.std_absrb_val[6] - sys_info.std_absrb_val[5]) )
			* (sys_info.act_stan_vals[6] - sys_info.act_stan_vals[5]) + sys_info.act_stan_vals[5]);
		sys_info.curr_Result_cat = 3;           //////////////Medium
	}
	else if (sys_info.curr_absrb_val >= sys_info.std_absrb_val[7])
	{
		sys_info.curr_ResVal = ( ( (sys_info.curr_absrb_val - sys_info.std_absrb_val[6]) / (sys_info.std_absrb_val[7] - sys_info.std_absrb_val[6]) )
			* (sys_info.act_stan_vals[7] - sys_info.act_stan_vals[6]) + sys_info.act_stan_vals[6]);
		sys_info.curr_Result_cat = 4;           //////////////High
	}
	else if (sys_info.curr_absrb_val >= sys_info.std_absrb_val[8])
	{
		sys_info.curr_ResVal = ( ( (sys_info.curr_absrb_val - sys_info.std_absrb_val[7]) / (sys_info.std_absrb_val[8] - sys_info.std_absrb_val[7]) )
			* (sys_info.act_stan_vals[8] - sys_info.act_stan_vals[7]) + sys_info.act_stan_vals[7]);
		sys_info.curr_Result_cat = 4;           //////////////High
	}
	else if (sys_info.curr_absrb_val >= sys_info.std_absrb_val[9])
	{
		sys_info.curr_ResVal = ( ( (sys_info.curr_absrb_val - sys_info.std_absrb_val[8]) / (sys_info.std_absrb_val[9] - sys_info.std_absrb_val[8]) )
			* (sys_info.act_stan_vals[9] - sys_info.act_stan_vals[8]) + sys_info.act_stan_vals[8]);
		sys_info.curr_Result_cat = 5;           //////////////Verry High
	}
	else if (sys_info.curr_absrb_val >= sys_info.std_absrb_val[10])
	{
		sys_info.curr_ResVal = ( ( (sys_info.curr_absrb_val - sys_info.std_absrb_val[9]) / (sys_info.std_absrb_val[10] - sys_info.std_absrb_val[9]) )
			* (sys_info.act_stan_vals[10] - sys_info.act_stan_vals[9]) + sys_info.act_stan_vals[9]);
		sys_info.curr_Result_cat = 5;           //////////////Verry High
	}
	else if (sys_info.curr_absrb_val < sys_info.std_absrb_val[10])
	{
		sys_info.curr_ResVal = (sys_info.act_stan_vals[10]/sys_info.std_absrb_val[10])
			* (sys_info.curr_absrb_val);
		sys_info.curr_Result_cat = 5;           //////////////Verry High
		//sys_info.curr_ResVal = sys_info.act_stan_vals[10];
		//sys_info.curr_Result_cat = 5;           //////////////Verry High
	}
}

void value_calculation(unn_std_var_typdef* opt_std_vars)
{
	//uint32_t sum_x, sum_y, sum_xy, sum_xx;
	sys_info.sum_x = 0; sys_info.sum_y = 0; sys_info.sum_xy = 0; sys_info.sum_xx = 0;
	uint8_t i;
	for(i = 0; i < NOS_STD; i++)
	{
		sys_info.sum_x += opt_std_vars->strd_vars[i][sys_info.val_cal_x];
		sys_info.sum_y += opt_std_vars->strd_vars[i][sys_info.val_cal_y];
		sys_info.sum_xy += opt_std_vars->strd_vars[i][sys_info.val_cal_x] * opt_std_vars->strd_vars[i][sys_info.val_cal_y];
		sys_info.sum_xx += opt_std_vars->strd_vars[i][sys_info.val_cal_x] * opt_std_vars->strd_vars[i][sys_info.val_cal_x];
	}
	sys_info.mslp = (((double)NOS_STD * (double)sys_info.sum_xy)
			- ((double)sys_info.sum_x * (double)sys_info.sum_y))
			/(((double)NOS_STD * (double)sys_info.sum_xx)
			- ((double)sys_info.sum_x * (double)sys_info.sum_x));

	sys_info.cshft = ((double)sys_info.sum_y - (sys_info.mslp * (double)sys_info.sum_x)) / NOS_STD;

	for(uint8_t i = 0; i < NOS_STD; i++)
	{
		sys_info.ref_stan_x[i] = ((sys_info.mslp * ((double)opt_std_vars->strd_vars[i][sys_info.val_cal_y] - sys_info.cshft))
			+ (double)opt_std_vars->strd_vars[i][sys_info.val_cal_x]) / ((sys_info.mslp * sys_info.mslp) + 1);
		sys_info.ref_stan_y[i] = (sys_info.mslp * sys_info.ref_stan_x[i]) + sys_info.cshft;
		sys_info.ref_stan_dist[i] = sqrt(((sys_info.ref_stan_x[0] - sys_info.ref_stan_x[i]) * (sys_info.ref_stan_x[0] - sys_info.ref_stan_x[i]))
			+ ((sys_info.ref_stan_y[0] - sys_info.ref_stan_y[i]) * (sys_info.ref_stan_y[0] - sys_info.ref_stan_y[i])));
	}

	//sys_info.curr_red_rcv = 16174;
	//sys_info.curr_green_rcv = 12952;
	//sys_info.curr_blue_rcv = 14442;
	//sys_info.curr_clear_rcv = 45558;
	sys_info.curr_x = ((sys_info.mslp * ((double)sys_info.curr_rgbc_vars.curr_rgbc_var[sys_info.val_cal_y] - sys_info.cshft))
		+ (double)sys_info.curr_rgbc_vars.curr_rgbc_var[sys_info.val_cal_x]) / ((sys_info.mslp * sys_info.mslp) + 1);
	sys_info.curr_y = (sys_info.mslp * sys_info.curr_x) + sys_info.cshft;
	sys_info.curr_dist = sqrt(((sys_info.ref_stan_x[0] - sys_info.curr_x) * (sys_info.ref_stan_x[0] - sys_info.curr_x))
		+ ((sys_info.ref_stan_y[0] - sys_info.curr_y) * (sys_info.ref_stan_y[0] - sys_info.curr_y)));

	if(sys_info.curr_dist <= sys_info.ref_stan_dist[0])
	{
		sys_info.curr_ResVal = 0;sys_info.curr_Result_cat = 1;
	}
	else if (sys_info.curr_dist <= sys_info.ref_stan_dist[1])
	{
		sys_info.curr_ResVal = ((sys_info.curr_dist / sys_info.ref_stan_dist[1])
			* sys_info.act_stan_vals[1]);
		sys_info.curr_Result_cat = 1;           //////////////VERRY LOW
	}
	else if (sys_info.curr_dist <= sys_info.ref_stan_dist[2])
	{
		sys_info.curr_ResVal = ( ( (sys_info.curr_dist - sys_info.ref_stan_dist[1]) / (sys_info.ref_stan_dist[2] - sys_info.ref_stan_dist[1]) )
			* (sys_info.act_stan_vals[2] - sys_info.act_stan_vals[1]) + sys_info.act_stan_vals[1]);
		sys_info.curr_Result_cat = 1;           //////////////VERRY LOW
	}
	else if (sys_info.curr_dist <= sys_info.ref_stan_dist[3])
	{
		sys_info.curr_ResVal = ( ( (sys_info.curr_dist - sys_info.ref_stan_dist[2]) / (sys_info.ref_stan_dist[3] - sys_info.ref_stan_dist[2]) )
			* (sys_info.act_stan_vals[3] - sys_info.act_stan_vals[2]) + sys_info.act_stan_vals[2]);
		sys_info.curr_Result_cat = 2;           //////////////LOW
	}
	else if (sys_info.curr_dist <= sys_info.ref_stan_dist[4])
	{
		sys_info.curr_ResVal = ( ( (sys_info.curr_dist - sys_info.ref_stan_dist[3]) / (sys_info.ref_stan_dist[4] - sys_info.ref_stan_dist[3]) )
			* (sys_info.act_stan_vals[4] - sys_info.act_stan_vals[3]) + sys_info.act_stan_vals[3]);
		sys_info.curr_Result_cat = 2;           //////////////LOW
	}
	else if (sys_info.curr_dist <= sys_info.ref_stan_dist[5])
	{
		sys_info.curr_ResVal = ( ( (sys_info.curr_dist - sys_info.ref_stan_dist[4]) / (sys_info.ref_stan_dist[5] - sys_info.ref_stan_dist[4]) )
			* (sys_info.act_stan_vals[5] - sys_info.act_stan_vals[4]) + sys_info.act_stan_vals[4]);
		sys_info.curr_Result_cat = 3;           //////////////Medium
	}
	else if (sys_info.curr_dist <= sys_info.ref_stan_dist[6])
	{
		sys_info.curr_ResVal = ( ( (sys_info.curr_dist - sys_info.ref_stan_dist[5]) / (sys_info.ref_stan_dist[6] - sys_info.ref_stan_dist[5]) )
			* (sys_info.act_stan_vals[6] - sys_info.act_stan_vals[5]) + sys_info.act_stan_vals[5]);
		sys_info.curr_Result_cat = 3;           //////////////Medium
	}
	else if (sys_info.curr_dist <= sys_info.ref_stan_dist[7])
	{
		sys_info.curr_ResVal = ( ( (sys_info.curr_dist - sys_info.ref_stan_dist[6]) / (sys_info.ref_stan_dist[7] - sys_info.ref_stan_dist[6]) )
			* (sys_info.act_stan_vals[7] - sys_info.act_stan_vals[6]) + sys_info.act_stan_vals[6]);
		sys_info.curr_Result_cat = 4;           //////////////High
	}
	else if (sys_info.curr_dist <= sys_info.ref_stan_dist[8])
	{
		sys_info.curr_ResVal = ( ( (sys_info.curr_dist - sys_info.ref_stan_dist[7]) / (sys_info.ref_stan_dist[8] - sys_info.ref_stan_dist[7]) )
			* (sys_info.act_stan_vals[8] - sys_info.act_stan_vals[7]) + sys_info.act_stan_vals[7]);
		sys_info.curr_Result_cat = 4;           //////////////High
	}
	else if (sys_info.curr_dist <= sys_info.ref_stan_dist[9])
	{
		sys_info.curr_ResVal = ( ( (sys_info.curr_dist - sys_info.ref_stan_dist[8]) / (sys_info.ref_stan_dist[9] - sys_info.ref_stan_dist[8]) )
			* (sys_info.act_stan_vals[9] - sys_info.act_stan_vals[8]) + sys_info.act_stan_vals[8]);
		sys_info.curr_Result_cat = 5;           //////////////Verry High
	}
	else if (sys_info.curr_dist <= sys_info.ref_stan_dist[10])
	{
		sys_info.curr_ResVal = ( ( (sys_info.curr_dist - sys_info.ref_stan_dist[9]) / (sys_info.ref_stan_dist[10] - sys_info.ref_stan_dist[9]) )
			* (sys_info.act_stan_vals[10] - sys_info.act_stan_vals[9]) + sys_info.act_stan_vals[9]);
		sys_info.curr_Result_cat = 5;           //////////////Verry High
	}
	else if (sys_info.curr_dist > sys_info.ref_stan_dist[10])
	{
		sys_info.curr_ResVal = ((sys_info.act_stan_vals[10]/sys_info.ref_stan_dist[10])
			* (sys_info.curr_dist - sys_info.ref_stan_dist[10]))
			+ sys_info.act_stan_vals[10];
		sys_info.curr_Result_cat = 5;           //////////////Verry High
		//sys_info.curr_ResVal = sys_info.act_stan_vals[10];
		//sys_info.curr_Result_cat = 5;           //////////////Verry High
	}
}

void value_calculation3(unn_std_var_typdef* opt_std_vars)
{
	double absrb0, absrb1, absrb2, absrb3, avg_absrb;
	double std_absrb_val_mul_sum, std_absrb_sqr_sum;
	std_absrb_val_mul_sum = 0; std_absrb_sqr_sum = 0;
	sys_info.std_multplr = 0;
	for(uint8_t i = 0; i < NOS_STD; i++)
	{
		absrb0 = log10((double)opt_std_vars->stan_0_red / (double)opt_std_vars->strd_vars[i][0]);
		absrb1 = log10((double)opt_std_vars->stan_0_green / (double)opt_std_vars->strd_vars[i][1]);
		absrb2 = log10((double)opt_std_vars->stan_0_blue / (double)opt_std_vars->strd_vars[i][2]);
		absrb3 = log10((double)opt_std_vars->stan_0_clear / (double)opt_std_vars->strd_vars[i][3]);
		avg_absrb = (absrb0 + absrb1 + absrb2 + absrb3) / 4.00;
		//sys_info.std_absrb_val_mul[i] = (avg_absrb * (double)sys_info.act_stan_vals[i]);
		//sys_info.std_absrb_sqr[i] = (avg_absrb * avg_absrb);
		std_absrb_val_mul_sum = std_absrb_val_mul_sum + (avg_absrb * (double)sys_info.act_stan_vals[i]);
		std_absrb_sqr_sum = std_absrb_sqr_sum + (avg_absrb * avg_absrb);
	}
	sys_info.std_multplr = std_absrb_val_mul_sum / std_absrb_sqr_sum;

	absrb0 = log10((double)opt_std_vars->stan_0_red / (double)sys_info.curr_rgbc_vars.curr_red_rcv);
	absrb1 = log10((double)opt_std_vars->stan_0_green / (double)sys_info.curr_rgbc_vars.curr_green_rcv);
	absrb2 = log10((double)opt_std_vars->stan_0_blue / (double)sys_info.curr_rgbc_vars.curr_blue_rcv);
	absrb3 = log10((double)opt_std_vars->stan_0_clear / (double)sys_info.curr_rgbc_vars.curr_clear_rcv);
	avg_absrb = (absrb0 + absrb1 + absrb2 + absrb3) / 4;

	sys_info.curr_ResVal = avg_absrb * sys_info.std_multplr;

	if(sys_info.curr_ResVal <= sys_info.act_stan_vals[0])
	{
		sys_info.curr_ResVal = sys_info.act_stan_vals[0];
		sys_info.curr_Result_cat = 1;
	}
	else if(sys_info.curr_ResVal <= sys_info.act_stan_vals[1])
	{
		sys_info.curr_Result_cat = 1;           //////////////VERRY LOW
	}
	else if(sys_info.curr_ResVal <= sys_info.act_stan_vals[2])
	{
		sys_info.curr_Result_cat = 1;           //////////////VERRY LOW
	}
	else if(sys_info.curr_ResVal <= sys_info.act_stan_vals[3])
	{
		sys_info.curr_Result_cat = 2;           //////////////LOW
	}
	else if(sys_info.curr_ResVal <= sys_info.act_stan_vals[4])
	{
		sys_info.curr_Result_cat = 2;           //////////////LOW
	}
	else if(sys_info.curr_ResVal <= sys_info.act_stan_vals[5])
	{
		sys_info.curr_Result_cat = 3;           //////////////Medium
	}
	else if(sys_info.curr_ResVal <= sys_info.act_stan_vals[6])
	{
		sys_info.curr_Result_cat = 3;           //////////////Medium
	}
	else if(sys_info.curr_ResVal <= sys_info.act_stan_vals[7])
	{
		sys_info.curr_Result_cat = 4;           //////////////High
	}
	else if(sys_info.curr_ResVal <= sys_info.act_stan_vals[8])
	{
		sys_info.curr_Result_cat = 4;           //////////////High
	}
	else if(sys_info.curr_ResVal <= sys_info.act_stan_vals[9])
	{
		sys_info.curr_Result_cat = 5;           //////////////Verry High
	}
	else if(sys_info.curr_ResVal <= sys_info.act_stan_vals[10])
	{
		sys_info.curr_Result_cat = 5;           //////////////Verry High
	}
	else if(sys_info.curr_ResVal > sys_info.act_stan_vals[10])
	{
		sys_info.curr_Result_cat = 5;           //////////////Verry High
		//sys_info.curr_ResVal = sys_info.act_stan_vals[10];
		//sys_info.curr_Result_cat = 5;           //////////////Verry High
	}

}

void cal_result(void)
{

#ifdef ARC_CAL
	if(sys_info.Stat_L.alt_func == 0)
	{
		switch(sys_info.set_sys_add)
		{
		case SULPHUR:value_calculation(&sys_info.opt_std_vars);break;
		case PHOSPHORUS:value_calculation2(&sys_info.opt_std_vars);break;
		case NITROGEN:value_calculation3(&sys_info.opt_std_vars);break;
		default:value_calculation(&sys_info.opt_std_vars);
		  //value_calculation(&sys_info.opt_std_vars);
		break;
		}
	}
	else if(sys_info.Stat_L.alt_func == 1)
	{
		switch(sys_info.set_sys_add)
		{
		case SULPHUR:value_calculation(&sys_info.opt_std_vars2);break;
		case PHOSPHORUS:value_calculation2(&sys_info.opt_std_vars2);break;
		case NITROGEN:value_calculation3(&sys_info.opt_std_vars2);break;
		default:value_calculation(&sys_info.opt_std_vars2);
		  //value_calculation(&sys_info.opt_std_vars2);
		break;
		}
		sys_info.Stat_L.alt_func = 0;
	}
#endif
#ifdef OLD_CAL
	value_calculation(&sys_info.opt_std_vars);
	sys_info.Stat_L.alt_func = 0;
#endif
}

void auto_zero_adjust(unn_std_var_typdef* opt_std_vars)
{
	float factor;
	factor = (float)sys_info.curr_rgbc_vars.curr_red_rcv/(float)opt_std_vars->stan_0_red;
	opt_std_vars->stan_0_red = sys_info.curr_rgbc_vars.curr_red_rcv;
	for(uint8_t i = 1; i < NOS_STD; i++)
	{opt_std_vars->strd_vars[i][0] =
	(uint16_t)(factor * (float)opt_std_vars->strd_vars[i][0]);}

	factor = (float)sys_info.curr_rgbc_vars.curr_green_rcv/(float)opt_std_vars->stan_0_green;
	opt_std_vars->stan_0_green = sys_info.curr_rgbc_vars.curr_green_rcv;
	for(uint8_t i = 1; i < NOS_STD; i++)
	{opt_std_vars->strd_vars[i][1] =
	(uint16_t)(factor * (float)opt_std_vars->strd_vars[i][1]);}

	factor = (float)sys_info.curr_rgbc_vars.curr_blue_rcv/(float)opt_std_vars->stan_0_blue;
	opt_std_vars->stan_0_blue = sys_info.curr_rgbc_vars.curr_blue_rcv;
	for(uint8_t i = 1; i < NOS_STD; i++)
	{opt_std_vars->strd_vars[i][2] =
	(uint16_t)(factor * (float)opt_std_vars->strd_vars[i][2]);}

	factor = (float)sys_info.curr_rgbc_vars.curr_clear_rcv/(float)opt_std_vars->stan_0_clear;
	opt_std_vars->stan_0_clear = sys_info.curr_rgbc_vars.curr_clear_rcv;
	for(uint8_t i = 1; i < NOS_STD; i++)
	{opt_std_vars->strd_vars[i][3] =
	(uint16_t)(factor * (float)opt_std_vars->strd_vars[i][3]);}
}

int mem_fresh_check(void)
{
	if( (save_sys_info.bk_data[0] == MEM_ID1) && (save_sys_info.bk_data[0] == MEM_ID2) ){return 0;}
	return 1;
}

int sw_update(void)
{
	if( (save_sys_info.bk_var.maj_sw_rv_no != MAJ_SW_RV_NO)
	 || (save_sys_info.bk_var.mid_sw_rv_no != MID_SW_RV_NO)
	 || (save_sys_info.bk_var.min_sw_rv_no != MIN_SW_RV_NO) )
	{
		return 1;
	}
	return 0;
}

int validate_data(void)
{
	//sys_address check
	if(save_sys_info.bk_var.curr_sys_add != sys_info.set_sys_add){return 1;}
	return 0;
}

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

        save_sys_info.bk_var.hrd_std_vars.stan_3_red = 14000;        save_sys_info.bk_var.hrd_std_vars2.stan_3_red = 14000;
        save_sys_info.bk_var.hrd_std_vars.stan_3_green = 12129;      save_sys_info.bk_var.hrd_std_vars2.stan_3_green = 12129;
        save_sys_info.bk_var.hrd_std_vars.stan_3_blue = 13953;       save_sys_info.bk_var.hrd_std_vars2.stan_3_blue = 13953;
        save_sys_info.bk_var.hrd_std_vars.stan_3_clear = 41985;      save_sys_info.bk_var.hrd_std_vars2.stan_3_clear = 41985;

        save_sys_info.bk_var.hrd_std_vars.stan_4_red = 10423;        save_sys_info.bk_var.hrd_std_vars2.stan_4_red = 10423;
        save_sys_info.bk_var.hrd_std_vars.stan_4_green = 9970;       save_sys_info.bk_var.hrd_std_vars2.stan_4_green = 9970;
        save_sys_info.bk_var.hrd_std_vars.stan_4_blue = 11836;       save_sys_info.bk_var.hrd_std_vars2.stan_4_blue = 11836;
        save_sys_info.bk_var.hrd_std_vars.stan_4_clear = 33830;      save_sys_info.bk_var.hrd_std_vars2.stan_4_clear = 33830;

        save_sys_info.bk_var.hrd_std_vars.stan_5_red = 10423;        save_sys_info.bk_var.hrd_std_vars2.stan_5_red = 10423;
        save_sys_info.bk_var.hrd_std_vars.stan_5_green = 9970;       save_sys_info.bk_var.hrd_std_vars2.stan_5_green = 9970;
        save_sys_info.bk_var.hrd_std_vars.stan_5_blue = 11836;       save_sys_info.bk_var.hrd_std_vars2.stan_5_blue = 11836;
        save_sys_info.bk_var.hrd_std_vars.stan_5_clear = 33830;      save_sys_info.bk_var.hrd_std_vars2.stan_5_clear = 33830;

        save_sys_info.bk_var.hrd_std_vars.stan_6_red = 7956;         save_sys_info.bk_var.hrd_std_vars2.stan_6_red = 7956;
        save_sys_info.bk_var.hrd_std_vars.stan_6_green = 8365;       save_sys_info.bk_var.hrd_std_vars2.stan_6_green = 8365;
        save_sys_info.bk_var.hrd_std_vars.stan_6_blue = 10230;       save_sys_info.bk_var.hrd_std_vars2.stan_6_blue = 10230;
        save_sys_info.bk_var.hrd_std_vars.stan_6_clear = 27924;      save_sys_info.bk_var.hrd_std_vars2.stan_6_clear = 27924;

        save_sys_info.bk_var.hrd_std_vars.stan_7_red = 7956;         save_sys_info.bk_var.hrd_std_vars2.stan_7_red = 7956;
        save_sys_info.bk_var.hrd_std_vars.stan_7_green = 8365;       save_sys_info.bk_var.hrd_std_vars2.stan_7_green = 8365;
        save_sys_info.bk_var.hrd_std_vars.stan_7_blue = 10230;       save_sys_info.bk_var.hrd_std_vars2.stan_7_blue = 10230;
        save_sys_info.bk_var.hrd_std_vars.stan_7_clear = 27924;      save_sys_info.bk_var.hrd_std_vars2.stan_7_clear = 27924;

        save_sys_info.bk_var.hrd_std_vars.stan_8_red = 5811;         save_sys_info.bk_var.hrd_std_vars2.stan_8_red = 5811;
        save_sys_info.bk_var.hrd_std_vars.stan_8_green = 6847;       save_sys_info.bk_var.hrd_std_vars2.stan_8_green = 6847;
        save_sys_info.bk_var.hrd_std_vars.stan_8_blue = 8673;        save_sys_info.bk_var.hrd_std_vars2.stan_8_blue = 8673;
        save_sys_info.bk_var.hrd_std_vars.stan_8_clear = 22489;      save_sys_info.bk_var.hrd_std_vars2.stan_8_clear = 22489;

        save_sys_info.bk_var.hrd_std_vars.stan_9_red = 5811;         save_sys_info.bk_var.hrd_std_vars2.stan_9_red = 5811;
        save_sys_info.bk_var.hrd_std_vars.stan_9_green = 6847;       save_sys_info.bk_var.hrd_std_vars2.stan_9_green = 6847;
        save_sys_info.bk_var.hrd_std_vars.stan_9_blue = 8673;        save_sys_info.bk_var.hrd_std_vars2.stan_9_blue = 8673;
        save_sys_info.bk_var.hrd_std_vars.stan_9_clear = 22489;      save_sys_info.bk_var.hrd_std_vars2.stan_9_clear = 22489;

        save_sys_info.bk_var.hrd_std_vars.stan_10_red = 4425;        save_sys_info.bk_var.hrd_std_vars2.stan_10_red = 4425;
        save_sys_info.bk_var.hrd_std_vars.stan_10_green = 5785;      save_sys_info.bk_var.hrd_std_vars2.stan_10_green = 5785;
        save_sys_info.bk_var.hrd_std_vars.stan_10_blue = 7557;       save_sys_info.bk_var.hrd_std_vars2.stan_10_blue = 7557;
        save_sys_info.bk_var.hrd_std_vars.stan_10_clear = 18768;     save_sys_info.bk_var.hrd_std_vars2.stan_10_clear = 18768;
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

        save_sys_info.bk_var.hrd_std_vars.stan_3_red = 14000;        save_sys_info.bk_var.hrd_std_vars2.stan_3_red = 14000;
        save_sys_info.bk_var.hrd_std_vars.stan_3_green = 12129;      save_sys_info.bk_var.hrd_std_vars2.stan_3_green = 12129;
        save_sys_info.bk_var.hrd_std_vars.stan_3_blue = 13953;       save_sys_info.bk_var.hrd_std_vars2.stan_3_blue = 13953;
        save_sys_info.bk_var.hrd_std_vars.stan_3_clear = 41985;      save_sys_info.bk_var.hrd_std_vars2.stan_3_clear = 41985;

        save_sys_info.bk_var.hrd_std_vars.stan_4_red = 10423;        save_sys_info.bk_var.hrd_std_vars2.stan_4_red = 10423;
        save_sys_info.bk_var.hrd_std_vars.stan_4_green = 9970;       save_sys_info.bk_var.hrd_std_vars2.stan_4_green = 9970;
        save_sys_info.bk_var.hrd_std_vars.stan_4_blue = 11836;       save_sys_info.bk_var.hrd_std_vars2.stan_4_blue = 11836;
        save_sys_info.bk_var.hrd_std_vars.stan_4_clear = 33830;      save_sys_info.bk_var.hrd_std_vars2.stan_4_clear = 33830;

        save_sys_info.bk_var.hrd_std_vars.stan_5_red = 10423;        save_sys_info.bk_var.hrd_std_vars2.stan_5_red = 10423;
        save_sys_info.bk_var.hrd_std_vars.stan_5_green = 9970;       save_sys_info.bk_var.hrd_std_vars2.stan_5_green = 9970;
        save_sys_info.bk_var.hrd_std_vars.stan_5_blue = 11836;       save_sys_info.bk_var.hrd_std_vars2.stan_5_blue = 11836;
        save_sys_info.bk_var.hrd_std_vars.stan_5_clear = 33830;      save_sys_info.bk_var.hrd_std_vars2.stan_5_clear = 33830;

        save_sys_info.bk_var.hrd_std_vars.stan_6_red = 7956;         save_sys_info.bk_var.hrd_std_vars2.stan_6_red = 7956;
        save_sys_info.bk_var.hrd_std_vars.stan_6_green = 8365;       save_sys_info.bk_var.hrd_std_vars2.stan_6_green = 8365;
        save_sys_info.bk_var.hrd_std_vars.stan_6_blue = 10230;       save_sys_info.bk_var.hrd_std_vars2.stan_6_blue = 10230;
        save_sys_info.bk_var.hrd_std_vars.stan_6_clear = 27924;      save_sys_info.bk_var.hrd_std_vars2.stan_6_clear = 27924;

        save_sys_info.bk_var.hrd_std_vars.stan_7_red = 7956;         save_sys_info.bk_var.hrd_std_vars2.stan_7_red = 7956;
        save_sys_info.bk_var.hrd_std_vars.stan_7_green = 8365;       save_sys_info.bk_var.hrd_std_vars2.stan_7_green = 8365;
        save_sys_info.bk_var.hrd_std_vars.stan_7_blue = 10230;       save_sys_info.bk_var.hrd_std_vars2.stan_7_blue = 10230;
        save_sys_info.bk_var.hrd_std_vars.stan_7_clear = 27924;      save_sys_info.bk_var.hrd_std_vars2.stan_7_clear = 27924;

        save_sys_info.bk_var.hrd_std_vars.stan_8_red = 5811;         save_sys_info.bk_var.hrd_std_vars2.stan_8_red = 5811;
        save_sys_info.bk_var.hrd_std_vars.stan_8_green = 6847;       save_sys_info.bk_var.hrd_std_vars2.stan_8_green = 6847;
        save_sys_info.bk_var.hrd_std_vars.stan_8_blue = 8673;        save_sys_info.bk_var.hrd_std_vars2.stan_8_blue = 8673;
        save_sys_info.bk_var.hrd_std_vars.stan_8_clear = 22489;      save_sys_info.bk_var.hrd_std_vars2.stan_8_clear = 22489;

        save_sys_info.bk_var.hrd_std_vars.stan_9_red = 5811;         save_sys_info.bk_var.hrd_std_vars2.stan_9_red = 5811;
        save_sys_info.bk_var.hrd_std_vars.stan_9_green = 6847;       save_sys_info.bk_var.hrd_std_vars2.stan_9_green = 6847;
        save_sys_info.bk_var.hrd_std_vars.stan_9_blue = 8673;        save_sys_info.bk_var.hrd_std_vars2.stan_9_blue = 8673;
        save_sys_info.bk_var.hrd_std_vars.stan_9_clear = 22489;      save_sys_info.bk_var.hrd_std_vars2.stan_9_clear = 22489;

        save_sys_info.bk_var.hrd_std_vars.stan_10_red = 4425;        save_sys_info.bk_var.hrd_std_vars2.stan_10_red = 4425;
        save_sys_info.bk_var.hrd_std_vars.stan_10_green = 5785;      save_sys_info.bk_var.hrd_std_vars2.stan_10_green = 5785;
        save_sys_info.bk_var.hrd_std_vars.stan_10_blue = 7557;       save_sys_info.bk_var.hrd_std_vars2.stan_10_blue = 7557;
        save_sys_info.bk_var.hrd_std_vars.stan_10_clear = 18768;     save_sys_info.bk_var.hrd_std_vars2.stan_10_clear = 18768;
		break;
	case COPPER:
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

        save_sys_info.bk_var.hrd_std_vars.stan_3_red = 14000;        save_sys_info.bk_var.hrd_std_vars2.stan_3_red = 14000;
        save_sys_info.bk_var.hrd_std_vars.stan_3_green = 12129;      save_sys_info.bk_var.hrd_std_vars2.stan_3_green = 12129;
        save_sys_info.bk_var.hrd_std_vars.stan_3_blue = 13953;       save_sys_info.bk_var.hrd_std_vars2.stan_3_blue = 13953;
        save_sys_info.bk_var.hrd_std_vars.stan_3_clear = 41985;      save_sys_info.bk_var.hrd_std_vars2.stan_3_clear = 41985;

        save_sys_info.bk_var.hrd_std_vars.stan_4_red = 10423;        save_sys_info.bk_var.hrd_std_vars2.stan_4_red = 10423;
        save_sys_info.bk_var.hrd_std_vars.stan_4_green = 9970;       save_sys_info.bk_var.hrd_std_vars2.stan_4_green = 9970;
        save_sys_info.bk_var.hrd_std_vars.stan_4_blue = 11836;       save_sys_info.bk_var.hrd_std_vars2.stan_4_blue = 11836;
        save_sys_info.bk_var.hrd_std_vars.stan_4_clear = 33830;      save_sys_info.bk_var.hrd_std_vars2.stan_4_clear = 33830;

        save_sys_info.bk_var.hrd_std_vars.stan_5_red = 10423;        save_sys_info.bk_var.hrd_std_vars2.stan_5_red = 10423;
        save_sys_info.bk_var.hrd_std_vars.stan_5_green = 9970;       save_sys_info.bk_var.hrd_std_vars2.stan_5_green = 9970;
        save_sys_info.bk_var.hrd_std_vars.stan_5_blue = 11836;       save_sys_info.bk_var.hrd_std_vars2.stan_5_blue = 11836;
        save_sys_info.bk_var.hrd_std_vars.stan_5_clear = 33830;      save_sys_info.bk_var.hrd_std_vars2.stan_5_clear = 33830;

        save_sys_info.bk_var.hrd_std_vars.stan_6_red = 7956;         save_sys_info.bk_var.hrd_std_vars2.stan_6_red = 7956;
        save_sys_info.bk_var.hrd_std_vars.stan_6_green = 8365;       save_sys_info.bk_var.hrd_std_vars2.stan_6_green = 8365;
        save_sys_info.bk_var.hrd_std_vars.stan_6_blue = 10230;       save_sys_info.bk_var.hrd_std_vars2.stan_6_blue = 10230;
        save_sys_info.bk_var.hrd_std_vars.stan_6_clear = 27924;      save_sys_info.bk_var.hrd_std_vars2.stan_6_clear = 27924;

        save_sys_info.bk_var.hrd_std_vars.stan_7_red = 7956;         save_sys_info.bk_var.hrd_std_vars2.stan_7_red = 7956;
        save_sys_info.bk_var.hrd_std_vars.stan_7_green = 8365;       save_sys_info.bk_var.hrd_std_vars2.stan_7_green = 8365;
        save_sys_info.bk_var.hrd_std_vars.stan_7_blue = 10230;       save_sys_info.bk_var.hrd_std_vars2.stan_7_blue = 10230;
        save_sys_info.bk_var.hrd_std_vars.stan_7_clear = 27924;      save_sys_info.bk_var.hrd_std_vars2.stan_7_clear = 27924;

        save_sys_info.bk_var.hrd_std_vars.stan_8_red = 5811;         save_sys_info.bk_var.hrd_std_vars2.stan_8_red = 5811;
        save_sys_info.bk_var.hrd_std_vars.stan_8_green = 6847;       save_sys_info.bk_var.hrd_std_vars2.stan_8_green = 6847;
        save_sys_info.bk_var.hrd_std_vars.stan_8_blue = 8673;        save_sys_info.bk_var.hrd_std_vars2.stan_8_blue = 8673;
        save_sys_info.bk_var.hrd_std_vars.stan_8_clear = 22489;      save_sys_info.bk_var.hrd_std_vars2.stan_8_clear = 22489;

        save_sys_info.bk_var.hrd_std_vars.stan_9_red = 5811;         save_sys_info.bk_var.hrd_std_vars2.stan_9_red = 5811;
        save_sys_info.bk_var.hrd_std_vars.stan_9_green = 6847;       save_sys_info.bk_var.hrd_std_vars2.stan_9_green = 6847;
        save_sys_info.bk_var.hrd_std_vars.stan_9_blue = 8673;        save_sys_info.bk_var.hrd_std_vars2.stan_9_blue = 8673;
        save_sys_info.bk_var.hrd_std_vars.stan_9_clear = 22489;      save_sys_info.bk_var.hrd_std_vars2.stan_9_clear = 22489;

        save_sys_info.bk_var.hrd_std_vars.stan_10_red = 4425;        save_sys_info.bk_var.hrd_std_vars2.stan_10_red = 4425;
        save_sys_info.bk_var.hrd_std_vars.stan_10_green = 5785;      save_sys_info.bk_var.hrd_std_vars2.stan_10_green = 5785;
        save_sys_info.bk_var.hrd_std_vars.stan_10_blue = 7557;       save_sys_info.bk_var.hrd_std_vars2.stan_10_blue = 7557;
        save_sys_info.bk_var.hrd_std_vars.stan_10_clear = 18768;     save_sys_info.bk_var.hrd_std_vars2.stan_10_clear = 18768;
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

        save_sys_info.bk_var.hrd_std_vars.stan_3_red = 14000;        save_sys_info.bk_var.hrd_std_vars2.stan_3_red = 14000;
        save_sys_info.bk_var.hrd_std_vars.stan_3_green = 12129;      save_sys_info.bk_var.hrd_std_vars2.stan_3_green = 12129;
        save_sys_info.bk_var.hrd_std_vars.stan_3_blue = 13953;       save_sys_info.bk_var.hrd_std_vars2.stan_3_blue = 13953;
        save_sys_info.bk_var.hrd_std_vars.stan_3_clear = 41985;      save_sys_info.bk_var.hrd_std_vars2.stan_3_clear = 41985;

        save_sys_info.bk_var.hrd_std_vars.stan_4_red = 10423;        save_sys_info.bk_var.hrd_std_vars2.stan_4_red = 10423;
        save_sys_info.bk_var.hrd_std_vars.stan_4_green = 9970;       save_sys_info.bk_var.hrd_std_vars2.stan_4_green = 9970;
        save_sys_info.bk_var.hrd_std_vars.stan_4_blue = 11836;       save_sys_info.bk_var.hrd_std_vars2.stan_4_blue = 11836;
        save_sys_info.bk_var.hrd_std_vars.stan_4_clear = 33830;      save_sys_info.bk_var.hrd_std_vars2.stan_4_clear = 33830;

        save_sys_info.bk_var.hrd_std_vars.stan_5_red = 10423;        save_sys_info.bk_var.hrd_std_vars2.stan_5_red = 10423;
        save_sys_info.bk_var.hrd_std_vars.stan_5_green = 9970;       save_sys_info.bk_var.hrd_std_vars2.stan_5_green = 9970;
        save_sys_info.bk_var.hrd_std_vars.stan_5_blue = 11836;       save_sys_info.bk_var.hrd_std_vars2.stan_5_blue = 11836;
        save_sys_info.bk_var.hrd_std_vars.stan_5_clear = 33830;      save_sys_info.bk_var.hrd_std_vars2.stan_5_clear = 33830;

        save_sys_info.bk_var.hrd_std_vars.stan_6_red = 7956;         save_sys_info.bk_var.hrd_std_vars2.stan_6_red = 7956;
        save_sys_info.bk_var.hrd_std_vars.stan_6_green = 8365;       save_sys_info.bk_var.hrd_std_vars2.stan_6_green = 8365;
        save_sys_info.bk_var.hrd_std_vars.stan_6_blue = 10230;       save_sys_info.bk_var.hrd_std_vars2.stan_6_blue = 10230;
        save_sys_info.bk_var.hrd_std_vars.stan_6_clear = 27924;      save_sys_info.bk_var.hrd_std_vars2.stan_6_clear = 27924;

        save_sys_info.bk_var.hrd_std_vars.stan_7_red = 7956;         save_sys_info.bk_var.hrd_std_vars2.stan_7_red = 7956;
        save_sys_info.bk_var.hrd_std_vars.stan_7_green = 8365;       save_sys_info.bk_var.hrd_std_vars2.stan_7_green = 8365;
        save_sys_info.bk_var.hrd_std_vars.stan_7_blue = 10230;       save_sys_info.bk_var.hrd_std_vars2.stan_7_blue = 10230;
        save_sys_info.bk_var.hrd_std_vars.stan_7_clear = 27924;      save_sys_info.bk_var.hrd_std_vars2.stan_7_clear = 27924;

        save_sys_info.bk_var.hrd_std_vars.stan_8_red = 5811;         save_sys_info.bk_var.hrd_std_vars2.stan_8_red = 5811;
        save_sys_info.bk_var.hrd_std_vars.stan_8_green = 6847;       save_sys_info.bk_var.hrd_std_vars2.stan_8_green = 6847;
        save_sys_info.bk_var.hrd_std_vars.stan_8_blue = 8673;        save_sys_info.bk_var.hrd_std_vars2.stan_8_blue = 8673;
        save_sys_info.bk_var.hrd_std_vars.stan_8_clear = 22489;      save_sys_info.bk_var.hrd_std_vars2.stan_8_clear = 22489;

        save_sys_info.bk_var.hrd_std_vars.stan_9_red = 5811;         save_sys_info.bk_var.hrd_std_vars2.stan_9_red = 5811;
        save_sys_info.bk_var.hrd_std_vars.stan_9_green = 6847;       save_sys_info.bk_var.hrd_std_vars2.stan_9_green = 6847;
        save_sys_info.bk_var.hrd_std_vars.stan_9_blue = 8673;        save_sys_info.bk_var.hrd_std_vars2.stan_9_blue = 8673;
        save_sys_info.bk_var.hrd_std_vars.stan_9_clear = 22489;      save_sys_info.bk_var.hrd_std_vars2.stan_9_clear = 22489;

        save_sys_info.bk_var.hrd_std_vars.stan_10_red = 4425;        save_sys_info.bk_var.hrd_std_vars2.stan_10_red = 4425;
        save_sys_info.bk_var.hrd_std_vars.stan_10_green = 5785;      save_sys_info.bk_var.hrd_std_vars2.stan_10_green = 5785;
        save_sys_info.bk_var.hrd_std_vars.stan_10_blue = 7557;       save_sys_info.bk_var.hrd_std_vars2.stan_10_blue = 7557;
        save_sys_info.bk_var.hrd_std_vars.stan_10_clear = 18768;     save_sys_info.bk_var.hrd_std_vars2.stan_10_clear = 18768;
		break;
	case BORON:
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

        save_sys_info.bk_var.hrd_std_vars.stan_3_red = 14000;        save_sys_info.bk_var.hrd_std_vars2.stan_3_red = 14000;
        save_sys_info.bk_var.hrd_std_vars.stan_3_green = 12129;      save_sys_info.bk_var.hrd_std_vars2.stan_3_green = 12129;
        save_sys_info.bk_var.hrd_std_vars.stan_3_blue = 13953;       save_sys_info.bk_var.hrd_std_vars2.stan_3_blue = 13953;
        save_sys_info.bk_var.hrd_std_vars.stan_3_clear = 41985;      save_sys_info.bk_var.hrd_std_vars2.stan_3_clear = 41985;

        save_sys_info.bk_var.hrd_std_vars.stan_4_red = 10423;        save_sys_info.bk_var.hrd_std_vars2.stan_4_red = 10423;
        save_sys_info.bk_var.hrd_std_vars.stan_4_green = 9970;       save_sys_info.bk_var.hrd_std_vars2.stan_4_green = 9970;
        save_sys_info.bk_var.hrd_std_vars.stan_4_blue = 11836;       save_sys_info.bk_var.hrd_std_vars2.stan_4_blue = 11836;
        save_sys_info.bk_var.hrd_std_vars.stan_4_clear = 33830;      save_sys_info.bk_var.hrd_std_vars2.stan_4_clear = 33830;

        save_sys_info.bk_var.hrd_std_vars.stan_5_red = 10423;        save_sys_info.bk_var.hrd_std_vars2.stan_5_red = 10423;
        save_sys_info.bk_var.hrd_std_vars.stan_5_green = 9970;       save_sys_info.bk_var.hrd_std_vars2.stan_5_green = 9970;
        save_sys_info.bk_var.hrd_std_vars.stan_5_blue = 11836;       save_sys_info.bk_var.hrd_std_vars2.stan_5_blue = 11836;
        save_sys_info.bk_var.hrd_std_vars.stan_5_clear = 33830;      save_sys_info.bk_var.hrd_std_vars2.stan_5_clear = 33830;

        save_sys_info.bk_var.hrd_std_vars.stan_6_red = 7956;         save_sys_info.bk_var.hrd_std_vars2.stan_6_red = 7956;
        save_sys_info.bk_var.hrd_std_vars.stan_6_green = 8365;       save_sys_info.bk_var.hrd_std_vars2.stan_6_green = 8365;
        save_sys_info.bk_var.hrd_std_vars.stan_6_blue = 10230;       save_sys_info.bk_var.hrd_std_vars2.stan_6_blue = 10230;
        save_sys_info.bk_var.hrd_std_vars.stan_6_clear = 27924;      save_sys_info.bk_var.hrd_std_vars2.stan_6_clear = 27924;

        save_sys_info.bk_var.hrd_std_vars.stan_7_red = 7956;         save_sys_info.bk_var.hrd_std_vars2.stan_7_red = 7956;
        save_sys_info.bk_var.hrd_std_vars.stan_7_green = 8365;       save_sys_info.bk_var.hrd_std_vars2.stan_7_green = 8365;
        save_sys_info.bk_var.hrd_std_vars.stan_7_blue = 10230;       save_sys_info.bk_var.hrd_std_vars2.stan_7_blue = 10230;
        save_sys_info.bk_var.hrd_std_vars.stan_7_clear = 27924;      save_sys_info.bk_var.hrd_std_vars2.stan_7_clear = 27924;

        save_sys_info.bk_var.hrd_std_vars.stan_8_red = 5811;         save_sys_info.bk_var.hrd_std_vars2.stan_8_red = 5811;
        save_sys_info.bk_var.hrd_std_vars.stan_8_green = 6847;       save_sys_info.bk_var.hrd_std_vars2.stan_8_green = 6847;
        save_sys_info.bk_var.hrd_std_vars.stan_8_blue = 8673;        save_sys_info.bk_var.hrd_std_vars2.stan_8_blue = 8673;
        save_sys_info.bk_var.hrd_std_vars.stan_8_clear = 22489;      save_sys_info.bk_var.hrd_std_vars2.stan_8_clear = 22489;

        save_sys_info.bk_var.hrd_std_vars.stan_9_red = 5811;         save_sys_info.bk_var.hrd_std_vars2.stan_9_red = 5811;
        save_sys_info.bk_var.hrd_std_vars.stan_9_green = 6847;       save_sys_info.bk_var.hrd_std_vars2.stan_9_green = 6847;
        save_sys_info.bk_var.hrd_std_vars.stan_9_blue = 8673;        save_sys_info.bk_var.hrd_std_vars2.stan_9_blue = 8673;
        save_sys_info.bk_var.hrd_std_vars.stan_9_clear = 22489;      save_sys_info.bk_var.hrd_std_vars2.stan_9_clear = 22489;

        save_sys_info.bk_var.hrd_std_vars.stan_10_red = 4425;        save_sys_info.bk_var.hrd_std_vars2.stan_10_red = 4425;
        save_sys_info.bk_var.hrd_std_vars.stan_10_green = 5785;      save_sys_info.bk_var.hrd_std_vars2.stan_10_green = 5785;
        save_sys_info.bk_var.hrd_std_vars.stan_10_blue = 7557;       save_sys_info.bk_var.hrd_std_vars2.stan_10_blue = 7557;
        save_sys_info.bk_var.hrd_std_vars.stan_10_clear = 18768;     save_sys_info.bk_var.hrd_std_vars2.stan_10_clear = 18768;
		break;
	case SULPHUR://15-05-2026
        save_sys_info.bk_var.hrd_std_vars.stan_0_red = 18994;        save_sys_info.bk_var.hrd_std_vars2.stan_0_red = 18994;
        save_sys_info.bk_var.hrd_std_vars.stan_0_green = 19019;      save_sys_info.bk_var.hrd_std_vars2.stan_0_green = 19019;
        save_sys_info.bk_var.hrd_std_vars.stan_0_blue = 21154;       save_sys_info.bk_var.hrd_std_vars2.stan_0_blue = 21154;
        save_sys_info.bk_var.hrd_std_vars.stan_0_clear = 53306;      save_sys_info.bk_var.hrd_std_vars2.stan_0_clear = 53306;

        save_sys_info.bk_var.hrd_std_vars.stan_1_red = 18702;        save_sys_info.bk_var.hrd_std_vars2.stan_1_red = 18702;
        save_sys_info.bk_var.hrd_std_vars.stan_1_green = 18626;      save_sys_info.bk_var.hrd_std_vars2.stan_1_green = 18626;
        save_sys_info.bk_var.hrd_std_vars.stan_1_blue = 20685;       save_sys_info.bk_var.hrd_std_vars2.stan_1_blue = 20685;
        save_sys_info.bk_var.hrd_std_vars.stan_1_clear = 52283;      save_sys_info.bk_var.hrd_std_vars2.stan_1_clear = 52283;

        save_sys_info.bk_var.hrd_std_vars.stan_2_red = 18702;        save_sys_info.bk_var.hrd_std_vars2.stan_2_red = 18702;
        save_sys_info.bk_var.hrd_std_vars.stan_2_green = 18626;      save_sys_info.bk_var.hrd_std_vars2.stan_2_green = 18626;
        save_sys_info.bk_var.hrd_std_vars.stan_2_blue = 20685;       save_sys_info.bk_var.hrd_std_vars2.stan_2_blue = 20685;
        save_sys_info.bk_var.hrd_std_vars.stan_2_clear = 52283;      save_sys_info.bk_var.hrd_std_vars2.stan_2_clear = 52283;

        save_sys_info.bk_var.hrd_std_vars.stan_3_red = 16999;        save_sys_info.bk_var.hrd_std_vars2.stan_3_red = 16999;
        save_sys_info.bk_var.hrd_std_vars.stan_3_green = 16769;      save_sys_info.bk_var.hrd_std_vars2.stan_3_green = 16769;
        save_sys_info.bk_var.hrd_std_vars.stan_3_blue = 18524;       save_sys_info.bk_var.hrd_std_vars2.stan_3_blue = 18524;
        save_sys_info.bk_var.hrd_std_vars.stan_3_clear = 47202;      save_sys_info.bk_var.hrd_std_vars2.stan_3_clear = 47202;

        save_sys_info.bk_var.hrd_std_vars.stan_4_red = 15441;        save_sys_info.bk_var.hrd_std_vars2.stan_4_red = 15441;
        save_sys_info.bk_var.hrd_std_vars.stan_4_green = 15215;      save_sys_info.bk_var.hrd_std_vars2.stan_4_green = 15215;
        save_sys_info.bk_var.hrd_std_vars.stan_4_blue = 16753;       save_sys_info.bk_var.hrd_std_vars2.stan_4_blue = 16753;
        save_sys_info.bk_var.hrd_std_vars.stan_4_clear = 42886;      save_sys_info.bk_var.hrd_std_vars2.stan_4_clear = 42886;

        save_sys_info.bk_var.hrd_std_vars.stan_5_red = 13027;        save_sys_info.bk_var.hrd_std_vars2.stan_5_red = 13027;
        save_sys_info.bk_var.hrd_std_vars.stan_5_green = 12591;      save_sys_info.bk_var.hrd_std_vars2.stan_5_green = 12591;
        save_sys_info.bk_var.hrd_std_vars.stan_5_blue = 13724;       save_sys_info.bk_var.hrd_std_vars2.stan_5_blue = 13724;
        save_sys_info.bk_var.hrd_std_vars.stan_5_clear = 35669;      save_sys_info.bk_var.hrd_std_vars2.stan_5_clear = 35669;

        save_sys_info.bk_var.hrd_std_vars.stan_6_red = 11156;        save_sys_info.bk_var.hrd_std_vars2.stan_6_red = 11156;
        save_sys_info.bk_var.hrd_std_vars.stan_6_green = 10652;      save_sys_info.bk_var.hrd_std_vars2.stan_6_green = 10652;
        save_sys_info.bk_var.hrd_std_vars.stan_6_blue = 11524;       save_sys_info.bk_var.hrd_std_vars2.stan_6_blue = 11524;
        save_sys_info.bk_var.hrd_std_vars.stan_6_clear = 30284;      save_sys_info.bk_var.hrd_std_vars2.stan_6_clear = 30284;

        save_sys_info.bk_var.hrd_std_vars.stan_7_red = 9716;        save_sys_info.bk_var.hrd_std_vars2.stan_7_red = 9716;
        save_sys_info.bk_var.hrd_std_vars.stan_7_green = 9119;      save_sys_info.bk_var.hrd_std_vars2.stan_7_green = 9119;
        save_sys_info.bk_var.hrd_std_vars.stan_7_blue = 9833;       save_sys_info.bk_var.hrd_std_vars2.stan_7_blue = 9833;
        save_sys_info.bk_var.hrd_std_vars.stan_7_clear = 26104;      save_sys_info.bk_var.hrd_std_vars2.stan_7_clear = 26104;

        save_sys_info.bk_var.hrd_std_vars.stan_8_red = 9716;        save_sys_info.bk_var.hrd_std_vars2.stan_8_red = 9716;
        save_sys_info.bk_var.hrd_std_vars.stan_8_green = 9119;      save_sys_info.bk_var.hrd_std_vars2.stan_8_green = 9119;
        save_sys_info.bk_var.hrd_std_vars.stan_8_blue = 9833;       save_sys_info.bk_var.hrd_std_vars2.stan_8_blue = 9833;
        save_sys_info.bk_var.hrd_std_vars.stan_8_clear = 26104;      save_sys_info.bk_var.hrd_std_vars2.stan_8_clear = 26104;

        save_sys_info.bk_var.hrd_std_vars.stan_9_red = 8092;        save_sys_info.bk_var.hrd_std_vars2.stan_9_red = 8092;
        save_sys_info.bk_var.hrd_std_vars.stan_9_green = 7519;      save_sys_info.bk_var.hrd_std_vars2.stan_9_green = 7519;
        save_sys_info.bk_var.hrd_std_vars.stan_9_blue = 8029;       save_sys_info.bk_var.hrd_std_vars2.stan_9_blue = 8029;
        save_sys_info.bk_var.hrd_std_vars.stan_9_clear = 21533;      save_sys_info.bk_var.hrd_std_vars2.stan_9_clear = 21533;

        save_sys_info.bk_var.hrd_std_vars.stan_10_red = 8092;        save_sys_info.bk_var.hrd_std_vars2.stan_10_red = 8092;
        save_sys_info.bk_var.hrd_std_vars.stan_10_green = 7519;      save_sys_info.bk_var.hrd_std_vars2.stan_10_green = 7519;
        save_sys_info.bk_var.hrd_std_vars.stan_10_blue = 8029;       save_sys_info.bk_var.hrd_std_vars2.stan_10_blue = 8029;
        save_sys_info.bk_var.hrd_std_vars.stan_10_clear = 21533;     save_sys_info.bk_var.hrd_std_vars2.stan_10_clear = 21533;
		break;
	case POTASSIUM:
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

        save_sys_info.bk_var.hrd_std_vars.stan_3_red = 14000;        save_sys_info.bk_var.hrd_std_vars2.stan_3_red = 14000;
        save_sys_info.bk_var.hrd_std_vars.stan_3_green = 12129;      save_sys_info.bk_var.hrd_std_vars2.stan_3_green = 12129;
        save_sys_info.bk_var.hrd_std_vars.stan_3_blue = 13953;       save_sys_info.bk_var.hrd_std_vars2.stan_3_blue = 13953;
        save_sys_info.bk_var.hrd_std_vars.stan_3_clear = 41985;      save_sys_info.bk_var.hrd_std_vars2.stan_3_clear = 41985;

        save_sys_info.bk_var.hrd_std_vars.stan_4_red = 10423;        save_sys_info.bk_var.hrd_std_vars2.stan_4_red = 10423;
        save_sys_info.bk_var.hrd_std_vars.stan_4_green = 9970;       save_sys_info.bk_var.hrd_std_vars2.stan_4_green = 9970;
        save_sys_info.bk_var.hrd_std_vars.stan_4_blue = 11836;       save_sys_info.bk_var.hrd_std_vars2.stan_4_blue = 11836;
        save_sys_info.bk_var.hrd_std_vars.stan_4_clear = 33830;      save_sys_info.bk_var.hrd_std_vars2.stan_4_clear = 33830;

        save_sys_info.bk_var.hrd_std_vars.stan_5_red = 10423;        save_sys_info.bk_var.hrd_std_vars2.stan_5_red = 10423;
        save_sys_info.bk_var.hrd_std_vars.stan_5_green = 9970;       save_sys_info.bk_var.hrd_std_vars2.stan_5_green = 9970;
        save_sys_info.bk_var.hrd_std_vars.stan_5_blue = 11836;       save_sys_info.bk_var.hrd_std_vars2.stan_5_blue = 11836;
        save_sys_info.bk_var.hrd_std_vars.stan_5_clear = 33830;      save_sys_info.bk_var.hrd_std_vars2.stan_5_clear = 33830;

        save_sys_info.bk_var.hrd_std_vars.stan_6_red = 7956;         save_sys_info.bk_var.hrd_std_vars2.stan_6_red = 7956;
        save_sys_info.bk_var.hrd_std_vars.stan_6_green = 8365;       save_sys_info.bk_var.hrd_std_vars2.stan_6_green = 8365;
        save_sys_info.bk_var.hrd_std_vars.stan_6_blue = 10230;       save_sys_info.bk_var.hrd_std_vars2.stan_6_blue = 10230;
        save_sys_info.bk_var.hrd_std_vars.stan_6_clear = 27924;      save_sys_info.bk_var.hrd_std_vars2.stan_6_clear = 27924;

        save_sys_info.bk_var.hrd_std_vars.stan_7_red = 7956;         save_sys_info.bk_var.hrd_std_vars2.stan_7_red = 7956;
        save_sys_info.bk_var.hrd_std_vars.stan_7_green = 8365;       save_sys_info.bk_var.hrd_std_vars2.stan_7_green = 8365;
        save_sys_info.bk_var.hrd_std_vars.stan_7_blue = 10230;       save_sys_info.bk_var.hrd_std_vars2.stan_7_blue = 10230;
        save_sys_info.bk_var.hrd_std_vars.stan_7_clear = 27924;      save_sys_info.bk_var.hrd_std_vars2.stan_7_clear = 27924;

        save_sys_info.bk_var.hrd_std_vars.stan_8_red = 5811;         save_sys_info.bk_var.hrd_std_vars2.stan_8_red = 5811;
        save_sys_info.bk_var.hrd_std_vars.stan_8_green = 6847;       save_sys_info.bk_var.hrd_std_vars2.stan_8_green = 6847;
        save_sys_info.bk_var.hrd_std_vars.stan_8_blue = 8673;        save_sys_info.bk_var.hrd_std_vars2.stan_8_blue = 8673;
        save_sys_info.bk_var.hrd_std_vars.stan_8_clear = 22489;      save_sys_info.bk_var.hrd_std_vars2.stan_8_clear = 22489;

        save_sys_info.bk_var.hrd_std_vars.stan_9_red = 5811;         save_sys_info.bk_var.hrd_std_vars2.stan_9_red = 5811;
        save_sys_info.bk_var.hrd_std_vars.stan_9_green = 6847;       save_sys_info.bk_var.hrd_std_vars2.stan_9_green = 6847;
        save_sys_info.bk_var.hrd_std_vars.stan_9_blue = 8673;        save_sys_info.bk_var.hrd_std_vars2.stan_9_blue = 8673;
        save_sys_info.bk_var.hrd_std_vars.stan_9_clear = 22489;      save_sys_info.bk_var.hrd_std_vars2.stan_9_clear = 22489;

        save_sys_info.bk_var.hrd_std_vars.stan_10_red = 4425;        save_sys_info.bk_var.hrd_std_vars2.stan_10_red = 4425;
        save_sys_info.bk_var.hrd_std_vars.stan_10_green = 5785;      save_sys_info.bk_var.hrd_std_vars2.stan_10_green = 5785;
        save_sys_info.bk_var.hrd_std_vars.stan_10_blue = 7557;       save_sys_info.bk_var.hrd_std_vars2.stan_10_blue = 7557;
        save_sys_info.bk_var.hrd_std_vars.stan_10_clear = 18768;     save_sys_info.bk_var.hrd_std_vars2.stan_10_clear = 18768;
		break;
	case PHOSPHORUS://29-04-2026 alkline							//19-05-2026 acidic
        save_sys_info.bk_var.hrd_std_vars.stan_0_red   = 16604;  save_sys_info.bk_var.hrd_std_vars2.stan_0_red   = 16713;
        save_sys_info.bk_var.hrd_std_vars.stan_0_green = 15462;  save_sys_info.bk_var.hrd_std_vars2.stan_0_green = 15570;
        save_sys_info.bk_var.hrd_std_vars.stan_0_blue  = 18440;  save_sys_info.bk_var.hrd_std_vars2.stan_0_blue  = 18772;
        save_sys_info.bk_var.hrd_std_vars.stan_0_clear = 47486;  save_sys_info.bk_var.hrd_std_vars2.stan_0_clear = 47914;

        save_sys_info.bk_var.hrd_std_vars.stan_1_red   = 14995;  save_sys_info.bk_var.hrd_std_vars2.stan_1_red   = 15262;
        save_sys_info.bk_var.hrd_std_vars.stan_1_green = 14440;  save_sys_info.bk_var.hrd_std_vars2.stan_1_green = 14729;
        save_sys_info.bk_var.hrd_std_vars.stan_1_blue  = 17352;  save_sys_info.bk_var.hrd_std_vars2.stan_1_blue  = 17901;
        save_sys_info.bk_var.hrd_std_vars.stan_1_clear = 43996;  save_sys_info.bk_var.hrd_std_vars2.stan_1_clear = 44955;

        save_sys_info.bk_var.hrd_std_vars.stan_2_red   = 13285;  save_sys_info.bk_var.hrd_std_vars2.stan_2_red   = 13632;
        save_sys_info.bk_var.hrd_std_vars.stan_2_green = 13287;  save_sys_info.bk_var.hrd_std_vars2.stan_2_green = 13667;
        save_sys_info.bk_var.hrd_std_vars.stan_2_blue  = 16187;  save_sys_info.bk_var.hrd_std_vars2.stan_2_blue  = 16770;
        save_sys_info.bk_var.hrd_std_vars.stan_2_clear = 40162;  save_sys_info.bk_var.hrd_std_vars2.stan_2_clear = 41371;

        save_sys_info.bk_var.hrd_std_vars.stan_3_red   = 10378;  save_sys_info.bk_var.hrd_std_vars2.stan_3_red   = 10755;
        save_sys_info.bk_var.hrd_std_vars.stan_3_green = 11221;  save_sys_info.bk_var.hrd_std_vars2.stan_3_green = 11699;
        save_sys_info.bk_var.hrd_std_vars.stan_3_blue  = 13916;  save_sys_info.bk_var.hrd_std_vars2.stan_3_blue  = 14661;
        save_sys_info.bk_var.hrd_std_vars.stan_3_clear = 33405;  save_sys_info.bk_var.hrd_std_vars2.stan_3_clear = 34847;

        save_sys_info.bk_var.hrd_std_vars.stan_4_red   = 10378;  save_sys_info.bk_var.hrd_std_vars2.stan_4_red   = 10755;
        save_sys_info.bk_var.hrd_std_vars.stan_4_green = 11221;  save_sys_info.bk_var.hrd_std_vars2.stan_4_green = 11699;
        save_sys_info.bk_var.hrd_std_vars.stan_4_blue  = 13916;  save_sys_info.bk_var.hrd_std_vars2.stan_4_blue  = 14661;
        save_sys_info.bk_var.hrd_std_vars.stan_4_clear = 33405;  save_sys_info.bk_var.hrd_std_vars2.stan_4_clear = 34847;

        save_sys_info.bk_var.hrd_std_vars.stan_5_red   = 8011;   save_sys_info.bk_var.hrd_std_vars2.stan_5_red   = 8207;
        save_sys_info.bk_var.hrd_std_vars.stan_5_green = 9411;   save_sys_info.bk_var.hrd_std_vars2.stan_5_green = 9766;
        save_sys_info.bk_var.hrd_std_vars.stan_5_blue  = 11962;  save_sys_info.bk_var.hrd_std_vars2.stan_5_blue  = 12556;
        save_sys_info.bk_var.hrd_std_vars.stan_5_clear = 27648;  save_sys_info.bk_var.hrd_std_vars2.stan_5_clear = 28672;

        save_sys_info.bk_var.hrd_std_vars.stan_6_red   = 8011;   save_sys_info.bk_var.hrd_std_vars2.stan_6_red   = 8207;
        save_sys_info.bk_var.hrd_std_vars.stan_6_green = 9411;   save_sys_info.bk_var.hrd_std_vars2.stan_6_green = 9766;
        save_sys_info.bk_var.hrd_std_vars.stan_6_blue  = 11962;  save_sys_info.bk_var.hrd_std_vars2.stan_6_blue  = 12556;
        save_sys_info.bk_var.hrd_std_vars.stan_6_clear = 27648;  save_sys_info.bk_var.hrd_std_vars2.stan_6_clear = 28672;

        save_sys_info.bk_var.hrd_std_vars.stan_7_red   = 6181;   save_sys_info.bk_var.hrd_std_vars2.stan_7_red   = 6370;
        save_sys_info.bk_var.hrd_std_vars.stan_7_green = 7914;   save_sys_info.bk_var.hrd_std_vars2.stan_7_green = 8293;
        save_sys_info.bk_var.hrd_std_vars.stan_7_blue  = 10319;  save_sys_info.bk_var.hrd_std_vars2.stan_7_blue  = 10927;
        save_sys_info.bk_var.hrd_std_vars.stan_7_clear = 22971;  save_sys_info.bk_var.hrd_std_vars2.stan_7_clear = 24034;

        save_sys_info.bk_var.hrd_std_vars.stan_8_red   = 6181;   save_sys_info.bk_var.hrd_std_vars2.stan_8_red   = 6370;
        save_sys_info.bk_var.hrd_std_vars.stan_8_green = 7914;   save_sys_info.bk_var.hrd_std_vars2.stan_8_green = 8293;
        save_sys_info.bk_var.hrd_std_vars.stan_8_blue  = 10319;  save_sys_info.bk_var.hrd_std_vars2.stan_8_blue  = 10927;
        save_sys_info.bk_var.hrd_std_vars.stan_8_clear = 22971;  save_sys_info.bk_var.hrd_std_vars2.stan_8_clear = 24034;

        save_sys_info.bk_var.hrd_std_vars.stan_9_red   = 4516;   save_sys_info.bk_var.hrd_std_vars2.stan_9_red   = 4979;
        save_sys_info.bk_var.hrd_std_vars.stan_9_green = 6480;   save_sys_info.bk_var.hrd_std_vars2.stan_9_green = 7100;
        save_sys_info.bk_var.hrd_std_vars.stan_9_blue  = 8728;   save_sys_info.bk_var.hrd_std_vars2.stan_9_blue  = 9585;
        save_sys_info.bk_var.hrd_std_vars.stan_9_clear = 18561;  save_sys_info.bk_var.hrd_std_vars2.stan_9_clear = 20341;

        save_sys_info.bk_var.hrd_std_vars.stan_10_red   = 4516;  save_sys_info.bk_var.hrd_std_vars2.stan_10_red   = 4979;
        save_sys_info.bk_var.hrd_std_vars.stan_10_green = 6480;  save_sys_info.bk_var.hrd_std_vars2.stan_10_green = 7100;
        save_sys_info.bk_var.hrd_std_vars.stan_10_blue  = 8728;  save_sys_info.bk_var.hrd_std_vars2.stan_10_blue  = 9585;
        save_sys_info.bk_var.hrd_std_vars.stan_10_clear = 18561; save_sys_info.bk_var.hrd_std_vars2.stan_10_clear = 20341;
		break;
	case NITROGEN:
        save_sys_info.bk_var.hrd_std_vars.stan_0_red = 17073;        save_sys_info.bk_var.hrd_std_vars2.stan_0_red = 17073;
        save_sys_info.bk_var.hrd_std_vars.stan_0_green = 16682;      save_sys_info.bk_var.hrd_std_vars2.stan_0_green = 16682;
        save_sys_info.bk_var.hrd_std_vars.stan_0_blue = 17398;       save_sys_info.bk_var.hrd_std_vars2.stan_0_blue = 17398;
        save_sys_info.bk_var.hrd_std_vars.stan_0_clear = 48515;      save_sys_info.bk_var.hrd_std_vars2.stan_0_clear = 48515;

        save_sys_info.bk_var.hrd_std_vars.stan_1_red = 16314;        save_sys_info.bk_var.hrd_std_vars2.stan_1_red = 16314;
        save_sys_info.bk_var.hrd_std_vars.stan_1_green = 13538;      save_sys_info.bk_var.hrd_std_vars2.stan_1_green = 13538;
        save_sys_info.bk_var.hrd_std_vars.stan_1_blue = 12862;       save_sys_info.bk_var.hrd_std_vars2.stan_1_blue = 12862;
        save_sys_info.bk_var.hrd_std_vars.stan_1_clear = 40536;      save_sys_info.bk_var.hrd_std_vars2.stan_1_clear = 40536;

        save_sys_info.bk_var.hrd_std_vars.stan_2_red = 16314;        save_sys_info.bk_var.hrd_std_vars2.stan_2_red = 16314;
        save_sys_info.bk_var.hrd_std_vars.stan_2_green = 13538;      save_sys_info.bk_var.hrd_std_vars2.stan_2_green = 13538;
        save_sys_info.bk_var.hrd_std_vars.stan_2_blue = 12862;       save_sys_info.bk_var.hrd_std_vars2.stan_2_blue = 12862;
        save_sys_info.bk_var.hrd_std_vars.stan_2_clear = 40536;      save_sys_info.bk_var.hrd_std_vars2.stan_2_clear = 40536;

        save_sys_info.bk_var.hrd_std_vars.stan_3_red = 16101;        save_sys_info.bk_var.hrd_std_vars2.stan_3_red = 16101;
        save_sys_info.bk_var.hrd_std_vars.stan_3_green = 11230;      save_sys_info.bk_var.hrd_std_vars2.stan_3_green = 11230;
        save_sys_info.bk_var.hrd_std_vars.stan_3_blue = 9825;       save_sys_info.bk_var.hrd_std_vars2.stan_3_blue = 9825;
        save_sys_info.bk_var.hrd_std_vars.stan_3_clear = 35260;      save_sys_info.bk_var.hrd_std_vars2.stan_3_clear = 35260;

        save_sys_info.bk_var.hrd_std_vars.stan_4_red = 16101;        save_sys_info.bk_var.hrd_std_vars2.stan_4_red = 16101;
        save_sys_info.bk_var.hrd_std_vars.stan_4_green = 11230;       save_sys_info.bk_var.hrd_std_vars2.stan_4_green = 11230;
        save_sys_info.bk_var.hrd_std_vars.stan_4_blue = 9825;       save_sys_info.bk_var.hrd_std_vars2.stan_4_blue = 9825;
        save_sys_info.bk_var.hrd_std_vars.stan_4_clear = 35260;      save_sys_info.bk_var.hrd_std_vars2.stan_4_clear = 35260;

        save_sys_info.bk_var.hrd_std_vars.stan_5_red = 15347;        save_sys_info.bk_var.hrd_std_vars2.stan_5_red = 15347;
        save_sys_info.bk_var.hrd_std_vars.stan_5_green = 9500;       save_sys_info.bk_var.hrd_std_vars2.stan_5_green = 9500;
        save_sys_info.bk_var.hrd_std_vars.stan_5_blue = 7923;       save_sys_info.bk_var.hrd_std_vars2.stan_5_blue = 7923;
        save_sys_info.bk_var.hrd_std_vars.stan_5_clear = 31112;      save_sys_info.bk_var.hrd_std_vars2.stan_5_clear = 31112;

        save_sys_info.bk_var.hrd_std_vars.stan_6_red = 15347;         save_sys_info.bk_var.hrd_std_vars2.stan_6_red = 15347;
        save_sys_info.bk_var.hrd_std_vars.stan_6_green = 9500;       save_sys_info.bk_var.hrd_std_vars2.stan_6_green = 9500;
        save_sys_info.bk_var.hrd_std_vars.stan_6_blue = 7923;       save_sys_info.bk_var.hrd_std_vars2.stan_6_blue = 7923;
        save_sys_info.bk_var.hrd_std_vars.stan_6_clear = 31112;      save_sys_info.bk_var.hrd_std_vars2.stan_6_clear = 31112;

        save_sys_info.bk_var.hrd_std_vars.stan_7_red = 14752;         save_sys_info.bk_var.hrd_std_vars2.stan_7_red = 14752;
        save_sys_info.bk_var.hrd_std_vars.stan_7_green = 8199;       save_sys_info.bk_var.hrd_std_vars2.stan_7_green = 8199;
        save_sys_info.bk_var.hrd_std_vars.stan_7_blue = 6557;       save_sys_info.bk_var.hrd_std_vars2.stan_7_blue = 6557;
        save_sys_info.bk_var.hrd_std_vars.stan_7_clear = 28013;      save_sys_info.bk_var.hrd_std_vars2.stan_7_clear = 28013;

        save_sys_info.bk_var.hrd_std_vars.stan_8_red = 14752;         save_sys_info.bk_var.hrd_std_vars2.stan_8_red = 14752;
        save_sys_info.bk_var.hrd_std_vars.stan_8_green = 8199;       save_sys_info.bk_var.hrd_std_vars2.stan_8_green = 8199;
        save_sys_info.bk_var.hrd_std_vars.stan_8_blue = 6557;        save_sys_info.bk_var.hrd_std_vars2.stan_8_blue = 6557;
        save_sys_info.bk_var.hrd_std_vars.stan_8_clear = 28013;      save_sys_info.bk_var.hrd_std_vars2.stan_8_clear = 28013;

        save_sys_info.bk_var.hrd_std_vars.stan_9_red = 13030;         save_sys_info.bk_var.hrd_std_vars2.stan_9_red = 13030;
        save_sys_info.bk_var.hrd_std_vars.stan_9_green = 6087;       save_sys_info.bk_var.hrd_std_vars2.stan_9_green = 6087;
        save_sys_info.bk_var.hrd_std_vars.stan_9_blue = 4636;        save_sys_info.bk_var.hrd_std_vars2.stan_9_blue = 4636;
        save_sys_info.bk_var.hrd_std_vars.stan_9_clear = 22535;      save_sys_info.bk_var.hrd_std_vars2.stan_9_clear = 22535;

        save_sys_info.bk_var.hrd_std_vars.stan_10_red = 13030;        save_sys_info.bk_var.hrd_std_vars2.stan_10_red = 13030;
        save_sys_info.bk_var.hrd_std_vars.stan_10_green = 6087;      save_sys_info.bk_var.hrd_std_vars2.stan_10_green = 6087;
        save_sys_info.bk_var.hrd_std_vars.stan_10_blue = 4636;       save_sys_info.bk_var.hrd_std_vars2.stan_10_blue = 4636;
        save_sys_info.bk_var.hrd_std_vars.stan_10_clear = 22535;     save_sys_info.bk_var.hrd_std_vars2.stan_10_clear = 22535;
		break;
	case MASTER_SYS:
	case ORGANIC_CARBON:
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

        save_sys_info.bk_var.hrd_std_vars.stan_3_red = 14000;        save_sys_info.bk_var.hrd_std_vars2.stan_3_red = 14000;
        save_sys_info.bk_var.hrd_std_vars.stan_3_green = 12129;      save_sys_info.bk_var.hrd_std_vars2.stan_3_green = 12129;
        save_sys_info.bk_var.hrd_std_vars.stan_3_blue = 13953;       save_sys_info.bk_var.hrd_std_vars2.stan_3_blue = 13953;
        save_sys_info.bk_var.hrd_std_vars.stan_3_clear = 41985;      save_sys_info.bk_var.hrd_std_vars2.stan_3_clear = 41985;

        save_sys_info.bk_var.hrd_std_vars.stan_4_red = 10423;        save_sys_info.bk_var.hrd_std_vars2.stan_4_red = 10423;
        save_sys_info.bk_var.hrd_std_vars.stan_4_green = 9970;       save_sys_info.bk_var.hrd_std_vars2.stan_4_green = 9970;
        save_sys_info.bk_var.hrd_std_vars.stan_4_blue = 11836;       save_sys_info.bk_var.hrd_std_vars2.stan_4_blue = 11836;
        save_sys_info.bk_var.hrd_std_vars.stan_4_clear = 33830;      save_sys_info.bk_var.hrd_std_vars2.stan_4_clear = 33830;

        save_sys_info.bk_var.hrd_std_vars.stan_5_red = 10423;        save_sys_info.bk_var.hrd_std_vars2.stan_5_red = 10423;
        save_sys_info.bk_var.hrd_std_vars.stan_5_green = 9970;       save_sys_info.bk_var.hrd_std_vars2.stan_5_green = 9970;
        save_sys_info.bk_var.hrd_std_vars.stan_5_blue = 11836;       save_sys_info.bk_var.hrd_std_vars2.stan_5_blue = 11836;
        save_sys_info.bk_var.hrd_std_vars.stan_5_clear = 33830;      save_sys_info.bk_var.hrd_std_vars2.stan_5_clear = 33830;

        save_sys_info.bk_var.hrd_std_vars.stan_6_red = 7956;         save_sys_info.bk_var.hrd_std_vars2.stan_6_red = 7956;
        save_sys_info.bk_var.hrd_std_vars.stan_6_green = 8365;       save_sys_info.bk_var.hrd_std_vars2.stan_6_green = 8365;
        save_sys_info.bk_var.hrd_std_vars.stan_6_blue = 10230;       save_sys_info.bk_var.hrd_std_vars2.stan_6_blue = 10230;
        save_sys_info.bk_var.hrd_std_vars.stan_6_clear = 27924;      save_sys_info.bk_var.hrd_std_vars2.stan_6_clear = 27924;

        save_sys_info.bk_var.hrd_std_vars.stan_7_red = 7956;         save_sys_info.bk_var.hrd_std_vars2.stan_7_red = 7956;
        save_sys_info.bk_var.hrd_std_vars.stan_7_green = 8365;       save_sys_info.bk_var.hrd_std_vars2.stan_7_green = 8365;
        save_sys_info.bk_var.hrd_std_vars.stan_7_blue = 10230;       save_sys_info.bk_var.hrd_std_vars2.stan_7_blue = 10230;
        save_sys_info.bk_var.hrd_std_vars.stan_7_clear = 27924;      save_sys_info.bk_var.hrd_std_vars2.stan_7_clear = 27924;

        save_sys_info.bk_var.hrd_std_vars.stan_8_red = 5811;         save_sys_info.bk_var.hrd_std_vars2.stan_8_red = 5811;
        save_sys_info.bk_var.hrd_std_vars.stan_8_green = 6847;       save_sys_info.bk_var.hrd_std_vars2.stan_8_green = 6847;
        save_sys_info.bk_var.hrd_std_vars.stan_8_blue = 8673;        save_sys_info.bk_var.hrd_std_vars2.stan_8_blue = 8673;
        save_sys_info.bk_var.hrd_std_vars.stan_8_clear = 22489;      save_sys_info.bk_var.hrd_std_vars2.stan_8_clear = 22489;

        save_sys_info.bk_var.hrd_std_vars.stan_9_red = 5811;         save_sys_info.bk_var.hrd_std_vars2.stan_9_red = 5811;
        save_sys_info.bk_var.hrd_std_vars.stan_9_green = 6847;       save_sys_info.bk_var.hrd_std_vars2.stan_9_green = 6847;
        save_sys_info.bk_var.hrd_std_vars.stan_9_blue = 8673;        save_sys_info.bk_var.hrd_std_vars2.stan_9_blue = 8673;
        save_sys_info.bk_var.hrd_std_vars.stan_9_clear = 22489;      save_sys_info.bk_var.hrd_std_vars2.stan_9_clear = 22489;

        save_sys_info.bk_var.hrd_std_vars.stan_10_red = 4425;        save_sys_info.bk_var.hrd_std_vars2.stan_10_red = 4425;
        save_sys_info.bk_var.hrd_std_vars.stan_10_green = 5785;      save_sys_info.bk_var.hrd_std_vars2.stan_10_green = 5785;
        save_sys_info.bk_var.hrd_std_vars.stan_10_blue = 7557;       save_sys_info.bk_var.hrd_std_vars2.stan_10_blue = 7557;
        save_sys_info.bk_var.hrd_std_vars.stan_10_clear = 18768;     save_sys_info.bk_var.hrd_std_vars2.stan_10_clear = 18768;
		break;
	}
}
#endif

#ifdef OLD_CAL
void init_hrd_strd_old(void)
{
	switch(save_sys_info.bk_var.curr_sys_add)
	{
	case MAGNESIUM:

        save_sys_info.bk_var.hrd_std_vars.stan_0_red = 42575;        save_sys_info.bk_var.hrd_std_vars2.stan_0_red = 42575;
        save_sys_info.bk_var.hrd_std_vars.stan_0_green = 42575;      save_sys_info.bk_var.hrd_std_vars2.stan_0_green = 42575;
        save_sys_info.bk_var.hrd_std_vars.stan_0_blue = 42575;       save_sys_info.bk_var.hrd_std_vars2.stan_0_blue = 42575;
        save_sys_info.bk_var.hrd_std_vars.stan_0_clear = 42575;      save_sys_info.bk_var.hrd_std_vars2.stan_0_clear = 42575;

        save_sys_info.bk_var.hrd_std_vars.stan_1_red = 41843;        save_sys_info.bk_var.hrd_std_vars2.stan_1_red = 41843;
        save_sys_info.bk_var.hrd_std_vars.stan_1_green = 41843;      save_sys_info.bk_var.hrd_std_vars2.stan_1_green = 41843;
        save_sys_info.bk_var.hrd_std_vars.stan_1_blue = 41843;       save_sys_info.bk_var.hrd_std_vars2.stan_1_blue = 41843;
        save_sys_info.bk_var.hrd_std_vars.stan_1_clear = 41843;      save_sys_info.bk_var.hrd_std_vars2.stan_1_clear = 41843;

        save_sys_info.bk_var.hrd_std_vars.stan_2_red = 41843;        save_sys_info.bk_var.hrd_std_vars2.stan_2_red = 41843;
        save_sys_info.bk_var.hrd_std_vars.stan_2_green = 41843;      save_sys_info.bk_var.hrd_std_vars2.stan_2_green = 41843;
        save_sys_info.bk_var.hrd_std_vars.stan_2_blue = 41843;       save_sys_info.bk_var.hrd_std_vars2.stan_2_blue = 41843;
        save_sys_info.bk_var.hrd_std_vars.stan_2_clear = 41843;      save_sys_info.bk_var.hrd_std_vars2.stan_2_clear = 41843;

        save_sys_info.bk_var.hrd_std_vars.stan_3_red = 41169;        save_sys_info.bk_var.hrd_std_vars2.stan_3_red = 41169;
        save_sys_info.bk_var.hrd_std_vars.stan_3_green = 41169;      save_sys_info.bk_var.hrd_std_vars2.stan_3_green = 41169;
        save_sys_info.bk_var.hrd_std_vars.stan_3_blue = 41169;       save_sys_info.bk_var.hrd_std_vars2.stan_3_blue = 41169;
        save_sys_info.bk_var.hrd_std_vars.stan_3_clear = 41169;      save_sys_info.bk_var.hrd_std_vars2.stan_3_clear = 41169;

        save_sys_info.bk_var.hrd_std_vars.stan_4_red = 41169;        save_sys_info.bk_var.hrd_std_vars2.stan_4_red = 41169;
        save_sys_info.bk_var.hrd_std_vars.stan_4_green = 41169;       save_sys_info.bk_var.hrd_std_vars2.stan_4_green = 41169;
        save_sys_info.bk_var.hrd_std_vars.stan_4_blue = 41169;       save_sys_info.bk_var.hrd_std_vars2.stan_4_blue = 41169;
        save_sys_info.bk_var.hrd_std_vars.stan_4_clear = 41169;      save_sys_info.bk_var.hrd_std_vars2.stan_4_clear = 41169;

        save_sys_info.bk_var.hrd_std_vars.stan_5_red = 40604;        save_sys_info.bk_var.hrd_std_vars2.stan_5_red = 40604;
        save_sys_info.bk_var.hrd_std_vars.stan_5_green = 40604;       save_sys_info.bk_var.hrd_std_vars2.stan_5_green = 40604;
        save_sys_info.bk_var.hrd_std_vars.stan_5_blue = 40604;       save_sys_info.bk_var.hrd_std_vars2.stan_5_blue = 40604;
        save_sys_info.bk_var.hrd_std_vars.stan_5_clear = 40604;      save_sys_info.bk_var.hrd_std_vars2.stan_5_clear = 40604;

        save_sys_info.bk_var.hrd_std_vars.stan_6_red = 40604;         save_sys_info.bk_var.hrd_std_vars2.stan_6_red = 40604;
        save_sys_info.bk_var.hrd_std_vars.stan_6_green = 40604;       save_sys_info.bk_var.hrd_std_vars2.stan_6_green = 40604;
        save_sys_info.bk_var.hrd_std_vars.stan_6_blue = 40604;       save_sys_info.bk_var.hrd_std_vars2.stan_6_blue = 40604;
        save_sys_info.bk_var.hrd_std_vars.stan_6_clear = 40604;      save_sys_info.bk_var.hrd_std_vars2.stan_6_clear = 40604;

        save_sys_info.bk_var.hrd_std_vars.stan_7_red = 40018;         save_sys_info.bk_var.hrd_std_vars2.stan_7_red = 40018;
        save_sys_info.bk_var.hrd_std_vars.stan_7_green = 40018;       save_sys_info.bk_var.hrd_std_vars2.stan_7_green = 40018;
        save_sys_info.bk_var.hrd_std_vars.stan_7_blue = 40018;       save_sys_info.bk_var.hrd_std_vars2.stan_7_blue = 40018;
        save_sys_info.bk_var.hrd_std_vars.stan_7_clear = 40018;      save_sys_info.bk_var.hrd_std_vars2.stan_7_clear = 40018;

        save_sys_info.bk_var.hrd_std_vars.stan_8_red = 40018;         save_sys_info.bk_var.hrd_std_vars2.stan_8_red = 40018;
        save_sys_info.bk_var.hrd_std_vars.stan_8_green = 40018;       save_sys_info.bk_var.hrd_std_vars2.stan_8_green = 40018;
        save_sys_info.bk_var.hrd_std_vars.stan_8_blue = 40018;        save_sys_info.bk_var.hrd_std_vars2.stan_8_blue = 40018;
        save_sys_info.bk_var.hrd_std_vars.stan_8_clear = 40018;      save_sys_info.bk_var.hrd_std_vars2.stan_8_clear = 40018;

        save_sys_info.bk_var.hrd_std_vars.stan_9_red = 20160;         save_sys_info.bk_var.hrd_std_vars2.stan_9_red = 20160;
        save_sys_info.bk_var.hrd_std_vars.stan_9_green = 20160;       save_sys_info.bk_var.hrd_std_vars2.stan_9_green = 20160;
        save_sys_info.bk_var.hrd_std_vars.stan_9_blue = 20160;        save_sys_info.bk_var.hrd_std_vars2.stan_9_blue = 20160;
        save_sys_info.bk_var.hrd_std_vars.stan_9_clear = 20160;      save_sys_info.bk_var.hrd_std_vars2.stan_9_clear = 20160;

        save_sys_info.bk_var.hrd_std_vars.stan_10_red = 11148;        save_sys_info.bk_var.hrd_std_vars2.stan_10_red = 11148;
        save_sys_info.bk_var.hrd_std_vars.stan_10_green = 11148;      save_sys_info.bk_var.hrd_std_vars2.stan_10_green = 11148;
        save_sys_info.bk_var.hrd_std_vars.stan_10_blue = 11148;       save_sys_info.bk_var.hrd_std_vars2.stan_10_blue = 11148;
        save_sys_info.bk_var.hrd_std_vars.stan_10_clear = 11148;     save_sys_info.bk_var.hrd_std_vars2.stan_10_clear = 11148;
		break;
	case IRON:
        save_sys_info.bk_var.hrd_std_vars.stan_0_red = 39950;        save_sys_info.bk_var.hrd_std_vars2.stan_0_red = 39950;
        save_sys_info.bk_var.hrd_std_vars.stan_0_green = 39950;      save_sys_info.bk_var.hrd_std_vars2.stan_0_green = 39950;
        save_sys_info.bk_var.hrd_std_vars.stan_0_blue = 39950;       save_sys_info.bk_var.hrd_std_vars2.stan_0_blue = 39950;
        save_sys_info.bk_var.hrd_std_vars.stan_0_clear = 39950;      save_sys_info.bk_var.hrd_std_vars2.stan_0_clear = 39950;

        save_sys_info.bk_var.hrd_std_vars.stan_1_red = 36049;        save_sys_info.bk_var.hrd_std_vars2.stan_1_red = 36049;
        save_sys_info.bk_var.hrd_std_vars.stan_1_green = 36049;      save_sys_info.bk_var.hrd_std_vars2.stan_1_green = 36049;
        save_sys_info.bk_var.hrd_std_vars.stan_1_blue = 36049;       save_sys_info.bk_var.hrd_std_vars2.stan_1_blue = 36049;
        save_sys_info.bk_var.hrd_std_vars.stan_1_clear = 36049;      save_sys_info.bk_var.hrd_std_vars2.stan_1_clear = 36049;

        save_sys_info.bk_var.hrd_std_vars.stan_2_red = 36049;        save_sys_info.bk_var.hrd_std_vars2.stan_2_red = 36049;
        save_sys_info.bk_var.hrd_std_vars.stan_2_green = 36049;      save_sys_info.bk_var.hrd_std_vars2.stan_2_green = 36049;
        save_sys_info.bk_var.hrd_std_vars.stan_2_blue = 36049;       save_sys_info.bk_var.hrd_std_vars2.stan_2_blue = 36049;
        save_sys_info.bk_var.hrd_std_vars.stan_2_clear = 36049;      save_sys_info.bk_var.hrd_std_vars2.stan_2_clear = 36049;

        save_sys_info.bk_var.hrd_std_vars.stan_3_red = 28471;        save_sys_info.bk_var.hrd_std_vars2.stan_3_red = 28471;
        save_sys_info.bk_var.hrd_std_vars.stan_3_green = 28471;      save_sys_info.bk_var.hrd_std_vars2.stan_3_green = 28471;
        save_sys_info.bk_var.hrd_std_vars.stan_3_blue = 28471;       save_sys_info.bk_var.hrd_std_vars2.stan_3_blue = 28471;
        save_sys_info.bk_var.hrd_std_vars.stan_3_clear = 28471;      save_sys_info.bk_var.hrd_std_vars2.stan_3_clear = 28471;

        save_sys_info.bk_var.hrd_std_vars.stan_4_red = 28471;        save_sys_info.bk_var.hrd_std_vars2.stan_4_red = 28471;
        save_sys_info.bk_var.hrd_std_vars.stan_4_green = 28471;       save_sys_info.bk_var.hrd_std_vars2.stan_4_green = 28471;
        save_sys_info.bk_var.hrd_std_vars.stan_4_blue = 28471;       save_sys_info.bk_var.hrd_std_vars2.stan_4_blue = 28471;
        save_sys_info.bk_var.hrd_std_vars.stan_4_clear = 28471;      save_sys_info.bk_var.hrd_std_vars2.stan_4_clear = 28471;

        save_sys_info.bk_var.hrd_std_vars.stan_5_red = 19374;        save_sys_info.bk_var.hrd_std_vars2.stan_5_red = 19374;
        save_sys_info.bk_var.hrd_std_vars.stan_5_green = 19374;       save_sys_info.bk_var.hrd_std_vars2.stan_5_green = 19374;
        save_sys_info.bk_var.hrd_std_vars.stan_5_blue = 19374;       save_sys_info.bk_var.hrd_std_vars2.stan_5_blue = 19374;
        save_sys_info.bk_var.hrd_std_vars.stan_5_clear = 19374;      save_sys_info.bk_var.hrd_std_vars2.stan_5_clear = 19374;

        save_sys_info.bk_var.hrd_std_vars.stan_6_red = 19374;         save_sys_info.bk_var.hrd_std_vars2.stan_6_red = 19374;
        save_sys_info.bk_var.hrd_std_vars.stan_6_green = 19374;       save_sys_info.bk_var.hrd_std_vars2.stan_6_green = 19374;
        save_sys_info.bk_var.hrd_std_vars.stan_6_blue = 19374;       save_sys_info.bk_var.hrd_std_vars2.stan_6_blue = 19374;
        save_sys_info.bk_var.hrd_std_vars.stan_6_clear = 19374;      save_sys_info.bk_var.hrd_std_vars2.stan_6_clear = 19374;

        save_sys_info.bk_var.hrd_std_vars.stan_7_red = 13313;         save_sys_info.bk_var.hrd_std_vars2.stan_7_red = 13313;
        save_sys_info.bk_var.hrd_std_vars.stan_7_green = 13313;       save_sys_info.bk_var.hrd_std_vars2.stan_7_green = 13313;
        save_sys_info.bk_var.hrd_std_vars.stan_7_blue = 13313;       save_sys_info.bk_var.hrd_std_vars2.stan_7_blue = 13313;
        save_sys_info.bk_var.hrd_std_vars.stan_7_clear = 13313;      save_sys_info.bk_var.hrd_std_vars2.stan_7_clear = 13313;

        save_sys_info.bk_var.hrd_std_vars.stan_8_red = 13313;         save_sys_info.bk_var.hrd_std_vars2.stan_8_red = 13313;
        save_sys_info.bk_var.hrd_std_vars.stan_8_green = 13313;       save_sys_info.bk_var.hrd_std_vars2.stan_8_green = 13313;
        save_sys_info.bk_var.hrd_std_vars.stan_8_blue = 13313;        save_sys_info.bk_var.hrd_std_vars2.stan_8_blue = 13313;
        save_sys_info.bk_var.hrd_std_vars.stan_8_clear = 13313;      save_sys_info.bk_var.hrd_std_vars2.stan_8_clear = 13313;

        save_sys_info.bk_var.hrd_std_vars.stan_9_red = 2010;         save_sys_info.bk_var.hrd_std_vars2.stan_9_red = 2010;
        save_sys_info.bk_var.hrd_std_vars.stan_9_green = 2010;       save_sys_info.bk_var.hrd_std_vars2.stan_9_green = 2010;
        save_sys_info.bk_var.hrd_std_vars.stan_9_blue = 2010;        save_sys_info.bk_var.hrd_std_vars2.stan_9_blue = 2010;
        save_sys_info.bk_var.hrd_std_vars.stan_9_clear = 2010;      save_sys_info.bk_var.hrd_std_vars2.stan_9_clear = 2010;

        save_sys_info.bk_var.hrd_std_vars.stan_10_red = 530;        save_sys_info.bk_var.hrd_std_vars2.stan_10_red = 530;
        save_sys_info.bk_var.hrd_std_vars.stan_10_green = 530;      save_sys_info.bk_var.hrd_std_vars2.stan_10_green = 530;
        save_sys_info.bk_var.hrd_std_vars.stan_10_blue = 530;       save_sys_info.bk_var.hrd_std_vars2.stan_10_blue = 530;
        save_sys_info.bk_var.hrd_std_vars.stan_10_clear = 530;     save_sys_info.bk_var.hrd_std_vars2.stan_10_clear = 530;
		break;
	case COPPER:
        save_sys_info.bk_var.hrd_std_vars.stan_0_red = 34750;        save_sys_info.bk_var.hrd_std_vars2.stan_0_red = 34750;
        save_sys_info.bk_var.hrd_std_vars.stan_0_green = 34750;      save_sys_info.bk_var.hrd_std_vars2.stan_0_green = 34750;
        save_sys_info.bk_var.hrd_std_vars.stan_0_blue = 34750;       save_sys_info.bk_var.hrd_std_vars2.stan_0_blue = 34750;
        save_sys_info.bk_var.hrd_std_vars.stan_0_clear = 34750;      save_sys_info.bk_var.hrd_std_vars2.stan_0_clear = 34750;

        save_sys_info.bk_var.hrd_std_vars.stan_1_red = 34709;        save_sys_info.bk_var.hrd_std_vars2.stan_1_red = 34709;
        save_sys_info.bk_var.hrd_std_vars.stan_1_green = 34709;      save_sys_info.bk_var.hrd_std_vars2.stan_1_green = 34709;
        save_sys_info.bk_var.hrd_std_vars.stan_1_blue = 34709;       save_sys_info.bk_var.hrd_std_vars2.stan_1_blue = 34709;
        save_sys_info.bk_var.hrd_std_vars.stan_1_clear = 34709;      save_sys_info.bk_var.hrd_std_vars2.stan_1_clear = 34709;

        save_sys_info.bk_var.hrd_std_vars.stan_2_red = 34709;        save_sys_info.bk_var.hrd_std_vars2.stan_2_red = 34709;
        save_sys_info.bk_var.hrd_std_vars.stan_2_green = 34709;      save_sys_info.bk_var.hrd_std_vars2.stan_2_green = 34709;
        save_sys_info.bk_var.hrd_std_vars.stan_2_blue = 34709;       save_sys_info.bk_var.hrd_std_vars2.stan_2_blue = 34709;
        save_sys_info.bk_var.hrd_std_vars.stan_2_clear = 34709;      save_sys_info.bk_var.hrd_std_vars2.stan_2_clear = 34709;

        save_sys_info.bk_var.hrd_std_vars.stan_3_red = 34556;        save_sys_info.bk_var.hrd_std_vars2.stan_3_red = 34556;
        save_sys_info.bk_var.hrd_std_vars.stan_3_green = 34556;      save_sys_info.bk_var.hrd_std_vars2.stan_3_green = 34556;
        save_sys_info.bk_var.hrd_std_vars.stan_3_blue = 34556;       save_sys_info.bk_var.hrd_std_vars2.stan_3_blue = 34556;
        save_sys_info.bk_var.hrd_std_vars.stan_3_clear = 34556;      save_sys_info.bk_var.hrd_std_vars2.stan_3_clear = 34556;

        save_sys_info.bk_var.hrd_std_vars.stan_4_red = 34556;        save_sys_info.bk_var.hrd_std_vars2.stan_4_red = 34556;
        save_sys_info.bk_var.hrd_std_vars.stan_4_green = 34556;       save_sys_info.bk_var.hrd_std_vars2.stan_4_green = 34556;
        save_sys_info.bk_var.hrd_std_vars.stan_4_blue = 34556;       save_sys_info.bk_var.hrd_std_vars2.stan_4_blue = 34556;
        save_sys_info.bk_var.hrd_std_vars.stan_4_clear = 34556;      save_sys_info.bk_var.hrd_std_vars2.stan_4_clear = 34556;

        save_sys_info.bk_var.hrd_std_vars.stan_5_red = 34215;        save_sys_info.bk_var.hrd_std_vars2.stan_5_red = 34215;
        save_sys_info.bk_var.hrd_std_vars.stan_5_green = 34215;       save_sys_info.bk_var.hrd_std_vars2.stan_5_green = 34215;
        save_sys_info.bk_var.hrd_std_vars.stan_5_blue = 34215;       save_sys_info.bk_var.hrd_std_vars2.stan_5_blue = 34215;
        save_sys_info.bk_var.hrd_std_vars.stan_5_clear = 34215;      save_sys_info.bk_var.hrd_std_vars2.stan_5_clear = 34215;

        save_sys_info.bk_var.hrd_std_vars.stan_6_red = 34215;         save_sys_info.bk_var.hrd_std_vars2.stan_6_red = 34215;
        save_sys_info.bk_var.hrd_std_vars.stan_6_green = 34215;       save_sys_info.bk_var.hrd_std_vars2.stan_6_green = 34215;
        save_sys_info.bk_var.hrd_std_vars.stan_6_blue = 34215;       save_sys_info.bk_var.hrd_std_vars2.stan_6_blue = 34215;
        save_sys_info.bk_var.hrd_std_vars.stan_6_clear = 34215;      save_sys_info.bk_var.hrd_std_vars2.stan_6_clear = 34215;

        save_sys_info.bk_var.hrd_std_vars.stan_7_red = 33682;         save_sys_info.bk_var.hrd_std_vars2.stan_7_red = 33682;
        save_sys_info.bk_var.hrd_std_vars.stan_7_green = 33682;       save_sys_info.bk_var.hrd_std_vars2.stan_7_green = 33682;
        save_sys_info.bk_var.hrd_std_vars.stan_7_blue = 33682;       save_sys_info.bk_var.hrd_std_vars2.stan_7_blue = 33682;
        save_sys_info.bk_var.hrd_std_vars.stan_7_clear = 33682;      save_sys_info.bk_var.hrd_std_vars2.stan_7_clear = 33682;

        save_sys_info.bk_var.hrd_std_vars.stan_8_red = 33682;         save_sys_info.bk_var.hrd_std_vars2.stan_8_red = 33682;
        save_sys_info.bk_var.hrd_std_vars.stan_8_green = 33682;       save_sys_info.bk_var.hrd_std_vars2.stan_8_green = 33682;
        save_sys_info.bk_var.hrd_std_vars.stan_8_blue = 33682;        save_sys_info.bk_var.hrd_std_vars2.stan_8_blue = 33682;
        save_sys_info.bk_var.hrd_std_vars.stan_8_clear = 33682;      save_sys_info.bk_var.hrd_std_vars2.stan_8_clear = 33682;

        save_sys_info.bk_var.hrd_std_vars.stan_9_red = 28996;         save_sys_info.bk_var.hrd_std_vars2.stan_9_red = 28996;
        save_sys_info.bk_var.hrd_std_vars.stan_9_green = 28996;       save_sys_info.bk_var.hrd_std_vars2.stan_9_green = 28996;
        save_sys_info.bk_var.hrd_std_vars.stan_9_blue = 28996;        save_sys_info.bk_var.hrd_std_vars2.stan_9_blue = 28996;
        save_sys_info.bk_var.hrd_std_vars.stan_9_clear = 28996;      save_sys_info.bk_var.hrd_std_vars2.stan_9_clear = 28996;

        save_sys_info.bk_var.hrd_std_vars.stan_10_red = 24706;       save_sys_info.bk_var.hrd_std_vars2.stan_10_red = 24706;
        save_sys_info.bk_var.hrd_std_vars.stan_10_green = 24706;     save_sys_info.bk_var.hrd_std_vars2.stan_10_green = 24706;
        save_sys_info.bk_var.hrd_std_vars.stan_10_blue = 24706;      save_sys_info.bk_var.hrd_std_vars2.stan_10_blue = 24706;
        save_sys_info.bk_var.hrd_std_vars.stan_10_clear = 24706;     save_sys_info.bk_var.hrd_std_vars2.stan_10_clear = 24706;
		break;
	case ZINC:
        save_sys_info.bk_var.hrd_std_vars.stan_0_red = 7404;        save_sys_info.bk_var.hrd_std_vars2.stan_0_red = 7404;
        save_sys_info.bk_var.hrd_std_vars.stan_0_green = 7404;      save_sys_info.bk_var.hrd_std_vars2.stan_0_green = 7404;
        save_sys_info.bk_var.hrd_std_vars.stan_0_blue = 7404;       save_sys_info.bk_var.hrd_std_vars2.stan_0_blue = 7404;
        save_sys_info.bk_var.hrd_std_vars.stan_0_clear = 7404;      save_sys_info.bk_var.hrd_std_vars2.stan_0_clear = 7404;

        save_sys_info.bk_var.hrd_std_vars.stan_1_red = 6641;        save_sys_info.bk_var.hrd_std_vars2.stan_1_red = 6641;
        save_sys_info.bk_var.hrd_std_vars.stan_1_green = 6641;      save_sys_info.bk_var.hrd_std_vars2.stan_1_green = 6641;
        save_sys_info.bk_var.hrd_std_vars.stan_1_blue = 6641;       save_sys_info.bk_var.hrd_std_vars2.stan_1_blue = 6641;
        save_sys_info.bk_var.hrd_std_vars.stan_1_clear = 6641;      save_sys_info.bk_var.hrd_std_vars2.stan_1_clear = 6641;

        save_sys_info.bk_var.hrd_std_vars.stan_2_red = 6641;        save_sys_info.bk_var.hrd_std_vars2.stan_2_red = 6641;
        save_sys_info.bk_var.hrd_std_vars.stan_2_green = 6641;      save_sys_info.bk_var.hrd_std_vars2.stan_2_green = 6641;
        save_sys_info.bk_var.hrd_std_vars.stan_2_blue = 6641;       save_sys_info.bk_var.hrd_std_vars2.stan_2_blue = 6641;
        save_sys_info.bk_var.hrd_std_vars.stan_2_clear = 6641;      save_sys_info.bk_var.hrd_std_vars2.stan_2_clear = 6641;

        save_sys_info.bk_var.hrd_std_vars.stan_3_red = 6332;        save_sys_info.bk_var.hrd_std_vars2.stan_3_red = 6332;
        save_sys_info.bk_var.hrd_std_vars.stan_3_green = 6332;      save_sys_info.bk_var.hrd_std_vars2.stan_3_green = 6332;
        save_sys_info.bk_var.hrd_std_vars.stan_3_blue = 6332;       save_sys_info.bk_var.hrd_std_vars2.stan_3_blue = 6332;
        save_sys_info.bk_var.hrd_std_vars.stan_3_clear = 6332;      save_sys_info.bk_var.hrd_std_vars2.stan_3_clear = 6332;

        save_sys_info.bk_var.hrd_std_vars.stan_4_red = 6332;        save_sys_info.bk_var.hrd_std_vars2.stan_4_red = 6332;
        save_sys_info.bk_var.hrd_std_vars.stan_4_green = 6332;      save_sys_info.bk_var.hrd_std_vars2.stan_4_green = 6332;
        save_sys_info.bk_var.hrd_std_vars.stan_4_blue = 6332;       save_sys_info.bk_var.hrd_std_vars2.stan_4_blue = 6332;
        save_sys_info.bk_var.hrd_std_vars.stan_4_clear = 6332;      save_sys_info.bk_var.hrd_std_vars2.stan_4_clear = 6332;

        save_sys_info.bk_var.hrd_std_vars.stan_5_red = 6242;        save_sys_info.bk_var.hrd_std_vars2.stan_5_red = 6242;
        save_sys_info.bk_var.hrd_std_vars.stan_5_green = 6242;      save_sys_info.bk_var.hrd_std_vars2.stan_5_green = 6242;
        save_sys_info.bk_var.hrd_std_vars.stan_5_blue = 6242;       save_sys_info.bk_var.hrd_std_vars2.stan_5_blue = 6242;
        save_sys_info.bk_var.hrd_std_vars.stan_5_clear = 6242;      save_sys_info.bk_var.hrd_std_vars2.stan_5_clear = 6242;

        save_sys_info.bk_var.hrd_std_vars.stan_6_red = 6242;        save_sys_info.bk_var.hrd_std_vars2.stan_6_red = 6242;
        save_sys_info.bk_var.hrd_std_vars.stan_6_green = 6242;      save_sys_info.bk_var.hrd_std_vars2.stan_6_green = 6242;
        save_sys_info.bk_var.hrd_std_vars.stan_6_blue = 6242;       save_sys_info.bk_var.hrd_std_vars2.stan_6_blue = 6242;
        save_sys_info.bk_var.hrd_std_vars.stan_6_clear = 6242;      save_sys_info.bk_var.hrd_std_vars2.stan_6_clear = 6242;

        save_sys_info.bk_var.hrd_std_vars.stan_7_red = 5629;        save_sys_info.bk_var.hrd_std_vars2.stan_7_red = 5629;
        save_sys_info.bk_var.hrd_std_vars.stan_7_green = 5629;      save_sys_info.bk_var.hrd_std_vars2.stan_7_green = 5629;
        save_sys_info.bk_var.hrd_std_vars.stan_7_blue = 5629;       save_sys_info.bk_var.hrd_std_vars2.stan_7_blue = 5629;
        save_sys_info.bk_var.hrd_std_vars.stan_7_clear = 5629;      save_sys_info.bk_var.hrd_std_vars2.stan_7_clear = 5629;

        save_sys_info.bk_var.hrd_std_vars.stan_8_red = 5629;        save_sys_info.bk_var.hrd_std_vars2.stan_8_red = 5629;
        save_sys_info.bk_var.hrd_std_vars.stan_8_green = 5629;      save_sys_info.bk_var.hrd_std_vars2.stan_8_green = 5629;
        save_sys_info.bk_var.hrd_std_vars.stan_8_blue = 5629;       save_sys_info.bk_var.hrd_std_vars2.stan_8_blue = 5629;
        save_sys_info.bk_var.hrd_std_vars.stan_8_clear = 5629;      save_sys_info.bk_var.hrd_std_vars2.stan_8_clear = 5629;

        save_sys_info.bk_var.hrd_std_vars.stan_9_red = 4716;        save_sys_info.bk_var.hrd_std_vars2.stan_9_red = 4716;
        save_sys_info.bk_var.hrd_std_vars.stan_9_green = 4716;      save_sys_info.bk_var.hrd_std_vars2.stan_9_green = 4716;
        save_sys_info.bk_var.hrd_std_vars.stan_9_blue = 4716;       save_sys_info.bk_var.hrd_std_vars2.stan_9_blue = 4716;
        save_sys_info.bk_var.hrd_std_vars.stan_9_clear = 4716;      save_sys_info.bk_var.hrd_std_vars2.stan_9_clear = 4716;

        save_sys_info.bk_var.hrd_std_vars.stan_10_red = 2907;       save_sys_info.bk_var.hrd_std_vars2.stan_10_red = 2907;
        save_sys_info.bk_var.hrd_std_vars.stan_10_green = 2907;     save_sys_info.bk_var.hrd_std_vars2.stan_10_green = 2907;
        save_sys_info.bk_var.hrd_std_vars.stan_10_blue = 2907;      save_sys_info.bk_var.hrd_std_vars2.stan_10_blue = 2907;
        save_sys_info.bk_var.hrd_std_vars.stan_10_clear = 2907;     save_sys_info.bk_var.hrd_std_vars2.stan_10_clear = 2907;
		break;
	case BORON:
        save_sys_info.bk_var.hrd_std_vars.stan_0_red = 20626;       save_sys_info.bk_var.hrd_std_vars2.stan_0_red = 20626;
        save_sys_info.bk_var.hrd_std_vars.stan_0_green = 20626;     save_sys_info.bk_var.hrd_std_vars2.stan_0_green = 20626;
        save_sys_info.bk_var.hrd_std_vars.stan_0_blue = 20626;      save_sys_info.bk_var.hrd_std_vars2.stan_0_blue = 20626;
        save_sys_info.bk_var.hrd_std_vars.stan_0_clear = 20626;     save_sys_info.bk_var.hrd_std_vars2.stan_0_clear = 20626;

        save_sys_info.bk_var.hrd_std_vars.stan_1_red = 20271;       save_sys_info.bk_var.hrd_std_vars2.stan_1_red = 20271;
        save_sys_info.bk_var.hrd_std_vars.stan_1_green = 20271;     save_sys_info.bk_var.hrd_std_vars2.stan_1_green = 20271;
        save_sys_info.bk_var.hrd_std_vars.stan_1_blue = 20271;      save_sys_info.bk_var.hrd_std_vars2.stan_1_blue = 20271;
        save_sys_info.bk_var.hrd_std_vars.stan_1_clear = 20271;     save_sys_info.bk_var.hrd_std_vars2.stan_1_clear = 20271;

        save_sys_info.bk_var.hrd_std_vars.stan_2_red = 20271;       save_sys_info.bk_var.hrd_std_vars2.stan_2_red = 20271;
        save_sys_info.bk_var.hrd_std_vars.stan_2_green = 20271;     save_sys_info.bk_var.hrd_std_vars2.stan_2_green = 20271;
        save_sys_info.bk_var.hrd_std_vars.stan_2_blue = 20271;      save_sys_info.bk_var.hrd_std_vars2.stan_2_blue = 20271;
        save_sys_info.bk_var.hrd_std_vars.stan_2_clear = 20271;     save_sys_info.bk_var.hrd_std_vars2.stan_2_clear = 20271;

        save_sys_info.bk_var.hrd_std_vars.stan_3_red = 19623;        save_sys_info.bk_var.hrd_std_vars2.stan_3_red = 19623;
        save_sys_info.bk_var.hrd_std_vars.stan_3_green = 19623;      save_sys_info.bk_var.hrd_std_vars2.stan_3_green = 19623;
        save_sys_info.bk_var.hrd_std_vars.stan_3_blue = 19623;       save_sys_info.bk_var.hrd_std_vars2.stan_3_blue = 19623;
        save_sys_info.bk_var.hrd_std_vars.stan_3_clear = 19623;      save_sys_info.bk_var.hrd_std_vars2.stan_3_clear = 19623;

        save_sys_info.bk_var.hrd_std_vars.stan_4_red = 19623;        save_sys_info.bk_var.hrd_std_vars2.stan_4_red = 19623;
        save_sys_info.bk_var.hrd_std_vars.stan_4_green = 19623;       save_sys_info.bk_var.hrd_std_vars2.stan_4_green = 19623;
        save_sys_info.bk_var.hrd_std_vars.stan_4_blue = 19623;       save_sys_info.bk_var.hrd_std_vars2.stan_4_blue = 19623;
        save_sys_info.bk_var.hrd_std_vars.stan_4_clear = 19623;      save_sys_info.bk_var.hrd_std_vars2.stan_4_clear = 19623;

        save_sys_info.bk_var.hrd_std_vars.stan_5_red = 18836;        save_sys_info.bk_var.hrd_std_vars2.stan_5_red = 18836;
        save_sys_info.bk_var.hrd_std_vars.stan_5_green = 18836;       save_sys_info.bk_var.hrd_std_vars2.stan_5_green = 18836;
        save_sys_info.bk_var.hrd_std_vars.stan_5_blue = 18836;       save_sys_info.bk_var.hrd_std_vars2.stan_5_blue = 18836;
        save_sys_info.bk_var.hrd_std_vars.stan_5_clear = 18836;      save_sys_info.bk_var.hrd_std_vars2.stan_5_clear = 18836;

        save_sys_info.bk_var.hrd_std_vars.stan_6_red = 18836;         save_sys_info.bk_var.hrd_std_vars2.stan_6_red = 18836;
        save_sys_info.bk_var.hrd_std_vars.stan_6_green = 18836;       save_sys_info.bk_var.hrd_std_vars2.stan_6_green = 18836;
        save_sys_info.bk_var.hrd_std_vars.stan_6_blue = 18836;       save_sys_info.bk_var.hrd_std_vars2.stan_6_blue = 18836;
        save_sys_info.bk_var.hrd_std_vars.stan_6_clear = 18836;      save_sys_info.bk_var.hrd_std_vars2.stan_6_clear = 18836;

        save_sys_info.bk_var.hrd_std_vars.stan_7_red = 18544;         save_sys_info.bk_var.hrd_std_vars2.stan_7_red = 18544;
        save_sys_info.bk_var.hrd_std_vars.stan_7_green = 18544;       save_sys_info.bk_var.hrd_std_vars2.stan_7_green = 18544;
        save_sys_info.bk_var.hrd_std_vars.stan_7_blue = 18544;       save_sys_info.bk_var.hrd_std_vars2.stan_7_blue = 18544;
        save_sys_info.bk_var.hrd_std_vars.stan_7_clear = 18544;      save_sys_info.bk_var.hrd_std_vars2.stan_7_clear = 18544;

        save_sys_info.bk_var.hrd_std_vars.stan_8_red = 18544;         save_sys_info.bk_var.hrd_std_vars2.stan_8_red = 18544;
        save_sys_info.bk_var.hrd_std_vars.stan_8_green = 18544;       save_sys_info.bk_var.hrd_std_vars2.stan_8_green = 18544;
        save_sys_info.bk_var.hrd_std_vars.stan_8_blue = 18544;        save_sys_info.bk_var.hrd_std_vars2.stan_8_blue = 18544;
        save_sys_info.bk_var.hrd_std_vars.stan_8_clear = 18544;      save_sys_info.bk_var.hrd_std_vars2.stan_8_clear = 18544;

        save_sys_info.bk_var.hrd_std_vars.stan_9_red = 11999;         save_sys_info.bk_var.hrd_std_vars2.stan_9_red = 11999;
        save_sys_info.bk_var.hrd_std_vars.stan_9_green = 11999;       save_sys_info.bk_var.hrd_std_vars2.stan_9_green = 11999;
        save_sys_info.bk_var.hrd_std_vars.stan_9_blue = 11999;        save_sys_info.bk_var.hrd_std_vars2.stan_9_blue = 11999;
        save_sys_info.bk_var.hrd_std_vars.stan_9_clear = 11999;      save_sys_info.bk_var.hrd_std_vars2.stan_9_clear = 11999;

        save_sys_info.bk_var.hrd_std_vars.stan_10_red = 7629;        save_sys_info.bk_var.hrd_std_vars2.stan_10_red = 7629;
        save_sys_info.bk_var.hrd_std_vars.stan_10_green = 7629;      save_sys_info.bk_var.hrd_std_vars2.stan_10_green = 7629;
        save_sys_info.bk_var.hrd_std_vars.stan_10_blue = 7629;       save_sys_info.bk_var.hrd_std_vars2.stan_10_blue = 7629;
        save_sys_info.bk_var.hrd_std_vars.stan_10_clear = 7629;     save_sys_info.bk_var.hrd_std_vars2.stan_10_clear = 7629;
		break;
	case SULPHUR:
        save_sys_info.bk_var.hrd_std_vars.stan_0_red = 23030;        save_sys_info.bk_var.hrd_std_vars2.stan_0_red = 23030;
        save_sys_info.bk_var.hrd_std_vars.stan_0_green = 23030;      save_sys_info.bk_var.hrd_std_vars2.stan_0_green = 23030;
        save_sys_info.bk_var.hrd_std_vars.stan_0_blue = 23030;       save_sys_info.bk_var.hrd_std_vars2.stan_0_blue = 23030;
        save_sys_info.bk_var.hrd_std_vars.stan_0_clear = 23030;      save_sys_info.bk_var.hrd_std_vars2.stan_0_clear = 23030;

        save_sys_info.bk_var.hrd_std_vars.stan_1_red = 22692;        save_sys_info.bk_var.hrd_std_vars2.stan_1_red = 22692;
        save_sys_info.bk_var.hrd_std_vars.stan_1_green = 22692;      save_sys_info.bk_var.hrd_std_vars2.stan_1_green = 22692;
        save_sys_info.bk_var.hrd_std_vars.stan_1_blue = 22692;       save_sys_info.bk_var.hrd_std_vars2.stan_1_blue = 22692;
        save_sys_info.bk_var.hrd_std_vars.stan_1_clear = 22692;      save_sys_info.bk_var.hrd_std_vars2.stan_1_clear = 22692;

        save_sys_info.bk_var.hrd_std_vars.stan_2_red = 22692;        save_sys_info.bk_var.hrd_std_vars2.stan_2_red = 22692;
        save_sys_info.bk_var.hrd_std_vars.stan_2_green = 22692;      save_sys_info.bk_var.hrd_std_vars2.stan_2_green = 22692;
        save_sys_info.bk_var.hrd_std_vars.stan_2_blue = 22692;       save_sys_info.bk_var.hrd_std_vars2.stan_2_blue = 22692;
        save_sys_info.bk_var.hrd_std_vars.stan_2_clear = 22692;      save_sys_info.bk_var.hrd_std_vars2.stan_2_clear = 22692;

        save_sys_info.bk_var.hrd_std_vars.stan_3_red = 22148;        save_sys_info.bk_var.hrd_std_vars2.stan_3_red = 22148;
        save_sys_info.bk_var.hrd_std_vars.stan_3_green = 22148;      save_sys_info.bk_var.hrd_std_vars2.stan_3_green = 22148;
        save_sys_info.bk_var.hrd_std_vars.stan_3_blue = 22148;       save_sys_info.bk_var.hrd_std_vars2.stan_3_blue = 22148;
        save_sys_info.bk_var.hrd_std_vars.stan_3_clear = 22148;      save_sys_info.bk_var.hrd_std_vars2.stan_3_clear = 22148;

        save_sys_info.bk_var.hrd_std_vars.stan_4_red = 22148;        save_sys_info.bk_var.hrd_std_vars2.stan_4_red = 22148;
        save_sys_info.bk_var.hrd_std_vars.stan_4_green = 22148;      save_sys_info.bk_var.hrd_std_vars2.stan_4_green = 22148;
        save_sys_info.bk_var.hrd_std_vars.stan_4_blue = 22148;       save_sys_info.bk_var.hrd_std_vars2.stan_4_blue = 22148;
        save_sys_info.bk_var.hrd_std_vars.stan_4_clear = 22148;      save_sys_info.bk_var.hrd_std_vars2.stan_4_clear = 22148;

        save_sys_info.bk_var.hrd_std_vars.stan_5_red = 21502;        save_sys_info.bk_var.hrd_std_vars2.stan_5_red = 21502;
        save_sys_info.bk_var.hrd_std_vars.stan_5_green = 21502;      save_sys_info.bk_var.hrd_std_vars2.stan_5_green = 21502;
        save_sys_info.bk_var.hrd_std_vars.stan_5_blue = 21502;       save_sys_info.bk_var.hrd_std_vars2.stan_5_blue = 21502;
        save_sys_info.bk_var.hrd_std_vars.stan_5_clear = 21502;      save_sys_info.bk_var.hrd_std_vars2.stan_5_clear = 21502;

        save_sys_info.bk_var.hrd_std_vars.stan_6_red = 21502;        save_sys_info.bk_var.hrd_std_vars2.stan_6_red = 21502;
        save_sys_info.bk_var.hrd_std_vars.stan_6_green = 21502;      save_sys_info.bk_var.hrd_std_vars2.stan_6_green = 21502;
        save_sys_info.bk_var.hrd_std_vars.stan_6_blue = 21502;       save_sys_info.bk_var.hrd_std_vars2.stan_6_blue = 21502;
        save_sys_info.bk_var.hrd_std_vars.stan_6_clear = 21502;      save_sys_info.bk_var.hrd_std_vars2.stan_6_clear = 21502;

        save_sys_info.bk_var.hrd_std_vars.stan_7_red = 20720;        save_sys_info.bk_var.hrd_std_vars2.stan_7_red = 20720;
        save_sys_info.bk_var.hrd_std_vars.stan_7_green = 20720;      save_sys_info.bk_var.hrd_std_vars2.stan_7_green = 20720;
        save_sys_info.bk_var.hrd_std_vars.stan_7_blue = 20720;       save_sys_info.bk_var.hrd_std_vars2.stan_7_blue = 20720;
        save_sys_info.bk_var.hrd_std_vars.stan_7_clear = 20720;      save_sys_info.bk_var.hrd_std_vars2.stan_7_clear = 20720;

        save_sys_info.bk_var.hrd_std_vars.stan_8_red = 20720;        save_sys_info.bk_var.hrd_std_vars2.stan_8_red = 20720;
        save_sys_info.bk_var.hrd_std_vars.stan_8_green = 20720;      save_sys_info.bk_var.hrd_std_vars2.stan_8_green = 20720;
        save_sys_info.bk_var.hrd_std_vars.stan_8_blue = 20720;       save_sys_info.bk_var.hrd_std_vars2.stan_8_blue = 20720;
        save_sys_info.bk_var.hrd_std_vars.stan_8_clear = 20720;      save_sys_info.bk_var.hrd_std_vars2.stan_8_clear = 20720;

        save_sys_info.bk_var.hrd_std_vars.stan_9_red = 19603;        save_sys_info.bk_var.hrd_std_vars2.stan_9_red = 19603;
        save_sys_info.bk_var.hrd_std_vars.stan_9_green = 19603;      save_sys_info.bk_var.hrd_std_vars2.stan_9_green = 19603;
        save_sys_info.bk_var.hrd_std_vars.stan_9_blue = 19603;       save_sys_info.bk_var.hrd_std_vars2.stan_9_blue = 19603;
        save_sys_info.bk_var.hrd_std_vars.stan_9_clear = 19603;      save_sys_info.bk_var.hrd_std_vars2.stan_9_clear = 19603;

        save_sys_info.bk_var.hrd_std_vars.stan_10_red = 17299;        save_sys_info.bk_var.hrd_std_vars2.stan_10_red = 17299;
        save_sys_info.bk_var.hrd_std_vars.stan_10_green = 17299;      save_sys_info.bk_var.hrd_std_vars2.stan_10_green = 17299;
        save_sys_info.bk_var.hrd_std_vars.stan_10_blue = 17299;       save_sys_info.bk_var.hrd_std_vars2.stan_10_blue = 17299;
        save_sys_info.bk_var.hrd_std_vars.stan_10_clear = 17299;     save_sys_info.bk_var.hrd_std_vars2.stan_10_clear = 17299;
		break;
	case POTASSIUM:
        save_sys_info.bk_var.hrd_std_vars.stan_0_red = 24295;        save_sys_info.bk_var.hrd_std_vars2.stan_0_red = 24295;
        save_sys_info.bk_var.hrd_std_vars.stan_0_green = 24295;      save_sys_info.bk_var.hrd_std_vars2.stan_0_green = 24295;
        save_sys_info.bk_var.hrd_std_vars.stan_0_blue = 24295;       save_sys_info.bk_var.hrd_std_vars2.stan_0_blue = 24295;
        save_sys_info.bk_var.hrd_std_vars.stan_0_clear = 24295;      save_sys_info.bk_var.hrd_std_vars2.stan_0_clear = 24295;

        save_sys_info.bk_var.hrd_std_vars.stan_1_red = 23466;        save_sys_info.bk_var.hrd_std_vars2.stan_1_red = 23466;
        save_sys_info.bk_var.hrd_std_vars.stan_1_green = 23466;      save_sys_info.bk_var.hrd_std_vars2.stan_1_green = 23466;
        save_sys_info.bk_var.hrd_std_vars.stan_1_blue = 23466;       save_sys_info.bk_var.hrd_std_vars2.stan_1_blue = 23466;
        save_sys_info.bk_var.hrd_std_vars.stan_1_clear = 23466;      save_sys_info.bk_var.hrd_std_vars2.stan_1_clear = 23466;

        save_sys_info.bk_var.hrd_std_vars.stan_2_red = 23466;        save_sys_info.bk_var.hrd_std_vars2.stan_2_red = 23466;
        save_sys_info.bk_var.hrd_std_vars.stan_2_green = 23466;      save_sys_info.bk_var.hrd_std_vars2.stan_2_green = 23466;
        save_sys_info.bk_var.hrd_std_vars.stan_2_blue = 23466;       save_sys_info.bk_var.hrd_std_vars2.stan_2_blue = 23466;
        save_sys_info.bk_var.hrd_std_vars.stan_2_clear = 23466;      save_sys_info.bk_var.hrd_std_vars2.stan_2_clear = 23466;

        save_sys_info.bk_var.hrd_std_vars.stan_3_red = 22649;        save_sys_info.bk_var.hrd_std_vars2.stan_3_red = 22649;
        save_sys_info.bk_var.hrd_std_vars.stan_3_green = 22649;      save_sys_info.bk_var.hrd_std_vars2.stan_3_green = 22649;
        save_sys_info.bk_var.hrd_std_vars.stan_3_blue = 22649;       save_sys_info.bk_var.hrd_std_vars2.stan_3_blue = 22649;
        save_sys_info.bk_var.hrd_std_vars.stan_3_clear = 22649;      save_sys_info.bk_var.hrd_std_vars2.stan_3_clear = 22649;

        save_sys_info.bk_var.hrd_std_vars.stan_4_red = 22649;        save_sys_info.bk_var.hrd_std_vars2.stan_4_red = 22649;
        save_sys_info.bk_var.hrd_std_vars.stan_4_green = 22649;       save_sys_info.bk_var.hrd_std_vars2.stan_4_green = 22649;
        save_sys_info.bk_var.hrd_std_vars.stan_4_blue = 22649;       save_sys_info.bk_var.hrd_std_vars2.stan_4_blue = 22649;
        save_sys_info.bk_var.hrd_std_vars.stan_4_clear = 22649;      save_sys_info.bk_var.hrd_std_vars2.stan_4_clear = 22649;

        save_sys_info.bk_var.hrd_std_vars.stan_5_red = 19175;        save_sys_info.bk_var.hrd_std_vars2.stan_5_red = 19175;
        save_sys_info.bk_var.hrd_std_vars.stan_5_green = 19175;       save_sys_info.bk_var.hrd_std_vars2.stan_5_green = 19175;
        save_sys_info.bk_var.hrd_std_vars.stan_5_blue = 19175;       save_sys_info.bk_var.hrd_std_vars2.stan_5_blue = 19175;
        save_sys_info.bk_var.hrd_std_vars.stan_5_clear = 19175;      save_sys_info.bk_var.hrd_std_vars2.stan_5_clear = 19175;

        save_sys_info.bk_var.hrd_std_vars.stan_6_red = 19175;         save_sys_info.bk_var.hrd_std_vars2.stan_6_red = 19175;
        save_sys_info.bk_var.hrd_std_vars.stan_6_green = 19175;       save_sys_info.bk_var.hrd_std_vars2.stan_6_green = 19175;
        save_sys_info.bk_var.hrd_std_vars.stan_6_blue = 19175;       save_sys_info.bk_var.hrd_std_vars2.stan_6_blue = 19175;
        save_sys_info.bk_var.hrd_std_vars.stan_6_clear = 19175;      save_sys_info.bk_var.hrd_std_vars2.stan_6_clear = 19175;

        save_sys_info.bk_var.hrd_std_vars.stan_7_red = 14468;         save_sys_info.bk_var.hrd_std_vars2.stan_7_red = 14468;
        save_sys_info.bk_var.hrd_std_vars.stan_7_green = 14468;       save_sys_info.bk_var.hrd_std_vars2.stan_7_green = 14468;
        save_sys_info.bk_var.hrd_std_vars.stan_7_blue = 14468;       save_sys_info.bk_var.hrd_std_vars2.stan_7_blue = 14468;
        save_sys_info.bk_var.hrd_std_vars.stan_7_clear = 14468;      save_sys_info.bk_var.hrd_std_vars2.stan_7_clear = 14468;

        save_sys_info.bk_var.hrd_std_vars.stan_8_red = 14468;         save_sys_info.bk_var.hrd_std_vars2.stan_8_red = 14468;
        save_sys_info.bk_var.hrd_std_vars.stan_8_green = 14468;       save_sys_info.bk_var.hrd_std_vars2.stan_8_green = 14468;
        save_sys_info.bk_var.hrd_std_vars.stan_8_blue = 14468;        save_sys_info.bk_var.hrd_std_vars2.stan_8_blue = 14468;
        save_sys_info.bk_var.hrd_std_vars.stan_8_clear = 14468;      save_sys_info.bk_var.hrd_std_vars2.stan_8_clear = 14468;

        save_sys_info.bk_var.hrd_std_vars.stan_9_red = 12985;         save_sys_info.bk_var.hrd_std_vars2.stan_9_red = 12985;
        save_sys_info.bk_var.hrd_std_vars.stan_9_green = 12985;       save_sys_info.bk_var.hrd_std_vars2.stan_9_green = 12985;
        save_sys_info.bk_var.hrd_std_vars.stan_9_blue = 12985;        save_sys_info.bk_var.hrd_std_vars2.stan_9_blue = 12985;
        save_sys_info.bk_var.hrd_std_vars.stan_9_clear = 12985;      save_sys_info.bk_var.hrd_std_vars2.stan_9_clear = 12985;

        save_sys_info.bk_var.hrd_std_vars.stan_10_red = 11098;        save_sys_info.bk_var.hrd_std_vars2.stan_10_red = 11098;
        save_sys_info.bk_var.hrd_std_vars.stan_10_green = 11098;      save_sys_info.bk_var.hrd_std_vars2.stan_10_green = 11098;
        save_sys_info.bk_var.hrd_std_vars.stan_10_blue = 11098;       save_sys_info.bk_var.hrd_std_vars2.stan_10_blue = 11098;
        save_sys_info.bk_var.hrd_std_vars.stan_10_clear = 11098;     save_sys_info.bk_var.hrd_std_vars2.stan_10_clear = 11098;
		break;
	case PHOSPHORUS:
        save_sys_info.bk_var.hrd_std_vars.stan_0_red = 34882;        save_sys_info.bk_var.hrd_std_vars2.stan_0_red = 34882;
        save_sys_info.bk_var.hrd_std_vars.stan_0_green = 34882;      save_sys_info.bk_var.hrd_std_vars2.stan_0_green = 34882;
        save_sys_info.bk_var.hrd_std_vars.stan_0_blue = 34882;       save_sys_info.bk_var.hrd_std_vars2.stan_0_blue = 34882;
        save_sys_info.bk_var.hrd_std_vars.stan_0_clear = 34882;      save_sys_info.bk_var.hrd_std_vars2.stan_0_clear = 34882;

        save_sys_info.bk_var.hrd_std_vars.stan_1_red = 33320;        save_sys_info.bk_var.hrd_std_vars2.stan_1_red = 33320;
        save_sys_info.bk_var.hrd_std_vars.stan_1_green = 33320;      save_sys_info.bk_var.hrd_std_vars2.stan_1_green = 33320;
        save_sys_info.bk_var.hrd_std_vars.stan_1_blue = 33320;       save_sys_info.bk_var.hrd_std_vars2.stan_1_blue = 33320;
        save_sys_info.bk_var.hrd_std_vars.stan_1_clear = 33320;      save_sys_info.bk_var.hrd_std_vars2.stan_1_clear = 33320;

        save_sys_info.bk_var.hrd_std_vars.stan_2_red = 33320;        save_sys_info.bk_var.hrd_std_vars2.stan_2_red = 33320;
        save_sys_info.bk_var.hrd_std_vars.stan_2_green = 33320;      save_sys_info.bk_var.hrd_std_vars2.stan_2_green = 33320;
        save_sys_info.bk_var.hrd_std_vars.stan_2_blue = 33320;       save_sys_info.bk_var.hrd_std_vars2.stan_2_blue = 33320;
        save_sys_info.bk_var.hrd_std_vars.stan_2_clear = 33320;      save_sys_info.bk_var.hrd_std_vars2.stan_2_clear = 33320;

        save_sys_info.bk_var.hrd_std_vars.stan_3_red = 32216;        save_sys_info.bk_var.hrd_std_vars2.stan_3_red = 32216;
        save_sys_info.bk_var.hrd_std_vars.stan_3_green = 32216;      save_sys_info.bk_var.hrd_std_vars2.stan_3_green = 32216;
        save_sys_info.bk_var.hrd_std_vars.stan_3_blue = 32216;       save_sys_info.bk_var.hrd_std_vars2.stan_3_blue = 32216;
        save_sys_info.bk_var.hrd_std_vars.stan_3_clear = 32216;      save_sys_info.bk_var.hrd_std_vars2.stan_3_clear = 32216;

        save_sys_info.bk_var.hrd_std_vars.stan_4_red = 32216;        save_sys_info.bk_var.hrd_std_vars2.stan_4_red = 32216;
        save_sys_info.bk_var.hrd_std_vars.stan_4_green = 32216;      save_sys_info.bk_var.hrd_std_vars2.stan_4_green = 32216;
        save_sys_info.bk_var.hrd_std_vars.stan_4_blue = 32216;       save_sys_info.bk_var.hrd_std_vars2.stan_4_blue = 32216;
        save_sys_info.bk_var.hrd_std_vars.stan_4_clear = 32216;      save_sys_info.bk_var.hrd_std_vars2.stan_4_clear = 32216;

        save_sys_info.bk_var.hrd_std_vars.stan_5_red = 28463;         save_sys_info.bk_var.hrd_std_vars2.stan_5_red = 28463;
        save_sys_info.bk_var.hrd_std_vars.stan_5_green = 28463;       save_sys_info.bk_var.hrd_std_vars2.stan_5_green = 28463;
        save_sys_info.bk_var.hrd_std_vars.stan_5_blue = 28463;       save_sys_info.bk_var.hrd_std_vars2.stan_5_blue = 28463;
        save_sys_info.bk_var.hrd_std_vars.stan_5_clear = 28463;      save_sys_info.bk_var.hrd_std_vars2.stan_5_clear = 28463;

        save_sys_info.bk_var.hrd_std_vars.stan_6_red = 28463;         save_sys_info.bk_var.hrd_std_vars2.stan_6_red = 28463;
        save_sys_info.bk_var.hrd_std_vars.stan_6_green = 28463;       save_sys_info.bk_var.hrd_std_vars2.stan_6_green = 28463;
        save_sys_info.bk_var.hrd_std_vars.stan_6_blue = 28463;       save_sys_info.bk_var.hrd_std_vars2.stan_6_blue = 28463;
        save_sys_info.bk_var.hrd_std_vars.stan_6_clear = 28463;      save_sys_info.bk_var.hrd_std_vars2.stan_6_clear = 28463;

        save_sys_info.bk_var.hrd_std_vars.stan_7_red = 25216;         save_sys_info.bk_var.hrd_std_vars2.stan_7_red = 25216;
        save_sys_info.bk_var.hrd_std_vars.stan_7_green = 25216;       save_sys_info.bk_var.hrd_std_vars2.stan_7_green = 25216;
        save_sys_info.bk_var.hrd_std_vars.stan_7_blue = 25216;       save_sys_info.bk_var.hrd_std_vars2.stan_7_blue = 25216;
        save_sys_info.bk_var.hrd_std_vars.stan_7_clear = 25216;      save_sys_info.bk_var.hrd_std_vars2.stan_7_clear = 25216;

        save_sys_info.bk_var.hrd_std_vars.stan_8_red = 25216;         save_sys_info.bk_var.hrd_std_vars2.stan_8_red = 25216;
        save_sys_info.bk_var.hrd_std_vars.stan_8_green = 25216;       save_sys_info.bk_var.hrd_std_vars2.stan_8_green = 25216;
        save_sys_info.bk_var.hrd_std_vars.stan_8_blue = 25216;       save_sys_info.bk_var.hrd_std_vars2.stan_8_blue = 25216;
        save_sys_info.bk_var.hrd_std_vars.stan_8_clear = 25216;      save_sys_info.bk_var.hrd_std_vars2.stan_8_clear = 25216;

        save_sys_info.bk_var.hrd_std_vars.stan_9_red = 23480;         save_sys_info.bk_var.hrd_std_vars2.stan_9_red = 23480;
        save_sys_info.bk_var.hrd_std_vars.stan_9_green = 23480;       save_sys_info.bk_var.hrd_std_vars2.stan_9_green = 23480;
        save_sys_info.bk_var.hrd_std_vars.stan_9_blue = 23480;        save_sys_info.bk_var.hrd_std_vars2.stan_9_blue = 23480;
        save_sys_info.bk_var.hrd_std_vars.stan_9_clear = 23480;      save_sys_info.bk_var.hrd_std_vars2.stan_9_clear = 23480;

        save_sys_info.bk_var.hrd_std_vars.stan_10_red = 11506;        save_sys_info.bk_var.hrd_std_vars2.stan_10_red = 11506;
        save_sys_info.bk_var.hrd_std_vars.stan_10_green = 11506;      save_sys_info.bk_var.hrd_std_vars2.stan_10_green = 11506;
        save_sys_info.bk_var.hrd_std_vars.stan_10_blue = 11506;       save_sys_info.bk_var.hrd_std_vars2.stan_10_blue = 11506;
        save_sys_info.bk_var.hrd_std_vars.stan_10_clear = 11506;     save_sys_info.bk_var.hrd_std_vars2.stan_10_clear = 11506;
		break;
	case NITROGEN:
        save_sys_info.bk_var.hrd_std_vars.stan_0_red = 51024;        save_sys_info.bk_var.hrd_std_vars2.stan_0_red = 51024;
        save_sys_info.bk_var.hrd_std_vars.stan_0_green = 51024;      save_sys_info.bk_var.hrd_std_vars2.stan_0_green = 51024;
        save_sys_info.bk_var.hrd_std_vars.stan_0_blue = 51024;       save_sys_info.bk_var.hrd_std_vars2.stan_0_blue = 51024;
        save_sys_info.bk_var.hrd_std_vars.stan_0_clear = 51024;      save_sys_info.bk_var.hrd_std_vars2.stan_0_clear = 51024;

        save_sys_info.bk_var.hrd_std_vars.stan_1_red = 38451;        save_sys_info.bk_var.hrd_std_vars2.stan_1_red = 38451;
        save_sys_info.bk_var.hrd_std_vars.stan_1_green = 38451;      save_sys_info.bk_var.hrd_std_vars2.stan_1_green = 38451;
        save_sys_info.bk_var.hrd_std_vars.stan_1_blue = 38451;       save_sys_info.bk_var.hrd_std_vars2.stan_1_blue = 38451;
        save_sys_info.bk_var.hrd_std_vars.stan_1_clear = 38451;      save_sys_info.bk_var.hrd_std_vars2.stan_1_clear = 38451;

        save_sys_info.bk_var.hrd_std_vars.stan_2_red = 38451;        save_sys_info.bk_var.hrd_std_vars2.stan_2_red = 38451;
        save_sys_info.bk_var.hrd_std_vars.stan_2_green = 38451;      save_sys_info.bk_var.hrd_std_vars2.stan_2_green = 38451;
        save_sys_info.bk_var.hrd_std_vars.stan_2_blue = 38451;       save_sys_info.bk_var.hrd_std_vars2.stan_2_blue = 38451;
        save_sys_info.bk_var.hrd_std_vars.stan_2_clear = 38451;      save_sys_info.bk_var.hrd_std_vars2.stan_2_clear = 38451;

        save_sys_info.bk_var.hrd_std_vars.stan_3_red = 30138;        save_sys_info.bk_var.hrd_std_vars2.stan_3_red = 30138;
        save_sys_info.bk_var.hrd_std_vars.stan_3_green = 30138;      save_sys_info.bk_var.hrd_std_vars2.stan_3_green = 30138;
        save_sys_info.bk_var.hrd_std_vars.stan_3_blue = 30138;       save_sys_info.bk_var.hrd_std_vars2.stan_3_blue = 30138;
        save_sys_info.bk_var.hrd_std_vars.stan_3_clear = 30138;      save_sys_info.bk_var.hrd_std_vars2.stan_3_clear = 30138;

        save_sys_info.bk_var.hrd_std_vars.stan_4_red = 30138;        save_sys_info.bk_var.hrd_std_vars2.stan_4_red = 30138;
        save_sys_info.bk_var.hrd_std_vars.stan_4_green = 30138;       save_sys_info.bk_var.hrd_std_vars2.stan_4_green = 30138;
        save_sys_info.bk_var.hrd_std_vars.stan_4_blue = 30138;       save_sys_info.bk_var.hrd_std_vars2.stan_4_blue = 30138;
        save_sys_info.bk_var.hrd_std_vars.stan_4_clear = 30138;      save_sys_info.bk_var.hrd_std_vars2.stan_4_clear = 30138;

        save_sys_info.bk_var.hrd_std_vars.stan_5_red = 21178;        save_sys_info.bk_var.hrd_std_vars2.stan_5_red = 21178;
        save_sys_info.bk_var.hrd_std_vars.stan_5_green = 21178;       save_sys_info.bk_var.hrd_std_vars2.stan_5_green = 21178;
        save_sys_info.bk_var.hrd_std_vars.stan_5_blue = 21178;       save_sys_info.bk_var.hrd_std_vars2.stan_5_blue = 21178;
        save_sys_info.bk_var.hrd_std_vars.stan_5_clear = 21178;      save_sys_info.bk_var.hrd_std_vars2.stan_5_clear = 21178;

        save_sys_info.bk_var.hrd_std_vars.stan_6_red = 21178;         save_sys_info.bk_var.hrd_std_vars2.stan_6_red = 21178;
        save_sys_info.bk_var.hrd_std_vars.stan_6_green = 21178;       save_sys_info.bk_var.hrd_std_vars2.stan_6_green = 21178;
        save_sys_info.bk_var.hrd_std_vars.stan_6_blue = 21178;       save_sys_info.bk_var.hrd_std_vars2.stan_6_blue = 21178;
        save_sys_info.bk_var.hrd_std_vars.stan_6_clear = 21178;      save_sys_info.bk_var.hrd_std_vars2.stan_6_clear = 21178;

        save_sys_info.bk_var.hrd_std_vars.stan_7_red = 16829;         save_sys_info.bk_var.hrd_std_vars2.stan_7_red = 16829;
        save_sys_info.bk_var.hrd_std_vars.stan_7_green = 16829;       save_sys_info.bk_var.hrd_std_vars2.stan_7_green = 16829;
        save_sys_info.bk_var.hrd_std_vars.stan_7_blue = 16829;       save_sys_info.bk_var.hrd_std_vars2.stan_7_blue = 16829;
        save_sys_info.bk_var.hrd_std_vars.stan_7_clear = 16829;      save_sys_info.bk_var.hrd_std_vars2.stan_7_clear = 16829;

        save_sys_info.bk_var.hrd_std_vars.stan_8_red = 16829;         save_sys_info.bk_var.hrd_std_vars2.stan_8_red = 16829;
        save_sys_info.bk_var.hrd_std_vars.stan_8_green = 16829;       save_sys_info.bk_var.hrd_std_vars2.stan_8_green = 16829;
        save_sys_info.bk_var.hrd_std_vars.stan_8_blue = 16829;        save_sys_info.bk_var.hrd_std_vars2.stan_8_blue = 16829;
        save_sys_info.bk_var.hrd_std_vars.stan_8_clear = 16829;      save_sys_info.bk_var.hrd_std_vars2.stan_8_clear = 16829;

        save_sys_info.bk_var.hrd_std_vars.stan_9_red = 14414;         save_sys_info.bk_var.hrd_std_vars2.stan_9_red = 14414;
        save_sys_info.bk_var.hrd_std_vars.stan_9_green = 14414;       save_sys_info.bk_var.hrd_std_vars2.stan_9_green = 14414;
        save_sys_info.bk_var.hrd_std_vars.stan_9_blue = 14414;        save_sys_info.bk_var.hrd_std_vars2.stan_9_blue = 14414;
        save_sys_info.bk_var.hrd_std_vars.stan_9_clear = 14414;      save_sys_info.bk_var.hrd_std_vars2.stan_9_clear = 14414;

        save_sys_info.bk_var.hrd_std_vars.stan_10_red = 10455;        save_sys_info.bk_var.hrd_std_vars2.stan_10_red = 10455;
        save_sys_info.bk_var.hrd_std_vars.stan_10_green = 10455;      save_sys_info.bk_var.hrd_std_vars2.stan_10_green = 10455;
        save_sys_info.bk_var.hrd_std_vars.stan_10_blue = 10455;       save_sys_info.bk_var.hrd_std_vars2.stan_10_blue = 10455;
        save_sys_info.bk_var.hrd_std_vars.stan_10_clear = 10455;     save_sys_info.bk_var.hrd_std_vars2.stan_10_clear = 10455;
		break;
	case MASTER_SYS:
	case ORGANIC_CARBON:
        save_sys_info.bk_var.hrd_std_vars.stan_0_red = 37768;        save_sys_info.bk_var.hrd_std_vars2.stan_0_red = 37768;
        save_sys_info.bk_var.hrd_std_vars.stan_0_green = 37768;      save_sys_info.bk_var.hrd_std_vars2.stan_0_green = 37768;
        save_sys_info.bk_var.hrd_std_vars.stan_0_blue = 37768;       save_sys_info.bk_var.hrd_std_vars2.stan_0_blue = 37768;
        save_sys_info.bk_var.hrd_std_vars.stan_0_clear = 37768;      save_sys_info.bk_var.hrd_std_vars2.stan_0_clear = 37768;

        save_sys_info.bk_var.hrd_std_vars.stan_1_red = 28815;        save_sys_info.bk_var.hrd_std_vars2.stan_1_red = 28815;
        save_sys_info.bk_var.hrd_std_vars.stan_1_green = 28815;      save_sys_info.bk_var.hrd_std_vars2.stan_1_green = 28815;
        save_sys_info.bk_var.hrd_std_vars.stan_1_blue = 28815;       save_sys_info.bk_var.hrd_std_vars2.stan_1_blue = 28815;
        save_sys_info.bk_var.hrd_std_vars.stan_1_clear = 28815;      save_sys_info.bk_var.hrd_std_vars2.stan_1_clear = 28815;

        save_sys_info.bk_var.hrd_std_vars.stan_2_red = 28815;        save_sys_info.bk_var.hrd_std_vars2.stan_2_red = 28815;
        save_sys_info.bk_var.hrd_std_vars.stan_2_green = 28815;      save_sys_info.bk_var.hrd_std_vars2.stan_2_green = 28815;
        save_sys_info.bk_var.hrd_std_vars.stan_2_blue = 28815;       save_sys_info.bk_var.hrd_std_vars2.stan_2_blue = 28815;
        save_sys_info.bk_var.hrd_std_vars.stan_2_clear = 28815;      save_sys_info.bk_var.hrd_std_vars2.stan_2_clear = 28815;

        save_sys_info.bk_var.hrd_std_vars.stan_3_red = 21984;        save_sys_info.bk_var.hrd_std_vars2.stan_3_red = 21984;
        save_sys_info.bk_var.hrd_std_vars.stan_3_green = 21984;      save_sys_info.bk_var.hrd_std_vars2.stan_3_green = 21984;
        save_sys_info.bk_var.hrd_std_vars.stan_3_blue = 21984;       save_sys_info.bk_var.hrd_std_vars2.stan_3_blue = 21984;
        save_sys_info.bk_var.hrd_std_vars.stan_3_clear = 21984;      save_sys_info.bk_var.hrd_std_vars2.stan_3_clear = 21984;

        save_sys_info.bk_var.hrd_std_vars.stan_4_red = 21984;        save_sys_info.bk_var.hrd_std_vars2.stan_4_red = 21984;
        save_sys_info.bk_var.hrd_std_vars.stan_4_green = 21984;       save_sys_info.bk_var.hrd_std_vars2.stan_4_green = 21984;
        save_sys_info.bk_var.hrd_std_vars.stan_4_blue = 21984;       save_sys_info.bk_var.hrd_std_vars2.stan_4_blue = 21984;
        save_sys_info.bk_var.hrd_std_vars.stan_4_clear = 21984;      save_sys_info.bk_var.hrd_std_vars2.stan_4_clear = 21984;

        save_sys_info.bk_var.hrd_std_vars.stan_5_red = 16773;        save_sys_info.bk_var.hrd_std_vars2.stan_5_red = 16773;
        save_sys_info.bk_var.hrd_std_vars.stan_5_green = 16773;       save_sys_info.bk_var.hrd_std_vars2.stan_5_green = 16773;
        save_sys_info.bk_var.hrd_std_vars.stan_5_blue = 16773;       save_sys_info.bk_var.hrd_std_vars2.stan_5_blue = 16773;
        save_sys_info.bk_var.hrd_std_vars.stan_5_clear = 16773;      save_sys_info.bk_var.hrd_std_vars2.stan_5_clear = 16773;

        save_sys_info.bk_var.hrd_std_vars.stan_6_red = 16773;         save_sys_info.bk_var.hrd_std_vars2.stan_6_red = 16773;
        save_sys_info.bk_var.hrd_std_vars.stan_6_green = 16773;       save_sys_info.bk_var.hrd_std_vars2.stan_6_green = 16773;
        save_sys_info.bk_var.hrd_std_vars.stan_6_blue = 16773;       save_sys_info.bk_var.hrd_std_vars2.stan_6_blue = 16773;
        save_sys_info.bk_var.hrd_std_vars.stan_6_clear = 16773;      save_sys_info.bk_var.hrd_std_vars2.stan_6_clear = 16773;

        save_sys_info.bk_var.hrd_std_vars.stan_7_red = 12797;         save_sys_info.bk_var.hrd_std_vars2.stan_7_red = 12797;
        save_sys_info.bk_var.hrd_std_vars.stan_7_green = 12797;       save_sys_info.bk_var.hrd_std_vars2.stan_7_green = 12797;
        save_sys_info.bk_var.hrd_std_vars.stan_7_blue = 12797;       save_sys_info.bk_var.hrd_std_vars2.stan_7_blue = 12797;
        save_sys_info.bk_var.hrd_std_vars.stan_7_clear = 12797;      save_sys_info.bk_var.hrd_std_vars2.stan_7_clear = 12797;

        save_sys_info.bk_var.hrd_std_vars.stan_8_red = 12797;         save_sys_info.bk_var.hrd_std_vars2.stan_8_red = 12797;
        save_sys_info.bk_var.hrd_std_vars.stan_8_green = 12797;       save_sys_info.bk_var.hrd_std_vars2.stan_8_green = 12797;
        save_sys_info.bk_var.hrd_std_vars.stan_8_blue = 12797;        save_sys_info.bk_var.hrd_std_vars2.stan_8_blue = 12797;
        save_sys_info.bk_var.hrd_std_vars.stan_8_clear = 12797;      save_sys_info.bk_var.hrd_std_vars2.stan_8_clear = 12797;

        save_sys_info.bk_var.hrd_std_vars.stan_9_red = 4336;         save_sys_info.bk_var.hrd_std_vars2.stan_9_red = 4336;
        save_sys_info.bk_var.hrd_std_vars.stan_9_green = 4336;       save_sys_info.bk_var.hrd_std_vars2.stan_9_green = 4336;
        save_sys_info.bk_var.hrd_std_vars.stan_9_blue = 4336;        save_sys_info.bk_var.hrd_std_vars2.stan_9_blue = 4336;
        save_sys_info.bk_var.hrd_std_vars.stan_9_clear = 4336;      save_sys_info.bk_var.hrd_std_vars2.stan_9_clear = 4336;

        save_sys_info.bk_var.hrd_std_vars.stan_10_red = 497;        save_sys_info.bk_var.hrd_std_vars2.stan_10_red = 497;
        save_sys_info.bk_var.hrd_std_vars.stan_10_green = 497;      save_sys_info.bk_var.hrd_std_vars2.stan_10_green = 497;
        save_sys_info.bk_var.hrd_std_vars.stan_10_blue = 497;       save_sys_info.bk_var.hrd_std_vars2.stan_10_blue = 497;
        save_sys_info.bk_var.hrd_std_vars.stan_10_clear = 497;     save_sys_info.bk_var.hrd_std_vars2.stan_10_clear = 497;
		break;
	}
}
#endif

void wrt_dflt(void)
{
	save_sys_info.bk_var.start_add1 = MEM_ID1;
	save_sys_info.bk_var.start_add2 = MEM_ID2;
	save_sys_info.bk_var.maj_sw_rv_no = MAJ_SW_RV_NO;
	save_sys_info.bk_var.mid_sw_rv_no = MID_SW_RV_NO;
	save_sys_info.bk_var.min_sw_rv_no = MIN_SW_RV_NO;
	save_sys_info.bk_var.curr_sys_add = sys_info.set_sys_add;
#ifdef ARC_CAL
	init_hrd_strd();
#endif
#ifdef OLD_CAL
	init_hrd_strd_old();
#endif
	save_sys_info.bk_var.end_add1 = 0x55;
	save_sys_info.bk_var.end_add2 = 0xAA;
}

#ifdef OLD_CAL
void sys_var_init_old(void)
{
	//system chemical quantity info
	switch(save_sys_info.bk_var.curr_sys_add)
	{
	case MAGNESIUM:
		sys_info.chem_1a_qty = 2000;//20.00ml
		sys_info.chem_1b_qty = 0;
		sys_info.chem_2c_qty = 200;
		sys_info.chem_3d_qty = 0;
		sys_info.chem_4e_qty = 0;
		sys_info.chem_5f_qty = 0;
		sys_info.wtr_g_qty = 0;
		sys_info.led_pwm_red = 153;//pwm value
		sys_info.led_pwm_green = 153;
		sys_info.led_pwm_blue = 153;
		sys_info.val_cal_x = 1;//0 - red, 1 - green, 2- blue, 3 - Clear.
		sys_info.val_cal_y = 1;//0 - red, 1 - green, 2- blue, 3 - Clear.
		sys_info.act_stan_vals[0] = 0.00;
		sys_info.act_stan_vals[1] = 1.00;
		sys_info.act_stan_vals[2] = 1.00;
		sys_info.act_stan_vals[3] = 2.00;
		sys_info.act_stan_vals[4] = 2.00;
		sys_info.act_stan_vals[5] = 3.00;
		sys_info.act_stan_vals[6] = 3.00;
		sys_info.act_stan_vals[7] = 4.00;
		sys_info.act_stan_vals[8] = 4.00;
		sys_info.act_stan_vals[9] = 50.00;
		sys_info.act_stan_vals[10] = 100.00;
		break;
	case IRON:
		sys_info.chem_1a_qty = 200;///2.00ml
		sys_info.chem_1b_qty = 0;
		sys_info.chem_2c_qty = 200;
		sys_info.chem_3d_qty = 200;
		sys_info.chem_4e_qty = 0;
		sys_info.chem_5f_qty = 0;
		sys_info.wtr_g_qty = 0;
		sys_info.led_pwm_red = 0;
		sys_info.led_pwm_green = 160;
		sys_info.led_pwm_blue = 160;
		sys_info.val_cal_x = 1;//0 - red, 1 - green, 2- blue, 3 - Clear.
		sys_info.val_cal_y = 1;//0 - red, 1 - green, 2- blue, 3 - Clear.
		sys_info.act_stan_vals[0] = 0.00;
		sys_info.act_stan_vals[1] = 2.00;
		sys_info.act_stan_vals[2] = 2.00;
		sys_info.act_stan_vals[3] = 5.00;
		sys_info.act_stan_vals[4] = 5.00;
		sys_info.act_stan_vals[5] = 10.00;
		sys_info.act_stan_vals[6] = 10.00;
		sys_info.act_stan_vals[7] = 15.00;
		sys_info.act_stan_vals[8] = 15.00;
		sys_info.act_stan_vals[9] = 50.00;
		sys_info.act_stan_vals[10] = 100.00;
		break;
	case COPPER:
		sys_info.chem_1a_qty = 300;
		sys_info.chem_1b_qty = 0;
		sys_info.chem_2c_qty = 150;
		sys_info.chem_3d_qty = 0;
		sys_info.chem_4e_qty = 0;
		sys_info.chem_5f_qty = 0;
		sys_info.wtr_g_qty = 0;
		sys_info.led_pwm_red = 0;
		sys_info.led_pwm_green = 0;
		sys_info.led_pwm_blue = 160;
		sys_info.val_cal_x = 2;//0 - red, 1 - green, 2- blue, 3 - Clear.
		sys_info.val_cal_y = 2;//0 - red, 1 - green, 2- blue, 3 - Clear.
		sys_info.act_stan_vals[0] = 0.00;
		sys_info.act_stan_vals[1] = 0.10;
		sys_info.act_stan_vals[2] = 0.10;
		sys_info.act_stan_vals[3] = 0.20;
		sys_info.act_stan_vals[4] = 0.20;
		sys_info.act_stan_vals[5] = 0.40;
		sys_info.act_stan_vals[6] = 0.40;
		sys_info.act_stan_vals[7] = 1.00;
		sys_info.act_stan_vals[8] = 1.00;
		sys_info.act_stan_vals[9] = 5.00;
		sys_info.act_stan_vals[10] = 10.00;
		break;
	case ZINC:
		sys_info.chem_1a_qty = 2000;
		sys_info.chem_1b_qty = 0;
		sys_info.chem_2c_qty = 200;
		sys_info.chem_3d_qty = 300;
		sys_info.chem_4e_qty = 0;
		sys_info.chem_5f_qty = 0;
		sys_info.wtr_g_qty = 0;
		sys_info.led_pwm_red = 0;
		sys_info.led_pwm_green = 255;
		sys_info.led_pwm_blue = 0;
		sys_info.val_cal_x = 1;
		sys_info.val_cal_y = 1;
		sys_info.act_stan_vals[0] = 0.00;
		sys_info.act_stan_vals[1] = 0.20;
		sys_info.act_stan_vals[2] = 0.20;
		sys_info.act_stan_vals[3] = 0.60;
		sys_info.act_stan_vals[4] = 0.60;
		sys_info.act_stan_vals[5] = 1.00;
		sys_info.act_stan_vals[6] = 1.00;
		sys_info.act_stan_vals[7] = 2.00;
		sys_info.act_stan_vals[8] = 2.00;
		sys_info.act_stan_vals[9] = 5.00;
		sys_info.act_stan_vals[10] = 10.00;
		break;
	case BORON:
		sys_info.chem_1a_qty = 2000;
		sys_info.chem_1b_qty = 0;
		sys_info.chem_2c_qty = 200;
		sys_info.chem_3d_qty = 0;
		sys_info.chem_4e_qty = 0;
		sys_info.chem_5f_qty = 0;
		sys_info.wtr_g_qty = 0;
		sys_info.led_pwm_red = 0;
		sys_info.led_pwm_green = 0;
		sys_info.led_pwm_blue = 255;
		sys_info.val_cal_x = 1;
		sys_info.val_cal_y = 1;
		sys_info.act_stan_vals[0] = 0.00;
		sys_info.act_stan_vals[1] = 0.20;
		sys_info.act_stan_vals[2] = 0.20;
		sys_info.act_stan_vals[3] = 0.50;
		sys_info.act_stan_vals[4] = 0.50;
		sys_info.act_stan_vals[5] = 0.75;
		sys_info.act_stan_vals[6] = 0.75;
		sys_info.act_stan_vals[7] = 1.00;
		sys_info.act_stan_vals[8] = 1.00;
		sys_info.act_stan_vals[9] = 5.00;
		sys_info.act_stan_vals[10] = 10.00;
		break;
	case SULPHUR:///////////////////////////////////////////
		sys_info.chem_1a_qty = 1500;
		sys_info.chem_1b_qty = 0;
		sys_info.chem_2c_qty = 30;
		sys_info.chem_3d_qty = 0;
		sys_info.chem_4e_qty = 0;
		sys_info.chem_5f_qty = 0;
		sys_info.wtr_g_qty = 120;
		sys_info.led_pwm_red = 106;
		sys_info.led_pwm_green = 0;
		sys_info.led_pwm_blue = 255;
		sys_info.val_cal_x = 1;
		sys_info.val_cal_y = 1;
		sys_info.act_stan_vals[0] = 0.00;//verry low							1
		sys_info.act_stan_vals[1] = 5.00;//verry low							1
		sys_info.act_stan_vals[2] = 5.00;////verry low						1
		sys_info.act_stan_vals[3] = 10.00;//low					//Deficient		2
		sys_info.act_stan_vals[4] = 10.00;//low				//Sufficient	2
		sys_info.act_stan_vals[5] = 20.00;//Medium				//Sufficient	3
		sys_info.act_stan_vals[6] = 20.00;//Medium								3
		sys_info.act_stan_vals[7] = 30.00;//High								4
		sys_info.act_stan_vals[8] = 30.00;//High								4
		sys_info.act_stan_vals[9] = 50.00;//Verry High 						5
		sys_info.act_stan_vals[10] = 100.00;//Verry High 						5
		break;
	case POTASSIUM:
		sys_info.chem_1a_qty = 500;
		sys_info.chem_1b_qty = 100;
		sys_info.chem_2c_qty = 100;
		sys_info.chem_3d_qty = 55;
		sys_info.chem_4e_qty = 100;
		sys_info.chem_5f_qty = 0;
		sys_info.wtr_g_qty = 520;
		sys_info.led_pwm_red = 106;
		sys_info.led_pwm_green = 0;
		sys_info.led_pwm_blue = 255;
		sys_info.val_cal_x = 1;
		sys_info.val_cal_y = 1;
		sys_info.act_stan_vals[0] = 0.00;
		sys_info.act_stan_vals[1] = 60.00;
		sys_info.act_stan_vals[2] = 60.00;
		sys_info.act_stan_vals[3] = 120.00;
		sys_info.act_stan_vals[4] = 120.00;
		sys_info.act_stan_vals[5] = 280.00;
		sys_info.act_stan_vals[6] = 280.00;
		sys_info.act_stan_vals[7] = 560.00;
		sys_info.act_stan_vals[8] = 560.00;
		sys_info.act_stan_vals[9] = 800.00;
		sys_info.act_stan_vals[10] = 1000.00;
		break;
	case PHOSPHORUS:
		sys_info.chem_1a_qty = 1000;
		sys_info.chem_1b_qty = 0;
		sys_info.chem_2c_qty = 25;
		sys_info.chem_3d_qty = 200;
		sys_info.chem_4e_qty = 100;
		sys_info.chem_5f_qty = 0;
		sys_info.wtr_g_qty = 475;
		sys_info.led_pwm_red = 130;
		sys_info.led_pwm_green = 130;
		sys_info.led_pwm_blue = 130;
		sys_info.val_cal_x = 1;
		sys_info.val_cal_y = 1;
		sys_info.act_stan_vals[0] = 0.00;
		sys_info.act_stan_vals[1] = 5.00;
		sys_info.act_stan_vals[2] = 5.00;
		sys_info.act_stan_vals[3] = 10.00;
		sys_info.act_stan_vals[4] = 10.00;
		sys_info.act_stan_vals[5] = 25.00;
		sys_info.act_stan_vals[6] = 25.00;
		sys_info.act_stan_vals[7] = 40.00;
		sys_info.act_stan_vals[8] = 40.00;
		sys_info.act_stan_vals[9] = 50.00;
		sys_info.act_stan_vals[10] = 150.00;
		break;
	case NITROGEN:
		sys_info.chem_1a_qty = 1000;
		sys_info.chem_1b_qty = 0;
		sys_info.chem_2c_qty = 100;
		sys_info.chem_3d_qty = 100;
		sys_info.chem_4e_qty = 0;
		sys_info.chem_5f_qty = 0;
		sys_info.wtr_g_qty = 580;
		sys_info.led_pwm_red = 255;
		sys_info.led_pwm_green = 255;
		sys_info.led_pwm_blue = 255;
		sys_info.val_cal_x = 1;
		sys_info.val_cal_y = 1;
		sys_info.act_stan_vals[0] = 0.00;
		sys_info.act_stan_vals[1] = 140.00;
		sys_info.act_stan_vals[2] = 140.00;
		sys_info.act_stan_vals[3] = 280.00;
		sys_info.act_stan_vals[4] = 280.00;
		sys_info.act_stan_vals[5] = 560.00;
		sys_info.act_stan_vals[6] = 560.00;
		sys_info.act_stan_vals[7] = 700.00;
		sys_info.act_stan_vals[8] = 700.00;
		sys_info.act_stan_vals[9] = 800.00;
		sys_info.act_stan_vals[10] = 1000.00;
		break;
	case MASTER_SYS:
	case ORGANIC_CARBON:
		sys_info.chem_1a_qty = 1000;
		sys_info.chem_1b_qty = 0;
		sys_info.chem_2c_qty = 1000;
		sys_info.chem_3d_qty = 0;
		sys_info.chem_4e_qty = 0;
		sys_info.chem_5f_qty = 0;
		sys_info.wtr_g_qty = 0;
		sys_info.led_pwm_red = 130;
		sys_info.led_pwm_green = 130;
		sys_info.led_pwm_blue = 130;
		sys_info.val_cal_x = 1;
		sys_info.val_cal_y = 1;
		sys_info.act_stan_vals[0] = 0.00;
		sys_info.act_stan_vals[1] = 0.25;
		sys_info.act_stan_vals[2] = 0.25;
		sys_info.act_stan_vals[3] = 0.50;
		sys_info.act_stan_vals[4] = 0.50;
		sys_info.act_stan_vals[5] = 0.75;
		sys_info.act_stan_vals[6] = 0.75;
		sys_info.act_stan_vals[7] = 1.00;
		sys_info.act_stan_vals[8] = 1.00;
		sys_info.act_stan_vals[9] = 2.00;
		sys_info.act_stan_vals[10] = 4.00;
		break;
	}
	sys_info.wtr_f_wsh = 1300;
	sys_info.wtr_f_wsh_in_tm = sys_info.wtr_f_wsh * TM_MULTIPLIER;
	sys_info.drain_tm = (1000*55);//55 sec
	sys_info.drain_mix_tm = (1000*10);//10 sec
	sys_info.tak_data_skp_tm = (10*25);//solution rest time//25 sec
	sys_info.auto_zero_skp_tm = (10*25);//solution rest time//25 sec
	//sys_info.auto_set_skp_tm = (10*25);//solution rest time//25 sec

	for(uint8_t i = 0; i < NOS_STD; i++)
	{
		for(uint8_t j = 0; j < 4; j++)
		{
			sys_info.opt_std_vars.strd_vars[i][j] = save_sys_info.bk_var.hrd_std_vars.strd_vars[i][j];
			sys_info.opt_std_vars2.strd_vars[i][j] = save_sys_info.bk_var.hrd_std_vars2.strd_vars[i][j];
		}
	}

	sys_info.Stat_L.warm_up_stat = 1;
	sys_info.Stat_L.drain_wsh_stat = 1;//1
}
#endif

#ifdef ARC_CAL
void sys_var_init(void)
{
	//system chemical quantity info
	switch(save_sys_info.bk_var.curr_sys_add)
	{
	case MAGNESIUM:
		sys_info.chem_1a_qty = 2000;//20.00ml
		sys_info.chem_1b_qty = 0;
		sys_info.chem_2c_qty = 200;
		sys_info.chem_3d_qty = 0;
		sys_info.chem_4e_qty = 0;
		sys_info.chem_5f_qty = 0;
		sys_info.wtr_g_qty = 0;
		sys_info.led_pwm_red = 0xff;//pwm value
		sys_info.led_pwm_green = 0xff;
		sys_info.led_pwm_blue = 0xff;
		sys_info.val_cal_x = 1;//green//0 - red, 1 - green, 2- blue, 3 - Clear.
		sys_info.val_cal_y = 2;//blue//0 - red, 1 - green, 2- blue, 3 - Clear.
		sys_info.act_stan_vals[0] = 0;
		sys_info.act_stan_vals[1] = 0;
		sys_info.act_stan_vals[2] = 0;
		sys_info.act_stan_vals[3] = 0;
		sys_info.act_stan_vals[4] = 0;
		sys_info.act_stan_vals[5] = 0;
		sys_info.act_stan_vals[6] = 0;
		sys_info.act_stan_vals[7] = 0;
		sys_info.act_stan_vals[8] = 0;
		sys_info.act_stan_vals[9] = 0;
		sys_info.act_stan_vals[10] = 0;
		break;
	case IRON:
		sys_info.chem_1a_qty = 200;///2.00ml
		sys_info.chem_1b_qty = 0;
		sys_info.chem_2c_qty = 200;
		sys_info.chem_3d_qty = 200;
		sys_info.chem_4e_qty = 0;
		sys_info.chem_5f_qty = 0;
		sys_info.wtr_g_qty = 0;
		sys_info.led_pwm_red = 0xff;
		sys_info.led_pwm_green = 0xff;
		sys_info.led_pwm_blue = 0xff;
		sys_info.val_cal_x = 1;//green
		sys_info.val_cal_y = 2;//blue
		sys_info.act_stan_vals[0] = 0;
		sys_info.act_stan_vals[1] = 0;
		sys_info.act_stan_vals[2] = 0;
		sys_info.act_stan_vals[3] = 0;
		sys_info.act_stan_vals[4] = 0;
		sys_info.act_stan_vals[5] = 0;
		sys_info.act_stan_vals[6] = 0;
		sys_info.act_stan_vals[7] = 0;
		sys_info.act_stan_vals[8] = 0;
		sys_info.act_stan_vals[9] = 0;
		sys_info.act_stan_vals[10] = 0;
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
		sys_info.val_cal_x = 1;//green
		sys_info.val_cal_y = 2;//blue
		sys_info.act_stan_vals[0] = 0;
		sys_info.act_stan_vals[1] = 0;
		sys_info.act_stan_vals[2] = 0;
		sys_info.act_stan_vals[3] = 0;
		sys_info.act_stan_vals[4] = 0;
		sys_info.act_stan_vals[5] = 0;
		sys_info.act_stan_vals[6] = 0;
		sys_info.act_stan_vals[7] = 0;
		sys_info.act_stan_vals[8] = 0;
		sys_info.act_stan_vals[9] = 0;
		sys_info.act_stan_vals[10] = 0;
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
		sys_info.val_cal_x = 1;//green
		sys_info.val_cal_y = 2;//blue
		sys_info.act_stan_vals[0] = 0;
		sys_info.act_stan_vals[1] = 0;
		sys_info.act_stan_vals[2] = 0;
		sys_info.act_stan_vals[3] = 0;
		sys_info.act_stan_vals[4] = 0;
		sys_info.act_stan_vals[5] = 0;
		sys_info.act_stan_vals[6] = 0;
		sys_info.act_stan_vals[7] = 0;
		sys_info.act_stan_vals[8] = 0;
		sys_info.act_stan_vals[9] = 0;
		sys_info.act_stan_vals[10] = 0;
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
		sys_info.val_cal_x = 1;//green
		sys_info.val_cal_y = 2;//blue
		sys_info.act_stan_vals[0] = 0;
		sys_info.act_stan_vals[1] = 1;
		sys_info.act_stan_vals[2] = 1;
		sys_info.act_stan_vals[3] = 2;
		sys_info.act_stan_vals[4] = 2;
		sys_info.act_stan_vals[5] = 4;
		sys_info.act_stan_vals[6] = 4;
		sys_info.act_stan_vals[7] = 10;
		sys_info.act_stan_vals[8] = 10;
		sys_info.act_stan_vals[9] = 20;
		sys_info.act_stan_vals[10] = 20;
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
		sys_info.val_cal_x = 0;//
		sys_info.val_cal_y = 2;//
		sys_info.act_stan_vals[0] = 0;//verry low							1
		sys_info.act_stan_vals[1] = 1.00;//verry low							1
		sys_info.act_stan_vals[2] = 1.00;////verry low						1
		sys_info.act_stan_vals[3] = 5;//low					//Deficient		2
		sys_info.act_stan_vals[4] = 10;//low				//Sufficient	2
		sys_info.act_stan_vals[5] = 20;//Medium				//Sufficient	3
		sys_info.act_stan_vals[6] = 30;//Medium								3
		sys_info.act_stan_vals[7] = 40;//High								4
		sys_info.act_stan_vals[8] = 40;//High								4
		sys_info.act_stan_vals[9] = 50;//Verry High 						5
		sys_info.act_stan_vals[10] = 50;//Verry High 						5
		break;
	case POTASSIUM:
		sys_info.chem_1a_qty = 500;
		sys_info.chem_1b_qty = 100;
		sys_info.chem_2c_qty = 100;
		sys_info.chem_3d_qty = 55;
		sys_info.chem_4e_qty = 100;
		sys_info.chem_5f_qty = 0;
		sys_info.wtr_g_qty = 520;
		sys_info.led_pwm_red = 0xff;
		sys_info.led_pwm_green = 0xff;
		sys_info.led_pwm_blue = 0xff;
		sys_info.val_cal_x = 1;//green
		sys_info.val_cal_y = 2;//blue
		sys_info.act_stan_vals[0] = 0;
		sys_info.act_stan_vals[1] = 0;
		sys_info.act_stan_vals[2] = 0;
		sys_info.act_stan_vals[3] = 0;
		sys_info.act_stan_vals[4] = 0;
		sys_info.act_stan_vals[5] = 0;
		sys_info.act_stan_vals[6] = 0;
		sys_info.act_stan_vals[7] = 0;
		sys_info.act_stan_vals[8] = 0;
		sys_info.act_stan_vals[9] = 0;
		sys_info.act_stan_vals[10] = 0;
		break;
	case PHOSPHORUS:
		sys_info.chem_1a_qty = 1000;
		sys_info.chem_1b_qty = 0;
		sys_info.chem_2c_qty = 25;
		sys_info.chem_3d_qty = 200;
		sys_info.chem_4e_qty = 100;
		sys_info.chem_5f_qty = 0;
		sys_info.wtr_g_qty = 475;
		sys_info.led_pwm_red = 0xff;
		sys_info.led_pwm_green = 0xff;
		sys_info.led_pwm_blue = 0xff;
		sys_info.val_cal_x = 1;//green
		sys_info.val_cal_y = 2;//blue
		sys_info.act_stan_vals[0] = 0.00;
		sys_info.act_stan_vals[1] = 0.1;
		sys_info.act_stan_vals[2] = 0.2;
		sys_info.act_stan_vals[3] = 0.4;
		sys_info.act_stan_vals[4] = 0.4;
		sys_info.act_stan_vals[5] = 0.6;
		sys_info.act_stan_vals[6] = 0.6;
		sys_info.act_stan_vals[7] = 0.8;
		sys_info.act_stan_vals[8] = 0.8;
		sys_info.act_stan_vals[9] = 1.0;
		sys_info.act_stan_vals[10] = 1.0;
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
		sys_info.val_cal_x = 1;//green
		sys_info.val_cal_y = 2;//blue
		sys_info.act_stan_vals[0] = 0;
		sys_info.act_stan_vals[1] = 2;
		sys_info.act_stan_vals[2] = 2;
		sys_info.act_stan_vals[3] = 4;
		sys_info.act_stan_vals[4] = 4;
		sys_info.act_stan_vals[5] = 6;
		sys_info.act_stan_vals[6] = 6;
		sys_info.act_stan_vals[7] = 8;
		sys_info.act_stan_vals[8] = 8;
		sys_info.act_stan_vals[9] = 10;
		sys_info.act_stan_vals[10] = 10;
		break;
	case MASTER_SYS:
	case ORGANIC_CARBON:
		sys_info.chem_1a_qty = 1000;
		sys_info.chem_1b_qty = 0;
		sys_info.chem_2c_qty = 1000;
		sys_info.chem_3d_qty = 0;
		sys_info.chem_4e_qty = 0;
		sys_info.chem_5f_qty = 0;
		sys_info.wtr_g_qty = 0;
		sys_info.led_pwm_red = 0xff;
		sys_info.led_pwm_green = 0xff;
		sys_info.led_pwm_blue = 0xff;
		sys_info.val_cal_x = 1;//green
		sys_info.val_cal_y = 2;//blue
		sys_info.act_stan_vals[0] = 0;
		sys_info.act_stan_vals[1] = 0;
		sys_info.act_stan_vals[2] = 0;
		sys_info.act_stan_vals[3] = 0;
		sys_info.act_stan_vals[4] = 0;
		sys_info.act_stan_vals[5] = 0;
		sys_info.act_stan_vals[6] = 0;
		sys_info.act_stan_vals[7] = 0;
		sys_info.act_stan_vals[8] = 0;
		sys_info.act_stan_vals[9] = 0;
		sys_info.act_stan_vals[10] = 0;
		break;
	}
	sys_info.wtr_f_wsh = 1300;
	sys_info.wtr_f_wsh_in_tm = sys_info.wtr_f_wsh * TM_MULTIPLIER;
	sys_info.drain_tm = (1000*55);//55 sec
	sys_info.drain_mix_tm = (1000*10);//10 sec
	sys_info.tak_data_skp_tm = (10*25);//solution rest time//25 sec
	sys_info.auto_zero_skp_tm = (10*25);//solution rest time//25 sec
	//sys_info.auto_set_skp_tm = (10*25);//solution rest time//25 sec

	for(uint8_t i = 0; i < NOS_STD; i++)
	{
		for(uint8_t j = 0; j < 4; j++)
		{
			sys_info.opt_std_vars.strd_vars[i][j] = save_sys_info.bk_var.hrd_std_vars.strd_vars[i][j];
			sys_info.opt_std_vars2.strd_vars[i][j] = save_sys_info.bk_var.hrd_std_vars2.strd_vars[i][j];
		}
	}

	sys_info.Stat_L.warm_up_stat = 0;
	sys_info.Stat_L.drain_wsh_stat = 0;//1
}
#endif

void dflt_tm_set(void)
{
	save_sys_info.bk_var.chem_1a_in_tm = sys_info.chem_1a_qty * TM_MULTIPLIER;
	save_sys_info.bk_var.chem_1b_in_tm = sys_info.chem_1b_qty * TM_MULTIPLIER;
	save_sys_info.bk_var.chem_2c_in_tm = sys_info.chem_2c_qty * TM_MULTIPLIER;
	save_sys_info.bk_var.chem_3d_in_tm = sys_info.chem_3d_qty * TM_MULTIPLIER;
	save_sys_info.bk_var.chem_4e_in_tm = sys_info.chem_4e_qty * TM_MULTIPLIER;
	save_sys_info.bk_var.chem_5f_in_tm = sys_info.chem_5f_qty * TM_MULTIPLIER;
	save_sys_info.bk_var.wtr_g_in_tm = sys_info.wtr_g_qty * TM_MULTIPLIER;

	sys_info.sys_f.mem_save = 1;
}

void gpio_config_check(void)
{
//gpio check
//	sys_info.set_sys_add = MAGNESIUM;
//	sys_info.set_sys_add = IRON;
//	sys_info.set_sys_add = COPPER;
//	sys_info.set_sys_add = ZINC;
//	sys_info.set_sys_add = BORON;
//	sys_info.set_sys_add = SULPHUR;
//	sys_info.set_sys_add = POTASSIUM;
//	sys_info.set_sys_add = PHOSPHORUS;
	sys_info.set_sys_add = NITROGEN;
//	sys_info.set_sys_add = ORGANIC_CARBON;
}

void sys_mem_validate(void)
{
	//note:- we should not do mid program parameter change, if we do it will remain till machine "ON"
	gpio_config_check();

	if( (mem_fresh_check()) || (sw_update()) || (validate_data() ) )
	{
		wrt_dflt();
#ifdef ARC_CAL
		sys_var_init();
#endif
#ifdef OLD_CAL
		sys_var_init_old();
#endif
		dflt_tm_set();
	}
	else
	{
#ifdef ARC_CAL
		sys_var_init();
#endif
#ifdef OLD_CAL
		sys_var_init_old();
#endif
	}
}

void global_timer_100msec(void)
{
	if(sys_info.Stat_L.warm_up_stat)
	{//manage warmup time
		sys_info.wrm_up_tm++;
		if(sys_info.wrm_up_tm >= WRM_UP_TM)
		{sys_info.Stat_L.warm_up_stat = 0;sys_info.wrm_up_tm = WRM_UP_TM;}
	}

	if(sys_info.Stat_L.sen_stat)
	{
		//manage take data skipp time
		if(sys_info.Stat_M.tak_data_stat)
		{
		  sys_info.tak_data_skp_run_tm++;
		  /*if(sys_info.tak_data_skp_run_tm >= sys_info.tak_data_skp_tm)
		  { sys_info.tak_data_skp_run_tm = sys_info.tak_data_skp_tm; }
		  */
		}

		//manage take data skipp time
		if(sys_info.Stat_L.auto_zero)
		{
			sys_info.auto_zero_skp_run_tm++;
			/*if(sys_info.auto_zero_skp_run_tm >= sys_info.auto_zero_skp_tm)
			{ sys_info.auto_zero_skp_run_tm = sys_info.auto_zero_skp_tm; }
			*/
		}

		//manage auto_set skipp time
		if(sys_info.Stat_M2.auto_set)
		{
			sys_info.auto_set_skp_run_tm++;
			/*if(sys_info.auto_set_skp_run_tm >= sys_info.auto_set_skp_tm)
			{ sys_info.auto_set_skp_run_tm = sys_info.auto_set_skp_tm; }
			*/
		}
	}
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
