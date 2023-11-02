#ifndef _CPU_UTILIZATION_TEST_CFG_H_
#define _CPU_UTILIZATION_TEST_CFG_H_

#define CUT_TEST // 模块开关（代码裁剪）

#ifdef CUT_TEST

#include <string.h>
#include "SEGGER_RTT.h"

#define CUT_MEMSET memset
#define CUT_PRINTF(...) SEGGER_RTT_printf(0, __VA_ARGS__) // 打印函数

#define CUT_USING_ERROR_CHECK 0      // 错误检查（关闭以减少测试开销）
#define CUT_GPT_TIMER_PERIOD 4000000 // 定时器时钟频率

// 任务定义（用户按照如下格式定义任务）
#define TASK_TABLE                        \
    TASK_DEFINE(TASK_2ms)                 \
    TASK_DEFINE(TASK_5ms)                 \
    TASK_DEFINE(TASK_10ms)                \
    TASK_DEFINE(TASK_LPIT_0_CH_0_ISR)     \
    TASK_DEFINE(TASK_Can_IsrFCA_MB_00_15) \
    TASK_DEFINE(TASK_MCL_DMA_CH_4_ISR)    \
    TASK_DEFINE(TASK_FTM_3_CH_0_CH_1_ISR)

#endif

#endif