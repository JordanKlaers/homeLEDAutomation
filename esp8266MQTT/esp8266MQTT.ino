#include <sstream>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
using namespace std;

#define CLIENT_NAME "ESP8266_1" // just a name to identify this client
WiFiClient wifiClient;
PubSubClient mqttClient("10.0.0.131", 1883, wifiClient);
using std::vector;
using std::istream;
using std::ostream;
using std::istringstream;

//set rgb led pins
int pinGreen = 4;
int pinBlue = 14;
int pinRed = 12;

int currentRed = 0;
int currentGreen = 0;
int currentBlue = 0;

int patternIndex = 0;
int startTime;
class color
{
  public:
    int red;
    int green;
    int blue;
    int transitionTime;
    istream &readStringData(istream &is);
};
vector<color> vec;


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
  mqttClient.subscribe("colorPattern/start");
  mqttClient.subscribe("colorPattern/end");
}

void loop() 
{
  if (!mqttClient.connected()) 
  {
    connectToWiFiAndBroker();
  }
  mqttClient.loop();

  if (vec.size() > 0) {
    runPattern();
  }
}
    
void callback(char* topic, byte* payload, unsigned int length) 
{
  Serial.print("-inc-");
  bool isEnd = false;
  if (strcmp(topic, "colorPattern/start") == 0) {
    vec.clear();
    Serial.println("cleared");
  } else {
    if (strcmp(topic, "colorPattern/end") == 0) {
      isEnd = true;
      Serial.println("end pattern");
    } else {
      Serial.println("regular pattern");
    }
    char pattern[length+1];
    for (int i = 0; i < length; i++) 
    {
      pattern[i] = (char)payload[i];
    }
    pattern[length+1] = '\0';
    parsePattern(pattern, isEnd);
  }
  
}

istream &
color::readStringData(istream &is)
{
  char comma1, comma2, comma3;
  is >> red >> comma1 >> green >> comma2 >> blue >> comma3 >> transitionTime;
  if (!(comma1== ',' && comma2 == ',' && comma3 == ',')) {
    // indicate that the conversion failed
    is.setstate(is.rdstate() | std::ios::failbit);
  }
  return is;
}



void parsePattern(char *pattern, bool isEnd) {
  istringstream strm(pattern);
  
  color c;
  while (c.readStringData(strm)) {
    char ch;
    vec.push_back(c);
    strm >> ch;   // read the semicolon
  }
  Serial.println("parsed one chunk");
  Serial.println(vec.size());
  if (isEnd == true) {
    patternIndex = 0;
    startTime = millis();
      
  }
}

void runPattern() {
  int nextIndex;
  if ((patternIndex + 1) == vec.size()) {
    nextIndex = 0;
  } else {
    nextIndex = patternIndex + 1;
  }
  int diffTime = millis() - startTime;
  if (diffTime < vec[patternIndex].transitionTime) {
    int percentage = (diffTime * 10000) / vec[patternIndex].transitionTime;

    //current is equal to the current value + a percentage of the difference to the next color;
    currentRed = vec[patternIndex].red + (((vec[nextIndex].red - vec[patternIndex].red) * percentage) / 10000);
    currentGreen = vec[patternIndex].green + (((vec[nextIndex].green - vec[patternIndex].green) * percentage) / 10000);
    currentBlue = vec[patternIndex].blue + (((vec[nextIndex].blue - vec[patternIndex].blue) * percentage) / 10000);
  } else {
    currentRed = vec[nextIndex].red;
    currentGreen = vec[nextIndex].green;
    currentBlue = vec[nextIndex].blue;
    startTime = millis();
    if ((patternIndex + 1) == vec.size()) {
      patternIndex = 0;
    } else {
      patternIndex = patternIndex + 1;
    }
  }
  analogWrite(pinRed, currentRed);
  analogWrite(pinGreen, currentGreen);
  analogWrite(pinBlue, currentBlue);
}
