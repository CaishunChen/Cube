#include <imu.h>

#define KP  2.0
#define KI  0.005
static double exInt = 0;
static double eyInt = 0;
static double ezInt = 0;

void imu_filter_gyro(const struct imu_6x_double *ag, double *q, double hdt) {
    double ax = ag->ax;
    double ay = ag->ay;
    double az = ag->az;
    double gx = ag->gx;
    double gy = ag->gy;
    double gz = ag->gz;

    double norm = 1.0 / sqrt(ax*ax + ay*ay + az*az);
    ax *= norm;
    ay *= norm;
    az *= norm;

    double vx = 2 * (q[1]*q[3] - q[0]*q[2]);
    double vy = 2 * (q[0]*q[1] + q[2]*q[3]);
    double vz = q[0]*q[0] - q[1]*q[1] - q[2]*q[2] + q[3]*q[3];

    double ex = ay * vz - az * vy;
    double ey = az * vx - ax * vz;
    double ez = ax * vy - ay * vx;

    exInt += ex * KI;
    eyInt += ey * KI;
    ezInt += ez * KI;

    gx += ex * KP + exInt;
    gy += ey * KP + eyInt;
    gz += ez * KP + ezInt;

    q[0] += (-q[1]*gx - q[2]*gy - q[3]*gz)*hdt;
    q[1] += (q[0]*gx + q[2]*gz - q[3]*gy)*hdt;
    q[2] += (q[0]*gy - q[1]*gz + q[3]*gx)*hdt;
    q[3] += (q[0]*gz + q[1]*gy - q[2]*gx)*hdt;

    norm = 1 / sqrt(q[0]*q[0] + q[1]*q[1] + q[2]*q[2] + q[3]*q[3]);
    q[0] *= norm;
    q[1] *= norm;
    q[2] *= norm;
    q[3] *= norm;
}

/*
void imu_get_euler_angle(const struct quaternion *quat, struct eular_angle *rpy) {
    rpy->roll = atan2(2 * (q[0]*q[1] + q[2]*q[3]), 1 - 2 * (q[1]*q[1] + q[2]*q[2])) * 180 / 3.1415926;
    rpy->pitch = asin(2 * (q[1]*q[3] - q[0]*q[2])) * 180 / 3.1415926;
    rpy->yaw = atan2(2 * (q[1]*q[2] + q[0]*q[3]), q[0]*q[0] + q[1]*q[1] - q[2]*q[2] - q[3]*q[3]) * 180 / 3.1415926;
}

*/
