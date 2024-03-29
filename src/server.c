#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/neutrino.h>
#include <sys/iofunc.h>
#include <sys/dispatch.h>
#include <time.h>


void poweron();

void poweron(){
sleep(3); // Sleep for 3 seconds
//val = 1;
printf("Power on Succcessful \n");
//load2 = rand() % 3;

}

int main(int argc, char **argv)
{
int coid, status,pid,status2,load2;
int incoming_msg = 0;
int load;
int state;

typedef struct
{
   int load;// [0,1,2]  0 = low , 1 = mid = 2 = high
   int state; // 0 if not working , 1 if its active
} client_t;


//Create a Connection to Server Channel
coid = name_open("Server Room",0);

//Print client pid
pid = getpid();
printf("My Pid is %d \n", pid);

srand(time(NULL));
load = rand() % 3;
printf("Random number is %d, \n", load);


//send the message to the server and get a reply
status = MsgSend(coid, &load, sizeof(load), &incoming_msg, sizeof(incoming_msg));
printf("message is %d \n", incoming_msg);
printf("Status is %d \n", status);

if(incoming_msg == 1){
printf("Received an overload notification, powering off \n");
state = 0;
poweron(state);
load2 = rand() % 3;
printf("new load is %d \n",load2);
status2 = MsgSend(coid, &load2, sizeof(load2), &incoming_msg, sizeof(incoming_msg));
printf("Status2 is %d \n", status2);

}



return 0;
}