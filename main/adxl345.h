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

#ifndef MAIN_ADXL345_H_
#define MAIN_ADXL345_H_

#define ADXL345_SLAVE_ADDRESS			0x1D
#define ADXL345_ALT_SLAVE_ADDRESS		0x53

#define ADXL345_DEVID				0x00
#define ADXL345_THRESH_TAP			0x1D
#define ADXL345_OFSX				0x1E
#define ADXL345_OFSY				0x1F
#define ADXL345_OFSZ				0x20
#define ADXL345_DUR				0x21
#define ADXL345_LATENT				0x22
#define ADXL345_WINDOW				0x23
#define ADXL345_THRESH_ACT			0x24
#define ADXL345_TIME_INACT			0x25
#define ADXL345_THRESH_INACT			0x26
#define ADXL345_ACT_INACT_CTL			0x27
#define ADXL345_THRESH_FF			0x28
#define ADXL345_TIME_FF				0x29
#define ADXL345_TAP_AXES			0x2A
#define ADXL345_ACT_TAP_STATUS			0x2B
#define ADXL345_BW_RATE				0x2C
#define ADXL345_POWER_CTL			0x2D
#define ADXL345_INT_ENABLE			0x2E
#define ADXL345_INT_MAP				0x2F
#define ADXL345_INT_SOURCE			0x30
#define ADXL345_DATAFORMAT			0x31
#define ADXL345_DATAX				0x32
#define ADXL345_DATAY				0x34
#define ADXL345_DATAZ				0x36
#define ADXL345_FIFO_CTL			0x38
#define ADXL345_FIFO_STATUS			0x39

// Bit flags for INT_ENABLE, INT_MAP and INT_SOURCE
#define DATA_READY				0x80
#define SINGLE_TAP				0x40
#define DOUBLE_TAP				0x20
#define ACTIVITY				0x10
#define INACTIVITY 				0x08
#define FREE_FALL				0x04
#define WATERMARK				0x02
#define OVERRUN					0x01

// Bit flags for ACT_INACT_CTL
#define ACT_AC_COUPLED				0x80
#define ACT_X_EN				0x40
#define ACT_Y_EN				0x20
#define ACT_Z_EN				0x10
#define INACT_AC_COUPLED			0x08
#define INACT_X_EN				0x04
#define INACT_Y_EN				0x02
#define INACT_Z_EN				0x01

void adxl345_init(uint8_t i2c_master_port);
int16_t adxl345_read_x(uint8_t i2c_master_port);
int16_t adxl345_read_y(uint8_t i2c_master_port);
int16_t adxl345_read_z(uint8_t i2c_master_port);

#endif
