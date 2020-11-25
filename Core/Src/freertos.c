/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : freertos.c
 * Description        : Code for freertos applications
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Ultimate Liberty license
 * SLA0044, the "License"; You may not use this file except in compliance with
 * the License. You may obtain a copy of the License at:
 *                             www.st.com/SLA0044
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
#include "string.h"
#include "stdbool.h"
#include <stdio.h>
#include <stdlib.h>

#include "adc.h"
#include "dma.h"
#include "spi.h"
#include "tim.h"
#include "gpio.h"

#include "queue.h"

#include "crc8.h"
#include "hand_controller.h"
#include "feedback_reader.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* Команды, которые протез может принимать по протоколу */
enum ProtocolCommand {
	/* Телеметрия устройства. Протез возвращает позиции пальцев в градусах и текущий режим. */
	Telemetry,

	/* Запрос установки позиций протеза. */
	SetPositions,

	/* Сохранения миимальных и максимальных позиций приводов. */
	CalibrationSettingsSave,

	/* Получение минимальных и максимальных позиций приводов. */
	CalibrationSettingsGet,

	/* Начать автоматическу калибровку */
	CalibrationStartAuto,

	/* Начать ручную калибровку протеза. Протез начинает отправку телеметрии TelemetryCalibration. */
	CalibrationStartManual,

	/* Остановить калибровку протеза. Протез начинает отправку телеметрии Telemetry. */
	CalibrationStop,

	/* Предыдущий запрос находится в обработке. Ответ еще не сформирован.
	 * Подобное состояние возможно, в случае если пришел запрос с SPI, но программа еще
	 * не успела сформировать на него ответ. В этом случае при запросе ответа мастером
	 * будет отправлено это состояние. */
	Busy
};

/* SPI протокол. */
typedef struct {
	enum ProtocolCommand Command;
	//enum TypeWork CurrentRegime;
	uint8_t data[24];
	uint8_t CRC8;
} ProtocolStruct;

/* Телеметрия. */
typedef struct {
	//enum TypeWork CurrentRegime;
	uint8_t PointerFinger;
	uint8_t MiddleFinger;
	uint8_t RingFinger;
	uint8_t LittleFinger;
	uint8_t ThumbFinger;
	uint8_t ThumbEjector;
} TelemetryStruct;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* Основная конфигурация протеза для осуществления движений.
 * На ее основе работает вся прошивка МК. */
static HandStruct handConfig;

/* ADC PV Variables */
volatile uint16_t ADC_Data[6];

/* SPI data receive */
/* Режим передачи по SPI. Если true, то передается порция команды
 * , если false, то передаются данные.*/
static bool isCommand = true;
/* Структура для приема запроса с мастера. */
static ProtocolStruct requestSpi;
/* Структура для отправки ответа Busy */
static ProtocolStruct responseBusySpi;
/* Структура для отправки ответа.  */
static ProtocolStruct responseSpi;
/* Заполнитель SPI шины нулями при отправке ответа. */
static uint8_t zeroBuffer[sizeof(ProtocolStruct)];
/* Буфер для приема заполнителя с master устройства */
static uint8_t trashBuffer[sizeof(ProtocolStruct)];
/* Очередь для приема по SPI */
static QueueHandle_t spiReceiveQueue;

/* USER CODE END Variables */
osThreadId mainTaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

ProtocolStruct Handle_Telemetry_Command();

/**
 * @brief Выполняет инициализацию буферов SPI и начинает прием.
 * @retval None.
 */
void Init_Spi();

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
		StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize);

/* GetTimerTaskMemory prototype (linked to static allocation support) */
void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
		StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize);

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
		StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize) {
	*ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
	*ppxIdleTaskStackBuffer = &xIdleStack[0];
	*pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
	/* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/* USER CODE BEGIN GET_TIMER_TASK_MEMORY */
static StaticTask_t xTimerTaskTCBBuffer;
static StackType_t xTimerStack[configTIMER_TASK_STACK_DEPTH];

void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
		StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize) {
	*ppxTimerTaskTCBBuffer = &xTimerTaskTCBBuffer;
	*ppxTimerTaskStackBuffer = &xTimerStack[0];
	*pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
	/* place for user code */
}
/* USER CODE END GET_TIMER_TASK_MEMORY */

/**
 * @brief  FreeRTOS initialization
 * @param  None
 * @retval None
 */
void MX_FREERTOS_Init(void) {
	/* USER CODE BEGIN Init */
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*) &ADC_Data, 6);

	FeedbackReaderStruct littleFingerReader;
	FeedbackReader_Init(&littleFingerReader, (uint16_t*) &ADC_Data[5]);

	FeedbackReaderStruct ringFingerReader;
	FeedbackReader_Init(&ringFingerReader, (uint16_t*) &ADC_Data[4]);

	FeedbackReaderStruct middleFingerReader;
	FeedbackReader_Init(&middleFingerReader, (uint16_t*) &ADC_Data[3]);

	FeedbackReaderStruct indexFingerReader;
	FeedbackReader_Init(&indexFingerReader, (uint16_t*) &ADC_Data[2]);

	FeedbackReaderStruct thumbFingerReader;
	FeedbackReader_Init(&thumbFingerReader, (uint16_t*) &ADC_Data[1]);

	FeedbackReaderStruct thumbEjectorFingerReader;
	FeedbackReader_Init(&thumbEjectorFingerReader, (uint16_t*) &ADC_Data[0]);

	HandController_Init(&handConfig, littleFingerReader, ringFingerReader,
			middleFingerReader, indexFingerReader, thumbFingerReader,
			thumbEjectorFingerReader);

	//Init_Spi();
	/* USER CODE END Init */

	/* USER CODE BEGIN RTOS_MUTEX */
	/* add mutexes, ... */
	/* USER CODE END RTOS_MUTEX */

	/* USER CODE BEGIN RTOS_SEMAPHORES */
	/* add semaphores, ... */
	/* USER CODE END RTOS_SEMAPHORES */

	/* USER CODE BEGIN RTOS_TIMERS */
	/* start timers, add new ones, ... */
	/* USER CODE END RTOS_TIMERS */

	/* USER CODE BEGIN RTOS_QUEUES */
	/* add queues, ... */

	spiReceiveQueue = xQueueCreate(1, sizeof(ProtocolStruct));

	/* USER CODE END RTOS_QUEUES */

	/* Create the thread(s) */
	/* definition and creation of mainTask */
	osThreadDef(mainTask, StartDefaultTask, osPriorityNormal, 0, 256);
	mainTaskHandle = osThreadCreate(osThread(mainTask), NULL);

	/* USER CODE BEGIN RTOS_THREADS */
	/* add threads, ... */
	/* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
 * @brief  Function implementing the mainTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const *argument) {
	/* USER CODE BEGIN StartDefaultTask */

	/* Infinite loop */
	for (;;) {
		HandController_UpdateFingers(&handConfig);
		osDelay(1);
	}
	/* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

void ProtocolParser() {
	ProtocolStruct receiveData;
	xQueueReceive(spiReceiveQueue, &receiveData, portMAX_DELAY);
	switch (receiveData.Command) {
	case Telemetry: {
		ProtocolStruct responseTelemetry = Handle_Telemetry_Command();
		memcpy(&responseSpi, &responseTelemetry, sizeof(ProtocolStruct));
		//responseSpi.CurrentRegime = test;
		HAL_SPI_TransmitReceive_IT(&hspi2, (uint8_t*) &responseSpi, trashBuffer,
				sizeof(ProtocolStruct));

		break;
	}
	case SetPositions:
		break;

	case CalibrationSettingsSave:
		break;

	case CalibrationSettingsGet:
		break;

	case CalibrationStartAuto:
		break;

	case CalibrationStartManual:
		break;

	case CalibrationStop:
		break;

	default:
		break;
	}
}

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi) {
	if (hspi == &hspi2) {
		if (!hspi2.TxXferCount) {
			if (isCommand) {
				// Устанавливает ответом Busy по умолчанию.
				isCommand = false;

				ProtocolStruct copyRequest;
				memcpy(&copyRequest, &requestSpi, sizeof(ProtocolStruct));
				xQueueSendFromISR(spiReceiveQueue, &copyRequest, NULL);
			} else {
				isCommand = true;
				HAL_SPI_TransmitReceive_IT(&hspi2, zeroBuffer,
						(uint8_t*) &requestSpi, sizeof(ProtocolStruct));
			}
		}
	}
}

void Init_Spi() {
	memset(&responseBusySpi, 0, sizeof(ProtocolStruct));
	responseBusySpi.Command = Busy;
	responseBusySpi.CRC8 = calculate_crc8((unsigned char*) &responseSpi,
			sizeof(ProtocolStruct) - 1);
	HAL_SPI_TransmitReceive_IT(&hspi2, zeroBuffer, (uint8_t*) &requestSpi,
			sizeof(ProtocolStruct));
}

ProtocolStruct Handle_Telemetry_Command() {
	ProtocolStruct responseData;
	memset(&responseData, 0, sizeof(ProtocolStruct));
	responseData.Command = Telemetry;
//	responseData.CurrentRegime = handConfig.CurrentRegime;
//	responseData.data[0] = handConfig.PointerFinger.position;
//	responseData.data[1] = handConfig.MiddleFinger.position;
//	responseData.data[2] = handConfig.RingFinger.position;
//	responseData.data[3] = handConfig.LittleFinger.position;
//	responseData.data[4] = handConfig.ThumbFinger.position;
//	responseData.data[5] = handConfig.ThumbEjector.position;
//	responseData.CRC8 = calculate_crc8((unsigned char*) &responseData,
//			sizeof(ProtocolStruct) - 1);

	return responseData;
}

TelemetryStruct Get_Telemetry() {
	TelemetryStruct telemetry;
	//telemetry.CurrentRegime = handConfig.CurrentRegime;
	return telemetry;
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Hand_Error_Handler(void) {
	//handConfig.currentMode = ErrorMode;
}

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
