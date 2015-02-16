
#include "helpers/geometry_msgs.h"
#include <geometry_msgs/Quaternion.h>
#define USE_MATH_DEFINES
int main(){
	printf("1? %0.3f\n",RSN::getYaw(RSN::getQ(0,0,1)));
	printf("0? %0.3f\n",RSN::getYaw(RSN::getQ(0,1,0)));
	printf("0? %0.3f\n",RSN::getYaw(RSN::getQ(1,0,0)));
	printf("pi/2? %0.3f\n",RSN::getYaw(RSN::getQ(0,0,M_PI/2)));
	printf("0? %0.3f\n",RSN::getYaw(RSN::getQ(0,0,2*M_PI)));
	printf("-pi/2? %0.3f\n",RSN::getYaw(RSN::getQ(0,0,-M_PI/2)));
}
