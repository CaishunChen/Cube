#ifndef PID_H
#define PID_H

/*
 * pid_params - PID的参数配置(通用)
 */
struct pid_params {
    double Kp;  /* 比例系数 */
    double Ti;  /* 积分系数 */
    double Td;  /* 微分系数 */
    double T;   /* 采样周期 */
};

/*
 * pid_params_inc - 增量式算法变形后的PID参数
 */
struct pid_params_inc {
    double A;   /* A = 1 + (T/Ti) + (Td/T) */
    double B;   /* B = 1 + 2Td/T           */
    double C;   /* C = KpTd/T              */
};

/*
 * pid_params_general2inc - 通用PID参数转换为增量式参数
 *
 * @general: 通用PID参数
 * @inc: 增量式PID参数
 */
void pid_params_general2inc(const struct pid_params *general, struct pid_params_inc *inc);

/*
 * pid_errors - 增量式PID算法递推公式中前后三次的偏差值
 */
struct pid_errors {
    double ek;      /* e(k)   */
    double ek_1;    /* e(k-1) */
    double ek_2;    /* e(k-2) */
};

/*
 * pid_inc_uk - 增量式PID算法
 *
 * @p: 增量式PID参数
 * @e: 前后三次的偏差值，函数退出后更新结构体 ek_2 = ek_1, ek_1 = ek, ek = 0
 */
double pid_inc_uk(const struct pid_params_inc *p, struct pid_errors *e);

#endif

