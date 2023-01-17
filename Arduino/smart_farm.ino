/**
   Smart farm for Arduino

   Copyright (C) 2023, Yeo Ji Su

   Released under the MIT License.
*/

#define soilMoisture A1 // 토양수분센서 핀
#define photoresistor A0 // 조도센서 핀
#define waterPump 13 // 워터펌프 핀
#define led 12 // led 핀
#define stdBrightness 500 // 기준 명도
#define stdMoisture 700 // 기준 수분량


void setup() 
{
    Serial.begin(9600);
    pinMode(waterPump, OUTPUT);
    pinMode(led, OUTPUT);
}
 
void loop() 
{
    Serial.print("A value of soil moisture:");
    Serial.println(analogRead(soilMoisture)); // 토양습도센서로 수분 측정하기 0(습함) ~ 1023(건조)
    Serial.print("A value of light:");
    Serial.println(analogRead(photoresistor)); // 조도센서로 현재 빛의 양 측정하기 0() ~ 1023()
    delay(100); // 0.1초 간격으로 수분 측정

    if (analogRead(photoresistor)>= stdBrightness) { // 밝다면
        // led 끄기
        digitalWrite(led, LOW);
    }
    if (analogRead(photoresistor)< stdBrightness){  // 어둡다면
        // led 켜기
        digitalWrite(led, HIGH);
    }
    if (analogRead(soilMoisture) > stdMoisture){ // 토양의 수분이 부족하다면
        // 건조하다고 출력.
        Serial.println("Very Dry!");
        // 수분 공급하기. 
        digitalWrite(waterPump, HIGH); // analogWrite 사용해도 됨.
        delay(1000);  
    }
    if (analogRead(soilMoisture) <= stdMoisture){ // 토양의 수분이 충분하다면,
        Serial.println("Very Wet!");
        // 수분 공급 X.
        digitalWrite(waterPump, LOW); 
        delay(1000);
    }
}