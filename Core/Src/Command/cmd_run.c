/* Private includes ----------------------------------------------------------*/
#include <stddef.h>
#include <stdlib.h>
#include "cmd_run.h"

/* Private macro -------------------------------------------------------------*/

/* Private constants ---------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
static void _destroy(void* self);
static void _exe(void* const self);

/* Private function implementations ------------------------------------------*/
static void _destroy(void* self)
{
  if (NULL == self) {
    return;
  }

  free(self);
}

static void _exe(void* const self)
{
  if (NULL == self) {
    return;
  }

  st_cmd_run* cmd_run = (st_cmd_run*) self;
  st_pulse_generator* pg = cmd_run->v_base.v_pg;
  st_sensor_reader* sr = cmd_run->v_base.v_sr;

  if ((NULL == pg) || (NULL == sr)) {
    return;
  }

  if (CMD_RUN_STOP == cmd_run->v_cmd_par) {
    sr->f_stop(sr);
    pg->f_stop(pg);
  } else if (CMD_RUN_START == cmd_run->v_cmd_par) {
    sr->f_start(sr);
    pg->f_start(pg);
  } else {
    // Do nothing
  }

}

/* Exported function implementations ------------------------------------------*/
st_cmd_run* cmd_run_create(E_CMD_RUN_PAR par, UART_HandleTypeDef* huart,
    st_pulse_generator* pg, st_sensor_reader* sr)
{
  if ((NULL == pg) || (NULL == sr)) {
    return NULL;
  }

  /* Create command */
  st_cmd_run* cmd_run = (st_cmd_run*) malloc(sizeof(st_cmd_run));
  cmd_init(&cmd_run->v_base, CMD_ID_RUN, huart, pg, sr);
  cmd_run->v_base.f_exe = _exe;
  cmd_run->v_base.f_destroy = _destroy;
  cmd_run->v_cmd_par = par;
  cmd_run->f_destroy = _destroy;

  return cmd_run;
}
