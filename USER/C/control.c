#include "control.h"
#include "Control_logic.h"
#include "chess_task.h"

typedef struct {
   uint32_t x;      // X轴目标位置
   uint32_t y;      // Y轴目标位置
} Position;

//棋盘坐标
Position board_position[9]=
{
    // 第0行（实际开发中建议从0开始计数）
    {5200, 16600}, {5200, 13650}, {5200, 10500},  // 棋盘位置1 , 2 , 3 -200 -550
    // 第1行
    {8300, 16500}, {8250, 13650}, {8100, 10500},  // 棋盘位置4 , 5 , 6
    // 第2行
    {11150, 16500}, {11150, 13650}, {11150, 10500}   // 棋盘位置7 , 8 , 9
};
//棋盘旋转45度坐标
Position board_rotate_position[9]=
{

                     {4000, 13900},//1 
            {6100, 15800},       {5900, 11700}, //2 3
   {8300, 17800},   {8100, 13700},       {7900, 9600},  // 4 , 5 , 6
            {10300,15600},       {10000,11500},//7 8
                     {12100, 13400}   //9
};

//棋子坐标
Position chess_position[10]=
{
   {1850, 23400},   // 棋子1的初始位置
   {4850, 23550},   // 棋子2
   {7850, 23700},   // 棋子3
   {10900, 23800},   // 棋子4
   {13950, 23800},   // 棋子5
   {2500, 3650},   // 棋子6 x方向 y方向CW方向
   {5500, 3650},  // 棋子7
   {8500, 3650},  // 棋子8
   {11500,3750},  // 棋子9
   {14500,3850}   // 棋子10
};




/*
 *@brief:放置棋子
 *param: 棋子号，祺盘号
*/
void Place_Chess(uint8_t Chess_ID,uint8_t Board_ID)
{
    // 检查参数的有效性
    if (Chess_ID == 0 || Chess_ID > 10 || Board_ID == 0 || Board_ID > 10) {
      return;
  }
   // 计算棋子的起始位置索引（假设Chess_ID从1开始）
   uint8_t chessIndex = Chess_ID - 1;
   uint8_t boardIndex = Board_ID - 1;

   Position start = {chess_position[chessIndex].x, chess_position[chessIndex].y};   //棋子位置
   Position end = {board_position[boardIndex].x, board_position[boardIndex].y};     //棋盘位置
   
   if(rotate_flag == 1)  //换成旋转后的棋盘位置
   {
      end.x = board_rotate_position[boardIndex].x;
      end.y = board_rotate_position[boardIndex].y;
   }

   // 1. 移动机械臂到棋子存放位置
   control_t(start.x, start.y);
   Delay_Timer(180);
   while(!checkDelayTimer());

   
   // 2. 执行抓取动作（需实现夹爪控制）
   Servo_Down();
   Magnet_On();
   Delay_Timer(100);
   while(!checkDelayTimer());
   Servo_Up();
   Delay_Timer(100);
   while(!checkDelayTimer());
   
   // 3. 移动机械臂到目标棋盘位置
   control_t(end.x,end.y);
   Delay_Timer(150);
   while(!checkDelayTimer());

   // 4. 执行放置动作
   Servo_Down();
   Delay_Timer(150);
   while(!checkDelayTimer());
   Magnet_Off();
   Servo_Up();
   
   // 5. 返回回零位置
   control_to_zero();
   Delay_Timer(100);
   while(!checkDelayTimer());
//   LED_Task_On();

}






void control_t(uint32_t x_distance,uint32_t y_distance){















}

