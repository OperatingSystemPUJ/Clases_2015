import Hash

def Pry():
    Proyecto=Hash.FileSystem()
    #Proyecto.addFile("txt1.txt")
    #Proyecto.addFile("txt2.txt")
    Proyecto.FileContent()
    #Proyecto.updateFile("Archivo1")
    command=''
    while True:
        command=input("Enter your Command: ")
        if command is "show":
            Proyecto.printFiles()
        elif (command is "delete"):
            filetmp=input("Enter the file name:")
            Proyecto.deleteFile(filetmp)
            print "The file was deleted succesfully."
        elif (command is "update"):
            filetmp=input("Enter the file name:")
            Proyecto.updateFile(filetmp)
            print "The file was updated succesfully."
        elif (command is "add"):
            filetmp=input("Enter the file name:")
            Proyecto.addFile(filetmp)
            print "The file was added succesfully."
        elif (command is "help"):
            print "This is a FileSystem simulator 2015. \n"
            print "The commands avaliable are: \n"
            print "add, delete, update, show and quit \n"
        elif (command is "quit"):
            return "Good Bye"            
        else:
            print "Invalid command, type help for more information"
            
    
Pry()          
              
    
    #Proyecto.deleteFile("Archivo1")
    #Proyecto.THash["Archivo1"]
##    print Proyecto.keys
##    print Proyecto.details
##    print Proyecto.THash.keys()
##    print Proyecto.THash.values()
    #del Proyecto.TContent['Archivo1']
    #print Proyecto.TContent
