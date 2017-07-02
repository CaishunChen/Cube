#include "queue.h"

/*
 * init_queue - 初始化队列
 *
 * @q: 目标队列
 * @buf: 队列缓存
 * @size: 队列长度，应当比实际缓存长度小1
 */
void init_queue(Queue_T *q, uint8 *buf, int size) {
    q->head = 0;
    q->tail = 0;
	q->size = size;
	q->buffer = buf;
}

static void fix_queue_edge(Queue_T *q) {
    if ((q->size + 1) == q->tail)
        q->tail = 0;
    if ((q->size + 1) == q->head)
        q->head = 0;
}

/*
 * get_queue_size - 队列长度
 *
 * @q: 目标队列
 */
int get_queue_size(Queue_T *q) {
    fix_queue_edge(q);
    
    if (q->head < q->tail)
        return q->tail - q->head;
    else if (q->head == q->tail)
        return 0;
    else
        return (q->size - q->head) + q->tail +1;
}
/*
 * is_queue_empty - 查询队列是否为空
 *
 * @q: 目标队列
 * return: TRUE-空
 */
bool is_queue_empty(Queue_T *q) {
    fix_queue_edge(q);
    return (q->head == q->tail) ? TRUE : FALSE;
}
/*
 * is_queue_full - 查询队列是否full
 *
 * @q: 目标队列
 * return: TRUE-满
 */
bool is_queue_full(Queue_T *q) {
    return (q->size <= get_queue_size(q)) ? TRUE : FALSE;
}
/*
 * enqueue - 将数据插入队尾
 *
 * @q: 目标队列
 * @data: 数据
 * return: TRUE-成功插入队列
 */
bool enqueue(Queue_T *q, uint8 data) {
    // 队列已满
    if (is_queue_full(q))
        return FALSE;
    
    q->buffer[q->tail] = data;
    q->tail++;
    return TRUE;
}
/*
 * dequeue - 队首数据出队
 *
 * @q: 目标队列
 * @re[OUT]: 输出数据
 * return: TRUE-成功出队
 */
bool dequeue(Queue_T *q, uint8 *re) {
    // 队列为空
    if (is_queue_empty(q))
        return FALSE;

    *re = q->buffer[q->head];
    q->head++;
    return TRUE;
}

void clear_queue(Queue_T *q) {
	q->head = 0;
	q->tail = 0;
}
