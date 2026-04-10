#ifndef INC_EXECUTOR_PULSE_GENERATOR_H_
#define INC_EXECUTOR_PULSE_GENERATOR_H_

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "stm32l4xx_hal.h"
#include "stm32l4xx_hal_dac.h"
#include "stm32l4xx_hal_tim.h"

/* Exported macro ------------------------------------------------------------*/
#define DAC_DATA_ALIGNMENT      (DAC_ALIGN_8B_R)

#if DAC_DATA_ALIGNMENT == DAC_ALIGN_8B_R
#define DAC_DATA_TYPE           uint8_t
#define DAC_DAX_MAX             (uint8_t)(0xFF)

#elif (DAC_DATA_ALIGNMENT == DAC_ALIGN_12B_R)
  || (DAC_DATA_ALIGNMENT == DAC_ALIGN_12B_L)
    #define DAC_DATA_TYPE       uint16_t
    #define DAC_DAX_MAX         0xFFFF

#else
  #define DAC_DATA_TYPE         uint8_t
  #define DAC_DAX_MAX           0xFF
#endif

/* Exported constants --------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
typedef enum {
  PG_PULSE_NONE = 0x00,
  PG_PULSE_SQUARE = 0x01,
  PG_PULSE_SINE = 0x02,
  PG_PULSE_MAX = 0xFF
} E_PG_PULSE_TYPE;

typedef struct {
  double v_dutyc;
  double v_val_max; // Voltage
  double v_val_min; // Voltage
} st_pg_sett_trigger;

typedef struct {
  E_PG_PULSE_TYPE v_type;
  double v_dutyc;
  double v_val_max; // Voltage
  double v_val_min; // Voltage
} st_pg_sett_pulse;

typedef struct {
  uint32_t v_freq; // Hz
  uint32_t v_samples;
  st_pg_sett_trigger v_trigger;
  st_pg_sett_pulse v_pulse;
} st_pg_sett;

typedef struct st_pulse_generator {
  DAC_HandleTypeDef* v_hdac;
  TIM_HandleTypeDef* v_htim;
  st_pg_sett v_sett;
  DAC_DATA_TYPE* v_sample_vals_trigger;
  DAC_DATA_TYPE* v_sample_vals_pulse;

  void (*f_start)(struct st_pulse_generator* self);
  void (*f_stop)(struct st_pulse_generator* self);
  void (*f_update_sett)(struct st_pulse_generator* self, st_pg_sett sett);
  void (*f_destroy)(struct st_pulse_generator* self);
} st_pulse_generator;

/* Exported function prototypes ----------------------------------------------*/
st_pulse_generator* pg_create(DAC_HandleTypeDef* hdac, TIM_HandleTypeDef* htim,
    st_pg_sett sett);

#endif /* INC_EXECUTOR_PULSE_GENERATOR_H_ */
