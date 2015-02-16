
#ifndef MATH_HELPERS_H
#define MATH_HELPERS_H

namespace RSN{
inline double degToRad(double deg){
	return deg/180.0*M_PI;
}

inline double normalizeAngle(double phi){
	return atan2(sin(phi),cos(phi));
}

inline double compassToXAngleRad(double deg){
	double rads = ((90-deg)/180.0) * M_PI;
	return normalizeAngle(rads);
}

inline double normanglediff(double a1, double a2){
	a1 = normalizeAngle(a1);
	a2 = normalizeAngle(a2);
	return normalizeAngle((a1+2*M_PI)-(a2+2*M_PI));
}
inline double diff(double a1, double a2, double mn, double max){
	if (a1==a2){
		return 0;
	}
	double range = max-mn;
	return (a1+range)-(a2+range);
}
}
#endif
