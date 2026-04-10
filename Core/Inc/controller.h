#ifndef INC_CONTROLLER_H_
#define INC_CONTROLLER_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"
#include "stm32l4xx_hal_dac.h"
#include "stm32l4xx_hal_tim.h"

/* Exported macro ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/* Exported function prototypes ----------------------------------------------*/
void con_init(ADC_HandleTypeDef *hadc, DAC_HandleTypeDef *hdac,
              TIM_HandleTypeDef *htim, UART_HandleTypeDef *huart);
void con_start(void);
void con_HAL_GPIO_EXTI_Callback(void);
void con_HAL_ADC_ConvCpltCallback(void);
void con_HAL_UART_TxCpltCallback(void);
void con_HAL_UART_RxCpltCallback(void);
void con_destroy(void);

/* Private defines -----------------------------------------------------------*/

#endif /* INC_CONTROLLER_H_ */
