

/**

----- Brief -----
Time measurement library, which also abstracts the OS layer.
----- Brief -----

*/

#ifndef BASE_TIME_CORE_H
# define BASE_TIME_CORE_H


// -- Preprocessor Defines
// -- Preprocessor Defines


// -- Enums/Flags
typedef enum Month {
	Month_Jan,
	Month_Feb,
	Month_Mar,
	Month_Apr,
	Month_May,
	Month_Jun,
	Month_Jul,
	Month_Aug,
	Month_Sep,
	Month_Oct,
	Month_Nov,
	Month_Dec
} Month;

typedef enum DayOfWeek {
	DayOfWeek_Sunday,
	DayOfWeek_Monday,
	DayOfWeek_Tuesday,
	DayOfWeek_Wednesday,
	DayOfWeek_Thursday,
	DayOfWeek_Friday,
	DayOfWeek_Saturday
} DayOfWeek;
// -- Enums/Flags


// -- Types
typedef u64 TimeDense;

typedef struct TimeDate TimeDate;
struct TimeDate {
	u16 msec;
	u8 sec;
	u8 min;
	u8 hour;
	u8 day;
	u8 month;
	s16 year;  // negative is BCE
};
// -- Types


// -- Functions
internalf void base_time_init(void);
internalf void base_time_deinit(void);

internalf const s8* cstr8_from_month(Month month);
internalf const s8* cstr8_from_day_of_week(DayOfWeek day_of_week);

internalf TimeDense time_dense_from_date(TimeDate* date);
internalf TimeDate time_date_from_dense(TimeDense dense);

internalf u64 time_in_nanos(void);
internalf void time_sleep_in_nanos(u64 time);
internalf TimeDate time_now_universal(void);
internalf TimeDate time_local_from_universal(TimeDate* date);
internalf TimeDate time_universal_from_local(TimeDate* date);
// -- Functions


#endif  // BASE_TIME_CORE_H
