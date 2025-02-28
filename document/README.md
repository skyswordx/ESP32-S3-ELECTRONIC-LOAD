# README

## ✌🏻 进度安排与更新记录

| 有效性 | 阶段              | 时间                  | 备注                         | 详细的子页面\|作者                 |
| --- | --------------- | ------------------- | -------------------------- | -------------------------- |
| ❌   | 准备阶段            | 2025.2.20-2025.x    | 题目选定 + GUI 显示接口            | links 部分\|yy<br>GUI 需求\|hm |
| ✔   | GUI + RTOS 框架建立 | 2025.2.25-2025.2.28 | 搭建 RTOS 框架 + 移植 GUI-guider | LVGL-RTOS 学习资料总结部分\|yy     |


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

## LVGL-RTOS 学习资料总结

### 快速入门 links

物料链接
- 开发板 [ESP32-S3-DevKitC-1 v1.1 - ESP32-S3 - — esp-dev-kits latest 文档](https://docs.espressif.com/projects/esp-dev-kits/zh_CN/latest/esp32s3/esp32-s3-devkitc-1/user_guide.html)
- 屏幕 [2.4inch SPI Module ILI9341 SKU:MSP2402 - LCD wiki](http://www.lcdwiki.com/2.4inch_SPI_Module_ILI9341_SKU%3AMSP2402)


TFT-ESPI 驱动部分
- [ESP32使用TFT_eSPI库实现屏幕触摸_esp32 tft-CSDN博客](https://blog.csdn.net/Z1576796635/article/details/130480188)
- 配置文件参考 [ESP32驱动3.2寸ILI9341显示屏+XPT2046触摸，GUIslice用户图形库_ili9341 esp32-CSDN博客](https://blog.csdn.net/zgj_online/article/details/104992395)
- 连线参考 [platformio下ESP32驱动屏幕ili9486 spi触摸xpt2046 GUIslice TFT_eSPI – 走着的小站](https://www.openpilot.cc/archives/2978)
- [ESP32使用TFT_eSPI库实现屏幕触摸_esp32 tft-CSDN博客](https://blog.csdn.net/Z1576796635/article/details/130480188)


LVGL + RTOS 部分（包括 TFT-ESPI 驱动）
- 重点 >>> [基于Arduino框架下的ESP32-S3移植LVGL教程_esp32s3 lvgl-CSDN博客](https://blog.csdn.net/qq_64346054/article/details/134634996)
- [ESP32 IDF 在vs code 下移植LVGL_v8.3.11](https://blog.csdn.net/qq_34885669/article/details/145567419)
- [STM32F4+FreeRTOS+LVGL实现嵌入式快速开发(缝合怪)_freertos lvgl-CSDN博客](https://blog.csdn.net/freestep96/article/details/130175829)
	- [在FreeRTOS使用Lvgl任务死机的问题，基于esp32 - 哔哩哔哩](https://www.bilibili.com/opus/733439281703419959)
	- [2.freeRTOS使用互斥锁（Mutex）解决多任务竞争冒险](https://blog.csdn.net/m0_60134435/article/details/135777319)
- 重点 >>> [基于Gui Guider进行LVGL的页面绘制和移植 stm32](https://gitcode.csdn.net/66c6d41ca1ed2f4c853c0bb4.html#devmenu9)
- [QR-code-二维码 LVGL 文档](https://lvgl.100ask.net/master/details/libs/qrcode.html)


RTOS 快速入门相关
- 感觉还行 [Learn_FreeRTOS_with_ESP32](https://gitee.com/yx_wang/learn_freertos_with_esp32#https://gitee.com/link?target=https%3A%2F%2Fblog.csdn.net%2Fwangyx1234%2Farticle%2Fdetails%2F127217253)
	- 详情细分章节链接见 [Learn-RTOS-With-ESP32 细分链接文档](Learn-RTOS-With-ESP32.md)
 
### 操作时的 Trouble Shootings


在使用 port 文件时，官方给的是 `c` 文件，而 `tft` 库是 `cpp` 库
一种修改方法是把文件改为 `.cpp` ，并且删掉 `extern C` 避免在文件中引用 ` cpp ` 库错
```cpp
#ifdef __cplusplus
extern "C" {
#endif

// 使用 C 编译器编译的部分

#ifdef __cplusplus
}
#endif
```

实际使用 GUI-Guider 生成的文件方法是，直接 `copy` 其 `generated` 文件夹到 `pio` 工程的 `lib` 路径下，然后在使用到图片的对应数组 `c` 文件开头添加
```c
#define LV_LVGL_H_INCLUDE_SIMPLE 1
```


触摸屏的那个方向会是反的，或者按屏幕显示的控件没有对应的 LVGL 动画反应，可能是因为设置 tft 显示驱动的方向和 LVGL 排布元素的固有方向不匹配
```cpp
extern TFT_eSPI tft; // 使用外部定义的 tft

/*Initialize your display and the required peripherals.*/
static void disp_init(void)
{
    /*You code here*/
    // 这个函数是设置显示屏 IC 的初始化配置
    // 例如：tft.begin();  //初始化配置

    tft.begin();  //初始化配置
    tft.setRotation(1);//设置显示方向
    // 实测从 3 变到 1 就正常了，可选的值为 0 ~3 可以在 vscode 跳转查看
}
```
### 移植的经验总结

LVGL 框架的两个核心组件就是任务调度器 (`timer_handler` 或者 `task_handler` 旧版)和系统心跳（`lv_tick_inc`）
- 一般的 LVGL 页面显示函数只需要在主循环外调用一次即可，然后在主循环中使用任务调度器进行具体的调配
- 然后为了合理调度，需要 LVGL 被告知系统自启动后运行至今的时间，一般是选择一个固定的定时器时间触发中断（已选 ESP 硬件定时器中断了）

要在 `RTOS` 系统中添加 LVGL 框架，只需要把 LVGL 的任务调度器封装进 `RTOS` 的任务即可，然后 LVGL 的系统心跳由系统硬件支持，不用修改

所以只需要把原来放在主循环中的任务调度器 (`timer_handler` 或者 `task_handler` 旧版) 剪切出，然后封装进 `RTOS` 的任务即可
```cpp
void setup() {

  system_init(); // 打印系统信息
  lv_init(); // 初始化 LVGL 必须运行的
  lv_port_disp_init(); // 初始化绑定显示接口
  lv_port_indev_init(); // 初始化和绑定触摸接口

  setup_ui(&guider_ui); // 初始化 gui_guider

  /* GUI-guider-api */
  setup_scr_screen(&guider_ui); // gui_guider 为每一个页面生成的，这里是名字为 screen 的页面
  lv_scr_load(guider_ui.screen); //每一个页面的名字都是 gui_guider 结构体的元素

  xMutex = xSemaphoreCreateMutex(); // 创建一个互斥信号量
  xTaskCreate(lvgl_task,
              "demo_test",
              1024*4,
              NULL,
              1,
              &lvgl_task_handle
            );
}
```

但是因为 LVGL 是线程不安全的，所以要在这个 LVGL 任务加上互斥锁
```cpp
TaskHandle_t lvgl_task_handle; // LVGL 任务结构对象

SemaphoreHandle_t xMutex;  // 互斥锁句柄，LVGL 线程不安全，需要加锁
 
void lvgl_task(void *pvParameters)
{
  while(1)
  {
    if (xSemaphoreTake(xMutex, portMAX_DELAY)) { // 获取互斥锁
      lv_task_handler();  //开启LVGL任务调度
      xSemaphoreGive(xMutex); // 释放互斥锁
    }
    vTaskDelay( 20 );
  }
  vTaskDelete(NULL);
}
```