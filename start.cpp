/*
 * start.cpp
 *
 *  Created on: Sep 25, 2013
 *      Author: lilith
 */
#include <opencv2/opencv.hpp>
#include "CustomFilters.h"
using namespace cv;
using namespace std;

void DFTshift(Mat& fImage);
Mat calcMagnitude(Mat& complexImg);

int main()
{
	Mat img = imread("moon_gray.png", CV_LOAD_IMAGE_UNCHANGED);

	int r = getOptimalDFTSize(img.rows);
	int c = getOptimalDFTSize(img.cols);

	copyMakeBorder(img, img, r - img.rows, 0, 0, c- img.cols, BORDER_CONSTANT, Scalar::all(0));

	Mat planes[2] = { Mat_<float>(img), Mat::zeros(img.size(), CV_32F) };
	Mat complex;
	merge(planes, 2, complex);

	dft(complex, complex);
	CustomFilters cf = CustomFilters(img.rows, img.cols);
	Mat filter = cf.BHPF(250, 2);


	Mat_<float> temp[] = {filter.clone(), Mat_<float>(filter.rows, filter.cols, 0.0f)};
	split(filter, temp);
	Mat filterReal = temp[0];
	Mat filterImg = temp[1];


	DFTshift(complex);

	Mat_<float> temp2[] = {complex.clone(), Mat_<float>(complex.rows, complex.cols, 0.0f)};
	split(complex, temp2);
	Mat imgReal = temp2[0];
	Mat imgImg = temp2[1];

	try{
		//cout << filter.depth() << ", " << filter.size() << endl;
		//cout << complex.depth() << ", " << complex.size() << endl;
		mulSpectrums(imgReal, filterReal, imgReal, DFT_ROWS);
		//mulSpectrums(imgImg, filterImg, imgImg, DFT_ROWS);

	}
	catch(Exception ee)
	{
		cout << ee.err << endl;
	}
	//Mat toMerge[] = {imgReal, imgImg};
	//merge(toMerge, 2, complex);

	DFTshift(imgReal);

	//Mat mag = calcMagnitude(complex);

	Mat_<float> output;
	dft(imgReal, output, DFT_INVERSE | DFT_REAL_OUTPUT);

	normalize(output, output, 0, 1, CV_MINMAX);
	imshow("out", output);

	Mat filterOUt;
	split(filter, planes);
	normalize(planes[0], filterOUt, 0, 1, CV_MINMAX);

	imshow("filter", filterOUt);
	//imshow("magnitude", mag);

	waitKey(0);

}

void DFTshift(Mat& fImage )
{
	Mat tmp, q0, q1, q2, q3;

	// first crop the image, if it has an odd number of rows or columns

	fImage = fImage(Rect(0, 0, fImage.cols & -2, fImage.rows & -2));

	int cx = fImage.cols/2;
	int cy = fImage.rows/2;

	// rearrange the quadrants of Fourier image
	// so that the origin is at the image center

	q0 = fImage(Rect(0, 0, cx, cy));
	q1 = fImage(Rect(cx, 0, cx, cy));
	q2 = fImage(Rect(0, cy, cx, cy));
	q3 = fImage(Rect(cx, cy, cx, cy));

	q0.copyTo(tmp);
	q3.copyTo(q0);
	tmp.copyTo(q3);

	q1.copyTo(tmp);
	q2.copyTo(q1);
	tmp.copyTo(q2);
}

Mat calcMagnitude(Mat& complexImg)
{
	Mat planes[2];

	split(complexImg, planes);
	magnitude(planes[0], planes[1], planes[0]);

	Mat mag = (planes[0]).clone();
	mag += Scalar::all(1);
	log(mag, mag);

	DFTshift(mag);

	normalize(mag, mag, 0, 1, CV_MINMAX);

	return mag;

}
