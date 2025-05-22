///**
// * @file uart.c
// * @brief 串口通信模块，用于接收树莓派发送的棋子ID和棋盘ID
// * @details 使用DMA方式接收数据，自动解析接收到的ASCII码为数字
// */

//#include "uart.h"
//#include "stm32f4xx_hal.h"

//// 全局变量定义
//uint8_t motor_flag = 0; // 机械臂控制标志位，当接收到新数据时置1
//uint8_t rotate_flag = 0; // 旋转标志位

//// 串口接收相关定义
//#define RX_BUFFER_SIZE 10  // 定义接收缓冲区大小
//uint8_t rx_buffer[RX_BUFFER_SIZE];  // DMA接收缓冲区
//uint8_t piece_id = 0;    // 存储解析后的棋子ID
//uint8_t board_id = 0;    // 存储解析后的棋盘ID

//// 串口和DMA句柄定义
//UART_HandleTypeDef huart1;  // 串口1句柄
//DMA_HandleTypeDef hdma_usart1_rx;  // 串口1接收DMA句柄

//// // 初始化串口和DMA
//// void UART_Init(void)
//// {
////     // 配置串口参数
////     huart1.Instance = USART1;
////     huart1.Init.BaudRate = 115200;
////     huart1.Init.WordLength = UART_WORDLENGTH_8B;
////     huart1.Init.StopBits = UART_STOPBITS_1;
////     huart1.Init.Parity = UART_PARITY_NONE;
////     huart1.Init.Mode = UART_MODE_TX_RX;
////     huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
////     huart1.Init.OverSampling = UART_OVERSAMPLING_16;
//    
////     if (HAL_UART_Init(&huart1) != HAL_OK)
////     {
////         Error_Handler();
////     }
//    
////     // 启动DMA接收
////     HAL_UART_Receive_DMA(&huart1, rx_buffer, RX_BUFFER_SIZE);
//// }

//// 串口接收完成回调函数
//// 当DMA接收完成时会自动调用此函数
//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
//{
//    // 判断是否是串口1接收完成
//    if(huart->Instance == USART1)
//    {
//        // 解析第一个字节为棋子ID（ASCII码转数字）
//        if(rx_buffer[0] >= '0' && rx_buffer[0] <= '9')
//        {
//            piece_id = rx_buffer[0] - '0';
//        }
//        // 解析第二个字节为棋盘ID（ASCII码转数字）
//        if(rx_buffer[1] >= '0' && rx_buffer[1] <= '9')
//        {
//            board_id = rx_buffer[1] - '0';
//        }
//        
//        // 设置机械臂控制标志位，表示接收到新数据
//        motor_flag = 1;
//        
//        // 重新启动DMA接收，准备接收下一组数据
//        HAL_UART_Receive_DMA(&huart1, rx_buffer, RX_BUFFER_SIZE);
//    }
//}

//// 获取当前棋子ID
//// 返回值：解析后的棋子ID（0-9）
//uint8_t Get_Piece_ID(void)
//{
//    return piece_id;
//}

//// 获取当前棋盘ID
//// 返回值：解析后的棋盘ID（0-9）
//uint8_t Get_Board_ID(void)
//{
//    return board_id;
//}


