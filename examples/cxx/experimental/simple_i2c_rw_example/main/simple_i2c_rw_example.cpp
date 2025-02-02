/*
 * SPDX-FileCopyrightText: 2021 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0
 *
 * MPU9250 I2C Sensor C++ Example
 *
 * This example code is in the Public Domain (or CC0 licensed, at your option.)
 *
 * Unless required by applicable law or agreed to in writing, this
 * software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
 * CONDITIONS OF ANY KIND, either express or implied.
*/

#include <iostream>
#include "esp_log.h"
#include "i2c_cxx.hpp"

using namespace std;
using namespace idf;

static const char *TAG = "i2c-cxx-simple-example";

#define I2C_MASTER_NUM              0   /*!< I2C master i2c port number, the number of i2c peripheral interfaces
                                             available will depend on the chip */
#define I2C_MASTER_SCL_IO           CONFIG_I2C_MASTER_SCL      /*!< GPIO number used for I2C master clock */
#define I2C_MASTER_SDA_IO           CONFIG_I2C_MASTER_SDA      /*!< GPIO number used for I2C master data  */

#define MPU9250_SENSOR_ADDR                 0x68        /*!< Slave address of the MPU9250 sensor */
#define MPU9250_WHO_AM_I_REG_ADDR           0x75        /*!< Register addresses of the "who am I" register */

extern "C" void app_main(void)
{
    try {
        // creating master bus
        shared_ptr<I2CMaster> master(new I2CMaster(I2C_MASTER_NUM, I2C_MASTER_SCL_IO, I2C_MASTER_SDA_IO, 400000));
        ESP_LOGI(TAG, "I2C initialized successfully");

        // writing the pointer to the WHO_AM_I register to the device
        master->sync_write(MPU9250_SENSOR_ADDR, {MPU9250_WHO_AM_I_REG_ADDR});

        // reading back the value of WHO_AM_I register which should be 71
        vector<uint8_t> data = master->sync_read(MPU9250_SENSOR_ADDR, 2);

        ESP_LOGI(TAG, "WHO_AM_I = %X", data[0]);
    } catch (const I2CException &e) {
        cout << "I2C Exception with error: "  << e.what();
        cout << " (" << e.error<< ")" << endl;
        cout << "Couldn't read sensor!" << endl;
    }

    // The I2CMaster object is de-initialized in its destructor when going out of scope.
    ESP_LOGI(TAG, "I2C de-initialized successfully");
}
