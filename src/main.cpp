#include <Arduino.h>

// #define R_BIAS 2240
#define R_BIAS 2200
#define V_BIAS 2200
#define DAC_RESOLUTION 12
#define ADC_VREF 3300
#define ADC_RESOLUTION 13
#define ADC_MAX ((1<<ADC_RESOLUTION) - 1)

void setup()
{
    Serial.begin(9600);
    while(!Serial){}
    Serial.println("hello\n");
    analogWriteResolution(DAC_RESOLUTION);
    analogReadResolution(ADC_RESOLUTION);
    analogWrite(A21, (((uint32_t)2200) * ((1<<DAC_RESOLUTION) - 1) / 3300));
}

void loop()
{
    uint32_t raw_v = analogRead(A0);
    uint32_t vout = (raw_v * ADC_VREF) / ADC_MAX;
    uint32_t denom = V_BIAS - vout;
    uint32_t r2 = (vout*R_BIAS)/denom;
    Serial.print("r_v:");
    Serial.print(raw_v);
    Serial.print(" vout:");
    Serial.print(vout);
    Serial.print(" r2:");
    if (denom == 0) {
        Serial.println("Inf");
    } else {
        Serial.println(r2);
    }
    delay(10);
}
