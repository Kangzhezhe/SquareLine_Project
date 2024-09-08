#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "network_1.h"
#include "network_1_data.h"
#include <opencv2/opencv.hpp>
#include <string>
#include <fstream>
using namespace std;
using namespace cv;

/**
 * @brief Statically allocated buffers.
 * Buffers can be dynamically allocated using malloc and size information
 * given by the report in ai_network_get_report().
 */
ai_u8 activations[AI_NETWORK_1_DATA_ACTIVATIONS_SIZE];
int8_t in_data[AI_NETWORK_1_IN_1_SIZE_BYTES];
int8_t out_data[AI_NETWORK_1_OUT_1_SIZE_BYTES];

/* AI buffer IO handlers */
ai_buffer *ai_input;
ai_buffer *ai_output;

ai_handle network_1;
ai_error err;
ai_network_report report;
const ai_handle acts[] = { activations };

int8_t* readImage(const string& img_path, int& len) {
    Mat img = imread(img_path, IMREAD_COLOR);
    if (img.empty()) {
        cerr << "Failed to read image from " << img_path << endl;
        exit(EXIT_FAILURE);
    }

    // assert(input_height == 112 && input_width == 96 && input_channels == 3);
    resize(img, img, Size(96, 112));
    // imwrite("output.jpg",img);
    // imshow("img", img);
    // waitKey(0);

    cvtColor(img, img, COLOR_BGR2RGB);

    // Convert image to float and normalize
    img.convertTo(img, CV_32FC3);
    cout<<img.at<Vec3f>(0,0)<<endl;
    img = (img - 128.0)/128 ;
    img = img*128.0;
    cout<<img.at<Vec3f>(0,0)<<endl;
    img.convertTo(img, CV_8SC3);

    

    len = img.total() * img.channels();
    int8_t* data = new int8_t[len];
    memcpy(data, img.ptr<int8_t>(), len * sizeof(int8_t));
    cout<<to_string(data[0])<<" "<<to_string(data[1])<<" "<<to_string(data[2])<<endl;

    return data;
}

void performInference(const string& img_path) {
    int len = 0; // Length of the processed image data
    int8_t* img_data = readImage(img_path, len);
    if (len != AI_NETWORK_1_IN_1_SIZE_BYTES) {
        cerr << "Image data size does not match network input size, check the processing steps." << endl;
        exit(EXIT_FAILURE); // Or handle the error as appropriate
    }

    // Copy the processed image data to in_data, assuming in_data is of the correct type and size
    memcpy(in_data, img_data, len);

    // Free the dynamically allocated memory if you're not using it beyond this point
    delete[] img_data;

    // Normalize, convert and/or quantize inputs if necessary...
    err = ai_network_1_create_and_init(&network_1, acts, NULL);
    if (err.type != AI_ERROR_NONE) {
        printf("ai init_and_create error\n");
        // return -1;
    }
    ai_input = ai_network_1_inputs_get(network_1, NULL);
    ai_output = ai_network_1_outputs_get(network_1, NULL);
    // Perform inference
    ai_input[0].data = AI_HANDLE_PTR(in_data);
    ai_output[0].data = AI_HANDLE_PTR(out_data);
    ai_i32 n_batch = ai_network_1_run(network_1, ai_input, ai_output);
    if (n_batch != 1) {
        ai_error err = ai_network_1_get_error(network_1);
        printf("ai run error %d, %d\n", err.type, err.code);
        exit(EXIT_FAILURE); // Or handle the error as appropriate
    }

    // Post-process the output results/predictions
    printf("Inference output..\n");
    for (int i = 0; i < AI_NETWORK_1_OUT_1_SIZE_BYTES; i++) {
        printf("%d,", out_data[i]+10);
    }
    printf("\n");
}


int AI_Init(){
   /* Create an instance of the model */
   err = ai_network_1_create_and_init(&network_1, acts, NULL);
    if (err.type != AI_ERROR_NONE) {
        printf("ai init_and_create error\n");
        return -1;
    }
    ai_input = ai_network_1_inputs_get(network_1, NULL);
    ai_output = ai_network_1_outputs_get(network_1, NULL);
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

int main(int argc, char const *argv[])
{
    
     /** @brief Initialize network_1 */
    
    

    // /** @brief {optional} for debug/log purpose */
    // if (ai_network_1_get_report(network_1, &report) != true) {
    //     printf("ai get report error\n");
    //     return -1;
    // }

    // printf("Model name      : %s\n", report.model_name);
    // printf("Model signature : %s\n", report.model_signature);

    // ai_input = &report.inputs[0];
    // ai_output = &report.outputs[0];
    // printf("input[0] : (%d, %d, %d)\n", AI_BUFFER_SHAPE_ELEM(ai_input, AI_SHAPE_HEIGHT),
    //                                     AI_BUFFER_SHAPE_ELEM(ai_input, AI_SHAPE_WIDTH),
    //                                     AI_BUFFER_SHAPE_ELEM(ai_input, AI_SHAPE_CHANNEL));
    // printf("output[0] : (%d, %d, %d)\n", AI_BUFFER_SHAPE_ELEM(ai_output, AI_SHAPE_HEIGHT),
    //                                      AI_BUFFER_SHAPE_ELEM(ai_output, AI_SHAPE_WIDTH),
    //                                      AI_BUFFER_SHAPE_ELEM(ai_output, AI_SHAPE_CHANNEL));

    Mat img(112, 96, CV_8UC3);
    
    readImageFromTxt(img, "D:/linux/TESTAI4/MDK-ARM/img.txt");
    imwrite("output.jpg",img);

    performInference("output.jpg");
    return 0;
}