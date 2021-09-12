#pragma once
#include "Bi1.h"
using namespace std;

class ZhongShu1 {
private:
    float zhongshu_high = 0;
    float zhongshu_low = 0;
    float max_high = 0;
    float min_low = 0;
    Bi1 input_bi = Bi1();
    Bi1 output_bi = Bi1();
public:
    vector<Bi1> biList;
    ZhongShu1() {
        this->input_bi = Bi1();
        zhongshu_high = 0;
        zhongshu_low = 0;
        max_high = 0;
        min_low = 0;
    }

    ZhongShu1(Bi1 in, Bi1 a, Bi1 b, Bi1 c) {
        this->input_bi = in;
        this->biList.push_back(a);
        this->biList.push_back(b);
        this->biList.push_back(c);
        this->max_high = max(a.get_high(), c.get_high());
        this->zhongshu_high = min(a.get_high(), c.get_high());
        this->min_low = min(a.get_low(), c.get_low());
        this->zhongshu_low = max(a.get_low(), c.get_low());
    }

    float get_zhongshu_high() {
        return(this->zhongshu_high);
    }

    float get_zhongshu_low() {
        return(this->zhongshu_low);
    }

    float get_max_high() {
        return(this->max_high);
    }

    void set_max_high(float high) {
        this->max_high = high;
    }

    void set_min_low(float low) {
        this->min_low = low;
    }

    float get_min_low() {
        return(this->min_low);
    }

    void set_output_bi(Bi1 bi) {
        this->output_bi = bi;
    }
};

enum class ZhongShuChuLiStatus { NONE, IN, OUT };
class ZhongShuChuLi {
private:
    ZhongShu1 zhongshu;
    ZhongShuChuLiStatus status;
public:
    ZhongShuChuLi(ZhongShu1 zs);
    ZhongShu1 find_zhongshu(Bi1 bi);
    void set_status(ZhongShuChuLiStatus st);
};
