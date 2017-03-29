#bin/bash

gcc `pkg-config --libs --cflags dbus-1 glib-2.0 dbus-glib-1` dbus_listener.c -o listener `pkg-config --libs --cflags dbus-1 glib-2.0 dbus-glib-1` 
gcc `pkg-config --libs --cflags dbus-1 glib-2.0 dbus-glib-1` dbus_sender.c -o sender `pkg-config --libs --cflags dbus-1 glib-2.0 dbus-glib-1` 




