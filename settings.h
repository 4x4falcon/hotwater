// Network settings

const char* ssid = "_____add_your_ssid____";
const char* password = "____the_password____";
const char* espName = "hotwater";

const long oneHour = 60 * 60 * 1000;    // one hour in milliseconds
const long oneDay = 24 * oneHour;

unsigned long loopStart = 0;

// this the pin that the relay is controlled from.
// could be a relay or opto-isolator shield

int relayPin = D1;

int status = 0;

#define SECS_PER_MIN  (60UL)
#define SECS_PER_HOUR (3600UL)
#define SECS_PER_DAY  (SECS_PER_HOUR * 24UL)

/* Useful Macros for getting elapsed time */
#define numberOfSeconds(_time_) (_time_ % SECS_PER_MIN)
#define numberOfMinutes(_time_) ((_time_ / SECS_PER_MIN) % SECS_PER_MIN)
#define numberOfHours(_time_) (( _time_% SECS_PER_DAY) / SECS_PER_HOUR)
#define elapsedDays(_time_) ( _time_ / SECS_PER_DAY)


