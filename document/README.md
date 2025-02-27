# README

## ✌🏻 进度安排与更新记录

| 有效性  | 阶段   | 时间               | 备注              | 详细的子页面\|作者   |
| ---- | ---- | ---------------- | --------------- | ------------ |
| ❌->✔ | 准备阶段 | 2025.2.20-2025.x | 题目选定 + GUI 显示接口 | links 部分\|yy |
|      |      |                  |                 | GUI 需求\|hm   |

## JLC 文档

- [🎉2025年电赛备赛资料合集—持续更新中 | 嘉立创EDA教育与开源文档中心](https://wiki.lceda.cn/zh-hans/contest/e-contests/resource/)
- [《立创EDA硬件开发规范文档》](https://www.yuque.com/lceda_design/template/zso261/#dBBwU)，剪裁下到本地的文档在 [manual](manual.md)

## GUI 需求

需要显示的信息：
- 当前设置电流（数字）0~1 A，分辨率 10 mA
- 显示当前负载两端电压，分辨率 10 mA
- 显示当前负载两端电流，分辨率 10 mA
- 测量并显示电源负载调整率（范围 0.1%~19.9%, 10 V max, 1 A）
- 过压/不工作时显示保护信息

## links

物料链接
- 开发板 [ESP32-S3-DevKitC-1 v1.1 - ESP32-S3 - — esp-dev-kits latest 文档](https://docs.espressif.com/projects/esp-dev-kits/zh_CN/latest/esp32s3/esp32-s3-devkitc-1/user_guide.html)
- 屏幕 [2.4inch SPI Module ILI9341 SKU:MSP2402 - LCD wiki](http://www.lcdwiki.com/2.4inch_SPI_Module_ILI9341_SKU%3AMSP2402)


TFT-ESPI 驱动部分
- [ESP32使用TFT_eSPI库实现屏幕触摸_esp32 tft-CSDN博客](https://blog.csdn.net/Z1576796635/article/details/130480188)
- 配置文件参考 [ESP32驱动3.2寸ILI9341显示屏+XPT2046触摸，GUIslice用户图形库_ili9341 esp32-CSDN博客](https://blog.csdn.net/zgj_online/article/details/104992395)
- 连线参考 [platformio下ESP32驱动屏幕ili9486 spi触摸xpt2046 GUIslice TFT_eSPI – 走着的小站](https://www.openpilot.cc/archives/2978)
- [ESP32使用TFT_eSPI库实现屏幕触摸_esp32 tft-CSDN博客](https://blog.csdn.net/Z1576796635/article/details/130480188)


LVGL + RTOS 部分（包括 TFT-ESPI 驱动）
- [基于Arduino框架下的ESP32-S3移植LVGL教程_esp32s3 lvgl-CSDN博客](https://blog.csdn.net/qq_64346054/article/details/134634996)
- [ESP32 IDF 在vs code 下移植LVGL_v8.3.11（致力打造史上最详细lvgl移植教程，步骤堪称保姆级，小白上手即可实现100%成功移植！！！）_esp32 lvgl 8.3.10 和8.3.11-CSDN博客](https://blog.csdn.net/qq_34885669/article/details/145567419)
	- [在FreeRTOS使用Lvgl任务死机的问题，基于esp32 - 哔哩哔哩](https://www.bilibili.com/opus/733439281703419959)


在使用 port 文件时，官方给的是 `c` 文件，而 `tft` 库是 `cpp` 库
- 要注意改为 `.cpp` ，并且删掉 `extern C` 避免在文件中引用 ` cpp ` 库错

```cpp
#ifdef __cplusplus
extern "C" {
#endif

// 使用 C 编译器编译的部分

#ifdef __cplusplus
}
#endif
```

RTOS 快速入门相关
- 感觉还行 [Learn_FreeRTOS_with_ESP32](https://gitee.com/yx_wang/learn_freertos_with_esp32#https://gitee.com/link?target=https%3A%2F%2Fblog.csdn.net%2Fwangyx1234%2Farticle%2Fdetails%2F127217253)
	- 详情细分章节链接见 [Learn-RTOS-With-ESP32 细分链接文档](Learn-RTOS-With-ESP32.md)
 