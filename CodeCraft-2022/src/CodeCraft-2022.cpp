#include <iostream>
#include <algorithm>
#include <cstring>
#include <cmath>
#include <queue>
#include <map>
#include <time.h>
using namespace std;

//----------------------------------------------------------//
const bool OnlineJudge = false; // 本地测试使用相对路径填 false，在线评测使用绝对路径填 true
//----------------------------------------------------------//

// 本文件中的数组均为 1-index
// T : 时刻, T95 : 排名第T95的带宽算入总分, T05 : 剩下 T05 个带宽不算入总分
// N : 客户, M : 边缘
const int T_limit = 8930, User_limit = 36, Server_limit = 136, Flow_limit = 101;
int T, T95, T05, T90, T10, N, M, F;
int Demand[T_limit][User_limit][Flow_limit], MaxBandwidth[Server_limit]; // Demand : 客户节点需求， MaxBandwidth : 边缘节点上限（可取到）
int LinkAble[User_limit][Server_limit]; // 0/1，是通过 Y 和 Q 得到的数据，表示两点之间是否有边
int BaseCost;
// 以上数据通过 io.read(); 得到

int Answer[T_limit][User_limit][Flow_limit], AnswerSave[T_limit][User_limit][Flow_limit];
int BestAnswer = 2e9;
bool NewRecord = false;
pair<int,int> Bandwidth[Server_limit][T_limit]; // io.check(); 计算出的排序后带宽使用量 (first) 和位置 (second)

clock_t start_time; // 程序开始的时间，限时 300s
bool TLE_flag = false; // 全局定时停止变量

inline void CheckTime() {
    const double TIME_LIMIT = 290;
    clock_t current_time = clock();
    double total_t = (double)(current_time - start_time) / CLOCKS_PER_SEC;
    if (total_t > TIME_LIMIT) TLE_flag = true;
}

#include "io.h"
#include "sol1.h"
#include "sol2.h"

int main() {
    start_time = clock();
    io.read();
    // io.show();

    sol2::main();

    io.write();
}