#!/usr/bin/python3
#SELECT COUNT(retina_sensores_id) FROM `retina_data` WHERE retina_sensores_id = 2
#DELETE FROM `retina_data` WHERE retina_sensores_id = 2 LIMIT 1

#Librerias
import serial
from serial import *
from time import sleep
import urllib
from urllib import request
import pymysql.cursors
import datetime
import math

#User Librerias
from data_adquisition_self_fuctions import *


try:
    arduino = serial.Serial("/dev/ttyACM1",9600,timeout = 5)
    sleep(1)
except:
    arduino = serial.Serial("/dev/ttyACM0",9600,timeout = 5)
    sleep(1)

while True:
   if arduino.in_waiting:
       packet = arduino.read.line()
       packet.decode(('utf').rstrip('\n'))
        
