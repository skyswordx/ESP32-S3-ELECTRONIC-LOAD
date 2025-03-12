# README

## ✌🏻 进度安排与更新记录

| 有效性 | 阶段              | 时间                  | 备注                         | 详细的子页面\|作者                 |
| --- | --------------- | ------------------- | -------------------------- | -------------------------- |
| ❌   | 准备阶段            | 2025.2.20-2025.x    | 题目选定 + GUI 显示接口            | links 部分\|yy<br>GUI 需求\|hm |
| ✔   | GUI + RTOS 框架建立 | 2025.2.25-2025.2.28 | 搭建 RTOS 框架 + 移植 GUI-guider | LVGL-RTOS 学习资料总结部分\|yy     |
## 硬件电路参考 links

- [#第五届立创电子设计大赛#数控直流电子负载 - 立创开源硬件平台](https://oshwhub.com/micespring/digital-dc-elecload)
- [触屏迷你电子负载 - 立创开源硬件平台](https://oshwhub.com/flyn/mi-ni-dian-zi-fu-zai)
	- **采样范围**：电压0-60V，电流0-4A
	- **负载能力**：恒流0-4A，误差<0.04A | 恒压0-48V，误差<0.2V | 恒阻1-1000R，误差<5%，恒功率0-100W，误差<5%
	- **带载功率**：长时间运行30W，短时运行100W(10S)
	- **保护方式**：过流保护、过压保护、过温保护、反接保护

## 硬件电路设计点

采用核心板 + 功率板分开，中间使用排针连接
- 核心板留有另一组排针和 tft SPI 显示屏连线
- 泪滴
	- [立创eda学习笔记二十一：添加、移除泪滴_立创eda泪滴-CSDN博客](https://blog.csdn.net/gutie_bartholomew/article/details/123069415)

文档
- [泪滴 | 嘉立创EDA专业版用户指南](https://prodocs.lceda.cn/cn/pcb/tools-teardrop/index.html)
## 恒流指标

基本指标
- 恒流电流设置范围为 100 mA~1000 mA
	- **需要用怎么达到？**
- 当电子负载两端电压变化 10 V 时，要求输出电流变化的绝对值小于变化前电流值的 1%
- 具有过压保护功能，过压阀值电压为 18 V+0.2 V

发挥部分
- 能实时测量并数字显示电子负载两端的电压，电压测量精度为士 (0.02%+0.02%FS)，分辨力为 1 mV
- 能实时测量并数字显示流过电子负载的电流，电流测量精度为士 (0.1%+0.1%FS)，分辨力为 1 mA
- 具有直流稳压电源负载调整率自动测量功能，测量范围为 0.1%~19.9%，测量精度为土 1%。为方便，本题要求被测直流稳压电源的输出电压在 10 V 以内。

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


[LVGL lv_label_set_text_fmt 显示只有f_lvgl浮点-CSDN博客](https://blog.csdn.net/weixin_44684950/article/details/124426341)
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

## JLC 文档

- [🎉2025年电赛备赛资料合集—持续更新中 | 嘉立创EDA教育与开源文档中心](https://wiki.lceda.cn/zh-hans/contest/e-contests/resource/)
- [《立创EDA硬件开发规范文档》](https://www.yuque.com/lceda_design/template/zso261/#dBBwU)，剪裁下到本地的文档在 [manual](manual.md)
## 3d 模型获取

- [▷ spi tft display 3d models 【 STLFinder 】](https://www.stlfinder.com/3dmodels/spi-tft-display/#google_vignette)
- [DISPLAY LCD TFT 2.4" 240x320 TOUCH SPI | 3D CAD Model Library | GrabCAD](https://grabcad.com/library/display-lcd-tft-2-4-240x320-touch-spi-1)
- [低成本LCD显示屏4.3、5、7英寸Arduino SPI I2C TFT触摸屏 Parasolid 3D模型下载 - 迪威模型](https://www.3dwhere.com/models/s4fv15syxuspa8om)
- [ESP32-S3-DEVKITC-1U-N8R2 EDA | CAD 3D 模型下载| Digikey](https://www.digikey.cn/zhs/models/16162648)
- [电池 3D模型下载,CAD图纸下载_三维模型网www.sanweimoxing.com](https://www.sanweimoxing.com/t-%E7%94%B5%E6%B1%A0-1.html)


https://deepinout.com/lvgl-tutorials/lvgl-widgets/lvgl-lv_chart.html
https://zhuanlan.zhihu.com/p/570310650