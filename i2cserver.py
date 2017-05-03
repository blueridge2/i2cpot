#!/usr/bin/python
import rpyc
import subprocess
from rpyc.utils.server import ThreadedServer # or ForkingServer

class I2Cservice(rpyc.Service):
   """A very simple server to run the pots on the i2c subsystem"""
   def exposed_WiperWrite(self, bus, pot,value):
      """This will run the writewiper command which is in the current directory"""
      i2cargs="./writewiper "+ str(bus)+" "+str(pot)+" "+str(value)
      i2cWriteValue=subprocess.check_output(i2cargs,shell=True)
      return i2cWriteValue
   def myservice(self):
      print "pii2c"
   pass

if __name__ == "__main__":
   print "Start an i2cwiperWrite service on port 12345"
   print "Needs rpyc and subprocess to run"
   server = ThreadedServer(I2Cservice, port = 12345)
   server.start()
