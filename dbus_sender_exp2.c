#include <stdio.h>
#include <stdlib.h>
#include <dbus/dbus.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <glib.h>

#define ESLAB_INTERFACE "/org/eslab/injung"
#define ESLAB_PATH "org.eslab.injung"

static DBusConnectino *Conn = NULL;  //Only 1 Connection
static char eslabPath[100];
static char eslabInterface[100];
/*
DBusHandlerResult replyGet(DBusConnection *connection, DBusMessage *message, void *iface_user_data)
{
	DBusError err;

	dbus_error_init(&err);

	dbus_message_get_args(message, &err,
			DBUS_TYPE_STRING, &reply


}

*/
static DBusHandlerResult dbus_response(DBusConnection *connection, DBusMessage *message, void *user_data)
{
	if(dbus_message_is_signal(message, eslabInterface, "GetReply")){
		return replyGet(connection, message, user_data);
	}

	return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
}



void pid_to_char(*output)
{
	pid_t pid = getpid();

	unsigned int value = (unsigned int) pid;
	unsigned int temp;

	output[5] = NULL;

	for(int i=4; i>=0; i--)
	{
		temp = value % 10;
		output[i] = ((char)(temp)) + 65;
		value = value / 10;
	}
	printf("Convert: from %u to %s \n", (unsigned int)pid, output);

}



DBusConnection *DbusInit()
{
	DBusConnection *connection;
	DBusError error;
	
	dbus_error_init(&error);

	char total_address[100];
	const char *address1 = "type='signal', interface='org.eslab.';
	char address2[6];
	const char *address3 = "'";

	printf(" Start dbus initialzing \n");

	pid_to_char(address2);
	
	sprintf(total_address, "%s%s%s", address1, address2, address3);
	printf("Total address: %s\n", total_address);

	sprintf(eslabPath, "/org/eslab/%s", address2);
	sprintf(eslabInterface, "org.eslab.%s", address2);

	connection = dbus_bus_get(DBUS_BUS_SYSTEM, &error);

	if(dbus_error_is_set(&error))
	{
		printf("Error connecting to the daemon bus: %s", error.message);
		dbus_error_free(&error);
	
	}
	dbus_connection_set_exit_on_disconnect(connection, FALSE);

	/*
	//if(!eslab_DbusConnection(connection)){
	//	printf("Failed to initialize connection \n");
	//
	//}
	*/

	
	dbus_bus_add_match(connection, total_address, NULL);    //set bus position
	dbus_connection_flush(connection);

	dbus_connection_add_filter(connection, dbus_response, NULL, NULL);		//register dbus message handling fucntion "dbus_response"

	printf("Dbus initializing complete\n");

	return connection;

}


int main()
{
	Conn = DbusInit();
	char *str;
	char *reply_str;



	// Dbus "Get" Start !!!!!!!   
	
	DBusMessage *msg;
	DBusMessage *reply;
	DBusError error;	
	
	dbus_error_init(&error);
	


	msg = dbus_message_new_method_call("org.eslab.injung",   //Target for the method Call
			"/", //object to call on
			"org.eslab.injung",   //interface to call on
			"Get");

	if(msg == NULL){
		printf("Message NULL\n");
	}

	str = "Hello Are you there?";
	dbus_message_append_args(msg,
			DBUS_TYPE_STRING, &(str),
			DBUS_TYPE_INVALID);

	//send message and get a reply
	if( Conn == NULL){
		printf("why null?\n");
		Conn = dbus_bus_get(DBUS_BUS_SYSTEM, &error);
	}

	reply = dbus_connection_send_with_reply_and_block(Conn, msg, 500, &error);
	if(reply == NULL)
	{
		printf("Get Null Reply\n");
		printF("Error: %s\n", error.message);
	}

	dbus_error_free(&error);
	dbus_message_unref(msg);

	dbus_message_get_args(reply, NULL,
			DBUS_TYPE,STRING, &(reply_str),
			DBUS_TYPE_INVALID);

	dbus_message_unref(reply);

	printf("receive: %s\n", reply_str);

	printf("End!\n");

	return 0;

}



