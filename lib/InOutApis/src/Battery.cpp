#include "Battery.h"
#include <Arduino.h>
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include "esp32-hal-gpio.h"

/**
 * @brief Construct a new Battery:: Battery object
 * 
 * @param pinPowerIn pin to detect the power input
 * @param pinBatteryLevel pin to measure the battery levl
 */
Battery::Battery(int pinPowerIn, int pinBatteryLevel)
{
    this->pinPowerIn = pinPowerIn;
    this->pinBatteryLevel = pinBatteryLevel;
}

/**
 * @brief Configure peripherals
 * 
 */
void Battery::setup()
{
    pinMode(pinPowerIn,INPUT_PULLDOWN);
    pinMode(pinBatteryLevel,INPUT);
    adc1_config_width(ADC_WIDTH_BIT_10);
    adc1_config_channel_atten((adc1_channel_t)digitalPinToAnalogChannel(pinBatteryLevel), ADC_ATTEN_DB_11);

    adc_chars = (esp_adc_cal_characteristics_t*) calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_10, 1100, adc_chars);
}

uint32_t Battery::readBatteryLevel()
{
    
    uint32_t batval = 0;

    for (int i = 0; i < 64; i++)
    {
        batval += adc1_get_raw((adc1_channel_t)digitalPinToAnalogChannel(pinBatteryLevel));
        delay(1);
    }
    batval /= 64;
    return esp_adc_cal_raw_to_voltage(batval, adc_chars)*2;
}

/**
 * @brief read if the power in charger is connected
 * 
 * @return true if is plugged in
 * @return false if the charger is disconnected
 */
bool Battery::readPowerIn()
{
    return digitalRead(pinPowerIn) ? true : false ;
}
