/**
 * @file sci.h
 * @author Masaya Kobayashi
 * @brief RS232によるシリアル通信。RX220からの出力のみ実装
 * @version 0.1
 * @date 2020-09-09
 * @note 苦しんでつくるマイクロマウスのソフトを少し修正
 * @copyright Copyright (c) 2020
 * 
 */
#ifndef __SCI_H__
#define __SCI_H__

/**
 * @brief RS232によるシリアル通信の初期化処理
 * 
 */
void init_sci(void);

/**
 * @brief 文字列の送信処理
 * 
 * @param str 送信したい文字列
 * @param ... 
 * @return short 送信文字列
 */
short sci_printf(char *str , ...);

/**
 * @brief 1byteの送信処理
 * 
 * @param c 送信したい1byteデータ
 * @return char 送信文字数
 */
char sci_put_1byte(unsigned char c);

#endif

