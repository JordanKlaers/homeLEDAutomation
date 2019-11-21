#include <ESP8266WiFi.h>
#include <PubSubClient.h>


#define CLIENT_NAME "ESP8266_1" // just a name to identify this client
WiFiClient wifiClient;
PubSubClient mqttClient("10.0.0.131", 1883, wifiClient);

//set rgb led pins
int pinGreen = 4;
int pinBlue = 14;
int pinRed = 12;

void setup() 
{
  Serial.begin(9600);
  //set rgb led pins to output
  
  pinMode(pinRed, OUTPUT);
  pinMode(pinGreen, OUTPUT);
  pinMode(pinBlue, OUTPUT);
  digitalWrite(pinRed, HIGH);
  digitalWrite(pinGreen, HIGH);
  digitalWrite(pinBlue, HIGH);
  
  mqttClient.setCallback(callback);
  WiFi.begin("partyHouse", "whatsthepassword");
  connectToWiFiAndBroker();
}

void connectToWiFiAndBroker() 
{
  Serial.print("\nConnecting to WIFI");
  while (WiFi.status() != WL_CONNECTED) 
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println("Connected to WIFI!");

  Serial.println("Connecting to broker");
  while (!mqttClient.connect(CLIENT_NAME)) 
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println("Connected to broker!");

  mqttClient.subscribe("InterestingTopics/#");
}

//savaing for use when sending data 

//#define MILLISEC_DELAY_BETWEEN_PUBLISH 10000
//unsigned long lastTime = 0;
//unsigned long currentValue = 1;
//char msg[50];

void loop() 
{
  if (!mqttClient.connected()) 
  {
    connectToWiFiAndBroker();
  }

  mqttClient.loop();
  //Savaing as a refernce for how to send data
  
  //  if(millis() - lastTime > MILLISEC_DELAY_BETWEEN_PUBLISH) 
  //  {
  //    lastTime = millis();
  //    Serial.println("Publishing a new value");
  //    snprintf (msg, 75, "%ld", currentValue);
  //    Serial.println(currentValue);
  //    mqttClient.publish("ESP8266/CurrentValue", msg);
  //    currentValue++;
  //  }
}
int red;
int green;
int blue;

void callback(char* topic, byte* payload, unsigned int length) 
{
  Serial.println("Message received: ");
  Serial.println(topic);
  char colorCommand[length+1];
  for (int i = 0; i < length; i++) 
  {
    colorCommand[i] = (char)payload[i];
  }
  colorCommand[length+1] = '\0';
  splitAndSaveRGBValues(colorCommand);
  updateRGBFromIncomingValues();
}
void splitAndSaveRGBValues(char *colorCommand) {
  char *color;
  color = strtok (colorCommand,",");
  red = atoi(color);
  int increment = 1;
  while (color != NULL)
  {
    color = strtok (NULL, ",");
    if (increment == 1) {
      green = atoi(color);
    } if (increment == 2) {
      blue = atoi(color);
    }
    increment ++;
  }
}

void updateRGBFromIncomingValues() {
  Serial.print("red:");
  Serial.print(red);
  Serial.print(" - green:");
  Serial.print(green);
  Serial.print(" - blue:");
  Serial.println(blue);
  digitalWrite(pinRed, red);
  digitalWrite(pinGreen, green);
  digitalWrite(pinBlue, blue);
}
