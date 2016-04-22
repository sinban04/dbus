#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include <dbus/dbus.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <signal.h>

#include <time.h>
#include <sys/time.h>


static DBusConnection *conn = NULL;

int main()
{
	DBusMessage *msg;
	DBusMessage *reply;
	DBusError error;
	char *msg_str = "aaaaaaa";
	char *msg_in;

	struct timeval start, end;

//	gettimeofday(&start, NULL);

	dbus_error_init(&error);
	conn = dbus_bus_get(DBUS_BUS_SYSTEM, &error);

	if(dbus_error_is_set(&error)){
		printf("Error connecting to the daemon bus %s", error.message);
		dbus_error_free(&error);
	}
	dbus_connection_set_exit_on_disconnect(conn, FALSE);
	


	msg = dbus_message_new_method_call("org.eslab.injung",
			"/",
			"org.eslab.injung",
			"Get");
	if(NULL == msg){
		printf("Message NULL\n");
		exit(1);	
	}
	dbus_message_append_args(msg,
			DBUS_TYPE_STRING, &(msg_str),
			DBUS_TYPE_INVALID);

	dbus_error_init(&error);

	if(conn == NULL){
		printf("Why null? \n");
		conn = dbus_bus_get(DBUS_BUS_SYSTEM, &error);
	}
	gettimeofday(&start,NULL);
	reply = dbus_connection_send_with_reply_and_block(conn, msg, 500, &error);

	if(reply == NULL){
		printf("Get NULL reply\n");
		printf("Error: %s\n", error.message);
	}
	dbus_error_free(&error);

	dbus_message_unref(msg);

	dbus_message_get_args(reply,NULL,
			DBUS_TYPE_STRING,&(msg_in),
			DBUS_TYPE_INVALID);
	
	dbus_message_unref(reply);

//	printf("Received message: %s\n",msg_in);
	
	gettimeofday(&end, NULL);
	printf("%ld\n", ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec)));

	return 0;

}
