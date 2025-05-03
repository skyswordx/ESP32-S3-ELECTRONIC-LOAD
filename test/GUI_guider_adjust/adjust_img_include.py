import os

def comment_custom_include(file_path):
    with open(file_path, 'r') as file:
        lines = file.readlines()

    changes_made = False
    with open(file_path, 'w') as file:
        for line in lines:
            if '#include "custom.h"' in line:
                file.write('// ' + line)
                changes_made = True
            else:
                file.write(line)
    
    # 输出处理信息
    if changes_made:
        print(f"已处理文件: {file_path}")
        print(f"  操作: 注释掉 #include \"custom.h\" 行")
    return changes_made

def add_define_if_missing(file_path):
    with open(file_path, 'r') as file:
        lines = file.readlines()

    define_added = False
    if not lines or '#define LV_LVGL_H_INCLUDE_SIMPLE 1' not in lines[0]:
        lines.insert(0, '#define LV_LVGL_H_INCLUDE_SIMPLE 1\n')
        define_added = True

    with open(file_path, 'w') as file:
        file.writelines(lines)
    
    # 输出处理信息
    if define_added:
        print(f"已处理文件: {file_path}")
        print(f"  操作: 添加 #define LV_LVGL_H_INCLUDE_SIMPLE 1")
    return define_added

def process_files():
    # 获取项目根目录
    generated_dir = 'D:/Musii-SnapShot/GithubRepo/ESP32-S3-ELECTRONIC-LOAD/lib/generated'
    images_dir = os.path.join(generated_dir, 'images')

    print("开始处理主目录中的文件...")
    files_processed = 0
    changes_made = 0
    
    for root, _, files in os.walk(generated_dir):
        for file in files:
            if file.endswith('.h') or file.endswith('.c'):
                file_path = os.path.join(root, file)
                files_processed += 1
                # 取消下面的注释以启用处理custom.h的功能
                # if comment_custom_include(file_path):
                #    changes_made += 1

    print(f"\n主目录处理完成：检查了 {files_processed} 个文件")

    print("\n开始处理images目录中的文件...")
    files_processed = 0
    images_changes_made = 0
    
    for root, _, files in os.walk(images_dir):
        for file in files:
            if file.endswith('.h') or file.endswith('.c'):
                file_path = os.path.join(root, file)
                files_processed += 1
                if add_define_if_missing(file_path):
                    images_changes_made += 1

    print(f"\nimages目录处理完成：检查了 {files_processed} 个文件，修改了 {images_changes_made} 个文件")

if __name__ == "__main__":
    process_files()
    print("\n处理完成!")