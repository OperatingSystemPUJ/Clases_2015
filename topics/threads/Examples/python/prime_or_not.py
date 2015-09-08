import threading 
 
class PrimeNumber(threading.Thread): 
    def __init__(self, number): 
        threading.Thread.__init__(self) 
        self.Number = number
 
    def run(self): 
        if self.Number > 1:
            # check for factors
            for i in range(2, self.Number):
                if (self.Number % i) == 0:
                    print self.Number,"is not a prime number"
                    print i,"times",self.Number//i,"is",self.Number
                    return
            print self.Number,"is a prime number"

threads = [] 
while True: 
    input = long(raw_input("number: ")) 
    if input < 1: 
        break 
 
    thread = PrimeNumber(input) 
    threads += [thread] 
    thread.start() 
 
for x in threads: 
    x.join()