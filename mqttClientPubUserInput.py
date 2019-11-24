import paho.mqtt.client as mqtt

def onDisconnect(client):
    print "reconnecting"
    client.reconnect()


client = mqtt.Client()
client.connect("10.0.0.131", 1883, 60)
client.on_disconnect = onDisconnect

def map(x,in_min,in_max,out_min,out_max):
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;


while True:
    # red = raw_input("red: ")
    # green = raw_input("green: ")
    # blue = raw_input("blue: ")
    # text = str(int(map(int(red),0,255,1023,0))) + "," + str(int(map(int(green),0,255,1023,0))) + "," + str(int(map(int(blue),0,255,1023,0)))
    color = raw_input("Color: ")
    color = color.split(',')
    color[0] = int(map(int(color[0]),0,255,1024,0))
    color[1] = int(map(int(color[1]),0,255,1024,0))
    color[2] = int(map(int(color[2]),0,255,1024,0))
    client.publish("InterestingTopics/time", str(int(color[3]) +1))
    client.publish("InterestingTopics/red", str(color[0]));
    client.publish("InterestingTopics/green", str(color[1]));
    client.publish("InterestingTopics/blue", str(color[2]));