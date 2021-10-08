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
    tiempoInicio = datetime.datetime.now()

    #Funcion para leer Serial
    data = arduino.readline()

    if(data == "@" & var == 1):
        var += 1
        while arduino.inWaiting() > 0:
            sleep(0.1)
            Temperatura_Raw = arduino.readall().decode()
            Temperatura = Temperatura_Raw.split(',')
    elif(data == "#" & var == 1):
        var += 1
        while arduino.inWaiting() > 0:
            sleep(0.1)
            Humedad_Raw = arduino.readall().decode()
            Humedad = Temperatura_Raw.split(',')
    elif(data == "%" & var == 1):
        var += 1
        while arduino.inWaiting() > 0:
            sleep(0.1)
            Voltaje_Raw = arduino.readall().decode()
            Voltaje = Voltaje_Raw.split(',')
    elif(data == "&" & var == 1):
        var += 1
        while arduino.inWaiting() > 0:
            sleep(0.1)
            Corriente_Raw = arduino.readall().decode()
            Corriente = Corriente_Raw.split(',')
    elif(data == "*" & var == 1):
        var += 1
        while arduino.inWaiting() > 0:
            sleep(0.1)
            Potencia_Raw = arduino.readall().decode()
            Potencia = Potencia_Raw.split(',')
    elif(data == "+" & var == 1):
        var += 1
        while arduino.inWaiting() > 0:
            sleep(0.1)
            Potencia_Raw = arduino.readall().decode()
            Potencia = Potencia_Raw.split(',')
    else:
        var = 1
        
