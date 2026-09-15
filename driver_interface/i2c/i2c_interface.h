#ifndef I2C_INTERFACE_H
#define I2C_INTERFACE_H

#include "clock_config.h"
#include "board_pins.h"
#include "DSP2833x_Device.h"

/*
 * F28335 I2C-A master for the CAT24C256WI-GT3 on drive.md.
 * Pin mux is in gpio_interface. Protocol follows C2000Ware
 * Example_2833xI2C_eeprom.c (FIFO preload, SCD/ARDY ISR).
 *
 * Schematic:
 *   EEPROM_SDA  GPIO32/SDAA  mux 1  pin 74
 *   EEPROM_SCL  GPIO33/SCLA  mux 1  pin 75
 *   EEPROM_A1   GPIO22       GPIO   pin 66, driven low
 *   EEPROM_WP   GPIO23       GPIO   pin 67, driven low (writes enabled)
 *
 * 7-bit slave 0x50 = 1010 A2 A1 A0 with A1=0, assuming A0 and A2 are GND
 * on the CAT24C256. 16-bit memory address, high byte first.
 *
 * Module clock = SYSCLKOUT / (I2C_IPSC + 1) = 10 MHz (7..12 MHz required).
 * SCL = 10 MHz / ((ICCL+5) + (ICCH+5)) = 400 kHz.
 *
 * TX FIFO is 16 bytes. Each EEPROM access uses 2 address bytes, so
 * payload is 1..14 bytes and must not cross a 64-byte CAT24C256 page.
 *
 * i2c_tx() queues a transaction. I2C_TX writes data[]. I2C_RX reads
 * n bytes; the result is popped later with i2c_rx().
 * i2c_write() is the idle-loop pump (same role as sci_write).
 */

typedef enum
{
    I2C_A = 0
} i2c_module_t;

#define I2C_COUNT                   1U

#define I2C_A_PIN_SDA               PIN_EEPROM_SDA
#define I2C_A_PIN_SCL               PIN_EEPROM_SCL
#define I2C_A_PIN_A1                PIN_EEPROM_A1
#define I2C_A_PIN_WP                PIN_EEPROM_WP
#define I2C_A_PIN_MUX               1U

#define I2C_EEPROM_ADDR             0x50U
#define I2C_MAX_BYTES               14U
#define I2C_QUEUE_LEN               8U

#define I2C_MODCLK_MHZ              10U
#define I2C_IPSC                    ((SYSCLK_MHZ / I2C_MODCLK_MHZ) - 1U)
#define I2C_ICCL                    10U
#define I2C_ICCH                    5U

#if (SYSCLK_MHZ < I2C_MODCLK_MHZ)
#error "SYSCLK_MHZ too low for I2C_MODCLK_MHZ."
#endif
#if (I2C_IPSC > 255U)
#error "I2C_IPSC does not fit I2CPSC."
#endif

typedef enum
{
    I2C_TX = 0,
    I2C_RX = 1
} i2c_dir_t;

typedef struct
{
    i2c_dir_t dir;
    Uint16 slave;                   /* 7-bit */
    Uint16 mem_addr;                /* 16-bit CAT24C256 word address */
    Uint16 n;                       /* 1 .. I2C_MAX_BYTES */
    Uint16 data[I2C_MAX_BYTES];
} i2c_msg_t;

void i2c_init(void);
Uint16 i2c_tx(i2c_module_t mod, const i2c_msg_t *msg);
Uint16 i2c_rx(i2c_module_t mod, i2c_msg_t *msg);
Uint16 i2c_idle(void);

/*
 * Idle-loop pump: one step per call.
 * Starts the next queued EEPROM write/read. SCD/ARDY ISR finishes it.
 * EEPROM NACKs during its ~5 ms write cycle; the pump retries.
 */
void i2c_write(i2c_module_t mod);

#endif /* I2C_INTERFACE_H */
