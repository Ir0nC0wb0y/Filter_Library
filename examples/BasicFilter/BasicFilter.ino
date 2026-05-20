#include <ExpFilter.h>

ExpFilter<float> temperatureFilter;

void setup() {
    Serial.begin(115200);

    temperatureFilter.setWeight(0.9);
    temperatureFilter.setValue(20.0);
}

void loop() {
    float sensorReading = analogRead(A0);

    float filtered =
        temperatureFilter.filter(sensorReading);

    Serial.print("Raw: ");
    Serial.print(sensorReading);

    Serial.print(" Filtered: ");
    Serial.println(filtered);

    delay(100);
}