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

Cartella di lavoro: examples/basic
Files: sender.c e receiver.c

Compilare
	$ gcc sender.c plugin/can_custom_lib.c -o sender
	$ gcc receiver.c plugin/can_custom_lib.c -o receiver

Aprire un terminale ed eseguire receiver
	$ ./receiver

Aprire un altro terminale ed eseguire sender
	$ ./sender

In receiver vedremo i dati mandati dal sender (in questo caso dati predefiniti)

*********************************************

Cartella di lavoro: examples/token
Files: token.c

Compilare
	$ gcc token.c plugin/can_custom_lib.c -o token

Aprire diversi terminali e eseguire più istanze di token
	$./token

Ogni istanza avrà un identificativo diverso. 
le verie istanze possono comunicare tra di loro (con un sistema a token)
Il proprietario del token può mandare un messaggio (a un istanza o in broadcast) o inviare il token.
Chi non possiede il token può solo ricevere i messaggi.
