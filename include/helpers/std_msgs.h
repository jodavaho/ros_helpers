
#ifndef STD_MSGS_HELPERS_H
#define STD_MSGS_HELPERS_H
#include <math.h>
#include "helpers/math.h"
#include <std_msgs/Float64.h>

namespace RSN{
	inline std::vector<double> GetVec(std::vector<std_msgs::Float64> in){
		std::vector<double> ret;
		for (std::vector<std_msgs::Float64>::iterator it=in.begin();
				it!=in.end();it++){
			ret.push_back(it->data);
		}
		return ret;
	}

}
#endif
