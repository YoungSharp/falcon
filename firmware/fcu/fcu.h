#define F_CPU 32000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <math.h>
#include <util/delay.h>
#include <string.h>

#include "avr_compiler.h"
#include "spi.h"
#include "uart.h"
#include "clk.h"
#include "crc.h"
#include "adc.h"
#include "adc_driver.h"
#include "pid.h"
#include "parity_byte.h"

//#include "/usr/lib/avr/include/avr/iox128a3.h"

/* LED bitmasks */
#define LED_1_RED_ON()      PORTA.OUTSET=PIN4_bm;
#define LED_1_RED_OFF()     PORTA.OUTCLR=PIN4_bm;
#define LED_1_GREEN_ON()    PORTF.OUTSET=PIN4_bm;
#define LED_1_GREEN_OFF()   PORTF.OUTCLR=PIN4_bm;
#define LED_2_RED_ON()      PORTA.OUTSET=PIN5_bm;
#define LED_2_RED_OFF()     PORTA.OUTCLR=PIN5_bm;
#define LED_2_GREEN_ON()    PORTF.OUTSET=PIN5_bm;
#define LED_2_GREEN_OFF()   PORTF.OUTCLR=PIN5_bm;
#define LED_3_RED_ON()      PORTA.OUTSET=PIN6_bm;
#define LED_3_RED_OFF()     PORTA.OUTCLR=PIN6_bm;
#define LED_3_GREEN_ON()    PORTF.OUTSET=PIN6_bm;
#define LED_3_GREEN_OFF()   PORTF.OUTCLR=PIN6_bm;
#define LED_4_RED_ON()      PORTA.OUTSET=PIN7_bm;
#define LED_4_RED_OFF()     PORTA.OUTCLR=PIN7_bm;
#define LED_4_GREEN_ON()    PORTF.OUTSET=PIN7_bm;
#define LED_4_GREEN_OFF()   PORTF.OUTCLR=PIN7_bm;

#define MCU_START 0xB5

#define IMU_TX_START    0xFACE
#define IMU_TX_START_H  0xFA
#define IMU_TX_START_L  0xCE

#define IMU_RX_START    0xAA
//AA

//#define ROLL_OFFSET     610
#define ROLL_OFFSET     622
//#define PITCH_OFFSET    -304
#define PITCH_OFFSET    -342

#define YAW_OFFSET      -298
//#define YAW_OFFSET      -304

#define X_OFFSET        -11571
#define Y_OFFSET        -11429
//#define Z_OFFSET        -8728
#define Z_OFFSET        -11500

/* Global Variables */

static FILE xbee_out    = FDEV_SETUP_STREAM (putchar_xbee,  NULL, _FDEV_SETUP_WRITE);
static FILE usb_out     = FDEV_SETUP_STREAM (putchar_usb,   NULL, _FDEV_SETUP_WRITE);
static FILE rs232_out   = FDEV_SETUP_STREAM (putchar_rs232, NULL, _FDEV_SETUP_WRITE);
static FILE sonar_out   = FDEV_SETUP_STREAM (putchar_sonar, NULL, _FDEV_SETUP_WRITE);

/* Data Structures */
struct mcu_tx_pkt_t
{
    volatile uint8_t start;
    volatile uint16_t tgt_1;
    volatile uint16_t tgt_2;
    volatile uint16_t tgt_3;
    volatile uint16_t tgt_4;
    volatile uint8_t crc;
};

struct mcu_rx_pkt_t
{
    //volatile uint8_t start;
    volatile uint16_t spd_1;
    volatile uint16_t spd_2;
    volatile uint16_t spd_3;
    volatile uint16_t spd_4;
    volatile uint8_t crc;
};

struct imu_tx_pkt_t
{
    volatile uint16_t start;
};

struct imu_rx_pkt_t
{
    volatile uint8_t start;
    volatile uint8_t parity;
    volatile int16_t roll;
    volatile int16_t pitch_tmp;
    volatile int16_t pitch;
    volatile int16_t yaw;
    volatile int16_t yaw_tmp;
    volatile int16_t z_accel;
    volatile int16_t x_accel;
    volatile int16_t y_accel;
};

struct fcu_pkt_t
{
    volatile uint8_t start;
    volatile uint8_t parity;
    volatile int16_t x_gyro;
    volatile int16_t x_gyro_tmp;
    volatile int16_t y_gyro;
    volatile int16_t z_gyro;
    volatile int16_t z_gyro_tmp;
    volatile int16_t z_accel;
    volatile int16_t x_accel;
    volatile int16_t y_accel;
    volatile int16_t roll;
    volatile int16_t pitch;
    volatile int16_t yaw;
    volatile int16_t rollTarget;
    volatile int16_t pitchTarget;
    volatile int16_t yawTarget;
    volatile int16_t motor1;
    volatile int16_t motor2;
    volatile int16_t motor3;
    volatile int16_t motor4;
};

/* Function Prototypes */
void init_mcu_tx_pkt(volatile struct mcu_tx_pkt_t * pkt);
void init_mcu_rx_pkt(volatile struct mcu_rx_pkt_t * pkt);
void print_mcu_pkts(volatile struct mcu_tx_pkt_t * tx_pkt, volatile struct mcu_rx_pkt_t * rx_pkt);

void init_imu_tx_pkt(volatile struct imu_tx_pkt_t * pkt);
void init_imu_rx_pkt(volatile struct imu_rx_pkt_t * pkt);
void request_imu_pkt();
void send_mcu_pkt();
void print_imu_pkts(volatile struct imu_tx_pkt_t * tx_pkt, volatile struct imu_rx_pkt_t * rx_pkt);

void process_rx_buf(volatile char * rx_buf);
