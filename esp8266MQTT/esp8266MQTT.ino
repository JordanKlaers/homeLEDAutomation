#include <ESP8266WiFi.h>
#include <PubSubClient.h>


#define CLIENT_NAME "ESP8266_1" // just a name to identify this client
WiFiClient wifiClient;
PubSubClient mqttClient("10.0.0.131", 1883, wifiClient);

//set rgb led pins
int pinGreen = 4;
int pinBlue = 14;
int pinRed = 12;
int patternLength = 0;

class color {
  public:
    int red;
    int green;
    int blue;
    int transitionTime;
    void printObj() {
      Serial.print("red:");
      Serial.print(red);
      Serial.print(" - green:");
      Serial.print(green);
      Serial.print(" - blue:");
      Serial.print(blue);
      Serial.print(" - time:");
      Serial.println(transitionTime);
    }
};
color *list[] = {};

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

  mqttClient.subscribe("colorPattern");
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
  char pattern[length+1];
  for (int i = 0; i < length; i++) 
  {
    pattern[i] = (char)payload[i];
  }
  pattern[length+1] = '\0';
  splitPatternTicks(pattern);
}

void splitPatternTicks(char *pattern) {
  char *colorsAtTick;
  colorsAtTick= strtok (pattern,";");
  while (colorsAtTick != NULL)
  {
    parseTickValues(colorsAtTick, patternLength);
    patternLength ++;
    colorsAtTick = strtok (NULL,";");
  }
  for (int i = 0; i < patternLength; i ++) {
    list[i] -> printObj();
  }
}
void parseTickValues(char *tick, int patternIndex) {
  char *value;
  color TickObj;
  //red green blue time seperated by ","
  for (int i = 0; i < 4; i ++) {
    value = strtok (tick,",");
    if (i == 0) {
      TickObj.red = atoi(value);
    } else if (i == 1) {
      TickObj.green = atoi(value);
    } else if (i == 2) {
      TickObj.blue = atoi(value);
    } else if (i == 3) {
      TickObj.transitionTime = atoi(value);
    }
  }
  //add color obj to the list
  list[patternIndex] = &TickObj;
}





void updateRGBFromIncomingValues() {
  digitalWrite(pinRed, red);
  digitalWrite(pinGreen, green);
  digitalWrite(pinBlue, blue);
}
