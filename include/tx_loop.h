#ifndef _TX_LOOP_H_
#define _TX_LOOP_H_

#if defined(WIN32) && defined(SLIST_ENTRY)
#warning "SLIST_ENTRY is aready defined"
#undef SLIST_ENTRY
#endif

#include <sys/queue.h>

#define TASK_IDLE 0x1

struct tx_poll_t;

struct tx_task_t {
	int tx_flags;
	void *tx_data;
	void (*tx_call)(void *ctx);
	struct tx_loop_t *tx_loop;
	TAILQ_ENTRY(tx_task_t) entries;
};

TAILQ_HEAD(tx_task_q, tx_task_t);

struct tx_loop_t {
	int tx_busy;
	int tx_stop;
	int tx_upcount;
	void *tx_holder;
	tx_poll_t *tx_poller;
	tx_task_q tx_taskq;
};

struct tx_loop_t *tx_loop_new(void);
struct tx_loop_t *tx_loop_default(void);
struct tx_loop_t *tx_loop_get(tx_task_t *task);

int  tx_loop_timeout(tx_loop_t *up, const void *verify);
void tx_loop_delete(tx_loop_t *up);
void tx_loop_main(tx_loop_t *up);
void tx_loop_stop(tx_loop_t *up);

void tx_task_init(tx_task_t *task, tx_loop_t *loop, void (*call)(void *), void *ctx);
void tx_task_active(tx_task_t *task);
void tx_task_drop(tx_task_t *task);

struct tx_iocb_t;
struct tx_wait_t {
	int tx_flag;
	tx_task_t *tx_task;
	tx_iocb_t *tx_iocb;
	LIST_ENTRY(tx_wait_t) entries;
};

#define WAIT_IDLE 0x1
typedef LIST_HEAD(tx_wait_q, tx_wait_t) tx_wait_q;

struct tx_iocb_t {
	int tx_flag;
	tx_wait_q tx_waitq;
};

int  tx_wait_init(tx_wait_t *wcbp, tx_iocb_t *iocbp, tx_task_t *task);
int  tx_wait_active(tx_wait_t *wcbp);
int  tx_wait_cancel(tx_wait_t *wcbp);

#endif
