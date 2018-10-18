*******************
*Installare Docker*
*******************

curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo apt-key add -

sudo add-apt-repository "deb [arch=amd64] https://download.docker.com/linux/ubuntu $(lsb_release -cs) stable"

sudo apt-get update

apt-cache policy docker-ce

sudo apt-get install -y docker-ce

sudo systemctl status docker

Per maggiori informazioni:

ttps://www.digitalocean.com/community/tutorials/how-to-install-and-use-docker-on-ubuntu-16-04

*****************
*Tutorial Docker*
*****************

https://github.com/pbosetti/rpi-cross-template
