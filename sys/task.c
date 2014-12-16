#include "types.h"
#include "task.h"
#include "dc.h"
#include "prot.h"
#include "pmu.h"
#include "batt.h"
#include "kpd.h"
#include "led.h"

task_queue_t	q;
task_func_t		funcs[MSG_ID_TOTAL] = {
    Prot_Cmd_Handler,
    DC_Sampling,
    DC_Dispatch,
    PMU_IndHandler,
    Batt_StatusHandler,
    KPD_PressInd,
    KPD_ReleaseInd,
    KPD_LongPressInd,
    LED_FlashInd
};

#ifdef TASK_OPTIMIZE
/*
 * Add a task into task queue
 *   - return queue rear or TASK_ERROR if queue full
 */
s8 Task_Push(u8 msg_id, u8 parm1, u16 parm2)
{
    if ((q.rear + 1) & QSIZE_MASK == q.front)	/* queue full */
        return TASK_ERROR;

    q.msgs[q.rear].id = msg_id;
    q.msgs[q.rear].parm1 = parm1;
    q.msgs[q.rear].parm2 = parm2;
    q.rear = (q.rear + 1) & QSIZE_MASK;

    return q.rear;
}

/*
 * Get current task in the task queue
 *   - return msg_id or TASK_ERROR if queue emtpy
 */
s8 Task_GetCurrent(void)
{
    if (q.front == q.rear)
        return TASK_ERROR;

    return q.msgs[q.front].id;
}

#endif

/*
 * Remove a task from task queue
 *   - return msg_id or TASK_ERROR if queue empty
 */
s8 Task_Pop(void)
{
    u8 msg_id;

    if (q.front == q.rear)
        return TASK_ERROR;

    msg_id = q.msgs[q.front].id;
    q.front = (q.front + 1) & QSIZE_MASK;

    return msg_id;
}
