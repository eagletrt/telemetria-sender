*****************
*Install Mongodb*
*****************

Per RPI
^^^^^^^

sudo apt-get install mongodb-server -y

sudo mkdir -p /data/db

sudo service mongodb start

Per Linux
^^^^^^^^^

sudo apt-key adv --keyserver hkp://keyserver.ubuntu.com:80 --recv 9DA31620334BD75D9DCB49F368818C72E52529D4

echo "deb [ arch=amd64,arm64  ] https://repo.mongodb.org/apt/ubuntu xenial/mongodb-org/4.0 multiverse" | sudo tee /etc/apt/sources.list.d/mongodb-org-4.0.list

sudo apt-get update

sudo apt-get install -y mongodb-org


****************
*Remove MongoDB*
****************

sudo service mongod stop
sudo apt-get purge mongodb-org*
sudo rm -r /var/log/mongodb
sudo rm -r /var/lib/mongodb

*************
*Run MongoDB*
*************

sudo service mongod start

mongo --host 127.0.0.1:27017

use name_of_the_db

db.students.insert([{"student": "Luca Martinelli",
                     "street": "Via Boiane",
                     "Sex": "Male",
                     "Nation": "Italy",
                     "Number": "333 4895818"
                 }])



*******************
*Mongo MQTT NodeJS*
*******************

https://github.com/dennisdegreef/mqtt-mongo-recorder