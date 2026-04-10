/* Private includes ----------------------------------------------------------*/
#include <stddef.h>
#include <stdlib.h>
#include "cmd_set.h"
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

  st_cmd_set* cmd_set = (st_cmd_set*) self;
  st_cmd_set_par *par = &(cmd_set->v_par);
  st_pulse_generator* pg = cmd_set->v_base.v_pg;
  st_sensor_reader* sr = cmd_set->v_base.v_sr;

  if ((NULL == pg) || (NULL == sr)) {
    return;
  }

  // Copy the original settings
  st_pg_sett pg_sett = pg->v_sett;
  st_sr_sett sr_sett = sr->v_sett;

  // Update settings
  pg_sett.v_freq = par->v_pulse_freq;
  pg_sett.v_pulse.v_type = par->v_pulse_type;
  pg_sett.v_pulse.v_dutyc = par->v_pulse_dutyc;
  pg_sett.v_pulse.v_val_max = par->v_pulse_val_max;
  pg_sett.v_pulse.v_val_min = par->v_pulse_val_min;
  pg_sett.v_samples = par->v_pulse_samples;

  sr_sett.v_pulse_freq = par->v_pulse_freq;
  sr_sett.v_adc_clk_prescl = par->v_sensor_adc_clk_prescl;
  sr_sett.v_adc_sam_time_x10 = par->v_sensor_adc_sam_time_x10;
  sr_sett.v_samples = par->v_sensor_samples;

  // Update settings for executors
  pg->f_update_sett(pg, pg_sett);
  sr->f_update_sett(sr, sr_sett);
}

/* Exported function implementations ------------------------------------------*/
st_cmd_set* cmd_set_create(const uint8_t* const bytes, uint32_t len,
    UART_HandleTypeDef* huart, st_pulse_generator* pg, st_sensor_reader* sr)
{
  if ((NULL == bytes) || (NULL == pg) || (NULL == sr)) {
    return NULL;
  }

  /* Create command */
  st_cmd_set* cmd_set = (st_cmd_set*) malloc(sizeof(st_cmd_set));
  cmd_init(&cmd_set->v_base, CMD_ID_SET, huart, pg, sr);
  cmd_set->v_base.f_exe = _exe;
  cmd_set->v_base.f_destroy = _destroy;
  cmd_set->f_destroy = _destroy;

  uint32_t idx = 0;
  if (idx >= len) {
    return NULL;
  }
  cmd_set->v_par.v_pulse_type = (E_PG_PULSE_TYPE) bytes[idx];
  idx += 1;

  if (idx >= len) {
    return NULL;
  }
  cmd_set->v_par.v_pulse_freq = util_bytes_to_uint32(&bytes[idx]);
  idx += 4;

  if (idx >= len) {
    return NULL;
  }
  cmd_set->v_par.v_pulse_dutyc = (double) util_bytes_to_uint32(&bytes[idx])
      / 100;
  idx += 4;

  if (idx >= len) {
    return NULL;
  }
  cmd_set->v_par.v_pulse_val_max = (double) util_bytes_to_uint32(&bytes[idx])
      / 1000;
  idx += 4;

  if (idx >= len) {
    return NULL;
  }
  cmd_set->v_par.v_pulse_val_min = (double) util_bytes_to_uint32(&bytes[idx])
      / 1000;
  idx += 4;

  if (idx >= len) {
    return NULL;
  }
  cmd_set->v_par.v_pulse_samples = util_bytes_to_uint32(&bytes[idx]);
  idx += 4;

  if (idx >= len) {
    return NULL;
  }
  cmd_set->v_par.v_sensor_adc_clk_prescl = util_bytes_to_uint32(&bytes[idx]);
  idx += 4;

  if (idx >= len) {
    return NULL;
  }
  cmd_set->v_par.v_sensor_adc_sam_time_x10 = util_bytes_to_uint32(&bytes[idx]);
  idx += 4;

  if (idx >= len) {
    return NULL;
  }
  cmd_set->v_par.v_sensor_samples = util_bytes_to_uint32(&bytes[idx]);
  idx += 4;

  return cmd_set;
}
