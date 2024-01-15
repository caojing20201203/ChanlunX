#pragma once
#include "bi1.h"
#include "bi_zhongshu.h"
#include<vector>

using namespace std;

enum class XianDuanType { NONE, TEMP_UP, TEMP_DOWN, FAILURE_TEMP_UP, FAILURE_TEMP_DOWN, UP, DOWN, FAILURE_UP, FAILURE_DOWN, LONG_XIANDUAN };
enum class XianDuanKind {NONE, SigleBi,ThreeBi,LONGXIANDUAN, PANZHENG, QUSHI};

class Bi_ZhongShu {
private:
    Bi input;
    Bi bi1;
    Bi bi2;
    Bi bi3;
    Bi output;
    Bi max_bi;
    Bi min_bi;
    float max_high, min_low, high, low;
public:
    Bi_ZhongShu() {
        this->max_high = 0;
        this->min_low = 0;
        this->high = 0;
        this->low = 0;
    }

    Bi_ZhongShu(Bi input, Bi bi1, Bi bi2, Bi bi3, Bi output) {
        this->input = input;
        this->max_high = max(bi1.get_high(), bi3.get_high());
        this->high = min(bi1.get_high(), bi3.get_high());
        this->min_low = min(bi1.get_low(), bi3.get_low());
        this->low = max(bi1.get_low(), bi3.get_low());
        this->output = output;
    }

    float get_low() {
        return(this->low);
    }

    float get_high() {
        return(this->high);
    }

    void set_max_bi(Bi bi) {
        this->max_bi = bi;
    }

    void set_min_bi(Bi bi) {
        this->min_bi = bi;
    }

    float get_max_high() {
        return(this->max_high);
    }

    float get_min_low() {
        return(this->min_low);
    }

    Bi get_input() {
        return(this->input);
    }

    Bi get_output() {
        return(this->output);
    }

    void set_input(Bi bi) {
        this->input = bi;
    }

    void set_output(Bi bi) {
        this->output = bi;
    }
};

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

enum class XianDuanChuLiStatus { a_1, a_2, a1_equal_a2, a_3, a2_equal_a3, a3_highlow_a1, a3_normal_a1, longxianduan, longxianduan_normal, A, b_3, b_3_normal, b, b2_equal_b3, B};

enum class FindXianDuanReturnType { None, Failure, NewXianDuan, XianDuanUpgrade, One, Two, Two_Bi, Three, FindZhongShu, ZhongShuSuccess, ZhongShuFailer, ZhongShuUpgrade };
struct FindXianDuanReturn {
    FindXianDuanReturnType type;
    XianDuan xd1;
    XianDuan xd2;
    XianDuan xd3;
    Bi_ZhongShu zhongshu;
};

enum class FindZhongShuReturnType {None, ThreeBuy, ThreeSell, BeiChi};
struct FindZhongShuReturn {
    FindZhongShuReturnType type;
    ZhongShu zs;
};

class XianDuanChuLi {
private:
    XianDuan last_xd;
    XianDuanChuLiStatus status;
    Bi last_bi;

    Bi a_1 = Bi();
    Bi a_2 = Bi();
    Bi a_3 = Bi();
    Bi a_4 = Bi();
    Bi a_5 = Bi();
    Bi b_1 = Bi();
    Bi b_2 = Bi();
    Bi b_3 = Bi();
    Bi b_4 = Bi();
    Bi b_5 = Bi();
    Bi b = Bi();
    Bi a = Bi();

    Bi zhongshu_last_bi = Bi();

    BiChuLi bicl;

    Bi_ZhongShuChuLi A_zscl, B_zscl;
    ZhongShu A, B;

    FindXianDuanReturn find_xianduan(Bi bi);
    FindXianDuanReturn __find_xianduan(Bi bi);


    void debug_xianduan(XianDuan xd);
    FindXianDuanReturn failure_xd(Bi bi1, Bi bi2);
    vector<Bi> normal_process(Bi bi1, Bi bi2, Bi bi3);
    vector<Bi> pre_zhongshu_process(Bi input, Bi bi1, Bi bi2, Bi bi3);
    FindZhongShuReturn zhongshu_process(ZhongShu zs, Bi bi);

    XianDuan get_last_xd(int num);
    void push_bi_list();

public:
    XianDuanChuLi();
    void handle(vector<Kxian1>& kxlist);


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
