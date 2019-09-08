/*
 * date.cpp
 *
 *  Created on: Nov 29, 2018
 *      Author: Tim Stark
 */

#include <sys/time.h>
#include <unistd.h>

#include "main/core.h"
#include "main/date.h"

static const char *monthList[] =
{ "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

Date::Date()
: year(0), month(0), day(0),
  hour(0), minute(0), second(0),
  wday(0), utcOffset(0), tzName("UTC")
{
}

Date::Date(int y, int m, int d)
: year(y), month(m), day(d),
  hour(0), minute(0), second(0),
  wday(0), utcOffset(0), tzName("UTC")
{
}

Date::Date(double jd)
{
	int64_t a = (int64_t)floor(jd + 0.5);
	wday = (a + 1) % 7;

	double c;
	if (a < 2299161)
		c = (double)(a + 1524);
	else {
		double b = (double)((int64_t)floor((a - 1867215.25) / 36524.25));
		c = a + b - (int64_t)floor(b / 4) + 1525;
	}

	int64_t d = (int64_t)floor((c - 122.1) / 365.25);
	int64_t e = (int64_t)floor(365.25 * d);
	int64_t f = (int64_t)floor((c - e) / 30.6001);

	double dday = c - e - (int64_t)floor(30.6001 * f) + ((jd + 0.5) - a);

	month     = (int)(f - 1 - 12 * (int64_t)(f / 14));
	year      = (int)(d - 4715 - (int64_t)((7.0 + month) / 10.0));
	day       = (int)dday;

	double dhour = (dday - day) * 24;
	hour      = (int)dhour;

	double dminute = (dhour - hour) * 60;
	minute    = (int)dminute;

	second    = (dminute - minute) * 60;

	utcOffset = 0;
	tzName    = "UTC";
}

Date::~Date()
{
}

double Date::getJulian() const
{
	int y, m, ly = 0;

	if (month <= 2) {
		y = year  - 1;
		m = month + 12;
	} else {
		y = year;
		m = month;
	}

	if (year > 1582 || (year == 1582 && (month > 10 || (month == 10 && day >= 15))))
		ly = 2 - y/100 + y/400;

	return (floor(365.25 * y) + floor(30.6001 * (m + 1)) + ly + 1720996.5 +
			day + (hour / HOURS_PER_DAY) + (minute / MINUTES_PER_DAY) +
			(second / SECONDS_PER_DAY));
}

string Date::getString() const
{
	char date[255];

	snprintf(date, sizeof(date), "%04d %s %02d %02d:%02d:%02d %s",
			year, monthList[month-1], day, hour, minute, int(second),
			tzName.c_str());

	return string(date);
}

void Date::reset()
{
	struct timeval now;

	gettimeofday(&now, nullptr);
	sysTime  = double(now.tv_sec) + (double(now.tv_usec) / 1000000.0f);
	lastTime = sysTime;

	this->now();
}

double Date::update()
{
	struct timeval now;
	double dt;

	gettimeofday(&now, nullptr);
	sysTime  = double(now.tv_sec) + (double(now.tv_usec) / 1000000.0f);
	dt       = sysTime - lastTime;
	lastTime = sysTime;

	// Update current julian calendar time
	jdTime  += dt / SECONDS_PER_DAY;

	return dt;
}

void Date::now()
{
	time_t t = time(NULL);
	struct tm *gmt = gmtime(&t);

	year   = gmt->tm_year + 1900;
	month  = gmt->tm_mon + 1;
	day    = gmt->tm_mday;
	hour   = gmt->tm_hour;
	minute = gmt->tm_min;
	second = gmt->tm_sec;

	wday   = gmt->tm_wday;
	dst    = gmt->tm_isdst;

	jdTime = getJulian();
}
