#include "WiFi.h"
#include <PubSubClient.h>
#include <map>
#include <string.h>
typedef void (*callbackScript)(String);
std::map<String,callbackScript> topics;

 
const char* ssid = "alka2";
const char* password =  "inet12inet12";
const char* mqtt_server = "lapsoft.mooo.com";
const char* mqtt_login = "esp";
const char* mqtt_password = "L9{HTRfq7#N!";
const int mqtt_port = 10101;

WiFiClient espClient;
PubSubClient client(espClient);



//Функция для подписывания
void subscribe(const char* name, callbackScript script){
  topics.insert(std::make_pair(String(name),script));
  client.subscribe(name);
}

//Вызывается при получении сообщения топиков
void MQTTcallback(char* topic, byte* payload, unsigned int length) 
{
  // Serial.print("Message received in topic: (");
  // Serial.print(topic);
  // Serial.println(")");
  // Serial.print("Message:");
  String message;
  for (int i = 0; i < length; i++) 
  {
    message = message + (char)payload[i];
  }
  // Serial.print(message);
  // Serial.println();
  
  if ((topics.find(String(topic)))==topics.end())
  {
    Serial.println("Error");
  }
  else{
    ((*topics.find(String(topic))).second)(message);
  }
  // Serial.println();
  // Serial.println();
  
}

void TopicOut(String s){
  Serial.println(s);
}


void Topic2Out(String s){
  Serial.println("msg from test1:");
  Serial.println(s);
}


void setup() 
{
  Serial.begin(115200);
  WifiConnect();
  MQTTSetup();
  subscribe("esp/test",TopicOut);
  subscribe("esp/test1",Topic2Out);
}


void loop(){
client.loop();
delay(100);
}


//Ниже точно все нормально
void WifiConnect(){
WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println(WiFi.SSID());
}
void MQTTSetup(){
client.setServer(mqtt_server, mqtt_port);
  client.setCallback(MQTTcallback);
  while (!client.connected()) 
  {
    Serial.println("Connecting to MQTT...");
    if (client.connect("esp", mqtt_login,mqtt_password))
    {
      Serial.println("connected");
    }
    else
    {
      Serial.print("failed with state ");
      Serial.println(client.state());
      
    }
  }
}
