##from PIL import Image
##import numpy as np
import os
class FileManager:
    def __init__(self):
        self.PosE=0 #Position after writing
        self.PosI=0 #Position before writing
        self.Size=0
        self.File="FileSys.txt" 
    #Add txt to the Mem
    def AddTxt(self,In):
        self.Size=str(os.path.getsize(In))
        s=open(self.File,'a')        
        f=open(In,'r+')        
        #old=s.read()
        self.PosI=s.tell()
        s.write('\n') 
        s.write(f.read()+'\n'+'%\n')        
        self.PosE=s.tell()
        s.close()
        f.close()
        
    def getFiles(self):
        s=open(self.File,'r+')
        tmp=s.read()
        split=tmp.split('%')        
        del split[0]
        return split    
        
    def HashFiles(self):
        f=open(self.File,'r+')        
        Files=f.readline().split('|')
        for i in range(0,len(Files)):
            Files[i]=Files[i].strip('\n')
            Files[i]=Files[i].strip()
        f.close()
        return Files
    
    def HashSize(self):
        f=open(self.File,'r+')
        Sizes=f.readline()
        Sizes=f.readline().split('|')
        for i in range(0,len(Sizes)):
            Sizes[i]=Sizes[i].strip('\n')
            Sizes[i]=Sizes[i].strip()
        f.close()
        return Sizes
    
    def rewrite(self,names,details,content):
        os.remove(self.File)
        f=open(self.File,'w')
        #Header
        for x in range(0,len(names)-1):
            f.write(names[x]+'|')        
        f.write(names[x+1])
        f.write('\n')
        #Details        
        for y in range(0,len(details)-1):
            f.write(details[y]+'|')
        f.write(details[y+1])
        f.write('\n')
        f.write('%')
        for i in content:
            f.write(i+'%')
        f.close()

    def getContent(self,name):
        self.Size=str(os.path.getsize(name))        
        f=open(name,'r')
        content=f.read()
        f.close()
        return content
        
        
                  
##    def AddImg(In,Mem):
##        s=open(Mem,'w')
##        image = Image.open(source)           
##        img = np.array(image, dtype=np.float)
##        img.tolist()
##        s.write(img)
##        self.Pos=s.tell
##        s.close()
##        
        
        
        
        
    
