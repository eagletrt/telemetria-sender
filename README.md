# fenice-telemetria-sender

This is the telemetry repo of **[@eagletrt](https://www.github.com/eagletrt)**

## Project purpose

This project is made for the [@eagletrt](https://www.github.com/eagletrt) car, in order to save data when the car is running. The telemetry is a program located in the **raspberry** attached to the **canbus** of the car and directly to the serial porf of the **rover gps**, making it working. It reads the **messages** sent by the **sensors** attached to the canbus or the rover gps, accumulates them in an **organized structure** and every 500 milliseconds saves the data in a local **mongodb** and forwards it via **mqtt**. In order to avoid saving a huge amount of unneeded data when the car is not moving, the telemetry can be **controlled** by the **steering-wheel**, that can send messages via can to start or stop the telemetry to save the data in the database and pass parameters such as **pilot** and **race** to track the current session in the database.

## How was it made

The telemetry is a **C** program, thought to run on a **linux system**. Being the code related to the **saved structure** very long and since it changed very frequently, I eventuallly came up with a strange solution. There is the `structure.json` file that determines the structure of the saved data and with an **[npm module](https://github.com/euberdeveloper/eagletrt-code-generator)** the c code related to that part is generated automatically.

## How to use it

On the raspeberry of the telemetry there is the **Ubuntu** for ARM operative system. Hence, all the code was thought to run on a **Linux** system.

### The prerequisites to run the telemetry are:
* Use a **Linux** operative system
* Install **gcc** to compile c programs, on ubuntu `sudo apt install build-essential`
* Install **nodejs** to execute js scripts, on ubuntu I suggest reading [this](https://tecadmin.net/install-latest-nodejs-npm-on-ubuntu)
* Install **mosquitto** to host an mqtt broker, `sudo apt install mosquitto && sudo apt install mosquitto-clients`
* Install **canutils** to connect to canbus, `sudo apt install can-utils`
* Install **mongodb** to have the local database, on ubuntu I suggest reading [this](https://hevodata.com/blog/install-mongodb-on-ubuntu/)
* Install **mongodriver for c** to use mongodb from c, on ubuntu `sudo apt install libmongoc-dev && sudo apt install libbson-dev`
* Install **mqttdriver for c** to use mqtt from c, on ubuntu `sudo apt install libmosquitto-dev`

### Before running the telemetry:
* Make sure mongod service is active (check `mongo` works)
* Simulate the canbus and the gps serial port (check [this repo](https://github.com/FilippoGas/eagletrt-telemetry-simulator))
This is needed when debugging the telemetry in a local pc

### To run the telemetry:
* Clone [this repo](https://github.com/eagletrt/fenice-telemetria-sender)
* Execute `npm i` to install the nodejs dependencies
* Execute `npm run transpile` or `npx eagle generate` to execute the js script and generate the c code
* Execute `npm run compile` or `./compile.sh` to compile the c code
* Execute `npm run start` or `sudo ./sender.out config.json` to start the telemetry (executing `npm run serve` does all the last three points with an only command)

### To change the telemetry status:
* Execute `npm run enable` or `./enable.sh` to enable the telemetry and make it saving data on the db
* Execute `npm run idle` or `./idle.sh` to disable the telemetry and make it stopping saving data on the db
This is usually useful when debbugging the application on a local computer, because simulates what does the steering wheel of the car

### To check everything is going well:

* Before starting the telemetry execute `mosquitto_sub -t telemetria_log`. It should show the log of the telemetry.
* Execute `mosquitto_sub -t telemetria`, it should show the data sent by the telemetry via mqtt (it is sent even when the telemetry is idle).
* Open **mongo compass** and check the data is saved on mogodb
