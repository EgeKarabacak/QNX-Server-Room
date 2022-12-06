#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/neutrino.h>
#include <sys/iofunc.h>
#include <sys/dispatch.h>

#define MAX_STRING_LEN    256

//resources:

//http://www.qnx.com/developers/docs/7.1/#com.qnx.doc.neutrino.lib_ref/topic/m/msgsend.html
//http://www.qnx.com/developers/docs/7.1/#com.qnx.doc.neutrino.lib_ref/topic/n/name_open.html
//http://www.qnx.com/developers/docs/7.1/#com.qnx.doc.neutrino.lib_ref/topic/n/name_close.html


int main(int argc, char **argv)
{
	//Declare variables needed
	int rcvid, coid, reply;
	char msg[MAX_STRING_LEN +1];
	char *server_name = "server";
	
	//set the message value to 'test'
	strcpy(msg, "test");
	//establish a connection to the server's channel
	
	coid = name_open(server_name, 0);

	//send the message to the server and get a reply
	rcvid = MsgSend(coid, msg, sizeof(msg), &reply, sizeof(reply));

    //print received checksum
	printf("Received checksum: %d\n", reply);

    //print returned status
	printf("Returned Status: %d\n", rcvid);

    //send a pulse to the server
	MsgSendPulse(coid, 0, 2, 100);

    //Close a server connection that was opened by name_open()
	name_close(coid);

	return 0;
}
