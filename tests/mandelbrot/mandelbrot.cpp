#include <iostream>
#include <vector>
#include <thread>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "../../bf.hpp"

struct color{
	uint8_t r, g, b;
};

template<class T> color computeMandelbrotPixel(const std::pair<T, T>& pos){
	constexpr unsigned int max_iteration = 4;
	T x2((int)0);
	T y2((int)0);
	T x((int)0);
	T y((int)0);
	T four(4);
	T one = T(1);
	int iteration = 0;
	/*
	while(x2 + y2 <= four && iteration < max_iteration){
		y = (x + x) * y + pos.second;
		x = x2 - y2 + pos.first;
		x2 = x * x;
		y2 = y * y;
		iteration++;
	}
	*/

	//float output = ((float)iteration / max_iteration);
	int output = (((pos.first * pos.first) + (pos.second * pos.second)) < one);
	std::cout<<pos.first.to_string()<<"^2 + "<<pos.second.to_string()<<"^2 < one = "<<output<<std::endl;
	uint8_t out_real = (uint8_t)(output * 255);
	return {out_real, out_real, out_real};
}

template<class T> void mandelbrotRow(const std::pair<T, T>& x_extent, const T& y, int j, unsigned int xsize, std::vector<std::vector<color>>& output){
	for(unsigned int i=0;i<xsize;i++){
		T x_norm = (T(i) / T(xsize));
		T x = x_extent.first + (x_norm * (x_extent.second - x_extent.first));
		output[i][j] = computeMandelbrotPixel<T>({x, y});
	}
}
template<class T> std::vector<std::vector<color>> mandelbrot(const std::pair<T, T>& x_extent, const std::pair<T, T>& y_extent, const std::pair<unsigned int, unsigned int>& size){
	std::vector<std::vector<color>> output;
	output.resize(size.first);
	for(auto& o : output)
		o.resize(size.second);

	unsigned int threadsToUse = 1;std::thread::hardware_concurrency();
	unsigned int rowsPerThread = size.second / threadsToUse;
	unsigned int leftoverRows = size.second % rowsPerThread;//will just be put on the first thread;
	
	std::vector<std::thread> threads;
	unsigned int currentRow = 0;
	for(unsigned int tr=0;tr<threadsToUse;tr++){
		unsigned int rowsToDo = rowsPerThread;
		if(tr == 0)
			rowsToDo += leftoverRows;
		threads.push_back(std::thread([currentRow, rowsToDo, size, &y_extent, &x_extent, &output](){
			for(int j=currentRow;j<currentRow+rowsToDo;j++){
				T y_norm = (T(j) / T(size.second));
				T y = y_extent.first + (y_norm * (y_extent.second - y_extent.first));
				mandelbrotRow<T>(x_extent, y, j, size.first, output);
			}
		}));
		currentRow += rowsToDo;
	}

	for(auto& o : threads)
		o.join();
	
	return output;
}

void writePNGImage(const std::vector<std::vector<color>>& img, const std::string& filename){
	if(img.size() == 0){
		std::cout<<"Tried to write image with zero width!"<<std::endl;
		return;
	}
	if(img[0].size() == 0){
		std::cout<<"Tried to write image with zero height!"<<std::endl;
		return;
	}

	std::vector<uint8_t> imageArray;
	imageArray.resize(img.size() * img[0].size() * 3);
	int image_temp_itor = 0;
	for(int j=0;j<img[0].size();j++){
		for(int i=0;i<img.size();i++){
			imageArray[image_temp_itor] = img[i][j].r;
			imageArray[image_temp_itor+1] = img[i][j].g;
			imageArray[image_temp_itor+2] = img[i][j].b;
			image_temp_itor+=3;
		}
	}

	stbi_write_png(filename.c_str(), img.size(), img[0].size(), 3, imageArray.data(), 0);
}

int main(){
	constexpr unsigned int bf_prec = 64;
	using bft = bf<bf_prec>;
	bft pos1dot5 = bft(3) / bft(2);
	bft neg1dot5 = bft(-3) / bft(2);
	constexpr std::pair<unsigned int, unsigned int> size = {16, 16};
	//auto refArray = mandelbrot<double>({-1.5, 1.5}, {-1.5, 1.5}, size);
	//writePNGImage(refArray, "reference.png");
	auto colArray = mandelbrot<bft>({neg1dot5, pos1dot5}, {neg1dot5, pos1dot5}, size);
	writePNGImage(colArray, "output.png");
}
