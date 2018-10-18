**********************
*Installare Mosquitto*
**********************

sudo apt-get update
sudo apt-get upgrade
sudo apt-get install mosquitto mosquitto-clients

sudo nano /etc/mosquitto/mosquitto.conf

allow_anonymous false
password_file /etc/mosquitto/pwfile
listener 1883

sudo mosquitto_passwd -c /etc/mosquitto/pwfile pi
"telemetrypy"
sudo reboot

mosquitto_sub -u pi -P telemetrypi -t dev/test
mosquitto_sub -h telemetrypi -u pi -P telemetrypi -t dev/test
mosquitto_pub -h telemetrypi -u pi -P telemetrypi -t dev/test -m "provare per credere"

*******************
*Mongo MQTT NodeJS*
*******************

https://github.com/dennisdegreef/mqtt-mongo-recorder