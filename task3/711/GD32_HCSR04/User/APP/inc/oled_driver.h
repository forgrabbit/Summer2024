#ifndef _OLED_DRIVER_H
#define _OLED_DRIVER_H

// OLED 模块管脚
#define OLED_CK_PIN GPIO_PIN_15 //OLED CLK引脚
#define OLED_CK_PORT GPIOC			//OLED CLK引脚端口
#define OLED_CK_CLK RCU_GPIOC		//OLED CLK引脚时钟

#define OLED_DI_PIN GPIO_PIN_14
#define OLED_DI_PORT GPIOC
#define OLED_DI_CLK RCU_GPIOC

#define OLED_RST_PIN GPIO_PIN_13
#define OLED_RST_PORT GPIOC
#define OLED_RST_CLK RCU_GPIOC

#define OLED_DC_PIN GPIO_PIN_0
#define OLED_DC_PORT GPIOA
#define OLED_DC_CLK RCU_GPIOA

#define OLED_CS_PIN GPIO_PIN_12
#define OLED_CS_PORT GPIOA
#define OLED_CS_CLK RCU_GPIOA

#define OLED_DC_H gpio_bit_set(OLED_DC_PORT, OLED_DC_PIN);
#define OLED_DC_L gpio_bit_reset(OLED_DC_PORT, OLED_DC_PIN);

#define OLED_RST_H gpio_bit_set(OLED_RST_PORT, OLED_RST_PIN);
#define OLED_RST_L gpio_bit_reset(OLED_RST_PORT, OLED_RST_PIN);

#define OLED_DI_H gpio_bit_set(OLED_DI_PORT, OLED_DI_PIN);
#define OLED_DI_L gpio_bit_reset(OLED_DI_PORT, OLED_DI_PIN);

#define OLED_CK_H gpio_bit_set(OLED_CK_PORT, OLED_CK_PIN);
#define OLED_CK_L gpio_bit_reset(OLED_CK_PORT, OLED_CK_PIN);

// #define OLED_CS_H  gpio_bit_set(OLED_CS_PORT, OLED_CS_PIN);      /*!< CS管脚 */
// #define OLED_CS_L gpio_bit_reset(OLED_CS_PORT, OLED_CS_PIN);    /*!< CS管脚 */

#define X_WIDTH 132
#define Y_WIDTH 64

void OLED_Init(void);                                                                                                     // OLED初始化
void OLED_Fill(void);                                                                                                     // OLED全亮
void OLED_CLS(void);                                                                                                      // OLED清屏
void OLED_PutPixel(unsigned char x, unsigned char y);                                                                     // 画一个点
void OLED_ClrDot(unsigned char x, unsigned char y);                                                                       // 清除8个点
void OLED_Rectangle(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, unsigned char gif);           // 画矩形
void OLED_P6x8Str(unsigned char x, unsigned char y, char ch[]);                                                           // 写入一组标准ASCII字符串
void OLED_P8x16Str(unsigned char x, unsigned char y, char ch[]);                                                          // 写入一组标准ASCII字符串
void OLED_Show_BMP(unsigned char x0, unsigned char y0, unsigned char wide, unsigned char high, const unsigned char *bmp); // 显示BMP图片 最大 128×64
void OLED_WrCmd(unsigned char cmd);                                                                                       // OLED命令
void OLED_Set_Pos(unsigned char x, unsigned char y);                                                                      // OLED设置点
void Test_OLED(void);                                                                                                     // OLED 测试函数

#endif
