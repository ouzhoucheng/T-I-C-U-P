#include "data.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

const float gx_to_delta_angle = PIT_MS * 2.0 / 32768; //TimePeriod(ms)*GyrFullScale(°/ms)/DataFullScale
const float gz_to_delta_angle = PIT_MS * 2.0 / 32768; //TimePeriod(ms)*GyrFullScale(°/ms)/DataFullScale

/*******************************************************************************
 * Variables
 ******************************************************************************/

//struct indata_STRUCT indata;

/*******************************************************************************
 * Code
 ******************************************************************************/

Kalman_t KalmanX = {
        .Q_angle = 0.001f,
        .Q_bias = 0.003f,
        .R_measure = 0.03f
};
Kalman_t KalmanY = {
        .Q_angle = 0.001f,
        .Q_bias = 0.003f,
        .R_measure = 0.03f,
};
//卡尔曼参数
float Q_angle = 0.001;
float Q_gyro  = 0.003;
float R_angle = 0.5;
float dt      = 0.01;//dt为kalman滤波器采样时间
char  C_0     = 1;
float Q_bias, Angle_err;
float PCt_0, PCt_1, E;
float K_0, K_1, t_0, t_1;
float Pdot[4] ={0,0,0,0};
float PP[2][2] = { { 1, 0 },{ 0, 1 } };
float Angle_X_Final,Angle_Y_Final;


void data_input()
{
    // get_speed();
    if(T>1000){
        get_angle();
    }
    get_speed();
    // get_cam_fps();
    // Get_BattVolt();
    if (IsCaminfo)
    {
        indata.cam_dir = caminfo.dir;
        indata.cam_ddir = caminfo.ddir;
    }
}

uint16_t Get_ADC1_Value(uint8_t chan)
{
    uint16_t adcval = 0;
    ADC1->HC[0] = chan;
    while (!(ADC1->HS & (1 << 0)))
        ;
    adcval = ADC1->R[0];
    return adcval;
}

uint16_t Get_ADC2_Value(uint8_t chan)
{
    uint16_t adcval = 0;
    ADC2->HC[0] = chan;
    while (!(ADC2->HS & (1 << 0)))
        ;
    adcval = ADC2->R[0];
    return adcval;
}

void Get_BattVolt()
{
    float fBattVolt = 0;
    fBattVolt = (float)adc_convert(ADC_1, ADC1_CH5_B16) / 3350.0 * 8.1 * 16.5;
    indata.battvolt = (int)(100 * fBattVolt);
    // if (indata.battvolt < 700) //2-27 600->700 约7v警告
    // {
    //     if (mycar.status == 1)
    //     {
    //         gpio_set(C10, 1); //电压不足警告
    //     }
    //     else //停车时不报警
    //     {
    //         gpio_set(C10, 0);
    //     }
    // }
}

void get_speed()
{
    //mini系数计算：
    //一米走11圈
    indata.speedL = ENC_GetPositionValue(ENC1);
    indata.speedL = (-1) * indata.speedL * 625 / 176;
    ENC_SetInitialPositionValue(ENC1, 0);
    ENC_DoSoftwareLoadInitialPositionValue(ENC1);

    indata.speedR = ENC_GetPositionValue(ENC2);
    indata.speedR = indata.speedR * 625 / 176;
    ENC_SetInitialPositionValue(ENC2, 0);
    ENC_DoSoftwareLoadInitialPositionValue(ENC2);

    indata.speed = (indata.speedL + indata.speedR) / 2;
}

void get_angle()
{
    //获取数据-21O

    struct
    {
        uint8 reg;
        uint8 dat[6];
    } bufG;
    bufG.reg = ICM20602_GYRO_XOUT_H | ICM20602_SPI_R;

    icm_spi_r_reg_bytes(&bufG.reg, 7);
    indata.imu_gyr_x = (int16)(((uint16)bufG.dat[0] << 8 | bufG.dat[1]));
    indata.imu_gyr_y = (int16)(((uint16)bufG.dat[2] << 8 | bufG.dat[3]));
    indata.imu_gyr_z = (int16)(((uint16)bufG.dat[4] << 8 | bufG.dat[5]));

    struct
    {
        uint8 reg;
        uint8 dat[6];
    } bufA;
    bufA.reg = ICM20602_ACCEL_XOUT_H | ICM20602_SPI_R;

    icm_spi_r_reg_bytes(&bufA.reg, 7);
    indata.imu_acc_x = (int16)(((uint16)bufA.dat[0] << 8 | bufA.dat[1]));
    indata.imu_acc_y = (int16)(((uint16)bufA.dat[2] << 8 | bufA.dat[3]));
    indata.imu_acc_z = (int16)(((uint16)bufA.dat[4] << 8 | bufA.dat[5]));
    indata.ax = indata.imu_acc_x;
    indata.ay = indata.imu_acc_y;
    indata.az = indata.imu_acc_z;
////////////////////////陀螺仪矫正相关////////////////////////////////////
    static int16_t gx_h[10] = {0};
    static int16_t gy_h[10] = {0};
    static int16_t gz_h[10] = {0};

    for (uint8_t i = 9; i > 0; i--)
        gx_h[i] = gx_h[i - 1];
    for (uint8_t i = 9; i > 0; i--)
        gy_h[i] = gy_h[i - 1];
    for (uint8_t i = 9; i > 0; i--)
        gz_h[i] = gz_h[i - 1];

    /*偏移校正*/
    gx_h[0] = indata.imu_gyr_x - indata.gyr_bias_x;
    gy_h[0] = indata.imu_gyr_y - indata.gyr_bias_y;
    gz_h[0] = indata.imu_gyr_z - indata.gyr_bias_z;

    indata.gx = (gx_h[0] + gx_h[1] + gx_h[2] + gx_h[3] + gx_h[4] + gx_h[5] + gx_h[6] + gx_h[7] + gx_h[8] + gx_h[9]) / 10;
    indata.gy = (gy_h[0] + gy_h[1] + gy_h[2] + gy_h[3] + gy_h[4] + gy_h[5] + gy_h[6] + gy_h[7] + gy_h[8] + gy_h[9]) / 10;
    indata.gz = (gz_h[0] + gz_h[1] + gz_h[2] + gz_h[3] + gz_h[4] + gz_h[5] + gz_h[6] + gz_h[7] + gz_h[8] + gz_h[9]) / 10;

    //陀螺仪自动校准
    if (T > 1000 && T < 1500)
    {
        static int times = 1;
        static int gx_adj = 0;
        static int gy_adj = 0;
        static int gz_adj = 0;
        times++;
        gx_adj += indata.gx;
        gy_adj += indata.gy;
        gz_adj += indata.gz;
        if (times == (500 / PIT_MS))
        {
            indata.gyr_bias_x = gx_adj / times;
            indata.gyr_bias_y = gy_adj / times;
            indata.gyr_bias_z = gz_adj / times;
            times = 0;
        }
    }
////////////////////////陀螺仪矫正相关////////////////////////////////////


    // if (caminfo.loop_flag)
    // {
    //     setpara.camcfg.loop_flag = caminfo.loop_flag;
    //     //loop_angle(indata.caminfo.loop_flag);      //圆环中对gz积分
    //     //beep();
    // // }
    // if(watch.zebra_flag && flag.CircCount == 3)    // 斑马线积分角
    // {
    //     indata.YawAngle += gz_to_delta_angle*indata.gz;
    //     if((indata.YawAngle > 75 || indata.YawAngle < -75)&&mycar.status == 1)
    //     {
    //         // flag.stop = 1;   // 刹车
    //         mycar.status =2;
    //     }
    // }
    // if(watch.turn90)
    // {
    //     indata.YawAngle += gz_to_delta_angle*indata.gz;
        
    //     if(indata.YawAngle > 88 && setpara.TextMode%2)
    //     {
    //         watch.turn90 = 0;
    //         watch.cross_flag = 0;
    //     }
    //     else if(indata.YawAngle < -88 && !(setpara.TextMode%2))
    //     {
    //         watch.turn90 = 0;
    //         watch.cross_flag = 0;
    //     }
    // }
}

void Kalman_Filter_X(float Accel,float Gyro) //卡尔曼函数
{
	Angle_X_Final += (Gyro - Q_bias) * dt; //先验估计

	Pdot[0]=Q_angle - PP[0][1] - PP[1][0]; // Pk-先验估计误差协方差的微分

	Pdot[1]= -PP[1][1];
	Pdot[2]= -PP[1][1];
	Pdot[3]= Q_gyro;

	PP[0][0] += Pdot[0] * dt;   // Pk-先验估计误差协方差微分的积分
	PP[0][1] += Pdot[1] * dt;   // =先验估计误差协方差
	PP[1][0] += Pdot[2] * dt;
	PP[1][1] += Pdot[3] * dt;

	Angle_err = Accel - Angle_X_Final;	//zk-先验估计

	PCt_0 = C_0 * PP[0][0];
	PCt_1 = C_0 * PP[1][0];

	E = R_angle + C_0 * PCt_0;

	K_0 = PCt_0 / E;
	K_1 = PCt_1 / E;

	t_0 = PCt_0;
	t_1 = C_0 * PP[0][1];

	PP[0][0] -= K_0 * t_0;		 //后验估计误差协方差
	PP[0][1] -= K_0 * t_1;
	PP[1][0] -= K_1 * t_0;
	PP[1][1] -= K_1 * t_1;

	Angle_X_Final += K_0 * Angle_err;	 //后验估计
	Q_bias        += K_1 * Angle_err;	 //后验估计
	indata.gx         = Gyro - Q_bias;	 //输出值（后验估计）的微分 = 角速度
}

void Kalman_Filter_Y(float Accel,float Gyro) //卡尔曼函数
{
	Angle_Y_Final += (Gyro - Q_bias) * dt; //先验估计

	Pdot[0]=Q_angle - PP[0][1] - PP[1][0]; // Pk-先验估计误差协方差的微分

	Pdot[1]=- PP[1][1];
	Pdot[2]=- PP[1][1];
	Pdot[3]=Q_gyro;

	PP[0][0] += Pdot[0] * dt;   // Pk-先验估计误差协方差微分的积分
	PP[0][1] += Pdot[1] * dt;   // =先验估计误差协方差
	PP[1][0] += Pdot[2] * dt;
	PP[1][1] += Pdot[3] * dt;

	Angle_err = Accel - Angle_Y_Final;	//zk-先验估计

	PCt_0 = C_0 * PP[0][0];
	PCt_1 = C_0 * PP[1][0];

	E = R_angle + C_0 * PCt_0;

	K_0 = PCt_0 / E;
	K_1 = PCt_1 / E;

	t_0 = PCt_0;
	t_1 = C_0 * PP[0][1];

	PP[0][0] -= K_0 * t_0;		 //后验估计误差协方差
	PP[0][1] -= K_0 * t_1;
	PP[1][0] -= K_1 * t_0;
	PP[1][1] -= K_1 * t_1;

	Angle_Y_Final	+= K_0 * Angle_err;	 //后验估计
	Q_bias	+= K_1 * Angle_err;	 //后验估计
	indata.gy   = Gyro - Q_bias;	 //输出值（后验估计）的微分 = 角速度
}
