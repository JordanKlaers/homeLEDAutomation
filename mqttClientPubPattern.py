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

pattern = [red, off, red, off, green, off ,green, off, blue, off, blue, off, green, blue, red]
print len(pattern)
send = ""
client.publish("colorPattern/start", "0");
time.sleep(0.5)
for x in pattern:
  if len(send + x) < 125:
    print("adding:", x)
    send = send + x
  else:
    print("\n")
    client.publish("colorPattern", send);
    print("sent")
    time.sleep(0.5)
    send = x
client.publish("colorPattern", send);
print(off)
client.publish("colorPattern/end", off);



