import subprocess 
import os 
import sys

FILE=[]
f=""  
def dels(s,i):
    ss=""
    k=0
    while k<len(s):
        if i!=k:
            ss+=s[k]
        k+=1
    return ss
def zenitylist(cmd):
    s=subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE).stdout.read()
    return s.split("|")[0]
def question(s):
    cmd="zenity --question --text="
    cmd=cmd+s
    proc=subprocess.Popen(cmd, shell=True ) 
    proc.communicate() 
    if proc.returncode:
        return False
    else:
        return True
def save(cmd=""):
    cmd="zenity --title='guardar archivo' --file-selection --save "+cmd+" $HOME"
    f=subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE).stdout.read()
    return f.split("\n")[0]
def selection():
    cmd="zenity --title='abrir archivo' --file-selection $HOME"
    f=subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE).stdout.read()
    return f.split("\n")[0]

def conver(n):
    m=int(n)
    if int(m/1000000000)!=0:
        return str(m/1000000000)+"GB" 
    if int(m/1000000)!=0:
        return str(m/1000000)+"MB" 
    if int(m/1000)!=0:
        return str(m/1000)+"kB" 
    else:
        return n+"B"      

def add(fl,name):
    a=open(f,'rb+')
    fi=a.readlines()
    a.close()
    i=0
    aa=open(fl,'r')
    aa.read()
    while i<len(fi):
        if fi[i].split("\n")[0]=="/////":
            fi.insert(i,name+"//"+str(aa.tell())+"//\n")
            break
        i=i+1
    aa.seek(0,0)
    for i in aa.readlines():
        fi.append(i)
    aa.close()
    if os.path.exists(f):
        os.remove(f)
    a=open(f,'wb')
    a.writelines(fi)
    a.close()
def dele(fl):
    a=open(f,'rb+')
    fi=a.readlines()
    a.close()
    n=0
    t=0
    b=False
    bb=False
    i=0
    while i<len(fi):
        #print fi
        line=fi[i].split("\n")[0]
        if (not(b) and not(bb)):
            l=line.split("//")
            if fl==l[0]:
                t=int(l[1])
                del fi[i]
                bb=True
                #print "borre"
                continue
            n=n+int(l[1])
        elif line=="/////" and not b:
            #print "fin"
            b=True
        elif b and bb:
            #print "borrando"
            j=0
            while j<len(fi[i]):
                #print fi
                if n>0:
                    n=n-1
                elif t>0:
                    fi[i]=fi[i][:j]+fi[i][j+1:]
                    j=j-1
                    t=t-1
                else:
                    break
                j=j+1
        #print i
        i=i+1
    if os.path.exists(f):
        os.remove(f)
    a=open(f,'w')
    a.writelines(fi)
    a.close()
def ext(fl,fls):
    a=open(f,'rb')
    fi=a.readlines()
    a.close()
    n=0
    t=0
    b=False
    bb=False
    i=0
    fil=[]
    while i<len(fi):
        line=fi[i].split("\n")[0]
        if line=="/////" and not b:
            b=True
            i+=1
            continue
        elif not b and not bb:
            l=line.split("//")
            if fl==l[0]:
                t=int(l[1])
                #fi.pop(i)
                i+=1
                bb=True
                continue
            n=n+int(l[1])
        elif b and bb:
            j=0
            tmp=""
            while j<len(fi[i]):
                if n>0:
                    n-=1
                elif t>0:
                    tmp+=fi[i][j]
                    t-=1
                else:
                    break
                j+=1
            fil.append(tmp)
        i+=1
    if os.path.exists(fls):
        os.remove(fls)
    a=open(fls,'w')
    a.writelines(fil)
    a.close()
if question("'desea crear un archivo o usar un archivo existente?' --cancel-label='crear uno nuevo' --ok-label='abrir archivo'")==False:
    f=save()
    if f=="":
        exit(1)
    if os.path.exists(f):
        os.remove(f)
    a=open(f,'w')
    a.write("/////\n")
    a.close()
else:
    f=selection()
if f=="":
    sys.exit(1)
a=open(f,'r')
q=False
for i in a.readlines():
    if i.split("\n")[0]=="/////":
        q=True
        break
a.close()
if not q:
    os.system("zenity --error --text 'Imposible continuar, archivo es invalido'")
    exit(1)
#     FILE.append(i)
# a.close()
while 1:
    temp="zenity --list --title='opciones' "
    s=zenitylist(temp+"--column opcion 'extraer archivo' 'guardar archivo' 'eliminar archivo'")
    if "\n" in s:
        s=s.split("\n")[0]
    #print s
    if s=="extraer archivo":
        sek=0
        a=open(f,'r')
        l=[]
        line=""
        while True:
            line=a.readline()
            if  not line:
                #exit(1)
                break
            line=line.split("\n")[0]
            #print a.tell()
            if line=="/////":
                break
            tmp=line.split("//")
            l.append(tmp)
        sek=a.tell()
        a.close()
        s=temp+"--column 'nombre archivo' --column 'size archivo' "
        for i in l:
            #tmp=i[1].split("\n")[0]
            m=conver(i[1])
            s=s+" '"+i[0]+"' '"+m+"' "
        s=zenitylist(s)
        if s=="":
            continue
        ss=save("--filename='"+s+"'")
        #print s
        ext(s,ss)
        dele(s)
    if s=="guardar archivo":
        s=selection()
        if s=="":
            continue
        ss=""
        m=len(s)-1
        while m>0:
            if s[m]=="/":
                break
            ss+=s[m]
            m=m-1
        ss=ss[::-1]
        b=True
        while b:
            a=open(f,'r')
            line=""
            while True:
                line=a.readline()
                if  not line:
                    b=False
                    #exit(1)
                    break
                line=line.split("\n")[0]
                if line.split("//")[0]==ss:
                    break
                if line=="/////":
                    b=False
                    break
            if b:
                if question("'existe un archivo con ese nombre, que desea hacer?' --cancel-label='remplazar' --ok-label='crear uno nuevo'")==False:
                    b=False
                    dele(ss)
                else:
                    ss=zenitylist("zenity --entry --title='nombre archivo'").split("\n")[0]
                    #print ss
            a.close()
        add(s,ss)
    if s=="eliminar archivo":
        sek=0
        a=open(f,'r')
        l=[]
        line=""
        while True:
            line=a.readline()
            if  not line:
                #exit(1)
                break
            line=line.split("\n")[0]
            #print a.tell()
            if line=="/////":
                break
            tmp=line.split("//")
            l.append(tmp)
        sek=a.tell()
        a.close()
        s=temp+"--column 'nombre archivo' --column 'size archivo' "
        for i in l:
            #tmp=i[1].split("\n")[0]
            m=conver(i[1])
            s=s+" '"+i[0]+"' '"+m+"' "
        s=zenitylist(s)
        if s=="":
            continue
        dele(s)
    if s=="":
        break

        


    
