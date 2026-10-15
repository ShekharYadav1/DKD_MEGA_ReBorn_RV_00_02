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
#include "hard_std_vals.h"
#include "auto_zero_adjust.h"
#include "Cal_Formula.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define ARC_CAL
//#define OLD_CAL

#define TM_MULTIPLIER	20                          // 20ms * 20 = 400ms, this is use for pump time calculation. 1ms pump timer is used for drain and drain mix time calculation.

#define FLASH_OVER_WRITE                           // this is used to overwrite the flash memory if it is not valid. If this is not defined then it will not overwrite the flash memory if it is not valid.
#define MEM_ID1 0xAA                              // this is used to identify the flash memory. If this is not defined then it will not overwrite the flash memory if it is not valid.
#define MEM_ID2 0x55
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
extern double factor_value;                       // we can use global variables as needed for calibration

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
sys_var_typdef sys_info;                           // this is the main system variable which contains all the system variables.
union_bk_var_typdef save_sys_info;                // this is the backup variable which contains all the system variables. This is used to save the system variables in flash memory.

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);                    // this function is used to configure the system clock.
/* USER CODE BEGIN PFP */   
void sys_mem_validate(void);                     
void auto_zero_adjust(unn_std_var_typdef* opt_std_vars);
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
  HAL_Init();                                                // this function is used to initialize the HAL library. It must be called before any other HAL function. It initializes the Flash interface, the Systick timer, and the NVIC priority grouping.

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();                                                // this function is used to configure the system clock. It must be called before any other HAL function that uses the system clock.

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();		 //all gpios init for  motors 
  MX_I2C1_Init();		 // sensor i2c init
  MX_TIM3_Init();		 // pwm init for led
  MX_USART2_UART_Init(); // uart rs485 for GUI init
  /* USER CODE BEGIN 2 */
  // HAL_Delay(100);
  Flash_Read_Data(MEM_STRT_ADD, save_sys_info.bkdata, MEM_SIZE);     // this function is used to read the flash memory. It reads the flash memory from the start address and stores it in the backup variable. The size of the data to be read is defined by MEM_SIZE.
  sys_mem_validate();                                                // this function is used to validate the flash memory. It checks if the flash memory is valid or not. If it is not valid, it will overwrite the flash memory with the default values.
  TCS3472X_init(&hi2c1);                                             // this function is used to initialize the TCS34725 sensor. It takes the I2C handle as an argument. The I2C handle is used to communicate with the sensor.
  RGB_Led_init(&htim3);                                              // this function is used to initialize the RGB LED. It takes the timer handle as an argument. The timer handle is used to generate the PWM signal for the LED.
  GUI_Comm_init(&huart2);                                            // this function is used to initialize the GUI communication. It takes the UART handle as an argument. The UART handle is used to communicate with the GUI.
  pump1(0);
  pump2(0);
  pump3(0);
  pump4(0);
  pump5(0);
  pump6(0);
  pump7(0);
  pump8(0);
  motor1(0);                                                    // here initially all the pumps and motors are turned off. The argument 0 means off and 1 means on. The pumps and motors are controlled by the pump and motor functions. The pump and motor functions are defined in the Pump.c file.
  // PUMP1OFF;PUMP2OFF;PUMP3OFF;PUMP4OFF;PUMP5OFF;PUMP6OFF;PUMP7OFF;
  // PUMP8OFF;MOTOR1OFF;//pump & motors
  set_Chnl_PWM(sys_info.led_pwm_red, sys_info.led_pwm_green, sys_info.led_pwm_blue);      // this function is used to set the PWM values for the RGB LED. It takes the red, green and blue values as arguments. The values are defined in the system variable. The values are in the range of 0 to 255. The PWM values are used to control the brightness of the LED.

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  if ((sys_info.sys_f.mem_save == 1) && (sys_info.sys_f.comm_mode == RCV_MODE))                // this condition is used to check if the flash memory needs to be saved. The flash memory is saved only if the mem_save flag is set and the comm_mode is set to receive mode. The comm_mode is set to receive mode when the GUI sends a command to save the flash memory. The mem_save flag is set when the system variables are changed and need to be saved in the flash memory.
	  {
		  Flash_Write_Data(MEM_STRT_ADD, save_sys_info.bkdata, MEM_SIZE);                        // this function is used to write the flash memory. It writes the flash memory from the start address and stores it in the backup variable. The size of the data to be written is defined by MEM_SIZE.
		  sys_info.sys_f.mem_save = 0;                                                          // this flag is cleared after the flash memory is saved. It indicates that the flash memory has been saved and does not need to be saved again until the system variables are changed.
	  }
	  TCS3472X_handler();                                                                      // this function is used to handle the TCS34725 sensor. It reads the sensor data and stores it in the system variable. The sensor data is read only if the sensor is initialized and the I2C communication is idle. The sensor data is read in a non-blocking manner using interrupts. The sensor data is read in a non-blocking manner using interrupts. The sensor data is read in a non-blocking manner using interrupts.
	  get_sen_stat() ? (sys_info.Stat_L.sen_stat |= 1) : (sys_info.Stat_L.sen_stat &= 0);      // this condition is used to check if the sensor is initialized and the sensor data is valid. The sen_stat flag is set if the sensor data is valid and cleared if the sensor data is invalid. The sen_stat flag is used to indicate the status of the sensor. The sen_stat flag is used to indicate the status of the sensor. The sen_stat flag is used to indicate the status of the sensor.
	  if (sys_info.Stat_L.sen_stat)                                                            // this condition is used to check if the sensor data is valid. The sensor data is valid only if the sensor is initialized and the sensor data is valid. The sensor data is valid only if the sensor is initialized and the sensor data is valid. The sensor data is valid only if the sensor is initialized and the sensor data is valid.
	  {
		  get_mavg_rgbc_val(&sys_info.curr_rgbc_vars.curr_red_rcv, &sys_info.curr_rgbc_vars.curr_green_rcv, &sys_info.curr_rgbc_vars.curr_blue_rcv, &sys_info.curr_rgbc_vars.curr_clear_rcv); // this function is used to get the average RGB and clear values from the sensor. It takes the pointers to the red, green, blue and clear variables as arguments. The average values are calculated by taking the average of the last 10 readings from the sensor. The average values are stored in the system variable. The average values are used to calculate the absorbance values and the result category.
		  if (!sys_info.Stat_L.warm_up_stat)                                                                                                                                                   // this condition is used to check if the system is in warm up mode. The warm up mode is set when the system is powered on and the sensor is initialized. The warm up mode is cleared when the system is ready to take data. The warm up mode is used to indicate that the system is not ready to take data. The warm up mode is used to indicate that the system is not ready to take data.
		  {
			  if (sys_info.Stat_M.tak_data_stat)                                               // this condition is used to check if the system is in take data mode. The take data mode is set when the system is ready to take data. The take data mode is cleared when the system is not ready to take data. The take data mode is used to indicate that the system is ready to take data. The take data mode is used to indicate that the system is ready to take data.                                    
			  {
				  if (sys_info.tak_data_skp_run_tm >= sys_info.tak_data_skp_tm)
				  {
					//  if ((!sys_info.curr_ResVal) && (!sys_info.curr_Result_cat))
					//  {
						  cal_result();
					//  }
					  // sys_info.Stat_L.drain_wsh_stat = 1;
					  sys_info.Stat_M.tak_data_stat = 0;
					  sys_info.Stat_L.alt_func = 0;
					  sys_info.tak_data_skp_run_tm = 0;
				  }
			  }
			  if (sys_info.Stat_L.auto_zero)
			  {
				  if (sys_info.auto_zero_skp_run_tm >= sys_info.auto_zero_skp_tm)
				  {
#ifdef ARC_CAL
					  if (sys_info.Stat_L.alt_func == 0)
					  {
						  auto_zero_adjust(&sys_info.opt_std_vars);
//						  auto_zero_adjust(&sys_info.opt_std_vars2);
					  }
					  else if (sys_info.Stat_L.alt_func == 1)
					  {
//						  auto_zero_adjust(&sys_info.opt_std_vars);
						  auto_zero_adjust(&sys_info.opt_std_vars2);
						  sys_info.Stat_L.alt_func = 0;
					  }
#endif

					  sys_info.Stat_L.auto_zero_save = 1;
					  sys_info.Stat_L.alt_func = 0;
					  sys_info.Stat_L.auto_zero = 0;
					  // sys_info.Stat_L.drain_wsh_stat = 1;
					  sys_info.auto_zero_skp_run_tm = 0;
					  sys_info.curr_ResVal = 0;
					  sys_info.curr_Result_cat = 0;
				  }
			  }

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

#if 0
void cal_result(void)
{
#ifdef ARC_CAL
	
	unn_std_var_typdef *p_std_vars = (sys_info.Stat_L.alt_func == 1) ? &sys_info.opt_std_vars2 : &sys_info.opt_std_vars;

	switch (sys_info.set_sys_add)
	{
	case SULPHUR:
		SUL_value_calculation1(p_std_vars);              // calibration done
		break;

	case PHOSPHORUS:
		PHOS_value_calculation2(p_std_vars);             // calibration done
		break;

	case NITROGEN:
		NIT_interpolation_calculation3(p_std_vars);      // interpolation formula
		break;

	case POTASSIUM:
		POT_value_calculation5(p_std_vars);              // calibration done
		break;

	case ORGANIC_CARBON: 
		OC_value_calculation6(p_std_vars);               // calibration done
		break;

	case COPPER:
		COP_value_calculation4(p_std_vars);              // red channel interpolation
		break;

	default:
		// Unknown or uncalibrated nutrient
		sys_info.curr_ResVal = 0;
		sys_info.curr_Result_cat = 0;
		break;
	}

	sys_info.Stat_L.alt_func = 0; // Operation complete होने पर रीसेट
#endif
}
#endif


void cal_result(void)
{

#ifdef ARC_CAL

	if (sys_info.Stat_L.alt_func == 0)
	{
		switch (sys_info.set_sys_add)
		{
		case SULPHUR:
			SUL_value_calculation1(&sys_info.opt_std_vars);              // calibration done
		   break;
		case PHOSPHORUS:
			PHOS_value_calculation2(&sys_info.opt_std_vars); 
			break;                                                        // calibration done
		case NITROGEN:
//			 NIT_value_calculation3(&sys_info.opt_std_vars);           // calibration done  
			 NIT_interpolation_calculation3(&sys_info.opt_std_vars); // interpolation formula
			break;                                                                                                     // pending calibration
		case POTASSIUM:
			POT_value_calculation5(&sys_info.opt_std_vars);
			break;                                                  //  calibration Done
        case ORGANIC_CARBON: 
		    OC_value_calculation6(&sys_info.opt_std_vars);         // calibration done
		     break;
		case COPPER:
			COP_value_calculation4(&sys_info.opt_std_vars); // best for red channel , similer to oc
		break; // pending calibration
		case BORON:
			COP_value_calculation4(&sys_info.opt_std_vars); // pending calibration(same formula as copper)
		    break;

		default:
					
		sys_info.curr_ResVal = 0;
		sys_info.curr_Result_cat = 0;
			break;
		}
	}
	else if (sys_info.Stat_L.alt_func == 1)
	{
		switch (sys_info.set_sys_add)
		{
		case SULPHUR:
			SUL_value_calculation1(&sys_info.opt_std_vars2);   // done calibration
			break;
		case PHOSPHORUS:
			PHOS_value_calculation2(&sys_info.opt_std_vars2);    // done calibration
			break;
		case NITROGEN:
//			 NIT_value_calculation3(&sys_info.opt_std_vars2);              // done calibration 
			 NIT_interpolation_calculation3(&sys_info.opt_std_vars2); // interpolation formula
			break;                                                                                                 
		case POTASSIUM:
			POT_value_calculation5(&sys_info.opt_std_vars2);         // done calibration
			break; 
        case ORGANIC_CARBON:
		     OC_value_calculation6(&sys_info.opt_std_vars2);        // done calibration
		    break;
		case COPPER:
			COP_value_calculation4(&sys_info.opt_std_vars2);
			break;                                                    // pending calibration
		case BORON:
			COP_value_calculation4(&sys_info.opt_std_vars2);       // pending calibration (same formula as copper)
		    break;
		default:
            		
		sys_info.curr_ResVal = 0;
		sys_info.curr_Result_cat = 0;
		break;
		}
		sys_info.Stat_L.alt_func = 0;
	}
#endif
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

	save_sys_info.bk_var.end_add1 = 0x55;
	save_sys_info.bk_var.end_add2 = 0xAA;
}




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
 // sys_info.set_sys_add = ORGANIC_CARBON;
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

		dflt_tm_set();
	}
	else
	{
#ifdef ARC_CAL
		sys_var_init();
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
