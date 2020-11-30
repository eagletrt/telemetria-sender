#include "gps_service.h"


/* INTERNAL FUNCTIONS SIGNATURES */

// Allocates and returns a new gps_structure
static gps_struct* gpsNew();
// Adds a GGA message to a result object
static void pushGGA(gps_struct* data, gps_gga_struct* gga);
// Adds a GLL message to a result object
static void pushGLL(gps_struct* data, gps_gll_struct* gll);
// Adds a VTG message to a result object
static void pushVTG(gps_struct* data, gps_vtg_struct* vtg);
// Adds a RMC message to a result object
static void pushRMC(gps_struct* data, gps_rmc_struct* rmc);
// Parses the given GGA message and returns it as an object
static gps_gga_struct* parseGGA(char *message);
// Parses the given GLL message and returns it as an object
static gps_gll_struct* parseGLL(char *message);
// Parses the given VTG message and returns it as an object
static gps_vtg_struct* parseVTG(char *message);
// Parses the given RMC message and returns it as an object
static gps_rmc_struct* parseRMC(char *message);
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
	// Instantiate and fill the buffer
	char read_buf[2048];
	memset(&read_buf,'\0',sizeof(read_buf));
	
	// Get size in bytes and handle errors;
	int num_bytes = read(condition.gps.port, &read_buf, sizeof(read_buf));
	if(num_bytes < 0){
		logWarning("GPS not reading");
		return NULL;
	}

	char* block = strdup(read_buf);

	gps_struct *result = gpsNew();
	int i = 0;
	while (block != NULL) {
		char* line = strsep(&block, "\n");

		if (strstr(line, "GGA")) {
			gps_gga_struct* obj = parseGGA(line);
			if (obj != NULL) {
				pushGGA(result, obj);
			}
		}
		else if (strstr(line, "GLL")) {
			gps_gll_struct* obj = parseGLL(line);
			if (obj != NULL) {
				pushGLL(result, obj);
			}
		}
		else if (strstr(line, "VTG")) {
			gps_vtg_struct* obj = parseVTG(line);
			if (obj != NULL) {
				pushVTG(result, obj);
			}
		}
		else if (strstr(line, "RMC")) {
			gps_rmc_struct* obj = parseRMC(line);
			if (obj != NULL) {
				pushRMC(result, obj);
			}
		}
	}
	
	return result;
}

void gpsFree(gps_struct* gps_data) {
	free(gps_data->gga);
	free(gps_data->gll);
	free(gps_data->vtg);
	free(gps_data->rmc);
	free(gps_data);
}

/* INTERNAL FUNCTIONS DEFINITIONS */

static gps_struct* gpsNew() {
	gps_struct* result = (gps_struct*) malloc(sizeof(gps_struct));
	
	result->gga = NULL;
	result->gga_count = 0;
	result->gll = NULL;
	result->gll_count = 0;
	result->vtg = NULL;
	result->vtg_count = 0;
	result->rmc = NULL;
	result->rmc_count = 0;

	return result;
}

static void pushGGA(gps_struct* data, gps_gga_struct* gga) {
	if (data->gga == NULL) {
		data->gga_count = 1;
		data->gga = (gps_gga_struct**) malloc(sizeof(gps_gga_struct*));
		data->gga[0] = gga;
	}
	else {
		gps_gga_struct** temp = data->gga;
		data->gga_count++;
		data->gga = (gps_gga_struct**) malloc(sizeof(gps_gga_struct*) * data->gga_count);
		for (int i = 0; i < data->gga_count - 1; i++) {
			data->gga[i] = temp[i];
		}
		data->gga[data->gga_count - 1] = gga;
		free(temp);
	}
}

static void pushGLL(gps_struct* data, gps_gll_struct* gll) {
	if (data->gll == NULL) {
		data->gll_count = 1;
		data->gll = (gps_gll_struct**) malloc(sizeof(gps_gll_struct*));
		data->gll[0] = gll;
	}
	else {
		gps_gll_struct** temp = data->gll;
		data->gll_count++;
		data->gll = (gps_gll_struct**) malloc(sizeof(gps_gll_struct*) * data->gll_count);
		for (int i = 0; i < data->gll_count - 1; i++) {
			data->gll[i] = temp[i];
		}
		data->gll[data->gll_count - 1] = gll;
		free(temp);
	}
}

static void pushVTG(gps_struct* data, gps_vtg_struct* vtg) {
	if (data->vtg == NULL) {
		data->vtg_count = 1;
		data->vtg = (gps_vtg_struct**) malloc(sizeof(gps_vtg_struct*));
		data->vtg[0] = vtg;
	}
	else {
		gps_vtg_struct** temp = data->vtg;
		data->vtg_count++;
		data->vtg = (gps_vtg_struct**) malloc(sizeof(gps_vtg_struct*) * data->vtg_count);
		for (int i = 0; i < data->vtg_count - 1; i++) {
			data->vtg[i] = temp[i];
		}
		data->vtg[data->vtg_count - 1] = vtg;
		free(temp);
	}
}

static void pushRMC(gps_struct* data, gps_rmc_struct* rmc) {
	if (data->rmc == NULL) {
		data->rmc_count = 1;
		data->rmc = (gps_rmc_struct**) malloc(sizeof(gps_rmc_struct*));
		data->rmc[0] = rmc;
	}
	else {
		gps_rmc_struct** temp = data->rmc;
		data->rmc_count++;
		data->rmc = (gps_rmc_struct**) malloc(sizeof(gps_rmc_struct*) * data->rmc_count);
		for (int i = 0; i < data->rmc_count - 1; i++) {
			data->rmc[i] = temp[i];
		}
		data->rmc[data->rmc_count - 1] = rmc;
		free(temp);
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

	return (i < 15) ? NULL : result;
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

	return (i < 8) ? NULL : result;
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

	return (i < 14) ? NULL : result;
}

static double parseCoordinates(double raw) {
	double temp = raw / 100.;
	double left = floor(temp);
	double right = (temp - left) * (5./3.);
	return (left + right) * 100;
}
