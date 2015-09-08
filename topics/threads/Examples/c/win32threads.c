#include <windows.h>
#include <stdio.h>

DWORD Cont=0; /* Variable compartida */


DWORD  WINAPI incrementar(LPVOID param) {
  DWORD n = *(DWORD*)param;
  int i;
  for(i=0;i<n;i++)
    Cont=Cont+1;
  return 0;
}

DWORD  WINAPI decrementar(LPVOID param) {
  DWORD n = *(DWORD*)param;
  int i;
  for(i=0;i<n;i++)
    Cont=Cont-1;
  return 0;
}

int main(int argc, char *argv[]) {
    DWORD TIdi,TIdd;
    HANDLE THandlei,THandled;
    int param;
    
    if (argc < 2) {
       fprintf(stderr,"win32threads1 <valor entero>\n");
       return -1;
    }
    param = atoi(argv[1]);

    THandlei = CreateThread(NULL,0,incrementar,&param,0,&TIdi);
    THandled = CreateThread(NULL,0,decrementar,&param,0,&TIdd);
    printf("Contador = %d\n",Cont);
    WaitForSingleObject(THandlei,INFINITE);
    WaitForSingleObject(THandled,INFINITE);
    CloseHandle(THandlei);
    CloseHandle(THandled);
    printf("Contador = %d\n",Cont);
}
