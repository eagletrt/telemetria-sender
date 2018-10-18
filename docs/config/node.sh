********************
*Installare Node JS*
********************

sudo apt-get update
sudo apt-get dist-upgrade

curl -sL https://deb.nodesource.com/setup_8.x | sudo -E bash -

sudo apt-get install gcc g++ make

curl -sL https://dl.yarnpkg.com/debian/pubkey.gpg | sudo apt-key add -

echo "deb https://dl.yarnpkg.com/debian/ stable main" | sudo tee /etc/apt/sources.list.d/yarn.list

sudo apt-get update && sudo apt-get install yarn

sudo apt-get install -y nodejs

"Gestione GPIO da NodeJS"
npm install onoff

*******************
*Mongo MQTT NodeJS*
*******************

https://github.com/dennisdegreef/mqtt-mongo-recorder