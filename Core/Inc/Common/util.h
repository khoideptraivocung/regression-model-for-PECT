#ifndef INC_COMMON_UTIL_H_
#define INC_COMMON_UTIL_H_

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* Exported macro ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/* Exported function prototypes ----------------------------------------------*/
uint32_t util_bytes_to_uint32(const uint8_t* const bytes);
void util_uint32_to_bytes(uint32_t val, uint8_t* bytes);

#endif /* INC_COMMON_UTIL_H_ */
