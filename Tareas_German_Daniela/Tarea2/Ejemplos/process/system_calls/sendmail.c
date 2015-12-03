#include <stdio.h>
#include <stdlib.h>

int main(void) {
    FILE *in;
    extern FILE *popen();
    char buff[512];

    if(!(in = popen("echo \"This is the message body and contains the message\" | mailx -v -r \"<<<yourusername>>>@gmail.com\" -s \"This is the subject\" -S smtp=\"smtp.gmail.com:587\" -S smtp-use-starttls -S smtp-auth=login -S smtp-auth-user=\"<<<yourusername>>>@gmail.com\" -S smtp-auth-password=\"<<<yourpassword>>>\" -S ssl-verify=ignore <<<destination>>>@gmail.com", "r"))){
        exit(1);
    }

    while(fgets(buff, sizeof(buff), in)!=NULL){
        printf("%s", buff);
    }
    pclose(in);

}