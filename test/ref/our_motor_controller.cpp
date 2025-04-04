#include "our_motor_controller.h"
#include <Arduino.h>

/**
 * cpp 特性:
 * 1.在头文件的类中声明的成员函数，可以在 cpp 文件中定义，格式就是【函数类型 类名::函数名(参数){实体}】
 *    例子：void Motor::pid_position_controller(){实体}
 * 2.this 主要是在成员函数的实现中使用的比较多，里面的 this 就是指向我未来要声明的类的其他对象的指针
 *    例子：我在其他地方声明了一个 Motor 对象 motor1，那么我在调用 motor1.pid_position_controller() 这个函数时，这里面的 this 就是一个指向 motor1 的指针
 *          我又声明了一个 Motor 对象 motor2，那么我在调用 motor2.pid_position_controller() 这个函数时，这里面的 this 就是一个指向 motor2 的指针
 */

/**
 * todolist
 * 1.启动和停止 pid 控制器
 *   运行到稳定时停止 pid 控制器，并能无痛启动
 *   停止 pid 很简单，就是不要进入计算函数就行
 *   但是启动 pid 就不简单了，因为 pid 控制器的计算需要用到上一次的一些变量
 *   而且期望值和测量值也要在启动时重新设置
 */
#define PID_COMPUTE_ON 1 // 标志进入 pid 计算
#define PID_COMPUTE_OFF 0 // 标志不进入 pid 计算
bool status_flag = PID_COMPUTE_ON; // pid 控制器的状态标志

// 用于积分限幅和输出限幅 // 电机的转速有限，所以也要限幅
#define OUTPUT_PWM_MAX 255
#define OUTPUT_PWM_MIN -255

#define DEAD_ZONE_VOLTAGE_PWM 38 // Arduino: 38/255 量程是 0~255
/**电机转动的死区电压，小于这个电压的电机不转动，分情况决定是否加上去
 * 在进行 pid 控制，也就是还没运动到指定位置时加上
 * pid 控制结束，也就是已经运动到指定位置时不加上
 * 在限幅之前加上去
 */

#define POSITIVE false // 方向设置为这个时，编码器数值正增长，电机逆时针转动
#define NEGATIVE true // 方向设置为这个时，编码器数值负增长，电机顺时针转动
#define LN4 25 // 电机驱动板 L289N 引脚
#define LN3 26 // 电机驱动板 L289N 引脚
#define ENCODER_A 35 // 电机 1 的编码器引脚
#define ENCODER_B 34 // 电机 1 的编码器引脚
#define PWM_PIN_MOTOR_1 13 // 电机 1 的 pwm 引脚

Motor motor1; // 电机 1 对象
Motor motor2; // 电机 2 对象
Motor motor3; // 电机 3 对象
Motor motor4; // 电机 4 对象
Motor* ptr2current_selected_Motor = &motor1; // 指向当前选中的电机对象的指针

// 使用定时器 0 来作为获取采样时间的定时器，并且获取电机的编码器数据之后立刻进行 pid 控制
hw_timer_t * timer0_for_motor_sample = NULL; // 初始化定时器 0 对象，主要是为了初始化定时器进行定时中断
bool flag_of_timer0_itr = false; // 定时中断软件标志位

// 定时中断，非常好编码器库函数会阻塞，使中断看门狗旋转，所以只使用软件中断
void IRAM_ATTR timer0_itr_callback(){
  flag_of_timer0_itr = true;
}

// 初始化用于采样的定时器，并设置采样时间，并且绑定中断函数
void timer0_init_with_ISR(){ // SAMPLE_TIME 相关的宏定义在头文件中
  timer0_for_motor_sample = timerBegin(0, 80, true); // 初始化定时器，设置分频系数为 80，80MHz/80 = 1MHz = 1us，向上计数
  timerAttachInterrupt(timer0_for_motor_sample, &timer0_itr_callback, true); // 绑定中断函数到定时器，边沿触发
  timerAlarmWrite(timer0_for_motor_sample, CONVERT_ms_TO_us(SAMPLE_TIME_ms), true); // 设置自动重装值为 1000000，对应 1000000us 则定时器的周期为 1s，自动重装
  timerAlarmEnable(timer0_for_motor_sample); // 使能定时器
}

// 分为两个部分，一个是驱动板相关的引脚配置，另一个部分是初始化各个电机
void motors_and_board_init(){
  
  // 使用 MCU 设置控制用 L298N 控制电机转向的两对引脚
  pinMode(LN4, OUTPUT); // ln4
  pinMode(LN3, OUTPUT); // ln3
  // 无论用几个电机(2或1)或者哪个特定电机，都要设置这两个引脚，因为这两个引脚是用来控制电机转向的
  /**
   * motor1 <--> ln4, ln3
   * motor2 <--> ln2, ln1
   */

  /* 假定只使用电机 1，再添加其他电机重复下面的步骤 */
  motor1.attach(PWM_PIN_MOTOR_1, POSITIVE, ENCODER_A, ENCODER_B); // 把 motor1 的引脚设置好，包括 pwm 引脚，方向，编码器引脚初始化
  // motor1.output_final = 0.0; // 初始化电机的输出值
  motor1.from_output_final_to_set_pwm(); // 应该要先关闭电机，但是初始化对象的时候，默认的 output_final 输出值正好是 0

}


Motor::Motor(){ // 构造函数，声明对象的时候就会调用，这里不承担初始化的工作，把这份工作交给 attach 函数
    /* 赋一些没意义的值 */
    this->pin_pwm = 0;
    this->flag_of_motor_direction = POSITIVE;
    this->pin_encoder_A = 0;
    this->pin_encoder_B = 0;
    // this 是指向当前构造函数将会生成对象的指针
}

// 给一个电机设置引脚，并分配编码器，设置电机旋转方向
int Motor::attach(int pin_pwm, bool flag_of_motor_direction, int pin_encoder_A, int pin_encoder_B){
    // 初始化电机的所有的引脚
    this->pin_pwm = pin_pwm; // 电机的 pwm 引脚
    this->pin_encoder_A = pin_encoder_A; // 电机的编码器 A 引脚
    this->pin_encoder_B = pin_encoder_B; // 电机的编码器 B 引脚
    
    // 设置电机相关引脚分别为输入输出模式 
    pinMode(this->pin_pwm, OUTPUT);
    pinMode(this->pin_encoder_A, INPUT);
    pinMode(this->pin_encoder_B, INPUT);
    
    // 使用 L298N 驱动电机，所以方向引脚不止一个，作为标志位得了
    this->flag_of_motor_direction = flag_of_motor_direction;
    this->set_motor_direction();

    // 初始化计数模块，准备给电机编码器计数，设置为四倍频计数，提高采样精度
    this->encoder.attachFullQuad(this->pin_encoder_A, this->pin_encoder_B);
    return 0;
}

// 根据电机对象的旋转方向标志位设置电机驱动相关的配置
void Motor::set_motor_direction(){ // L298N 驱动板的两个引脚设置，和这个驱动板耦合
  if (this->flag_of_motor_direction == POSITIVE){
    digitalWrite(LN4, LOW);
    digitalWrite(LN3, HIGH);
  }else if (this->flag_of_motor_direction == NEGATIVE){
    digitalWrite(LN4, HIGH);
    digitalWrite(LN3, LOW);
  }
}

// 读取编码器的计数器脉冲数并清零
int64_t Motor::read_encoder_count_and_clear(){
  int64_t count = this->encoder.getCount(); // 读取编码器的脉冲数，这个库函数是依赖 esp32 的硬件 pcnt 计数器的
  /**不是直接使用 pcnt 计数器的值作为编码器的脉冲累积值
   * 而是在程序中声明一个变量来存储编码器的值
   * 然后通过电机转动 Delta 圈，使得计数器读取到一段 Delta 的值
   * 再把这个值累加到程序中声明的变量中，这个一段一段累加起来的值就是电机的编码器位置值
   * 再把计数器清零，这样可以在有限的计数器位数内存储无限的编码器转了多少圈的位置值
   */
  
  this->encoder.clearCount(); // 采集编码器数据，在采集后要清零 pcnt 计数器，以便 10 ms 之后的下一次采集是真正采集到的 10 ms 内的脉冲数
  return count;
}

// 位置环串联速度环输出，未测试，可以跳过先不看
void Motor:: position_in_serial_connection_to_velocity(){
  // 位置环串联速度环进行控制
  /*
    传入目标位置 -> 位置环-> 得到期望的速度(如果大于传入的目标速度就限幅) -> 速度环 -> 得到 pwm -> 电机 -> 编码器定时采样得到一段时间的脉冲 -> 累加得到代表位置的脉冲
                     |—————————————————————————————————— 编码器测速 ———————|————————————————————————————————————|                         |
                                                                          |—————————————————————————————— 编码器计数 —————————————————————|  
  */

  // 相隔一段时间进入这里采集电机的编码器数据
  this->measure_encoder_count_in_velocity = this->read_encoder_count_and_clear(); // 获取实际一段时间内采集到的脉冲数（可代表转速）
  
  this->measure_encoder_count_in_position += this->measure_encoder_count_in_position; // 累加一段时间的个数，来获取实现记录自开始以来的总脉冲个数（可代表电机转过的角度/圈数）

  this->pid_position_controller(); // 进行位置式 pid，输出值作为目标速度给下一个速度环
  
  // 位置环输出限幅，保证位置环输出的绝对值小于设定好的目标速度
  if (this->output_in_position > this->target_encoder_count_in_velocity){
      this->output_in_position = this->target_encoder_count_in_velocity;
  }
  if (this->output_in_position < -motor1.target_encoder_count_in_velocity){
      this->output_in_position = -motor1.target_encoder_count_in_velocity;
  }// 位置环的动态输出作为速度环的目标速度，使得速度环的目标速度逐渐改变至一开始设定的目标速度
  this->target_encoder_count_in_velocity = this->output_in_position;

  if (abs(motor1.measure_encoder_count_in_position - this->target_encoder_count_in_position) < 3){
    // 系统达到稳定，停止输出 pid 并且 pid 输出设置为 0
    // 但是还有死区电压
    if (status_flag != PID_COMPUTE_OFF){status_flag = PID_COMPUTE_OFF;}
    // 在第一次停止后把 pid 计算状态设置为 PID_COMPUTE_OFF
  }else{
    if (status_flag != PID_COMPUTE_ON){status_flag = PID_COMPUTE_ON; update_config_when_restart_pid_controller();}
    // 如果是从关掉状态进来的，就更新 pid 配置
    this->pid_velocity_controller();
    // 直接把速度环的输出给到电机驱动
    // 注意 pid 输出值有正负
  }
}

// 根据控制器的输出值设置电机的 pwm 输出，注意，这个成员函数操作的参数是成员变量 output_final，在使用这个之前要把这个值计算好
// 这是因为我有三个 pid 控制器，一个是位置 pid 控制器，一个是速度 pid 控制器，还有一个是位置和速度串级 pid 控制器，这些个控制器的输出值是用不一样的成员变量表示的
// 为了复用这个函数，我就把这个函数的参数设置为成员变量 out_put_final，用到什么控制器就要【手动】把对应的输出值赋值或者叠加给这个成员变量
void Motor::from_output_final_to_set_pwm(){// 参数 output_final 要手动设置成你使用到的 pid 控制器的输出值的叠加

  if(this->output_final > 0){ // 输出值为正，让电机的编码器数值正增加????????真的是这样吗，不是的，控制器输出要和过程变量(过程变量就是测量值)镜像
    // 这里是因为我一开始定义的宏 POSITIVE 是 false，所以这里要取反怪怪的，反正就是要镜像
    this->flag_of_motor_direction = POSITIVE;
  }else{ 
    this->flag_of_motor_direction = NEGATIVE;
  }
  this->set_motor_direction();

  if(this->output_final){ // 控制器有输出
    // 这个 output 从 pid 控制器获取时还没加上了死区电压转换的 PWM 值，所以这里要加上
    this->output_final = abs(this->output_final) + DEAD_ZONE_VOLTAGE_PWM;
    // 限幅: arduino analogwrite 限制输出的幅度在 0~255
    if(this->output_final > OUTPUT_PWM_MAX){this->output_final = OUTPUT_PWM_MAX;}
    if(this->output_final < OUTPUT_PWM_MIN){this->output_final = OUTPUT_PWM_MIN;}
    analogWrite(this->pin_pwm, (int)this->output_final);
    
  }else{ // 控制器没有输出，电机不转动
    analogWrite(this->pin_pwm, 0);
  }
}


/**
 * 位置环测试
 * 1.初始化电机、编码器和驱动板引脚和定时器
 * 2.串口中断获取上位机发送的数据，设置好目标位置（用指向对象的指针直接改变对象的成员
 * 3.在主循环中向上位机实时发送 目标位置 和编码器采集到的 位置数据
 * 4.设置一个 LED 闪烁灯，验证程序在主循环正常运行
 * 
 * done:
 * 1.重新标定 kp 符号和负反馈的关系，p 参数是正的才是负反馈
 * 2.处理串口接收的负号
 * 
 * todo:
 * 1.圈数过多，导致 output set pwm 的 pwm 很快就到了最大值，经过限幅之后使得电机转到中间的位置就停止了
 * 
 * done:
 * 问题1
 * 经过开环阶跃测试，发现位置其实是一个积分对象，所以在一开始目标圈数很小的时候，自带的积分作用是够用的，所以积分项在目标圈数很小的时候不设置也可以
 * 但是在目标圈数很大的时候，积分项就要设置，否则电机转到中间的位置就停止了
 */
void Motor::only_position_control_service(){
  /* 在定时器中断函数中使用，每隔一定的采样时间被调用 */
  this->output_in_position = 0; // 确保电机对象在计算之前的默认 pwm 输出是 0

  // 获取实际一段时间内采集到的脉冲数，并累加到测量的总脉冲数中，相隔同样时间累加一次当轮的采样
  this->measure_encoder_count_in_position += this->read_encoder_count_and_clear();

  if (this->measure_encoder_count_in_position == this->target_encoder_count_in_position){
    // 系统达到稳定，停止输出 pid 【以后才慢慢搞，以便下次重新启动 pid 控制器更新状态】
    // 此时 output_position 被之前强制改为 0，并没变
  }else{
    this->pid_position_controller(); // 其实就是普通的并联 pid 控制器
    // 从这个函数出来之后应该电机对象的 output_position 已经被计算改变了
  }
  this->output_final = this->output_in_position; // 手动把 pid 控制器的输出值赋值给 output_final
  this->from_output_final_to_set_pwm(); // 设置电机的 pwm 输出

  // 打印 pid 控制器的输出值和过程变量（测量值）还有设定值
  Serial.printf("sp_count/pv_count/output_pwm:%f,%f,%f\n",this->target_encoder_count_in_position, this->measure_encoder_count_in_position, this->output_final);
}

void Motor::pid_position_controller(){
  /* 开启或关闭 pid 控制器 */
    // TODO：支持关闭后再开启时自动更新 pid 计算用的变量 i_term 和 d_term 
    // TODO：完善参数的正负，反馈的方向约定

  this->error_in_position = this->target_encoder_count_in_position - this->measure_encoder_count_in_position;
  double p_term = this->kp_in_position * this->error_in_position;
  // p_term = kp_in_position *  (input_init - measure_encoder_count_in_position);
  // proportion on measurement

  this->i_term_in_position += this->ki_in_position * this->error_in_position;
  // 把 i_term 单独存储，每一次根据一个 ki 和 error_current 相乘最后各自相加
  // 而不是原始的先把所有 error 求和再统一乘同一个数
  // 这种方式可以支持改变下一次 I 参数而不会产生积分冲击
  

  double d_term = this->kd_in_position * (this->last_input_in_position - this->measure_encoder_count_in_position);
  // 防止微分冲击

  this->output_in_position = p_term + this->i_term_in_position +d_term;
  // 电机特化，加上死区电压
  
  /* 积分限幅和输出限幅 */
  if(this->output_in_position > OUTPUT_PWM_MAX){ 
    // 核心在于 output_in_position 计算完毕但在被限幅之前调整 i_term
    // 这样比直接把 i_term 和 output_in_position 限幅在同一个值更加合理 
    this->i_term_in_position -= this->output_in_position - OUTPUT_PWM_MAX;
    this->output_in_position = OUTPUT_PWM_MAX;
  }
  else if(this->output_in_position < OUTPUT_PWM_MIN){
    this->i_term_in_position += OUTPUT_PWM_MIN - this->output_in_position;
    this->output_in_position = OUTPUT_PWM_MIN;
  }
  
  /* 为下一次的算法计算记住一些变量 */
  this->last_input_in_position = this->measure_encoder_count_in_position;

}

/**
 * 速度环测试
 * 
 * done
 * 1.整数 c++ 除法会向 0 取整，所以要把 1000 转换成 1000.0，以及把宏定义的 SAMPLE_TIME_ms 转换成 double
 * 2.电机满占空比空载转速的 10ms 定时器采样值是 67【68-[69 正常】-70 野兽]，要 pid 控制器想达到距离这个最大值的误差更小，那么这样比例系数就要很大，使得低转速的时候不好控制
 * 3.电机低转速抖动问题
 * 
 * 通过把增量式换成普通并联的形式，用lambda整定法解决上述的问题 2 和问题 3
 * 
 */
void Motor::only_velocity_control_service(){
  this->output_in_velocity = 0; // 确保电机对象在计算之前的默认 pwm 输出是 0

  this->measure_encoder_count_in_velocity = this->read_encoder_count_and_clear(); // 获取实际一段时间内采集到的脉冲数（可代表转速）

  if((this->target_encoder_count_in_velocity == 0) && (abs(this->measure_encoder_count_in_velocity)<15)){
      // 目标速度设置为 0，且测量速度绝对值大小小于 10，认为电机已经停止
  }else{
      // 在这里放置 pid 控制器进入控制
      this->pid_velocity_controller();
      // 出去之后 output_in_velocity 已经被计算好了
  } 
  
  this->output_final = this->output_in_velocity;
  this->from_output_final_to_set_pwm(); // 设置电机的 pwm 输出

  Serial.printf("sp_count/pv_count/output_pwm:%f,%f,%f\n",this->target_encoder_count_in_velocity, this->measure_encoder_count_in_velocity, this->output_final);
}

void Motor::pid_velocity_controller(){ // 普通并联 pid 控制器，不过控制的过程变量是速度，和网上常见的增量式 pid 控制器不同
  /**
   * 因为获取电机速度的方法，就是 mcu 隔一段固定时间得到这段时间电机转角累积得到的脉冲
   * 也就是每次进入定时中断，调用 read_encoder_count_and_clear() 函数，得到的返回值，这个值就表征了电机的转速
   * 表征的方法就是利用电机码盘(也就是转一圈有多少个脉冲数)、采样时间、减速比、MCU采样脉冲的倍频数值
   * 【采样脉冲四倍频的意思就是一个脉冲采样四次，所以read_encoder_count_and_clear()计数的值是实际脉冲的 4 倍】
   * 
   * 因为我们已经知道了采样时间，所以可以根据上述的电机参数计算出这段时间按照期望速度，应该要获得多少脉冲值的，并且 MCU 计到多少数的
   * 这个预先算出来的数就是可以是我们设置 pid 控制器要是过程变量的设定值(过程变量就是测量值，这里是 MCU 计到多少数)
   * 
   * pid 控制器只要保持这个测量值靠近设定值就行了，其实和物理意义就关系不大了
   * 没必要使用增量式 pid 控制器【不是因为我不会整定增量式 pid 控制器hhh】
   * 
   * 只要搞清楚开环【控制器输出】和【过程变量】的阶跃响应，然后用 lambda 整定法调节 pid 控制器的参数就行了，管他什么物理意义都可以把过程变量控制到你想要的设定值
   * 控制器输出和过程变量不是一个量纲的东西，也没有什么物理的因果关系，所以不用管他们的物理意义，也不是一一对应的关系
   * 
   * 只要是控制器输出导致的过程中的一个变量，都可以被 pid 控制器控制，所以才有【过程变量】这个名词的出现
   * 
   * 注意这里使用 lambda 整定在计算积分时间的时候，要小心，因为积分时间是和采样时间有关的，而且是同一个量纲的，所以要小心不要单位错啦
   * 
   * 下面的代码就是普通并联 pid 控制器，和 pid_position_controller() 函数的区别就是换了一套成员变量，从 _position 改成了 _velocity 的不同
   */

  // error = target - measure
  this->error_in_velocity = this->target_encoder_count_in_velocity - this->measure_encoder_count_in_velocity;

  // p_term = kp * error
  double p_term = this->kp_in_velocity * this->error_in_velocity;

  // i_term += ki * error  
  this->i_term_in_velocity += this->ki_in_velocity * this->error_in_velocity;

  // d_term = kd * (error - last_error) 但是防止微分冲击 kd * (last_input - input)
  double d_term = this->kd_in_velocity * (this->last_input_in_velocity - this->measure_encoder_count_in_velocity);

  this->output_in_velocity = p_term + this->i_term_in_velocity + d_term;

  /* 积分限幅和输出限幅 */
  if(this->output_in_velocity > OUTPUT_PWM_MAX){ 
    // 核心在于 output_in_velocity 计算完毕但在被限幅之前调整 i_term
    // 这样比直接把 i_term 和 output_in_velocity 限幅在同一个值更加合理 
    this->i_term_in_velocity -= this->output_in_velocity - OUTPUT_PWM_MAX;
    this->output_in_velocity = OUTPUT_PWM_MAX;
  }
  else if(this->output_in_velocity < OUTPUT_PWM_MIN){
    this->i_term_in_velocity += OUTPUT_PWM_MIN - this->output_in_velocity;
    this->output_in_velocity = OUTPUT_PWM_MIN;
  }

  // 为下一次的算法计算记住一些变量
  this->last_input_in_velocity = this->measure_encoder_count_in_velocity;
}


// 电机的开环阶跃测试，直接在定时中断调用即可，可以用串口直接指定控制器输出什么值，观察响应曲线
void Motor::open_loop_step_test(int flag){  
  switch (flag)
  {
  case FOR_POSITION_PID:
    // 获取过程变量，位置 pid 就是，一段一段的编码器脉冲数累加起来
    this->measure_encoder_count_in_position += this->read_encoder_count_and_clear();
    this->output_final = this->output_in_position;
    this->from_output_final_to_set_pwm();
    // 打印 pid 控制器的输出值和过程变量（测量值）
    Serial.printf("pwm/measure_count:%f,%f\n",this->output_final, this->measure_encoder_count_in_position);

    break;
  case FOR_VELOCITY_PID:
    // 获取过程变量，速度 pid 就是，一段时间内编码器采样值
    this->measure_encoder_count_in_velocity = this->read_encoder_count_and_clear();
    this->output_final = this->output_in_velocity;
    this->from_output_final_to_set_pwm();
    // 打印 pid 控制器的输出值和过程变量（测量值）
    Serial.printf("pwm/measure_count:%f,%f\n",this->output_final, this->measure_encoder_count_in_velocity);
    break;
  
  default:
    break;
  }
}


// 在重启 pid 控制器后更新 pid 参数，未测试，暂时不用
void Motor::update_config_when_restart_pid_controller(){
  // 重新开始 pid 控制器的时候更新配置 i_term 和 d_term 参数
  
  // 位置式 pid 控制器
  // i_term 参数
  this->i_term_in_position = this->output_in_position;
  // 因为 output 是 pid 之前选中的的值，所以用他来初始化 i_term 作为下一阶段的开始起点
  /* 积分限幅和输出限幅 */
  if(this->output_in_position > OUTPUT_PWM_MAX){ 
    // 核心在于 output_in_position 计算完毕但在被限幅之前调整 i_term
    // 这样比直接把 i_term 和 output_in_position 限幅在同一个值更加合理 
    this->i_term_in_position -= this->output_in_position - OUTPUT_PWM_MAX;
    this->output_in_position = OUTPUT_PWM_MAX;
  }
  else if(this->output_in_position < OUTPUT_PWM_MIN){
    this->i_term_in_position += OUTPUT_PWM_MIN - this->output_in_position;
    this->output_in_position = OUTPUT_PWM_MIN;
  }

  // d_term 参数
  this->last_input_in_position = this->measure_encoder_count_in_position;

  // 速度式 pid 控制器
  // i_term 参数
  this->last_error_in_velocity = this->error_in_velocity;
 
  // d_term 参数
  this->last_last_input_in_velocity = this->last_input_in_velocity;
  this->last_input_in_velocity = this->measure_encoder_count_in_velocity;

}

// 增量式 pid 控制器，整定效果不好，暂时不用
void Motor::incremental_pid_controller(){
  /* 开启或关闭 pid 控制器 */
    // 支持关闭后再开启时自动更新 pid 计算用的变量 i_term 和 d_term 
    // TODO：完善参数的正负，反馈的方向约定

  /* 计算误差 */
  this->error_in_velocity = this->target_encoder_count_in_velocity - this->measure_encoder_count_in_velocity;
  double p_term = this->kp_in_velocity * this->error_in_velocity;

  // p_term = kp_in_velocity *  (input_init - measure_encoder_count_in_velocity);
  // proportion on measurement

  double i_term =this->ki_in_velocity * (this->error_in_velocity - this->last_error_in_velocity);

  // double d_term = kd_in_velocity * (error - 2 * last_error + last_last_error);
  double d_term = this->kd_in_velocity * (2 * this->last_input_in_velocity - this->last_last_input_in_velocity - this->measure_encoder_count_in_velocity);
  // measure_encoder_count_in_velocity 就是 input 
  /** 防止微分冲击
   * d(Error)/dt = d(Setpoint)/dt - d(Input)/dt
   * 当 Setpoint 是一个常数时，d(Setpoint)/dt = 0
   * 所以 d(Error)/dt = - d(Input)/dt
   */

  this->output_in_velocity = p_term + i_term + d_term;

  /* 积分限幅和输出限幅 */
  if(this->output_in_velocity > OUTPUT_PWM_MAX){ 
    // 核心在于 output_in_velocity 计算完毕但在被限幅之前调整 i_term
    // 这样比直接把 i_term 和 output_in_velocity 限幅在同一个值更加合理 
    i_term -= this->output_in_velocity - OUTPUT_PWM_MAX;
    this->output_in_velocity = OUTPUT_PWM_MAX;
  }
  else if(this->output_in_velocity < OUTPUT_PWM_MIN){
    i_term += OUTPUT_PWM_MIN - this->output_in_velocity;
    this->output_in_velocity = OUTPUT_PWM_MIN;
  }

  /* 为下一次的算法计算记住一些变量 */
  this->last_last_input_in_velocity = this->last_input_in_velocity;
  this->last_input_in_velocity = this->measure_encoder_count_in_velocity;
  // last_last_error = last_error;
  this->last_error_in_velocity = this->error_in_velocity;

}

// 把电机要转的圈数转换成编码器脉冲数，是位置控制所需要的转接口
double Motor::from_revolution_toget_count(double number_of_revolution){
  // 如果减速比是 30:1，编码器是 11 ppr，那么编码器转 1 圈就有 11 个脉冲
  // 电机输出轴转 1 圈，编码器就转了 30 圈，但是因为是四倍频，所以采集到的脉冲数是 1 * 30 * 11 * 4
  return (number_of_revolution * this->ratio_of_reduction * this->ppr * 4);
}

// 把电机的转速转换成采样时间内编码器的脉冲数，是速度控制需要的转接口，注意 cpp 除法取整的问题
double Motor::from_rpm_toget_count(double rpm){
  // 正解算的感觉
  return (rpm * this->ratio_of_reduction * this->ppr * 4.0 * (((double)SAMPLE_TIME_ms/1000.0/60.0)));
}

// 逆解算，用于显示电机转速，注意 cpp 除法取整的问题
double Motor::from_count_toget_rpm(double encoder_count){
  // double measure_motor_rpm = (((double)encoder_count)/4.0/((double)this->ratio_of_load)/((double)this->ppr))/(((double)SAMPLE_TIME_ms)/1000.0 * 60.0);
  /**
   * 四倍频 -> 实际脉冲数是计数器值的 1/4
   * 减速比 -> 测量的轴并不是直接输出的，输出轴和测量轴相比要减速
   * 码盘一圈的脉冲数 -> 根据比例计算出当前时间采集的脉冲数相当于转了多少圈
   * 采样时间 -> 圈数除时间
   */

  return (((double)encoder_count)/4.0/((double)this->ratio_of_load)/((double)this->ppr))/(((double)SAMPLE_TIME_ms)/1000.0 * 60.0);
}


// 和全局变量类的对象耦合，接收上位机的位姿和速度消息，把他们转换成编码器数值并写入到 pid 控制器目标值，【未测试】以后用到再修改
void backforward_kinematics(double vx, double vy, double omega){

  //车身坐标系，车身前后是 y 轴，车身左右是 x 轴
  //车身的右前轮是 1 号轮，按照逆时针方向依次是 2 号轮，3 号轮，4 号轮
  //车身的速度分量 vx, vy, omega(角速度，量纲还是角度/s)
  double motor1_rpm = (vy - vx + omega * (FRONT_BACK_WHEEL_DISTANCE + LEFT_RIGHT_WHEEL_DISTANCE))/WHEEL_RADIUS/(2*PI)*60;
  double motor2_rpm = (vy + vx - omega * (FRONT_BACK_WHEEL_DISTANCE + LEFT_RIGHT_WHEEL_DISTANCE))/WHEEL_RADIUS/(2*PI)*60;
  double motor3_rpm = (vy - vx - omega * (FRONT_BACK_WHEEL_DISTANCE + LEFT_RIGHT_WHEEL_DISTANCE))/WHEEL_RADIUS/(2*PI)*60;
  double motor4_rpm = (vy + vx + omega * (FRONT_BACK_WHEEL_DISTANCE + LEFT_RIGHT_WHEEL_DISTANCE))/WHEEL_RADIUS/(2*PI)*60;
  // wheel_omega_speed 是 m/s 的单位 -> rad/s ->rpm
  // rpm = wheel_omega_speed/WHEEL_RADIUS/(2*PI)*60;

  motor1.target_encoder_count_in_velocity = motor1.from_rpm_toget_count(motor1_rpm);
  motor2.target_encoder_count_in_velocity = motor2.from_rpm_toget_count(motor2_rpm);
  motor3.target_encoder_count_in_velocity = motor3.from_rpm_toget_count(motor3_rpm);
  motor4.target_encoder_count_in_velocity = motor4.from_rpm_toget_count(motor4_rpm);

}

// 老版本，没和全局变量类的对象耦合，目前也用不到，【未修改 && 未测试】
void forward_kinematics(double wheel_omega_speed1, double wheel_omega_speed2, double wheel_omega_speed3, double wheel_omega_speed4, double* vx, double* vy, double* omega){

  //车身坐标系，车身前后是 y 轴，车身左右是 x 轴
  //车身的右前轮是 1 号轮，按照逆时针方向依次是 2 号轮，3 号轮，4 号轮
  //车身的速度分量 vx, vy, omega(角速度，量纲还是角度/s)
  *vy = (wheel_omega_speed1 + wheel_omega_speed2 + wheel_omega_speed3 + wheel_omega_speed4) / 4;
  *vx = (-wheel_omega_speed1 + wheel_omega_speed2 - wheel_omega_speed3 + wheel_omega_speed4) / 4;
  *omega = (wheel_omega_speed1 - wheel_omega_speed2 - wheel_omega_speed3 + wheel_omega_speed4) / (4 * (FRONT_BACK_WHEEL_DISTANCE + LEFT_RIGHT_WHEEL_DISTANCE));
}



