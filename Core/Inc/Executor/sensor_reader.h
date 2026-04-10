#ifndef INC_EXECUTOR_SENSOR_READER_H_
#define INC_EXECUTOR_SENSOR_READER_H_

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include "stm32l4xx_hal.h"
#include "stm32l4xx_hal_adc.h"
#include "stm32l4xx_hal_uart.h"

/* Exported macro ------------------------------------------------------------*/
#define ADC_RESOLUTION_BITS   8 /* 6, 8, 10, 12 or 16 */

#if ADC_RESOLUTION_BITS == 8
#define ADC_DATA_TYPE uint8_t
#define ADC_CONVERSION_TIME (8.5f)
#else
/* TBD: will be implemented later */
#endif

/* Exported constants --------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
typedef struct {
  uint32_t v_pulse_freq;
  uint32_t v_adc_clk_prescl;
  uint32_t v_adc_sam_time_x10;
  uint32_t v_samples;
} st_sr_sett;

typedef struct st_sensor_reader {
  ADC_HandleTypeDef* v_hadc;
  UART_HandleTypeDef* v_huart;
  st_sr_sett v_sett;
  uint32_t v_buff_len;
  ADC_DATA_TYPE** v_buff;
  int32_t v_ring_idx_reading;
  int32_t v_ring_idx_writing;
  bool v_is_stop;
  bool v_is_adc_conv_completed;

  void (*f_start)(struct st_sensor_reader* self);
  void (*f_stop)(struct st_sensor_reader* self);
  void (*f_update_sett)(struct st_sensor_reader* self, st_sr_sett sett);
  void (*f_GPIO_EXTI_Callback)(struct st_sensor_reader* self);
  void (*f_ADC_ConvCpltCallback)(struct st_sensor_reader* self);
  void (*f_HAL_UART_TxCpltCallback)(struct st_sensor_reader* self);
  void (*f_destroy)(struct st_sensor_reader* self);
} st_sensor_reader;

/* Exported function prototypes ----------------------------------------------*/
st_sensor_reader* sr_create(ADC_HandleTypeDef* hadc, UART_HandleTypeDef* huart,
    st_sr_sett sett);

#endif /* INC_EXECUTOR_SENSOR_READER_H_ */
