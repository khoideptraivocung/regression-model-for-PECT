/* Private includes ----------------------------------------------------------*/
#include <stddef.h>
#include <stdlib.h>
#include <util.h>
#include "cmd_get.h"
#include "resp.h"
#include "util.h"

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

  st_cmd_get* cmd_get = (st_cmd_get*) self;
  st_resp resp;
  UART_HandleTypeDef* huart = cmd_get->v_base.v_huart;
  st_pg_sett pg_sett = cmd_get->v_base.v_pg->v_sett;
  st_sr_sett sr_sett = cmd_get->v_base.v_sr->v_sett;
  uint8_t bytes[4];

  resp_init(&resp);

  resp.f_add_bytes(&resp, &(cmd_get->v_base.v_id), 1);

  resp.f_add_bytes(&resp, &(pg_sett.v_pulse.v_type), 1);

  util_uint32_to_bytes(pg_sett.v_freq, bytes);
  resp.f_add_bytes(&resp, bytes, sizeof(bytes) / sizeof(bytes[0]));

  util_uint32_to_bytes((uint32_t) (pg_sett.v_pulse.v_dutyc * 100), bytes);
  resp.f_add_bytes(&resp, bytes, sizeof(bytes) / sizeof(bytes[0]));

  util_uint32_to_bytes((uint32_t) (pg_sett.v_pulse.v_val_max * 1000), bytes);
  resp.f_add_bytes(&resp, bytes, sizeof(bytes) / sizeof(bytes[0]));

  util_uint32_to_bytes((uint32_t) (pg_sett.v_pulse.v_val_min * 1000), bytes);
  resp.f_add_bytes(&resp, bytes, sizeof(bytes) / sizeof(bytes[0]));

  util_uint32_to_bytes(pg_sett.v_samples, bytes);
  resp.f_add_bytes(&resp, bytes, sizeof(bytes) / sizeof(bytes[0]));

  util_uint32_to_bytes(sr_sett.v_adc_clk_prescl, bytes);
  resp.f_add_bytes(&resp, bytes, sizeof(bytes) / sizeof(bytes[0]));

  util_uint32_to_bytes(sr_sett.v_adc_sam_time_x10, bytes);
  resp.f_add_bytes(&resp, bytes, sizeof(bytes) / sizeof(bytes[0]));

  util_uint32_to_bytes(sr_sett.v_samples, bytes);
  resp.f_add_bytes(&resp, bytes, sizeof(bytes) / sizeof(bytes[0]));

  HAL_UART_Transmit_DMA(huart, resp.v_bytes, resp.v_len);
}

/* Exported function implementations ------------------------------------------*/
st_cmd_get* cmd_get_create(UART_HandleTypeDef* huart, st_pulse_generator* pg,
    st_sensor_reader* sr)
{
  if ((NULL == pg) || (NULL == sr)) {
    return NULL;
  }

  /* Create command */
  st_cmd_get* cmd_get = (st_cmd_get*) malloc(sizeof(st_cmd_get));
  cmd_init(&cmd_get->v_base, CMD_ID_GET, huart, pg, sr);
  cmd_get->v_base.f_exe = _exe;
  cmd_get->v_base.f_destroy = _destroy;
  cmd_get->f_destroy = _destroy;

  return cmd_get;
}
