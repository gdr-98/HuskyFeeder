#ifndef TIME_HELPER
#define TIME_HELPER
#define	HFEED_TIME_STM32F446RE	1
#if HFEED_TIME_STM32F446RE
#include "main.h"
/* Returns the current values in milliseconds of che system counter*/
#define HFEED_TIME_GET_CURR_MILLIS 		HAL_GetTick()
typedef uint32_t hfeed_millis_type;
#endif
// TO DO: Update Giornaliero, Gestione reset contatore
struct tim{
	uint8_t seconds;
	uint8_t minutes;
	uint8_t hours;
	bool operator >=(const struct tim&);
	bool operator ==(const struct tim&);
	friend struct tim operator+(const struct tim&, const struct tim&);
};

typedef struct tim TimeObj;
TimeObj millis_to_Hours(hfeed_millis_type  ,hfeed_millis_type );
struct Tim_Helper{
	TimeObj start_time;
	hfeed_millis_type start_millis;
	static struct Tim_Helper& GetNew();
	inline void start(const TimeObj&start ){	start_time=start;	start_millis=HFEED_TIME_GET_CURR_MILLIS;}
	inline void reset(){ start_time.seconds=0;start_time.minutes=0;start_time.hours=0;  start_millis=0;}
	// checks if now > cmp
	inline bool is_greater(const TimeObj& to_cmp){ return ( millis_to_Hours(HFEED_TIME_GET_CURR_MILLIS,start_millis)+start_time )>=to_cmp;}
};
typedef struct Tim_Helper Hfeed_Tim_Helper;

#endif
