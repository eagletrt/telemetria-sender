#include "gather_gps_service.h"

/* INTERNAL VARIABLES AND STRUCTS */

pthread_t gather_gps_thread;
pthread_attr_t gather_gps_thread_attr;

/* INTERNAL FUNCTIONS SIGNATURES */

static double parseNmeaCoord(double coord);
static void* parseGpsMessages(void *args);

/* EXPORTED FUNCTIONS */

void gatherGpsStartThread() {
	// Execute only if GPS is plugged
    if (condition.gps.plugged) {
		pthread_attr_init(&gather_gps_thread_attr);
		pthread_attr_setdetachstate(&gather_gps_thread_attr, PTHREAD_CREATE_JOINABLE);
		pthread_create(&gather_gps_thread, &gather_gps_thread_attr, &parseGpsMessages, NULL);
		pthread_attr_destroy(&gather_gps_thread_attr);
	}
}

void gatherGpsStopThread() {
	// Execute only if GPS is plugged
    if (condition.gps.plugged) {
		pthread_cancel(gather_gps_thread);
	}
}

/* INTERNAL FUNCTIONS DEFINITIONS */

static double parseNmeaCoord(double coord) {
    double temp = coord / 100;
    double left = floor(temp);
    double right = (temp - left) * (5 / 3);
    return (left + right);
}

static void* parseGpsMessages(void *args) {
	// Declare used variables
	data_t* document;
    gps_struct* gps_data;
	lc_point_t point;

	while (1) {
		// Read gps serial port
		gps_data = readGPS();

		// Lock document
        pthread_mutex_lock(&condition.structure.threads.data_head_mutex);

		// Create reference to data_head
        document = condition.structure.data_head;

		// Parse gps message
		if (gps_data != NULL) {

			if (gps_data->gga && gps_data->gga->status && document->gps.gga_count < document->gps.gga_size) {
				document->gps.gga[document->gps.gga_count].timestamp = getCurrentTimestamp();

				document->gps.gga[document->gps.gga_count].value.latitude_safe = gps_data->gga->latitude;
				document->gps.gga[document->gps.gga_count].value.longitude_safe = gps_data->gga->longitude;
				document->gps.gga[document->gps.gga_count].value.latitude = parseNmeaCoord(gps_data->gga->latitude);
				document->gps.gga[document->gps.gga_count].value.longitude = parseNmeaCoord(gps_data->gga->longitude);
				document->gps.gga[document->gps.gga_count].value.altitude = gps_data->gga->altitude;
				document->gps.gga[document->gps.gga_count].value.ns_indicator = strdup(gps_data->gga->ns_indicator);
				document->gps.gga[document->gps.gga_count].value.ew_indicator = strdup(gps_data->gga->ew_indicator);
				document->gps.gga[document->gps.gga_count].value.utc_time = strdup(gps_data->gga->utc_time);

				++(document->gps.gga_count);
			}
			else if (gps_data->gll && gps_data->gll->status && document->gps.gll_count < document->gps.gll_size) {
				document->gps.gll[document->gps.gll_count].timestamp = getCurrentTimestamp();

				document->gps.gll[document->gps.gll_count].value.latitude = parseNmeaCoord(gps_data->gll->latitude);
				document->gps.gll[document->gps.gll_count].value.longitude = parseNmeaCoord(gps_data->gll->longitude);
				document->gps.gll[document->gps.gll_count].value.ns_indicator = strdup(gps_data->gll->ns_indicator);
				document->gps.gll[document->gps.gll_count].value.ew_indicator = strdup(gps_data->gll->ew_indicator);
				document->gps.gll[document->gps.gll_count].value.utc_time = strdup(gps_data->gll->utc_time);

				++(document->gps.gll_count);
			}
			else if (gps_data->vtg && document->gps.vtg_count < document->gps.vtg_size) {
				document->gps.vtg[document->gps.vtg_count].timestamp = getCurrentTimestamp();

				document->gps.vtg[document->gps.vtg_count].value.ground_speed_knots = gps_data->vtg->ground_speed_knots;
				document->gps.vtg[document->gps.vtg_count].value.ground_speed_human = gps_data->vtg->ground_speed_human;

				++(document->gps.vtg_count);
			}
			else if (gps_data->rmc && gps_data->rmc->status && document->gps.rmc_count < document->gps.rmc_size) {
				document->gps.rmc[document->gps.rmc_count].timestamp = getCurrentTimestamp();

				document->gps.rmc[document->gps.rmc_count].value.latitude = parseNmeaCoord(gps_data->rmc->latitude);
				document->gps.rmc[document->gps.rmc_count].value.longitude = parseNmeaCoord(gps_data->rmc->longitude);
				document->gps.rmc[document->gps.rmc_count].value.ns_indicator = strdup(gps_data->rmc->ns_indicator);
				document->gps.rmc[document->gps.rmc_count].value.ew_indicator = strdup(gps_data->rmc->ew_indicator);
				document->gps.rmc[document->gps.rmc_count].value.utc_time = strdup(gps_data->rmc->utc_time);
				document->gps.rmc[document->gps.rmc_count].value.date = strdup(gps_data->rmc->date);
				document->gps.rmc[document->gps.rmc_count].value.ground_speed_knots = gps_data->rmc->ground_speed_knots;

				point.x = document->gps.rmc[document->gps.rmc_count].value.latitude;
				point.y = document->gps.rmc[document->gps.rmc_count].value.longitude;
				if (lc_eval_point(condition.lapcounter.lapcounter, &point)) {
					condition.structure.lap_index += 1;

					if (condition.structure.enabled == 1) {
						mongoNewLap();
					}
				}

				++(document->gps.rmc_count);
			} 
			
			gpsFree(gps_data);
		}

		// Unlock document
        pthread_mutex_unlock(&condition.structure.threads.data_head_mutex);
	}

	return NULL;
}