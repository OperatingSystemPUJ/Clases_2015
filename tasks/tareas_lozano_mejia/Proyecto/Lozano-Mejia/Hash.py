import Files 
class FileSystem:
    def __init__(self):
        self.FM= Files.FileManager()
        self.keys=self.FM.HashFiles()
        self.details=self.FM.HashSize()
        self.THash=self.THash={k:v for k, v in zip(self.keys,self.details)}
        self.TContent={}       
        self.Content=[]

        
    def addFile(self,name):
        self.keys.append(name)
        self.FM.AddTxt(name)
        self.details.append(self.FM.Size)
        self.THash={k:v for k, v in zip(self.keys,self.details)}
        self.FileContent()
        self.FM.rewrite(self.THash.keys(),self.THash.values(),self.TContent.values())
         
    def FileContent(self):
        self.Content=self.FM.getFiles()
        self.TContent={k:v for k, v in zip(self.keys,self.Content)}        

    def deleteFile(self,name):
        del self.THash[name]
        del self.TContent[name]
        self.FM.rewrite(self.THash.keys(),self.THash.values(),self.TContent.values())
        self.FileContent()

    def updateFile(self,name):        
        self.TContent[name]=self.FM.getContent(name)
        self.THash[name]=self.FM.Size       
        self.FM.rewrite(self.THash.keys(),self.THash.values(),self.TContent.values())
        
    def printFiles(self):
        print self.TContent.values()
        for i, v in enumerate(self.TContent.keys()):
            print "File",i,"is",v
        for x, y in enumerate(self.TContent.values()):
            print "The content for the file",x,"is",y
            
                
