#include <ESP8266WiFi.h>
#include <PubSubClient.h>


#define CLIENT_NAME "ESP8266_1" // just a name to identify this client
WiFiClient wifiClient;
PubSubClient mqttClient("10.0.0.131", 1883, wifiClient);

//set rgb led pins
int pinGreen = 4;
int pinBlue = 14;
int pinRed = 12;

int red;
int green;
int blue;

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

color *patternListObjects;
bool shouldRunPattern = false;

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

void loop() 
{
  if (!mqttClient.connected()) 
  {
    connectToWiFiAndBroker();
  }
  mqttClient.loop();
  if (shouldRunPattern) {
    Serial.println("in running pattern loop: ");
    for (int i = 0; i < patternLength; i ++) {
      Serial.print("Index: ");
      Serial.println(i);
      Serial.println(patternListObjects[i].red);
      //.printObj();
    }
    Serial.println(" ");
    Serial.println(" ");
    delay(10000);
  }
}
    
    void callback(char* topic, byte* payload, unsigned int length) 
    {
      shouldRunPattern = false;
      //save payload bytes into char
      int ticks = 1;
      char pattern[length+1];
      for (int i = 0; i < length; i++) 
      {
        pattern[i] = (char)payload[i];
      }
      pattern[length+1] = '\0';
//      Serial.print("this is the whole pattern: ");
//      Serial.println(pattern);
      splitPatternTicks(pattern, ticks);
    }
    
    void splitPatternTicks(char *pattern, int ticks) {
      char *colorsAtTick;
      char *colorsAtTickArray[ticks];
      //split pattern on ";" into chars
      colorsAtTick = strtok (pattern,";");
      while (colorsAtTick != NULL)
      {
        //save the chars into an array
        colorsAtTickArray[patternLength] = colorsAtTick;
        patternLength ++;
        colorsAtTick = strtok (NULL,";");
      }
      color list[patternLength];// = {};
      for (int i = 0; i < patternLength-1; i ++) {
        //loop over the array of char to further split and save values
        //colorsAtTickArray ex. "255,0,0,1000"
        Serial.println("trying to parse values, sending ->");
        Serial.println(colorsAtTickArray[i]);
        parseTickValues(colorsAtTickArray[i], i, list);
      }
//      Serial.print("LIST 0???: ");
//      Serial.println(list[0].red);
//      Serial.println(list[0].green);
//      Serial.println(list[0].blue);
//      Serial.println(list[0].transitionTime);
      patternListObjects = list;
      shouldRunPattern = true;
    }
void parseTickValues(char *tick, int patternIndex, color *list) {
  char *value;
  color TickObj;
  //red green blue time seperated by ","
  value = strtok (tick,",");
  for (int i = 0; i < 4; i ++) {
    if (i == 0) {
      TickObj.red = atoi(value);
    } else if (i == 1) {
      TickObj.green = atoi(value);
    } else if (i == 2) {
      TickObj.blue = atoi(value);
    } else if (i == 3) {
      TickObj.transitionTime = atoi(value);
    }
    value = strtok (NULL, ",");
  }
  
  //add color obj to the list
  list[patternIndex] = TickObj;
  Serial.println("tick obj red: ");
  Serial.println(list[patternIndex].red);
}



void updateRGBFromIncomingValues() {
  digitalWrite(pinRed, red);
  digitalWrite(pinGreen, green);
  digitalWrite(pinBlue, blue);
}
