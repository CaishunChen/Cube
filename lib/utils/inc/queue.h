#ifndef QUEUE_H
#define QUEUE_H

#include "types.h"

typedef struct {
    uint8 *buffer;
    int head;
    int tail;
	int size;
} Queue_T;

/*
 * init_queue - 初始化队列
 *
 * @q: 目标队列
 * @buf: 队列缓存
 * @size: 队列长度，应当比实际缓存长度小1
 */
void init_queue(Queue_T *q, uint8 *buf, int size);
/*
 * get_queue_size - 队列长度
 *
 * @q: 目标队列
 */
int get_queue_size(Queue_T *q);
/*
 * is_queue_empty - 查询队列是否为空
 *
 * @q: 目标队列
 * return: TRUE-空
 */
bool is_queue_empty(Queue_T *q);
/*
 * is_queue_full - 查询队列是否full
 *
 * @q: 目标队列
 * return: TRUE-满
 */
bool is_queue_full(Queue_T *q);
/*
 * enqueue - 将数据插入队尾
 *
 * @q: 目标队列
 * @data: 数据
 * return: TRUE-成功插入队列
 */
bool enqueue(Queue_T *q, uint8 data);
/*
 * dequeue - 队首数据出队
 *
 * @q: 目标队列
 * @re[OUT]: 输出数据
 * return: TRUE-成功出队
 */
bool dequeue(Queue_T *q, uint8 *re);

void clear_queue(Queue_T *q);
#endif
