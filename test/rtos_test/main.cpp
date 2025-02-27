#include <Arduino.h>

TaskHandle_t task1; // 任务1结构对象
TaskHandle_t task2;
TaskHandle_t task3;

static int task1_flag;           // 用于指示任务运行状态的标志变量
static int task2_flag;           
static int task3_flag;           

/**
 * 任务的运行代码
 * @param param 任务初始运行参数
 */
static void task1_process(void *arg)
{
    static const char *TASK1_TAG = "TASK1";
    while (1) {
        ESP_LOGI(TASK1_TAG, "task1_flag = %d, arg1 = %s", task1_flag, (char *)arg);
        task1_flag++;
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

static void task2_process(void *arg)
{
    static const char *TASK2_TAG = "TASK2";
    while (1) {
        ESP_LOGI(TASK2_TAG, "task2_flag = %d, arg2 = %s", task2_flag, (char *)arg);
        task2_flag++;
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

static void task3_process(void *arg)
{
    static const char *TASK3_TAG = "TASK3";
    while (1) {
        ESP_LOGI(TASK3_TAG, "task3_flag = %d, arg3 = %s", task3_flag, (char *)arg);
        task3_flag++;
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

static void system_init(void) {
    printf("Hello world!\n");

    /* Print chip information */
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    printf("This is %s chip with %d CPU core(s), WiFi%s%s, ",
            CONFIG_IDF_TARGET,
            chip_info.cores,
            (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
            (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");

    printf("Minimum free heap size: %d bytes\n", esp_get_minimum_free_heap_size());
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Hello World");

  system_init();
}

void loop() {
  // put your main code here, to run repeatedly:

  xTaskCreate(&task1_process, "task1", 1024*2, (void *)"1", configMAX_PRIORITIES, &task1); // configMAX_PRIORITIES = 25
  xTaskCreate(&task2_process, "task2", 1024*2, (void *)"2", configMAX_PRIORITIES-1, &task2);
  xTaskCreatePinnedToCore(&task3_process, "task3", 1024*2, (void *)"3", configMAX_PRIORITIES-2, &task3, 0);

}