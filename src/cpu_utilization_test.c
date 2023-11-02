#include "cpu_utilization_test.h"

#ifdef CUT_TEST

extern void cut_timer_start(void);
extern void cut_timer_stop(void);
extern void cut_report(struct cut_context_t *cut_context);
extern uint32_t cut_get_tick(void);
extern uint32_t cut_get_sys_time(void);
extern uint32_t cut_calc_tick_span(uint32_t start_tick, uint32_t end_tick);
extern uint32_t cut_clac_sys_time_span(uint32_t start_time, uint32_t end_time);
static struct cut_context_t cut_context;

void cut_init(void)
{
    if (CUT_STATE_START == cut_context.state)
    {
        CUT_PRINTF("CUT_TEST is start\r\n");
        return;
    }
    cut_context.run_time_start = 0;
    cut_context.run_time_end = 0;
    cut_context.task_stack_deep = 0;
    cut_context.reimbursed_time_acc = 0;
    cut_context.state = CUT_STATE_STOP;
    CUT_MEMSET(cut_context.task_stack, 0x00, sizeof(enum task_name_t) * (TASK_NEST_MAX_DEEP + 1));
    CUT_MEMSET(cut_context.cut_task_info, 0x00, sizeof(struct cut_task_info) * (TASK_NUM + 1));
    cut_context.init = 1;
}

void cut_start(void)
{
    if (!cut_context.init)
    {
        CUT_PRINTF("CUT TEST not init\r\n");
        return;
    }
    if (CUT_STATE_START == cut_context.state)
    {
        return;
    }
    cut_context.run_time_start = cut_get_sys_time();
    cut_timer_start();
    cut_context.state = CUT_STATE_START;
    CUT_PRINTF("CUT TEST START\r\n");
}

void cut_stop(void)
{
    if (!cut_context.init)
    {
        CUT_PRINTF("CUT TEST not init\r\n");
        return;
    }
    if (CUT_STATE_STOP == cut_context.state)
    {
        return;
    }
    cut_context.state = CUT_STATE_STOP;
    cut_context.run_time_end = cut_get_sys_time();
    cut_timer_stop();
    cut_report(&cut_context);
}

void cut_task_entry(enum task_name_t task)
{
    if (!cut_context.init || CUT_STATE_STOP == cut_context.state)
    {
        return;
    }
#if (CUT_USING_ERROR_CHECK == 1)
    if (cut_context.task_stack_deep >= (TASK_NEST_MAX_DEEP + 1))
    {
        CUT_PRINTF("cut entry: task stack overflow\r\n");
        return;
    }
#endif
    if (0 != cut_context.task_stack_deep)
    {
        cut_context.cut_task_info[cut_context.task_stack[cut_context.task_stack_deep - 1]].acc +=
            cut_calc_tick_span(cut_context.cut_task_info[cut_context.task_stack[cut_context.task_stack_deep - 1]].cur,
                               cut_get_tick());
        cut_context.cut_task_info[cut_context.task_stack[cut_context.task_stack_deep - 1]].acc -= CUT_TEST_NEST_REIMBURSED_TIME;
        cut_context.reimbursed_time_acc += CUT_TEST_NEST_REIMBURSED_TIME;
    }
    cut_context.task_stack[cut_context.task_stack_deep++] = task;
    cut_context.cut_task_info[task].cur = cut_get_tick();
    cut_context.cut_task_info[task].cnt += 1;
}

void cut_task_leave(enum task_name_t task)
{
    if (!cut_context.init || CUT_STATE_STOP == cut_context.state)
    {
        return;
    }
#if (CUT_USING_ERROR_CHECK == 1)
    if (0 == cut_context.task_stack_deep ||
        cut_context.task_stack[cut_context.task_stack_deep - 1] != task)
    {
        CUT_PRINTF("cut leave: task stack overflow\r\n");
        return;
    }
#endif
    cut_context.cut_task_info[task].acc +=
        cut_calc_tick_span(cut_context.cut_task_info[task].cur,
                           cut_get_tick());
    cut_context.cut_task_info[task].acc -= CUT_TEST_REIMBURSED_TIME;
    cut_context.reimbursed_time_acc += CUT_TEST_REIMBURSED_TIME;
    cut_context.task_stack[--cut_context.task_stack_deep] = 0;
    if (cut_context.task_stack_deep)
    {
        cut_context.cut_task_info[cut_context.task_stack[cut_context.task_stack_deep - 1]].cur =
            cut_get_tick();
    }
}

#endif