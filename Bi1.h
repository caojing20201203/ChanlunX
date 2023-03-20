﻿#ifndef __BI1_H__
#define __BI1_H__
#include "BaoHanChuLi.h"
#include "FenXingChuLi.h"
#include <vector>

using namespace std;

#pragma pack(push, 1)

enum class BiType {NONE, UP, DOWN, FAILURE_UP, FAILURE_DOWN, TEMP_UP, TEMP_DOWN, FAILURE_TEMP_UP, FAILURE_TEMP_DOWN, NEW_UP, NEW_DOWN};
class Bi {
private:
    BiType type = BiType::NONE;
    FenXing start_fx = FenXing();
    FenXing stop_fx = FenXing();
    int start_pos, stop_pos, verify_stop_pos;
    float high = 0.0;
    float low = 0.0;
    float length = 0.0;
    HighLowType high_low_type = HighLowType::NONE;

public:
    Bi() {
        this->type = BiType::NONE;
        this->start_fx = FenXing();
        this->stop_fx = FenXing();
        this->high = 0;
        this->low = 0;
        this->length = 0;
        this->start_pos = 0;
        this->stop_pos = 0;
        this->verify_stop_pos = 0;
        
    }

    Bi(FenXing start, FenXing stop) {
        this->start_fx = start;
        this->stop_fx = stop;
        if (start.get_type() == FenXingType::VERIFY_TOP) {
            this->type = BiType::DOWN;
            this->high = start.get_middle().get_high();
            this->low = stop.get_middle().get_low();
        }
        else {
            if (start.get_type() == FenXingType::VERIFY_BOTTOM) {
                this->type = BiType::UP;
                this->high = stop.get_middle().get_high();
                this->low = start.get_middle().get_low();
            }
        }
        this->length = this->high - this->low;
        this->start_pos = this->start_fx.get_stop_position();
        this->stop_pos = this->stop_fx.get_stop_position();
        this->verify_stop_pos = this->stop_fx.get_verify_stop_position();
        this->high_low_type = stop.get_high_low_type();
    }

    BiType get_type() {
        return(this->type);
    }

    void set_type(BiType bi_type) {
        this->type = bi_type;
    }

    float get_high() {
        return(this->high);
    }

    float get_low() {
        return(this->low);
    }

    float get_length() {
        return(this->length);
    }

    FenXing get_start_fx() {
        return(this->start_fx);
    }

    FenXing get_stop_fx() {
        return(this->stop_fx);
    }

    bool operator==(Bi comp) {
        if (this->type == comp.type && this->start_fx == comp.start_fx && this->stop_fx == comp.stop_fx)
            return(true);
        else
            return(false);
    }

    Bi generate_bi(Bi first_bi, Bi second_bi, Bi three_bi) {
        FenXing start_fx, stop_fx;
        start_fx = first_bi.get_start_fx();
        stop_fx = three_bi.get_stop_fx();
        return(Bi(start_fx, stop_fx));
    }

    bool equal(Bi bi) {
        if (abs(this->high - bi.get_high()) <= 0.10 && abs(this->low - bi.get_low()) <= 0.10)
            return(true);
        else
            return(false);
    }

    Bi update_stop_bi(Bi bi, FenXing stop_fx) {
        FenXing start_fx = bi.get_start_fx();
        return(Bi(start_fx, stop_fx));
    }

    int get_start_pos() {
        return(this->start_pos);
    }

    int get_stop_pos() {
        return(this->stop_pos);
    }

    int get_stop_verify_position() {
        return(this->verify_stop_pos);
    }

    HighLowType get_high_low_type() {
        return(this->high_low_type);
    }
};


enum class BiChuLiStatus {START, END, FINDBI, FINDNEWBI};

class BiChuLi {
private:
    BiChuLiStatus status = BiChuLiStatus::START;
    FenXing last_fx = FenXing();
    FenXingChuLi fxcl;
    Bi find_new_bi(FenXing tmp_fx);
public:
    vector<Bi> biList;
    vector<Bi> keyBiList;
    Bi last_bi = Bi();
    void handle(vector<Kxian1> &kxlist);
    Bi __find_bi(FenXing fx);
    vector<Bi> __baohan_process(int start, int stop);
    vector<Bi> __remove_baohan_bi();

    BiChuLi();
};


//extern "C" _declspec(dllexport) void Bi3(int nCount, float* pOut, float* pHigh, float* pLow, float* pIn);
void Bi3_bi(int nCount, float* pOut, float* pHigh, float* pLow, float* pIn);
void Bi4_bi(int nCount, float* pOut, float* pHigh, float* pLow, float* pIn);
extern void OutputDebugPrintf(const char* strOutputString, ...);

#pragma pack(pop)
#endif
