#include "DHT.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>


#define DHTPIN 2

#define DHTTYPE DHT11

DHT dht(DHTPIN,DHTTYPE);


float humidityData;
float temperatureData;


const char* ssid = "dlink";//internet fablab 
//const char* password = "ssss123456";

String apiKey = "3BEVHLU7JHD8MZ9T";     //   API key from ThingSpeak
const char* server = "api.thingspeak.com";
char server1[] = "192.168.1.35";   // Local IP Adress


WiFiClient client;    


void setup()
{
 Serial.begin(115200);
 delay(10);
 dht.begin();
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.begin(ssid);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
  // Start the server
  //server.begin();
  Serial.println("Server started");
  Serial.print(WiFi.localIP());
  delay(1000);
  Serial.println("connecting...");
 }

 
void loop()
{ 
  humidityData = dht.readHumidity();
  temperatureData = dht.readTemperature(); 
  sending_to_phpmyadmindb(); 
  sending_to_thingspeak();
  delay(2000); // interval
 }
 
void sending_to_thingspeak()
{
  if (client.connect(server,80))   
      {  
       String sendData = apiKey+"&field1="+String(temperatureData)+"&field2="+String(humidityData)+"\r\n\r\n"; 
      
       client.print("POST /update HTTP/1.1\n");
       client.print("Host: api.thingspeak.com\n");
       client.print("Connection: close\n");
       client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
       client.print("Content-Type: application/x-www-form-urlencoded\n");
       client.print("Content-Length: ");
       client.print(sendData.length());
       client.print("\n\n");
       client.print(sendData);

       Serial.print("Temperature: ");
       Serial.print(temperatureData);
       Serial.print("deg C. Humidity: ");
       Serial.print(humidityData);
       Serial.println("%. Connecting to Thingspeak.");
       }
      
      client.stop();

      Serial.println("Sending....");
  
 delay(5000);

}


 void sending_to_phpmyadmindb()   //Connect to mysql
 {
   if (client.connect(server1, 80)) {
    Serial.println("connected");
    // Make a HTTP request:
    Serial.println("humidity = ");
    client.print("GET http://localhost/testcode/dht.php?humidity=");     
    Serial.println(humidityData);
    client.print(humidityData);
    client.print("&temperature=");
    Serial.println("temperature = ");
    client.print(temperatureData);
    Serial.println(temperatureData);
    client.print(" ");      //SPACE BEFORE HTTP/1.1
    client.print("HTTP/1.1");
    client.println();
    client.println("Host: Your Local IP");
    client.println("Connection: close");
    client.println();
  } else {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
  }
 }
