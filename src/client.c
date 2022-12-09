#include <stdio.h>
#include <stdlib.h>
#include <sys/neutrino.h>
#include <process.h>
#include <errno.h>
#include <sys/netmgr.h>
#include <string.h>
#include <sys/iofunc.h>
#include <sys/dispatch.h>

#define MAX_STRING_LEN    256
#define ARR_SIZE 10

int duplicate(int*, int*, int);

int duplicate(int *arr, int* arr2, int size){
// Input elements of array
int is_duplicate;

// Check for duplicates
for(int i=0; i<size; i++){

   for(int j=i+1; j<size; j++){

       if(arr[i] == arr[j]){
        arr[i] = arr[j];
        arr2[i] = arr2[j];
           is_duplicate = 1;
           size-=1;
           break;
       }
   }
}

// Print result
if(is_duplicate == 1)
{
   printf("Array contains duplicate elements.\n");
}
else
{
   printf("Array does not contain duplicate elements.\n");
}

return size;


}

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
//printf("Overload occurred\n");
pidArray[index] = info.pid;
loadArray[index] = msg.load;
index+=1;
index = duplicate(pidArray, loadArray, index);

        printf("New index is %d \n", index);

        for(int i = 0; i < index; i++){
        printf("Client %d's Pid is %d \n",i+1, pidArray[i]);
        printf("Client %d's load is  %d \n",i+1, loadArray[i]);
        }

        //do room management here
         if(index>5){ // if we have over 5 clients in the room
         printf("We have more than 5 clients in the room \n");
         //we calculate the average load.
         for(int i=0; i<index; i++){
                if(loadArray[i] == 2){
                    avg_load += 2;
                }
                if(loadArray[i] == 1){
                    avg_load += 1;
                }
                else{
                    avg_load += 0;
                }

                if(avg_load >= 2*index - 2 && avg_load > 8){
                //send message to client to swithc it state to idle = 0
                printf("We have overload in the room \n");
                overload = 1;
                MsgReply(rcvid, EOK, &overload, sizeof(overload));
                overload = 0;
                for(int i = 0; i < index; i++){
                    loadArray[index] = 0;
                    avg_load = 0;
                }
            }

          }
        }
       }



    return 0;
}