#ifndef _CPU_UTILIZATION_TEST_
#define _CPU_UTILIZATION_TEST_

#include <stdint.h>
#include "SEGGER_RTT.h"
#include "cpu_utilization_test_cfg.h"

#ifdef CUT_TEST

#define TASK_NEST_MAX_DEEP TASK_NUM                                  // 任务嵌套深度
#define CUT_TEST_REIMBURSED_TIME (CUT_GPT_TIMER_PERIOD / 1000000)    // 时间补偿(定时器时钟频率 / 10000000)
#define CUT_TEST_NEST_REIMBURSED_TIME CUT_TEST_REIMBURSED_TIME * 1.5 // 嵌套时间补偿(定时器时钟频率 / 10000000 * 1.5)

enum task_name_t
{
#define TASK_DEFINE(task) task,
    TASK_TABLE
#undef TASK_DEFINE
        TASK_NUM
};

enum run_state_t
{
    CUT_STATE_STOP,
    CUT_STATE_START
};

struct cut_task_info
{
    int32_t acc;
    uint32_t cur;
    uint32_t cnt;
};

struct cut_context_t
{
    uint8_t init;
    enum run_state_t state;
    uint16_t task_stack_deep;
    uint32_t run_time_start;
    uint32_t run_time_end;
    uint32_t reimbursed_time_acc;
    enum task_name_t task_stack[TASK_NEST_MAX_DEEP + 1];
    struct cut_task_info cut_task_info[TASK_NUM + 1];
};

extern void cut_init(void);
extern void cut_start(void);
extern void cut_stop(void);
extern void cut_task_entry(enum task_name_t task);
extern void cut_task_leave(enum task_name_t task);
extern char *task_description[];

#endif

/* Pubilc API*/
#ifdef CUT_TEST
#define CUT_INIT cut_init
#define CUT_START cut_start
#define CUT_STOP cut_stop
#define CUT_TASK_ENTRY(x) cut_task_entry(x)
#define CUT_TASK_LEAVE(x) cut_task_leave(x)
#else
#define CUT_INIT()
#define CUT_START()
#define CUT_STOP()
#define CUT_TASK_ENTRY(x)
#define CUT_TASK_LEAVE(x)
#endif

#endif