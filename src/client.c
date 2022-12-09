#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/neutrino.h>
#include <sys/iofunc.h>
#include <sys/dispatch.h>
#include <time.h>

#define MAX_STRING_LEN    256
#define TIMER_PULSE_EVENT (_PULSE_CODE_MINAVAIL + 7)
//resources:
//http://www.qnx.com/developers/docs/7.1/#com.qnx.doc.neutrino.lib_ref/topic/c/connectattach.html
//http://www.qnx.com/developers/docs/7.1/#com.qnx.doc.neutrino.lib_ref/topic/m/msgsend.html

//int random_generator();


int main(int argc, char **argv)
{
int coid, status;
char message[MAX_STRING_LEN] = "test";
int incoming_msg, pid;
timer_t timerid;
struct itimerspec it;
struct sigevent event;
int load;

typedef struct
{
   int load;// [0,1,2]  0 = low , 1 = mid = 2 = high
   int state; // 0 if not working , 1 if its active
} client_t;

client_t client_status;

//Create a Connection to Server Channel
coid = name_open("Server Room",0);

//Print client pid
pid = getpid();
printf("My Pid is %d \n", pid);

while(1){

   switch{
      case(state = 1){
         srand(time(NULL));
         load = rand() % 3;
         printf("Random number is %d, \n", load);

   //send the message to the server and get a reply
   status = MsgSend(coid, &load, sizeof(load), &incoming_msg, sizeof(incoming_msg));
   if(incoming_msg == 1){
      load = 0;
      state = 0;
      printf("Switching off \n");
      }
      printf("message is %d \n", incoming_msg);
      printf("Status is %d \n", status);
      }

      case(state = 0){
         //sleep between 1 and 10 seconds and switch state back to 1 and pick a random load
         srand(time(NULL));
         sleep(rand() % 10 + 1);
         printf("Switching back on \n");
         state = 1;
      }
   }
}
return 0;

}
