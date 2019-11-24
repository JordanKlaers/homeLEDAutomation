import paho.mqtt.client as mqtt
import time

def onDisconnect(client):
    print "reconnecting"
    client.reconnect()


client = mqtt.Client()
client.connect("10.0.0.131", 1883, 60)
client.on_disconnect = onDisconnect

transition = 1
def map(x,in_min,in_max,out_min,out_max):
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;

def redtoorange():
  global greenbrightlvl
  greenbrightlvl += 1;
  if (greenbrightlvl >=79):
      greenbrightlvl = 80
      global transition
      transition = 2

def orangetoyellow():
  global greenbrightlvl
  greenbrightlvl += 2;
  if (greenbrightlvl >= 220):
      greenbrightlvl = 225
      global transition
      transition = 3

def yellowtolimegreen():
  global redbrightlvl
  redbrightlvl -= 2
  if (redbrightlvl <= 130):
      redbrightlvl = 128
      global transition
      transition = 4

def limegreentogreen():
  global redbrightlvl
  redbrightlvl -= 1
  if (redbrightlvl <=2):
      redbrightlvl = 0
      global transition
      transition = 5

def greentoturquoise():
  global bluebrightlvl
  bluebrightlvl += 3
  if (bluebrightlvl >= 219):
      bluebrightlvl = 225
      global transition
      transition = 6

def turquoisetoblue():
  global greenbrightlvl
  greenbrightlvl -= 3
  if (greenbrightlvl <= 6):
      greenbrightlvl = 0
      global transition
      transition = 7

def bluetopurpley():
  global redbrightlvl
  redbrightlvl += 1
  if (redbrightlvl >= 124):
      redbrightlvl = 127
      global transition
      transition = 8

def purpleytopurple():
  global redbrightlvl
  redbrightlvl += 2
  if (redbrightlvl >=220):
      redbrightlvl = 225
      global transition
      transition = 9

def purpletopink():
  global bluebrightlvl
  bluebrightlvl -= 3
  if (bluebrightlvl <= 84):
      bluebrightlvl = 80
      global transition
      transition = 10

def pinktored(): 
  global bluebrightlvl
  bluebrightlvl -= 1
  if (bluebrightlvl <= 2):
      bluebrightlvl = 0
      global transition
      transition = 1



bluebrightlvl = 0
greenbrightlvl = 0
redbrightlvl = 255

while True:
    # text = raw_input("Message to send: ")
    # print text
    text = str(int(map(redbrightlvl,0,255,1023,0))) + "," +str(int(map(greenbrightlvl,0,255,1023,0)))+ ","+str(int(map(bluebrightlvl,0,255,1023,0)))
    red = int(map(int(redbrightlvl),0,255,1023,0))
    green = int(map(int(greenbrightlvl),0,255,1023,0))
    blue = int(map(int(bluebrightlvl),0,255,1023,0))
    client.publish("InterestingTopics/red", red);
    client.publish("InterestingTopics/green", green);
    client.publish("InterestingTopics/blue", blue);
    time.sleep(0.02)
    # client.publish("InterestingTopics/CurrentValue", text);

    if (transition == 1):
      redtoorange()
    elif (transition == 2):
      orangetoyellow()
    elif (transition == 3):
      yellowtolimegreen()
    elif (transition == 4):
      limegreentogreen()
    elif (transition == 5):
      greentoturquoise()
    elif (transition == 6):
      turquoisetoblue()
    elif (transition == 7):
      bluetopurpley()
    elif (transition == 8):
      purpleytopurple()
    elif (transition == 9):
      purpletopink()
    elif (transition == 10):
      pinktored()
