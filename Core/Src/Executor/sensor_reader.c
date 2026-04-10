/* Private includes ----------------------------------------------------------*/
#include <stddef.h>
#include <stdlib.h>
#include <math.h>
#include "network.h"
#include "resp.h"
#include "ai.h"
#include "sensor_reader.h"
#include "main.h"

/* Private macro -------------------------------------------------------------*/
#//define TEST_AI_MODEL

/* Private typedef -----------------------------------------------------------*/
typedef struct st_adc_clock_prescaler {
  uint32_t v_val_cal;
  uint32_t v_val_def;
} st_adc_clock_prescaler;

typedef struct st_adc_sampling_time {
  uint32_t v_val_cal_x10;
  uint32_t v_val_def;
} st_adc_sampling_time;

/* Private constants ---------------------------------------------------------*/
#ifdef TEST_AI_MODEL
static float INPUT_MATERIAL_0_0[AI_NETWORK_IN_1_SIZE] = {
		24,26,32,41,48,53,60,67,73,76,79,84,88,89,90,93,96,97,96,98,100,101,100,100,103,103,102,102,103,105,103,102,104,105,104,103,104,106,105,104,104,106,106,104,104,106,107,105,104,106,107,106,104,106,107,106,105,105,107,107,105,105,107,107,105,105,106,107,106,105,106,107,106,105,106,107,107,105,105,107,107,106,105,107,108,106,105,106,108,106,105,106,108,107,105,106,107,107,105,105,107,107,106,105,107,108,106,105,106,108,107,105,106,108,107,105,106,107,107,106,105,107,108,106,105,107,108,106,105,106,108,107,105,106,108,107,106,106,107,108,106,105,107,108,106,105,106,108,107,105,106,108,107,105,106,107,108,106,106,107,108,106,105,107,108,107,105,106,108,106,101,95,88,81,74,68,62,57,53,49,45,43,40,38,36,35,33,32,31,30,30,29,29,28,28,27,27,27,26,26,26,26,26,26,25,25,25,25,25,25,25,25,25,25,25,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,23,24,23,24,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23
};
static float INPUT_MATERIAL_0_1[AI_NETWORK_IN_1_SIZE] = {
		26,28,34,42,49,54,60,67,73,76,80,85,89,91,92,95,98,99,99,100,103,103,103,103,105,106,105,105,107,108,107,106,107,109,108,107,108,109,109,107,108,110,110,108,108,110,110,109,108,109,111,109,108,109,111,110,109,109,111,110,109,109,111,111,109,109,110,111,110,109,110,111,110,109,110,111,110,109,109,111,111,109,109,111,111,110,109,111,111,110,109,110,111,111,109,110,111,111,109,110,111,111,110,109,111,111,110,109,110,112,110,109,110,112,111,109,110,111,111,110,109,111,111,110,109,111,112,110,109,110,112,111,109,110,111,111,109,110,111,111,109,109,111,111,110,109,111,112,110,109,110,111,111,109,110,111,111,110,110,111,111,110,109,111,112,110,109,110,111,109,105,99,92,86,79,73,67,62,58,54,50,47,44,42,40,39,37,36,35,34,33,32,32,31,31,30,30,29,29,29,29,28,28,28,28,28,28,28,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,25,26,25,26,26,25,25,26,26,25,26,25,26,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25
};

static float INPUT_MATERIAL_1_0[AI_NETWORK_IN_1_SIZE] = {41,45,56,70,82,90,100,112,122,127,131,139,145,147,148,152,157,158,157,159,163,163,161,161,165,166,163,163,165,168,165,163,165,168,166,164,165,168,168,165,164,167,168,165,164,167,169,166,164,166,169,167,165,166,168,168,165,165,168,169,166,165,167,169,166,165,167,169,167,165,166,169,168,165,166,169,169,166,165,168,169,167,165,167,169,167,165,166,169,168,165,166,169,169,166,165,168,169,167,165,167,169,167,165,167,170,168,165,166,169,169,166,166,169,169,167,165,168,170,167,165,167,170,168,165,166,169,169,166,166,169,169,166,165,168,170,167,165,167,170,168,165,166,169,169,166,166,169,169,166,166,168,170,167,166,167,170,168,166,167,170,169,166,166,169,170,166,165,164,159,150,139,128,117,107,98,89,82,76,70,66,62,59,56,54,52,50,49,48,47,46,45,45,44,44,44,43,43,43,43,43,43,43,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41
};
static float INPUT_MATERIAL_2_0[AI_NETWORK_IN_1_SIZE] = {
     };
#endif // TEST_AI_MODEL

static const uint32_t ADC_RING_SIZE = 50u;
static const uint32_t BUFF_LEN_FOR_RESULT = 1u;
static const uint32_t BUFF_LEN_FOR_TERMINATION_CHAR = 1u;
static const uint32_t BUFF_LEN_WITHOUT_DATA = BUFF_LEN_FOR_RESULT
    + BUFF_LEN_FOR_TERMINATION_CHAR;

static const st_adc_clock_prescaler ADC_CLOCK_PRESCALER[] = {
    {
        1,
        ADC_CLOCK_ASYNC_DIV1 },
    {
        2,
        ADC_CLOCK_ASYNC_DIV2 },
    {
        4,
        ADC_CLOCK_ASYNC_DIV4 },
    {
        6,
        ADC_CLOCK_ASYNC_DIV6 },
    {
        8,
        ADC_CLOCK_ASYNC_DIV8 },
    {
        10,
        ADC_CLOCK_ASYNC_DIV10 },
    {
        12,
        ADC_CLOCK_ASYNC_DIV12 },
    {
        16,
        ADC_CLOCK_ASYNC_DIV16 },
    {
        32,
        ADC_CLOCK_ASYNC_DIV32 },
    {
        64,
        ADC_CLOCK_ASYNC_DIV64 },
    {
        128,
        ADC_CLOCK_ASYNC_DIV128 },
    {
        256,
        ADC_CLOCK_ASYNC_DIV256 } };

static const st_adc_sampling_time ADC_SAMPLING_TIME[] = {
    {
        25,
        ADC_SAMPLETIME_2CYCLES_5 },
    {
        65,
        ADC_SAMPLETIME_6CYCLES_5 },
    {
        125,
        ADC_SAMPLETIME_12CYCLES_5 },
    {
        245,
        ADC_SAMPLETIME_24CYCLES_5 },
    {
        475,
        ADC_SAMPLETIME_47CYCLES_5 },
    {
        925,
        ADC_SAMPLETIME_92CYCLES_5 },
    {
        2475,
        ADC_SAMPLETIME_247CYCLES_5 },
    {
        6405,
        ADC_SAMPLETIME_640CYCLES_5 } };

/* Private variables ---------------------------------------------------------*/
float m_ai_input_normalized[AI_NETWORK_IN_1_SIZE];
extern UART_HandleTypeDef huart2; // Khai báo tham chiếu

/* Private function prototypes -----------------------------------------------*/
static void nomarlize_data(const uint8_t* data, uint32_t data_len);

static void start_adc_conv_and_dma(struct st_sensor_reader* self);
static void start_dma_and_uart(struct st_sensor_reader* self);

static void __start(struct st_sensor_reader* self);
static void _stop(struct st_sensor_reader* self);
static void _update_sett(struct st_sensor_reader* self, st_sr_sett sett);
static void _GPIO_EXTI_Callback(struct st_sensor_reader* self);
static void _ADC_ConvCpltCallback(struct st_sensor_reader* self);
static void _HAL_UART_TxCpltCallback(struct st_sensor_reader* self);
static void _destroy(struct st_sensor_reader* self);

/* Private function implementations ------------------------------------------*/
static void nomarlize_data(const uint8_t* data, uint32_t data_len) {
    if (NULL == data || 0 == data_len) {
        return;
    }

    // Tính tổng bình phương các phần tử
    float sum_squares = 0.0f;
    for (uint32_t i = 0; i < data_len; i++) {
        sum_squares += data[i] * data[i];
    }

    // Tính norm L2
    float l2_norm = sqrtf(sum_squares);
    if (l2_norm == 0.0f) {
        l2_norm = 1.0f; // Tránh chia cho 0
    }

    int normalized_len = sizeof(m_ai_input_normalized) / sizeof(m_ai_input_normalized[0]);
    int min_len = data_len < normalized_len ? data_len : normalized_len;

    // Chuẩn hóa dữ liệu
    for (int i = 0; i < min_len; i++) {
        m_ai_input_normalized[i] = data[i] / l2_norm;
    }

    // Điền giá trị 0.0f vào phần còn lại
    for (int i = min_len; i < normalized_len; i++) {
        m_ai_input_normalized[i] = 0.0f;
    }
}


static void start_adc_conv_and_dma(struct st_sensor_reader* self)
{
  if (NULL == self) {
    return;
  }

  self->v_ring_idx_writing++;
  self->v_ring_idx_writing = (self->v_ring_idx_writing % ADC_RING_SIZE);

  /* ADC will not write to the last character of the buffer because
   * it will be reserved for the termination char
   */
  HAL_ADC_Start_DMA(self->v_hadc,
      (uint32_t*) self->v_buff[self->v_ring_idx_writing],
      self->v_sett.v_samples);

}

static void start_dma_and_uart(struct st_sensor_reader* self)
{
  if (NULL == self) {
    return;
  }

  self->v_ring_idx_reading = self->v_ring_idx_writing - 1;
  if (self->v_ring_idx_reading < 0) {
    self->v_ring_idx_reading = ADC_RING_SIZE - 1;
  }
  // Run AI model then update the result to the last byte
  // TODO: Data type and Range of input?
  nomarlize_data((uint8_t*) (self->v_buff[self->v_ring_idx_writing]),self->v_sett.v_samples);
#ifdef TEST_AI_MODEL
  nomarlize_data(INPUT_MATERIAL_0_0, AI_NETWORK_IN_1_SIZE);
#endif
  // check value of m_ai_input_normalized?
  //HAL_UART_Transmit(&huart2, (uint8_t*)m_ai_input_normalized,333*sizeof(m_ai_input_normalized),100);
  //HAL_UART_Transmit(&huart2, (uint8_t*)  float_array, sizeof(float_array), HAL_MAX_DELAY);
  //HAL_UART_Transmit(&huart2, self->v_buff[self->v_ring_idx_reading],
	//      self->v_buff_len,100);

//  int8_t material_plus_one = ai_run(m_ai_input_normalized,
//      sizeof(m_ai_input_normalized) / sizeof(m_ai_input_normalized[0])) + 1;
  int8_t material_plus_one = 0;
  //
  self->v_buff[self->v_ring_idx_reading][self->v_sett.v_samples] = (
      material_plus_one > 0 ? material_plus_one : 0);

  HAL_UART_Transmit_DMA(self->v_huart, self->v_buff[self->v_ring_idx_reading],
      self->v_buff_len);
}

static void __start(struct st_sensor_reader* self)
{
  if (NULL == self) {
    return;
  }

  HAL_ADCEx_Calibration_Start(self->v_hadc, ADC_SINGLE_ENDED);
  self->v_ring_idx_reading = -1;
  self->v_ring_idx_writing = -1;
  self->v_is_stop = false;
  self->v_is_adc_conv_completed = true;
}

static void _stop(struct st_sensor_reader* self)
{
  if (NULL == self) {
    return;
  }

  HAL_ADC_Stop_DMA(self->v_hadc);
  self->v_is_stop = true;
  self->v_is_adc_conv_completed = true;
  self->v_ring_idx_reading = -1;
  self->v_ring_idx_writing = -1;
}

static void _update_sett(struct st_sensor_reader* self, st_sr_sett sett)
{
  if (NULL == self) {
    return;
  }

  self->v_sett = sett;

  self->v_buff_len = self->v_sett.v_samples + BUFF_LEN_WITHOUT_DATA;

  if (NULL != self->v_buff) {
    for (uint32_t i = 0; i < ADC_RING_SIZE; i++) {
      free(self->v_buff[i]);
    }
    free(self->v_buff);
  }

  self->v_buff = (ADC_DATA_TYPE**) malloc(
      ADC_RING_SIZE * sizeof(ADC_DATA_TYPE*));
  for (uint32_t i = 0; i < ADC_RING_SIZE; i++) {
    self->v_buff[i] = (ADC_DATA_TYPE*) malloc(
        self->v_buff_len * sizeof(ADC_DATA_TYPE));
  }
  for (uint32_t i = 0; i < ADC_RING_SIZE; i++) {
    for (uint32_t j = 0; j < self->v_buff_len; j++) {
      self->v_buff[i][j] = RESP_TERMINATION_CHAR;
    }
  }

  // Update setting for ADC
  for (uint32_t i = 0;
      i < sizeof(ADC_CLOCK_PRESCALER) / sizeof(ADC_CLOCK_PRESCALER)[0]; i++) {
    if (ADC_CLOCK_PRESCALER[i].v_val_cal == self->v_sett.v_adc_clk_prescl) {
      self->v_hadc->Init.ClockPrescaler = ADC_CLOCK_PRESCALER[i].v_val_def;
    }
  }
  ADC_ChannelConfTypeDef sConfig = { 0 };
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  for (uint32_t i = 0;
      i < sizeof(ADC_SAMPLING_TIME) / sizeof(ADC_SAMPLING_TIME)[0]; i++) {
    if (ADC_SAMPLING_TIME[i].v_val_cal_x10 == self->v_sett.v_adc_sam_time_x10) {
      sConfig.SamplingTime = ADC_SAMPLING_TIME[i].v_val_def;
    }
  }
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;

  // Re-init ADC
  HAL_ADC_Init(self->v_hadc);
  HAL_ADC_ConfigChannel(self->v_hadc, &sConfig);
}

static void _GPIO_EXTI_Callback(struct st_sensor_reader* self)
{
  if (NULL == self) {
    return;
  }

  if (true == self->v_is_stop) {
    return;
  }

  if (false == self->v_is_adc_conv_completed) {
    return;
  }

  self->v_is_adc_conv_completed = false;
  start_adc_conv_and_dma(self);
}

static void _ADC_ConvCpltCallback(struct st_sensor_reader* self)
{
  if (NULL == self) {
    return;
  }

  self->v_is_adc_conv_completed = true;

  if (true == self->v_is_stop) {
    return;
  }

  if (self->v_ring_idx_reading == -1) {/* copy buff to uart for the first time*/
    start_dma_and_uart(self);
  }
}

static void _HAL_UART_TxCpltCallback(struct st_sensor_reader* self)
{
  if (NULL == self) {
    return;
  }

  if (true == self->v_is_stop) {
    return;
  }

  start_dma_and_uart(self);
}

static void _destroy(struct st_sensor_reader* self)
{
  if (NULL == self) {
    return;
  }

  for (uint32_t i = 0; i < ADC_RING_SIZE; i++) {
    free(self->v_buff[i]);
  }
  free(self->v_buff);
  free(self);
}

/* Exported function implementations -----------------------------------------*/
st_sensor_reader* sr_create(ADC_HandleTypeDef* hadc, UART_HandleTypeDef* huart,
    st_sr_sett sett)
{
  // Initialize AI
  ai_init();

  // Create object
  st_sensor_reader* sr = (st_sensor_reader*) malloc(sizeof(st_sensor_reader));

  sr->v_hadc = hadc;
  sr->v_huart = huart;
  sr->v_buff = NULL;
  sr->v_ring_idx_reading = -1;
  sr->v_ring_idx_writing = -1;
  sr->v_is_stop = true;
  sr->v_is_adc_conv_completed = true;
  _update_sett(sr, sett);

  sr->f_start = __start;
  sr->f_stop = _stop;
  sr->f_update_sett = _update_sett;
  sr->f_GPIO_EXTI_Callback = _GPIO_EXTI_Callback;
  sr->f_ADC_ConvCpltCallback = _ADC_ConvCpltCallback;
  sr->f_HAL_UART_TxCpltCallback = _HAL_UART_TxCpltCallback;
  sr->f_destroy = _destroy;

  return sr;
}
