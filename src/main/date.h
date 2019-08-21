/*
 * date.h
 *
 *  Created on: Nov 29, 2018
 *      Author: Tim Stark
 */

#pragma once

#define DAYS_PER_YEAR		365.25

#define HOURS_PER_DAY		24.0
#define MINUTES_PER_DAY		1440.0
#define SECONDS_PER_DAY		86400.0

class Date {
public:
	Date();
	Date(int y, int m, int d);
	Date(float);
	~Date();

	float  getJulian() const;
	string getString() const;

	inline operator float() const { return getJulian(); }

	void   reset();
	void   now();
	float  update();

private:
	float lastTime;
	float sysTime;
	float jdTime;

	int    year;
	int    month;
	int    day;

	int    hour;
	int    minute;
	float  second;

	int    wday;

	bool   dst;
	int    utcOffset;
	string tzName;
};
