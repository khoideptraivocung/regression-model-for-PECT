/* Private includes ----------------------------------------------------------*/
#include <stddef.h>
#include <stdlib.h>
#include <util.h>

/* Private macro -------------------------------------------------------------*/

/* Private constants ---------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private function implementations ------------------------------------------*/

/* Exported function implementations ------------------------------------------*/
uint32_t util_bytes_to_uint32(const uint8_t* const bytes)
{
  if (NULL == bytes) {
    return 0;
}

  return ((uint32_t) bytes[0] << 24) | ((uint32_t) bytes[1] << 16)
      | ((uint32_t) bytes[2] << 8) | ((uint32_t) bytes[3]);
}

void util_uint32_to_bytes(uint32_t val, uint8_t* bytes)
{
  if (NULL == bytes) {
    return;
  }

  bytes[0] = (val >> 24) & 0xFF;
  bytes[1] = (val >> 16) & 0xFF;
  bytes[2] = (val >> 8) & 0xFF;
  bytes[3] = val & 0xFF;
}
