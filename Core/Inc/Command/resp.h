#ifndef INC_RESP_H_
#define INC_RESP_H_

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* Exported macro ------------------------------------------------------------*/
#define RESP_TERMINATION_CHAR (uint8_t)(0x00)
#define RESP_BYTES_LEN (uint32_t)(48)

/* Exported constants --------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
typedef struct st_resp {
  uint8_t v_bytes[RESP_BYTES_LEN];
  uint32_t v_len;
  uint32_t v_next_idx;

  void (*f_add_bytes)(struct st_resp* self, const uint8_t* const bytes,
      uint32_t len);
} st_resp;

/* Exported functions prototypes ---------------------------------------------*/
void resp_init(struct st_resp* resp);

#endif /* INC_RESP_H_ */
