/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : 0602 MatrixKey 3×4矩阵键盘 → PF口LED输出421‑BCD
  *                 行：PE4 PE5 PE6    列：PE0 PE1 PE2 PE3
  *                 PF0~PF3 BCD输出，LED低电平点亮
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
static uint8_t KeyVal = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */
static void MatrixKey_Scan(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/**
 * @brief 3×4矩阵键盘扫描
 * 行：PE4、PE5、PE6
 * 列：PE0、PE1、PE2、PE3（上拉输入）
 * PF0-PF3输出BCD，低电平点亮LED
 */
static void MatrixKey_Scan(void)
{
    uint8_t i;
    uint16_t colState;

    // 依次扫描3行
    for(i = 0; i < 3; i++)
    {
        // 全部行先拉高
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6, GPIO_PIN_SET);
        // 拉低当前行
        if(i == 0) HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_RESET);
        if(i == 1) HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_RESET);
        if(i == 2) HAL_GPIO_WritePin(GPIOE, GPIO_PIN_6, GPIO_PIN_RESET);

        HAL_Delay(5);
        colState = GPIOE->IDR & 0x0FU;

        if(colState != 0x0FU)
        {
            HAL_Delay(16);  //消抖
            colState = GPIOE->IDR & 0x0FU;
            if(colState != 0x0FU)
            {
                switch(colState)
                {
                    case 0x0E: KeyVal = i*4 + 1; break;  //PE0按下
                    case 0x0D: KeyVal = i*4 + 2; break;  //PE1按下
                    case 0x0B: KeyVal = i*4 + 3; break;  //PE2按下
                    case 0x07: KeyVal = i*4 + 4; break;  //PE3按下
                    default: break;
                }
                //等待松手
                while( (GPIOE->IDR & 0x0FU) != 0x0FU );
                HAL_Delay(10);
            }
        }
    }

    // PF4~PF7保持不变，PF0~PF3输出BCD，低亮
    GPIOF->ODR = (GPIOF->ODR & 0xF0U) | ( ~KeyVal & 0x0FU );
}

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
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    MatrixKey_Scan();
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  * @note 这个函数是CubeMX自动生成，不要手动改！如果报错：回到.ioc重新Generate Code
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  RCC_OscInitStruct.OscillatorType      = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState            = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState        = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource       = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM            = 8;
  RCC_OscInitStruct.PLL.PLLN            = 336;
  RCC_OscInitStruct.PLL.PLLP           = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ            = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType           = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                                          |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource        = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider       = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider      = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider      = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();

  // PE0‑PE3 上拉输入（列）
  GPIO_InitStruct.Pin  = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  // PE4‑PE6 推挽输出（行），初始高
  GPIO_InitStruct.Pin   = GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6;
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull  = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6, GPIO_PIN_SET);
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  // PF0‑PF7 推挽输出，初始高
  GPIO_InitStruct.Pin   = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                        |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull  = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                        |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_SET);
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);
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

  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
