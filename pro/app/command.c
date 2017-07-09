#include <command.h>
#include <xtos.h>
#include <types.h>
#include <gVariables.h>
#include <stdio.h>
/*
 * 指令缓存队列
 */
Queue_T *g_cmdQueue = 0;
union Data16 _startAddr;
union Data16 _cmdLen;
bool _isWriteCmd;

#define ErrorNoErr              0
#define ErrorStartAddrTooLarge  1
#define ErrorInvalidCmd         2
uint8 _error = ErrorNoErr;

uint8(*g_resFunc)(const uint8 *buf, uint32 len);

/*******************************************************/

static uint8 _get_byte(void) {
    uint8 result;
    while (is_queue_empty(g_cmdQueue)) {
        xtos_schedule();
    }
    dequeue(g_cmdQueue, &result);
    return result;
}

static void _send_datas(void) {
    uint16 len = sizeof(gCube) - _startAddr.half_word;
    len = (_cmdLen.half_word > len) ? len : _cmdLen.half_word;
    uint8 *addr = (uint8*)&gCube;
    g_resFunc(addr + _startAddr.half_word, len);
}

static void _receive_datas(void) {
    uint16 len = sizeof(gCube) - _startAddr.half_word;
    len = (_cmdLen.half_word > len) ? len : _cmdLen.half_word;
    uint8 *buf = (uint8*)&gCube;
    uint16 add = _startAddr.half_word;
    for (int i = 0; i < len; i++) {
        uint8 tmp = _get_byte();
        add = i + _startAddr.half_word;
        if (add < CUBE_WRITABLE_ADDR)
            continue;
        buf[add] = tmp;
    }

    return;
}

/*
 * cmd_init - 初始化指令系统
 *
 * @q: 指令队列
 * @resFunc: 响应回调函数
 */
void cmd_init(Queue_T *q, uint8(*resFunc)(const uint8 *buf, uint32 len)) {
    g_cmdQueue = q;
    _startAddr.half_word = sizeof(gCube);
    _cmdLen.half_word = 0x00;

    g_resFunc = resFunc;
}


void Parse_Command(void) {
    while (0x55 != _get_byte());
    if (0xAA != _get_byte())
        return;

    _startAddr.byte[0] = _get_byte();
    _startAddr.byte[1] = _get_byte();

    _isWriteCmd = (0x8000 & _startAddr.half_word) ? TRUE : FALSE;
    _startAddr.half_word &= 0x7FFF;

    if (_startAddr.half_word >= sizeof(gCube)) {
        _error = ErrorStartAddrTooLarge;
        return;
    }

    _cmdLen.byte[0] = _get_byte();
    _cmdLen.byte[1] = _get_byte();
}

void Exec_Command(void) {
    uint16 tmp = 0xAA55;
    uint16 add = (_isWriteCmd) ? (0x8000 | _startAddr.half_word) : _startAddr.half_word;

    g_resFunc((uint8*)&tmp, 2);
    g_resFunc((uint8*)&add, 2);
    g_resFunc(_cmdLen.byte, 2);

    if (_startAddr.half_word >= sizeof(gCube)) {
        g_resFunc(&_error, 1);
        return;
    }

    if (_isWriteCmd) {
        g_resFunc(&_error, 1);
        _receive_datas();
    } else {
        g_resFunc(&_error, 1);
        _send_datas();
    }
}

void Clear_Command(void) {
    _startAddr.half_word = sizeof(gCube);
    _cmdLen.half_word = 0x00;
}

