#define LV_LVGL_H_INCLUDE_SIMPLE 1
/*
* Copyright 2025 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#include "lvgl.h"

#if LV_USE_FS_RAWFS

const rawfs_size_t rawfs_file_count = 1;
rawfs_file_t rawfs_files[1] = {
	0x0, 0, 10093, "/logo.bin",

};

#endif  /*LV_USE_FS_RAWFS*/ 