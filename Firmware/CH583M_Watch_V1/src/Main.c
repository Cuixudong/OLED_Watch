/********************************** (C) COPYRIGHT *******************************
 * File Name          : Main.c
 * Author             : WCH
 * Version            : V1.0
 * Date               : 2020/08/06
 * Description        : 串口1收发演示
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for 
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/

#include "CH58x_common.h"
#include "app.h"

/*********************************************************************
 * @fn      main
 *
 * @brief   主函数
 *
 * @return  none
 */
int main()
{
    main_init();
    main_loop();
    while(1)
    {}
}

__attribute__((interrupt("WCH-Interrupt-fast")))
__attribute__((section(".highcode")))
void TMR1_IRQHandler(void) // TMR1 定时中断
{
    if(TMR1_GetITFlag(TMR0_3_IT_CYC_END))
    {
        TMR1_ClearITFlag(TMR0_3_IT_CYC_END); // 清除中断标志
        HAL_IncTick();
    }
}

/*********************************************************************
 * @fn      UART1_IRQHandler
 *
 * @brief   UART1中断函数
 *
 * @return  none
 */
__INTERRUPT
__HIGH_CODE
void UART1_IRQHandler(void)
{
    uint16_t error;

    switch(UART1_GetITFlag())
    {
        case UART_II_LINE_STAT: // 线路状态错误
        {
            UART1_GetLinSTA();
            break;
        }
        case UART_II_RECV_RDY: // 数据达到设置触发点
        case UART_II_RECV_TOUT: // 接收超时，暂时一帧数据接收完成
            error = app_drv_fifo_write_from_same_addr(&app_uart_rx_fifo, (uint8_t *)&R8_UART1_RBR, R8_UART1_RFC);
            if(error != APP_DRV_FIFO_RESULT_SUCCESS)
            {
                for(uint8_t i = 0; i < R8_UART1_RFC; i++)
                {
                    //fifo full,put to fifo black hole
                    for_uart_rx_black_hole = R8_UART1_RBR;
                }
            }
            uart_rx_flag = 1;
            break;
        case UART_II_THR_EMPTY: // 发送缓存区空，可继续发送
            break;
        case UART_II_MODEM_CHG: // 只支持串口0
            break;
        default:
            break;
    }
}
