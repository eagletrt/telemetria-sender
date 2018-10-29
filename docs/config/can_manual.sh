********************************
* Installare CAN BUS for linux *
********************************

	$ sudo apt-get update
	$ sudo apt-get can-utils

	$ modprobe can_dev
	$ modprobe can
	$ modprobe can_raw
	$ sudo ip link set can0 type can

*********************************************
* Utilizzo dei programmini base per CAN BUS *
*********************************************

Cartella di lavoro: examples
Files: sender.c e receiver.c

Compilare
	$ gcc sender.c -o sender
	$ gcc receiver.c -o receiver

Aprire un terminale ed eseguire receiver
	$ ./receiver

Aprire un altro terminale ed eseguire sender
	$ ./sender

In receiver vedremo i dati mandati dal sender (in questo caso dati predefiniti)
Adesso sto creando delle librerie per agevolarci l'utilizzo del can

