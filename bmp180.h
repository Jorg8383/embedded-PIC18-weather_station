/* 
 * File:                bmp180.h
 * Author:              J. Striebel
 * Project:             Weather Station
 * Device:              PIC18F47Q10 @ 16 MHz internal OSC, 5V
 * Platform:            Curiosity HPC board (DM164136)
 * Compiler:            XC8 (v2.41)
 * IDE:                 MPLAB X (v6.10), MCC (5.3.7)
 * Program version:     1.0
 
 * Program Description:
 * --------------------
 * 
 * Hardware Description:
 * --------------------
 *    
 */

#ifndef BMP180_H
#define	BMP180_H

 /* I2C address definition */
#define BMP180_I2C_ADDR                     0x77

/* Hardware pressure sampling accuracy modes */
typedef enum {
    BMP180_MODE_ULTRALOWPOWER,           
    BMP180_MODE_STANDARD,               
    BMP180_MODE_HIGHRESOLUTION,          
    BMP180_MODE_ULTRAHIGHRESOLUTION    
} BMP180_PRESSURE_MODE;

/* Data registers to operate the BMP180 */
#define BMP180_REG_CHIP_ID                  0xD0 // Value is fixed to 0x55

#define BMP180_REG_SOFT_RESET               0xE0
#define BMP180_REG_CTRL_MEAS                0xF4
#define BMP180_REG_OUT_MSB                  0xF6
#define BMP180_REG_OUT_LSB                  0xF7
#define BMP180_REG_OUT_XLSB                 0xF8

/* Control register values for different oversampling settings (oss) */
#define BMP180_CTRL_VAL_TEMPERATURE         0x2E // Temperature
#define BMP180_CTRL_VAL_PRESSURE_OSS_0      0x34 // Pressure (oss = 0)
#define BMP180_CTRL_VAL_PRESSURE_OSS_1      0x74 // Pressure (oss = 1)
#define BMP180_CTRL_VAL_PRESSURE_OSS_2      0xB4 // Pressure (oss = 2)
#define BMP180_CTRL_VAL_PRESSURE_OSS_3      0xF4 // Pressure (oss = 3)

/* Note: According to the BMP180 data sheet, the max. conversion time depends
 * on the oversampling setting (oss). For oss = 0 the max. conversion time is
 * 5 ms */
#define BMP180_CONV_TIME_TEMPERATURE        5 // Temperature conversion time [ms]
#define BMP180_CONV_TIME_P_OSS_0            5 // Pressure conversion time [ms] 
#define BMP180_CONV_TIME_P_OSS_1            8 
#define BMP180_CONV_TIME_P_OSS_2            14  
#define BMP180_CONV_TIME_P_OSS_3            26  

/* Register addresses for calibration coefficients */
#define BMP180_CAL_DATA_SIZE                22 // in bytes
#define BMP180_CAL_COEFF_AC1_MSB            0xAA    
#define BMP180_CAL_COEFF_AC1_LSB            0xAB
#define BMP180_CAL_COEFF_AC2_MSB            0xAC
#define BMP180_CAL_COEFF_AC2_LSB            0xAD
#define BMP180_CAL_COEFF_AC3_MSB            0xAE
#define BMP180_CAL_COEFF_AC3_LSB            0xAF
#define BMP180_CAL_COEFF_AC4_MSB            0xB0
#define BMP180_CAL_COEFF_AC4_LSB            0xB1
#define BMP180_CAL_COEFF_AC5_MSB            0xB2
#define BMP180_CAL_COEFF_AC5_LSB            0xB3
#define BMP180_CAL_COEFF_AC6_MSB            0xB4
#define BMP180_CAL_COEFF_AC6_LSB            0xB5
#define BMP180_CAL_COEFF_B1_MSB             0xB6
#define BMP180_CAL_COEFF_B1_LSB             0xB7
#define BMP180_CAL_COEFF_B2_MSB             0xB8
#define BMP180_CAL_COEFF_B2_LSB             0xB9
#define BMP180_CAL_COEFF_MB_MSB             0xBA
#define BMP180_CAL_COEFF_MB_LSB             0xBB
#define BMP180_CAL_COEFF_MC_MSB             0xBC
#define BMP180_CAL_COEFF_MC_LSB             0xBD
#define BMP180_CAL_COEFF_MD_MSB             0xBE
#define BMP180_CAL_COEFF_MD_LSB             0xBF

/* Data structure that holds device specific calibration parameters */
typedef struct bmp180_cal_coeff_t
{
    int16_t     ac1;
    int16_t     ac2;
    int16_t     ac3;
    uint16_t    ac4;
    uint16_t    ac5;
    uint16_t    ac6;
    int16_t     b1;
    int16_t     b2;
    int16_t     mb;
    int16_t     mc;
    int16_t     md;
} BMP180_CAL_COEFF;


/* Data structure that holds the BMP180 initialisation parameters */
typedef struct bmp180_param_t
{
    BMP180_CAL_COEFF        calibParam;
    BMP180_PRESSURE_MODE    oversampling;
    uint8_t                 ossCtrlRegValue;
    uint32_t                conversionTime;
    uint8_t                 chipId;
    uint8_t                 deviceAddr;
    int32_t                 paramB5;
} BMP180_PARAM; 


/* Function prototype declarations */
void BMP180_Init(BMP180_PARAM *param);
uint16_t BMP180_ReadUncompensatedTemperature(void);
uint32_t BMP180_ReadUncompensatedPressure(void);
int16_t BMP180_CalcTemperature(uint16_t uncompensatedTemperature);
int32_t BMP180_CalcPressure(uint32_t uncompensatedPressure);


#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* BMP180_H */

