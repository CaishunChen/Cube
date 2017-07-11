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

    if (!gCtrolStarted) {
        return;
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

        LED_2 = LED_OFF;
    }
}

void taskb() {
    uint8 cmd;
    cmd_init(&gU1RxQ, usart1_send_bytes);

    cube_enable_dir(&gXDir);
    cube_enable_dir(&gZDir);

    while (1) {
        while (is_queue_empty(&gU1RxQ)) {
            xtos_schedule();
        }
        dequeue(&gU1RxQ, &cmd);
        switch (cmd) {
        case 'F':
            cube_set_forward_ctrl(&gXDir, 0.1);
            break;
        case 'B':
            cube_set_forward_ctrl(&gXDir, -0.1);
            break;
        case 'L':
            cube_set_dir_ctrl(&gZDir, 0.1);
            break;
        case 'R':
            cube_set_dir_ctrl(&gZDir, -0.1);
            break;
        case 'O':
            cube_set_dir_ctrl(&gZDir, 0.0);
            break;
        case 'P':
            cube_set_dir_ctrl(&gXDir, 0.0);
            break;
        }
        /*
        Parse_Command();
        Exec_Command();
        Clear_Command();
        */
    }
}
/*******************************************/

int main(void) {
    isStarted = FALSE;
    sys_init();
    led_init();
    usart1_init(9600);

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


