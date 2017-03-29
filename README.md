# DBUS
This is just the sample code using dbus IPC.


## Prerequisite
I assume the system is debian, and i used this code in Raspberry Pi 2,3
### Dependent Library
To build the code, you need to install


```
sudo apt-get install libdbus-1-dev
sudo apt-get install glib2.0
sudo apt-get install libdbus-glib-1-dev
```

### System Configuration
```
sudo cp dbus-config.conf /etc/dbus-1/system.d/
```

### Build the code
```
./build.sh
```

