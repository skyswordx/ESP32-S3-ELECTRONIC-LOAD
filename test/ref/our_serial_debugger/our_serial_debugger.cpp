#include "our_serial_debugger.h"

// #P0=1000.0000!! ->  15 格至少
char start_character = '#'; // 数据包的开始字符
char terminate_character = '!'; // 数据包的终止字符
char received_data_buff[RECEIVED_DATA_BUFF] = {0}; // 用来存放接收到的数据


extern Motor motor1;
extern Motor motor2;
extern Motor motor3;
extern Motor motor4;
extern Motor* ptr2current_selected_Motor;

bool flag_of_serial_uart0_interrupt = false;

// 初始化串口 0，使用手动的方式来注册中断服务函数
void serial_uart0_init_with_mannual_ISR(int baud_rate){
    // 初始化串口
  Serial.begin(baud_rate); 
  uart_driver_install(UART_NUM_0, 256, 0, 0, NULL, 0); // 初始化 uart0
  uart_isr_free(UART_NUM_0); // 释放 uart0 的中断
  // 重新注册 uart0 的中断服务函数
  uart_isr_handle_t handle;
  uart_isr_register(UART_NUM_0, uart0_isr_callback, NULL, ESP_INTR_FLAG_LOWMED | ESP_INTR_FLAG_IRAM, &handle);
}

// 串口 0 接收中断回调函数，对应的是 uart0 (Arduino 的 Serial)
void IRAM_ATTR uart0_isr_callback(void* arg){
  volatile uart_dev_t* uart = &UART0;
  uart->int_clr.rxfifo_full = 1;
  uart->int_clr.frm_err = 1;
  uart->int_clr.rxfifo_tout = 1;
 
  int number = 0;

  while (uart->status.rxfifo_cnt) {
        uint8_t character = uart->fifo.rw_byte;
        
        if (number < RECEIVED_DATA_BUFF){
          received_data_buff[number++] = character;// 把接收到的数据存放到数组里面
        }// uart_write_bytes(UART_NUM_0, &character, 1);  //把直接接收的数据打印出来
   }
  // serial_debug_service();
  flag_of_serial_uart0_interrupt = true;
  
}

 /**
   * 功能：
   *  1.一旦串口有数据传入，就会把数据读取到缓冲区中
   *  2.缓存区读取完毕之后，就会置标志位，然后根据标志位在主循环中进入这个函数处理数据
   *  3.在这里进行条件判断并置标志位，满足第一次选中这个电机后，就会一直选中这个电机，直到下一次更改
   *  4.并且可以设置电机位置环和速度环的参数，包括目标值和 pid 控制器的参数
   * 
   * 例子:
   *  收到 #1! 选中电机1
   *  收到 #P0=500.0000 更改电机1的目标位置
   *  收到 #2！选中电机2
   *  收到 #P0=500.0000 更改电机2的目标位置
   *  收到 #P0=600.0000 更改电机2的目标位置
   * 
   * 选中电机的数据包格式：
   *  字符串显示 ##1!
   *  对应的 ASCII 码 0x23 0x23 0x31 0x21 
   *                   #    #    1    !    
   * 
   * 给程序的变量设置数值的数据包格式：
   *  字符串显示 #P0=500.0000! 
   *  对应的 ASCII 码 
   *  0x23 0x50 0x30 0x3D 0x35 0x30 0x30 0x2E 0x30 0x30 0x30 0x30 0x21 
   *    #   P    0    =    5    0    0    .    0    0    0    0    !    
   */

// 串口调试的入口函数
void serial_debug_service(){
  char* ptr2received_data = received_data_buff; // 创建一个指向字符串数组第一个元素的指针，用来读取字符串数据

  if (received_data_buff[0] == start_character && received_data_buff[1] == start_character){
  // 判断是不是控制选中电机的命令，是则重新选中一个电机（检测到头两个是 '##' 就判断成功） 
  // 数据包格式: ##1! 或 ##12!
     
    ptr2received_data = received_data_buff + 2; // 移动指针到 '##' 之后的第一个位置
    int motor_number = 0;

    while(*ptr2received_data != terminate_character){
    // 直到检测到终止字符之前，都还在累加电机编号
      motor_number = 10*motor_number + (*ptr2received_data - '0'); // ASCII 码换算真正的数字
      ptr2received_data++; // 这种结构实现上述效果   
    } // 出去 while 之后，就意味着此时指针指向 '!'    
    
    Serial.print("已选中电机编号:");
    Serial.println(motor_number);
    switch (motor_number){
      case 1:
        ptr2current_selected_Motor = &motor1;
        break;
      case 2:
        ptr2current_selected_Motor = &motor2;
        break;
      case 3:
        ptr2current_selected_Motor = &motor3;
        break;
      case 4:
        ptr2current_selected_Motor = &motor4;
        break;
      default:
        // Serial.println("请输入一个合法的ASCII字符");
        // Serial.println("默认选择电机 1");
        ptr2current_selected_Motor = &motor1;
        break;
    }

  }else if(received_data_buff[0] == start_character){
  // 否则就是改变程序中变量的值（检测到头一个是 '#'）
    ptr2received_data = received_data_buff + 1; // 移动指针到 '#' 之后的第一个的位置
    // 数据包格式: #P0=500.0000!
    
    while(*ptr2received_data != '='){
      // 检测 '=' 之前，来决定值用来改变哪个参数
      ptr2received_data++;
    } // 出来这个 while 之后，指针指向 '='
    ptr2received_data += 1; // 微调一下，指向 '=' 之后的数字

    bool flag_of_encounter_negative = false; // 用来判断是否遇到负号
    // 如果出现负数的情况，可以在这里加一个判断
    // 比如数据包是 #P0=-500.0000!，那么此时指针指向 '-'，可以在这里判断一下
    if(*ptr2received_data == '-'){
      ptr2received_data++; // 跳过负号
      flag_of_encounter_negative = true; // 设置负号标志位
    }

    double data = 0; // 用来存放参数的值
    bool flag_of_encounter_dot = false; // 用来作为在整数部分和小数部分切换的标志位
    int times = 1; // 用来告诉是第几位小数，要除以几个 10
    
    
    while(*ptr2received_data != terminate_character){
    // 从 '=' 之后开始，直到检测到终止字符 '!' 之前，都还在累加数字
      
      if(*ptr2received_data == '.'){
        flag_of_encounter_dot = true;
        ptr2received_data++; // 判断出来指向 '.' 之后，立马跳过 '.'，直接指向 '.' 之后的数字（意义是小数）
      }

      if(flag_of_encounter_dot == false){
        // 在遇到 '.' 之前的分支，乘 10 加上去
        data = 10*data + (*ptr2received_data - '0'); // ASCII 码换算真正的数字
      }else{
        // 在遇到 '.' 之后的分支，第 n 次进去就除 n 次 10 加上去
        data = data +  divided_by_10_power((*ptr2received_data - '0'), times++); 
        // times++ 计算出来的值还是原来的 times，但是在取值之后自增
      }
      ptr2received_data++; // 继续移动指向下一个字符
    } // 出去 while 之后，就意味着此时指针指向 '!'，此时 data 已经有值了
    
    if (flag_of_encounter_negative == true){ // 在使用 data 之前，判断一下是否遇到负号
      data = -data; // 如果遇到负号，就把 data 取反
    }

    if (received_data_buff[1] == 'P'){
      // 如果是位置环的参数
      switch (received_data_buff[2]){
        case '0': // 设置目标位置
          ptr2current_selected_Motor->target_encoder_count_in_position = ptr2current_selected_Motor->from_revolution_toget_count(data);
          break;
        case '1': 
          ptr2current_selected_Motor->kp_in_position = data;
          break;
        case '2':
          ptr2current_selected_Motor->ki_in_position = data;
          break;
        case '3':
          ptr2current_selected_Motor->kd_in_position = data;
          break;
        case '4':
          ptr2current_selected_Motor->output_in_position = data; // 0~255
          break;
        default:
          Serial.println("请输入一个合法的ASCII字符");
          break;
      }
    }else if(received_data_buff[1] == 'V'){
      // 如果是速度环的参数
      switch (received_data_buff[2]){
        case '0': // 设置目标速度
          ptr2current_selected_Motor->target_encoder_count_in_velocity = ptr2current_selected_Motor->from_rpm_toget_count(data);
          Serial.printf("hhh:%f\n",ptr2current_selected_Motor->target_encoder_count_in_velocity);
          break;
        case '1':
          ptr2current_selected_Motor->kp_in_velocity = data;
          break;
        case '2':
          ptr2current_selected_Motor->ki_in_velocity = data;
          break;
        case '3':
          ptr2current_selected_Motor->kd_in_velocity = data;
          break;
        case '4':
          ptr2current_selected_Motor->output_in_velocity = data; // 0~255
          break;
        default:
          Serial.println("请输入一个合法的ASCII字符");
          break;
      }
    }
    uart_write_bytes(UART_NUM_0, received_data_buff+1, 2);
    Serial.println("");
    Serial.print("data:");
    Serial.println(data);
  }
}


// 把输入的数字除以 10 的 power 次方/
double divided_by_10_power(uint8_t input, uint8_t power){ 
  double result = input;
  while(power--){ // power-- 表达式的值是 power
    result/=10;
  }
  return result;
}


  