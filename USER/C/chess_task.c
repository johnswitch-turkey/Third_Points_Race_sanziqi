

#include "chess_task.h"

// ȫ��״̬���������ڼ�¼���ӷ���״̬
volatile uint8_t black_placed = 0;    // ������ñ�־
volatile uint8_t white_placed = 0;    // ������ñ�־
volatile uint8_t total_placed = 0;    // �ܷ�������
volatile uint8_t isTaskRunning;       // ����ִ��״̬��0-���У�1-������


//�����õ�����״̬�ṹ��
volatile Task_State task_state = {
    .last_chess_ID = 0xFF,      // ��һ�β���������ID����ʼֵ0xFF��ʾ��Ч��
    .last_board_ID = 0xFF,      // ��һ�β���������λ��ID����ʼֵ0xFF��ʾ��Ч��
    .total_chess_count = 0,     // ��ǰ�������ѷ��õ���������
    .data_refreshed = 0         // ���ݸ��±�־�����ڱ�������Ƿ�ˢ��
};


// ���������־λ
volatile uint8_t rotate_flag = 0;  // �Ƿ���Ҫ45����ת��־�����ڵ���������


