# import os

# # Get the current directory path
# current_dir = os.getcwd()

# # Specify the directory path to operate on
# target_dir = os.path.join(current_dir, 'bin/dataset')

# # Get all files and folders in the target directory
# files = os.listdir(target_dir)

# # Find the maximum number of digits in the filenames
# max_digits = len(str(max([int(file) for file in files if file.isdigit()])))

# # Iterate over file names, pad numbers with leading zeros
# for file in files:
#     if file.isdigit():
#         old_path = os.path.join(target_dir, file)
#         new_name = file.zfill(max_digits)  # Pad with leading zeros
#         new_path = os.path.join(target_dir, new_name)
#         os.rename(old_path, new_path)

import numpy as np
import cv2
import os
import random
def rgb2hsv_u8(img):
    return cv2.cvtColor(img, cv2.COLOR_RGB2HSV) / 255.0

def hsv2rgb_u8(img):
    img = (img * 255).astype(np.uint8)
    return cv2.cvtColor(img, cv2.COLOR_HSV2RGB)

def enhance_image_using_hsv(img):
    hsv_img = rgb2hsv_u8(img)
    v_channel = hsv_img[..., 2]
    
    # 计算直方图和CDF
    hist, bins = np.histogram(v_channel, bins=256, range=[0, 1])
    cdf = hist.cumsum() / hist.sum()
    
    threth = random.uniform(0.05, 0.15)
    min_gray_level = bins[np.searchsorted(cdf, threth)]
    max_gray_level = bins[np.searchsorted(cdf, 1.0-threth)]
    
    # 应用直方图均衡化
    v_equalized = np.clip((v_channel - min_gray_level) / (max_gray_level - min_gray_level), 0, 1)
    hsv_img[..., 2] = v_equalized
    
    return hsv2rgb_u8(hsv_img)

def enhance(img_path,output_path):
    img = cv2.imread(img_path)  # 读取输入图像
    img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)  # 转换为RGB格式
    enhanced_img = enhance_image_using_hsv(img)
    enhanced_img = cv2.cvtColor(enhanced_img, cv2.COLOR_RGB2BGR)  # 转换回BGR格式以便保存
    # 确保目录存在
    output_dir = os.path.dirname(output_path)
    os.makedirs(output_dir, exist_ok=True)
    cv2.imwrite(output_path, enhanced_img)  # 保存增强后的图像


output_dir = 'enhanced'
data_dir = 'bin/dataset'
for filedir in os.listdir(data_dir):
    for filename in os.listdir(os.path.join(data_dir, filedir)):
        input_path = os.path.join(data_dir,filedir, filename)
        output_path = os.path.join(output_dir,filedir, filename)
        name = os.path.splitext(output_path)[0]
        name_path = name + '.jpg'
        enhance(input_path, name_path)
        print(f'{input_path} -> {name_path}')
