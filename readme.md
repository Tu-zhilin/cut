# CUT

简易CPU负载率测试

## 1. 概述

[传送门](https://blog.csdn.net/qq_43629659/article/details/134167873)

## 2. API

| 函数           | 描述         |
| :------------- | :----------- |
| CUT_INIT       | 模块初始化   |
| CUT_START      | 开始测试     |
| CUT_STOP       | 结束测试     |
| CUT_TASK_ENTRY | 任务计时开始 |
| CUT_TASK_LEAVE | 任务计时结束 |

## 3. 使用

`开始测试`

```c
CUT_INIT();
CUT_START();
```

`停止测试`

```c
CUT_STOP(); // 此函数会调用打印报告函数
```

`任务计时`

```c
void Task_5ms(void)
{
    CUT_TASK_ENTRY(TASK_NAME); // 传入定义的任务名称
    
    // code...
    
    CUT_TASK_LEAVE(TASK_NAME); // 传入定义的任务名称
}
```

## 4. 移植

| 文件                        | 描述                 |
| --------------------------- | -------------------- |
| cpu_utilization_test.c      | 逻辑代码             |
| cpu_utilization_test.h      | 头文件               |
| cpu_utilization_test_cfg.h  | 配置文件（用户配置） |
| cpu_utilization_test_port.c | 接口文件（用户实现） |

1. 将上述4个文件添加到工程中
2. 用户需要实现接口文件`cpu_utilization_test_port.c`

| 函数                   | 描述             |
| ---------------------- | ---------------- |
| cut_timer_start        | 打开定时器       |
| cut_timer_stop         | 关闭定时器       |
| cut_report             | 打印报告         |
| cut_get_tick           | 获取定时器CNT    |
| cut_get_sys_time       | 获取系统运行时间 |
| cut_calc_tick_span     | 计数间隔时间计算 |
| cut_clac_sys_time_span | 系统运行时间计算 |

3. 用户根据芯片配置文件`cpu_utilization_test_cfg.h`

| 配置项                | 描述                                       |
| --------------------- | ------------------------------------------ |
| CUT_MEMSET            | 内存初始化函数                             |
| CUT_PRINTF            | 打印函数                                   |
| CUT_USING_ERROR_CHECK | 错误检查开关（关闭以减小测试代码运行时间） |
| CUT_GPT_TIMER_PERIOD  | 定时器时钟频率                             |
| TASK_TABLE            | 任务定义                                   |

  任务定义如下格式，定义完之后会自动生成枚举和任务描述数组：

```c
#define TASK_TABLE                        \
    TASK_DEFINE(TASK_2ms)                 \
    TASK_DEFINE(TASK_5ms)                 \
    TASK_DEFINE(TASK_10ms)                \
    TASK_DEFINE(TASK_LPIT_0_CH_0_ISR)     \
    TASK_DEFINE(TASK_Can_IsrFCA_MB_00_15) \
    TASK_DEFINE(TASK_MCL_DMA_CH_4_ISR)    \
    TASK_DEFINE(TASK_FTM_3_CH_0_CH_1_ISR)
```