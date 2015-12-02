from PIL import Image
import numpy as np
import os
def split(Entry):
    strtmp=''
    FContent=[]
    for i in range (0,len(Entry)):
        if Entry[i]!='|':
            strtmp=strtmp+Entry[i]
        elif Entry[i]=='|':
            FContent.append(strtmp)
            strtmp=''
    return FContent

def HashFiles(Mem):
    f=open(Mem,'r+')
    Files=f.readline()
    Files=split(Files)
    f.close()
    return Files
    
def readtxt(file):
    f=open(file,'r')
    s=open("resultado.txt",'w')
    return f.read()
    s.seek(2)
    for line in f:
        #print "Linea:", line
        s.write(line)
    print type(s.tell())
    print type(os.path.getsize(file))    
    f.close()
    
def AddImg(In):
        s=open("resultado.txt",'w')
        image = Image.open(In)           
        img = np.array(image, dtype=np.float)
        img.tolist()
        s.write(img)
        
        s.close()
