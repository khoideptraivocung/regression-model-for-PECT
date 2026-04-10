/* Private includes ----------------------------------------------------------*/
#include "stdbool.h"
#include "stm32l4xx_hal.h"
#include "cmd.h"
#include "cmd_run.h"
#include "cmd_get.h"
#include "cmd_set.h"
#include "pulse_generator.h"
#include "sensor_reader.h"

/* Private macro -------------------------------------------------------------*/
#define CMD_LEN 34

/* Private constants ---------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static ADC_HandleTypeDef* m_hadc;
static DAC_HandleTypeDef* m_hdac;
static TIM_HandleTypeDef* m_htim;
static UART_HandleTypeDef* m_huart;
static st_pulse_generator* m_pg;
static st_sensor_reader* m_sr;

static uint8_t m_cmd_bytes[CMD_LEN];

/* Private functions prototypes ----------------------------------------------*/

/* Private function implementations ------------------------------------------*/

/* Exported function implementations -----------------------------------------*/
void con_init(ADC_HandleTypeDef* hadc, DAC_HandleTypeDef* hdac,
    TIM_HandleTypeDef* htim, UART_HandleTypeDef* huart)
{
  m_hadc = hadc;
  m_hdac = hdac;
  m_htim = htim;
  m_huart = huart;
  st_pg_sett pg_sett;

  pg_sett.v_freq = 200;
  pg_sett.v_samples = 500;
  pg_sett.v_trigger.v_dutyc = 0.2;
  /*
   * Need this high values of Amplitude and Amplitude Offset for STM
   * to detect the rising edge of trigger
   * */
  pg_sett.v_trigger.v_val_max = 1.9;
  pg_sett.v_trigger.v_val_min = 0.4;

  pg_sett.v_pulse.v_type = PG_PULSE_SQUARE;
  pg_sett.v_pulse.v_dutyc = 0.5;
  pg_sett.v_pulse.v_val_max = 1.2;
  pg_sett.v_pulse.v_val_min = 0.2;

  m_pg = pg_create(hdac, htim, pg_sett);

  st_sr_sett sr_sett;
  sr_sett.v_pulse_freq = pg_sett.v_freq;
  sr_sett.v_adc_clk_prescl = 64;
  sr_sett.v_adc_sam_time_x10 = 65;
  sr_sett.v_samples = 333;
  m_sr = sr_create(hadc, huart, sr_sett);
}

void con_start(void)
{
  HAL_UART_Receive_DMA(m_huart, m_cmd_bytes,
      sizeof(m_cmd_bytes) / sizeof(m_cmd_bytes[0]));
}

void con_HAL_GPIO_EXTI_Callback(void)
{
  if (NULL == m_sr) {
    return;
  }
  m_sr->f_GPIO_EXTI_Callback(m_sr);
}

void con_HAL_ADC_ConvCpltCallback(void)
{
  if (NULL == m_sr) {
    return;
  }
  m_sr->f_ADC_ConvCpltCallback(m_sr);
}

void con_HAL_UART_TxCpltCallback(void)
{
  m_sr->f_HAL_UART_TxCpltCallback(m_sr);
}

void con_HAL_UART_RxCpltCallback(void)
{
  st_cmd* cmd = NULL;

  switch (m_cmd_bytes[0]) {
  case CMD_ID_RUN:
    cmd = (st_cmd*) cmd_run_create(m_cmd_bytes[1], m_huart, m_pg, m_sr);
    break;
  case CMD_ID_GET:
    cmd = (st_cmd*) cmd_get_create(m_huart, m_pg, m_sr);
    break;
  case CMD_ID_SET:
    cmd = (st_cmd*) cmd_set_create(&m_cmd_bytes[1], CMD_LEN - 1, m_huart, m_pg,
        m_sr);
    break;
  default:
    // Fall through
    break;
  }

  if (NULL != cmd) {
    cmd->f_exe(cmd);
    cmd->f_destroy(cmd);
  }

  HAL_UART_Receive_DMA(m_huart, m_cmd_bytes,
      sizeof(m_cmd_bytes) / sizeof(m_cmd_bytes[0]));
}

void con_destroy(void)
{
  if (NULL != m_pg) {
    m_pg->f_destroy(m_pg);
  }

  if (NULL != m_sr) {
    m_sr->f_destroy(m_sr);
  }
}
