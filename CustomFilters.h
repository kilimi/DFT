/*
 * CustomFilters.h
 *
 *  Created on: Sep 25, 2013
 *      Author: lilith
 */
#include <opencv2/opencv.hpp>
using namespace cv;

#ifndef CUSTOMFILTERS_H_
#define CUSTOMFILTERS_H_

class CustomFilters {
private:
	int rows;
	int cols;
public:
	CustomFilters(int, int);
	virtual ~CustomFilters();
	Mat BHPF(int, int);
	Mat BLPF(int, int);
	Mat GHPF(int);
	Mat GBRF(int, int);
};

#endif /* CUSTOMFILTERS_H_ */
