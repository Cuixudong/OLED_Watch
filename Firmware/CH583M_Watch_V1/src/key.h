/*
 * key.h
 *
 *  Created on: Oct 11, 2023
 *      Author: w
 */

#ifndef SRC_KEY_H_
#define SRC_KEY_H_

#include "CH58x_common.h"

#define ENC1_A_IO_INIT()  GPIOB_ModeCfg(GPIO_Pin_0, GPIO_ModeIN_PU)
#define ENC1_B_IO_INIT()  GPIOB_ModeCfg(GPIO_Pin_1, GPIO_ModeIN_PU)
#define ENC1_Z_IO_INIT()  GPIOB_ModeCfg(GPIO_Pin_22, GPIO_ModeIN_PU)

#define ENC1_A_READ()   ((GPIOB_ReadPortPin(GPIO_Pin_0) == 0x00)?1:0)
#define ENC1_B_READ()   ((GPIOB_ReadPortPin(GPIO_Pin_1) == 0x00)?1:0)
#define ENC1_Z_READ()   ((GPIOB_ReadPortPin(GPIO_Pin_22) == 0x00)?1:0)

#define ENC1_READ()     (ENC1_A_READ()|(ENC1_B_READ()<<1))

#define ENC_CODE_IDLE       0x00
#define ENC_CODE_LEFT       0x01
#define ENC_CODE_RIGHT      0x02
#define ENC_CODE_CLICK      0x03
#define ENC_CODE_D_CLICK    0x04
#define ENC_CODE_L_CLICK    0x05
#define ENC_CODE_T_CLICK    0x06

#define CODE_NONE               (0x00)
#define CODE_CLICK              (0x01)
#define CODE_D_CLICK            (0x02)
#define CODE_L_CLICK            (0x03)
#define CODE_T_CLICK            (0x04)

#define KEYA_CODE   0x01
#define KEYB_CODE   0x02
#define KEYD_CODE   0x04

extern __IO uint8_t func_key_fun;
extern __IO uint8_t enc_code[2];

#define FUNC_KEY_READ()     ENC1_Z_READ()
#define FUNC_KEY_ACTIVE     1
#define FUNC_KEY_CLEAR()    func_key_fun = CODE_NONE
#define FUNC_KEY_GET()      func_key_fun
#define BUTTON_GET()        button_code

#define ENC1_GET()          enc_code[0]
#define ENC1_CLEAR()        enc_code[0] = ENC_CODE_IDLE

void Key_Init(void);
void Button_handle(void);
uint8_t Check_enc(void);

#endif /* SRC_KEY_H_ */
