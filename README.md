# fenice-telemetria-sender

This is the telemetry repo of **[@eagletrt](https://www.github.com/eagletrt)**

## Project purpose

This project is made for the [@eagletrt](https://www.github.com/eagletrt) car, in order to save data when the car is running. The telemetry is a program located in the **raspberry** attached to the **canbus** of the car and directly to the serial porf of the **rover gps**, making it working. It reads the **messages** sent by the **sensors** attached to the canbus or the rover gps, accumulates them in an **organized structure** and every 500 milliseconds saves the data in a local **mongodb** and forwards it via **mqtt**. In order to avoid saving a huge amount of unneeded data when the car is not moving, the telemetry can be **controlled** by the **steering-wheel**, that can send messages via can to start or stop the telemetry to save the data in the database and pass parameters such as **pilot** and **race** to track the current session in the database.

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

## How was it made

The telemetry is a **C** program, thought to run on a **linux system**. Being the code related to the **saved structure** very long and since it changed very frequently, I eventuallly came up with a strange solution. There is the `structure.json` file that determines the structure of the saved data and with an **[npm module](https://github.com/euberdeveloper/eagletrt-code-generator)** the c code related to that part is generated automatically.

### State machine

The program is a quite simple **state machine**, implemented in c with a **matrix of transitions functions**. It has also a mega global variable called **condition**, accessible everywhere in the code.

The states are:

* __INIT__: When the telemetry is starting, it reads the **config** file and sets up everything (**canbus interface**, **gps serial port**, **mqtt connection**, **mongodb connection**). After being executed, it will pass to the **IDLE** state.
* __IDLE__: The telemetry is actually running. There are two threads that read all the can messages and gps messages and save them in a structure. Every 500ms this structure is converted to **bson** and sent via **mqtt**. After being executed, it will repeat itself unless a **can message** to enable the telemetry is received.
* __ENABLED__: The telemetry does the same thing of the **IDLE** state, but it also saves the data in **mongodb**. After being executed, it will repeat itself unless a **can message** to disable the telemetry is received.
* __EXIT__: The telemetry tries to gently deallocate all the data and close all connections before exiting. It is usually reached when an **error** occurs.

### Configuration

There is the **config.json** file to configure various aspects of the telemetry. The name can be actually changed and is given as a parameter when executing the compiled **sender.out** file. There is no need of recompiling the telemetry after changing this file.

The options are:

* __mqtt__: The options regarding the mqtt connection
  * __host__: The host of the mqtt connection
  * __port__: The port of the mqtt connection
  * __data_topic__: The topic where the bson data is sent
  * __log_topic__: The topic where the telemetry log is sent
  
* __mongodb__: The options regarding the mongodb connection
  * __host__: The host of the mongodb connection
  * __port__: The port of the mongodb connection
  * __db__: The db where the data will be saved
  * __collection__: The collection where the data will be saved
  
* __gps__: The options regarding the rover gps plugged to the telemetry
  * __plugged__: If the gps is plugged to the telemetry. If the gps is not plugged but this option is set to `1`, the telemetry will try to read the gps serial port and will crush.
  * __simulated__: If the gps is simulated and not real.
  * __interface__: The interface name of the gps serial port
  
* __pilots__: The array containing the possible pilots who drive the car. Every time the steering wheel enables the telemetry, it specifies also the index of the pilot who drives the car. The pilots name will be added to the current session, to the data saved in the database

* __races__: The array containing the possible races that the car can perform. Every time the steering wheel enables the telemetry, it specifies also the index of the race that the car is performing. The race name will be added to the current session, to the data saved in the database

* __circuit__: The array containing the possible circuits where the car is running. Every time the steering wheel enables the telemetry, it specifies also the index of the circuit where the car is running. The circuit name will be added to the current session, to the data saved in the database. **NB**: `currently not implemented`

* __can_interface__: The interface of the canbus

* __sending_rate__: The number of milliseconds every which the telemetry saves the accumulated data, before emptying it and repeating the cycle

* __verbose__: If also the debug messages will be logged in the console
