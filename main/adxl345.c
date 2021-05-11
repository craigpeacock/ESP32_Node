/*
 * ADXL345 - Analog Devices 3-axis accelerometer code
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

#include <stdio.h>
#include <string.h>
#include "esp_event.h"
#include "i2c.h"
#include "adxl345.h"

void adxl345_init(uint8_t i2c_master_port)
{
	printf("Manufacturer ID:        0x%02X\r\n",i2c_read_byte(i2c_master_port, ADXL345_ALT_SLAVE_ADDRESS, ADXL345_DEVID));
	i2c_write_byte(i2c_master_port, ADXL345_ALT_SLAVE_ADDRESS, ADXL345_POWER_CTL, 0b00001000);	// Enable measure mode

	// Enable threshold interrupt on Z axis
	i2c_write_byte(i2c_master_port, ADXL345_ALT_SLAVE_ADDRESS, ADXL345_THRESH_ACT, 4);			// Set threshold
	i2c_write_byte(i2c_master_port, ADXL345_ALT_SLAVE_ADDRESS, ADXL345_ACT_INACT_CTL, ACT_AC_COUPLED | ACT_Z_EN);	// Enable activity in Z axis
	i2c_write_byte(i2c_master_port, ADXL345_ALT_SLAVE_ADDRESS, ADXL345_INT_MAP, 0b00000000);		// All functions generate INT1
	i2c_write_byte(i2c_master_port, ADXL345_ALT_SLAVE_ADDRESS, ADXL345_INT_ENABLE, ACTIVITY);		// Enable activity function
}

int16_t adxl345_read_x(uint8_t i2c_master_port)
{
	return (__bswap16(i2c_read_short(i2c_master_port, ADXL345_ALT_SLAVE_ADDRESS, ADXL345_DATAX)));
}

int16_t adxl345_read_y(uint8_t i2c_master_port)
{
	return (__bswap16(i2c_read_short(i2c_master_port, ADXL345_ALT_SLAVE_ADDRESS, ADXL345_DATAY)));
}

int16_t adxl345_read_z(uint8_t i2c_master_port)
{
	return (__bswap16(i2c_read_short(i2c_master_port, ADXL345_ALT_SLAVE_ADDRESS, ADXL345_DATAZ)));
}
