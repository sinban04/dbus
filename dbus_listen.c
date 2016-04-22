#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include <dbus/dbus.h>
#include <dbus/dbus-glib.h>
#include <dbus/dbus-glib-lowlevel.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>


#define ESLAB_BUS_NAME "org.eslab.injung"

#define dbusAddress "type='signal', interface='eslab.skku.injung'"

#define ESLAB_INTERFACE "org.eslab.injung"

DBusConnection *connection;



static dataGet(DBusConnection *connection, DBusMessage *message, void *user_data)
{
	DBusMessage *reply;
	char *message_in;
	char *message_out;

	dbus_message_get_args(message, NULL,
			DBUS_TYPE_STRING, &(message_in),
			DBUS_TYPE_INVALID);

//	printf("receive: %s \n", message_in);

	
	reply = dbus_message_new_method_return(message);
	if(reply == NULL)
		printf("Reply Null Error!\n");

	message_out = "yes, thank you";
	dbus_message_append_args(reply,
			DBUS_TYPE_STRING, &(message_out),
			DBUS_TYPE_INVALID);

	dbus_connection_send(connection, reply, NULL);


	return DBUS_HANDLER_RESULT_HANDLED;

}


static DBusHandlerResult introspect(DBusConnection *connection, DBusMessage *message, void *iface_user_data){
	DBusMessage *reply;
	char *instrospect = (char *)malloc(2048);
	char *s_data = (char *)malloc(512);

	FILE *fp = fopen("introspect", "r");
	fgets(instrospect, 1024, fp);

	while (fgets(s_data, 512, fp) != NULL) {
		strcat(instrospect, s_data);
	}
	printf("Intropect file read = %d bytes\n", strlen(instrospect));

	reply = dbus_message_new_method_return(message);
	if (reply == NULL)
		printf("Reply Null Error!\n");

	dbus_message_append_args(reply, DBUS_TYPE_STRING, &instrospect, DBUS_TYPE_INVALID);
	dbus_connection_send(connection, reply, NULL);
	fclose(fp);
	free(instrospect);
	free(s_data);

	return DBUS_HANDLER_RESULT_HANDLED;
}



//Respone function for d-bus message
static DBusHandlerResult dbus_respone(DBusConnection *connection, DBusMessage *message, void *user_data)
{

    /*
    Current support list
    - Register
    - Unregister
    - Update
    - Terminate
    - Get
    Sync data get.

    */

    //printf("[SM] Message received\n");
    if (dbus_message_is_method_call(message, ESLAB_INTERFACE, "Get")){
        return dataGet(connection, message, user_data);
	}
    else if (dbus_message_is_method_call(message, "org.freedesktop.DBus.Introspectable", "Introspect")){
        return introspect(connection, message, user_data);
    }

}
                                                
void init_dbus(){
	int retval;

	printf("Start D-Bus Initializing\n");

	// Dbus Initializing (Path: /eslab.injung)

	DBusError error;
	GMainLoop *loop;

	loop = g_main_loop_new(NULL, FALSE);                             
	dbus_error_init(&error);

	connection = dbus_bus_get(DBUS_BUS_SYSTEM, &error);

	if(dbus_error_is_set(&error))
	{
		printf("Error: Connecting to the daemon bus: %s", error.message);
		dbus_error_free(&error);

	}

	retval = dbus_bus_request_name(connection, ESLAB_BUS_NAME, DBUS_NAME_FLAG_ALLOW_REPLACEMENT, &error);
	dbus_connection_flush(connection);



	switch(retval){
		case -1: {
			printf("Couldn't acquire name %s for connection: %s\n", ESLAB_BUS_NAME, error.message);
			dbus_error_free(&error);
			break;
		}
		case DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER:
			printf("Success to get bus name :  %s\n", ESLAB_BUS_NAME);
			break;
		case DBUS_REQUEST_NAME_REPLY_IN_QUEUE:
			printf("In queue :  %s\n", ESLAB_BUS_NAME);
			break;
		case DBUS_REQUEST_NAME_REPLY_EXISTS:
			printf("Already exists :  %s\n", ESLAB_BUS_NAME);
			break;
		case DBUS_REQUEST_NAME_REPLY_ALREADY_OWNER:
			printf("Already own this name :  %s\n", ESLAB_BUS_NAME);
			break;
		default:
			printf("Unknown result = %d\n", retval);
	}

	dbus_bus_add_match(connection, "type='signal',interface='org.eslab.injung'", NULL);  //Dbus ?„ì¹˜ ?¤ì •
	dbus_connection_add_filter(connection, dbus_respone, loop, NULL); //signal filter?¤ì •

	dbus_connection_setup_with_g_main(connection, NULL);
	g_main_loop_run(loop); // loop ?œìž‘
	printf("dbus loop end... \n");
	//
	//--------------------------------------------------------------------------------//
	


}


int main()
{
	init_dbus();

	return 0;
}




