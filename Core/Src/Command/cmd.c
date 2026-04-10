/* Private includes ----------------------------------------------------------*/
#include <stddef.h>
#include "cmd.h"

/* Private macro -------------------------------------------------------------*/

/* Private constants ---------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private function implementations ------------------------------------------*/

/* Exported function implementations -----------------------------------------*/
void cmd_init(st_cmd* cmd, E_CMD_ID id, UART_HandleTypeDef* huart,
    st_pulse_generator* pg, st_sensor_reader* sr)
{
  if ((NULL == cmd) || (NULL == huart) || (NULL == pg) || (NULL == sr)) {
    return;
  }

  cmd->v_id = id;
  cmd->v_huart = huart;
  cmd->v_pg = pg;
  cmd->v_sr = sr;

  cmd->f_exe = NULL;
}
