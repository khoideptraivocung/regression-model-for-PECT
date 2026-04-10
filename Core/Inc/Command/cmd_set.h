#ifndef INC_COMMAND_CMD_SET_H_
#define INC_COMMAND_CMD_SET_H_

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "cmd.h"

/* Exported macro ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
typedef struct st_cmd_set_par {
  E_PG_PULSE_TYPE v_pulse_type;
  uint32_t v_pulse_freq;
  double v_pulse_dutyc;
  double v_pulse_val_max;
  double v_pulse_val_min;
  uint32_t v_pulse_samples;
  uint32_t v_sensor_adc_clk_prescl;
  uint32_t v_sensor_adc_sam_time_x10;
  uint32_t v_sensor_samples;
} st_cmd_set_par;

typedef struct st_cmd_set {
  st_cmd v_base;
  st_cmd_set_par v_par;

  void (*f_destroy)(void* self);
} st_cmd_set;

/* Exported function prototypes ----------------------------------------------*/
st_cmd_set* cmd_set_create(const uint8_t* const bytes, uint32_t len,
    UART_HandleTypeDef* huart, st_pulse_generator* pg, st_sensor_reader* sr);

#endif /* INC_COMMAND_CMD_SET_H_ */
