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

pattern = str(map(255,0,255,1023,0)) + "," + str(map(0,0,255,1023,0)) + ","+str(map(0,0,255,1023,0)) + ",1000;"
pattern += str(map(255,0,255,1023,0)) + "," + str(map(255,0,255,1023,0)) + ","+str(map(0,0,255,1023,0)) + ",1000;"
pattern += str(map(0,0,255,1023,0)) + "," + str(map(255,0,255,1023,0)) + ","+str(map(255,0,255,1023,0)) + ",1000;"
pattern += str(map(0,0,255,1023,0)) + "," + str(map(0,0,255,1023,0)) + ","+str(map(255,0,255,1023,0)) + ",1000;"
pattern += str(map(255,0,255,1023,0)) + "," + str(map(0,0,255,1023,0)) + ","+str(map(255,0,255,1023,0)) + ",1000"
print pattern
client.publish("colorPattern", pattern);



