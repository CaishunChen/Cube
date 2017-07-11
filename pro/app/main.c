#include <system.h>
#include <xtos.h>
#include <gVariables.h>
#include <led.h>
#include <usart.h>
#include <command.h>
#include <cube.h>
#include <timer.h>
#include <mpu6050.h>
#include <pid.h>
#include <stdio.h>

void config_interruts(void);

bool isStarted = FALSE;
bool gDirCtrl = FALSE;
bool gXCtrl = FALSE;
bool gYCtrl = FALSE;
bool gAccCalibrate = FALSE;

float fdt = 0;
struct sys_time gTime1;
uint8 data = 0;

#define CTRL_PERIOD 1

// Yaw自由度PID控制
#define MAX_YAW_DUTY 0.1
float gYawCmd = 0.0;
struct pid_errors gYawErr = { 0.0, 0.0, 0.0 };
struct pid_params_inc gYawPidI; // 增量Pid控制参数
struct pid_params gYawPid = {
    .Kp = 0.01,
    .Ti = 0.0001,
    .Td = 0,
    .T = CTRL_PERIOD
};
double yaw_ctl = 0.0;
// XV自由度PID控制
#define MAX_XV_DUTY 0.1
float gXVCmd = 0.0;
struct pid_errors gXVErr = { 0.0, 0.0, 0.0 };
struct pid_params_inc gXVPidI; // 增量Pid控制参数
struct pid_params gXVPid = {
    .Kp = 0.001,
    .Ti = 0.000001,
    .Td = 0,
    .T = CTRL_PERIOD
};
double xv_ctl = 0.0;
// YV自由度PID控制
#define MAX_YV_DUTY 0.1
float gYVCmd = 0.0;
struct pid_errors gYVErr = { 0.0, 0.0, 0.0 };
struct pid_params_inc gYVPidI; // 增量Pid控制参数
struct pid_params gYVPid = {
    .Kp = 1,
    .Ti = 0.0001,
    .Td = 0,
    .T = CTRL_PERIOD
};
double yv_ctl = 0.0;

float gV[3] = { 0.0, 0.0, 0.0 };
float gR[3] = { 0.0, 0.0, 0.0 };
float gAccBG[3] = { 0.0, 0.0, 0.0 };

double nomalize(float value, float lmt) {
    value = value / lmt;
    if (value > 1.0f)
        return 1.0;
    if (value < -1.0f)
        return -1.0;
    return value;
}

void ctrl_routine(void) {
    if (!isStarted)
        return;

    if (!gAccCalibrate) {
        gAccBG[0] = gCube.m[0 * 3 + 0] * gCube.xn_acc_bias + gCube.m[1 * 3 + 0] * gCube.yn_acc_bias + gCube.m[2 * 3 + 0] * gCube.zn_acc_bias;
        gAccBG[1] = gCube.m[0 * 3 + 1] * gCube.xn_acc_bias + gCube.m[1 * 3 + 1] * gCube.yn_acc_bias + gCube.m[2 * 3 + 1] * gCube.zn_acc_bias;
        gAccBG[2] = gCube.m[0 * 3 + 2] * gCube.xn_acc_bias + gCube.m[1 * 3 + 2] * gCube.yn_acc_bias + gCube.m[2 * 3 + 2] * gCube.zn_acc_bias;

        gV[0] += 0.001f * (gCube.xb_acc - gAccBG[0]);
        gV[1] += 0.001f * (gCube.yb_acc - gAccBG[1]);
        gV[2] += 0.001f * (gCube.zb_acc - gAccBG[2]);

        gR[0] += 0.001f * gV[0];
        gR[1] += 0.001f * gV[1];
        gR[2] += 0.001f * gV[2];
    } else {
        gV[0] = 0.0;
        gV[1] = 0.0;
        gV[2] = 0.0;

        gR[0] = 0.0;
        gR[1] = 0.0;
        gR[2] = 0.0;
    }

    if (gDirCtrl) {
        gYawErr.ek = nomalize(gYawCmd, 180.0) - nomalize(gCube.yaw, 180.0);
        yaw_ctl += pid_inc_uk(&gYawPidI, &gYawErr);
        yaw_ctl = (yaw_ctl < -MAX_YAW_DUTY) ? -MAX_YAW_DUTY : yaw_ctl;
        yaw_ctl = (yaw_ctl > MAX_YAW_DUTY) ? MAX_YAW_DUTY : yaw_ctl;
        cube_set_dir_ctrl(&gZDir, yaw_ctl);
    }

    if (gXCtrl) {
        gXVErr.ek = nomalize(gXVCmd, 0.1) - nomalize(gR[0], 0.1);
        xv_ctl += pid_inc_uk(&gXVPidI, &gXVErr);
        xv_ctl = (xv_ctl < -MAX_XV_DUTY) ? -MAX_XV_DUTY : xv_ctl;
        xv_ctl = (xv_ctl > MAX_XV_DUTY) ? MAX_XV_DUTY : xv_ctl;
        cube_set_forward_ctrl(&gXDir, xv_ctl);
    }

    if (gYCtrl) {
        gYVErr.ek = nomalize(gYVCmd, 0.1) - nomalize(gR[1], 0.1);
        yv_ctl += pid_inc_uk(&gYVPidI, &gYVErr);
        yv_ctl = (yv_ctl < -MAX_YV_DUTY) ? -MAX_YV_DUTY : yv_ctl;
        yv_ctl = (yv_ctl > MAX_YV_DUTY) ? MAX_YV_DUTY : yv_ctl;
        cube_set_forward_ctrl(&gYDir, yv_ctl);
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
    pid_params_general2inc(&gYawPid, &gYawPidI);
    pid_params_general2inc(&gXVPid, &gXVPidI);
    pid_params_general2inc(&gYVPid, &gYVPidI);
    //mpu6050_calibrate();
    printf("start mpu6050 pose!\r\n");
    while (1) {
        LED_2 = LED_ON;
        data = mpu6050_read_uint8(&gMpu6050, 0x3A);
        if (data & 0x01) {
            mpu6050_pose();
        }
        if (gAccCalibrate) {
            printf("calculating accelerameter!\r\n");
            gV[0] = 0.0;
            gV[1] = 0.0;
            gV[2] = 0.0;
            gR[0] = 0.0;
            gR[1] = 0.0;
            gR[2] = 0.0;
            mpu6050_calibrate();
            printf("xn_bias = %2f\t yn_bias = %2f\t zn_bias = %2f\r\n", gCube.xn_acc_bias, gCube.yn_acc_bias, gCube.zn_acc_bias);
            gAccCalibrate = FALSE;
        }

        LED_2 = LED_OFF;
    }
}

void taskb() {
    uint8 cmd;
    float tmp;
    cmd_init(&gU1RxQ, usart1_send_bytes);

    cube_enable_dir(&gXDir);
    cube_enable_dir(&gYDir);
    cube_enable_dir(&gZDir);

    while (1) {
        while (is_queue_empty(&gU1RxQ)) {
            xtos_schedule();
        }
        dequeue(&gU1RxQ, &cmd);
        switch (cmd) {
        case 'A':
            tmp = gYawCmd + 10;
            gYawCmd = (tmp > 100) ? 100 : tmp;
            break;
        case 'Q':
            tmp = gYawCmd - 10;
            gYawCmd = (tmp < -100) ? -100 : tmp;
            break;
        case 'F':
            cube_set_forward_ctrl(&gXDir, 0.1);
            cube_set_forward_ctrl(&gYDir, 0.0);
            break;
        case 'B':
            cube_set_forward_ctrl(&gXDir, -0.1);
            cube_set_forward_ctrl(&gYDir, 0.0);
            break;
        case 'L':
            cube_set_forward_ctrl(&gXDir, 0.0);
            cube_set_forward_ctrl(&gYDir, 0.1);
            break;
        case 'R':
            cube_set_forward_ctrl(&gXDir, 0.0);
            cube_set_forward_ctrl(&gYDir, -0.1);
            break;
        case 'O':
            cube_set_dir_ctrl(&gZDir, 0.0);
            break;
        case 'P':
            cube_set_dir_ctrl(&gXDir, 0.0);
            cube_set_dir_ctrl(&gYDir, 0.0);
            break;
        case 'S':
            if (gDirCtrl) {
                printf("stop control!\r\n");
                gDirCtrl = FALSE;
            } else {
                printf("start control!\r\n");
                gDirCtrl = TRUE;
            }
            break;
        case 'E':
            if (gXCtrl) {
                printf("stop X control!\r\n");
                gXCtrl = FALSE;
            } else {
                printf("start X control!\r\n");
                gXCtrl = TRUE;
            }
            break;
        case 'T':
            if (gYCtrl) {
                printf("stop Y control!\r\n");
                gYCtrl = FALSE;
            } else {
                printf("start Y control!\r\n");
                gYCtrl = TRUE;
            }
            break;
        case 'U':
            printf("r = %2f\t p = %2f\t y = %2f\r\n", gCube.roll, gCube.pitch, gCube.yaw);
            printf("xv = %2f\t yv = %2f\t zv = %2f\r\n", gV[0], gV[1], gV[2]);
            //printf("xb = %2f\t yb = %2f\t zb = %2f\r\n", gAccBG[0], gAccBG[1], gAccBG[2]);
            
            break;
        case 'W':
            gAccCalibrate = TRUE;
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

    timer_init(10, 5000);
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


