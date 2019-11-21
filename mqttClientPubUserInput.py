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
    red = raw_input("red: ")
    green = raw_input("green: ")
    blue = raw_input("blue: ")
    text = str(int(map(int(red),0,255,1023,0))) + "," + str(int(map(int(green),0,255,1023,0))) + "," + str(int(map(int(blue),0,255,1023,0)))
    print "sending: "
    print text
    # text = raw_input("input: ")
    client.publish("InterestingTopics/CurrentValue", text);