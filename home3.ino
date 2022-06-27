//--------------------------------------------------------------------------------------------------------------------- START LIBRARY
//--------------------------------------------------------------- library wifi dan mqtt
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <WiFiManager.h>

//--------------------------------------------------------------- library pzem
#include <PZEM004Tv30.h>
#include <SoftwareSerial.h>
#if defined(ESP32)
#error "Software Serial is not supported on the ESP32"
#endif
#if !defined(PZEM_RX_PIN) && !defined(PZEM_TX_PIN)
#define PZEM_RX_PIN 12
#define PZEM_TX_PIN 13
#endif
//--------------------------------------------------------------------------------------------------------------------- END LIBRARY

//--------------------------------------------------------------------------------------------------------------------- START INISIALISASI
//--------------------------------------------------------------- inisialisasi pin led
int LED = 14;
int flag = 0;
//--------------------------------------------------------------- inisialisasi wifi
const char* ssid     = "Adeel";
const char* password = "misqueen";

//--------------------------------------------------------------- inisialisasi topic
const char *mqtt_broker = "132.145.112.175";
const char *topic1 = "homeassist/home/alat3/tegangan";
const char *topic2 = "homeassist/home/alat3/arus";
const char *topic3 = "homeassist/home/alat3/power";
const char *topic4 = "homeassist/home/alat3/energy";
const char *topic5 = "homeassist/home/alat3/frequency";
const char *topic6 = "homeassist/home/alat3/pf";
const char *topic7 = "homeassist/home/alat3/status";
const char *topic = "homeassist/home/alat3/led";
const char *mqtt_username = "adeela";
const char *mqtt_password = "adeela1";
const int mqtt_port = 1883;

unsigned long lastTime;

//--------------------------------------------------------------- inisialisasi broker
WiFiClient espClient;
PubSubClient client(espClient);

//--------------------------------------------------------------- inisialisasi pzem
SoftwareSerial pzemSWSerial(PZEM_RX_PIN, PZEM_TX_PIN);
PZEM004Tv30 pzem(pzemSWSerial);
//--------------------------------------------------------------------------------------------------------------------- STOP INISIALISASI

//--------------------------------------------------------------------------------------------------------------------- START SETUP
void setup() {
  pinMode(14, OUTPUT);
  //--------------------------------------------------------------- setup serial
  Serial.begin(115200);
  
  //--------------------------------------------------------------- setup koneksi wifi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
//      delay(500);
//      Serial.println(".");
    WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP    
    WiFiManager wm;
    wm.resetSettings();
    bool res;
    res = wm.autoConnect("AutoConnectAP","password"); // password protected ap
    if(!res) {
        Serial.println("Failed to connect");
    } 
    else {   
        Serial.println("connected...yeey :)");
    }
  }
  Serial.println("Connected to the WiFi network");
  
  //--------------------------------------------------------------- setup broker
  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);
  while (!client.connected()) {
    String client_id = "esp8266-client-";
    client_id += String(WiFi.macAddress());
    Serial.println("Connecting to public emqx mqtt broker.....");
    while (!client.connected()) {
      String client_id = "esp8266-client-";
      client_id += String(WiFi.macAddress());
      Serial.printf("The client %s connects to the public mqtt broker\n", client_id.c_str());
      if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
        Serial.println("Public emqx mqtt broker connected");
        } else {
          Serial.print("failed with state ");
          Serial.print(client.state());
          delay(2000);
          }        
        }
    }
    //subscribe
    client.publish(topic, "test");
    client.subscribe(topic);
}
//--------------------------------------------------------------------------------------------------------------------- STOP SETUP

//--------------------------------------------------------------------------------------------------------------------- START CALLBACK
void callback(char *topic, byte *payload, unsigned int length) {
    Serial.print("Message arrived in topic: ");
    Serial.println(topic);
    Serial.print("Message:");
    String message;
    for (int i = 0; i < length; i++) {
        message = message + (char) payload[i];  // convert *byte to string
    }
    Serial.print(message);
    if (message == "1") { 
      digitalWrite(LED, HIGH);
      Serial.println("ON");}   // LED on
    if (message == "0") { 
      digitalWrite(LED, LOW);
      Serial.println("OFF");} // LED off
    Serial.println();
    Serial.println("-----------------------");
}
//--------------------------------------------------------------------------------------------------------------------- STOP CALLBACK

//--------------------------------------------------------------------------------------------------------------------- START LOOP
void loop() {
  if(flag==1000){
    kirim_data();
    flag++;
  }else if ((flag==2000)){
    kirim_data();
    flag++;
  }else if ((flag==3000)){
    kirim_data();
    flag++;
  }else if ((flag==4000)){
    kirim_data();
    flag++;
  }else if ((flag==5000)){
    kirim_data();
    flag++;
  }else if ((flag==6000)){
    kirim_data();
    flag++;
  }else if ((flag==7000)){
    kirim_data();
    flag++;
  }else if ((flag==8000)){
    kirim_data();
    flag = 0;
  }else{
    client.loop();
    flag++;
    Serial.println(flag);
    
  }
  //menit();
}
//--------------------------------------------------------------------------------------------------------------------- STOP LOOP

//--------------------------------------------------------------------------------------------------------------------- START KIRIM DATA
void kirim_data(){
//  Serial.print("Custom Address:");
//  Serial.println(pzem.readAddress(), HEX);
  
  //--------------------------------------------------------------- membaca sensor
  float voltage = pzem.voltage();
  float current = pzem.current();
  float power = pzem.power();
  float energy = pzem.energy();
  float frequency = pzem.frequency();
  float pf = pzem.pf();
  float status;
  float statusmenit;
  if (power < 10){
    status = 0.0;
    statusmenit = 0.0;
    }
  else {
    status = 1.0;
    statusmenit = 1.0;
    }
  
  //--------------------------------------------------------------- cek validasi data
  if(isnan(voltage)){Serial.println("Error reading voltage");
  } else if (isnan(current))      {Serial.println("Error reading current");
  } else if (isnan(power))        {Serial.println("Error reading power");
  } else if (isnan(energy))       {Serial.println("Error reading energy");
  } else if (isnan(frequency))    {Serial.println("Error reading frequency");
  } else if (isnan(pf))           {Serial.println("Error reading power factor");
  }

  //--------------------------------------------------------------- konversi data kebentuk static char
  static char voltageTemp[7];     dtostrf(voltage, 6, 2, voltageTemp);
  static char currentTemp[7];     dtostrf(current, 6, 2, currentTemp);
  static char powerTemp[7];       dtostrf(power, 6, 2, powerTemp);
  static char energyTemp[7];      dtostrf(energy, 6, 2, energyTemp);
  static char frequencyTemp[7];   dtostrf(frequency, 6, 2, frequencyTemp);
  static char pfTemp[7];          dtostrf(pf, 6, 2, pfTemp);
  static char statusTemp[7];      dtostrf(status, 6, 2, statusTemp);
  static char statusmenitTemp[7];      dtostrf(statusmenit, 6, 2, statusmenitTemp);
  
  if(flag==1000){
    client.publish(topic1, voltageTemp);      client.subscribe(topic1);
  }else if ((flag==2000)){
    client.publish(topic2, currentTemp);      client.subscribe(topic2);
  }else if ((flag==3000)){
    client.publish(topic3, powerTemp);        client.subscribe(topic3);
  }else if ((flag==4000)){
    client.publish(topic4, energyTemp);       client.subscribe(topic4);
  }else if ((flag==5000)){
    client.publish(topic5, frequencyTemp);    client.subscribe(topic5);
  }else if ((flag==6000)){
    client.publish(topic6, pfTemp);           client.subscribe(topic6);
  }else if ((flag==7000)){
    client.publish(topic7, statusTemp);       client.subscribe(topic7);
  }else if ((flag==8000)){
    if (millis() - lastTime >= 60 * 1000UL) {
        lastTime = millis();
    client.publish("homeassist/home/alat3/statusmenit", statusmenitTemp);}
  }
 }
//--------------------------------------------------------------------------------------------------------------------- STOP KIRIM DATA
