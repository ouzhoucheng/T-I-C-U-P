#ifndef _CAMERA_PROCESS_H_
#define _CAMERA_PROCESS_H_

#include "car_global.h"

#define _GRAY_SCALE 64
#define _SHIFT_FOR_GRAYSCALE 2
#define _Thresh_Mult 2 //1/2*(256/_GRAY_SCALE)

#define LINE_WIDTH 160

#define WIDTH 88 //近点赛道宽度

#define _EDGE_STORE_SIZE 22 //必须为偶数
#define _MIN_EDGE_WIDTH 20  //绝对最小赛道宽度

#define _LEFT_MARGIN 3
#define _RIGHT_MARGIN (LINE_WIDTH - 4)

struct lineinfo_s
{
    unsigned char y;          //0~119
    unsigned char left;       //0~160
    unsigned char right;      //0~160
    unsigned char edge_count; //0~160
    // unsigned char edge_store;   //好像没用0612
    uint16_t persp_lx; //0~255
    uint16_t persp_ly; //0~255
    uint16_t persp_rx; //0~255
    uint16_t persp_ry; //0~255
    int angel_left;
    int angel_right;
    int left_lost;
    int right_lost;
    unsigned char zebra_flag;
    unsigned char apriltag_flag;
    
    unsigned char black_width;
    unsigned char junc_angle;
};

struct watch_o
{
    // uint32_t fullCameraBufferAddr;
    uint8_t brightness;
    int watch_line;
    int watch_lost;
    int loop_flag;
    short int gray;
    int cross;

    //左右丢线数——21区
    int left_lost;
    int right_lost;

    //圆环标志位
    int RLStraight;   //直线标志：左右直线0无1左2右3左右
    int InLoopAngle;  //经过圆环第一个角所在行
    int InLoopCirc;   //经过圆环上凸弧
    int InLoopAngle2; //入环前看见第二个角点
    int OutLoopAngle; //出环后看见第二个角点


    int InLoop; //圆环状态0无环
    //1：准备入左环，补第二段线 2：准备出左环，补线
    //5：准备入右环，补第二段线 6：准备出右环，补线
    int OutLoopRight;  //出环右角点横坐标
    int OutLoopRightY; //出环右角点纵坐标
    int OutLoopLeft;   //出环左角点横坐标
    int OutLoopLeftY;  //出环左角点纵坐标

    //是否进入三岔路标志——21区
    int Junc_flag; //0否1左2右3进入但不知道方向
    int DeltaR;    //暂时用作显示
    int DeltaL;    //暂时用作显示

    int JuncLineL; //三岔路角点左所在行
    int JuncLineR; //三岔路角点右所在行
    int JuncLine;  //取平均
    int JuncAngle;
    int JuncAngleX;  //取平均
    int JuncTime;  //三岔路时间

    
    int zebra_flag;
    int ZebraLine;
    int ZebraRowL;
    int ZebraRowR;

    uint32_t DectectStart;

    int April_flag;
    int AprilLine;
    uint32_t AprilStart;

    uint32_t FruitStart;
    uint32_t LaserStart;
    uint32_t AnimalStart;

    int distance;


    

};

extern uint16_t max_int_time_us;
extern struct watch_o watch;
extern uint8_t imo[120][160];
extern struct caminfo_s caminfo;
extern uint8_t IsCaminfo;
extern uint8_t IsDisplay;
extern struct lineinfo_s lineinfo[120];

int img_otsu(uint8_t *img, uint8_t img_v, uint8_t img_h, uint8_t step);
uint8_t get_orign_edges(uint8_t *inputimg, uint8_t *edge_store);
int get_best_edge(struct lineinfo_s *lineinfo, uint8_t *edge_store, struct lineinfo_s *lineinfo_ref);
int get_max_edge(struct lineinfo_s *lineinfo, uint8_t *edge_store);
int line_single(struct lineinfo_s *lineinfo, unsigned char *inputimg);
int line_findnext(struct lineinfo_s *lineinfo, uint8_t *inputimg, struct lineinfo_s *lineinfo_ref);
int zebra_detect(struct lineinfo_s *lineinfo, uint8_t *edge_store, uint8_t *inputimg);
int apriltag_detect(struct lineinfo_s *lineinfo, uint8_t *edge_store, uint8_t *inputimg);
int Junc_detect(struct lineinfo_s *lineinfo, uint8_t *edge_store, uint8_t *inputimg);

void Caminfo_Cal(void);
int clear_all_flags();

#endif