# Learn-RTOS-With-ESP32

认识 RTOS 的任务机制
- [FreeRTOS 创建第一个任务_freertos 新建 任务-CSDN博客](https://blog.csdn.net/wangyx1234/article/details/127217253)
- [FreeRTOS 创建任务的 Static 版本_freertos 静态创建任务【手动分配内存空间】-CSDN博客](https://blog.csdn.net/wangyx1234/article/details/127235522?spm=1001.2014.3001.5501)
- [RTOS 中的任务调度与三种任务模型_rtos任务调度-CSDN博客](https://blog.csdn.net/wangyx1234/article/details/127253649?spm=1001.2014.3001.5501)
- [RTOS 暂停任务-任务挂起与恢复_freertos任务暂停-CSDN博客](https://blog.csdn.net/wangyx1234/article/details/127273090?spm=1001.2014.3001.5502)
- [FreeRTOS 删除任务_freertos删除任务-CSDN博客](https://blog.csdn.net/wangyx1234/article/details/127292609?spm=1001.2014.3001.5502)
- [浅析 FreeRTOS SysTick 和任务延时_xtaskgettickcount-CSDN博客](https://blog.csdn.net/wangyx1234/article/details/127311949?spm=1001.2014.3001.5502)
- >>>
- [FreeRTOS 任务通知浅析_freertos发送任务通知后,会立即执行该任务么-CSDN博客](https://blog.csdn.net/wangyx1234/article/details/127354025?spm=1001.2014.3001.5502)
- [获取 FreeRTOS 栈空间大小及其高水位线_freertos获取堆栈-CSDN博客](https://blog.csdn.net/wangyx1234/article/details/127399009?spm=1001.2014.3001.5502)
- [RTOS任务状态总结及查看 RTOS 任务的状态_trace32调试arm查看freertos的任务状态-CSDN博客](https://blog.csdn.net/wangyx1234/article/details/127399140?spm=1001.2014.3001.5502)

多任务间的数据传递
- [ESP32 RTOS 任务间同步与通信篇概述_esp32 任务间通信-CSDN博客](https://blog.csdn.net/wangyx1234/article/details/127482275?spm=1001.2014.3001.5502)
- [使用二值信号量实现任务间单向同步_二值信号量的使用-CSDN博客](https://blog.csdn.net/wangyx1234/article/details/127503072?spm=1001.2014.3001.5502)
- [使用计数信号量实现任务间单向同步_计数信号量能否为负-CSDN博客](https://blog.csdn.net/wangyx1234/article/details/127522492?spm=1001.2014.3001.5502)
- [用事件组实现多事件的单向同步_xeventgroupsync-CSDN博客](https://blog.csdn.net/wangyx1234/article/details/127542729?spm=1001.2014.3001.5502)



LVGL+RTOS
- 不建议多个任务调用 gui 函数，因为 gui 的api并不是可重入的，可能操作了某些api操作了公共变量。举个例子，绘制曲线图函数，硬件传感器有新数据了直接调用添加点绘制，但此时gui线程处于挂起状态，显然是不合理的，这样使用可能就有意想不到的问题出现，你的问题也是类似的。通常做法是gui线程维护一个消息队列，gui线程在周期性刷新时读取，收到响应消息后做页面跳转，总之保持gui的api只在gui线程被调用
- 消息队列
	- [【FreeRTOS】详细讲解FreeRTOS中消息队列并通过示例讲述其用法_深入解析freertos消息队列及示例用法详解-CSDN博客](https://blog.csdn.net/qq_53960242/article/details/128795993)
	- https://github.com/0015/ThatProject/tree/master/ESP32_LVGL/LVGL9


- [【壹拾捌】FreeRTOS 上用一个队列接收多个来源的数据 - 知乎](https://zhuanlan.zhihu.com/p/451984348)
- RTOS 中断
	- [【ESP32+freeRTOS学习笔记-(七)中断管理】_esp32 freertos中断延时-CSDN博客](https://blog.csdn.net/weixin_45499326/article/details/128631454)


	LVGL page chart
	- [page-switch](https://blog.csdn.net/tilblackout/article/details/131058317)
	- [chart](https://zhuanlan.zhihu.com/p/8991331655)
