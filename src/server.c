#include <stdio.h>
#include <stdlib.h>
#include <sys/neutrino.h>
#include <process.h>
#include <errno.h>
#include <sys/netmgr.h>
#include <string.h>
#include <sys/iofunc.h>
#include <sys/dispatch.h>

//resources:
//http://www.qnx.com/developers/docs/7.1/#com.qnx.doc.neutrino.lib_ref/topic/c/channelcreate.html
//http://www.qnx.com/developers/docs/7.1/#com.qnx.doc.neutrino.lib_ref/topic/m/msgreceive.html
//http://www.qnx.com/developers/docs/7.1/#com.qnx.doc.neutrino.lib_ref/topic/m/msgreply.html
#define MAX_STRING_LEN    256
#define ARR_SIZE 10

void print_pid(int arr[]);

int main(void) {
    int chid, pid, rcvid, myMsgReply;
    name_attach_t *attach;
    struct _msg_info info;
    int checksumMsg =0;

    typedef union
    {
        struct _pulse pulse;
        char rmsg [MAX_STRING_LEN +1];
    } myMessage_t;

     typedef struct
    {
        int load;// [0,1,2]  0 = low , 1 = mid = 2 = high
        int state; // 0 if not working , 1 if its active
    } server_t;

    myMessage_t msg;

    //Create a Channel
    attach = name_attach(NULL, "Server Room", 0);

    while(1){
        rcvid = MsgReceive(attach->chid, &msg.rmsg, sizeof(msg.rmsg), &info);

        //Print Rcvid
        printf("Rcvid: %d \n", rcvid);
        printf("Client pid is %d \n", info.pid);
        //print_pid(pidArray);
        myMsgReply = MsgReply(rcvid, EOK, &checksumMsg, sizeof(checksumMsg));
    }

//    }

    return 0;
}
