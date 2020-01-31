/*GPS.h*/
#ifndef _GPS_H_
#define _GPS_H_

//types
struct GLL
{
	char *latitude,
		*longitude,
		ns_indicator,
		ew_indicator,
		*UTCtime,
		*status;
};
typedef struct GLL GLL;

struct GGA
{
	char *latitude,
		*longitude,
		ns_indicator,
		ew_indicator,
		*UTCtime,
		*altitude,
		*status;
};
typedef struct GGA GGA;

//signatures
int openPort(char *port);
GLL GLLstring2GLLstruct(char *message);
GGA *GGAstring2GGAstruct(char *message);
GGA *getGGAstruct(int serial_port);

#endif
