#define ENABLE_TESTS  // Must be defined before including headers
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include <CppUTest/CommandLineTestRunner.h>
#include "../Inc/GB_adc.h"
#include <cstring>
extern "C" {
#include "../Inc/GB_UART.h"
#include "../Inc/gb_gpio.h"
}

volatile uint32_t MockGPIOC;
volatile uint32_t MockRCC;

// Mock functions for GPIO
uint32_t RCC_APB2ENR_GPIO(void) {
    return mock().actualCall("RCC_APB2ENR_GPIO").returnUnsignedIntValue();
}

uint32_t GPIO_CRL(void) {
    return mock().actualCall("GPIO_CRL").returnUnsignedIntValue();
}

void gpio_init(void) {
    mock().actualCall("gpio_init");
}

void GPIO_set(volatile uint32_t* port, uint32_t pin) {
    mock().actualCall("GPIO_set")
          .withParameter("port", (unsigned long)port)
          .withParameter("pin", pin);
}

void GPIO_reset(volatile uint32_t* port, uint32_t pin) {
    mock().actualCall("GPIO_reset")
          .withParameter("port", (unsigned long)port)
          .withParameter("pin", pin);
}

// Mock functions for ADC
static void mock_adc_configure_gpio(void) {
    mock().actualCall("configure_gpio");
}

static void mock_adc_configure_adc(void) {
    mock().actualCall("configure_adc");
}

static uint16_t mock_adc_read_conversion(void) {
    return mock().actualCall("read_conversion")
                 .returnUnsignedIntValue();
}




// Mock functions for UART
class MockUARTHardware : public UART_Hardware_Interface {
public:
    MockUARTHardware() {
        this->enable_uart_clock = mock_enable_uart_clock;
        this->configure_gpio = mock_configure_gpio;
        this->set_baud_rate = mock_set_baud_rate;
        this->is_transmit_empty = mock_is_transmit_empty;
        this->is_receive_not_empty = mock_is_receive_not_empty;
        this->write_data = mock_write_data;
        this->read_data = mock_read_data;
    }
    
    static void mock_enable_uart_clock() {
        mock().actualCall("enable_uart_clock");
    }
    
    static void mock_configure_gpio() {
        mock().actualCall("configure_gpio");
    }
    
    static void mock_set_baud_rate() {
        mock().actualCall("set_baud_rate");
    }
    
    static bool mock_is_transmit_empty() {
        return mock().actualCall("is_transmit_empty").returnBoolValue();
    }
    
    static bool mock_is_receive_not_empty() {
        return mock().actualCall("is_receive_not_empty").returnBoolValue();
    }
    
    static void mock_write_data(uint32_t data) {
        mock().actualCall("write_data").withParameter("data", data);
    }
    
    static uint16_t mock_read_data() {
        return mock().actualCall("read_data").returnUnsignedIntValue();
    }
};

// Create a static instance of MockUARTHardware
static MockUARTHardware mockUARTHw;

// Test group for ADC
TEST_GROUP(ADCTests) {
    const ADC_Hardware_Interface* hw;
    ADC_Hardware_Interface hw_impl;

    void setup() override {
        hw_impl.configure_gpio = mock_adc_configure_gpio;
        hw_impl.configure_adc = mock_adc_configure_adc;
        hw_impl.read_conversion = mock_adc_read_conversion;
        hw = &hw_impl;
    }

    void teardown() override {
        mock().clear();
    }
};

// Expanded GPIO test group
TEST_GROUP(GPIOTests) {
    void setup() override {
        mock().clear();
        MockGPIOC = 0;
        MockRCC = 0;
    }
    
    void teardown() override {
        mock().checkExpectations();
        mock().clear();
    }
};



// Test group for UART
TEST_GROUP(UARTTests) {
    UART_Config* config;
    
    void setup() override {
        config = new UART_Config();
        config->hw = &mockUARTHw;
        config->baud_rate = 115200;  // Changed from baudRate to baud_rate
        mock().clear();
    }
    
    void teardown() override {
        delete config;
        mock().checkExpectations();
        mock().clear();
    }
};





// ADC Tests
TEST(ADCTests, InitializationCallsCorrectFunctions) {
    mock().expectOneCall("configure_gpio");
    mock().expectOneCall("configure_adc");
    
    adc_init(hw);
    
    mock().checkExpectations();
}

TEST(ADCTests, NullHardwareHandling) {
    adc_init(nullptr);
    CHECK_EQUAL(0xFFFF, adc_convert_START(nullptr));
}

TEST(ADCTests, ConversionReturnsCorrectValue) {
    mock().expectOneCall("read_conversion")
          .andReturnValue(1234);
    
    CHECK_EQUAL(1234, adc_convert_START(hw));
}

TEST(ADCTests, ConversionBoundaryValues) {
    mock().expectOneCall("read_conversion").andReturnValue(0);
    CHECK_EQUAL(0, adc_convert_START(hw));

    mock().expectOneCall("read_conversion").andReturnValue(4095);
    CHECK_EQUAL(4095, adc_convert_START(hw));
}



// UART Tests
TEST(UARTTests, Initialization) {
    mock().expectOneCall("enable_uart_clock");
    mock().expectOneCall("configure_gpio");
    mock().expectOneCall("set_baud_rate");
    
    bool result = GB_uart_init(config);
    CHECK_TRUE(result);
}

TEST(UARTTests, TransmitCharacter) {
    mock().expectOneCall("is_transmit_empty").andReturnValue(true);
    mock().expectOneCall("write_data").withParameter("data", 42);
    
    bool result = GB_UART_TxChar(config, 42);
    CHECK_TRUE(result);
}

TEST(UARTTests, ReceiveCharacter) {
    mock().expectOneCall("is_receive_not_empty").andReturnValue(true);
    mock().expectOneCall("read_data").andReturnValue(0x55);
    
    uint16_t rx_value = GB_UART_RxChar(config);
    CHECK_EQUAL(0x55, rx_value);
}

TEST(UARTTests, PrintString) {
    const char* testString = "Hello";
    
    for (size_t i = 0; testString[i]; i++) {
        mock().expectOneCall("is_transmit_empty").andReturnValue(true);
        mock().expectOneCall("write_data").withParameter("data", testString[i]);
    }
    
    bool result = GB_printString(config, testString);
    CHECK_TRUE(result);
}

TEST(UARTTests, PrintDecimal) {
    uint32_t testValue = 1234;
    const char expected[] = "1234";
    
    for (const char* p = expected; *p; ++p) {
        mock().expectOneCall("is_transmit_empty").andReturnValue(true);
        mock().expectOneCall("write_data").withParameter("data", *p);
    }
    
    bool result = GB_decimel(config, testValue);
    CHECK_TRUE(result);
}

TEST(UARTTests, NullConfigHandling) {
    bool result = GB_uart_init(nullptr);
    CHECK_FALSE(result);
    
    result = GB_UART_TxChar(nullptr, 42);
    CHECK_FALSE(result);
    
    uint16_t rx_value = GB_UART_RxChar(nullptr);
    CHECK_EQUAL(0xFFFF, rx_value);
}
TEST(GPIOTests, GPIOInitialization) {
    mock().expectOneCall("RCC_APB2ENR_GPIO").andReturnValue(1);
    mock().expectOneCall("GPIO_CRL").andReturnValue(1);
    gpio_init();
}

TEST(GPIOTests, GPIOSetPin) {
    mock().expectOneCall("GPIO_set")
          .withParameter("port", (unsigned long)&MockGPIOC)
          .withParameter("pin", 13);
    GPIO_set(&MockGPIOC, 13);
}

TEST(GPIOTests, GPIOResetPin) {
    mock().expectOneCall("GPIO_reset")
          .withParameter("port", (unsigned long)&MockGPIOC)
          .withParameter("pin", 13);
    GPIO_reset(&MockGPIOC, 13);
}

TEST(GPIOTests, GPIOInitializationFailure) {
    mock().expectOneCall("RCC_APB2ENR_GPIO").andReturnValue(0);
    gpio_init();
    // Add appropriate checks based on your error handling
}

// Integration test with GPIO
TEST_GROUP(IntegrationTests) {
    void setup() override {
        mock().clear();
    }
    
    void teardown() override {
        mock().checkExpectations();
        mock().clear();
    }
};

TEST(IntegrationTests, GPIOWithADC) {
    // GPIO Initialization
    mock().expectOneCall("RCC_APB2ENR_GPIO").andReturnValue(1);
    mock().expectOneCall("GPIO_CRL").andReturnValue(1);
    
    // ADC Initialization
    mock().expectOneCall("configure_gpio");
    mock().expectOneCall("configure_adc");
    
    gpio_init();
    //adc_init(hw);
}




int main(int argc, char** argv) {
    return CommandLineTestRunner::RunAllTests(argc, argv);
}

