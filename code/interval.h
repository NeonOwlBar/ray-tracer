#pragma once

#ifndef INTERVAL_H
#define INTERVAL_H

// Manages real-valued intervals with a min and max
class interval
{
public:
	double min, max;

	interval() : min(+infinity), max(-infinity) {} // Default interval is empty

	// creates interval with provided min and max values
	interval(double min, double max) : min(min), max(max) {}

	// returns the range of the interval
	double size() const
	{
		return max - min;
	}

	// whether a value is within the interval (min and max INclusive)
	bool contains(double x) const
	{
		return min <= x && x <= max;
	}

	// whether a value is within the interval (min and max EXclusive)
	bool surrounds(double x) const
	{
		return min < x && x < max;
	}

	// Returns x or the value of the boundary it passed (min or max)
	double clamp(double x) const
	{
		if (x < min) return min;
		if (x > max) return max;
		return x;
	}

	static const interval empty, universe;
};

// minimum is larger than maximum
const interval interval::empty = interval(+infinity, -infinity);
// infinite range
const interval interval::universe = interval(-infinity, +infinity);

#endif
