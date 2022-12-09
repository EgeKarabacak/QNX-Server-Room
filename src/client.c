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

srand(time(NULL));
//for(int i = 0; i < 10; i++){
load = rand() % 3;
printf("Random number is %d, \n", load);



/* set up the pulse event that will be delivered to us by the kernel
* whenever the timer expires  (USE the MACRO SIGEV_PULSE_INIT)
*/
SIGEV_PULSE_INIT(&event, coid, SIGEV_PULSE_PRIO_INHERIT, TIMER_PULSE_EVENT, &timerid);

/* Create a timer which will send the above pulse event
* 5 seconds from now and then repeatedly after that every
* 1500 milliseconds(1.5 secs).  The event to use has already been filled in
* above and is in the variable called 'event'.
*/

// timer_create(CLOCK_REALTIME, &event, &timerid);
// it.it_value.tv_sec = 5;
// it.it_value.tv_nsec = 0;
// it.it_interval.tv_sec = 1;
// it.it_interval.tv_nsec = 500000000;
// timer_settime(timerid, 0, &it, NULL);





//send the message to the server and get a reply
status = MsgSend(coid, &load, sizeof(load), &incoming_msg, sizeof(incoming_msg));
if(incoming_msg == 1){
   load = 0;
   state = 0;
   }
printf("message is %d \n", incoming_msg);
printf("Status is %d \n", status);



return 0;
}
