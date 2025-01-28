#ifdef ENABLE_TESTS
 #ifndef GB_UART_H
 #define GB_UART_H


 #include <stdbool.h>
 #include <stdint.h>

typedef struct {
    struct UART_Hardware_Interface* hw;
    uint32_t baud_rate;
} UART_Config;

typedef struct UART_Hardware_Interface {
    void (*enable_uart_clock)();
    void (*configure_gpio)();
    void (*set_baud_rate)();
    bool (*is_transmit_empty)();
    bool (*is_receive_not_empty)();
    void (*write_data)(uint32_t data);
    uint16_t (*read_data)();
} UART_Hardware_Interface;

bool GB_uart_init(UART_Config* config);
bool GB_UART_TxChar(UART_Config* config, uint32_t byte);
uint16_t GB_UART_RxChar(UART_Config* config);
bool GB_printString(UART_Config* config, const char *myString);
bool GB_decimel(UART_Config* config, uint32_t val);
bool GB_float_value(UART_Config* config, float gb_value);

#endif // GB_UART_H

#else
 #ifndef __GB_UART_H_
 #define __GB_UART_H_
 #include "stm32f1xx.h"
 #include <string.h>

 /********************STM32 -UART1 ********************/
 void GB_uart_pin_conf1();
 void GB_uart_boud_rate1();
 void GB_uart_init1();
 void GB_UART_TxChar1(uint32_t byte);
 uint16_t GB_UART_RxChar1();
 void GB_printString1(const char *myString);      /////to print any string
 void GB_bit1(uint32_t val);
 void GB_decimel1(uint32_t val);
 void GB_uart_newline1();
 void GB_uart_rec1(char *buff);
 void GB_float_value1(float gb_value);

 //************************STM32- UART2 functions*******************//
 //PA3- RX, PA2-TX
 void GB_uart_pin_conf2();
 void GB_uart_boud_rate2();
 void GB_uart_init2();
 void GB_UART_TxChar2(uint32_t byte);
 uint16_t GB_UART_RxChar2();
 void GB_printString2(const char *myString);      /////to print any string
 void GB_bit2(uint32_t val);
 void GB_decimel2(uint32_t val);
 void GB_uart_newline2();
 void GB_uart_rec2(char *buff);
 void GB_float_value2(float gb_value);


 #endif /* UART_H_ */
#endif