
#ifndef CFG_ONEWIRE_H
#define CFG_ONEWIRE_H

/**
 * Module logging level.
 *
 * $WIZ$ type = "enum"
 * $WIZ$ value_list = "log_level"
 */
#define ONEWIRE_LOG_LEVEL      	LOG_LVL_WARN

/**
 * Module logging format.
 *
 * $WIZ$ type = "enum"
 * $WIZ$ value_list = "log_format"
 */
#define ONEWIRE_LOG_FORMAT     	LOG_FMT_TERSE



/* Onewire support */
#define ONEWIRE_SUPPORT 			1

/* Onewire device detection support */
#define ONEWIRE_DETECT_SUPPORT		1

/* Onewire naming support */
#define ONEWIRE_NAMING_SUPPORT		0

/* Onewire Polling */
#define ONEWIRE_POLLING_SUPPORT 	1

/* Time between polling in 1s steps */
#define OW_POLLING_INTERVAL 		30

/* Maximum sensor count */
#define OW_SENSORS_COUNT 			8

/* Time between 1w-bus discoveries in 1s steps */
#define OW_DISCOVER_INTERVAL 		600

/* Hooks */
#define ONEWIRE_HOOK_SUPPORT		0
		


/* Onewire Power List support */
#define ONEWIRE_ECMD_LIST_POWER_SUPPORT		0

/* Onewire DS2450 (4 channel ADC) support */
#define ONEWIRE_DS2450_SUPPORT				0

/* Onewire DS2502 (eeprom) support */
#define ONEWIRE_DS2502_SUPPORT				0



#endif /* CFG_ONEWIRE_H */
