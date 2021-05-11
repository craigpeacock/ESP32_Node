/*
 * ESP32 Node example code
 * Copyright (C) 2021 Craig Peacock
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

#include "freertos/FreeRTOS.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "driver/gpio.h"
#include "wifi.h"
#include "mqtt_client.h"
#include "mqtt.h"
#include "i2c.h"
#include "bme280.h"
#include "ina226.h"
#include "ina228.h"
#include "adxl345.h"

void app_main(void)
{
	char buffer[80];

	printf("BMP280/BME280 Pressure/Humidity/Temperature Sensor Example\r\n");

	wifi_station_init();
	esp_mqtt_client_handle_t client = mqtt_init();

	i2c_init(I2C_CONTROLLER_0, 25, 26);
	bme280_init(I2C_CONTROLLER_0);

	i2c_init(I2C_CONTROLLER_1, 22, 23);
	ina228_init(I2C_CONTROLLER_1);
	//ina226_init(I2C_CONTROLLER_1);
	//adxl345_init(I2C_CONTROLLER_1);

	while (true) {

		//printf("Voltage         = %.03f V", ina228_voltage(I2C_CONTROLLER_1));
		//printf("Die Temperature = %.01f degC", ina228_dietemp(I2C_CONTROLLER_1));
		//printf("Shunt Voltage   = %.03f mV\r\n", ina228_shuntvoltage(I2C_CONTROLLER_1));
		//printf("Current         = %.03f A\r\n", ina228_current(I2C_CONTROLLER_1));
		//printf("Power           = %.03f W\r\n", ina228_power(I2C_CONTROLLER_1));
		//printf("Energy          = %.05f Wh\r\n", ina228_energy(I2C_CONTROLLER_1) / 3600);
		//printf("Charge          = %.05f Ah\r\n", ina228_charge(I2C_CONTROLLER_1) / 3600);
		//printf("\r\n");

		//printf("Temperature     = %.02f degC\r\n", bme280_temp(I2C_CONTROLLER_0));
		//printf("Pressure        = %.02f hPa\r\n",bme280_pressure(I2C_CONTROLLER_0));

		sprintf(buffer, "{\"Temperature\":%.02f,\"Pressure\":%.02f}", bme280_temp(I2C_CONTROLLER_0), bme280_pressure(I2C_CONTROLLER_0));
		mqtt_pub(client, "home/temp", buffer);

		//printf("Voltage         = %.02f V, ", ina226_voltage(I2C_CONTROLLER_1));
		//printf("Current         = %.02f A, ", ina226_current(I2C_CONTROLLER_1));
		//printf("Power           = %.02f W\r\n", ina226_power(I2C_CONTROLLER_1));

		//printf("X Axis          = %3d, ", adxl345_read_x(I2C_CONTROLLER_1));
		//printf("Y Axis          = %3d, ", adxl345_read_y(I2C_CONTROLLER_1));
		//printf("Z Axis          = %3d\r\n", adxl345_read_z(I2C_CONTROLLER_1));

		vTaskDelay(30000);
	}
}
