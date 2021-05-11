
#ifndef MAIN_I2C_H_
#define MAIN_I2C_H_

#define I2C_CONTROLLER_0		0
#define I2C_CONTROLLER_1		1

#define I2C_MASTER_FREQ_HZ		400000

#define I2C_MASTER_RX_BUF_DISABLE	0
#define I2C_MASTER_TX_BUF_DISABLE	0

#define ACK_CHECK_EN			0x1
#define ACK_CHECK_DIS			0x0

#define ACK_VAL				0x0
#define NACK_VAL			0x1

void i2c_init(uint8_t i2c_master_port, uint8_t sda_io_num, uint8_t scl_io_num);

esp_err_t i2c_write_byte(uint8_t i2c_master_port, uint8_t address, uint8_t command, uint8_t data);
esp_err_t i2c_write_short(uint8_t i2c_master_port, uint8_t address, uint8_t command, uint16_t data);
esp_err_t i2c_write_buf(uint8_t i2c_master_port, uint8_t address, uint8_t command, uint8_t *data, uint8_t len);

uint8_t   i2c_read_byte(uint8_t i2c_master_port, uint8_t address, uint8_t command);
uint16_t  i2c_read_short(uint8_t i2c_master_port, uint8_t address, uint8_t command);
esp_err_t i2c_read_buf(uint8_t i2c_master_port, uint8_t address, uint8_t command, uint8_t *buffer, uint8_t len);

#endif

