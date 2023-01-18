/**
   Smart farm for Arduino

   Copyright (C) 2023, Yeo Ji Su

   Released under the MIT License.
*/

#define soilMoisture A1 // pin number of soil moisture sensor
#define photoresistor A0 // pin number of photoresistor
#define waterPump 13 // pin number of water pump
#define led 12 // pin number of LED
#define stdBrightness 500 // standard brightness
#define stdMoisture 700 // standard moisture content


void setup() 
{
    Serial.begin(9600);
    pinMode(waterPump, OUTPUT);
    pinMode(led, OUTPUT);
}
 
void loop() 
{
    Serial.print("A value of soil moisture:");
    Serial.println(analogRead(soilMoisture)); // Measuring Moisture with Soil Moisture Sensor 0 (wet) to 1023 (dry)
    Serial.print("A value of light:");
    Serial.println(analogRead(photoresistor)); // Measuring the current amount of light with the photoresistor 0(bright) ~ 1023(dark)
    delay(100); // Measuring at 0.1 second intervals

    if (analogRead(photoresistor) < stdBrightness) { // If bright
        // Turn off LED
        digitalWrite(led, LOW);
    }
    if (analogRead(photoresistor) >= stdBrightness){  // If dark
        // Turn on LED
        digitalWrite(led, HIGH);
    }
    if (analogRead(soilMoisture) > stdMoisture){ // If the soil moisture is insufficient
        // Print dry
        Serial.println("Very Dry!");
        // Hydrate
        digitalWrite(waterPump, HIGH); 
        delay(1000);  
    }
    if (analogRead(soilMoisture) <= stdMoisture){ // If the soil has enough moisture
        Serial.println("Very Wet!");
        // Do not hydrate
        digitalWrite(waterPump, LOW); 
        delay(1000);
    }
}
