

#ifndef MAIN_BME280_H_
#define MAIN_BME280_H_

#define BME280_SLAVE_ADDRESS    0x76
#define BME280_RESET_REG        0xE0
#define BME280_CHIP_ID_REG      0xD0
#define BME280_HUM              0xFD
#define BME280_TEMP             0xFA
#define BME280_PRESSURE         0xF7

#define BME280_CONFIG           0xF5
#define BME280_CTRL_MEAS        0xF4
#define BME280_STATUS           0xF3
#define BME280_CTRL_HUM         0xF2

#define BME_DIG_CAL		0x88

struct CAL {
	uint16_t 	dig_T1;
	int16_t		dig_T2;
	int16_t 	dig_T3;
	uint16_t 	dig_P1;
	int16_t 	dig_P2;
	int16_t 	dig_P3;
	int16_t 	dig_P4;
	int16_t 	dig_P5;
	int16_t 	dig_P6;
	int16_t 	dig_P7;
	int16_t 	dig_P8;
	int16_t 	dig_P9;
};

#define BMP280_S32_t int32_t
#define BMP280_U32_t uint32_t
#define BMP280_S64_t int64_t

void bme280_init(uint8_t i2c_master_port);
float bme280_temp(uint8_t i2c_master_port);
float bme280_pressure(uint8_t i2c_master_port);

#endif
