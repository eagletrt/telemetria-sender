interface TelemetryConfigMqtt {
    host: string;
    port: number;
    data_topic: string;
    log_topic: string;
}

interface TelemetryConfigMongodb {
    host: string;
    port: number;
    db: string;
    collection: string;
}

interface TelemetryConfigGPS {
    plugged: number;
    simulated: number;
    interface: string;
}

export interface TelemetryConfig {
    mqtt: TelemetryConfigMqtt;
    mongodb: TelemetryConfigMongodb;
    gps: TelemetryConfigGPS;
    pilots: string[];
    races: string[];
    circuits: string[];
    can_interface: string;
    sending_rate: number;
    verbose: number;
}