/**
 * @file math_lib.h
 * @brief 汎用的な関数を用意
 * @author Masaya Kobayashi
 * 
 */

#ifndef __MATH_LIB_H__
#define __MATH_LIB_H__

/**
 * @brief 指定された値(val)を上限値・下限値で制限して返却
 * 
 * @param val int 制限したい値
 * @param u_limit int 上限値
 * @param l_limit int 下限値
 * @return int 制限した結果の値
 */
int upper_lower_limit(int val, int u_limit, int l_limit);

/**
 * @brief 絶対値を返却
 * 
 * @param j int 対象の数字
 * @return int 絶対値
 */
int abs(int j);

#endif