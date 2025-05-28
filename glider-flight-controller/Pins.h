#pragma once

#include "driver/gpio.h"  // ✅ Needed for gpio_num_t

// RS485
#define RS485_TX 17
#define RS485_RX 18

// CAN Bus
#define CAN_TX   GPIO_NUM_5 // Goes into the TX on the CAN Driver (not swapped around)
#define CAN_RX   GPIO_NUM_4 // Goes into the RX on the CAN Driver (not swapped around)

// IMU (I2C Bus)
#define IMU_SDA 8
#define IMU_SCL 9