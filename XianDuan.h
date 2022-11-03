#pragma once
#include "bi1.h"
#include "bi_zhongshu.h"
#include<vector>

using namespace std;

enum class XianDuanType { NONE, TEMP_UP, TEMP_DOWN, FAILURE_TEMP_UP, FAILURE_TEMP_DOWN, UP, DOWN, FAILURE_UP, FAILURE_DOWN, LONG_XIANDUAN };
enum class XianDuanKind {NONE, SigleBi,ThreeBi,LONGXIANDUAN, PANZHENG, QUSHI};
class XianDuan {
private:
    XianDuanType type = XianDuanType::NONE;
    vector<Bi> bi_list;
    Bi start_bi, stop_bi;
    int start_pos, stop_pos, verify_pos;
    float high, low;
    float length;
    HighLowType high_low_type;
    XianDuanKind kind;

public:
    XianDuan() {
        this->type = XianDuanType::NONE;
        this->high = 0;
        this->low = 0;
        this->length = 0;
        this->start_bi = Bi();
        this->stop_bi = Bi();
        this->start_pos = 0;
        this->stop_pos = 0;
        this->verify_pos = 0;
        this->high_low_type = HighLowType::NONE;
        this->kind = XianDuanKind::NONE;
    }

    XianDuan(Bi start_bi, Bi stop_bi) {
        this->start_bi = start_bi;
        this->stop_bi = stop_bi;
        this->high = 0;
        this->low = 0;
        this->length = 0;
        this->start_pos = start_bi.get_start_fx().get_middle().get_position();
        this->stop_pos = stop_bi.get_stop_fx().get_middle().get_position();
        this->verify_pos = stop_bi.get_stop_fx().get_free().get_position();

        if (start_bi.get_type() == BiType::DOWN) {
            this->high = start_bi.get_high();
            this->low = stop_bi.get_low();
            this->length = this->high - this->low;
            this->type = XianDuanType::DOWN;
        }
        else {
            if (start_bi.get_type() == BiType::UP) {
                this->low = start_bi.get_low();
                this->high = stop_bi.get_high();
                this->length = this->high - this->low;
                this->type = XianDuanType::UP;
            }
        }
        this->high_low_type = stop_bi.get_high_low_type();
    }

    XianDuanType get_type() {
        return(this->type);
    }

    void set_type(XianDuanType xd_type) {
        this->type = xd_type;
    }

    XianDuanKind get_kind() {
        return(this->kind);
    }

    void set_kind(XianDuanKind kind) {
        this->kind = kind;
    }

    float get_high() {
        return(this->high);
    }

    float get_low() {
        return(this->low);
    }

    Bi get_start_bi() {
        return(this->start_bi);
    }

    Bi get_stop_bi() {
        return(this->stop_bi);
    }

    int get_start_pos() {
        return(this->start_pos);
    }

    int get_stop_pos() {
        return(this->stop_pos);
    }

    int get_stop_verify_pos() {
        return(this->verify_pos);
    }

    float get_length() {
        return(this->length);
    }

    bool operator==(XianDuan that) {
        if (this->start_bi == that.start_bi && this->stop_bi == that.stop_bi && this->type == that.type)
            return(true);
        else
            return(false);
    }

    void save_bi_list(vector<Bi> bilist) {
        this->bi_list = bilist;
    }

    vector<Bi> get_bi_list() {
        return(this->bi_list);
    }

    XianDuan generate_xd(XianDuan xd1, XianDuan xd2, XianDuan xd3) {
        Bi start_bi, stop_bi;
        XianDuan tmp_xd;

        start_bi = xd1.get_start_bi();
        stop_bi = xd3.get_stop_bi();
        tmp_xd = XianDuan(start_bi, stop_bi);
        return(tmp_xd);
    }
};

enum class XianDuanChuLiStatus { START, LEFT, LEFT_EQUAL, ZHONGSHU_NO_INPUT, LEFT_EQUAL_NORMAL, AFTER_LEFT, MIDDLE_EQUAL, MIDDLE_EQUAL_NORMAL, MIDDLE_HIGHLOW, MIDDLE_NORMAL, LEFT_INCLUDE_MIDDLE, AFTER_MIDDLE, RIGHT, RIGHT_NORMAL, RIGHT_NORMAL_NORMAL, RIGHT_NORMAL_NORMAL_NORMAL, RIGHT_NORMAL_NORMAL_NORMAL_NORMAL, AFTER_RIGHT, FREE, AFTER_FREE, AFTER_FREE_1, AFTER_FREE_2, AFTER_FREE_3, LONGXIANDUAN, LONGXIANDUAN_RIGHT, LONGXIANDUAN_RIGHT_NORMAL, LONGXIANDUAN_FREE, LONGXIANDUAN_AFTER_FREE, LONGXIANDUAN_AFTER_FREE_1, A, b_3, b_3_normal, B_b1, B_b2, B_b3,B_b4, B, c_3, c_3_normal, C_c1, C_c2, C_c3};

enum class FindXianDuanReturnType { None, Failure, NewXianDuan, XianDuanUpgrade, One, Two, Two_Bi, Three, FindZhongShu, ZhongShuSuccess, ZhongShuFailer, ZhongShuUpgrade };
struct FindXianDuanReturn {
    FindXianDuanReturnType type;
    XianDuan xd1;
    XianDuan xd2;
    XianDuan xd3;
    Bi_ZhongShu zhongshu;
};

class XianDuanChuLi {
private:
    XianDuan last_xd;
    XianDuanChuLiStatus status;
    Bi last_bi;

    Bi start = Bi();
    Bi left = Bi();
    Bi after_left = Bi();
    Bi middle = Bi();
    Bi  after_middle = Bi();
    Bi right = Bi();
    Bi after_right = Bi();
    Bi free = Bi();
    Bi after_free = Bi();
    Bi after_free_1 = Bi();
    Bi after_free_2 = Bi();

    Bi a, b, b_1, b_2,b_3;
    Bi B_b1, B_b2, B_b3, B_b4;
    Bi c, c_1, c_2, c_3;
    Bi C_c1, C_c2, C_c3, C_c4;
    BiChuLi bicl;

    Bi_ZhongShuChuLi A_zscl, B_zscl;

    FindXianDuanReturn find_xianduan(Bi bi);
    FindXianDuanReturn __find_xianduan(Bi bi);
    FindXianDuanReturn __find_xianduan1(Bi bi);
    void __backroll(Bi bi);
    void debug_xianduan(XianDuan xd);
    FindXianDuanReturn failure_xd();
    FindXianDuanReturn __right_process(Bi bi);
    FindXianDuanReturn __left_process(Bi bi);
    bool __middle_process(Bi bi);
    bool __determ_zhongshu(Bi bi);
    XianDuan get_last_xd(int num);
    void push_bi_list();
    FindXianDuanReturn set_xianduan(FindXianDuanReturnType ret_type); 
    FindXianDuanReturn __enter_zhongshu(Bi input, Bi bi1, Bi bi2, Bi bi3, char type);
public:
    XianDuanChuLi();
    void handle(vector<Kxian1>& kxlist);
    vector<Bi> get_xd_bi_list();

    vector<XianDuan> xianDuanList;
    vector<XianDuan> key_xianduan_list;
    vector<Bi_ZhongShu> zhongshu_list;
};

void Bi3_xianduan(int nCount, float* pOut, float* pHigh, float* pLow, float* pIn);
void Bi3_xianduan_bi(int nCount, float* pOut, float* pHigh, float* pLow, float* pIn);
void Bi4_xianduan(int nCount, float* pOut, float* pHigh, float* pLow, float* pIn);
void Bi3_bi_zhongshu(int nCount, float* pOut, float* pHigh, float* pLow, float* pIn);
void Bi3_bi_zhongshu_high(int nCount, float* pOut, float* pHigh, float* pLow, float* pIn);
void Bi3_bi_zhongshu_low(int nCount, float* pOut, float* pHigh, float* pLow, float* pIn);
extern void OutputDebugPrintf(const char* strOutputString, ...);
