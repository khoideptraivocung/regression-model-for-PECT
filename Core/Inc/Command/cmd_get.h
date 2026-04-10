#ifndef INC_CMD_GET_H_
#define INC_CMD_GET_H_

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "cmd.h"

/* Exported macro ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
typedef struct st_cmd_get {
  st_cmd v_base;

  void (*f_destroy)(void* self);
} st_cmd_get;

/* Exported function prototypes ----------------------------------------------*/
st_cmd_get* cmd_get_create(UART_HandleTypeDef* huart, st_pulse_generator* pg,
    st_sensor_reader* sr);

#endif /* INC_CMD_GET_H_ */
