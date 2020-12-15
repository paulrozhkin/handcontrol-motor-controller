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
#include "stdint.h"

#include "adc.h"
#include "dma.h"
#include "spi.h"
#include "tim.h"
#include "gpio.h"

#include "queue.h"
#include "semphr.h"

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
	enum HandStateType CurrentRegime;
	uint8_t data[24];
	uint8_t CRC8;
} ProtocolStruct;

/* Телеметрия. */
typedef struct {
	enum HandStateType state;
	uint8_t littleFingerAnglePosition;
	uint8_t ringFingerAnglePosition;
	uint8_t middleFingerAnglePosition;
	uint8_t indexFingerAnglePosition;
	uint8_t thumbFingerAnglePosition;
	uint8_t thumbEjectorAnglePosition;
} ProtocolTelemetryStruct;
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

/* Очередь для установки новых положений протеза */
static QueueHandle_t newAnglePositionsQueue;

static SemaphoreHandle_t telemetrySemaphore;

/* Текущая телеметрия */
static ProtocolTelemetryStruct currentTelemetry;

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
osThreadId protocolTaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

ProtocolStruct Protocol_Handle_Telemetry_Command();

ProtocolTelemetryStruct Protocol_Get_Telemetry();

void ProtocolParser();

/**
 * @brief Выполняет инициализацию буферов SPI и начинает прием.
 * @retval None.
 */
void Init_Spi();

/* USER CODE END FunctionPrototypes */

void StartHandControllerTask(void const *argument);
void StartProtocolTask(void const *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
		StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize);

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

/**
 * @brief  FreeRTOS initialization
 * @param  None
 * @retval None
 */
void MX_FREERTOS_Init(void) {
	/* USER CODE BEGIN Init */
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*) &ADC_Data, 6);
	Init_Spi();

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

	/* USER CODE END Init */

	/* USER CODE BEGIN RTOS_MUTEX */
	/* add mutexes, ... */
	/* USER CODE END RTOS_MUTEX */

	/* USER CODE BEGIN RTOS_SEMAPHORES */
	/* add semaphores, ... */
	telemetrySemaphore = xSemaphoreCreateMutex();
	if (telemetrySemaphore == NULL) {
		HandController_Error(&handConfig);
	}

	xSemaphoreGive(telemetrySemaphore);
	/* USER CODE END RTOS_SEMAPHORES */

	/* USER CODE BEGIN RTOS_TIMERS */
	/* start timers, add new ones, ... */
	/* USER CODE END RTOS_TIMERS */

	/* USER CODE BEGIN RTOS_QUEUES */
	/* add queues, ... */

	spiReceiveQueue = xQueueCreate(1, sizeof(ProtocolStruct));
	newAnglePositionsQueue = xQueueCreate(10, sizeof(HandAnglePositionsStruct));

	/* USER CODE END RTOS_QUEUES */

	/* Create the thread(s) */
	/* definition and creation of mainTask */
	osThreadDef(mainTask, StartHandControllerTask, osPriorityNormal, 0, 256);
	mainTaskHandle = osThreadCreate(osThread(mainTask), NULL);

	/* definition and creation of protocolTask */
	osThreadDef(protocolTask, StartProtocolTask, osPriorityRealtime, 0, 256);
	protocolTaskHandle = osThreadCreate(osThread(protocolTask), NULL);

	/* USER CODE BEGIN RTOS_THREADS */
	/* add threads, ... */
	/* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartHandControllerTask */
/**
 * @brief  Function implementing the mainTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartHandControllerTask */
void StartHandControllerTask(void const *argument) {
	/* USER CODE BEGIN StartHandControllerTask */
	/* Infinite loop */

	// Initialise the xLastWakeTime variable with the current time.
	TickType_t xLastWakeTime = xTaskGetTickCount();

	for (;;) {
		if (uxQueueMessagesWaiting(newAnglePositionsQueue) != 0) {
			HandAnglePositionsStruct newPositions;
			if (xQueueReceive(newAnglePositionsQueue, &newPositions,
					0) == pdTRUE) {
				HandController_SetAnglePositions(&handConfig, newPositions);
			}
		}

		HandController_UpdateFingers(&handConfig);
		HandController_UpdateState(&handConfig);

		// Обновляем текущую телеметрию устройства.
		HandAnglePositionsStruct anglePositions =
				HandController_GetAnglePositions(&handConfig);

		if (xSemaphoreTake(telemetrySemaphore, pdMS_TO_TICKS(1)) == pdTRUE) {
			currentTelemetry.state = handConfig.state;
			currentTelemetry.littleFingerAnglePosition =
					anglePositions.littleFingerAnglePosition;
			currentTelemetry.ringFingerAnglePosition =
					anglePositions.ringFingerAnglePosition;
			currentTelemetry.middleFingerAnglePosition =
					anglePositions.middleFingerAnglePosition;
			currentTelemetry.indexFingerAnglePosition =
					anglePositions.indexFingerAnglePosition;
			currentTelemetry.thumbFingerAnglePosition =
					anglePositions.thumbFingerAnglePosition;
			currentTelemetry.thumbEjectorAnglePosition =
					anglePositions.thumbEjectorAnglePosition;
			xSemaphoreGive(telemetrySemaphore);
		} else {
			HandController_Error(&handConfig);
		}

		vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(1));
	}
	/* USER CODE END StartHandControllerTask */
}

/* USER CODE BEGIN Header_StartProtocolTask */
/**
 * @brief Function implementing the protocolTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartProtocolTask */
void StartProtocolTask(void const *argument) {
	/* USER CODE BEGIN StartProtocolTask */
	/* Infinite loop */
	for (;;) {
		ProtocolParser();
	}
	/* USER CODE END StartProtocolTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

void ProtocolParser() {
	ProtocolStruct receiveData;
	xQueueReceive(spiReceiveQueue, &receiveData, portMAX_DELAY);

	ProtocolStruct responseTelemetry = Protocol_Handle_Telemetry_Command();
	memcpy(&responseSpi, &responseTelemetry, sizeof(ProtocolStruct));
	//responseSpi.CurrentRegime = test;
	HAL_SPI_TransmitReceive_IT(&hspi2, (uint8_t*) &responseSpi, trashBuffer,
			sizeof(ProtocolStruct));

	switch (receiveData.Command) {
	case Telemetry: {
		// Телеметрия отправляется в любом случае.
		break;
	}
	case SetPositions: {
		HandAnglePositionsStruct newPositions;
		newPositions.littleFingerAnglePosition = receiveData.data[0];
		newPositions.ringFingerAnglePosition = receiveData.data[1];
		newPositions.middleFingerAnglePosition = receiveData.data[2];
		newPositions.indexFingerAnglePosition = receiveData.data[3];
		newPositions.thumbFingerAnglePosition = receiveData.data[4];
		newPositions.thumbEjectorAnglePosition = receiveData.data[5];

		if (xQueueSend(newAnglePositionsQueue, (void* )&newPositions,
				pdMS_TO_TICKS(3)) != pdPASS) {
			HandController_Error(&handConfig);
		}
		break;
	}
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

ProtocolStruct Protocol_Handle_Telemetry_Command() {
	ProtocolStruct responseData;
	memset(&responseData, 0, sizeof(ProtocolStruct));
	responseData.Command = Telemetry;

	ProtocolTelemetryStruct telemetry = Protocol_Get_Telemetry();
	memcpy(((uint8_t*) &responseData) + 1, (uint8_t*) &telemetry,
			sizeof(ProtocolTelemetryStruct));
	responseData.CRC8 = calculate_crc8((unsigned char*) &responseData,
			sizeof(ProtocolStruct) - 1);

	return responseData;
}

ProtocolTelemetryStruct Protocol_Get_Telemetry() {
	ProtocolTelemetryStruct telemetry;

	if (xSemaphoreTake(telemetrySemaphore, pdMS_TO_TICKS(1)) == pdTRUE) {
		memcpy(&telemetry, &currentTelemetry, sizeof(ProtocolTelemetryStruct));
		xSemaphoreGive(telemetrySemaphore);
	} else {
		HandController_Error(&handConfig);
		telemetry.state = HAND_STATE_ERROR;
	}

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
