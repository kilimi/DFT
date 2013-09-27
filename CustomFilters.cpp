/*
 * CustomFilters.cpp
 *
 *  Created on: Sep 25, 2013
 *      Author: lilith
 */

#include "CustomFilters.h"
#include <math.h>

CustomFilters::CustomFilters(int r, int col) {
	this->cols = col;
	this->rows = r;
}

CustomFilters::~CustomFilters() {
}

Mat CustomFilters::BHPF(int D0, int n)
{
	Mat_<float> res = Mat(this->rows, this->cols, CV_32F);
	Mat out;
	Point center = Point(this->rows/2,  this->cols/2);
	float Duv;
	for (int i = 0; i < this->rows; i++)
		for (int j = 0; j < this->cols; j++)
		{
			Duv = sqrt(pow(i - center.x, 2) + pow(j - center.y, 2));
			if (Duv == 0) res(i, j) = 0;
			else
			{
				res(i, j) = 1 / (1 + pow(D0/Duv, 2 * n));
			}
		}
	Mat toMerge[] = {res, Mat::zeros(res.size(), CV_32F)};
	merge(toMerge, 2, out);
	return out;
}
Mat CustomFilters::BLPF(int D0, int n)
{
	Mat_<float> res = Mat(this->rows, this->cols, CV_32F);
	Mat out;
	Point center = Point(this->rows/2,  this->cols/2);
	float Duv;
	for (int i = 0; i < this->rows; i++)
		for (int j = 0; j < this->cols; j++)
		{
			Duv = sqrt(pow(i - center.x, 2) + pow(j - center.y, 2));
			if (Duv == 0) res(i, j) = 0;
			else
			{
				res(i, j) = 1 / (1 + pow(Duv/ D0, 2 * n));
			}
		}
	Mat toMerge[] = {res, Mat::zeros(res.size(), CV_32F)};
	merge(toMerge, 2, out);
	return out;
}

Mat CustomFilters::GHPF(int D0)
{
	Mat_<float> res = Mat(this->rows, this->cols, CV_32F);
	Mat out;
	Point center = Point(this->rows/2,  this->cols/2);
	float Duv;
	for (int i = 0; i < this->rows; i++)
		for (int j = 0; j < this->cols; j++)
		{
		    Duv = sqrt((pow(i - center.x, 2) + pow(j - center.y, 2)));
			res(i, j) = 1 - exp((-Duv*Duv) / (2*D0*D0));
		}
	Mat toMerge[] = {res, Mat::zeros(res.size(), CV_32F)};
	merge(toMerge, 2, out);
	return out;
}
Mat CustomFilters::GBRF(int D0, int W)
{
	Mat_<float> res = Mat(this->rows, this->cols, CV_32F);
	Mat out;
	Point center = Point(this->rows/2,  this->cols/2);
	float Duv;
	for (int i = 0; i < this->rows; i++)
		for (int j = 0; j < this->cols; j++)
		{
		    Duv = sqrt((pow(i - center.x, 2) + pow(j - center.y, 2)));
		    float first = Duv*Duv - D0*D0;
		    float second = first / Duv*W;
		    float third = second*second;
			res(i, j) = 1 - exp(-third);
		}
	Mat toMerge[] = {res, Mat::zeros(res.size(), CV_32F)};
	merge(toMerge, 2, out);
	return out;
}
