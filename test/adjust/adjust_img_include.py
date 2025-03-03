import os

def comment_custom_include(file_path):
    with open(file_path, 'r') as file:
        lines = file.readlines()

    with open(file_path, 'w') as file:
        for line in lines:
            if '#include "custom.h"' in line:
                file.write('// ' + line)
            else:
                file.write(line)

# def add_define_if_missing(file_path):
    # with open(file_path, 'r') as file:
    #     lines = file.readlines()

    # if not lines or '#define LV_LVGL_H_INCLUDE_SIMPLE 1' not in lines[0]:
    #     lines.insert(0, '#define LV_LVGL_H_INCLUDE_SIMPLE 1\n')

    # with open(file_path, 'w') as file:
    #     file.writelines(lines)

def process_files():
    # 获取项目根目录
    
    generated_dir = 'D:/Musii-SnapShot/GithubRepo/ESP32-S3-TFT-LVGL/lib/generated'
    images_dir = os.path.join(generated_dir, 'images')

    for root, _, files in os.walk(generated_dir):
        for file in files:
            if file.endswith('.h') or file.endswith('.c'):
                file_path = os.path.join(root, file)
                comment_custom_include(file_path)

    for root, _, files in os.walk(images_dir):
        for file in files:
            if file.endswith('.h') or file.endswith('.c'):
                file_path = os.path.join(root, file)
                # add_define_if_missing(file_path)

if __name__ == "__main__":
    process_files()
    print("done")