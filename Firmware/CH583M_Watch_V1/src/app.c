/*
 * app.c
 *
 *  Created on: Oct 11, 2023
 *      Author: w
 */

#include "app.h"


__IO uint32_t uwTick = 0;
__IO uint32_t Timer_Task_Cnt[TASK_NUM] = { 0, 0, 0, 0, 0 };
__IO uint32_t Timer_Task_Tick[TASK_NUM] = { 5, 10, 25, 200, 2 };

__attribute__((section(".highcode")))
void HAL_IncTick(void) {
    int i;
    uwTick += 10;
    for (i = 0; i < TASK_NUM; i++) {
        if (Timer_Task_Cnt[i] > 0) {
            Timer_Task_Cnt[i]--;
        }
    }
    Button_handle();
}

//The tx buffer and rx buffer for app_drv_fifo
//length should be a power of 2
uint8_t app_uart_tx_buffer[APP_UART_TX_BUFFER_LENGTH] = {0};
uint8_t app_uart_rx_buffer[APP_UART_RX_BUFFER_LENGTH] = {0};

app_drv_fifo_t app_uart_tx_fifo;
app_drv_fifo_t app_uart_rx_fifo;

//interupt uart rx flag ,clear at main loop
uint8_t uart_rx_flag = 0;
//for interrupt rx blcak hole ,when uart rx fifo full
uint8_t for_uart_rx_black_hole = 0;

void app_uart_tx_data(uint8_t *data, uint16_t length)
{
    uint16_t write_length = length;
    app_drv_fifo_write(&app_uart_tx_fifo, data, &write_length);
}

void app_uart_process(void)
{
    UINT32 irq_status;
    if(uart_rx_flag)
    {
        uint16_t read_length = 20;
        uint8_t to_test_buffer[120];
        uint8_t buf[120];
        app_drv_fifo_result_t res = app_drv_fifo_read(&app_uart_rx_fifo, to_test_buffer, &read_length);
        if(res == APP_DRV_FIFO_RESULT_SUCCESS)
        {
#if 0
            sprintf(buf,"rev:%5d\n",read_length);
            app_uart_tx_data(buf,10);
            app_uart_tx_data(to_test_buffer,read_length);
#endif
            uint8_t id,op,len;
            uint8_t array[10];
            if(parsePacket(to_test_buffer,read_length,&id,&op,array,&len) == 0)
            {
            }
        }
        uart_rx_flag = 0;
    }
    //tx process
    if(R8_UART1_TFC < UART_FIFO_SIZE)
    {
        app_drv_fifo_read_to_same_addr(&app_uart_tx_fifo, (uint8_t *)&R8_UART1_THR, UART_FIFO_SIZE - R8_UART1_TFC);
    }
    if(0)
    {
        //清空fifo
        app_drv_fifo_flush(&app_uart_rx_fifo);
    }
}

/* CRC 高位字节值表 */
static const uint8_t s_crchi[] = {
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40
};

/* CRC 低位字节值表 */
const uint8_t s_crclo[] = {
    0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06,
    0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD,
    0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
    0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A,
    0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4,
    0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
    0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3,
    0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,
    0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
    0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29,
    0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED,
    0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
    0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,
    0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67,
    0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
    0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,
    0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E,
    0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
    0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71,
    0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92,
    0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
    0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B,
    0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B,
    0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
    0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42,
    0x43, 0x83, 0x41, 0x81, 0x80, 0x40
};

/**
 * @brief       CRC16校验
 * @param       *_pBuf：数据地址
 * @param       _usLen：数据长度
 * @note        采用crc16-modbus，多项式hex：8005
 * @retval      校验结果
 */
uint16_t crc16_calc(uint8_t *_pBuf, uint16_t _usLen)
{
    uint8_t ucCRCHi = 0xFF;                         /* 高CRC字节初始化 */
    uint8_t ucCRCLo = 0xFF;                         /* 低CRC 字节初始化 */
    uint16_t usIndex;                               /* CRC循环中的索引 */

    while (_usLen--)
    {
        usIndex = ucCRCLo ^ *_pBuf++;               /* 计算CRC */
        ucCRCLo = ucCRCHi ^ s_crchi[usIndex];
        ucCRCHi = s_crclo[usIndex];
    }
    return ((uint16_t)ucCRCHi << 8 | ucCRCLo);      /* 返回校验结果，高字节在高位 */
}

void createPacket(uint8_t id, uint8_t op, uint8_t *data, uint8_t dataLength, uint8_t *packet, uint8_t *packetLength)
{
    uint16_t checksum;
    *packetLength = dataLength + 6; // 起始字节、ID、操作码、长度、校验和
    packet[0] = START_BYTE;
    packet[1] = id;
    packet[2] = op;
    packet[3] = dataLength;
    if(dataLength != 0)
    {
        memcpy(packet + 4, data, dataLength);
    }
    checksum = crc16_calc(packet,*packetLength - 2);
    packet[*packetLength - 2] = (checksum >> 8) & 0xFF;
    packet[*packetLength - 1] = checksum & 0xFF;
}

uint8_t parsePacket(uint8_t *packet, uint8_t packetLength, uint8_t *id, uint8_t *op, uint8_t *data, uint8_t *dataLength)
{
    if (packetLength < 6 || packet[0] != START_BYTE)
    {
        app_uart_tx_data("err pack!\r\n", 11);
        return 1; // 无效的数据包
    }
    *id = packet[1];
    *op = packet[2];
    *dataLength = packet[3]; // 减去起始字节、ID、操作码

    //数据长度非0且完整
    if((*dataLength  > 0)&&(packetLength >= ((*dataLength) + 6)))
    {
        memcpy(data, packet + 4, *dataLength);
    }
    else
    {
        app_uart_tx_data("data len err!\r\n", 15);
        return 2; // 数据不完整
    }
    uint16_t checksum;
    checksum = crc16_calc(packet,packetLength - 2);

    if (!(((checksum & 0xFF) == packet[packetLength - 1]) && (((checksum >> 8) & 0xFF) == packet[packetLength - 2])))
    {
        app_uart_tx_data("check sum err!\r\n", 16);
        return 3; // 校验和不匹配
    }
    return 0; // 解析成功
}


uint16_t RoughCalib_Value = 0;
uint16_t adc_buf[2][501];

void system_init(void)
{
    uint8_t i;
    uint16_t adcBuff[20];

    SetSysClock(CLK_SOURCE_PLL_60MHz);

    //tx fifo and tx fifo
    //The buffer length should be a power of 2
    app_drv_fifo_init(&app_uart_tx_fifo, app_uart_tx_buffer, APP_UART_TX_BUFFER_LENGTH);
    app_drv_fifo_init(&app_uart_rx_fifo, app_uart_rx_buffer, APP_UART_RX_BUFFER_LENGTH);

    /* 配置串口1：先配置IO口模式，再配置串口 */
    GPIOA_SetBits(GPIO_Pin_9);
    GPIOA_ModeCfg(GPIO_Pin_8, GPIO_ModeIN_PU);      // RXD-配置上拉输入
    GPIOA_ModeCfg(GPIO_Pin_9, GPIO_ModeOut_PP_5mA); // TXD-配置推挽输出，注意先让IO口输出高电平
    UART1_DefInit();

    UART1_ByteTrigCfg(UART_7BYTE_TRIG);
    UART1_INTCfg(ENABLE, RB_IER_RECV_RDY | RB_IER_LINE_STAT);
    PFIC_EnableIRQ(UART1_IRQn);

    /* 定时器1，设定1ms定时器*/
    TMR1_TimerInit(FREQ_SYS / 100);         // 设置定时时间 10ms
    TMR1_ITCfg(ENABLE, TMR0_3_IT_CYC_END);  // 开启中断
    PFIC_EnableIRQ(TMR1_IRQn);

    GPIOA_ModeCfg(GPIO_Pin_5, GPIO_ModeIN_Floating);//ntc
    GPIOA_ModeCfg(GPIO_Pin_7, GPIO_ModeIN_Floating);//bat
    ADC_ExtSingleChSampInit(SampleFreq_3_2, ADC_PGA_0);
    for (i = 0; i < 5; i++)
        RoughCalib_Value = ADC_DataCalib_Rough();
    ADC_ChannelCfg(5);
    for (i = 0; i < 20; i++) {
        adcBuff[i] = ADC_ExcutSingleConver() + RoughCalib_Value; // 连续采样20次
    }
#if 0
    static uint8_t buf[2] = {0x52,0x10};
    static uint8_t pack_buf[12];
    static uint8_t pack_len;
    createPacket(245,3,buf,2,pack_buf,&pack_len);
    app_uart_tx_data(pack_buf, pack_len);
#endif
}

void main_init(void)
{
    system_init();
    PRINT("system run~~~\r\n");
    Key_Init();
    OLED_Init();
    OLED_SetDir(3);
    OLED_Clear(0);
}

void main_loop(void)
{
    while(1)
    {
        //50ms
        if(!Timer_Task_Cnt[0])
        {
            Timer_Task_Cnt[0] = Timer_Task_Tick[0];
            if(FUNC_KEY_GET() == CODE_CLICK)
            {
                PRINT("单击\n");
                oled_menu_handle(&menu_des, &screen_des, MENU_CTR_CODE_NEXT);
            }
            else if (FUNC_KEY_GET() == CODE_D_CLICK)
            {
                PRINT("双击\n");
                oled_menu_handle(&menu_des, &screen_des, MENU_CTR_CODE_PRE);
            }
            else if (FUNC_KEY_GET() == CODE_L_CLICK)
            {
                PRINT("长按\n");
            }
            else if (FUNC_KEY_GET() == CODE_T_CLICK)
            {
                PRINT("连击\n");
            }
            FUNC_KEY_CLEAR();
            if(ENC1_GET() == ENC_CODE_LEFT)
            {
                PRINT("左旋\n");
            }
            else if(ENC1_GET() == ENC_CODE_RIGHT)
            {
                PRINT("右旋\n");
            }
            ENC1_CLEAR();
        }
        //100ms
        if(!Timer_Task_Cnt[1])
        {
            Timer_Task_Cnt[1] = Timer_Task_Tick[1];
            uint16_t i;
            uint32_t sum;
            R8_ADC_CHANNEL = 11;//vol 3020 4.90V 2000 3.3V
            for(i=0; i<500; i++)
            {
                adc_buf[0][i] = ADC_ExcutSingleConver();
            }
            sum = 0;
            for(i=0; i<500; i++)
            {
                sum += adc_buf[0][i];
            }
            adc_buf[0][500] = sum/500;
            R8_ADC_CHANNEL = 1;//temp
            for(i=0; i<500; i++)
            {
                adc_buf[1][i] = ADC_ExcutSingleConver();
            }
            sum = 0;
            for(i=0; i<500; i++)
            {
                sum += adc_buf[1][i];
            }
            adc_buf[1][500] = sum/500;
        }
        //250//
        if(!Timer_Task_Cnt[2])
        {
            Timer_Task_Cnt[2] = Timer_Task_Tick[2];
        }
        //2000ms
        if(!Timer_Task_Cnt[3])
        {
            Timer_Task_Cnt[3] = Timer_Task_Tick[3];

            app_uart_tx_data("hello world\n",12);
        }
        //10ms
        if(!Timer_Task_Cnt[4])
        {
            Timer_Task_Cnt[4] = Timer_Task_Tick[4];

            OLED_Clear(0);
            uint32_t sec = uwTick / 1000 + 19 * 3600 + 59 * 60 + 56;
            Time_Display((sec / 3600) % 24, (sec / 60) % 60, sec % 60, uwTick % 1000);
            OLED_Refresh_Gram();
        }
        app_uart_process();
    }
}
