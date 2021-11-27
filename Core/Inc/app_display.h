/**
 * @file  app_display.h
 * @date  25-November-2021
 * @brief Main application function.
 *
 * @author
 * @author
 */

#ifndef INC_APP_DISPLAY_H_
#define INC_APP_DISPLAY_H_

/**
 * @brief  Routine creates tasks and starts RTOS.
 * @param  void.
 * @retval void
 * @note
 */

void start_rtos(void);
void displayTask(void *arg);
void i2cTask(void *arg);

#endif /* INC_APP_DISPLAY_H_ */
