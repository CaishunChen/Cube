#include <system.h>
#include <xtos.h>
#include <gVariables.h>
#include <led.h>
#include <usart.h>
#include <command.h>
#include <cube.h>
#include <timer.h>
#include <mpu6050.h>

#include <stdio.h>

void config_interruts(void);

bool isStarted = FALSE;

float fdt = 0;
struct sys_time gTime1;
uint8 data = 0;

float gvx = 0;
float gvy = 0;

void ctrl_routine(void) {
    if (!isStarted)
        return;

    struct sys_time time;
    sys_get_time(&time);
    fdt = sys_time_diff_fms(&gTime1, &time);
    gTime1.ms = time.ms;
    gTime1.us = time.us;

    gvx = (gCube.xn_acc - gCube.xn_acc_bias) * fdt * 0.001f;
    gvy = (gCube.yn_acc - gCube.yn_acc_bias) * fdt * 0.001f;

    if (!gCtrolStarted) {
        return;
    }

    gCube.rx += gCube.vx * fdt * 0.001f;
    gCube.ry += gCube.vy * fdt * 0.001f;

    gCube.vx += gvx;
    gCube.vy += gvy;

    // 位置控制
    if (gCube.ctrl.bits.pos_ctrl) {
        // todo
    }
}

/*******************************************/


#define TASKA_STK_SIZE 1024
#define TASKB_STK_SIZE 1024
static uint32 taskA_Stk[TASKA_STK_SIZE];
static uint32 taskB_Stk[TASKB_STK_SIZE];

static struct xtos_task_descriptor taskA;
static struct xtos_task_descriptor taskB;

void taska() {
    while (1) {
        LED_2 = LED_ON;
        data = mpu6050_read_uint8(&gMpu6050, 0x3A);
        if (data & 0x01) {
            mpu6050_pose();
        }

        if (gCube.ctrl.bits.pos_ctrl) {
            gCube.ctrl.bits.dir_ctrl = 0;
            gCube.ctrl.bits.motor_ctrl = 0;
            // 位置控制操作
        } else if (gCube.ctrl.bits.dir_ctrl) {
            gCube.ctrl.bits.motor_ctrl = 0;
            // 方向控制操作
            
        } else if (gCube.ctrl.bits.motor_ctrl) {
            for (int i = 0; i < 6; i++) {
                gMotor[i].en[0] = (gCube.ens.all & (0x01 << i)) ? 1 : 0;
                motor_set_pwm_duty(&gMotor[i], gCube.pwms[i]);
            }
        }
        LED_2 = LED_OFF;
    }
}

void taskb() {
    cmd_init(&gU1RxQ, usart1_send_bytes);
    while (1) {
        Parse_Command();
        Exec_Command();
        Clear_Command();
    }
}
/*******************************************/

int main(void) {
    isStarted = FALSE;
    sys_init();
    led_init();
    usart1_init(115200);

    timer_init(1000, 5000);
    control_func = ctrl_routine;
    sys_get_time(&gTime1);

    cube_init();
    
    config_interruts();

    printf("initialize the mpu6050\r\n");
    LED_2 = LED_ON;

    uint8 err = mpu6050_init();
    if (err) {
        printf("error, initialize the mpu6050: 0x%x\r\n", err);
        while (1);
    }

    printf("mpu6050 initialized\r\n");
    isStarted = TRUE;

    xtos_init();
    xtos_init_task_descriptor(&taskA, taska, &taskA_Stk[TASKA_STK_SIZE - 1], 0);
    xtos_init_task_descriptor(&taskB, taskb, &taskB_Stk[TASKB_STK_SIZE - 1], 1);
    xtos_start();


    while (1) {

    }
}


