#pragma once

#include <queue.h>

void cmd_init(Queue_T *q, uint8(*resFunc)(const uint8 *buf, uint32 len));
void Parse_Command(void);
void Exec_Command(void);
void Clear_Command(void);
