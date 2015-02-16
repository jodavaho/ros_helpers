
#ifndef OPENCV_HELP_H
#define OPENCV_HELP_H

#include <stdio.h>
namespace RSN{
	inline void dumpMatrix(cv::Mat m){
		printf("\n");
		for (int j =0;j<m.rows;j++){
			printf("\t");
			for (int i=0;i<m.cols;i++){
				printf(" %0.3f ",m.at<double>(j,i));
			}
			printf("\n");
		}
		printf("\n");
	}
}
#endif
