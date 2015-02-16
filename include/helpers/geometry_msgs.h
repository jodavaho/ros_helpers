
#ifndef GEOMETRY_MSGS_HELPERS_H
#define GEOMETRY_MSGS_HELPERS_H
#include <math.h>
#include "helpers/math.h"
#include <geometry_msgs/Quaternion.h>

namespace RSN{

	inline geometry_msgs::Quaternion getQ(double roll, double pitch, double yaw){
		double c1 = cos(yaw *0.5);
		double s1 = sin(yaw *0.5);
		double c2 = cos(pitch *0.5);
		double s2 = sin(pitch *0.5);
		double c3 = cos(roll *0.5);
		double s3 = sin(roll *0.5);

		geometry_msgs::Quaternion q;
		q.x = c1*c2*s3 -s1*s2*c3;
		q.y = c1*s2*c3 +s1*c2*s3;
		q.z = s1*c2*c3 -c1*s2*s3;
		q.w = c1*c2*c3 +s1*s2*s3;

		return q;
	}
	inline double getYaw(geometry_msgs::Quaternion q){
		q.x = 0;
		q.y = 0;
		double mag = q.z * q.z + q.w * q.w;
		q.z = q.z / sqrt(mag);
		q.w = q.w / sqrt(mag);
		return normalizeAngle(2* atan2(q.z,q.w));
	}
}
#endif
