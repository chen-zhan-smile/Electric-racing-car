/*
 * @Description: 
 * @Version: 1.0
 * @Autor: chen zhan
 * @Date: 2022-07-19 16:50:31
 * @LastEditors: chen zhan
 * @LastEditTime: 2022-07-20 21:18:44
 */
#ifndef Serial_port
#define Serial_port
#include <vector>
extern double serial_send(std::vector<unsigned char> &send_data);
extern int openPort();
#endif // !Serial_port
