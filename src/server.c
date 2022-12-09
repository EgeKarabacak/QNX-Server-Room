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
    int pidArray[ARR_SIZE];
    int loadArray[ARR_SIZE];
    int avg_load;
    int index = 0;
    int overload = 0;

    typedef union
    {
        struct _pulse pulse;
        char rmsg [MAX_STRING_LEN +1];
        int load;
    } myMessage_t;

     typedef struct
    {
        int load;// [0,1,2]  0 = low , 1 = mid = 2 = high
    } server_t;

    myMessage_t msg;

    //Create a Channel
    attach = name_attach(NULL, "Server Room", 0);

    while(1){
        rcvid = MsgReceive(attach->chid, &msg, sizeof(msg), &info);

        //Print Rcvid
        printf("Rcvid: %d \n", rcvid);
        printf("Client pid is %d \n", info.pid);
        printf("Client load is %d \n", msg.load);
        //add client pid to array
        pidArray[index] = info.pid;
        loadArray[index] = msg.load;
        index+=1;
        printf("New index is %d \n", index);

        for(int i = 0; i < index; i++){
        printf("Pid is %d \n", pidArray[i]);
        printf("load is  %d \n", loadArray[i]);
        }


        //do room management here
         if(index>5){ // if we have over 5 clients in the room
         printf("We have more than 5 clients in the room \n");
         //we calculate the average load.
         for(int i=0; i<sizeof(pidArray)/sizeof(int); i++){
                if(loadArray[i] == 2){
                    avg_load += 2;
                }
                if(loadArray[i] == 1){
                    avg_load += 1;
                }
                else{
                    avg_load += 0;
                }
    
            if(avg_load >= 2*index - 1 && avg_load > 8 ){
                //send message to client to swithc it state to idle = 0
                printf("We have overload in the room \n");
                overload = 1;
                MsgReply(rcvid, EOK, &overload, sizeof(overload));
                overload = 0;
                for(int i = 0; i < index; i++){
                    loadArray[index] == 0;
                    avg_load = 0;
                }
            }

            }
        }
        //myMsgReply = MsgReply(rcvid, EOK, &checksumMsg, sizeof(checksumMsg));
    }


    return 0;
}