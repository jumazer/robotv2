/*
 * utilities.h
 *
 *  Created on: May 24, 2026
 *      Author: julius
 */

#ifndef UTILITIES_H_
#define UTILITIES_H_

#define MODIFY_FIELD(reg, field, value) \
   ((reg) = ((reg & ~(field ## _Msk)) | \
	           (((uint32_t) (value)  << field ## _Pos) & field ## _Msk)))


#define ESF_GPIO_MODER_INPUT     (0)
#define ESF_GPIO_MODER_OUTPUT    (1)
#define ESF_GPIO_MODER_ALT_FUNC  (2)
#define ESF_GPIO_MODER_ANALOG    (3)

#define UNUSED(X)  (void)X

#define ABS(x) (((x) < 0) ? -(x) : (x))


void to_lower(char* str);

#endif /* UTILITIES_H_ */
