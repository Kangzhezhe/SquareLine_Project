#include <stdio.h>
#include "math.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;
using namespace cv;

//-------以下是HSV颜色空间和RGB颜色空间相互转换接口------------
/**
 * @brief HSV颜色空间
*/
typedef struct {
    float h;    // 色调H(hue)  0~360° {R(0°),G(120°),B(240°)}
    float s;    // 饱和度S(saturation)  0~1.0
    float v;    // 明度V(value)  0~1.0  
}color_hsv_t;

typedef struct {
    unsigned char r;    // 0-255
    unsigned char g;    // 0-255
    unsigned char b;    // 0-255
    unsigned int color; // 0x000000 ~ 0xffffff
}color_rgb_t;


/**
 * @brief   RGB颜色空间 转  HSV颜色空间 
 * @param   rgb:RGB颜色空间参数
 * @param   hsv:HSV颜色空间参数
 * @return  none
*/
void rgb2hsv(color_rgb_t *rgb, color_hsv_t *hsv);

/**
 * @brief   HSV颜色空间 转 RGB颜色空间
 * @param   hsv:HSV颜色空间参数
 * @param   rgb:RGB颜色空间参数
 * @return  none
*/
void hsv2rgb(color_hsv_t *hsv, color_rgb_t *rgb);

/**
 * @brief   调节HSV颜色空间 H值(0.0~360.0)
 * @param   hsv:HSV颜色空间参数
 * @param   h_offset:调节参数 增加>0.0，减小<0.0
 * @return  none
*/
void hsv_adjust_h(color_hsv_t *hsv, float h_offset);

/**
 * @brief   调节HSV颜色空间 S值(0.0~1.0)
 * @param   hsv:HSV颜色空间参数
 * @param   s_offset:调节参数 增加>0.0，减小<0.0
 * @return  none
*/
void hsv_adjust_s(color_hsv_t *hsv, float s_offset);

/**
 * @brief   调节HSV颜色空间 V值(0.0~1.0)
 * @param   hsv:HSV颜色空间参数
 * @param   v_offset:调节参数 增加>0.0，减小<0.0
 * @return  none
*/
void hsv_adjust_v(color_hsv_t *hsv, float v_offset);



// 以下是HSV颜色空间和RGB颜色空间相互转换接口
/*********************************************************************************************
红：    
    R   255         H:0
    G   0           S:100
    B   0           V:100    
绿：
    R   0           H:120   
    G   255         S:100
    B   0           V:100    
蓝：
    R   0           H:240   
    G   0           S:100
    B   255         V:100    
*********************************************************************************************/

#define max(a,b) ((a) > (b) ? (a) : (b))
#define min(a,b) ((a) < (b) ? (a) : (b))
#define max3(a,b,c) (((a) > (b) ? (a) : (b)) > (c) ? ((a) > (b) ? (a) : (b)) : (c))
#define min3(a,b,c) (((a) < (b) ? (a) : (b)) < (c) ? ((a) < (b) ? (a) : (b)) : (c))


/*********************************************************************************************
RGB转化到HSV的算法:
    max=max(R,G,B)；
    min=min(R,G,B)；
    V=max(R,G,B)；
    S=(max-min)/max；
    if (R = max) H =(G-B)/(max-min)* 60；
    if (G = max) H = 120+(B-R)/(max-min)* 60；
    if (B = max) H = 240 +(R-G)/(max-min)* 60；
    if (H < 0) H = H + 360；
***********************************************************************************************/

/**
 * @brief   RGB颜色空间 转  HSV颜色空间 
 * @param   rgb:RGB颜色空间参数
 * @param   hsv:HSV颜色空间参数
 * @note    The R,G,B values are divided by 255 to change the range from 0..255 to 0..1:
 * @return  none
*/
void rgb2hsv(color_rgb_t *rgb, color_hsv_t *hsv)
{
    float max, min, delta=0;
    float r = (float)((float)((int)rgb->r)/255);
    float g = (float)((float)((int)rgb->g)/255);
    float b = (float)((float)((int)rgb->b)/255);

    max = max3(r, g, b);
    min = min3(r, g, b);
    delta = (max - min);

    //printf("r:%f, g:%f, b:%f\n", r, g, b);

    if (delta == 0) {
        hsv->h = 0;
    } else {
        if (r == max) {
            hsv->h = ((g-b)/delta)*60; 
        } else if (g == max) {
            hsv->h = 120+(((b-r)/delta)*60); 
        } else if (b == max) {
            hsv->h = 240 + (((r-g)/delta)*60);
        }

        if (hsv->h < 0) {
            hsv->h += 360;
        }
    }

    if (max == 0) {
        hsv->s = 0;
    } else {
        hsv->s = (float)(delta/max);
    }

    hsv->v = max;

    rgb->color = (unsigned int)(((rgb->r&0xff)<<16) | ((rgb->g&0xff)<<8) | (rgb->b&0xff));
}


/*************************************************************************
HSV转化到RGB的算法:
    if (s = 0)
    R=G=B=V;
    else
    H /= 60;
    i = INTEGER(H);
    f = H - i;
    a = V * ( 1 - s );
    b = V * ( 1 - s * f );
    c = V * ( 1 - s * (1 - f ) );
    switch(i)
    case 0: R = V; G = c; B = a;
    case 1: R = b; G = v; B = a;
    case 2: R = a; G = v; B = c;
    case 3: R = a; G = b; B = v;
    case 4: R = c; G = a; B = v;
    case 5: R = v; G = a; B = b;
*******************************************************************************/

/**
 * @brief   HSV颜色空间 转 RGB颜色空间
 * @param   hsv:HSV颜色空间参数
 * @param   rgb:RGB颜色空间参数
 * @note    When 0 ≤ H < 360, 0 ≤ S ≤ 1 and 0 ≤ V ≤ 1:
 * @return  none
*/
void hsv2rgb(color_hsv_t *hsv, color_rgb_t *rgb)
{
    int i;
    float f,a,b,c;

    float h = hsv->h;
    float s = hsv->s;
    float v = hsv->v;
	if (h >= 360) {
		h = 0;
	}
	
    if (s == 0) {
        rgb->r = (unsigned char)((int)(v*255));
        rgb->g = (unsigned char)((int)(v*255));
        rgb->b = (unsigned char)((int)(v*255));
    } else {
        h /= 60.0;  // sector 0 to 5, h_max=360 360/60=6[0,1,2,3,4,5]
        i = (int)floor(h); // floor(h)
        f = h-i; // factorial path of h
        a = v * (1-s);
        b = v * (1-s*f);
        c = v * (1-s*(1-f));
        switch(i) {
            case 0:
                rgb->r = (unsigned char)((int)(v*255)); //v*255
                rgb->g = (unsigned char)((int)(c*255)); //c*255;
                rgb->b = (unsigned char)((int)(a*255)); //a*255;
                break;
            case 1:
                rgb->r = (unsigned char)((int)(b*255)); //b*255;
                rgb->g = (unsigned char)((int)(v*255)); //v*255;
                rgb->b = (unsigned char)((int)(a*255)); //a*255;
                break;
            case 2:
                rgb->r = (unsigned char)((int)(a*255)); //a*255;
                rgb->g = (unsigned char)((int)(v*255)); //v*255;
                rgb->b = (unsigned char)((int)(c*255)); //c*255;
                break;
            case 3:
                rgb->r = (unsigned char)((int)(a*255));//a*255;
                rgb->g = (unsigned char)((int)(b*255));//b*255;
                rgb->b = (unsigned char)((int)(v*255));//v*255;
                break;
            case 4:
                rgb->r = (unsigned char)((int)(c*255)); //c*255;
                rgb->g = (unsigned char)((int)(a*255)); //a*255;
                rgb->b = (unsigned char)((int)(v*255)); //v*255;
                break;
            default:
                rgb->r = (unsigned char)((int)(v*255)); //v*255;
                rgb->g = (unsigned char)((int)(a*255)); //a*255;
                rgb->b = (unsigned char)((int)(b*255)); //b*255;
                break;
        }

    }

    rgb->color = (unsigned int)(((rgb->r&0xff)<<16) | ((rgb->g&0xff)<<8) | (rgb->b&0xff));
}

/**
 * @brief   调节HSV颜色空间 H值(0.0~360.0)
 * @param   hsv:HSV颜色空间参数
 * @param   h_offset:调节参数 增加>0.0，减小<0.0
 * @return  none
*/
void hsv_adjust_h(color_hsv_t *hsv, float h_offset)
{
    hsv->h += h_offset;
    if (hsv->h >= 360) {
        hsv->h = 360;
    } else if (hsv->h<=0) {
        hsv->h = 0;
    }
}


/**
 * @brief   调节HSV颜色空间 S值(0.0~1.0)
 * @param   hsv:HSV颜色空间参数
 * @param   s_offset:调节参数 增加>0.0，减小<0.0
 * @return  none
*/
void hsv_adjust_s(color_hsv_t *hsv, float s_offset)
{
    hsv->s += s_offset;
    if (hsv->s >= 1.0) {
        hsv->s = 1.0;
    } else if (hsv->s <= 0) {
        hsv->s = 0;
    }
}


/**
 * @brief   调节HSV颜色空间 V值(0.0~1.0)
 * @param   hsv:HSV颜色空间参数
 * @param   v_offset:调节参数 增加>0.0，减小<0.0
 * @return  none
*/
void hsv_adjust_v(color_hsv_t *hsv, float v_offset)
{
    hsv->v += v_offset;
    if (hsv->v >= 1.0) {
        hsv->v = 1.0;
    } else if (hsv->v<=0) {
        hsv->v = 0;
    }
}

void rgb2hsv_u8(uint8_t* img, int h, int w,float* img_out){
    color_hsv_t hsv;
    color_rgb_t rgb;
    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            rgb.r = img[i*w*3 + j*3];
            rgb.g = img[i*w*3 + j*3 + 1];
            rgb.b = img[i*w*3 + j*3 + 2];
            rgb2hsv(&rgb, &hsv);
            img_out[i*w*3 + j*3] = hsv.h;
            img_out[i*w*3 + j*3 + 1] = hsv.s;
            img_out[i*w*3 + j*3 + 2] = hsv.v;
        }
    }
}

void hsv2rgb_u8(float* img, int h, int w,uint8_t* img_out){
    color_hsv_t hsv;
    color_rgb_t rgb;
    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            hsv.h = img[i*w*3 + j*3];
            hsv.s = img[i*w*3 + j*3 + 1];
            hsv.v = img[i*w*3 + j*3 + 2];
            hsv2rgb(&hsv, &rgb);
            img_out[i*w*3 + j*3] = rgb.r;
            img_out[i*w*3 + j*3 + 1] = rgb.g;
            img_out[i*w*3 + j*3 + 2] = rgb.b;
        }
    }
}

// 计算直方图
void calculateHistogram(int *hist, unsigned char *image, int width, int height) {
    for(int i = 0; i < 256; i++) {
        hist[i] = 0;
    }
    for(int i = 0; i < width * height; i++) {
        hist[image[i]]++;
    }
}

// 计算累计分布函数
void calculateCDF(int *hist, float *cdf, int totalPixels) {
    cdf[0] = (float)hist[0] / totalPixels;
    for(int i = 1; i < 256; i++) {
        cdf[i] = cdf[i - 1] + (float)hist[i] / totalPixels;
    }
}

void findCDFThresholds(float *cdf, int *minGrayLevel, int *maxGrayLevel,float threshold=0.1) {
    for(int i = 0; i < 256; i++) {
        if (cdf[i] >= threshold) {
            *minGrayLevel = i;
            break;
        }
    }

    for(int i = 255; i >= 0; i--) {
        if (cdf[i] <= 1-threshold) {
            *maxGrayLevel = i;
            break;
        }
    }
}

// 应用直方图均衡化
void histogramEqualization(unsigned char *image, float *cdf, int width, int height) {
    for(int i = 0; i < width * height; i++) {
        image[i] = (unsigned char)(255 * cdf[image[i]]);
    }
}

void enhanceImageUsingHSV(uint8_t *img, int height, int width,float *hsv_img, uint8_t* img_out,float threshold=0.1) {
    rgb2hsv_u8(img, height, width, hsv_img);

    // 步骤2: 计算直方图
    int hist[256] = {0};
    for (int i = 0; i < height * width; i++) {
        unsigned char v_value = (unsigned char)(hsv_img[i*3 + 2] * 255); // 提取V值并量化至0-255
        hist[v_value]++;
    }

    // 步骤3: 计算累计分布函数
    float cdf[256] = {0};
    calculateCDF(hist, cdf, height * width);

    int minGrayLevel = 0, maxGrayLevel = 0;
    findCDFThresholds(cdf, &minGrayLevel, &maxGrayLevel,threshold);

    // 步骤4: 应用直方图均衡
    for (int i = 0; i < height * width; i++) {
        unsigned char v_value = (unsigned char)(hsv_img[i*3 + 2] * 255);
        if(v_value > maxGrayLevel) v_value = maxGrayLevel;
        else if(v_value < minGrayLevel) v_value = minGrayLevel;
        v_value = (v_value - minGrayLevel) * 255 / (maxGrayLevel - minGrayLevel);
        // unsigned char equalized_v = (unsigned char)(255 * cdf[v_value]);
        unsigned char equalized_v = (unsigned char)(v_value);
        hsv_img[i*3 + 2] = (float)equalized_v / 255.0; // 更新V值，转换回0.0-1.0范围
    }

    hsv2rgb_u8(hsv_img, height, width, img_out);
}

// int8到uint8的转换
void S82U8(int8_t* src, uint8_t* dest, int height, int width) {
    int num_pixels = width * height * 3; // 三通道图像
    for (int i = 0; i < num_pixels; i++) {
        dest[i] = (uint8_t)(src[i] + 128);
    }
}

// uint8到int8的转换
void U82S8(uint8_t* src, int8_t* dest, int height, int width) {
    int num_pixels = width * height * 3; // 三通道图像
    for (int i = 0; i < num_pixels; i++) {
        dest[i] = (int8_t)(src[i] - 128);
    }
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
void drawHistogram(string name,const Mat &src) {
    // 将图像转换为灰度图
    Mat gray;
    if (src.channels() == 3) {
        cvtColor(src, gray, COLOR_BGR2GRAY);
    } else {
        gray = src.clone();
    }

    // 计算直方图
    int histSize = 256; // 项的数量（每个像素值一个项）
    float range[] = {0, 256}; // 灰度级的范围
    const float* histRange = {range};
    Mat hist;
    calcHist(&gray, 1, 0, Mat(), hist, 1, &histSize, &histRange, true, false);

    // 创建直方图画布
    int hist_w = 512; int hist_h = 400;
    int bin_w = cvRound((double) hist_w / histSize);
    Mat histImage(hist_h, hist_w, CV_8UC1, Scalar(255, 255, 255));

    // 归一化直方图到范围 [0, histImage.rows]
    normalize(hist, hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());

    // 绘制直方图
    for(int i = 1; i < histSize; i++) {
        line(histImage,
             Point(bin_w * (i-1), hist_h - cvRound(hist.at<float>(i-1))),
             Point(bin_w * (i), hist_h - cvRound(hist.at<float>(i))),
             Scalar(0), 2, 8, 0);
    }

    // 显示直方图
    imshow(name, histImage);
}



int main_utils(){

    //  测试代码
    static color_hsv_t hsv;
    static color_hsv_t tmp_hsv;
    static color_rgb_t rgb;

    // 调节H的值整数增加，负数减小
    hsv_adjust_h(&hsv, 10.0);
    hsv_adjust_h(&hsv, -10.0);

    // 调节S的值整数增加，负数减小
    hsv_adjust_s(&hsv, 0.05);
    hsv_adjust_s(&hsv, -0.05);

    // 调节S的值整数增加，负数减小
    hsv_adjust_v(&hsv, 0.05);
    hsv_adjust_v(&hsv, -0.05);

    // HSV颜色空间转RGB颜色空间
    hsv.h = 0;      // 色调H(hue)  0~360° {R(0°),G(120°),B(240°)}
    hsv.s = 0.5;    // 饱和度S(saturation)  0~1.0
    hsv.v = 1;      // 亮度V(value)  0~1.0  
    hsv2rgb(&hsv, &rgb);
    printf("h:%f, s:%f, v:%f,  r:%d, g:%d, b:%d, color:0x%06x\n", hsv.h, hsv.s, hsv.v, rgb.r, rgb.g, rgb.b, rgb.color);

    // RGB颜色空间转HSV颜色空间
    rgb.r = 0xff;   // Red 0~255
    rgb.g = 0x00;   // Green 0~255
    rgb.b = 0x00;   // Blue 0~255
    rgb2hsv(&rgb, &tmp_hsv);
    printf("h:%f, s:%f, v:%f\n\n", tmp_hsv.h, tmp_hsv.s, tmp_hsv.v);


    Mat image = imread("output/output.jpg");
    // cvtColor(image, image, COLOR_BGR2RGB);

    uint8_t * in_array = new uint8_t[image.rows * image.cols * 3];
    int8_t * s8_array = new int8_t[image.rows * image.cols * 3];
    // uint8_t * out_array = new uint8_t[image.rows * image.cols * 3];
    float * hsv_array = new float[image.rows * image.cols * 3];
    for (int i = 0; i < image.rows * image.cols * 3; i++) {
        in_array[i] = (uint8_t)image.data[i];
    }

    enhanceImageUsingHSV(in_array, image.rows, image.cols,  hsv_array,in_array);

    U82S8(in_array, s8_array, image.rows, image.cols);
    S82U8(s8_array, in_array, image.rows, image.cols);

    Mat out(image.rows, image.cols, CV_8UC3, in_array);
    Mat out1(image.rows, image.cols, CV_8UC3);
    Mat dataset1(image.rows, image.cols, CV_8UC3);
    imread("C:/Users/19178/SquareLine/SquareLine_Project/bin/output/output_4.jpg", dataset1);
    imwrite("output/output_myhsv.jpg", out);

    string dir = "D:/linux/TESTAI4/MDK-ARM/";

    readImageFromTxt(out, dir+"img.txt");
    enhanceImageUsingHSV(out.data, image.rows, image.cols,  hsv_array,out1.data);
    // Mat temp(236,167, CV_8UC3);
    // readImageFromTxt(temp, dir+"temp.txt");


    imshow("out", out);
    imshow("out1", out1);
    // imshow("temp", temp);
    drawHistogram("hist_out",out);
    drawHistogram("hist_out1",out1);


    // imshow("dataset1", dataset1);
    // drawHistogram("hist_dataset1",dataset1);
    waitKey(0);
    return 0;
}