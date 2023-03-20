#pragma once
#include "CompositeBi.h"
#include "ZhongShu.h"

enum class ZouShiType {NONE, PANZHENG, UP, DOWN};
class ZouShi {
    private:
        ZouShiType type;
        float high, low, lenght;
        int start_pos, stop_pos;
        CompositeBi start_bi, stop_bi;
    public:
        ZouShi() {
            this->type = ZouShiType::NONE;
            this->high = 0;
            this->low = 0;
            this->lenght = 0;
            this->start_pos = 0;
            this->stop_pos = 0;
            this->start_bi = CompositeBi();
            this->stop_bi = CompositeBi();
        }

        ZouShi(ZouShiType type, CompositeBi start_bi, CompositeBi stop_bi) {
            this->type = type;
            this->start_bi = start_bi;
            this->stop_bi = stop_bi;
            if (start_bi.get_type() == CompositeBiType::UP) {
                this->low = start_bi.get_low();
                this->high = stop_bi.get_high();
            } else {
                if (start_bi.get_type() == CompositeBiType::DOWN) {
                    this->high = start_bi.get_high();
                    this->low = stop_bi.get_low();
                }
            }
            this->lenght = this->high - this->low;
            this->start_pos = start_bi.get_start_pos();
            this->stop_pos = stop_bi.get_stop_pos();
        }

        ZouShiType get_type(){
            return(this->type);
        }

        int get_start_pos() {
            return(this->start_pos);
        }

        int get_stop_pos() {
            return(this->stop_pos);
        }

        CompositeBi get_start_xd(){
            return(this->start_bi);
        }

        CompositeBi get_stop_xd() {
            return(this->stop_bi);
        }

        float get_high(){
            return(this->high);
        }

        float get_low(){
            return(this->low);
        }
};

enum class FindZouShiReturnType {None, Failure, One, Two};
struct FindZouShiReturn{
    FindZouShiReturnType type;
    ZouShi zoushi1;
    ZouShi zoushi2;
};

enum class ZouShiChuLiStatus {NONE, START, LEFT, LEFT_EQUAL, AFTER_LEFT_EQUAL, AFTER_LEFT, AFTER_LEFT_NORMAL, AFTER_LEFT_NORMAL_NORMAL, a, A_xd1, A_xd2, A_xd2_highlow, A_xd2_normal, A_THREEBUY, A_THREESELL, A_REVERSE_THREESELL, A_REVERSE_THREEBUY, A_THREEBUY_NORMAL, A_THREESELL_NORMAL,  A_xd3, A, b, B_xd1, B_xd2, B_xd2_normal, B_xd3, B, c};
class ZouShiChuLi {
    private:
        CompositeBiChuLi compbicl;
        ZouShiChuLiStatus status;
        CompositeBi a, b, c;
        CompositeBi A_xd1, A_xd2, A_xd3, B_xd1, B_xd2, B_xd3;
        CompositeBi a_0, a_1, a_2;
        CompositeBi b_0, b_1, b_2;
        CompositeBi c_0, c_1, c_2;
        ZhongShu A, B;
    public:
        ZouShiChuLi();
        ZouShi get_last_zoushi();
        CompositeBi generate_bi(CompositeBi bi1, CompositeBi bi3);
        bool match_zhongshu_xianduan(CompositeBi bi1, CompositeBi bi2);
        void handle(vector<Kxian1>& kxianList);
        FindZouShiReturn find_zoushi(CompositeBi xd);
        vector<ZouShi> zoushi_list;
};

void Bi3_zoushi(int nCount, float* pOut, float* pHigh, float* pLow, float* pIn);
void Bi4_zoushi(int nCount, float* pOut, float* pHigh, float* pLow, float* pIn);
