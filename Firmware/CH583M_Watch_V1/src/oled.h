/*
 * oled.h
 *
 *  Created on: Mar 10, 2022
 *      Author: 27731
 */

#ifndef USER_OLED_H_
#define USER_OLED_H_

#include <stdint.h>

#define OLED_CMD    0       //写命令
#define OLED_DATA   1       //写数据

#define SCREEN_W    128
#define SCREEN_H    64
#define OLED_PAGE_V 2      //页数
#define OLED_PAGE_H 2
#define OLED_W      SCREEN_W * OLED_PAGE_V
#define OLED_H      SCREEN_H * OLED_PAGE_H

#define USB_HW_SPI  1       //硬SPI


#define OLED_BLK_Set()       GPIOB_SetBits(GPIO_Pin_12)
#define OLED_BLK_Clr()       GPIOB_ResetBits(GPIO_Pin_12)
#define OLED_CS_Set()        GPIOB_SetBits(GPIO_Pin_17)
#define OLED_CS_Clr()        GPIOB_ResetBits(GPIO_Pin_17)
#define OLED_RES_Set()       GPIOB_SetBits(GPIO_Pin_16)
#define OLED_RES_Clr()       GPIOB_ResetBits(GPIO_Pin_16)
#define OLED_DC_Set()        GPIOB_SetBits(GPIO_Pin_15)   //写数据
#define OLED_DC_Clr()        GPIOB_ResetBits(GPIO_Pin_15) //写命令

#define OLED_SCLK_Clr()      GPIOB_SetBits(GPIO_Pin_13)
#define OLED_SCLK_Set()      GPIOB_ResetBits(GPIO_Pin_13)
#define OLED_MOSI_Set()      GPIOB_SetBits(GPIO_Pin_14)
#define OLED_MOSI_Clr()      GPIOB_ResetBits(GPIO_Pin_14)

#define OLED_SPI_SEND(x)    SPI0_MasterSendByte(x)

enum
{
    OPT_NORMAL = 0,
    OPT_COVER  = 1,
    OPT_REV    = 2,
    OPT_REV_COV = 3,
    OPT_NOR_NC  = 4,
    OPT_REV_NC  = 5,
};

typedef struct
{
    uint16_t   widget;         //OLED宽度
    uint16_t   height;         //OLED高度
    uint8_t    oled_page_h;    //OLED水平页宽
    uint8_t    oled_page_v;    //OLED垂直页高
    uint8_t    oled_move_dir;  //OLED移动方向

    uint16_t   oled_cur_pos_x; //OLED当前水平显示位置
    uint16_t   oled_cur_pos_y; //OLED当前显示垂直位置

    uint16_t   oled_offset_x;  //OLED写水平偏移
    uint16_t   oled_offset_y;  //OLED写垂直偏移
}OLED_Display_Ctr;

typedef struct
{
    char * index;
    uint8_t * font;
    uint8_t w;
    uint8_t h;
}img_data_desc;

typedef struct
{
    img_data_desc chin;
    img_data_desc engl;
}font_desc;

typedef struct
{
    int16_t x;
    int16_t y;
    uint16_t w;
    uint16_t h;
    int16_t off_x;
    int16_t off_y;
    uint8_t dis_opt; //display option
}dis_data_desc;

typedef enum
{
    MENU_CTR_CODE_NONE = 0,
    MENU_CTR_CODE_NEXT,
    MENU_CTR_CODE_PRE,
    MENU_CTR_CODE_FIR,
    MENU_CTR_CODE_LAST,
}menu_ctr_typ;

typedef struct
{
    char ** menu_name;
    uint8_t menu_num;
    uint8_t meun_last;
    uint8_t menu_cur;
    uint8_t menu_pos;
    uint8_t menu_pct;   //进度百分比
    uint8_t menu_pct_cnt;
    uint8_t menu_dir;
    uint8_t language;
    uint8_t seekbar_w;
    font_desc *font_des;
}menu_desc;

typedef struct
{
    uint8_t screen_w;
    uint8_t screen_h;
}screen_desc;

extern OLED_Display_Ctr oled_ctr;

#define OLED_MOVE_DEMO_EN 0
#define OLED_CUR_POS_X  oled_ctr.oled_cur_pos_x
#define OLED_CUR_POS_Y  oled_ctr.oled_cur_pos_y
#define OLED_OFFSET_X   oled_ctr.oled_offset_x
#define OLED_OFFSET_Y   oled_ctr.oled_offset_y
#define OLED_MOVE_DIR   oled_ctr.oled_move_dir

#define EX_MOVE_POS(x,y)    do\
                            {\
                                if(x<0)\
                                {\
                                    if((OLED_CUR_POS_X + x) >= 0)\
                                    {\
                                        OLED_CUR_POS_X += x;\
                                    }\
                                    else\
                                    {\
                                        OLED_CUR_POS_X = 0;\
                                    }\
                                }\
                                else\
                                {\
                                    OLED_CUR_POS_X += x;\
                                    if(OLED_CUR_POS_X >= (OLED_W * (OLED_PAGE_H - 1)))\
                                    {\
                                        OLED_CUR_POS_X = (OLED_W * (OLED_PAGE_H - 1));\
                                    }\
                                }\
                                if(y<0)\
                                {\
                                    if((OLED_CUR_POS_Y + y) >= 0)\
                                    {\
                                        OLED_CUR_POS_Y += y;\
                                    }\
                                    else\
                                    {\
                                        OLED_CUR_POS_Y = 0;\
                                    }\
                                }\
                                else\
                                {\
                                    OLED_CUR_POS_Y += y;\
                                    if(OLED_CUR_POS_Y >= (OLED_H * (OLED_PAGE_V - 1)))\
                                    {\
                                        OLED_CUR_POS_Y = (OLED_H * (OLED_PAGE_V - 1));\
                                    }\
                                }\
                            }\
                            while(0)

extern uint8_t OLED_GRAM[SCREEN_W * OLED_PAGE_H][OLED_PAGE_V * SCREEN_H / 8];
extern const unsigned char bmp_battery[13][20];

extern const char * articl[46];

extern const char *menu_main[];
extern menu_desc menu_des;
extern screen_desc screen_des;


//OLED控制用函数
void OLED_WR_Byte(uint8_t dat,uint8_t cmd);
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Refresh_Gram(void);

void OLED_Init(void);
void OLED_Clear(uint8_t para);
void OLED_DrawPoint(uint16_t x,uint16_t y,uint8_t t);
void OLED_SetDir(uint8_t para);
void OLED_SetContrast(uint8_t para);

void OLED_Show_Picture(img_data_desc *img,dis_data_desc *dis);
void OLED_Show_String(img_data_desc *img,dis_data_desc *dis, const char *str);
void Display_String(font_desc *font,dis_data_desc *dis, const char *str);
void oled_menu_handle(menu_desc *menu, screen_desc *screen, menu_ctr_typ ctr_code);
void oled_menu_dis(menu_desc *menu, screen_desc *screen);

void Time_Display(uint8_t hou, uint8_t min, uint8_t sec, uint16_t msec);

void OLED_Fill(int16_t x1,int16_t y1,int16_t x2,int16_t y2,uint8_t dot);
void OLED_DrawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2,uint8_t dot);
void OLED_DrawRectangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2,uint8_t dot);

#endif /* USER_OLED_H_ */
