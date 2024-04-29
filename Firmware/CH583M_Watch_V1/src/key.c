/*
 * key.c
 *
 *  Created on: Oct 11, 2023
 *      Author: w
 */

#include "key.h"

__IO uint8_t func_key_cnt = 0;
__IO uint8_t func_key_code = 0;
__IO uint8_t func_key_fun = CODE_NONE;

void Key_Init(void)
{
    func_key_cnt = 0;
    func_key_code = 0;
    func_key_fun = CODE_NONE;

    ENC1_A_IO_INIT();
    ENC1_B_IO_INIT();
    ENC1_Z_IO_INIT();
}

__attribute__((section(".highcode")))
void Button_handle(void)
{
    if(func_key_code == 0)
    {
        if(FUNC_KEY_READ() == FUNC_KEY_ACTIVE)
        {
            func_key_code = 1;
        }
    }
    else if(func_key_code == 1)
    {
        if(FUNC_KEY_READ() == FUNC_KEY_ACTIVE)
        {
            func_key_code = 2;
        }
        else
        {
            func_key_code = 0;
        }
        func_key_cnt = 0;
    }
    else if(func_key_code == 2)
    {
        if(FUNC_KEY_READ() == FUNC_KEY_ACTIVE)
        {
            func_key_cnt ++;
            //10 * 120 = 1.2s
            if(func_key_cnt > 120)
            {
                //长按
                func_key_fun = CODE_L_CLICK;
                func_key_code = 3;
            }
        }
        //单击释放
        else
        {
            func_key_code = 4;
            func_key_cnt = 0;
        }
    }
    else if(func_key_code == 3)
    {
        //长按待释放
        if(FUNC_KEY_READ() != FUNC_KEY_ACTIVE)
        {
            func_key_code = 0;
            func_key_cnt = 0;
        }
    }
    else if(func_key_code == 4)
    {
        if(FUNC_KEY_READ() == FUNC_KEY_ACTIVE)
        {
            func_key_fun = CODE_D_CLICK;
            func_key_code = 5;
            func_key_cnt = 0;
        }
        else
        {
            //350ms
            func_key_cnt ++;
            if(func_key_cnt > 35)
            {
                func_key_fun = CODE_CLICK;
                func_key_code = 0;
                func_key_cnt = 0;
            }
        }
    }
    else if(func_key_code == 5)
    {
        if(FUNC_KEY_READ() != FUNC_KEY_ACTIVE)
        {
            func_key_code = 0;
            func_key_cnt = 0;
        }
    }
}

__IO uint8_t ENC_TAB[4] = {0,2,3,1};
__IO uint8_t g_enc_old_sta[2];
__IO uint8_t enc_code[2] = {0,};

__attribute__((section(".highcode")))
uint8_t Check_enc(void)
{
    uint8_t res = 0;
    uint8_t enc_status;
    if(1)
    {
        enc_status = ENC1_READ();
        if(((enc_status == ENC_TAB[0]) && (g_enc_old_sta[0] == ENC_TAB[1]))||\
           ((enc_status == ENC_TAB[1]) && (g_enc_old_sta[0] == ENC_TAB[2]))||\
           ((enc_status == ENC_TAB[2]) && (g_enc_old_sta[0] == ENC_TAB[3]))||\
           ((enc_status == ENC_TAB[3]) && (g_enc_old_sta[0] == ENC_TAB[0])))
        {
            res = ENC_CODE_RIGHT;
        }
        else if(((enc_status == ENC_TAB[0]) && (g_enc_old_sta[0] == ENC_TAB[3]))||\
                ((enc_status == ENC_TAB[1]) && (g_enc_old_sta[0] == ENC_TAB[0]))||\
                ((enc_status == ENC_TAB[2]) && (g_enc_old_sta[0] == ENC_TAB[1]))||\
                ((enc_status == ENC_TAB[3]) && (g_enc_old_sta[0] == ENC_TAB[2])))
        {
            res = ENC_CODE_LEFT;
        }

        g_enc_old_sta[0] = enc_status;
    }
    enc_code[0] = res;
    return res;
}
