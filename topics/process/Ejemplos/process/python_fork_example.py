import os

def child_2():  
   print 'A new child ',  os.getpid( )
   while True:
      if raw_input( ) == 'q': break

def child_1():
   print 'A new child ',  os.getpid( )
   os._exit(0)         

def parent():
   while True:
      newpid = os.fork()
      if newpid == 0:
         child_1()
      else:
         pids = (os.getpid(), newpid)
         print "parent: %d, child: %d" % pids
      if raw_input( ) == 'q': break

parent()