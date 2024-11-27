/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "hc_sr04.h"
#include "math.h"
#include "rdebug.h"
#include "stdio.h"
#include "stdint.h"
#include "stdlib.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
	int servoEnGrados(int grados){
		int gradosParaPlaca=(ceil((grados*11.111)+500));
		return gradosParaPlaca;
	}

	int cicloFor(TIM_HandleTypeDef servoMotor){
		int detector=0;
		for(int i=0;i<51;i++){
			servoMotor.Instance->CCR1=servoEnGrados(i);
			if(HCSR04_Get_Distance()<9){
				detector+=1;
			}
			HAL_Delay(40);
		}
		return detector;
	}

	void setBothServos(TIM_HandleTypeDef servoMotor1,int gradosServoMotor1,TIM_HandleTypeDef servoMotor2,int gradosServoMotor2){
		servoMotor1.Instance->CCR1=servoEnGrados(gradosServoMotor1);
		servoMotor2.Instance->CCR1=servoEnGrados(gradosServoMotor2);
	}

	void concatenar(int pagar, int pagado, char* tipoVehi){
		char resultado[50];

		sprintf(resultado, "%d,%d,%s", pagar, pagado, tipoVehi);

		rprintf("%s\n",resultado);
	}

	int pagar(int pagar){
		int pagado=0;
		while(pagar>pagado){
			if(HAL_GPIO_ReadPin(RecibidorUnPeso_GPIO_Port, RecibidorUnPeso_Pin)==0){
				pagado+=1;
				HAL_Delay(100);
			}
			if(HAL_GPIO_ReadPin(RecibidorDosPesos_GPIO_Port, RecibidorDosPesos_Pin)==0){
				pagado+=2;
				HAL_Delay(100);
			}
			if(HAL_GPIO_ReadPin(RecibidorCincoPesos_GPIO_Port, RecibidorCincoPesos_Pin)==0){
				pagado+=5;
				HAL_Delay(100);
			}
			if(HAL_GPIO_ReadPin(RecibidorDiezPesos_GPIO_Port,RecibidorDiezPesos_Pin)==0){
				pagado+=10;
				HAL_Delay(100);
			}
			concatenar(pagar,pagado,"");
			HAL_Delay(100);
		}
		return pagado;
	}

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

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
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_USART6_UART_Init();
  /* USER CODE BEGIN 2 */
  //HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  MX_TIM1_Init();
  HCSR04_Init();

  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);

  //Para el servo de la pluma
  TIM_HandleTypeDef servoPluma=htim1;

  //Para el servo que detecta el vehiculo
  TIM_HandleTypeDef servoMotorDetector=htim2;
  //Para el servo que baja el sensor
  TIM_HandleTypeDef servoMotorBajador=htim3;
  int detectarVehiculo;
  int moto=12;
  int carro=17;
  int camion=30;

  int pay;
  int paid;
  char* tipoVehiculo;
  while (1)
  {
	  detectarVehiculo=0;
	  HAL_GPIO_WritePin(Mandador_GPIO_Port, Mandador_Pin, 1);
		  setBothServos(servoMotorDetector, 10, servoMotorBajador, 60);
		  	  detectarVehiculo=cicloFor(servoMotorDetector);
		  	  if(detectarVehiculo>3){
		  		  //Significa que es camion
		  		  tipoVehiculo="Camion";
		  		  pay=camion;
		  		  HAL_Delay(100);
		  	  }else{
		  		  setBothServos(servoMotorDetector, 10, servoMotorBajador, 0);
		  		  detectarVehiculo=cicloFor(servoMotorDetector);

		  		  if(detectarVehiculo>3){
		  			  //tipoVehiculo='A';
		  			  tipoVehiculo="Automovil";
		  			  pay=carro;
		  			  HAL_Delay(100);
		  		  }else{
		  			  //tipoVehiculo='M';
		  			  tipoVehiculo="Motocicleta";
		  			  pay=moto;
		  			  HAL_Delay(100);
		  		  }
		  	  }
		  	  paid=pagar(pay);
		  	  concatenar(pay,paid,tipoVehiculo);



	  HAL_Delay(50);



	  /*
	   if(distancia<50){
		  while(htim2.Instance->CCR1< 2500){
			  htim2.Instance->CCR1+=10;
			  HAL_Delay(40);
		  }
		  htim2.Instance->CCR1=500;
		  distancia=HCSR04_Get_Distance();
		  distancia=1000;
		  delai=100;
	  }
	   */


	  //HAL_Delay(delai);
	  //delai=50;




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
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 72;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

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
