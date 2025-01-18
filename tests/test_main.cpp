#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include <CppUTest/CommandLineTestRunner.h>

#include "../Inc/gb_rcc.h"       
#include "../Inc/gb_t_delay.h"   
#include "../Inc/GB_adc.h"       
#include "../Inc/GB_UART.h"
#include "../Inc/gb_gpio.h"

// Mock implementations
void system_clk() { 
    mock().actualCall("system_clk"); 
}

void timer_initialise() { 
    mock().actualCall("timer_initialise"); 
}

void adc_init() { 
    mock().actualCall("adc_init"); 
}

uint16_t adc_convert_START() { 
    return mock().actualCall("adc_convert_START").returnUnsignedIntValue(); 
}

void GB_uart_init2() { 
    mock().actualCall("GB_uart_init2"); 
}

void GB_printString2(const char* string) { 
    if (string != nullptr) {
        mock().actualCall("GB_printString2").withParameter("string", string); 
    }
}

void GB_printChar2(char c) {
    mock().actualCall("GB_printChar2").withParameter("char", c);
}

void delay_ms(uint16_t ms) { 
    mock().actualCall("delay_ms").withParameter("time", ms); 
}

void GB_decimel2(uint16_t value) { 
    mock().actualCall("GB_decimel2").withParameter("value", value); 
}

void gpio_init() {
    mock().actualCall("gpio_init");
}

// Basic MainTests Group
TEST_GROUP(MainTests)
{
    void setup() { 
        mock().clear(); 
    }
    
    void teardown() { 
        mock().checkExpectations();
        mock().clear(); 
    }
};

// ADC Tests Group
TEST_GROUP(ADCTests)
{
    void setup() {
        mock().clear();
    }
    void teardown() {
        mock().checkExpectations();
        mock().clear();
    }
};

// UART Tests Group
TEST_GROUP(UARTTests)
{
    void setup() {
        mock().clear();
    }
    void teardown() {
        mock().checkExpectations();
        mock().clear();
    }
};

// GPIO Tests Group
TEST_GROUP(GPIOTests)
{
    void setup() {
        mock().clear();
    }
    void teardown() {
        mock().checkExpectations();
        mock().clear();
    }
};

// RCC Tests Group
TEST_GROUP(RCCTests)
{
    void setup() {
        mock().clear();
    }
    void teardown() {
        mock().checkExpectations();
        mock().clear();
    }
};

// Delay Tests Group
TEST_GROUP(DelayTests)
{
    void setup() {
        mock().clear();
    }
    void teardown() {
        mock().checkExpectations();
        mock().clear();
    }
};

// Syscalls Tests Group
TEST_GROUP(SyscallTests)
{
    void setup() {
        mock().clear();
    }
    void teardown() {
        mock().checkExpectations();
        mock().clear();
    }
};

// MainTests Group Tests
TEST(MainTests, SystemClockInitialization)
{
    mock().expectOneCall("system_clk");
    system_clk();
}

TEST(MainTests, TimerInitialization)
{
    mock().expectOneCall("timer_initialise");
    timer_initialise();
}

TEST(MainTests, FullInitializationSequence)
{
    mock().expectOneCall("system_clk");
    mock().expectOneCall("timer_initialise");
    mock().expectOneCall("adc_init");
    mock().expectOneCall("GB_uart_init2");
    mock().expectOneCall("gpio_init");
    
    system_clk();
    timer_initialise();
    adc_init();
    GB_uart_init2();
    gpio_init();
}

// ADC Tests
TEST(ADCTests, ADCInitialization)
{
    mock().expectOneCall("RCC_APB2ENR").andReturnValue(1);
    mock().expectOneCall("ADC_CR2").andReturnValue(1);
    adc_init();
}

TEST(ADCTests, ADCConversion)
{
    mock().expectOneCall("ADC_SQR3").andReturnValue(0);
    mock().expectOneCall("ADC_CR2_START").andReturnValue(1);
    mock().expectOneCall("ADC_SR_EOC").andReturnValue(1);
    uint16_t result = adc_convert_START();
    CHECK(result >= 0);
}

TEST(ADCTests, ADCValueRange)
{
    mock().expectOneCall("adc_convert_START").andReturnValue(0);
    uint16_t min_value = adc_convert_START();
    CHECK(min_value >= 0);
    
    mock().expectOneCall("adc_convert_START").andReturnValue(4095);
    uint16_t max_value = adc_convert_START();
    CHECK(max_value <= 4095);
}

TEST(ADCTests, ADCErrorHandling)
{
    mock().expectOneCall("adc_convert_START").andReturnValue(0xFFFF);
    uint16_t error_value = adc_convert_START();
    CHECK_EQUAL(0xFFFF, error_value);
}

// UART Tests
TEST(UARTTests, UARTInitialization)
{
    mock().expectOneCall("RCC_APB2ENR_USART").andReturnValue(1);
    mock().expectOneCall("USART_CR1").andReturnValue(1);
    mock().expectOneCall("USART_BRR").andReturnValue(1);
    GB_uart_init2();
}

TEST(UARTTests, UARTCharTransmission)
{
    mock().expectOneCall("USART_DR").andReturnValue(1);
    mock().expectOneCall("USART_SR_TXE").andReturnValue(1);
    GB_printChar2('A');
}

TEST(UARTTests, UARTStringTransmission)
{
    const char* test_string = "Test String";
    mock().expectOneCall("GB_printString2").withParameter("string", test_string);
    GB_printString2(test_string);
}

TEST(UARTTests, UARTNullHandling)
{
    GB_printString2(nullptr);
}

// GPIO Tests
TEST(GPIOTests, GPIOInitialization)
{
    mock().expectOneCall("RCC_APB2ENR_GPIO").andReturnValue(1);
    mock().expectOneCall("GPIO_CRL").andReturnValue(1);
    gpio_init();
}

// RCC Tests
TEST(RCCTests, SystemClockInit)
{
    mock().expectOneCall("RCC_CR").andReturnValue(1);
    mock().expectOneCall("RCC_CFGR").andReturnValue(2);
    mock().expectOneCall("RCC_CR_PLLON").andReturnValue(1);
    system_clk();
}

TEST(RCCTests, ClockConfiguration)
{
    mock().expectOneCall("RCC_CFGR_SW").andReturnValue(2);
    mock().expectOneCall("RCC_CFGR_SWS").andReturnValue(2);
    system_clk();
}

// Delay Tests
TEST(DelayTests, TimerInitialization)
{
    mock().expectOneCall("RCC_APB1ENR_TIM2").andReturnValue(1);
    mock().expectOneCall("TIM2_PSC").andReturnValue(1);
    mock().expectOneCall("TIM2_ARR").andReturnValue(1);
    timer_initialise();
}

TEST(DelayTests, DelayFunction)
{
    mock().expectOneCall("TIM2_CR1").andReturnValue(1);
    mock().expectOneCall("TIM2_SR_UIF").andReturnValue(1);
    delay_ms(1000);
}

TEST(DelayTests, MultipleDelays)
{
    mock().expectOneCall("delay_ms").withParameter("time", 1);
    mock().expectOneCall("delay_ms").withParameter("time", 10);
    mock().expectOneCall("delay_ms").withParameter("time", 100);
    
    delay_ms(1);
    delay_ms(10);
    delay_ms(100);
}

// Syscall Tests
TEST(SyscallTests, WriteFunction)
{
    char testData[] = "Test";
    mock().expectOneCall("ITM_SendChar").andReturnValue(1);
    // Simulate _write system call
    mock().actualCall("_write")
          .withParameter("file", 1)
          .withParameter("ptr", testData)
          .withParameter("len", sizeof(testData));
}

TEST(SyscallTests, ReadFunction)
{
    char buffer[10];
    mock().expectOneCall("ITM_ReceiveChar").andReturnValue(1);
    // Simulate _read system call
    mock().actualCall("_read")
          .withParameter("file", 0)
          .withParameter("ptr", buffer)
          .withParameter("len", sizeof(buffer));
}

// Integration Tests
TEST(MainTests, CompleteSystemTest)
{
    // Test full system initialization
    mock().expectOneCall("system_clk");
    mock().expectOneCall("gpio_init");
    mock().expectOneCall("timer_initialise");
    mock().expectOneCall("adc_init");
    mock().expectOneCall("GB_uart_init2");
    
    // Test ADC conversion and output
    mock().expectOneCall("adc_convert_START").andReturnValue(2048);
    mock().expectOneCall("GB_decimel2").withParameter("value", 2048);
    mock().expectOneCall("GB_printString2").withParameter("string", "\n");
    mock().expectOneCall("delay_ms").withParameter("time", 1000);
    
    // Execute sequence
    system_clk();
    gpio_init();
    timer_initialise();
    adc_init();
    GB_uart_init2();
    
    uint16_t adc_value = adc_convert_START();
    GB_decimel2(adc_value);
    GB_printString2("\n");
    delay_ms(1000);
}

// Main function
int main(int argc, char** argv) {
    return CommandLineTestRunner::RunAllTests(argc, argv);
}