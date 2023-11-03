#include "cpu_utilization_test.h"

#ifdef CUT_TEST

/// @brief 定时器开启
/// @param
void cut_timer_start(void)
{
}

/// @brief 定时器关闭
/// @param
void cut_timer_stop(void)
{
}

/// @brief 获取定时器CNT
/// @param
/// @return
uint32_t cut_get_tick(void)
{
    return 0; // 通过寄存器获取减小耗时
}

/// @brief 获取系统时间
/// @param
/// @return
uint32_t cut_get_sys_time(void)
{
    return 0;
}

/// @brief 计算定时器计数间隔
/// @param start_tick
/// @param end_tick
/// @return
uint32_t cut_calc_tick_span(uint32_t start_tick, uint32_t end_tick)
{
    return 0;
}

/// @brief 计算系统运行间隔
/// @param start_time
/// @param end_time
/// @return
uint32_t cut_clac_sys_time_span(uint32_t start_time, uint32_t end_time)
{
    return 0;
}

/// @brief 报告
/// @param cut_context
void cut_report(struct cut_context_t *cut_context)
{
    uint8_t i = 0;
    uint32_t runTime;
    CUT_PRINTF(">>>>>>>>>>>>>>>>>\r\n");
    CUT_PRINTF("User Report:\r\n");
    runTime = cut_clac_sys_time_span(cut_context->run_time_start, cut_context->run_time_end) * 100;
    CUT_PRINTF("Total runTime:%d us\r\n", runTime);
    runTime = runTime - cut_context->reimbursed_time_acc;
    CUT_PRINTF("Real runTime:%d us\r\n", runTime);
    CUT_PRINTF("%s\tTime(us)\tUsage(%%)\tCnt\tAvg(us)\t\r\n", "Task");
    for (i = 0; i < TASK_NUM; i++)
    {
        if (cut_context->cut_task_info[i].acc > 0)
        {
            CUT_PRINTF("%s\t\%f(us)\t%f%%\t%d\t%f(us)\r\n", task_description[i],
                       (float)cut_context->cut_task_info[i].acc * 0.25,
                       (float)cut_context->cut_task_info[i].acc * 0.25 / runTime * 100,
                       cut_context->cut_task_info[i].cnt,
                       (float)cut_context->cut_task_info[i].acc * 0.25 / cut_context->cut_task_info[i].cnt);
        }
        else
        {
            // 如果运行时间小于补偿时间 则acc < 0
            CUT_PRINTF("%s\t0us\t0%%\t0\t0us\r\n", task_description[i]);
        }
    }
    CUT_PRINTF(">>>>>>>>>>>>>>>>>\r\n");
}
#endif