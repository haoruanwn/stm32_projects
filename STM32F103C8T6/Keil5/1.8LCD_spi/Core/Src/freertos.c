/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "semphr.h"
#include "usart.h"
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
/* USER CODE BEGIN Variables */
uint32_t adc_value;
uint32_t voltage;

float P_value = 0.0;
float I_value = 0.0;
float D_value = 0.0;
/* USER CODE END Variables */
/* Definitions for TaskShow */
osThreadId_t TaskShowHandle;
const osThreadAttr_t TaskShow_attributes = {
  .name = "TaskShow",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for DMAData */
osThreadId_t DMADataHandle;
const osThreadAttr_t DMAData_attributes = {
  .name = "DMAData",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityBelowNormal,
};
/* Definitions for myBinSem_DataReady */
osSemaphoreId_t myBinSem_DataReadyHandle;
const osSemaphoreAttr_t myBinSem_DataReady_attributes = {
  .name = "myBinSem_DataReady"
};
/* Definitions for myBinDMA */
osSemaphoreId_t myBinDMAHandle;
const osSemaphoreAttr_t myBinDMA_attributes = {
  .name = "myBinDMA"
};
/* Definitions for Sem_Tables */
osSemaphoreId_t Sem_TablesHandle;
const osSemaphoreAttr_t Sem_Tables_attributes = {
  .name = "Sem_Tables"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void AppTaskShow(void *argument);
void APP_DMAData(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* creation of myBinSem_DataReady */
  myBinSem_DataReadyHandle = osSemaphoreNew(1, 1, &myBinSem_DataReady_attributes);

  /* creation of myBinDMA */
  myBinDMAHandle = osSemaphoreNew(1, 1, &myBinDMA_attributes);

  /* creation of Sem_Tables */
  Sem_TablesHandle = osSemaphoreNew(5, 0, &Sem_Tables_attributes);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of TaskShow */
  TaskShowHandle = osThreadNew(AppTaskShow, NULL, &TaskShow_attributes);

  /* creation of DMAData */
  DMADataHandle = osThreadNew(APP_DMAData, NULL, &DMAData_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_AppTaskShow */
/**
  * @brief  Function implementing the TaskShow thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_AppTaskShow */
void AppTaskShow(void *argument)
{
  /* USER CODE BEGIN AppTaskShow */
  
  /* Infinite loop */
  for(;;)
  {
    if (xSemaphoreTake(myBinSem_DataReadyHandle, portMAX_DELAY) == pdTRUE)//等待信号量，如果信号量可用则返回pdTRUE，否则不执行，任务进入阻塞�??
    {
      
      LCD_ShowString(10,30,16,"adc_value:",1);
      LCD_ShowNum(90,30,adc_value,4,16);
      
      
      voltage = adc_value * 36762  >> 12;
      LCD_ShowString(10,50,16,"Voltage:",1);
      LCD_ShowNum(90,50, voltage,5,16);
      LCD_ShowString(130,50,16,"mV",1);
      //printf("Voltage:%d\n",voltage);
      
    }
	  
   
		
    osDelay(100);
  }
  /* USER CODE END AppTaskShow */
}

/* USER CODE BEGIN Header_APP_DMAData */
/**
* @brief Function implementing the DMAData thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_APP_DMAData */
void APP_DMAData(void *argument)
{
  /* USER CODE BEGIN APP_DMAData */
  char *data_format = "P:%f, I:%f, D:%f"; // 用于解析的格式字符串
  float P_temp, I_temp, D_temp;           // 临时存储解析到的 PID 参数
  
  /* Infinite loop */
  for (;;)
  {
    if (xSemaphoreTake(myBinDMAHandle, portMAX_DELAY) == pdTRUE) // 等待信号量，如果信号量可用则返回pdTRUE，否则不执行，任务进入阻塞�??
    {
      if (xUSART1.ReceiveNum) // 判断字节�?
      {
        printf("Received Data: %s\r", (char *)xUSART1.ReceiveData); // 显示接收到的数据
        
        // 尝试解析接收到的字符串，解析成功时返回解析的参数个数
        if (sscanf((char *)xUSART1.ReceiveData, data_format, &P_temp, &I_temp, &D_temp) == 3)
        {
          // 成功解析到 PID 参数，赋值给全局变量
          P_value = P_temp;
          I_value = I_temp;
          D_value = D_temp;

          // 打印解析后的 PID 值
          printf("Parsed PID values: P = %f, I = %f, D = %f\r", P_value, I_value, D_value);
          
          // 可以在此添加将解析结果显示到 LCD 的代码
          char buffer[50];
          sprintf(buffer, "P:%.2f I:%.2f D:%.2f", P_value, I_value, D_value);
          LCD_ShowString(10, 70, 16, buffer, 1);
        }
        else
        {
          // 如果解析失败，显示错误信息
          printf("Error: Invalid PID data format\r");
        }

        xUSART1.ReceiveNum = 0; // �?0接收标记
      }
    }
    vTaskDelay(100);
  }
  /* USER CODE END APP_DMAData */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
  if(hadc->Instance == ADC1)
  {
    adc_value = HAL_ADC_GetValue(hadc);
    BaseType_t hightaskWoken = pdFALSE;//定义1个变量，来表示推出ISR之前是否�??????要进行一次任务调度申�???????
    if (myBinSem_DataReadyHandle != NULL)
    {
      xSemaphoreGiveFromISR(myBinSem_DataReadyHandle, &hightaskWoken);//在ADC转换完成后释放信号量，ISR中调度任�??????
      portYIELD_FROM_ISR(hightaskWoken);//如果hightaskWoken为pdTRUE，则进行1次任务调�??????
    }
    
  }
}

  

//DMA空闲中断回调函数
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    if (huart == &huart1)                                                                    // 判断串口
    {
      BaseType_t hightaskWoken = pdFALSE;                                                    // 定义1个变量，来表示推出ISR之前是否需要进行一次任务调度申请
      __HAL_UNLOCK(huart);                                                                   // 解锁串口状态
 
      xUSART1.ReceiveNum  = Size;                                                            // 把接收字节数，存入结构体xUSART1.ReceiveNum，以备使用
      memset(xUSART1.ReceiveData, 0, sizeof(xUSART1.ReceiveData));                           // 清零前一帧的接收数据
      memcpy(xUSART1.ReceiveData, xUSART1.BuffTemp, Size);                                   // 把新数据，从临时缓存中，复制到xUSART1.ReceiveData[], 以备使用
      HAL_UARTEx_ReceiveToIdle_DMA(&huart1, xUSART1.BuffTemp, sizeof(xUSART1.BuffTemp));     // 再次启动DMA空闲中断; 每当接收完指定长度，或产生空闲中断时，就会来到这里

      //DMA完成一次接收后，释放信号量，通知任务处理数据
      if (myBinDMAHandle != NULL)
      {
        xSemaphoreGiveFromISR(myBinDMAHandle, &hightaskWoken);                               // 释放信号量，通知任务处理数据
        portYIELD_FROM_ISR(hightaskWoken);                                                   // 如果hightaskWoken为pdTRUE，则进行一次任务调度
      }

    }
}


/* USER CODE END Application */

