#include "port_time.h"

bool TimeObj:: operator ==(const TimeObj& b){
	return ( (this->hours==b.hours) && (this->minutes== b.minutes) && (this->seconds== b.seconds) );
}

bool TimeObj:: operator >=(const TimeObj &b){
	if(*this==b)
		return true;
	return   (this->hours>b.hours) || ((this->hours==b.hours)&& (this->minutes>b.minutes)) || ((this->hours==b.hours)&& (this->minutes==b.minutes)&&(this->seconds>b.seconds));
}
TimeObj operator+(const struct tim& a, const struct tim& b){
	// all divisions should be floored
	struct tim to_ret;
	uint8_t sec_sum=a.seconds+b.seconds;
	uint8_t min_sum=a.minutes+b.minutes;
	uint8_t hours_sum=a.hours+b.hours;
	to_ret.seconds=(sec_sum)%60;
	to_ret.minutes=(min_sum+(sec_sum/60))%60;
	to_ret.hours=(hours_sum+(min_sum/60))%24;
	return to_ret;
}
Hfeed_Tim_Helper& Hfeed_Tim_Helper::GetNew(){
	static struct Tim_Helper k;
	k.start_millis=0;
	k.start_time.hours=0;
	k.start_time.minutes=0;
	k.start_time.seconds=0;
	return k;
}
TimeObj millis_to_Hours(hfeed_millis_type end_time ,hfeed_millis_type start_time){
	TimeObj to_ret={
			.seconds=0,
			.minutes=0,
			.hours=0
	};
	// checking input
	if (start_time>end_time)
		return to_ret;
	//	calculating difference in seconds
	hfeed_millis_type difference= (end_time-start_time)/1000;
	// Converting 7000:
	//		1-	floor(7000 /3600(seconds in one h) )== 1 hour
	//			difference= 7000-3600*hours=3400 ( equally it is  7000 mod 3600)
	//		2-	floor(3400 /60 )=56
	//			difference=3400-56*60=40(seconds);

	//	In uint conversion should be automatically floored
	to_ret.hours= (difference/3600);
	difference=difference%3600;
	to_ret.minutes= (difference/60);
	to_ret.seconds=(difference %60);
	return to_ret;
}
