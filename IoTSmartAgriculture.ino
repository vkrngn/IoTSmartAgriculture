#define BLYNK_TEMPLATE_ID "*********" // Data from Blynk
#define BLYNK_TEMPLATE_NAME "*********" // Data from Blynk
#define BLYNK_AUTH_TOKEN "*********" // Data from Blynk
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h> //Temp & Humidity

char auth[] = BLYNK_AUTH_TOKEN;

char ssid[] = "FantasticFive";  // Enter your wifi name
char pass[] = "123456789";  // Enter your wifi password

#define DHTPIN D2 // What digital pin we're connected to
const int analogPinA0 = A0;
int relaypin = D4;
#define DHTTYPE DHT11 // DHT 11

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

//Variables for Soil Moisture
int sensorDatas;
int output;

//LED
BLYNK_WRITE(V0){
  digitalWrite(D0, param.asInt()); // D0+Ground = Output pin on NodeMCU 
}

//DHT11
void sendSensor()
{
  //LED
  pinMode(D0, OUTPUT);

  //DHT11
  float h = dht.readHumidity();
  float t = dht.readTemperature(); 
  if (isnan(h) || isnan(t)) {
  Serial.println("Failed to read from DHT sensor!");
    return;
  }
  Blynk.virtualWrite(V2, h);
  Blynk.virtualWrite(V1, t);

  //Soil Moisture
  sensorDatas = analogRead(A0); //reading the sensor on A0

  if ( isnan(sensorDatas) ){
    
    return;
  } else {
    sensorDatas = constrain(sensorDatas,400,1023);  //Keep the ranges!
    output = map(sensorDatas,800,1023,100,0);  
    Blynk.virtualWrite(V3, output);
  }

}

void setup() {
  WiFi.hostname("Fantastic Five"); // Sets NodeMCU device Name
  Serial.begin(9600);
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  dht.begin();
  timer.setInterval(1000L, sendSensor);
  pinMode(relaypin,OUTPUT);
}

void loop() {
  Blynk.run();
  timer.run();
}
