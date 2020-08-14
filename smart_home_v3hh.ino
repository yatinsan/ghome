 
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <Servo.h>
//wifico
#include <ESP8266WebServer.h>
#include <EEPROM.h>
int i = 0;
int statusCode;
const char* ssid = "text";
const char* passphrase = "text";
String st;
String content;
//Function Decalration
bool testWifi(void);
void launchWeb(void);
void setupAP(void);
ESP8266WebServer server(80);


Servo myservo;


#define Relay0            D0
#define Relay1            D1
#define Relay2            D2
#define Relay3            D3
#define Relay4            D4
#define Relay5            D5
#define Relay6            D6
#define Relay7            D7



#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "RINOOFRAHMAN"            // Replace it with your username
#define  AIO_KEY        "aio_YELH461jsBTnKtJyJnQeWDvymaub"   // Replace with your Project Auth Key

int s;


WiFiClient client;


Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

/****************************** Feeds ***************************************/


// Setup a feed called 'onoff' for subscribing to changes
Adafruit_MQTT_Publish light0 = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/relay0");
Adafruit_MQTT_Publish light1 = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/relay1");
Adafruit_MQTT_Publish light2 = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/relay2");
Adafruit_MQTT_Publish light3 = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/relay3");
Adafruit_MQTT_Publish light4 = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/relay4");
Adafruit_MQTT_Publish light5 = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/relay5");
Adafruit_MQTT_Publish light6 = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/relay6");
Adafruit_MQTT_Publish light7 = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/relay7");
Adafruit_MQTT_Publish all = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/all");
Adafruit_MQTT_Publish gate = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/servo");

//after pub


Adafruit_MQTT_Subscribe Light0 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/relay0");
Adafruit_MQTT_Subscribe Light1 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/relay1"); // FeedName
Adafruit_MQTT_Subscribe Light2 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/relay2");
Adafruit_MQTT_Subscribe Light3 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/relay3");
Adafruit_MQTT_Subscribe Light4 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/relay4");
Adafruit_MQTT_Subscribe Light5 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/relay5"); // FeedName
Adafruit_MQTT_Subscribe Light6 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/relay6");
Adafruit_MQTT_Subscribe Light7 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/relay7");
Adafruit_MQTT_Subscribe ALL = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/all");
Adafruit_MQTT_Subscribe servo = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/servo");



void MQTT_connect();

void setup() {
  Serial.begin(115200);
  //wifico
  Serial.println();
  Serial.println("Disconnecting previously connected WiFi");
  WiFi.disconnect();
  EEPROM.begin(512); //Initialasing EEPROM
  delay(10);
   //---------------------------------------- Read eeprom for ssid and pass
   Serial.println("maked by yatin");
  Serial.println("Reading EEPROM ssid");
  String esid;
  for (int i = 0; i < 32; ++i)
  {
    esid += char(EEPROM.read(i));
  }
  Serial.println();
  Serial.print("SSID: ");
  Serial.println(esid);
  Serial.println("Reading EEPROM pass");

  String epass = "";
  for (int i = 32; i < 96; ++i)
  {
    epass += char(EEPROM.read(i));
  }
  Serial.print("PASS: ");
  Serial.println(epass);


   Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(esid);

   s = EEPROM.read(100);
  Serial.print("\n servo position  ");
  Serial.println(s);

    WiFi.begin(esid.c_str(),epass.c_str());
 if (testWifi())
  {
    Serial.println("Succesfully Connected!!!");
    
  }
  else
  {
    Serial.println("Turning the HotSpot On");
    launchWeb();
    setupAP();// Setup HotSpot
  }
   while ((WiFi.status() != WL_CONNECTED))
  {
    Serial.print(".");
    delay(100);
    server.handleClient();
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); 
  Serial.println(WiFi.localIP());
  

  
  
  


  pinMode(Relay0, OUTPUT);
  pinMode(Relay1, OUTPUT);
  pinMode(Relay2, OUTPUT);
  pinMode(Relay3, OUTPUT);
  pinMode(Relay4, OUTPUT);
  pinMode(Relay5, OUTPUT);
  pinMode(Relay6, OUTPUT);
  pinMode(Relay7, OUTPUT);
  digitalWrite(Relay0,1);
  digitalWrite(Relay1,1);
  digitalWrite(Relay2,1);
  digitalWrite(Relay3,1);
  digitalWrite(Relay4,1);
  digitalWrite(Relay5,1);
  digitalWrite(Relay6,1);
  digitalWrite(Relay7,1);
  myservo.attach(D8);
  myservo.write(s);

  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); 
  Serial.println(WiFi.localIP());
  

  
  // Setup MQTT subscription for onoff feed.
  mqtt.subscribe(&Light0);
  mqtt.subscribe(&Light1);
  mqtt.subscribe(&Light2);
  mqtt.subscribe(&Light3);
  mqtt.subscribe(&Light4);
  mqtt.subscribe(&Light5);
  mqtt.subscribe(&Light6);
  mqtt.subscribe(&Light7);
  mqtt.subscribe(&ALL);
  
  mqtt.subscribe(&servo);
  MQTT_connect();
  light0.publish("0");
  light1.publish("0");
  light2.publish("0");
  light3.publish("0");
  light4.publish("0");
  light5.publish("0");
  light6.publish("0");
  light7.publish("0");
  all.publish("0");
   gate.publish(s);
   


}

void loop() {

  MQTT_connect();

  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(20000))) {
    
    if (subscription == &servo) {
      Serial.print(F("Gate:  "));
      Serial.println((char *)servo.lastread);
      int servo_State = atoi((char *)servo.lastread);
      myservo.write (servo_State);
      EEPROM.write(100,servo_State);
      EEPROM.commit();
    }
    if (subscription == &Light0) {
      Serial.print(F("Relay0: "));
      Serial.println((char *)Light0.lastread);
      int Light0_State = atoi((char *)Light0.lastread);
      digitalWrite(Relay0,not Light0_State);
      
    }
    if (subscription == &Light1) {
      Serial.print(F("Relay1: "));
      Serial.println((char *)Light1.lastread);
      int Light1_State = atoi((char *)Light1.lastread);
      digitalWrite(Relay1,not Light1_State);
       
      
    }
    if (subscription == &Light2) {
      Serial.print(F("Relay2: "));
      Serial.println((char *)Light2.lastread);
      int Light2_State = atoi((char *)Light2.lastread);
      digitalWrite(Relay2,not Light2_State);
    }
    if (subscription == &Light3) {
      Serial.print(F("Relay3: "));
      Serial.println((char *)Light3.lastread);
      int Light3_State = atoi((char *)Light3.lastread);
      digitalWrite(Relay3,not Light3_State);
    }
    if (subscription == &Light4) {
      Serial.print(F("Relay4: "));
      Serial.println((char *)Light4.lastread);
      int Light4_State = atoi((char *)Light4.lastread);
      digitalWrite(Relay4,not Light4_State);
      
    }
    if (subscription == &Light5) {
      Serial.print(F("Relay5: "));
      Serial.println((char *)Light5.lastread);
      int Light5_State = atoi((char *)Light5.lastread);
      digitalWrite(Relay5,not Light5_State);
       
      
    }
    if (subscription == &Light6) {
      Serial.print(F("Relay6: "));
      Serial.println((char *)Light6.lastread);
      int Light6_State = atoi((char *)Light6.lastread);
      digitalWrite(Relay6,not Light6_State);
    }
    if (subscription == &Light7) {
      Serial.print(F("Relay7: "));
      Serial.println((char *)Light7.lastread);
      int Light7_State = atoi((char *)Light7.lastread);
      digitalWrite(Relay7,not Light7_State);
    }
    if (subscription == &ALL) {
      Serial.print(F("Relay7: "));
      Serial.println((char *)ALL.lastread);
      int ALL_State = atoi((char *)ALL.lastread);
      digitalWrite(Relay0,not ALL_State);
      digitalWrite(Relay1,not ALL_State);
      digitalWrite(Relay2,not ALL_State);
      digitalWrite(Relay3,not ALL_State);
      digitalWrite(Relay4,not ALL_State);
    }
  
  }

  
}

void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");
 

  uint8_t retries =10000;
  
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);  // wait 5 seconds
    retries;
    if (retries == 0) {
      // basically die and wait for WDT to reset me
      while (1);
    }
  }
  Serial.println("MQTT Connected!");
 
  
}
bool testWifi(void)
{
  int c = 0;
  Serial.println("Waiting for Wifi to connect");
  while ( c < 25 ) {
    if (WiFi.status() == WL_CONNECTED)
    {
      return true;
    }
    delay(1000);
    Serial.print("*");
    c++;
  }
  Serial.println("");
  Serial.println("Connect timed out, opening AP");
  return false;
}
