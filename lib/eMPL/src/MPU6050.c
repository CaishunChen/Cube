#include <mpu6050.h>
#include <stm32f411.h>
#include <stm32f4xx_rcc.h>
#include <gVariables.h>

#include <inv_mpu.h>
#include <inv_mpu_dmp_motion_driver.h>
#include <stdio.h>
#include <math.h>

//定义不同测量范围的刻度因子
#define Gyro_250_Scale_Factor   131.0f
#define Gyro_500_Scale_Factor   65.5f
#define Gyro_1000_Scale_Factor  32.8f
#define Gyro_2000_Scale_Factor  16.4f
#define Accel_2_Scale_Factor    16384.0f
#define Accel_4_Scale_Factor    8192.0f
#define Accel_8_Scale_Factor    4096.0f
#define Accel_16_Scale_Factor   2048.0f
#define DEFAULT_MPU_HZ 200

static signed char gyro_orientation[9] = { 1, 0, 0,
                                           0, 1, 0,
                                           0, 0, 1};//roll和pitch都是反的，yaw是正的

//float magOffset[3] = { (MAG0MAX + MAG0MIN) / 2, (MAG1MAX + MAG1MIN) / 2, (MAG2MAX + MAG2MIN) / 2 };
static  unsigned short inv_row_2_scale(const signed char *row)
{
    unsigned short b;

    if (row[0] > 0)
        b = 0;
    else if (row[0] < 0)
        b = 4;
    else if (row[1] > 0)
        b = 1;
    else if (row[1] < 0)
        b = 5;
    else if (row[2] > 0)
        b = 2;
    else if (row[2] < 0)
        b = 6;
    else
        b = 7;      // error
    return b;
}

static  unsigned short inv_orientation_matrix_to_scalar(const signed char *mtx) {
    unsigned short scalar;

    /*
       XYZ  010_001_000 Identity Matrix
       XZY  001_010_000
       YXZ  010_000_001
       YZX  000_010_001
       ZXY  001_000_010
       ZYX  000_001_010
     */

    scalar = inv_row_2_scale(mtx);
    scalar |= inv_row_2_scale(mtx + 3) << 3;
    scalar |= inv_row_2_scale(mtx + 6) << 6;


    return scalar;
}

static bool run_self_test(void) {
    int result;
    long gyro[3], accel[3];

    result = mpu_run_self_test(gyro, accel);
    if (result == 0x3) {
        /* Test passed. We can trust the gyro data here, so let's push it down
        * to the DMP.
        */
        float sens;
        unsigned short accel_sens;
        mpu_get_gyro_sens(&sens);
        gyro[0] = (long)(gyro[0] * sens);
        gyro[1] = (long)(gyro[1] * sens);
        gyro[2] = (long)(gyro[2] * sens);
        dmp_set_gyro_bias(gyro);
        mpu_get_accel_sens(&accel_sens);
        accel[0] *= accel_sens;
        accel[1] *= accel_sens;
        accel[2] *= accel_sens;
        dmp_set_accel_bias(accel);
        return TRUE;
    } else {
        return FALSE;
    }

}

uint8 mpu6050_read_uint8(struct mpu6050 *mpu, uint8 reg) {
    uint8 data;
    I2C_ReceiveDatas(mpu->i2c, &data, 1, mpu->addr, reg);
    return data;
}

/*********************************************************************/
#define q30  1073741824.0f
#define RAD_TO_DEG 57.295779513082320876798154814105  // 弧度转角度的转换率
#define DEG_TO_RAD 0.01745329251994329576923690768489 // 角度转弧度的转换率

#define GYRO_FILTER_N   7
#define ACC_FILTER_N    10

static float roll_rate_history[GYRO_FILTER_N] = { 0, 0, 0, 0, 0 };
static float pitch_rate_history[GYRO_FILTER_N] = { 0, 0, 0, 0, 0 };
static float yaw_rate_history[GYRO_FILTER_N] = { 0, 0, 0, 0, 0 };
static float bx_acc_history[ACC_FILTER_N] = { 0,0,0,0, 0,0,0,0 };
static float by_acc_history[ACC_FILTER_N] = { 0,0,0,0, 0,0,0,0 };
static float bz_acc_history[ACC_FILTER_N] = { 0,0,0,0, 0,0,0,0 };
/*
* smooth_filter - 平滑滤波
*
* @his: 历史数据缓存
* @value: 采样新值
* @n: 滤波宽度
*/
static float smooth_filter(float *his, double value, uint8 n) {
    float sum = 0, max = 0, min = 0;

    for (uint8 i = (n - 1); i > 0; i--) {
        his[i] = his[i - 1];
        sum += his[i];
        max = (his[i] > max) ? his[i] : max;
        min = (his[i] < min) ? his[i] : min;
    }
    his[0] = value;
    max = (his[0] > max) ? his[0] : max;
    min = (his[0] < min) ? his[0] : min;
    sum += (value - max - min);
    return sum / (float)(n - 2);
}

/*
 * MPU6050_Init- 初始化MPU6050
 */
uint8 mpu6050_init(void) {
    Init_I2C(&gI2C);

    while (mpu_init());

    if (mpu_set_sensors(INV_XYZ_GYRO | INV_XYZ_ACCEL))
        return 1;

    if (mpu_configure_fifo(INV_XYZ_GYRO | INV_XYZ_ACCEL))
        return 2;

    if (mpu_set_sample_rate(DEFAULT_MPU_HZ))
        return 3;

    if (dmp_load_motion_driver_firmware())
        return 4;

    if (dmp_set_orientation(inv_orientation_matrix_to_scalar(gyro_orientation)))
        return 5;

    if (dmp_enable_feature(DMP_FEATURE_6X_LP_QUAT | DMP_FEATURE_TAP | DMP_FEATURE_ANDROID_ORIENT | DMP_FEATURE_SEND_RAW_ACCEL | DMP_FEATURE_SEND_CAL_GYRO | DMP_FEATURE_GYRO_CAL))		   	 					 //dmp_enable_feature
        return 6;

    if (dmp_set_fifo_rate(DEFAULT_MPU_HZ))   	 			 //dmp_set_fifo_rate
        return 7;

    run_self_test();
    //if (!run_self_test())
    //    return 8;

	if(mpu_set_dmp_state(1))
		return 9;

    gImuValue.vx = 0;
    gImuValue.vy = 0;
    gImuValue.vz = 0;
    gImuValue.rx = 0;
    gImuValue.ry = 0;
    gImuValue.rz = 0;
    return 0;
}
/*
 * MPU6050_get_value - 读取传感器数值
 */
int MPU6050_get_value(struct mpu6050_measure_value *v) {
    short sensors;
    unsigned char more;
    unsigned long sensor_timestamp;
    return dmp_read_fifo(v->gyro, v->accel, v->quat, &sensor_timestamp, &sensors, &more);
}
/*
 * MPU6050_cal_quat - 换算四元数
 */
void MPU6050_cal_quat(struct mpu6050_measure_value *v) {
    v->q[0] = (float)gImuValue.quat[0] / q30;
    v->q[1] = (float)gImuValue.quat[1] / q30;
    v->q[2] = (float)gImuValue.quat[2] / q30;
    v->q[3] = (float)gImuValue.quat[3] / q30;
}
/*
* MPU6050_cal_rotation_matrix - 计算旋转矩阵
*/
void MPU6050_cal_rotation_matrix(struct mpu6050_measure_value *v) {
    float *q = v->q;
    float *m = v->m;

    double q1q1 = q[1] * q[1];
    double q2q2 = q[2] * q[2];
    double q3q3 = q[3] * q[3];

    double q0q3 = q[0] * q[3];
    double q0q2 = q[0] * q[2];
    double q0q1 = q[0] * q[1];
    double q1q2 = q[1] * q[2];
    double q1q3 = q[1] * q[3];
    double q2q3 = q[2] * q[3];

    m[0 * 3 + 0] = 1 - 2 * (q2q2 + q3q3);
    m[0 * 3 + 1] = 2 * (q1q2 - q0q3);
    m[0 * 3 + 2] = 2 * (q1q3 + q0q2);
    m[1 * 3 + 0] = 2 * (q1q2 + q0q3);
    m[1 * 3 + 1] = 1 - 2 * (q1q1 + q3q3);
    m[1 * 3 + 2] = 2 * (q2q3 - q0q1);
    m[2 * 3 + 0] = 2 * (q1q3 - q0q2);
    m[2 * 3 + 1] = 2 * (q2q3 + q0q1);
    m[2 * 3 + 2] = 1 - 2 * (q1q1 + q2q2);
}
/*
 * MPU6050_cal_rpy - 计算姿态角
 */
void MPU6050_cal_rpy(struct mpu6050_measure_value *v) {
    float *m = v->m;

    float tmp = atan2(m[2 * 3 + 1], m[2 * 3 + 2])* RAD_TO_DEG;	// roll
    if (fabs(tmp) <= 90)
        v->roll = tmp;

    tmp = asin(-m[2 * 3 + 0]) * RAD_TO_DEG;
    if (fabs(tmp) <= 90)
        v->pitch = tmp;

    tmp = atan2(m[1 * 3 + 0], m[0 * 3 + 0]) * RAD_TO_DEG;	//yaw	,单位：度
    if (fabs(tmp) <= 180)
        v->yaw = tmp;
}
/*
 * MPU6050_cal_rpy_rate - 对陀螺仪数据进行5点平滑滤波作为姿态角速率
 */
void MPU6050_cal_rpy_rate(struct mpu6050_measure_value *v) {
    double roll, pitch, yaw;

    roll = v->gyro[0] / (double)Gyro_2000_Scale_Factor;
    pitch = v->gyro[1] / (double)Gyro_2000_Scale_Factor;
    yaw = v->gyro[2] / (double)Gyro_2000_Scale_Factor;

    float rate = smooth_filter(roll_rate_history, roll, GYRO_FILTER_N);
    if (fabs(rate) <= 1000)
        v->roll_rate = rate;

    rate = smooth_filter(pitch_rate_history, pitch, GYRO_FILTER_N);
    if (fabs(rate) <= 1000)
        v->pitch_rate = rate;

    rate = smooth_filter(yaw_rate_history, yaw, GYRO_FILTER_N);
    if (fabs(rate) <= 1000)
        v->yaw_rate = rate;
}

/*
 * MPU6050_cal_body_acc - 对加速度计数据进行8点平滑滤波机体坐标系下加速度
 */
void MPU6050_cal_body_acc(struct mpu6050_measure_value *v) {
    double x = (double)(v->accel[0]) / Accel_2_Scale_Factor;
    double y = (double)(v->accel[1]) / Accel_2_Scale_Factor;
    double z = (double)(v->accel[2]) / Accel_2_Scale_Factor;

    float acc = smooth_filter(bx_acc_history, x, ACC_FILTER_N);
    if (fabs(acc) <= 1000)
        v->xb_acc = acc;

    acc = smooth_filter(by_acc_history, y, ACC_FILTER_N);
    if (fabs(acc) <= 1000)
        v->yb_acc = acc;

    acc = smooth_filter(bz_acc_history, z, ACC_FILTER_N);
    if (fabs(acc) <= 1000)
        v->zb_acc = acc;
}
/*
* MPU6050_cal_world_acc - 计算世界坐标系下加速度
*/
void MPU6050_cal_world_acc(struct mpu6050_measure_value *v) {
    float *m = v->m;
    float *ab = &(v->xb_acc);
    float *an = &(v->xn_acc);

    an[0] = m[0 * 3 + 0] * ab[0] + m[0 * 3 + 1] * ab[1] + m[0 * 3 + 2] * ab[2];
    an[1] = m[1 * 3 + 0] * ab[0] + m[1 * 3 + 1] * ab[1] + m[1 * 3 + 2] * ab[2];
    an[2] = m[2 * 3 + 0] * ab[0] + m[2 * 3 + 1] * ab[1] + m[2 * 3 + 2] * ab[2];
}

#define CALIBTIMES 100
void mpu6050_calibrate(void) {
    float sum[3] = { 0, 0, 0 };

    for (int i = 0; i < CALIBTIMES; i++) {
        while (!(mpu6050_read_uint8(&gMpu6050, 0x3A) & 0x01));
        while (MPU6050_get_value(&gImuValue));

        MPU6050_cal_quat(&gImuValue);
        MPU6050_cal_rotation_matrix(&gImuValue);
        MPU6050_cal_body_acc(&gImuValue);
        MPU6050_cal_world_acc(&gImuValue);

        sum[0] += gImuValue.xn_acc;
        sum[1] += gImuValue.yn_acc;
        sum[2] += gImuValue.zn_acc;
    }
    gImuValue.xn_acc_bias = sum[0] / CALIBTIMES;
    gImuValue.yn_acc_bias = sum[1] / CALIBTIMES;
    gImuValue.zn_acc_bias = sum[2] / CALIBTIMES;
}


/*
 * MPU6050_Pose - 获取MPU6050的姿态角和角速率
 */
void mpu6050_pose(void) {
    while (MPU6050_get_value(&gImuValue));
    
    MPU6050_cal_quat(&gImuValue);
    MPU6050_cal_rotation_matrix(&gImuValue);
    MPU6050_cal_rpy(&gImuValue);

    MPU6050_cal_rpy_rate(&gImuValue);
    MPU6050_cal_body_acc(&gImuValue);
    MPU6050_cal_world_acc(&gImuValue);
    
}

