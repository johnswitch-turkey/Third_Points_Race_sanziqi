#include "control.h"
#include "Control_logic.h"
#include "chess_task.h"

typedef struct {
   uint32_t x;      // X��Ŀ��λ��
   uint32_t y;      // Y��Ŀ��λ��
} Position;

//��������
Position board_position[9]=
{
    // ��0�У�ʵ�ʿ����н����0��ʼ������
    {5200, 16600}, {5200, 13650}, {5200, 10500},  // ����λ��1 , 2 , 3 -200 -550
    // ��1��
    {8300, 16500}, {8250, 13650}, {8100, 10500},  // ����λ��4 , 5 , 6
    // ��2��
    {11150, 16500}, {11150, 13650}, {11150, 10500}   // ����λ��7 , 8 , 9
};
//������ת45������
Position board_rotate_position[9]=
{

                     {4000, 13900},//1 
            {6100, 15800},       {5900, 11700}, //2 3
   {8300, 17800},   {8100, 13700},       {7900, 9600},  // 4 , 5 , 6
            {10300,15600},       {10000,11500},//7 8
                     {12100, 13400}   //9
};

//��������
Position chess_position[10]=
{
   {1850, 23400},   // ����1�ĳ�ʼλ��
   {4850, 23550},   // ����2
   {7850, 23700},   // ����3
   {10900, 23800},   // ����4
   {13950, 23800},   // ����5
   {2500, 3650},   // ����6 x���� y����CW����
   {5500, 3650},  // ����7
   {8500, 3650},  // ����8
   {11500,3750},  // ����9
   {14500,3850}   // ����10
};




/*
 *@brief:��������
 *param: ���Ӻţ����̺�
*/
void Place_Chess(uint8_t Chess_ID,uint8_t Board_ID)
{
    // ����������Ч��
    if (Chess_ID == 0 || Chess_ID > 10 || Board_ID == 0 || Board_ID > 10) {
      return;
  }
   // �������ӵ���ʼλ������������Chess_ID��1��ʼ��
   uint8_t chessIndex = Chess_ID - 1;
   uint8_t boardIndex = Board_ID - 1;

   Position start = {chess_position[chessIndex].x, chess_position[chessIndex].y};   //����λ��
   Position end = {board_position[boardIndex].x, board_position[boardIndex].y};     //����λ��
   
   if(rotate_flag == 1)  //������ת�������λ��
   {
      end.x = board_rotate_position[boardIndex].x;
      end.y = board_rotate_position[boardIndex].y;
   }

   // 1. �ƶ���е�۵����Ӵ��λ��
   control_t(start.x, start.y);
   Delay_Timer(180);
   while(!checkDelayTimer());

   
   // 2. ִ��ץȡ��������ʵ�ּ�צ���ƣ�
   Servo_Down();
   Magnet_On();
   Delay_Timer(100);
   while(!checkDelayTimer());
   Servo_Up();
   Delay_Timer(100);
   while(!checkDelayTimer());
   
   // 3. �ƶ���е�۵�Ŀ������λ��
   control_t(end.x,end.y);
   Delay_Timer(150);
   while(!checkDelayTimer());

   // 4. ִ�з��ö���
   Servo_Down();
   Delay_Timer(150);
   while(!checkDelayTimer());
   Magnet_Off();
   Servo_Up();
   
   // 5. ���ػ���λ��
   control_to_zero();
   Delay_Timer(100);
   while(!checkDelayTimer());
//   LED_Task_On();

}






void control_t(uint32_t x_distance,uint32_t y_distance){















}

