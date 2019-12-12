#include "config.h"

config_t* config_setup(const char* cfgpath) {
	//set the pointers to the file and to the configure structure
	FILE *fptr;
	config_t* toRtn;

	//try to open the file
	if (verbose) printf("Setup procedure started. Opening file %s.\n", cfgpath);
	fptr = fopen(cfgpath, "r");

	if (fptr == NULL) {
		if (verbose) printf("%s opening procedure failed.\n",cfgpath);
		return NULL;
	}

	if (verbose) printf("File %s opened with success. Parsing procedure started.\n", cfgpath);

	//if we are here the file has been opened with success
	//then we put the whole file into one string, reallocating it as needed
	char *json;
	char *line; size_t len = 0; int totlen = 0;

	getline(&line,&len,fptr);
	totlen += len;
	
	json = (char*) malloc(sizeof(char)*totlen);
	strcpy(json,line);

    while (getline(&line,&len,fptr) != -1) {       	
		totlen += len;

		json = (char*) realloc(json,sizeof(char)*totlen);
		strcat(json,line);
    }
    fclose(fptr);
    
    if (verbose) printf("Passed JSON(%s) : %s\n\n",cfgpath ,json);

    //we start the parsing procedure and we split the data in tokens.
	int result;
	jsmn_parser p;
	jsmntok_t tokens[64]; /* We expect no more than 64(39) JSON tokens */

	jsmn_init(&p);
	result = jsmn_parse(&p, json, strlen(json), tokens, 100);

	if (verbose) printf("Parsed #%d entries.\n",result-1);

	toRtn = (config_t*) malloc(sizeof(config_t));
	toRtn->sending_time = 500; //default

	toRtn->pilots_size = 0;
	toRtn->pilots = NULL;
	toRtn->chosen_pilot = 0;

	toRtn->races_size = 0;
	toRtn->races = NULL;
	toRtn->chosen_race = 0;
	for (int i = 1; i <result; i+=2) {
		jsmntok_t key = tokens[i];
		jsmntok_t value = tokens[i+1];
		

		unsigned int lenght = key.end - key.start;
		char keyString[lenght + 1];    
		memcpy(keyString, &json[key.start], lenght);
		keyString[lenght] = '\0';
		
		lenght = value.end - value.start;
		char valueString[lenght + 1];    
		memcpy(valueString, &json[value.start], lenght);
		valueString[lenght] = '\0';
				
		if (strcmp(keyString,"broker_host") == 0) {
			toRtn->broker_host = (char*) malloc(sizeof(char)*lenght);
			strcpy(toRtn->broker_host, valueString);	
		} else if (strcmp(keyString,"broker_port") == 0) {
			toRtn->broker_port = atoi(valueString);			
		} else if (strcmp(keyString,"mqtt_topic") == 0) {
			toRtn->mqtt_topic = (char*) malloc(sizeof(char)*lenght);
			strcpy(toRtn->mqtt_topic, valueString);
		} 

    	else if (strcmp(keyString,"mongo_host") == 0) {
			toRtn->mongo_host = (char*) malloc(sizeof(char)*lenght);
			strcpy(toRtn->mongo_host, valueString);			
		} else if (strcmp(keyString,"mongo_port") == 0) {
			toRtn->mongo_port = atoi(valueString);
		} else if (strcmp(keyString,"mongo_db") == 0) {
			toRtn->mongo_db = (char*) malloc(sizeof(char)*lenght);
			strcpy(toRtn->mongo_db, valueString);
		}

	    else if (strcmp(keyString,"sending_time") == 0) {
	      toRtn->sending_time = atoi(valueString);	
		} else if (strcmp(keyString,"status_checker") == 0) {
	      toRtn->status_checker = atoi(valueString);	
		} else if (strcmp(keyString, "pilots") == 0) {
			for (int j = 0; j < value.size; j++) {
				jsmntok_t child = tokens[i + j + 2];
				unsigned int length = child.end - child.start;
				char childString[length + 1];    
				memcpy(childString, &json[child.start], length);
				childString[length] = '\0';

				toRtn->pilots_size++;
				if (toRtn->pilots == NULL) {
					toRtn->pilots = (char**)malloc(sizeof(char*) * toRtn->pilots_size);
				}
				else {
					toRtn->pilots = (char**)realloc(toRtn->pilots, sizeof(char*) * toRtn->pilots_size);
				}
				toRtn->pilots[toRtn->pilots_size - 1] = (char*) malloc(sizeof(char)*length);
				strcpy(toRtn->pilots[toRtn->pilots_size - 1], childString);
			}
			i += value.size;
		}  else if (strcmp(keyString, "races") == 0) {
			for (int j = 0; j < value.size; j++) {
				jsmntok_t child = tokens[i + j + 2];
				unsigned int length = child.end - child.start;
				char childString[length + 1];    
				memcpy(childString, &json[child.start], length);
				childString[length] = '\0';

				toRtn->races_size++;
				if (toRtn->races == NULL) {
					toRtn->races = (char**)malloc(sizeof(char*) * toRtn->races_size);
				}
				else {
					toRtn->races = (char**)realloc(toRtn->races, sizeof(char*) * toRtn->races_size);
				}
				toRtn->races[toRtn->races_size - 1] = (char*) malloc(sizeof(char)*length);
				strcpy(toRtn->races[toRtn->races_size - 1], childString);
			}
			i += value.size;
		} 
	}

	for (int i = 0; i < toRtn->pilots_size; i++) {
		printf("i is %d, pilot is %s\n", i, toRtn->pilots[i]);
	}

	for (int i = 0; i < toRtn->races_size; i++) {
		printf("i is %d, race is %s\n", i, toRtn->races[i]);
	}


	if (verbose) printf("%s has generated a correct set of configurations.\n\n", cfgpath);
	return toRtn;}

int config_quit(config_t* cfg) {
	free(cfg->broker_host);
	free(cfg->mqtt_topic);
	free(cfg->mongo_host);
	free(cfg->mongo_db);

	free(cfg);
	return 0;}