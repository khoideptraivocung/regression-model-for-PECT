/* Private includes ----------------------------------------------------------*/
#include <stddef.h>
#include <stdlib.h>
#include "resp.h"

/* Private macro -------------------------------------------------------------*/

/* Private constants ---------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
static void _add_bytes(struct st_resp* self, const uint8_t* const bytes,
    uint32_t len);

/* Private function implementations ------------------------------------------*/
static void _add_bytes(struct st_resp* self, const uint8_t* const bytes,
    uint32_t len)
{

  if ((NULL == self) || (NULL == bytes)) {
    return;
  }

  if ((self->v_next_idx + len - 1) >= self->v_len) {
    return;
  }
  
  for (uint32_t i = 0; i < len; i++) {
    self->v_bytes[self->v_next_idx + i] = bytes[i];
  }
  self->v_next_idx += len;
}

/* Exported function implementations -----------------------------------------*/
void resp_init(struct st_resp* resp)
{
  if (NULL == resp) {
    return;
  }

  resp->v_len = sizeof(resp->v_bytes) / sizeof(resp->v_bytes[0]);
  for (uint32_t i = 0; i < resp->v_len; i++) {
    resp->v_bytes[i] = RESP_TERMINATION_CHAR;
  }
  resp->v_next_idx = 0;

  resp->f_add_bytes = _add_bytes;
}
