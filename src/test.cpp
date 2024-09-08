#include <stdio.h>
#include <string.h>

void parseAndGetValue(char *buffer, char *keyword, char *result) {
    char *q = strstr(buffer, keyword);
    if (q != NULL) {
        char *start = strchr(q, ':') + 1;
        char *end = strchr(start, ',');
        if (end == NULL) {
            end = strchr(start, '\"');
            if(end==NULL)
                end = strchr(start, '\0');
        }
        int length = end - start;
        strncpy(result, start, length);
        result[length] = '\0';
    } else {
        result[0] = '\0'; // Set result to empty string if keyword not found
    }
}

int main2() {
    // char RxBuffer[] = "faceid:2,tiwen:53.6,xueyang:79,xinlv:23";
    char RxBuffer[] = "tiwen:53.6,xueyang:79,xinlv:23,time:7 12 19 06;tiwen:12.2,xueyang:12,xinlv:83,time:7 12 19 06;tiwen:12.2,xueyang:12,xinlv:86,time:7 12 19 06;";
    char result[100]; // Buffer to store the result

    parseAndGetValue(RxBuffer, "faceid", result);
    if (result[0] != '\0') {
        printf("faceid: %s\n", result);
    }

    parseAndGetValue(RxBuffer, "tiwen", result);
    if (result[0] != '\0') {
        printf("tiwen: %s\n", result);
    }

    parseAndGetValue(RxBuffer, "xueyang", result);
    if (result[0] != '\0') {
        printf("xueyang: %s\n", result);
    }

    parseAndGetValue(RxBuffer, "xinlv", result);
    if (result[0] != '\0') {
        printf("xinlv: %s\n", result);
    }

    return 0;
}


typedef unsigned char uint8_t;
int main1() {
    uint8_t cur_dis_hr = 80;
    uint8_t cur_dis_spo2 = 95;
    float cur_temp = 37.5;
    int cur_persion_id = 0;

    char std[100];
    snprintf(std, 100, "faceid:%d,tiwen:%2.1f,xueyang:%02d,xinlv:%03d",
             cur_persion_id + 1, cur_temp, cur_dis_spo2, cur_dis_hr);
    int length = strlen(std);

    printf("Formatted String: %s\n", std);
    printf("String Length: %d\n", length);
    printf("AT+HMPUB=1,\"/test/M2M/aa\",%d,\"%s\"\r\n", length, std);

    return 0;
}


#include <iostream>
#include <boost/filesystem.hpp>
#include <string>
#include <vector>

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

using namespace std;
int main() {
    cout<<"hello world"<<endl;
    std::string folderPath = "./dataset/0";
    std::vector<std::string> subdirectories = getAllImagePaths(folderPath);

    for (const std::string& subdirectory : subdirectories) {
        std::cout << subdirectory << std::endl;
    }
    return 0;
}