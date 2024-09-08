#include <map>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include "tensorflow/lite/c/c_api.h"
#include "string.h"
using namespace std;
TfLiteTensor* input_tf;
TfLiteTensor* output_tf;
TfLiteInterpreter* interpreter; 

TfLiteTensor * getOutputTensorByName(TfLiteInterpreter * interpreter, const char * name)
{
	int count = TfLiteInterpreterGetOutputTensorCount(interpreter);
	for (int i = 0; i < count; ++i) {
		TfLiteTensor* ts = (TfLiteTensor*)TfLiteInterpreterGetOutputTensor(interpreter, i);
		if (!strcmp(ts->name, name)) {
			return ts;
		}
	}
	return nullptr;
}
TfLiteTensor * getInputTensorByName(TfLiteInterpreter * interpreter, const char * name)
{
	int count = TfLiteInterpreterGetInputTensorCount(interpreter);
	for (int i = 0; i < count; ++i) {
		TfLiteTensor* ts = TfLiteInterpreterGetInputTensor(interpreter, i);
		if (!strcmp(ts->name, name)) {
			return ts;
		}
	}
	return nullptr;
}

void initModel(string path ) {
	  
	TfLiteModel* model = TfLiteModelCreateFromFile(path.c_str()); 
	TfLiteInterpreterOptions* options = TfLiteInterpreterOptionsCreate();
	interpreter = TfLiteInterpreterCreate(model, options); 
	if (interpreter == nullptr) {
		printf("Failed to create interpreter");
		cout << (path) << endl;
		return  ;
	}
	// Allocate tensor buffers.
	if (TfLiteInterpreterAllocateTensors(interpreter) != kTfLiteOk) {
		printf("Failed to allocate tensors!");
		return  ;
	} 

	input_tf = getInputTensorByName(interpreter, "input"); 
	output_tf = getOutputTensorByName(interpreter, "MobilenetV3/Predictions/Softmax"); 
  
}

void forward(float* data, int len) {
	TfLiteTensorCopyFromBuffer(input_tf, data, len*sizeof(float));
	TfLiteInterpreterInvoke(interpreter);
	float logits[1001];
	TfLiteTensorCopyToBuffer(output_tf, logits, 1001*sizeof(float));
	float maxV = -1;
	int maxIdx = -1;
	for (int i = 0; i < 1001; ++i) {
		if (logits[i] > maxV) {
			maxV = logits[i];
			maxIdx = i;
		}
		//printf("%d->%f\n", i, logits[i]);
	}
	cout << "Àà±ð£º" << maxIdx << "£¬¸ÅÂÊ£º" << maxV << endl;
}
long getSize(string path) {
	ifstream file(path, ios::in | ios::binary); 
	long l, m;
	l = file.tellg();
	file.seekg(0, ios::end);
	m = file.tellg();
	file.close();
	return m - l;
}

float* readBmp(string path, int& len) {
	len = getSize(path);
	unsigned char* buff = (unsigned char*)calloc(len, sizeof(unsigned char*));
	ifstream fin(path, std::ifstream::binary);
	fin.read(reinterpret_cast<char*>(buff), len *sizeof(unsigned char*));
	fin.close();
	float* data = (float*)calloc(len, sizeof(float));
	for (int i = 0; i < len; ++i) {
		data[i] = (buff[i]/255.0-0.5)*2;
	}
	free(buff);
	return data;
}
 
int main()
{    
	initModel("v3-small_224_0.75_float.tflite" );
	int size=0;
	float* bmp = readBmp("input.bin", size);
	forward(bmp, size );
}
 