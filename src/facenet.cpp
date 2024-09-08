#include <cstdint>
#include <iostream>
#include <fstream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <string>
#include "tensorflow/lite/c/c_api.h"

using namespace std;
using namespace cv;

TfLiteInterpreter* interpreter;
TfLiteTensor* input_tensor;
TfLiteTensor* output_tensor;

TfLiteTensor* getTensorByName(TfLiteInterpreter* interpreter, const char* name, bool is_input) {
    int count = is_input ? TfLiteInterpreterGetInputTensorCount(interpreter) : TfLiteInterpreterGetOutputTensorCount(interpreter);
    for (int i = 0; i < count; ++i) {
        TfLiteTensor* tensor = is_input ? TfLiteInterpreterGetInputTensor(interpreter, i) : ((TfLiteTensor*)TfLiteInterpreterGetOutputTensor(interpreter, i));
        // if (strcmp(TfLiteTensorName(tensor), name) == 0) {
            return tensor;
        // }
    }
    return nullptr;
}

void initModel(const string& model_path) {
    TfLiteModel* model = TfLiteModelCreateFromFile(model_path.c_str());
    TfLiteInterpreterOptions* options = TfLiteInterpreterOptionsCreate();
    interpreter = TfLiteInterpreterCreate(model, options);
    if (interpreter == nullptr) {
        cerr << "Failed to create interpreter" << endl;
        exit(EXIT_FAILURE);
    }

    if (TfLiteInterpreterAllocateTensors(interpreter) != kTfLiteOk) {
        cerr << "Failed to allocate tensors!" << endl;
        exit(EXIT_FAILURE);
    }

    input_tensor = getTensorByName(interpreter, "input", true);
    output_tensor = getTensorByName(interpreter, "output", false);

    if (input_tensor == nullptr || output_tensor == nullptr) {
        cerr << "Failed to find input or output tensor!" << endl;
        exit(EXIT_FAILURE);
    }

    TfLiteInterpreterOptionsDelete(options);
    TfLiteModelDelete(model);
}

float* readImage(const string& img_path, int& len, const TfLiteTensor* input_tensor) {
    Mat img = imread(img_path, IMREAD_COLOR);
    if (img.empty()) {
        cerr << "Failed to read image from " << img_path << endl;
        exit(EXIT_FAILURE);
    }

    // Resize the image to the expected input size
    int input_height = TfLiteTensorDim(input_tensor, 1);
    int input_width = TfLiteTensorDim(input_tensor, 2);
    int input_channels = TfLiteTensorDim(input_tensor, 3);

    assert(input_height == 112 && input_width == 96 && input_channels == 3);
    resize(img, img, Size(input_width, input_height));
    imwrite("output.jpg",img);
    cvtColor(img, img, COLOR_BGR2RGB);

    // Convert image to float and normalize
    img.convertTo(img, CV_32FC3);
    img = (img - 128.0) / 128.0;

    len = img.total() * img.channels();
    float* data = new float[len];
    memcpy(data, img.ptr<float>(), len * sizeof(float));

    return data;
}

void saveFeaturesToFile(const vector<float>& features, const string& file_path) {
    ofstream ofs(file_path);
    if (!ofs.is_open()) {
        cerr << "Failed to open file " << file_path << endl;
        exit(EXIT_FAILURE);
    }

    for (float value : features) {
        ofs << std::to_string((char)(value / 0.013797775842249393 )) << " ";
    }
    ofs.close();
}

void runModel(float* data, int len, const string& output_file_path) {
    if (TfLiteTensorCopyFromBuffer(input_tensor, data, len * sizeof(float)) != kTfLiteOk) {
        cerr << "Failed to copy data to input tensor!" << endl;
        exit(EXIT_FAILURE);
    }

    if (TfLiteInterpreterInvoke(interpreter) != kTfLiteOk) {
        cerr << "Failed to invoke interpreter!" << endl;
        exit(EXIT_FAILURE);
    }

    int output_size = TfLiteTensorByteSize(output_tensor) / sizeof(float);
    vector<float> output_data(output_size);
    if (TfLiteTensorCopyToBuffer(output_tensor, output_data.data(), output_size * sizeof(float)) != kTfLiteOk) {
        cerr << "Failed to copy data from output tensor!" << endl;
        exit(EXIT_FAILURE);
    }
    
    saveFeaturesToFile(output_data, output_file_path);
}

void readImageFromTxt(cv::Mat& image,const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Unable to open file for reading: " << filename << std::endl;
        return ;
    }
    
    for (int i = 0; i < image.rows; ++i) {
        for (int j = 0; j < image.cols; ++j) {
            int b, g, r;
            file >> r>>g>>b;
            image.at<cv::Vec3b>(i, j) = cv::Vec3b(b, g, r);
        }
    }

    file.close();
    return ;
}

int main() {
    string dir = "bin/";
    string model_path = dir + "mobilefacenet.tflite";
    string img_path = dir + "input.jpg";
    string output_file_path =dir +  "output_facenet.txt";

    cout << "Hello, World!" << endl;
    initModel(model_path);

    //float* img_data = readImage(img_path, img_len, input_tensor);
    Mat img(112, 96, CV_8UC3);
    readImageFromTxt(img, "D:/linux/TESTAI4/MDK-ARM/img.txt");
    cvtColor(img, img, COLOR_BGR2RGB);
    // Convert image to float and normalize
    img.convertTo(img, CV_32FC3);
    img = (img - 128.0) / 128.0;

    int len = img.total() * img.channels();
    float* data = new float[len];
    memcpy(data, img.ptr<float>(), len * sizeof(float));
    runModel(data, len, output_file_path);
    delete []data;

    // delete[] img_data;
    TfLiteInterpreterDelete(interpreter);

    return 0;
}
