#include <command.h>

/*
 * 指令缓存队列
 */
Queue_T *g_cmdQueue = 0;
uint16 g_cmdCode = 0x00;
uint16 g_cmdLen = 0x00;
/*******************************************************/
/*                       响应数据缓存                   */
/*******************************************************/
#define RESBUF_PARAM_SIZE 64
struct resData {
    uint16 frame;
    uint16 len;
    uint16 cmdCode;
    uint8 params[RESBUF_PARAM_SIZE];
};
union resBuf {
    uint8 bytes[RESBUF_PARAM_SIZE + 6];
    struct resData data;
};
void _init_resBuf(union resBuf *rbuf) {
    rbuf->data.frame = 0x1400;
    rbuf->data.len = 0x0006;
    rbuf->data.cmdCode = 0x1400;
    for (int i = 0; i < RESBUF_PARAM_SIZE; i++)
        rbuf->data.params[i] = 0;
}
union resBuf g_resBuf;
uint8(*g_resFunc)(const uint8 *buf, uint32 len);
/*
 * Response - 返回数据
 *
 * @code: 响应指令代码
 * @buf: 数据缓存
 * @len: 数据大小
 */
uint8 Response(uint16 code, uint8 *buf, uint16 len) {
    _init_resBuf(&g_resBuf);
    g_resBuf.data.cmdCode = code;
    g_resBuf.data.len = len + 6;

    for (int i = 0; i < len; i++)
        g_resBuf.data.params[i] = buf[i];

    return g_resFunc(g_resBuf.bytes, g_resBuf.data.len);
}

/*******************************************************/
/*                    查询指令                          */
/*******************************************************/

int Query_Id(void) {
    return Response(0x0000, "Cube", 4);
}



/*******************************************************/

/*
 * cmd_init - 初始化指令系统
 *
 * @q: 指令队列
 * @resFunc: 响应回调函数
 */
void cmd_init(Queue_T *q, uint8(*resFunc)(const uint8 *buf, uint32 len)) {
    g_cmdQueue = q;
    g_cmdCode = 0x00;
    g_cmdLen = 0x00;

    _init_resBuf(&g_resBuf);
    g_resFunc = resFunc;
}

void Parse_Command(void) {
    uint8 *tmp = (uint8 *)(&g_cmdCode);
    for (int i = 0; i < 2; i++) {
        while (is_queue_empty(g_cmdQueue));
        dequeue(g_cmdQueue, &tmp[i]);
    }

    tmp = (uint8 *)(&g_cmdLen);
    for (int i = 0; i < 2; i++) {
        while (is_queue_empty(g_cmdQueue));
        dequeue(g_cmdQueue, &tmp[i]);
    }
}

void Exec_Command(void) {
    switch (g_cmdCode) {
    case 0x0000:
        Query_Id(); break;
    }
}

void Clear_Command(void) {
    g_cmdCode = 0x00;
    g_cmdLen = 0x00;
}

