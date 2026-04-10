#ifndef INC_EXECUTOR_AI_H_
#define INC_EXECUTOR_AI_H_

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* Exported macro ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/* Exported function prototypes ----------------------------------------------*/
int8_t ai_init(void);
int8_t ai_run(const float* input, uint32_t len);

#endif /* INC_EXECUTOR_AI_H_ */
