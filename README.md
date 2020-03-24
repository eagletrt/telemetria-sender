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

### Structure

The structure of the gathered data is saved in the `structure.json` file and is based on the possible **messages**:

* The **structure** is a **json object**, but every **primitive key** contains the **value type** instead of the value itself
* For each **message** there is an **array of objects**
* Each **array** contains only two elements: the **message object** and the **message max count**
* Each **message object** represents the model of the **parsed message**. It has the **timestamp** of the receive time and a **value** containing the value of the message (a value or an object of values)
* Each **message max count** represents the maximum number of messages that can be saved in a single document
* Each array can be saved **directly** in the "root" of the structure object, or **nested in other objects**, in order better to organize the structure. For instance, all the bms message arrays are nested in the bms object
* There is an **id** key, containing a progressive id of the document starting by `1`
* There is a **timestamp** key, containing the timestamp when the document is saved in the db, or sent via mqtt
* There is a **sessionName** key, that refers to the **session** of the document itself

Each time the telemetry enters in the **ENABLED** state, the **id** number is reset and a new **session** is created. A session is based on the **timestamp** when the telemetry was enabled and the **parameters** (pilot, race) passed by the steering wheel. A **session document** containing these parameters is created and inserted in the database.

This is an example of **session document**:

```json
{
    "sessionName": "20200309_175011_default_default",
    "timestamp": 1583772611,
    "formatted_timestamp": "20200309_175011",
    "pilot": "default",
    "race": "default"
}
```

This is an example of **structure.json**:

```
{
    "id": "int",
    "timestamp": "long",
    "sessionName": "char*",
    "throttle": [
        {
            "timestamp": "long",
            "value": "double"
        }, 200
    ],
    "brake": [
        {
            "timestamp": "long",
            "value": "double"
        }, 200
    ],
    "bms_hv": {
        "temperature": [
            {
                "timestamp": "long",
                "value": {
                    "max": "double",
                    "min": "double",
                    "average": "double"
                }
            }, 500
        ],
        "voltage": [
            {
                "timestamp": "long",
                "value": {
                    "max": "double",
                    "min": "double",
                    "total": "double"
                }
            }, 500
        ]
    }
}
```

### Code generation

Since a huge amount of code depended on the structure and the structure changed frequently, **lots of hours** used to be lost doing **repetitive work** and copy-paste for every changed message. **C** is a **static type** language and is not favorable for structure as the telemetry one. It is anyway **essential** because of the **performance** we needed. **Javascript** is instead a very **flexible** language and is not **statically typed**. So I made a [repository](https://github.com/euberdeveloper/eagletrt-code-generator) in **Typescript** that became an [npm module](https://www.npmjs.com/package/eagletrt-code-generator) to **generate** the **C code** that **depends** on the structure.

There are some files, the **template files**, ending with `.template.c` or `.template.h`. Those files contains some **special comments** such as `// {{GENERATE_BSON_CODE}}` instead of the pieces of code that depends on the structure.

The **code generator**:

* __Reads__ the `structure.json`
* __Looks__ for the **template files**
* __Substitutes__ the **special comments** with the code automatically generated and based on the read structure
* __Creates__ a file without the `.template` extension for each template file

So, when the structure changes, it is only needed to change the `structure.json` file properly and run `npm run transpile` to generate the new C code.

