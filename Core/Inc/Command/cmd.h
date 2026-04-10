#ifndef INC_CMD_H_
#define INC_CMD_H_

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "stm32l4xx_hal.h"
#include "stm32l4xx_hal_uart.h"
#include "resp.h"
#include "pulse_generator.h"
#include "sensor_reader.h"

/* Exported macro ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
typedef enum {
  CMD_ID_NONE = 0x00,
  CMD_ID_RUN = 0x01,
  CMD_ID_GET = 0x02,
  CMD_ID_SET = 0x03,
  CMD_ID_MAX = 0xFF
} E_CMD_ID;

typedef struct st_cmd {
  E_CMD_ID v_id;
  /* Executors ---------------> */
  UART_HandleTypeDef* v_huart;
  st_pulse_generator* v_pg;
  st_sensor_reader* v_sr;
  /* Executors <--------------- */

  void (*f_exe)(void* self);
  void (*f_destroy)(void* self);
} st_cmd;

/* Exported function prototypes ----------------------------------------------*/
void cmd_init(st_cmd* cmd, E_CMD_ID id, UART_HandleTypeDef* huart,
    st_pulse_generator* pg, st_sensor_reader* sr);

#endif /* INC_CMD_H_ */
