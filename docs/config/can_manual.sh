********************************
* Installare CAN BUS for linux *
********************************

	$ sudo apt-get update
	$ sudo apt-get can-utils

	$ modprobe vcan
	$ sudo ip link add dev vcan0 type vcan
	$ sudo ip link set up vcan0

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

