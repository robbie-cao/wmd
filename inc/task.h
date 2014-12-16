#ifndef __TASK_H__
#define __TASK_H__

#include "types.h"

#define TASK_OPTIMIZE

#define QSIZE_LOG2		2
#define QSIZE			(1 << QSIZE_LOG2)
#define QSIZE_MASK		(QSIZE - 1)

#define TASK_ERROR		(-1)
#define TASK_OK			0

enum {
    MSG_ID_CMDRCV_IND,
    MSG_ID_SAMP_IND,
    MSG_ID_DC_IND,
    MSG_ID_PMU_IND,
    MSG_ID_BATT_IND,
    MSG_ID_KEYPR_IND,
    MSG_ID_KEYREL_IND,
    MSG_ID_KEYLP_IND,
    MSG_ID_LED_FLASH,

    MSG_ID_TOTAL
};

typedef struct
{
    u8	id;
    u8	parm1;
    u16	parm2;
} task_msg_t;

typedef struct
{
    task_msg_t	msgs[QSIZE];
    u8			front;
    u8			rear;
} task_queue_t;

typedef void (*task_func_t)(void);

extern task_queue_t	q;
extern task_func_t	funcs[];

#define MSG_IDX		q.front
#define MSG_PARM1	q.msgs[q.front].parm1
#define MSG_PARM2	q.msgs[q.front].parm2

#ifdef TASK_OPTIMIZE
/*
 * Add a task into task queue
 *   - return queue rear or TASK_ERROR if queue full
 */
s8		Task_Push(u8 msg_id, u8 parm1, u16 parm2);
/*
 * Get current task in the task queue
 *   - return msg_id or TASK_ERROR if queue emtpy
 */
s8		Task_GetCurrent(void);

#else

#define Task_Push(msg_id, _parm1, _parm2)		\
    ((q.rear + 1) & QSIZE_MASK == q.front) ?	\
TASK_ERROR :								\
((q.msgs[q.rear].id = (msg_id)), 			\
 (q.msgs[q.rear].parm1 = (_parm1)), 		\
 (q.msgs[q.rear].parm2 = (_parm2)), 		\
 (q.rear = (q.rear + 1) & QSIZE_MASK) 		\
)

#define Task_GetCurrent()			\
    (q.front == q.rear) ? 			\
TASK_ERROR : 					\
q.msgs[q.front].id

#endif

/* Task queue initialization */
#define Task_Init()		(q.front = q.rear = 0)

/*
 * Remove a task from task queue
 *   - return msg_id or TASK_ERROR if queue empty
 */
s8		Task_Pop(void);

/* Execut the task */
#define Task_Run(msg_id)			\
    do {							\
        if (funcs[msg_id]) {		\
            funcs[msg_id]();		\
        }							\
    } while (0)

#endif
