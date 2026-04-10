		/* Private includes ----------------------------------------------------------*/
#include <stddef.h>
#include <stdlib.h>
#include <math.h>
#include "pulse_generator.h"

/* Private macro -------------------------------------------------------------*/
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

#define DAC_CHANNEL_TRIGGER     DAC_CHANNEL_1
#define DAC_CHANNEL_PULSE       DAC_CHANNEL_2

/* Private constants ---------------------------------------------------------*/
static const double DAC_VOL_REF = 3.3f;
static const uint32_t TIMER_FREQ = 64e6; // Hz

/* Private typedef -----------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
static DAC_DATA_TYPE convert_dac(double vol);
static void make_pulse_square(double dutyc, double val_max, double val_min,
    DAC_DATA_TYPE* vals, uint32_t len);
static void make_pulse_sine(double val_max, double val_min, DAC_DATA_TYPE* vals,
    uint32_t len);
static void find_tim_sett(struct st_pulse_generator* self, uint32_t* prescaler,
    uint32_t* period);
static void __start(struct st_pulse_generator* self);
static void _stop(struct st_pulse_generator* self);
static void _update_sett(struct st_pulse_generator* self, st_pg_sett sett);
static void _destroy(struct st_pulse_generator* self);

/* Private function implementations ------------------------------------------*/
static DAC_DATA_TYPE convert_dac(double vol)
{
  DAC_DATA_TYPE data;

  data = (DAC_DATA_TYPE) (vol * DAC_DAX_MAX / DAC_VOL_REF);

  return data;
}

static void make_pulse_square(double dutyc, double val_max, double val_min,
    DAC_DATA_TYPE* vals, uint32_t len)
{
  if (NULL == vals) {
    return;
  }

  uint32_t count_high;
  DAC_DATA_TYPE val_max_digital, val_min_digital;

  count_high = (uint32_t) (dutyc * len);
  val_max_digital = convert_dac(val_max);
  val_min_digital = convert_dac(val_min);

  for (uint32_t i = 0; i < count_high; i++) {
    vals[i] = val_max_digital;
  }
  for (int i = count_high; i < len; i++) {
    vals[i] = val_min_digital;
  }
}

static void make_pulse_sine(double val_max, double val_min, DAC_DATA_TYPE* vals,
    uint32_t len)
{
  if (NULL == vals) {
    return;
  }

  DAC_DATA_TYPE val_max_digital, val_min_digital;
  val_max_digital = convert_dac(val_max);
  val_min_digital = convert_dac(val_min);

  for (uint32_t i = 0; i < len; i++) {
    vals[i] = (DAC_DATA_TYPE) ((val_max_digital + val_min_digital) / 2.0f
        + (val_max_digital - val_min_digital) / 2.0f * sin(i * 2 * M_PI / len));
  }
}

static void find_tim_sett(struct st_pulse_generator* self, uint32_t* prescaler,
    uint32_t* period)
{
  if ((NULL == self) || (NULL == prescaler) || (NULL == period)) {
    return;
  }

  // (Prescaler + 1) x (Period + 1) x Samples x Freq = TIMER_FREQ
  uint32_t tmp = TIMER_FREQ / self->v_sett.v_samples
      / self->v_sett.v_freq;

  for (uint32_t i = MIN(10, tmp); i >= 1; i--) {
    if (0 == tmp % i) {
      *prescaler = i - 1;
      *period = tmp / i - 1;
    }
  }
}

static void __start(struct st_pulse_generator* self)
{
  if (NULL == self) {
    return;
  }

  HAL_DAC_SetValue(self->v_hdac, DAC_CHANNEL_TRIGGER, DAC_DATA_ALIGNMENT, 0x00);

  HAL_DAC_Start_DMA(self->v_hdac, DAC_CHANNEL_TRIGGER,
      (uint32_t*) self->v_sample_vals_trigger, self->v_sett.v_samples,
      DAC_DATA_ALIGNMENT);
  HAL_DAC_Start_DMA(self->v_hdac, DAC_CHANNEL_PULSE,
      (uint32_t*) self->v_sample_vals_pulse, self->v_sett.v_samples,
      DAC_DATA_ALIGNMENT);

  /* When the timer works, the DAC needs to be running */
  HAL_TIM_Base_Start(self->v_htim);
}

static void _stop(struct st_pulse_generator* self)
{
  if (NULL == self) {
    return;
  }

  HAL_DAC_Stop_DMA(self->v_hdac, DAC_CHANNEL_TRIGGER);
  HAL_DAC_Stop_DMA(self->v_hdac, DAC_CHANNEL_PULSE);
  HAL_TIM_Base_Stop(self->v_htim);
}

static void _update_sett(struct st_pulse_generator* self, st_pg_sett sett)
{
  if (NULL == self) {
    return;
  }

  self->v_sett = sett;

  // Make new trigger and pulse
  free(self->v_sample_vals_pulse);
  free(self->v_sample_vals_trigger);
  self->v_sample_vals_trigger = (DAC_DATA_TYPE*) malloc(self->v_sett.v_samples);
  self->v_sample_vals_pulse = (DAC_DATA_TYPE*) malloc(self->v_sett.v_samples);

  // Make trigger
  make_pulse_square(self->v_sett.v_trigger.v_dutyc,
      self->v_sett.v_trigger.v_val_max, self->v_sett.v_trigger.v_val_min,
      self->v_sample_vals_trigger, self->v_sett.v_samples);

  // Make pulse
  switch (self->v_sett.v_pulse.v_type) {
  case PG_PULSE_SQUARE:
    make_pulse_square(self->v_sett.v_pulse.v_dutyc,
        self->v_sett.v_pulse.v_val_max, self->v_sett.v_pulse.v_val_min,
        self->v_sample_vals_pulse, self->v_sett.v_samples);
    break;
  case PG_PULSE_SINE:
    make_pulse_sine(self->v_sett.v_pulse.v_val_max,
        self->v_sett.v_pulse.v_val_min, self->v_sample_vals_pulse,
        self->v_sett.v_samples);
    break;
  default:
    // Fall through
    break;
  }

  // Update setting for Timer
  uint32_t prescaler = 0;
  uint32_t period = 0;
  find_tim_sett(self, &prescaler, &period);
  self->v_htim->Init.Prescaler = prescaler;
  self->v_htim->Init.Period = period;

  // Re-init timer
  HAL_TIM_Base_Init(self->v_htim);
}

static void _destroy(struct st_pulse_generator* self)
{
  if (NULL == self) {
    return;
  }

  free(self->v_sample_vals_pulse);
  free(self->v_sample_vals_trigger);
  free(self);
}

/* Exported function implementations ------------------------------------------*/
st_pulse_generator* pg_create(DAC_HandleTypeDef* hdac, TIM_HandleTypeDef* htim,
    st_pg_sett sett)
{
  st_pulse_generator* pg = (st_pulse_generator*) malloc(
      sizeof(st_pulse_generator));

  pg->v_hdac = hdac;
  pg->v_htim = htim;
  pg->v_sample_vals_trigger = NULL;
  pg->v_sample_vals_pulse = NULL;
  _update_sett(pg, sett);

  pg->f_start = __start;
  pg->f_stop = _stop;
  pg->f_update_sett = _update_sett;
  pg->f_destroy = _destroy;

  return pg;
}
