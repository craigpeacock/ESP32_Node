#include <stdio.h>
#include <string.h>
#include "esp_event.h"
#include "i2c.h"
#include "bme280.h"

struct CAL cal;
BMP280_S32_t t_fine;

void bme280_init(uint8_t i2c_master_port)
{
	uint8_t ChipID = i2c_read_byte(i2c_master_port, BME280_SLAVE_ADDRESS, BME280_CHIP_ID_REG);
	switch (ChipID) {

		case 0x56:
		case 0x57:
		case 0x58:
			printf("Found BMP280\r\n");
			break;

		case 0x60:
			printf("Found BME280\r\n");
			break;

		default:
			printf("Unknown device, Manufacturer ID: 0x%02X\r\n", ChipID);
	}

	i2c_write_byte(i2c_master_port, BME280_SLAVE_ADDRESS, BME280_RESET_REG, 0xB6); // Reset
	sleep(1);

	i2c_write_byte(i2c_master_port, BME280_SLAVE_ADDRESS, BME280_CTRL_MEAS, 0x93); // 0b10010011); // Pressure & Temperature Oversampling x8, Normal Mode
	printf("CTRL_MEAS: 0x%02X\r\n", i2c_read_byte(i2c_master_port, BME280_SLAVE_ADDRESS, BME280_CTRL_MEAS));
	printf("STATUS:    0x%02X\r\n", i2c_read_byte(i2c_master_port, BME280_SLAVE_ADDRESS, BME280_STATUS));
	printf("CONFIG:    0x%02X\r\n", i2c_read_byte(i2c_master_port, BME280_SLAVE_ADDRESS, BME280_CONFIG));

	i2c_read_buf(i2c_master_port, BME280_SLAVE_ADDRESS, BME_DIG_CAL, (uint8_t *)&cal, sizeof(cal));
}

// Returns temperature in DegC, resolution is 0.01 DegC, Output value of "5123" equals 51.23 DegC.
// t_fine carries fine temperature as global value
BMP280_S32_t bmp280_compensate_T_int32(BMP280_S32_t adc_T) {
	BMP280_S32_t var1, var2, T;
	var1 = ((((adc_T >> 3) - ((BMP280_S32_t)cal.dig_T1 << 1))) * ((BMP280_S32_t)cal.dig_T2)) >> 11;
	var2 = (((((adc_T >> 4) - ((BMP280_S32_t)cal.dig_T1)) * ((adc_T >> 4) - ((BMP280_S32_t)cal.dig_T1))) >> 12) *
			((BMP280_S32_t)cal.dig_T3)) >> 14;
	t_fine = var1 + var2;
	T = (t_fine * 5 + 128) >> 8;
	return T;
}

float bme280_temp(uint8_t i2c_master_port)
{
	uint32_t rawtemp = 0;
	i2c_read_buf(i2c_master_port, BME280_SLAVE_ADDRESS, BME280_TEMP, (uint8_t *)&rawtemp, 3);
	rawtemp = __bswap32(rawtemp) >> 12;
	return ((float)bmp280_compensate_T_int32(rawtemp) / 100);
}

// Returns pressure in Pa as unsigned 32 bit integer in Q24.8 format (24 integer bits and 8 fractional bits).
// Output value of "24674867" represents 24674867/256 = 96386.2 Pa = 963.862 hPa
BMP280_U32_t bmp280_compensate_P_int64(BMP280_S32_t adc_P)
{
	BMP280_S64_t var1, var2, p;
	var1 = ((BMP280_S64_t)t_fine) - 128000;
	var2 = var1 * var1 * (BMP280_S64_t)cal.dig_P6;
	var2 = var2 + ((var1*(BMP280_S64_t)cal.dig_P5)<<17);
	var2 = var2 + (((BMP280_S64_t)cal.dig_P4)<<35);
	var1 = ((var1 * var1 * (BMP280_S64_t)cal.dig_P3)>>8) + ((var1 * (BMP280_S64_t)cal.dig_P2)<<12);
	var1 = (((((BMP280_S64_t)1)<<47)+var1))*((BMP280_S64_t)cal.dig_P1)>>33;
	if (var1 == 0)
	{
		return 0; // avoid exception caused by division by zero
	}
	p = 1048576-adc_P;
	p = (((p<<31)-var2)*3125)/var1;
	var1 = (((BMP280_S64_t)cal.dig_P9) * (p>>13) * (p>>13)) >> 25;
	var2 = (((BMP280_S64_t)cal.dig_P8) * p) >> 19;
	p = ((p + var1 + var2) >> 8) + (((BMP280_S64_t)cal.dig_P7)<<4);

	return (BMP280_U32_t)p;
}

float bme280_pressure(uint8_t i2c_master_port)
{
	uint32_t rawpressure = 0;
	i2c_read_buf(i2c_master_port, BME280_SLAVE_ADDRESS, BME280_PRESSURE, (unsigned char *)&rawpressure, 3);
	rawpressure = __bswap32(rawpressure) >> 12;
	return((float)bmp280_compensate_P_int64(rawpressure) / 25600);
}

