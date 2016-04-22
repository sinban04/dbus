#bin/bash

gcc `pkg-config --libs --cflags dbus-1 glib-2.0 dbus-glib-1` dbus_listen.c -o listen
gcc `pkg-config --libs --cflags dbus-1 glib-2.0 dbus-glib-1` dbus_sender.c -o sender




