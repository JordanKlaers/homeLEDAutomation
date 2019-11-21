import RPi.GPIO as GPIO
from lib_nrf24 import NRF24
import time
import spidev

GPIO.setmode (GPIO.BCM)

pipes = [[0xe7, 0xe7, 0xe7, 0xe7, 0xe7], [0xc2, 0xc2, 0xc2, 0xc2, 0xc2]]

radio = nrf24 (GPIO, spidev.SpiDev())
radio.begin(0, 17)
radio.setPayloadSize(32)
radio.setChannel(0x60)

radio.setDataRate(NRF24.BR_2MBPS)
radio.setPALevel(NRF24.PA_MAX)
radio.setAutoAck(true)
radio.enableDynamicPayloads()
radio.enableAckPayload()

#radio.openReadingPipe(1, pipes[1])
radio.openWritingPipe(pipes[1])
radio.printDetails()

#radio.startListening()

while True:
    message = list("Hello World")
    radio.write(message)
    print("we sent the message of {}".format(message))

    # check if it returned ackPL
    if radio.isAckPayloadAvailable():
        returnedPL = []
        radio.read(returnedPL, radio.getDynamicPayloadSize)
        print("Our returned playing was {}".format(returnedPL))
    else:
        print("no payload recieved")
    time.sleep(1);