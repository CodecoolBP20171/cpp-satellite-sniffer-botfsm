#include "stdafx.h"
#include "MechStandards.h"

double satelliteSniffer::calcMJD(int mn, double dy, int yr)
{
	int b, d, m, y;
	long c;
	double mjp;

	m = mn;
	y = (yr < 0) ? yr + 1 : yr;
	if (mn < 3) {
		m += 12;
		y -= 1;
	}

	if (yr < 1582 || (yr == 1582 && (mn < 10 || (mn == 10 && dy < 15))))
		b = 0;
	else {
		int a;
		a = y / 100;
		b = 2 - a + a / 4;
	}

	if (y < 0)
		c = static_cast<long>(((365.25*y) - 0.75) - 694025L);
	else
		c = static_cast<long>((365.25*y) - 694025L);

	d = static_cast<long>(30.6001*(m + 1));

	mjp = b + c + d + dy - 0.5;

	return mjp;
}

double satelliteSniffer::degrad(double value) {
	return (value * PI / 180);
}