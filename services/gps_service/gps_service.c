#include "gps_service.h"


/* INTERNAL FUNCTIONS SIGNATURES */

// Allocates and returns a new gps_structure
static gps_struct* gpsNew();
// Parses a line
static void parseLine(gps_struct* gps_obj, char* line);
// Parses the given GGA message and returns it as an object
static gps_gga_struct* parseGGA(char *message);
// Parses the given GLL message and returns it as an object
static gps_gll_struct* parseGLL(char *message);
// Parses the given VTG message and returns it as an object
static gps_vtg_struct* parseVTG(char *message);
// Parses the given RMC message and returns it as an object
static gps_rmc_struct* parseRMC(char *message);
// Frees the given GGA message
static void freeGGA(gps_gga_struct *message);
// Frees the given GLL message
static void freeGLL(gps_gll_struct *message);
// Frees the given RMC message
static void freeRMC(gps_rmc_struct *message);
// Formats latitude and longitude properly
static double parseCoordinates(double raw);

/* EXPORTED FUNCTIONS */

int openGPSPort() {
	// Opens the serial port
	int serial_port = open(condition.gps.interface, O_RDWR);

	// Handle in case of error
	if(serial_port < 0) {
		logWarning("Error in opening GPS port");
		return -1;
	}

	// Create termios struct to configure port
	struct termios tty;
	memset(&tty, 0, sizeof(tty));

	// Read in existing settings and handle errors
	if(tcgetattr(serial_port, &tty) != 0) {
		logWarning("Error in GPS tcgetattr");
		return -1;
	}

	// the c_cflags member of the termios contains control fields
	tty.c_cflag &= ~PARENB;				// disable parity bit
	tty.c_cflag &= ~CSTOPB;				// clear stop field
	tty.c_cflag |= CS8;					// 8 data bits per byte
	tty.c_cflag &= ~CRTSCTS;			// disable TRS/CTS hardware flow control
	tty.c_cflag |= CREAD | CLOCAL;		// turn on READ and ignore control lines, setting CLOCAL allows us to read data
	// local modes
	tty.c_lflag &= ~ICANON;		// disable canonical mode, in canonical mode input data is received line by line, usually undesired when dealing with serial ports
	tty.c_lflag &= ~ECHO;		// if this bit (ECHO) is set, sent characters will be echoed back.
	tty.c_lflag &= ~ECHOE;
	tty.c_lflag &= ~ECHONL;
	tty.c_lflag &= ~ISIG;		// when the ISIG bit is set, INTR,QUIT and SUSP characters are interpreted. we don't want this with a serial port
	// the c_iflag member of the termios struct contains low-level settings for input processing. the c_iflag member is an int
	tty.c_iflag &= ~(IXON | IXOFF | IXANY);											// clearing IXON,IXOFF,IXANY disable software flow control
	tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL);	// clearing all of this bits disable any special handling of received bytes, i want raw data
	// output modes (c_oflag). the c_oflag member of the termios struct contain low level settings for output processing, we want to disable any special handling of output chars/bytes
	tty.c_oflag &= ~OPOST;		// prevent special interpretation of output bytes
	tty.c_oflag &= ~ONLCR;		// prevent conversion of newline to carriage return/line feed
	// setting VTIME VMIN
	tty.c_cc[VTIME] = 10;		// read() will block until either any amount of data is received or the timeout ocurs
	tty.c_cc[VMIN] = 0;

	// Setting baud rate
	cfsetispeed(&tty, B460800);
	cfsetospeed(&tty, B460800);

	// After changing settings we need to save the tty termios struct, also error checking
	if(tcsetattr(serial_port, TCSANOW, &tty) != 0) {
		logWarning("Error in GPS tcsetattr");
		return -1;
	}

	return serial_port;
}

int prepareSimulatedPort() {
	struct termios ts;
    if(tcgetattr(condition.gps.port, &ts))
    {
		logWarning("Error in preparing GPS simulated port");
		return -1;
    }
    cfmakeraw(&ts);
    tcsetattr(condition.gps.port, TCSANOW, &ts);
	return 0;
}

gps_struct* readGPS() { 
	// Instantiate the line
	char line[512];
	int line_index = 0;
	char ch;
	
	// Read the line and handle errors;
	do {
		int read_bytes = read(condition.gps.port, &ch, sizeof(char));
		if(read_bytes < 0){
			logWarning("GPS not reading");
			return NULL;
		}
		line[line_index] = ch;
		line_index++;
	} while (ch != '\n');
	line[line_index] = '\0';

	// Instantiate the gps object
	gps_struct *result = gpsNew();

	// Parse the line
	parseLine(result, line);

	// Return the result
	return result;
}

void gpsFree(gps_struct* gps_data) {
	if (gps_data->gga != NULL) {
		freeGGA(gps_data->gga);
		free(gps_data->gga);
	}
	if (gps_data->gll != NULL) {
		freeGLL(gps_data->gll);
		free(gps_data->gll);
	}
	if (gps_data->vtg != NULL) {
		free(gps_data->vtg);
	}
	if (gps_data->rmc != NULL) {
		freeRMC(gps_data->rmc);
		free(gps_data->rmc);
	}
	free(gps_data);
}

/* INTERNAL FUNCTIONS DEFINITIONS */

static gps_struct* gpsNew() {
	gps_struct* result = (gps_struct*) malloc(sizeof(gps_struct));
	
	result->gga = NULL;
	result->gll = NULL;
	result->vtg = NULL;
	result->rmc = NULL;

	return result;
}

static void parseLine(gps_struct* gps_obj, char* line) {
	if (strstr(line, "GGA")) {
		gps_gga_struct* obj = parseGGA(line);
		if (obj != NULL) {
			gps_obj->gga = obj;
		}
	}
	else if (strstr(line, "GLL")) {
		gps_gll_struct* obj = parseGLL(line);
		if (obj != NULL) {
			gps_obj->gll = obj;
		}
	}
	else if (strstr(line, "VTG")) {
		gps_vtg_struct* obj = parseVTG(line);
		if (obj != NULL) {
			gps_obj->vtg = obj;
		}
	}
	else if (strstr(line, "RMC")) {
		gps_rmc_struct* obj = parseRMC(line);
		if (obj != NULL) {
			gps_obj->rmc = obj;
		}
	}
}

static gps_gga_struct* parseGGA(char *message) {
	gps_gga_struct* result = (gps_gga_struct*) malloc(sizeof(gps_gga_struct));

	int i = 0;
	
	while (message != NULL && *message) {
		char* value = strsep(&message, ",");

		switch (i)
		{
			case 1:
				result->utc_time = (value == NULL ? "" : strdup(value));
				break;

			case 2:
				result->latitude = (value == NULL ? 0.0 : parseCoordinates(atof(value)));
				break;

			case 3:
				result->ns_indicator = (value == NULL ? "" : strdup(value));
				break;
			
			case 4:
				result->longitude = (value == NULL ? 0.0 : parseCoordinates(atof(value)));
				break;

			case 5:
				result->ew_indicator = (value == NULL ? "" : strdup(value));
				break;

			case 6:
				result->status = (value == NULL ? 0 : atoi(value));
				break;

			case 11:
				result->altitude = (value == NULL ? 0.0 : atof(value));
				break;
			
			default:
				break;
		}

		i++;
	}

	if (i < 15) {
		if (i >= 5) {
			free(result->ew_indicator);
		}
		
		if (i >= 3) {
			free(result->ns_indicator);
		}

		if (i >= 1) {
			free(result->utc_time);
		}

		return NULL;
	}
	else {
		return result;
	}
}

static gps_gll_struct* parseGLL(char *message) {
	gps_gll_struct* result = (gps_gll_struct*) malloc(sizeof(gps_gll_struct));

	int i = 0;
	
	while (message != NULL && *message) {
		char* value = strsep(&message, ",");
		
		switch (i)
		{
			case 1:
				result->latitude = (value == NULL ? 0.0 : parseCoordinates(atof(value)));
				break;

			case 2:
				result->ns_indicator = (value == NULL ? "" : strdup(value));
				break;

			case 3:
				result->longitude = (value == NULL ? 0.0 : parseCoordinates(atof(value)));
				break;
			
			case 4:
				result->ew_indicator = (value == NULL ? "" : strdup(value));
				break;

			case 5:
				result->utc_time = (value == NULL ? "" : strdup(value));
				break;

			case 6:
				result->status = (value == NULL ? 0 : (strcmp("A", value) == 0));
				break;
			
			default:
				break;
		}

		i++;
	}

	if (i < 8) {
		if (i >= 5) {
			free(result->utc_time);
		}
		
		if (i >= 4) {
			free(result->ew_indicator);
		}

		if (i >= 2) {
			free(result->ns_indicator);
		}

		return NULL;
	}
	else {
		return result;
	}
}

static gps_vtg_struct* parseVTG(char *message) {
	gps_vtg_struct* result = (gps_vtg_struct*) malloc(sizeof(gps_vtg_struct));

	int i = 0;
	
	while (message != NULL && *message) {
		char* value = strsep(&message, ",");
	
		switch (i)
		{
			case 5:
				result->ground_speed_knots = (value == NULL ? 0.0 : atof(value));
				break;

			case 7:
				result->ground_speed_human = (value == NULL ? 0.0 : atof(value));
				break;

			default:
				break;
		}

		i++;
	}
	
	return (i < 10) ? NULL : result;
}

static gps_rmc_struct* parseRMC(char *message) {
	gps_rmc_struct* result = (gps_rmc_struct*) malloc(sizeof(gps_rmc_struct));

	int i = 0;
	
	while (message != NULL && *message) {
		char* value = strsep(&message, ",");
		
		switch (i)
		{
			case 1:
				result->utc_time = (value == NULL ? "" : strdup(value));
				break;

			case 2:
				result->status = (value == NULL ? 0 : (strcmp("A", value) == 0));
				break;

			case 3:
				result->latitude = (value == NULL ? 0.0 : parseCoordinates(atof(value)));
				break;
			
			case 4:
				result->ns_indicator = (value == NULL ? "" : strdup(value));
				break;

			case 5:
				result->longitude = (value == NULL ? 0.0 : parseCoordinates(atof(value)));
				break;

			case 6:
				result->ew_indicator = (value == NULL ? "" : strdup(value));
				break;

			case 7:
				result->ground_speed_knots = (value == NULL ? 0.0 : atof(value));
				break;

			case 9:
				result->date = (value == NULL ? "" : strdup(value));
				break;
			
			default:
				break;
		}

		i++;
	}

	if (i < 14) {
		if (i >= 9) {
			free(result->date);
		}
		
		if (i >= 6) {
			free(result->ew_indicator);
		}

		if (i >= 4) {
			free(result->ns_indicator);
		}

		if (i >= 1) {
			free(result->utc_time);
		}

		return NULL;
	}
	else {
		return result;
	}
}

static void freeGGA(gps_gga_struct *message) {
	free(message->ns_indicator);
	free(message->ew_indicator);
	free(message->utc_time);
}

static void freeGLL(gps_gll_struct *message) {
	free(message->ns_indicator);
	free(message->ew_indicator);
	free(message->utc_time);
}

static void freeRMC(gps_rmc_struct *message) {
	free(message->ns_indicator);
	free(message->ew_indicator);
	free(message->utc_time);
	free(message->date);
}

static double parseCoordinates(double raw) {
	double temp = raw / 100.;
	double left = floor(temp);
	double right = (temp - left) * (5./3.);
	return (left + right) * 100;
}
