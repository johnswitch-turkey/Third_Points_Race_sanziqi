 #include "bsp_uart.h"


// /*DMA接收缓存区*/
 uint8_t rx1_buffer[DMA_BUFFER] = {0};
 uint8_t rx2_buffer[DMA_BUFFER] = {0};



//UART_RxHandler tjc_rx_handler = {RX_WAIT_HEADER, {0}, 0};
UART_RxHandler smp_rx_handler = {RX_WAIT_HEADER, {0}, 0};



/*  
 * @brief   串口2发送数据(步进用)
*/
void usart2_SendCmd(uint8_t *cmd,uint8_t len)
{
    volatile uint8_t i = 0;
    for(i = 0; i < len; i++)
    {
    HAL_UART_Transmit(&huart2,&cmd[i],1,HAL_MAX_DELAY);
    }
}
/*
@bief:串口1调试printf重定向
*/
int fputc(int ch, FILE *f){
	uint8_t temp[1] = {ch};
	HAL_UART_Transmit(&huart1,temp,1,2);
	return ch; 
}


