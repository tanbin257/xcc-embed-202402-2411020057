#include "keypad.h"

#define ROW_PORT GPIOE
uint16_t ROW_PIN[3] = {GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_2};

#define COL_PORT GPIOE
uint16_t COL_PIN[4] = {GPIO_PIN_3, GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_6};

//3*4键盘扫描，返回0~11，无按键返回0xFF
uint8_t Key_Scan(void)
{
  uint8_t i, j;
  uint8_t keyval = 0xFF;
  //全部行拉高
  for(i = 0; i < 3; i++)
  {
    HAL_GPIO_WritePin(ROW_PORT, ROW_PIN[i], GPIO_PIN_SET);
  }
  //逐行扫描
  for(i = 0; i < 3; i++)
  {
    HAL_GPIO_WritePin(ROW_PORT, ROW_PIN[i], GPIO_PIN_RESET);
    HAL_Delay(1);
    for(j = 0; j < 4; j++)
    {
      if(HAL_GPIO_ReadPin(COL_PORT, COL_PIN[j]) == GPIO_PIN_RESET)
      {
        HAL_Delay(10); //消抖
        if(HAL_GPIO_ReadPin(COL_PORT, COL_PIN[j]) == GPIO_PIN_RESET)
        {
          keyval = i * 4 + j;
          while(HAL_GPIO_ReadPin(COL_PORT, COL_PIN[j]) == GPIO_PIN_RESET); //等待松手
          HAL_Delay(10);
        }
      }
    }
    HAL_GPIO_WritePin(ROW_PORT, ROW_PIN[i], GPIO_PIN_SET);
  }
  return keyval;
}

//键值映射 3行4列
//0:1,1:2,2:3,3:A
//4:4,5:5,6:6,7:B
//8:7,9:8,10:9,11:0
uint8_t Key2BCD(uint8_t key)
{
  uint8_t num;
  switch(key)
  {
    case 0:  num = 1; break;
    case 1:  num = 2; break;
    case 2:  num = 3; break;
    case 4:  num = 4; break;
    case 5:  num = 5; break;
    case 6:  num = 6; break;
    case 8:  num = 7; break;
    case 9:  num = 8; break;
    case 10: num = 9; break;
    case 11: num = 0; break;
    default: return 0xFF;
  }
  return num;
}
