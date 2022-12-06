//Ege Karabacak A8

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/neutrino.h>
#include <sys/dispatch.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <string.h>

//Resources
//http://www.qnx.com/developers/docs/7.1/#com.qnx.doc.neutrino.lib_ref/topic/c/channelcreate.html
//http://www.qnx.com/developers/docs/7.1/#com.qnx.doc.neutrino.lib_ref/topic/c/connectattach.html



#define TIMER_PULSE_EVENT (_PULSE_CODE_MINAVAIL + 7)

/* union of all different types of message(s) we will receive (for this
 * exercise we will only be receiving pulses)
 */
typedef union
{
	struct _pulse pulse;
} message_t;

char *progname = "timer";

int main(int argc, char *argv[])
{
	int rcvid;
	struct sigevent event;
	int chid, coid;
	message_t msg;
	timer_t timerid;
	struct itimerspec it;

	// create a channel
	chid = ChannelCreate(0);
	if (chid == -1){
		perror("ChannelCreate");
		exit(EXIT_FAILURE);
	}


    //establish a connection to the channel
	coid = ConnectAttach(0, 0, chid, _NTO_SIDE_CHANNEL, 0);


	/* set up the pulse event that will be delivered to us by the kernel
	 * whenever the timer expires  (USE the MACRO SIGEV_PULSE_INIT)
	 */
	SIGEV_PULSE_INIT(&event, coid, SIGEV_PULSE_PRIO_INHERIT, TIMER_PULSE_EVENT, 0);


	/* Create a timer which will send the above pulse event
	 * 5 seconds from now and then repeatedly after that every
	 * 1500 milliseconds.  The event to use has already been filled in
	 * above and is in the variable called 'event'.
	 */
	timer_create(CLOCK_REALTIME, &event, &timerid);
	
	it.it_value.tv_sec = 5;
	it.it_value.tv_nsec = 0;
	
	it.it_interval.tv_sec = 1.5;
	it.it_interval.tv_nsec = 0;
	
	timer_settime(timerid, 0, &it, NULL);


	while (1)
	{
		/* wait here for the pulse message
		 * if we got a pulse check if its out timer pulse and print got our pulse */
		rcvid = MsgReceive(chid, &msg, sizeof(msg), NULL);
		if (rcvid == 0){
			if (msg.pulse.code == TIMER_PULSE_EVENT){
				printf("Got our pulse \n");
			}
		}
	}
}
