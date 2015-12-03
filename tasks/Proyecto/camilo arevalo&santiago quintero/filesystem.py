import subprocess 
import os 
import sys

# File to use as archive for the filesystem
FILE = []
f = "" 

# Displays a zeniti message dialog
def message(text, m_type="info"):
    cmd = 'zenity --' + m_type + ' --text="' + text + '"'
    os.system(cmd)

# Displays a gtk+ dialog prompt for user text input
def text_entry(title):
    cmd = 'zenity --entry --title="' + title + '"'
    try:
        return subprocess.check_output(cmd, shell=True).strip()
    except subprocess.CalledProcessError:
        return ""

# Displays a gtk+ dialog list widget with title, headers, and listing elements
def zenitylist(title, headers, elements):
    cmd = 'zenity --list --title="' + title + '"'
    for item in headers:
        cmd += ' --column="' + item + '"'
    cmd += ' ' + elements
    try:
        return subprocess.check_output(cmd, shell=True).strip().split('|')[0]
    except subprocess.CalledProcessError:
        return ""

# Displays a gtk+ question dialog, returns True for confirmation, False otherwise
def question (q, negative="", positive=""):
    cmd = 'zenity --question --text="' + q + '"'
    if negative != "":
        cmd += ' --cancel-label="' + negative + '"'
    if positive != "":
        cmd += ' --ok-label="' + positive + '"'
    if subprocess.call(cmd, shell=True) == 0:
        return True
    return False

# Promps the user with a file selection dialog for saving a file
def save(cmd=""):
    cmd = 'zenity --file-selection --save --title="guardar archivo" ' + cmd + ' $HOME'
    try:
        return subprocess.check_output(cmd, shell=True).strip()
    except subprocess.CalledProcessError:
        return ""

# Promps the user with a file selection dialog for choosing a file
def selection():
    cmd = 'zenity --file-selection $HOME --title="abrir archivo"'
    try:
        return subprocess.check_output(cmd, shell=True).strip()
    except subprocess.CalledProcessError:
        return ""

# Convert a number n bytes to GB, MB, kB or B
def conver(n):
    m = int(n)
    if m >= 1000000000:
        return str(m/1000000000)+"GB" 
    if m >= 1000000:
        return str(m/1000000)+"MB" 
    if m >= 1000:
        return str(m/1000)+"kB" 
    else:
        return n+"B" 

# Adds a file from file_path content into the archive with name
def add(file_path, name):
    archive = open(f,'rb+')
    archive_content = archive.readlines()
    archive.close()

    i = 0
    new_file = open(file_path,'r')
    new_file.read()

    # Add the new file name to the list
    while i < len(archive_content):
        if archive_content[i].rstrip() == "/////":
            archive_content.insert(i,name + "//" + str(new_file.tell()) + "//\n")
            break
        i += 1

    # Add the new file contents to the archive
    new_file.seek(0,0)
    for i in new_file.readlines():
        archive_content.append(i)
    new_file.close()
    if os.path.exists(f):
        os.remove(f)
    archive=open(f,'wb')
    archive.writelines(archive_content)
    archive.close()

# deletes a file_name name and contents from the archive
def dele(file_name):
    archive = open(f,'rb+')
    archive_content = archive.readlines()
    archive.close()
    
    n = 0
    t = 0
    index_read = False
    found = False
    i = 0
    while i < len(archive_content):
        #print archive_content
        line = archive_content[i].rstrip()
        if (not(index_read) and not(found)):
            l = line.split("//")
            if file_name == l[0]:
                t = int(l[1])
                del archive_content[i]
                found = True
                #print "borre"
                continue
            n = n + int(l[1])
        elif line=="/////" and not index_read:
            #print "fin"
            index_read=True
        elif index_read and found:
            #print "borrando"
            j=0
            while j<len(archive_content[i]):
                #print archive_content
                if n > 0:
                    n = n - 1
                elif t > 0:
                    archive_content[i] = archive_content[i][:j] + archive_content[i][j+1:]
                    j -= 1
                    t -= 1
                else:
                    break
                j += 1
        #print i
        i += 1
    if os.path.exists(f):
        os.remove(f)
    archive = open(f,'w')
    archive.writelines(archive_content)
    archive.close()

# Extracts the file_name content from the archive to destinatio_path
def ext(file_name, destinatio_path):
    archive = open(f,'rb')
    archive_content = archive.readlines()
    archive.close()
    n = 0
    t = 0
    index_read = False
    found = False
    i = 0
    file_content = []
    while i<len(archive_content):
        line = archive_content[i].rstrip()
        if line=="/////" and not index_read:
            index_read = True
            i += 1
            continue
        elif not index_read and not found:
            l = line.split("//")
            if file_name == l[0]:
                t = int(l[1])
                #archive_content.pop(i)
                i += 1
                found = True
                continue
            n = n + int(l[1])
        elif index_read and found:
            j = 0
            tmp = ""
            while j < len(archive_content[i]):
                if n > 0:
                    n -= 1
                elif t > 0:
                    tmp += archive_content[i][j]
                    t -= 1
                else:
                    break
                j += 1
            file_content.append(tmp)
        i += 1
    if os.path.exists(destinatio_path):
        os.remove(destinatio_path)
    archive=open(destinatio_path,'w')
    archive.writelines(file_content)
    archive.close()

# Main program

# Create a existing archive or create a new one
if not question('Desea crear un archivo o usar un archivo existente?', 'crear uno nuevo', 'abrir archivo'):
    f = save()
    if f == "":
        exit(1)
    if os.path.exists(f):
        os.remove(f)
    archive = open(f,'w')
    archive.write("/////\n")
    archive.close()
else:
    f = selection()

# Check if a file was selected or created
if f == "":
    sys.exit(1)

archive = open(f,'r')
index_read = False
for i in archive.readlines():
    if i.rstrip() == "/////":
        index_read = True
        break
archive.close()

# Check if the index was read
if not index_read:
    message("Imposible continuar, archivo es invalido", "error")
    exit(1)
#     FILE.append(i)
# archive.close()

# Since the file is somewhat valid run the main interface
while 1:
    temp = ""
    s = zenitylist("Opciones", ['Opcion'], '"extraer archivo" "guardar archivo" "eliminar archivo"')
    #print s
    if s == "extraer archivo":
        sek = 0
        archive = open(f,'r')
        l = [] # list for the index of the archive
        line = ""
        # Read the index of the archive
        while True:
            line = archive.readline()
            if not line:
                #exit(1)
                break
            line = line.rstrip()
            #print archive.tell()
            if line == "/////":
                break
            tmp = line.split("//")
            l.append(tmp) # pair of file_name, file_size
        sek = archive.tell() # Pos of archive index end
        archive.close()

        # List the files in the index of the archive
        for i in l:
            #tmp=i[1].split("\n")[0]
            m = conver(i[1])
            temp += ' "' + i[0] + '" "' + m + '"'
        s = zenitylist("Archivos", ['nombre archivo', 'size archivo'], temp)

        # check if user choose a file
        if s == "":
            continue

        # Prompt the user to select a destination_path ss
        ss = save('--filename="' + s + '"')
        #print s
        if ss == "":
            continue

        # Extract the selected file from the archive to ss
        ext(s, ss)
        # Delete the extraxted file from the archive
        dele(s)
    elif s == "guardar archivo":
        # Prompt the user to select a file
        s = selection()
        # Check if the user selected a fil
        if s == "":
            continue

        # Extract the filename from the path
        ss = ""
        m = len(s) - 1
        while m > 0:
            if s[m] == "/":
                break
            ss += s[m]
            m = m - 1
        ss = ss[::-1]

        # Check if the file already exists in the archive
        b = True
        while b:
            archive = open(f,'r')
            line = ""
            while True:
                line = archive.readline()
                if not line:
                    b = False
                    #exit(1)
                    break
                line = line.rstrip()
                if line.split("//")[0] == ss:
                    break
                if line == "/////":
                    b = False
                    break
            if b:
                if not question('existe un archivo con ese nombre, que desea hacer?', 'remplazar', 'crear uno nuevo'):
                    b = False
                    dele(ss)
                else:
                    ss = text_entry('Nombre archivo')
                    #print ss
            archive.close()
        # Add the file contents to the archive
        add(s,ss)
    elif s == "eliminar archivo":
        sek = 0
        archive = open(f,'r')
        l = [] # list for the index of the archive
        line = ""
        # Read the index of the archive
        while True:
            line = archive.readline()
            if not line:
                #exit(1)
                break
            line = line.rstrip()
            #print archive.tell()
            if line == "/////":
                break
            tmp = line.split("//")
            l.append(tmp) # pair of file_name, file_size
        sek = archive.tell() # Pos of archive index end
        archive.close()

        # List the files in the index of the archive
        for i in l:
            #tmp=i[1].split("\n")[0]
            m = conver(i[1])
            temp += ' "' + i[0] + '" "' + m + '"'
        s = zenitylist("Archivos", ['nombre archivo', 'size archivo'], temp)

        # check if user choose a file
        if s == "":
            continue
        # Delete the file from the archive
        dele(s)
    if s=="":
        break # End program
