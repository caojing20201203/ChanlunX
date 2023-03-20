﻿#ifndef __FENXINGCHULI_H
#define __FENXINGCHULI_H

#include <vector>
#include <string>
#include "BaoHanChuLi.h"

using namespace std;

#pragma pack(push, 1)

enum class FenXingType { NONE, TOP, BOTTOM, FAILURE_TOP, FAILUER_TOP_VERIFY_BOTTOM, FAILURE_BOTTOM, FAILURE_BOTTOM_VERIFY_TOP, VERIFY_TOP, VERIFY_BOTTOM, FAILURE_VERIFY_TOP, FAILURE_VERIFY_BOTTOM, NEW_TOP, NEW_BOTTOM };
enum class HighLowType {NONE, NEW_HIGH, NEW_LOW};

//分型
class FenXing {
private:
    FenXingType type = FenXingType::NONE;
    HighLowType high_low_type = HighLowType::NONE;
    float gao = 0;
    float di = 0;
    Kxian1 left = Kxian1();
    Kxian1 middle = Kxian1();
    Kxian1 right = Kxian1();
    Kxian1 free = Kxian1();
    int start_pos = 0;
    int stop_pos = 0;
    int verify_stop_pos = 0;

public:
    FenXing() {
        this->type = FenXingType::NONE;
        this->gao = 0;
        this->di = 0;
        this->left = this->middle = this->right = this->free = Kxian1();
        this->start_pos = 0;
        this->stop_pos = 0;
        this->verify_stop_pos = 0;
    }

    FenXing(Kxian1 left, Kxian1 middle, Kxian1 right, Kxian1 free) {
        if (middle.get_high() > left.get_high()) {
            if (right.get_high() < middle.get_high()) {
                this->type = FenXingType::TOP;
                this->gao = middle.get_high();
                this->di = min(left.get_low(), right.get_low());
            }
        }
        else {
            if (right.get_low() > middle.get_low()) {
                this->type = FenXingType::BOTTOM;
                this->gao = max(left.get_high(), right.get_high());
                this->di = middle.get_low();
            }
        }
        this->high_low_type = high_low_type;
        this->left = left;
        this->middle = middle;
        this->right = right;
        this->free = free;
        this->start_pos = left.get_position();
        this->stop_pos = middle.get_position();
        this->verify_stop_pos = free.get_position();
    }

    FenXingType get_type() {
        return(this->type);
    }

    void set_type(FenXingType type) {
        this->type = type;
    }

    float get_high() {
        return(this->gao);
    }

    float get_low() {
        return(this->di);
    }

    int get_start_position() {
        return(this->start_pos);
    }

    int get_stop_position() {
        return(this->stop_pos);
    }

    int get_verify_stop_position() {
        return(this->verify_stop_pos);
    }

    void set_free(Kxian1 kx) {
        this->free = kx;
    }

    Kxian1 get_left() {
        return(this->left);
    }

    Kxian1 get_middle() {
        return(this->middle);
    }

    Kxian1 get_right() {
        return(this->right);
    }

    Kxian1 get_free() {
        return(this->free);
    }

    void set_high_low_type(HighLowType status) {
        this->high_low_type = status;
    }

    HighLowType get_high_low_type() {
        return(this->high_low_type);
    }

    bool operator==(FenXing comp) {
        if (this->type == comp.type && this->left == comp.get_left() && this->middle == comp.get_middle() && this->right == comp.get_right())
            return(true);
        else
            return(false);
    }
};

//enum class FenXingChuLiStatus { LEFT, MIDDLE, RIGHT, FREE };
enum class FreeStatus {NONE, FREE_0, FREE_1, FREE_2};
enum class FenXingChuLiStatus {START, ONE, TWO, HIGHLOW, LOWHIGH, THREE, FOUR, FIVE, SIX, FENXING_END};
enum class FenXingChuLiState { NONE, BOTTOM, VERIFY_BOTTOM, UP, TOP, VERIFY_TOP, DOWN };
class FenXingChuLi {
private:
    //Kxian1 left, middle, right, free, last_bar;
    Kxian1 one, two, three, four, five, six, last_bar;
    Kxian1 free_0, free_1, free_2;
    FreeStatus free_status;
    float min_low, max_high;
    FenXingChuLiStatus status;
    static float comp_fx_gao, comp_fx_di;
    static int comp_fx_gao_count, comp_fx_di_count;
    FenXing fx, temp_fx, last_fx;
    FenXing __right_process(Kxian1 kx);
    void __init_fenxing(Kxian1 kx);
    FenXing __high_process(Kxian1 kx);
    FenXing __low_process(Kxian1 kx);
    FenXing __high_low_process(Kxian1 kx);
    FenXing __failure_verify_top(Kxian1 kx);
    FenXing __failue_verify_bottom(Kxian1 kx);
    FenXing __determ_fenxing(Kxian1 kx);
    FenXing __kxian_process(Kxian1 kx);

public:
    vector<Kxian1> kxianList;
    vector<FenXing> fenXingList;
    vector<FenXing> keyFenXingList;
    void handle(vector<Kxian1>& kxianList);
    FenXing __find_fenxing(Kxian1 kx);
    FenXing __find_fenxing_old(Kxian1 kx);
    FenXingChuLi();
    bool firstBarStatus;
    FenXingChuLiState state;
    FenXing get_current_temp_fx();
    FenXing get_current_fx();
};

void Bi3_fenxing(int nCount, float* pOut, float* pHigh, float* pLow, float* pIn);
void Bi3_fenxing_highlow(int nCount, float* pOut, float* pHigh, float* pLow, float* pIn);
void Bi4_fenxing(int nCount, float* pOut, float* pHigh, float* pLow, float* pIn);
void GuiDao_Gao(int nCount, float* pOut, float* pHigh, float* pLow, float* pIn);
void GuiDao_Di(int nCount, float* pOut, float* pHigh, float* pLow, float* pIn);
void GuiDao_Zhong(int nCount, float* pOut, float* pHigh, float* pLow, float* pIn);
void GuiDao_Status(int nCount, float* pOut, float* pHigh, float* pLow, float* pIn);
void Bi4_fenxing_state(int nCount, float* pOut, float* pHigh, float* pLow, float* pIn);
#pragma pack(pop)

#endif
