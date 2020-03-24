# fenice-telemetria-sender

This is the telemetry repo of **[@eagletrt](https://www.github.com/eagletrt)**

## Project purpose

This project is made for the [@eagletrt](https://www.github.com/eagletrt) car, in order to save data when the car is running. The telemetry is a program located in the **raspberry** attached to the **canbus** of the car and directly to the serial porf of the **rover gps**, making it working. It reads the **messages** sent by the **sensors** attached to the canbus or the rover gps, accumulates them in an **organized structure** and every 500 milliseconds saves the data in a local **mongodb** and forwards it via **mqtt**. In order to avoid saving a huge amount of unneeded data when the car is not moving, the telemetry can be **controlled** by the **steering-wheel**, that can send messages via can to start or stop the telemetry to save the data in the database and pass parameters such as **pilot** and **race** to track the current session in the database.

## How to use it

On the raspeberry of the telemetry there is the **Ubuntu** for ARM operative system. Hence, all the code was thought to run on a **Linux** system.

The prerequisites to run the telemetry are:
* Use a **Linux** operative system
* Install **gcc** to compile c programs, on ubuntu `sudo apt install build-essential`
* Install **nodejs** to execute js scripts, on ubuntu I suggest reading [this](https://tecadmin.net/install-latest-nodejs-npm-on-ubuntu)
* Install **mosquitto** to host an mqtt broker, `sudo apt install mosquitto && sudo apt install mosquitto-clients`
* Install **canutils** to connect to canbus, `sudo apt install can-utils`
* Install **mongodb** to have the local database, I suggest reading [this](https://hevodata.com/blog/install-mongodb-on-ubuntu/)
* Install **mongodriver for c** to use mongodb from c, `sudo apt install 

* Clone [this repo](https://github.com/eagletrt/fenice-telemetria-sender)
* 
