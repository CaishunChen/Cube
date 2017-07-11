#include <pid.h>
#include <assert.h>

/*
 * pid_params_general2inc - 通用PID参数转换为增量式参数
 *
 * @general: 通用PID参数
 * @inc: 增量式PID参数
 */
void pid_params_general2inc(const struct pid_params *general, struct pid_params_inc *inc) {
    if (0 == general->Ti)
        inc->A = 1 + (general->Td / general->T);
    else
        inc->A = 1 + (general->T / general->Ti) + (general->Td / general->T);
    inc->A = general->Kp * inc->A;
    inc->B = -general->Kp * (1 + 2 * general->Td / general->T);
    inc->C = general->Kp * general->Td / general->T;
}

/*
* pid_inc_uk - 增量式PID算法
*
* @p: 增量式PID参数
* @e: 前后三次的偏差值，函数退出后更新结构体 ek_2 = ek_1, ek_1 = ek, ek = 0
*/
double pid_inc_uk(const struct pid_params_inc *p, struct pid_errors *e) {
    double re = (p->A * e->ek) + (p->B * e->ek_1) + (p->C * e->ek_2);
    e->ek_2 = e->ek_1;
    e->ek_1 = e->ek;
    e->ek = 0;

    return re;
}

