import paho.mqtt.client as mqtt
import time

def onDisconnect(client):
    print "reconnecting"
    client.reconnect()


client = mqtt.Client()
client.connect("10.0.0.131", 1883, 60)
client.on_disconnect = onDisconnect

def map(x,in_min,in_max,out_min,out_max):
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;


text = raw_input("enter anything to send the pattern: ")
red = str(map(255,0,255,1024,0)) + "," + str(map(0,0,255,1024,0)) + ","+str(map(0,0,255,1024,0)) + ",1000;"
green = str(map(0,0,255,1024,0)) + "," + str(map(255,0,255,1024,0)) + ","+str(map(0,0,255,1024,0)) + ",1000;"
blue = str(map(0,0,255,1024,0)) + "," + str(map(0,0,255,1024,0)) + ","+str(map(255,0,255,1024,0)) + ",1000;"
off = str(map(0,0,255,1024,0)) + "," + str(map(0,0,255,1024,0)) + ","+str(map(0,0,255,1024,0)) + ",1000;"
# pattern = off

pattern = blue
pattern += off
pattern += blue
pattern += off
pattern += blue
pattern += off

pattern += green
pattern += off
pattern += green
pattern += off
# pattern += green
# pattern += off

# pattern += red
# pattern += off
# pattern += red
# pattern += off
# pattern += red
# pattern += off

print pattern
client.publish("colorPattern", pattern);



