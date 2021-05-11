/*
 * INA228 - TI Current/Voltage/Power Monitor Code
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

#ifndef MAIN_INA228_H_
#define MAIN_INA228_H_

#define INA228_SLAVE_ADDRESS		0x40

#define INA228_CONFIG			0x00
#define INA228_ADC_CONFIG		0x01
#define INA228_SHUNT_CAL		0x02
#define INA228_SHUNT_TEMPCO		0x03
#define INA228_VSHUNT			0x04
#define INA228_VBUS			0x05
#define INA228_DIETEMP			0x06
#define INA228_CURRENT			0x07
#define INA228_POWER			0x08
#define INA228_ENERGY			0x09
#define INA228_CHARGE			0x0A
#define INA228_DIAG_ALRT		0x0B
#define INA228_SOVL			0x0C
#define INA228_SUVL			0x0D
#define INA228_BOVL			0x0E
#define INA228_BUVL			0x0F
#define INA228_TEMP_LIMIT		0x10
#define INA228_PWR_LIMIT		0x11
#define INA228_MANUFACTURER_ID	0x3E
#define INA228_DEVICE_ID		0x3F

void ina228_init(uint8_t i2c_master_port);
float ina228_voltage(uint8_t i2c_master_port);
float ina228_dietemp(uint8_t i2c_master_port);
float ina228_shuntvoltage(uint8_t i2c_master_port);
float ina228_current(uint8_t i2c_master_port);
float ina228_power(uint8_t i2c_master_port);
float ina228_energy(uint8_t i2c_master_port);
float ina228_charge(uint8_t i2c_master_port);

#endif
