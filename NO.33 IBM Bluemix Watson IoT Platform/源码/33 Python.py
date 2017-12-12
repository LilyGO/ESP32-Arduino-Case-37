from random import randint
import thread
import sys
try:
    import paho.mqtt.client as mqtt
except ImportError:
    import os
    import inspect
    cmd_subfolder = os.path.realpath(os.path.abspath(os.path.join(os.path.split(inspect.getfile( inspect.currentframe() ))[0],"../src")))
    if cmd_subfolder not in sys.path:
        sys.path.insert(0, cmd_subfolder)
    import paho.mqtt.client as mqtt

ORG = "910d3w"
DEVICE_TYPE = "ESP32_IoTSharing"
APP_ID = "iotsharingdotcom" #choose any string 
TOKEN = "86a2mx(YZaYo0*&)B6"
DEVICE_ID = "A09E02A4AE30"

server = ORG + ".messaging.internetofthings.ibmcloud.com";
pubTopic = "iot-2/type/" + DEVICE_TYPE + "/id/" + DEVICE_ID + "/cmd/test/fmt/String";
subTopic = "iot-2/type/+/id/+/evt/+/fmt/+";
authMethod = "a-910d3w-io5gxbfau6";
token = TOKEN;
clientId = "a:" + ORG + ":" + APP_ID;


def on_connect(mqttc, obj, flags, rc):
    print("rc: "+str(rc))

def on_message(mqttc, obj, msg):
    print(msg.topic+" "+str(msg.qos)+" "+str(msg.payload))

def on_publish(mqttc, obj, mid):
    print("mid: "+str(mid))

def on_subscribe(mqttc, obj, mid, granted_qos):
    print("Subscribed: "+str(mid)+" "+str(granted_qos))

def on_log(mqttc, obj, level, string):
    print(string)

mqttc = mqtt.Client(client_id=clientId)
mqttc.on_message = on_message
mqttc.on_connect = on_connect
mqttc.on_publish = on_publish
mqttc.on_subscribe = on_subscribe
mqttc.username_pw_set(authMethod, token)
mqttc.connect(server, 1883, 60)
mqttc.subscribe(subTopic, 0)


def ledControl( threadName, delay):
    while True:
        val = raw_input('Enter on or off ')
        if(val == "on"):
            mqttc.publish(pubTopic, "1")
        else:
            mqttc.publish(pubTopic, "0")

try:
    thread.start_new_thread( ledControl, ("ledControl", 0, ) )
except:
    print "Error: unable to start thread"

mqttc.loop_forever()