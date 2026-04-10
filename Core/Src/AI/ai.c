/* Private includes ----------------------------------------------------------*/
#include "ai_platform.h"
#include "ai_datatypes_defines.h"
#include "network.h"
#include "network_data.h"

#include "ai.h"

/* Private macro -------------------------------------------------------------*/

/* Private constants ---------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static ai_u8 m_activations[AI_NETWORK_DATA_ACTIVATIONS_SIZE];
static ai_handle m_network = AI_HANDLE_NULL;
static ai_buffer* m_ai_input;
static ai_buffer* m_ai_output;
static float m_output[AI_NETWORK_OUT_1_SIZE];

/* Private function prototypes -----------------------------------------------*/

/* Private function implementations ------------------------------------------*/

/* Exported function implementations -----------------------------------------*/
int8_t ai_init(void)
{
  ai_error err;

  /* Create and initialize an instance of the model */
  ai_handle act_addr[] = { m_activations };
  err = ai_network_create_and_init(&m_network, act_addr, NULL);
  if (err.type != AI_ERROR_NONE) {
    return -1;
  }

  m_ai_input = ai_network_inputs_get(m_network, NULL);
  m_ai_output = ai_network_outputs_get(m_network, NULL);

  return 0;
}

int8_t ai_run(const float* input, uint32_t len)
{
  if (len < AI_NETWORK_IN_1_SIZE) {
    return -1;
  }

  ai_i32 batch;

  m_ai_input[0].data = AI_HANDLE_PTR(input);
  m_ai_output[0].data = AI_HANDLE_PTR(m_output);

  batch = ai_network_run(m_network, m_ai_input, m_ai_output);
  if (batch != 1) {
    return -1;
  }

  float result = m_output[0];

  int8_t output = (int8_t)(result * 10.0f);

  return output;
}
