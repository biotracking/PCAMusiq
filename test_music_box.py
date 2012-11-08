#!/usr/bin/env python

import dpy
from OSC import OSCClient, OSCMessage
import time, math

print dir(OSCMessage)

period = 10
param_count = 9
param_low = 0
param_high = 127
frequency = 30

client = OSCClient()
client.connect( ("127.0.0.1", 50000) )

def oscillate(index):
  return int( (param_high + param_low) / 2.0 + ( ( param_high - param_low ) / 2.0 ) * math.sin(index + time.time() / period) )

while True:
  values = map(lambda i: oscillate(i), range(0, param_count))
  print values
  message = OSCMessage("/fish")
  message += values
  client.send( message )
  time.sleep(1.0 / frequency)
