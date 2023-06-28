/*
 * @file MAX7219.h
 * @brief Arquivo com algumas macros padrao do controlador da matriz de LED, conteudo retirado do link https://github.com/AndreasBur/MaxMatrix/blob/master/MaxMatrix/sync/MaxMatrix/MaxMatrix.h
 * @author Thiago Pavao
 * @date 20/06/2023
 */

#ifndef MAX7219_H_
#define MAX7219_H_

/* Max7219 Register Address Table */
#define MAX7219_REG_NO_OP_ADDRESS                                   0x00
#define MAX7219_REG_DIGIT0_ADDRESS                                  0x01
#define MAX7219_REG_DIGIT1_ADDRESS                                  0x02
#define MAX7219_REG_DIGIT2_ADDRESS                                  0x03
#define MAX7219_REG_DIGIT3_ADDRESS                                  0x04
#define MAX7219_REG_DIGIT4_ADDRESS                                  0x05
#define MAX7219_REG_DIGIT5_ADDRESS                                  0x06
#define MAX7219_REG_DIGIT6_ADDRESS                                  0x07
#define MAX7219_REG_DIGIT7_ADDRESS                                  0x08
#define MAX7219_REG_DECODE_MODE_ADDRESS                             0x09
#define MAX7219_REG_INTENSITY_ADDRESS                               0x0A
#define MAX7219_REG_SCAN_LIMIT_ADDRESS                              0x0B
#define MAX7219_REG_SHUTDOWN_ADDRESS                                0x0C
#define MAX7219_REG_DISPLAY_TEST_ADDRESS                            0x0F

/* Content of Max7219 Decode Mode Register */
#define MAX7219_REG_DECODE_MODE_NO_DECODE                           0x00
#define MAX7219_REG_DECODE_MODE_CODE_B_DECODE_DIGIT_0               0x01
#define MAX7219_REG_DECODE_MODE_CODE_B_DECODE_DIGIT_3_TO_0          0x0F
#define MAX7219_REG_DECODE_MODE_CODE_B_DECODE_DIGIT_7_TO_0          0xFF

/* Content of Max7219 Intensity Register */
#define MAX7219_REG_INTENSITY_MIN_VALUE                             0x00
#define MAX7219_REG_INTENSITY_MAX_VALUE                             0x0F

/* Content of Max7219 Scan Limit Register */
#define MAX7219_REG_SCAN_LIMIT_DISPLAY_DIGIT_0                      0x00
#define MAX7219_REG_SCAN_LIMIT_DISPLAY_DIGIT_0_TO_1                 0x01
#define MAX7219_REG_SCAN_LIMIT_DISPLAY_DIGIT_0_TO_2                 0x02
#define MAX7219_REG_SCAN_LIMIT_DISPLAY_DIGIT_0_TO_3                 0x03
#define MAX7219_REG_SCAN_LIMIT_DISPLAY_DIGIT_0_TO_4                 0x04
#define MAX7219_REG_SCAN_LIMIT_DISPLAY_DIGIT_0_TO_5                 0x05
#define MAX7219_REG_SCAN_LIMIT_DISPLAY_DIGIT_0_TO_6                 0x06
#define MAX7219_REG_SCAN_LIMIT_DISPLAY_DIGIT_0_TO_7                 0x07

/* Content of Max7219 Shutdown Register */
#define MAX7219_REG_SHUTDOWN_MODE_SHUTDOWN_MODE                     0x00
#define MAX7219_REG_SHUTDOWN_MODE_NORMAL_OPERATION                  0x01

/* Content of Max7219 Display Test Register */
#define MAX7219_REG_DISPLAY_TEST_NORMAL_OPERATION                   0x00
#define MAX7219_REG_DISPLAY_TEST_DISPLAY_TEST_MODE                  0x01


#endif /* MAX7219_H_ */
