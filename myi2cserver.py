#!/usr/bin/python
import rpyc
import subprocess
from rpyc.utils.server import ThreadedServer # or ForkingServer

class I2Cservice(rpyc.Service):
   def exposed_i2cWiperWrite(self, bus, pot,value):
      i2cargs="./writewiper "+ str(bus)+" "+str(pot)+" "+str(value)
      i2cWriteValue=subprocess.check_output(i2cargs,shell=True)
      return i2cWriteValue
   def myservice(self):
      print "pii2c"
   pass

if __name__ == "__main__":
    server = ThreadedServer(I2Cservice, port = 12345)
    server.start()
