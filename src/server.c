#include <stdio.h>
#include <stdlib.h>
#include <sys/neutrino.h>
#include <sys/iofunc.h>
#include <sys/dispatch.h>


//resources:

//http://www.qnx.com/developers/docs/7.1/#com.qnx.doc.neutrino.lib_ref/topic/m/msgreceive.html
//http://www.qnx.com/developers/docs/7.1/#com.qnx.doc.neutrino.lib_ref/topic/m/msgreply.html
//http://www.qnx.com/developers/docs/7.1/#com.qnx.doc.neutrino.lib_ref/topic/n/name_attach.html
#define MAX_STRING_LEN    256


int calculate_checksum(char *text);

int main(void) {

	typedef union
	{
		struct _pulse pulse;
	    char rmsg [MAX_STRING_LEN +1];
	} myMessage_t;


	
	//attach a name to the channel
	name_attach_t *attach = name_attach(NULL, "server", 0);
	int chid = attach->chid;
	
	//create a message
	myMessage_t msg;

    //the server should keep receiving, processing and replying to messages
	while(1)
	{
		//code to receive msg or pulse from client
		int rcvid = MsgReceive(chid, &msg, sizeof(msg), NULL);

		//print the returned value of MsgReceive
		printf("MsgReceive returned %d\n", rcvid);

		//check if it was a pulse or a message
		// if it is a pulse -> check the pulse code to see if the client is gone/disconnected and print (client is gone)  else if the pulse is something else print the code and value of the pulse

	  if (rcvid == 0)
	  {
		  //check pulse type
		  if (msg.pulse.code == _PULSE_CODE_DISCONNECT)
		  {
			 
			  printf("Client is gone\n");
			  
	
		  }
		  printf("Pulse received: %d\n", msg.pulse.code);
	  }

		// if it was a message
	  else
	  {
		  //print received message
		  printf("Message received: %s\n", msg.rmsg);
		  
		  //calculate checksum by calling calculate_checksum
		  int checksum = calculate_checksum(msg.rmsg);

		  //reply to client with checksum
		  MsgReply(rcvid, 0, &checksum, sizeof(checksum));

		  //print the return value of MsgReply
		  printf("MsgReply returned: %d\n", rcvid);
	  }

	}
		//remove the name from the namespace and destroy the channel
		name_detach(attach, 0);
		ChannelDestroy(chid);

return 0;
}




int calculate_checksum(char *text)
{
	char *c;
	int cksum = 0;

	for (c = text; *c != '\0'; c++)
		cksum += *c;
	return cksum;
}
