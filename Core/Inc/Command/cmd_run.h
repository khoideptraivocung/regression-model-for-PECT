#ifndef INC_CMD_RUN_H_
#define INC_CMD_RUN_H_

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include "cmd.h"

/* Exported macro ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
typedef enum {
  CMD_RUN_NONE = 0x00,
  CMD_RUN_START = 0x01,
  CMD_RUN_STOP = 0x02,
  CMD_RUN_MAX = 0xFF
} E_CMD_RUN_PAR;

typedef struct st_cmd_run {
  st_cmd v_base;
  E_CMD_RUN_PAR v_cmd_par;

  void (*f_destroy)(void* self);
} st_cmd_run;

/* Exported function prototypes ----------------------------------------------*/
st_cmd_run* cmd_run_create(E_CMD_RUN_PAR par, UART_HandleTypeDef* huart,
    st_pulse_generator* pg, st_sensor_reader* sr);

#endif /* INC_CMD_RUN_H_ */
