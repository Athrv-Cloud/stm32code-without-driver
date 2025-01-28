 //GB_adc.h
#ifdef ENABLE_TESTS
 #ifndef GB_ADC_H_
 #define GB_ADC_H_

 #include <stdint.h>

 #ifdef __cplusplus
 extern "C" {
 #endif

 typedef struct {
    void (*configure_gpio)(void);
    void (*configure_adc)(void);
    uint16_t (*read_conversion)(void);
 } ADC_Hardware_Interface;

 void adc_init(const ADC_Hardware_Interface* hw);
 uint16_t adc_convert_START(const ADC_Hardware_Interface* hw);

 #ifdef __cplusplus
 }
 #endif

 #endif
#else

 #ifndef GB_ADC_H_
 #define GB_ADC_H_

 #include "stm32f1xx.h"
 #include <string.h>


 void adc_init();
 uint16_t adc_convert_START();

 #endif /* GB_ADC_H_ */

#endif
