# Fenice Telemetria Sender - Refactor Edition

## File System

```bash
Telemetria
├─ src
│	├─ lib
│	│	├─ can_lib
│	│	│	├─ can_custom.h
│	│	│	└─ can_custom.c
│	│	├─ config_lib
│	│	│	├─ config_custom.h
│	│	│	├─ config_custom.c
│	│	│	└─ jsmn
│	│	├─ mongo_lib
│	│	│	├─ mongo_custom.h
│	│	│	└─ mongo_custom.c
│	│	├─ mosquitto_lib
│	│	│	├─ mosquitto_custom.h
│	│	│	└─ mosquitto_custom.c
│	│	└─ structure_lib
│	│		├─ structure_custom.h
│	│		├─ structure_custom.c
│	│		├─ structure_custom.template.h
│	│		└─ structure_custom.template.c
│	├─ javascript
│	│	├─ utils
│	│	├─ generators
│	│	└─ index.js
│	├─ compile.sh
│	├─ cfg.json
│	├─ package.json
│	├─ structure.json
│	└─ main.c
├─ test
├─ docs
├─ README.md
└─ .git
```

The directory **src** contains the code for the Sender application. 

The subdir *javascript* instead consist of all the file needed for the dynamic generation of the code. 
The javascript code is run through node before the compilation of the C code. 

Inside subdir *lib* there are all the ad hoc library developed for the project. 
The ones regarding *can_lib*, *mongo_lib* and *mosquitto_lib* are the one tested to check the correctness of the environment


The other directories **test** and **docs** contains the test codes regarding the upcited libraries and some useful documents for the understanding of the whole project **(TBC)**

## ENVIRONMENT SETUP

## INSTALLATION

## LAUNCH
```bash
	$ ./pub cfg.json
```
On the car system, a service is implemented. At the start up of the device, the application will launch by itself.