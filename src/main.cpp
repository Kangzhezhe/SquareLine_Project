#include <cstdlib>
#include <fstream>
#include <sstream>
#include <iostream>
#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <string>
#include "tensorflow/lite/c/c_api.h"
#include <boost/filesystem.hpp>
#include <string>
#include <vector>
#include "random"

float getRandomFloat(float min, float max) {
    // 随机数生成器，使用随机设备作为种子
    std::random_device rd;
    std::mt19937 gen(rd());
    // 定义范围在[min, max]之间的均匀分布
    std::uniform_real_distribution<> dis(min, max);
    return dis(gen);
}

using namespace cv;
using namespace dnn;
using namespace std;

void performInference(const string& img_path,bool enhance=false) ;
class YOLOv8_face
{
public:
	YOLOv8_face(string modelpath, float confThreshold, float nmsThreshold);
	void detect(Mat& frame);
private:
	Mat resize_image(Mat srcimg, int *newh, int *neww, int *padh, int *padw);
	const bool keep_ratio = true;
	const int inpWidth = 640;
	const int inpHeight = 640;
	float confThreshold;
	float nmsThreshold;
	const int num_class = 1;  ///只有人脸这一个类别
	const int reg_max = 16;
	Net net;
	void softmax_(const float* x, float* y, int length);
	void generate_proposal(Mat out, vector<Rect>& boxes, vector<float>& confidences, vector< vector<Point>>& landmarks, int imgh, int imgw, float ratioh, float ratiow, int padh, int padw);
	void drawPred(float conf, int left, int top, int right, int bottom, Mat& frame, vector<Point> landmark);
};

static inline float sigmoid_x(float x)
{
	return static_cast<float>(1.f / (1.f + exp(-x)));
}

YOLOv8_face::YOLOv8_face(string modelpath, float confThreshold, float nmsThreshold)
{
	this->confThreshold = confThreshold;
	this->nmsThreshold = nmsThreshold;
	this->net = readNet(modelpath);
}

Mat YOLOv8_face::resize_image(Mat srcimg, int *newh, int *neww, int *padh, int *padw)
{
	int srch = srcimg.rows, srcw = srcimg.cols;
	*newh = this->inpHeight;
	*neww = this->inpWidth;
	Mat dstimg;
	if (this->keep_ratio && srch != srcw) {
		float hw_scale = (float)srch / srcw;
		if (hw_scale > 1) {
			*newh = this->inpHeight;
			*neww = int(this->inpWidth / hw_scale);
			resize(srcimg, dstimg, Size(*neww, *newh), INTER_AREA);
			*padw = int((this->inpWidth - *neww) * 0.5);
			copyMakeBorder(dstimg, dstimg, 0, 0, *padw, this->inpWidth - *neww - *padw, BORDER_CONSTANT, 0);
		}
		else {
			*newh = (int)this->inpHeight * hw_scale;
			*neww = this->inpWidth;
			resize(srcimg, dstimg, Size(*neww, *newh), INTER_AREA);
			*padh = (int)(this->inpHeight - *newh) * 0.5;
			copyMakeBorder(dstimg, dstimg, *padh, this->inpHeight - *newh - *padh, 0, 0, BORDER_CONSTANT, 0);
		}
	}
	else {
		resize(srcimg, dstimg, Size(*neww, *newh), INTER_AREA);
	}
	return dstimg;
}

void YOLOv8_face::drawPred(float conf, int left, int top, int right, int bottom, Mat& frame, vector<Point> landmark)   // Draw the predicted bounding box
{
	//Draw a rectangle displaying the bounding box
	rectangle(frame, Point(left, top), Point(right, bottom), Scalar(0, 0, 255), 3);

	//Get the label for the class name and its confidence
	string label = format("face:%.2f", conf);

	//Display the label at the top of the bounding box
	/*int baseLine;
	Size labelSize = getTextSize(label, FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
	top = max(top, labelSize.height);
	rectangle(frame, Point(left, top - int(1.5 * labelSize.height)), Point(left + int(1.5 * labelSize.width), top + baseLine), Scalar(0, 255, 0), FILLED);*/
	putText(frame, label, Point(left, top-5), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 255, 0), 2);
	for (int i = 0; i < 5; i++)
	{
		circle(frame, landmark[i], 4, Scalar(0, 255, 0), -1);
	}
}

void YOLOv8_face::softmax_(const float* x, float* y, int length)
{
	float sum = 0;
	int i = 0;
	for (i = 0; i < length; i++)
	{
		y[i] = exp(x[i]);
		sum += y[i];
	}
	for (i = 0; i < length; i++)
	{
		y[i] /= sum;
	}
}

void YOLOv8_face::generate_proposal(Mat out, vector<Rect>& boxes, vector<float>& confidences, vector< vector<Point>>& landmarks, int imgh,int imgw, float ratioh, float ratiow, int padh, int padw)
{
	const int feat_h = out.size[2];
	const int feat_w = out.size[3];
	// cout << out.size[1] << "," << out.size[2] << "," << out.size[3] << endl;
	const int stride = (int)ceil((float)inpHeight / feat_h);
	const int area = feat_h * feat_w;
	float* ptr = (float*)out.data;
	float* ptr_cls = ptr + area * reg_max * 4;
	float* ptr_kp = ptr + area * (reg_max * 4 + num_class);

	for (int i = 0; i < feat_h; i++)
	{
		for (int j = 0; j < feat_w; j++)
		{
			const int index = i * feat_w + j;
			int cls_id = -1;
			float max_conf = -10000;
			for (int k = 0; k < num_class; k++)
			{
				float conf = ptr_cls[k*area + index];
				if (conf > max_conf)
				{
					max_conf = conf;
					cls_id = k;
				}
			}
			float box_prob = sigmoid_x(max_conf);
			if (box_prob > this->confThreshold)
			{
				float pred_ltrb[4];
				float* dfl_value = new float[reg_max];
				float* dfl_softmax = new float[reg_max];
				for (int k = 0; k < 4; k++)
				{
					for (int n = 0; n < reg_max; n++)
					{
						dfl_value[n] = ptr[(k*reg_max + n)*area + index];
					}
					softmax_(dfl_value, dfl_softmax, reg_max);

					float dis = 0.f;
					for (int n = 0; n < reg_max; n++)
					{
						dis += n * dfl_softmax[n];
					}

					pred_ltrb[k] = dis * stride;
				}
				float cx = (j + 0.5f)*stride;
				float cy = (i + 0.5f)*stride;
				float xmin = max((cx - pred_ltrb[0] - padw)*ratiow, 0.f);  ///还原回到原图
				float ymin = max((cy - pred_ltrb[1] - padh)*ratioh, 0.f);
				float xmax = min((cx + pred_ltrb[2] - padw)*ratiow, float(imgw - 1));
				float ymax = min((cy + pred_ltrb[3] - padh)*ratioh, float(imgh - 1));
				Rect box = Rect(int(xmin), int(ymin), int(xmax - xmin), int(ymax - ymin));
				boxes.push_back(box);
				confidences.push_back(box_prob);

				vector<Point> kpts(5);
				for (int k = 0; k < 5; k++)
				{
					float x = ((ptr_kp[(k * 3)*area + index] * 2 + j)*stride - padw)*ratiow;  ///还原回到原图
					float y = ((ptr_kp[(k * 3 + 1)*area + index] * 2 + i)*stride - padh)*ratioh;
					///float pt_conf = sigmoid_x(ptr_kp[(k * 3 + 2)*area + index]);
					kpts[k] = Point(int(x), int(y));
				}
				landmarks.push_back(kpts);
			}
		}
	}
}

TfLiteInterpreter* interpreter;
TfLiteTensor* input_tensor;
TfLiteTensor* output_tensor;
string dir = "";
string model_path = dir + "mobilefacenet.tflite";
string img_path = dir + "input.jpg";
string output_file_path =dir +  "output/output.txt";

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
        ofs << std::to_string((char)(value / 0.013797775842249393 - 10)) << " ";
    }
    ofs.close();
}
vector<vector<char>> features;

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

    vector<char>char_data;
    for(int i = 0 ;i<output_size;i++){
        // char_data.push_back((char)(output_data[i] / 0.013797775842249393 - 10));
        char_data.push_back((char)(output_data[i] / 0.013797775842249393 ));
    }
    features.push_back(char_data);

    
    ofstream ofs(output_file_path);
    if (!ofs.is_open()) {
        cerr << "Failed to open file " << output_file_path << endl;
        exit(EXIT_FAILURE);
    }
    for (char value : features.back()) {
        ofs << std::to_string(value) << " ";
    }
    ofs.close();

    // saveFeaturesToFile(output_data, output_file_path);
}

extern void enhanceImageUsingHSV(uint8_t *img, int height, int width,float *hsv_img, uint8_t* img_out,float threshold);

string labelFilename = "output/label.txt";
vector<string> labels;
void save_labels() {
    ofstream ofs(labelFilename);
    if (!ofs.is_open()) {
        cerr << "Failed to open file " << labelFilename << endl;
        exit(EXIT_FAILURE);
    }
    for (string label : labels) {
        ofs << label << endl;
    }
    ofs.close();
}

void randomROI(const Rect& box, const Size& imgSize, Rect& newBox, float minScale=0.1, float maxScale=0.28, float shiftRange=0.1) {
    // 随机生成缩放比例，范围在 1 + minScale 和 1 + maxScale 之间
    float scale = 1 + minScale + (rand() / (float)RAND_MAX) * (maxScale - minScale);
    int newWidth = int(box.width * scale);
    int newHeight = int(box.height * scale);

    // 随机生成水平和垂直位移，可以为负值
    int xShift = int((rand() / (float)RAND_MAX - 0.5) * 2 * box.width * shiftRange);
    int yShift = int((rand() / (float)RAND_MAX - 0.5) * 2 * box.height * shiftRange);

    // 计算中心位置
    int xCenter = box.x + box.width / 2;
    int yCenter = box.y + box.height / 2;

    // 计算新的位置，保持中心位置
    int x = xCenter + xShift - newWidth / 2;
    int y = yCenter + yShift - newHeight / 2;

    // 确保新的位置不超出边界
    x = std::max(0, std::min(x, imgSize.width - newWidth));
    y = std::max(0, std::min(y, imgSize.height - newHeight));

    newBox = Rect(x, y, newWidth, newHeight);
}

int num = 0;
int output_cnt = 0;
void YOLOv8_face::detect(Mat& srcimg){
	int newh = 0, neww = 0, padh = 0, padw = 0;
	Mat dst = this->resize_image(srcimg, &newh, &neww, &padh, &padw);
	Mat blob;
	blobFromImage(dst, blob, 1 / 255.0, Size(this->inpWidth, this->inpHeight), Scalar(0, 0, 0), true, false);
	this->net.setInput(blob);
	vector<Mat> outs;
	///net.enableWinograd(false);  ////如果是opencv4.7，那就需要加上这一行
	this->net.forward(outs, this->net.getUnconnectedOutLayersNames());
    
	/////generate proposals
	vector<Rect> boxes;
	vector<float> confidences;
	vector< vector<Point>> landmarks;
	float ratioh = (float)srcimg.rows / newh, ratiow = (float)srcimg.cols / neww;

	generate_proposal(outs[0], boxes, confidences, landmarks, srcimg.rows, srcimg.cols, ratioh, ratiow, padh, padw);
	generate_proposal(outs[1], boxes, confidences, landmarks, srcimg.rows, srcimg.cols, ratioh, ratiow, padh, padw);
	generate_proposal(outs[2], boxes, confidences, landmarks, srcimg.rows, srcimg.cols, ratioh, ratiow, padh, padw);

	// Perform non maximum suppression to eliminate redundant overlapping boxes with
	// lower confidences
    
	vector<int> indices;
	NMSBoxes(boxes, confidences, this->confThreshold, this->nmsThreshold, indices);
	for (size_t i = 0; i < indices.size(); ++i)
	{
		int idx = indices[i];
		Rect box = boxes[idx];
		// this->drawPred(confidences[idx], box.x, box.y,
		// 	box.x + box.width, box.y + box.height, srcimg, landmarks[idx]);
         
        // Crop the detected face region
        Mat faceROI = srcimg(box);
        // Resize the face region to 112x96
        Mat resizedFace;
        resize(faceROI, resizedFace, Size(96, 112));

         // Save the resized face region
        string faceFilename = format("output/%02d/output_%d.jpg",(int)num, (int)output_cnt++);
        string path = faceFilename.substr(0, faceFilename.find_last_of("/\\") + 1);
        if (!boost::filesystem::exists(path) ){
            boost::filesystem::create_directories(path);
        }
        labels.push_back(faceFilename.substr(faceFilename.find_first_of("/\\") + 1, faceFilename.length()) + " " + to_string(num));       
        imwrite(faceFilename ,resizedFace);

        faceFilename = format("output/%02d/output_%d.jpg",(int)num, (int)output_cnt++);
        path = faceFilename.substr(0, faceFilename.find_last_of("/\\") + 1);
        if (!boost::filesystem::exists(path) ){
            boost::filesystem::create_directories(path);
        }
        labels.push_back(faceFilename.substr(faceFilename.find_first_of("/\\") + 1, faceFilename.length()) + " " + to_string(num));      
        // 将图像从BGR转换到HSV颜色空间
        // Mat imgHSV;
        // cvtColor(resizedFace, imgHSV, COLOR_BGR2HSV);
        // std::vector<Mat> channels;
        // split(imgHSV, channels);
        // equalizeHist(channels[2], channels[2]);
        // merge(channels, imgHSV);
        // cvtColor(imgHSV, resizedFace, COLOR_HSV2BGR);

        float *hsv_img = new float[3 * resizedFace.rows * resizedFace.cols];
        enhanceImageUsingHSV(resizedFace.data, resizedFace.rows, resizedFace.cols,hsv_img, resizedFace.data,0.1);
        // cvtColor(resizedFace, resizedFace, COLOR_HSV2BGR);
        delete []hsv_img;

        imwrite(faceFilename ,resizedFace);
        imwrite("output/output.jpg" ,resizedFace);

        performInference("output/output.jpg");

        for (int j = 0; j < 10; ++j) { // Generate 5 augmented images
            Rect newBox;
            randomROI(box, srcimg.size(), newBox);
            Mat augFaceROI = srcimg(newBox);
            resize(augFaceROI, resizedFace, Size(96, 112));

            faceFilename = format("output/%02d/output_%d.jpg", (int)num, (int)output_cnt++);
            path = faceFilename.substr(0, faceFilename.find_last_of("/\\") + 1);
            if (!boost::filesystem::exists(path)) {
                boost::filesystem::create_directories(path);
            }
            labels.push_back(faceFilename.substr(faceFilename.find_first_of("/\\") + 1, faceFilename.length()) + " " + to_string(num));

            float *hsv_img = new float[3 * resizedFace.rows * resizedFace.cols];
            enhanceImageUsingHSV(resizedFace.data, resizedFace.rows, resizedFace.cols, hsv_img, resizedFace.data,getRandomFloat(0.05,0.15));
            delete[] hsv_img;

            imwrite(faceFilename, resizedFace);
        }
        break;
	}
}
std::vector<std::string> getAllImagePaths(const std::string& folderPath) {
    std::vector<std::string> imagePaths;

    boost::filesystem::path path(folderPath);
    if (!boost::filesystem::exists(path) || !boost::filesystem::is_directory(path)) {
        std::cout << "Invalid directory path" << std::endl;
        return imagePaths;
    }

    boost::filesystem::directory_iterator end_itr;
    for (boost::filesystem::directory_iterator itr(path); itr != end_itr; ++itr) {
        if (boost::filesystem::is_regular_file(itr->path())) {
            std::string filename = itr->path().filename().string();
            if (filename.find(".jpg") != std::string::npos || filename.find(".png") != std::string::npos|| filename.find(".bmp") != std::string::npos) {
                imagePaths.push_back(itr->path().string());
            }
        }
    }

    return imagePaths;
}

std::vector<std::string> getAllSubdirectories(const std::string& folderPath) {
    std::vector<std::string> subdirectories;
    for (const auto& entry : boost::filesystem::directory_iterator(folderPath)) {
        if (boost::filesystem::is_directory(entry.path())) {
            subdirectories.push_back(entry.path().string());
        }
    }
    return subdirectories;
}

void write_features_to_file(){
    // static std::vector<std::vector<char>> features = {{'a', 'b', 'c'}, {'d', 'e', 'f'}, {'g', 'h', 'i'}};
    // Flatten the vector<vector<char>> into a single vector<char>
    std::vector<char> flattenedFeatures;
    for (const auto& vec : features) {
        flattenedFeatures.insert(flattenedFeatures.end(), vec.begin(), vec.end());
    }

    // Write the flattened vector<char> to a file
    std::ofstream outFile("output/features.bin", std::ios::binary);
    if (outFile.is_open()) {
        outFile.write(flattenedFeatures.data(), flattenedFeatures.size());
        outFile.close();
        std::cout << "Vector<vector<char>> stored in file." << std::endl;
    } else {
        std::cerr << "Unable to open file." << std::endl;
    }
}

void read_features_from_file(){
    // Read the flattened vector<char> from the file
    std::ifstream inFile("output/features.bin", std::ios::binary);
    if (inFile.is_open()) {
        // Get the size of the file
        inFile.seekg(0, std::ios::end);
        std::size_t fileSize = inFile.tellg();
        inFile.seekg(0, std::ios::beg);

        // Create a vector<char> to hold the data
        std::vector<char> flattenedFeatures(fileSize);

        // Read the data from the file into the vector
        inFile.read(flattenedFeatures.data(), fileSize);
        inFile.close();

        // Reshape the flattened vector<char> back into a vector<vector<char>>
        // std::vector<std::vector<char>> features1;
        std::size_t currentIndex = 0;
        while (currentIndex < flattenedFeatures.size()) {
            std::vector<char> vec;
            for (std::size_t i = 0; i < 128; i++) {
                vec.push_back(flattenedFeatures[currentIndex++]);
            }
            features.push_back(vec);
        }
        // cout<<features1.size()<<endl;
        // cout<<features1[0].size()<<endl;
        // // Compare the elements of features1 and features
        // for (int i = 0; i < features1.size(); i++) {
        //     for (int j = 0; j < features1[i].size(); j++) {
        //         if (features1[i][j] != features[i][j]) {
        //             std::cout << "Element mismatch at position (" << i << ", " << j << "): "
        //                     << features1[i][j] << " != " << features[i][j] << std::endl;
        //         }
        //     }
        // }
    } else {
        std::cerr << "Unable to open file." << std::endl;
    }
}

int max_num = 20;

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "network_1.h"
#include "network_1_data.h"
#include <opencv2/opencv.hpp>
#include <string>
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
    cvtColor(img, img, COLOR_BGR2RGB);

    // Convert image to float and normalize
    img.convertTo(img, CV_32FC3);
    img = (img - 128.0) /128;
    img = img*128.0;
    img.convertTo(img, CV_8SC3);

    len = img.total() * img.channels();
    int8_t* data = new int8_t[len];
    memcpy(data, img.ptr<int8_t>(), len * sizeof(int8_t));

    return data;
}

bool init = 0;
void performInference(const string& img_path,bool enhance) {
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
    if(!init){
    err = ai_network_1_create_and_init(&network_1, acts, NULL);
    if (err.type != AI_ERROR_NONE) {
        printf("ai init_and_create error\n");
        // return -1;
    }
    ai_input = ai_network_1_inputs_get(network_1, NULL);
    ai_output = ai_network_1_outputs_get(network_1, NULL);
    init = 1;
    }
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
    // printf("Inference output..\n");
    // for (int i = 0; i < AI_NETWORK_1_OUT_1_SIZE_BYTES; i++) {
    //     printf("%d,", out_data[i]);
    // }
    // printf("\n");

    if(!enhance){
        vector<char>char_data;
        for(int i = 0 ;i<AI_NETWORK_1_OUT_1_SIZE_BYTES;i++){
            char_data.push_back((char)(out_data[i])+10);
        }
        features.push_back(char_data);
    }

    
    ofstream ofs(output_file_path);
    if (!ofs.is_open()) {
        cerr << "Failed to open file " << output_file_path << endl;
        exit(EXIT_FAILURE);
    }
    for (char value : features.back()) {
        ofs << std::to_string(value) << " ";
    }
    ofs.close();
}


int main1(){
    srand((unsigned)time(NULL));   
    features = {};
    initModel(model_path);
	YOLOv8_face YOLOv8_face_model("weights/yolov8n-face.onnx", 0.45, 0.5);
	
	// static const string kWinName = "Deep learning face detection use OpenCV";
	// namedWindow(kWinName, WINDOW_NORMAL);
	// imshow(kWinName, srcimg);
    cout<<"hello world"<<endl;
    std::string dir = "./dataset";

    output_cnt=0;
    num = 0;


    // std::vector<std::string> subdirectories = getAllSubdirectories(dir);
    // std::string folderPath = subdirectories[0];
    // std::vector<std::string> images = getAllImagePaths(folderPath);
    // string imgpath = "images/test.jpg";
	// Mat srcimg = imread(images[0]);
	// YOLOv8_face_model.detect(srcimg);


    std::vector<std::string> subdirectories =  getAllSubdirectories(dir);;
    std::vector<std::string> images;
    
    for (const std::string& subdirectory : subdirectories) {
        std::cout << subdirectory << std::endl;
        images = getAllImagePaths(subdirectory);
        int counter = 0;
        for (const std::string& imgpath : images) {
            cout << imgpath << endl;
            Mat srcimg = imread(imgpath);
            YOLOv8_face_model.detect(srcimg);
            counter++;
            if(counter == 4){
                break;
            }
        }
        output_cnt=0;
        num++;
        if(num == max_num){
            break;
        }
    }

    cout<<features.size()<<endl;
    cout<<features[0].size()<<endl;

    
    write_features_to_file();
    save_labels();

    TfLiteInterpreterDelete(interpreter);
	// waitKey(0);
	// destroyAllWindows();
    return 0;
}
