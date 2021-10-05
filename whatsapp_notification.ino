
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <DHT.h>

WiFiClient wifiClient;

const int interval=60*60*1000;
const char* ssid = "Ruggeri-SenesiWIFI-2.4_EXT";
const char* password = "mamalorechia";
String tobesend;

const int trigPin = 12;
const int echoPin = 14;
const int buttonPin = D7;
long startTime = 0;

#define DHTPIN D1
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

int buttonState = 0;  

int totalApiCall = 0;

void setup() {
    dht.begin();
    pinMode(buttonPin, INPUT); 
    Serial.begin(9600); // Starting Serial Terminal
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED){
      delay(500);
      Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");  
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    startTime=millis();
}

void loop() {
   int t = dht.readTemperature();
   int h = dht.readHumidity();
   buttonState = digitalRead(buttonPin);
   long duration, inches, cm;
   pinMode(trigPin, OUTPUT);
   digitalWrite(trigPin, LOW);
   delayMicroseconds(2);
   digitalWrite(trigPin, HIGH);
   delayMicroseconds(10);
   digitalWrite(trigPin, LOW);
   pinMode(echoPin, INPUT);
   duration = pulseIn(echoPin, HIGH);
   inches = microsecondsToInches(duration);
   cm = microsecondsToCentimeters(duration);
   Serial.print(inches);
   Serial.print("in, ");
   Serial.print(cm);
   Serial.print("cm");
   Serial.println();
   Serial.print("t ");
   Serial.println(t);
   Serial.print("h ");
   Serial.println(h);
   delay(100);

   if (millis() > startTime+interval){
      startTime=millis();
      Serial.print("Temperature alert!");
      Serial.print("connecting to ");
      Serial.println("whatsapp");  
      HTTPClient http; //Declare an object of class HTTPClient
      //Specify request destination
      tobesend = "http://api.callmebot.com/whatsapp.php?";
      tobesend = tobesend + "phone=+393930144941";
      tobesend = tobesend + "&text=Temperature:"+t;
      tobesend = tobesend + "-Umidity:"+h;
      tobesend = tobesend + "&apikey=324736";
      http.begin(wifiClient,tobesend); 
      int httpCode = http.GET(); //Send the request
      if (httpCode > 0) { 
        //Check the returning code
        String payload = http.getString(); //Get the request response payload
        Serial.println(payload); //Print the response payload
      } 
      totalApiCall++;
      delay (5000);
   }

   if (buttonState == HIGH){
      Serial.print("Button Pushed!");
      Serial.print("connecting to ");
      Serial.println("whatsapp");  
      HTTPClient http; //Declare an object of class HTTPClient
      //Specify request destination
      tobesend = "http://api.callmebot.com/whatsapp.php?";
      tobesend = tobesend + "phone=+393930144941";
      tobesend = tobesend + "&text=Button+has+been+pushed";
      tobesend = tobesend + "&apikey=324736";
      http.begin(wifiClient,tobesend); 
      int httpCode = http.GET(); //Send the request
      if (httpCode > 0) { 
        //Check the returning code
        String payload = http.getString(); //Get the request response payload
        Serial.println(payload); //Print the response payload
      } 
      totalApiCall++;
      delay (5000);
   }
   if ( cm<25 ){
      Serial.print("connecting to ");
      Serial.println("whatsapp");  
      HTTPClient http; //Declare an object of class HTTPClient
      //Specify request destination
      tobesend = "http://api.callmebot.com/whatsapp.php?";
      tobesend = tobesend + "phone=+393930144941";
      tobesend = tobesend + "&text=Proximity+object+detected!";
      tobesend = tobesend + "&apikey=324736";
      http.begin(wifiClient,tobesend); 
      int httpCode = http.GET(); //Send the request
      if (httpCode > 0) { 
        //Check the returning code
        String payload = http.getString(); //Get the request response payload
        Serial.println(payload); //Print the response payload
      }
      http.end(); //Close connection
      totalApiCall++;
      delay(5000);
   }
   Serial.println("Api called " + String(totalApiCall) + " times"); 
}

long microsecondsToInches(long microseconds) {
   return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds) {
   return microseconds / 29 / 2;
}
