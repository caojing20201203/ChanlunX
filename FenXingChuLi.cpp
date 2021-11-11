#include "FenXingChuLi.h"
#include "BaoHanChuLi.h"
#include <string>
#include <iostream>
#include <Windows.h>

using namespace std;

FenXingChuLiStatus FenXingChuLi::status;
Kxian1 FenXingChuLi::left, FenXingChuLi::middle, FenXingChuLi::right, FenXingChuLi::free, FenXingChuLi::last_bar;
FenXing FenXingChuLi::fx, FenXingChuLi::temp_fx;
float FenXingChuLi::comp_fx_gao, FenXingChuLi::comp_fx_di;
int FenXingChuLi::comp_fx_gao_count, FenXingChuLi::comp_fx_di_count;

FenXingChuLi::FenXingChuLi() {
    FenXing fx = FenXing();
    FenXingChuLi::status = FenXingChuLiStatus::LEFT;

    Kxian1 kx = {0};
    FenXingChuLi::left = kx;
    FenXingChuLi::middle = kx;
    FenXingChuLi::right = kx;
    FenXingChuLi::free = kx;
    FenXingChuLi::fx = fx;
    FenXingChuLi::temp_fx = fx;
}

FenXing FenXingChuLi::handle(vector<Kxian1>& kxianList) {
    FenXing fx;
    this->kxianList = kxianList;
    for (vector<Kxian1>::iterator iter = kxianList.begin(); iter != kxianList.end(); iter++) {
        fx = this->__find_fenxing((*iter));
        FenXingType fx_type = fx.get_type();
        if (fx_type != FenXingType::NONE) {
            switch (fx_type) {
            case FenXingType::VERIFY_BOTTOM:
                this->fx = fx;
                this->keyKxianList.push_back(fx);
                break;
            case FenXingType::VERIFY_TOP:
                this->fx = fx;
                this->keyKxianList.push_back(fx);
                break;
            case FenXingType::FAILURE_VERIFY_TOP:
                this->keyKxianList.pop_back();
                if (!this->keyKxianList.empty())
                    this->fx = this->keyKxianList.back();
                else
                    this->fx = FenXing();
                break;
            case FenXingType::FAILURE_VERIFY_BOTTOM:
                this->keyKxianList.pop_back();
                if (!this->keyKxianList.empty())
                    this->fx = this->keyKxianList.back();
                else
                    this->fx = FenXing();
                break;
            }
        this->fenXingList.push_back(fx);
        }
    }
    return(fx);
}

void OutputDebugPrintf(const char* strOutputString, ...)
{
    char strBuffer[4096] = { 0 };
    va_list vlArgs;
    va_start(vlArgs, strOutputString);
    _vsnprintf_s(strBuffer, sizeof(strBuffer) - 1, strOutputString, vlArgs);
    //vsprintf(strBuffer,strOutputString,vlArgs);
    va_end(vlArgs);
    OutputDebugString(strBuffer);
}

char* get_fenxing_status(FenXingChuLiStatus fx_type) {
    switch (fx_type) {
    case FenXingChuLiStatus::LEFT:
        return("LEFT");
    case FenXingChuLiStatus::MIDDLE:
        return("MIDDLE");
    case FenXingChuLiStatus::RIGHT:
        return("RIGHT");
    case FenXingChuLiStatus::TOP:
        return("TOP");
    case FenXingChuLiStatus::BOTTOM:
        return("BOTTOM");
    case FenXingChuLiStatus::FREE_NEW_TOP:
        return("FREE_NEW_TOP");
    case FenXingChuLiStatus::FREE_NEW_BOTTOM:
        return("FREE_NEW_BOTTOM");
    case FenXingChuLiStatus::VERIFY_TOP:
        return("VERIFY_TOP");
    case FenXingChuLiStatus::VERIFY_BOTTOM:
        return("VERIFY_BOTTOM");
    case FenXingChuLiStatus::VERIFY_NEW_TOP:
        return("VERIFY_NEW_TOP");
    case FenXingChuLiStatus::VERIFY_NEW_BOTTOM:
        return("VERIFY_NEW_BOTTOM");
    case FenXingChuLiStatus::BACK_ZHONGYIN_TOP:
        return("BACK_ZHONGSHUYIN_TOP");
    case FenXingChuLiStatus::BACK_ZHONGYIN_BOTTOM:
        return("BACK_ZHONGSHUYIN_BOTTOM");
    case FenXingChuLiStatus::AFTER_VERIFY_TOP:
        return("AFTER_VERIFY_TOP");
    case FenXingChuLiStatus::AFTER_VERIFY_BOTTOM:
        return("AFTER_VERIFY_BOTTOM");
    default:
        return("没有处理的Token");
    }
}

FenXing FenXingChuLi::__find_fenxing(Kxian1 kxian) {
    FenXing tmp_fx = FenXing();
    OutputDebugPrintf(" % s % f % f %d \n", get_fenxing_status(FenXingChuLi::status), kxian.gao, kxian.di, kxian.position);
    switch (FenXingChuLi::status) {
    case FenXingChuLiStatus::LEFT:
        tmp_fx = this->__last_fx_process(kxian);
        if (tmp_fx.get_type() == FenXingType::NONE) {
            this->left = kxian;
            this->status = FenXingChuLiStatus::MIDDLE;
        }
        break;

    case FenXingChuLiStatus::MIDDLE:
        tmp_fx = this->__last_fx_process(kxian);
        if (tmp_fx.get_type() == FenXingType::NONE) {
            this->middle = kxian;
            this->status = FenXingChuLiStatus::RIGHT;
        }
        break;

    case FenXingChuLiStatus::RIGHT:
        //分型处理
        tmp_fx = this->__right_process(kxian);
        break;

    case FenXingChuLiStatus::FREE:
        tmp_fx = this->__free_process(kxian);
        break;
    }
    this->last_bar = kxian;
    return(tmp_fx);
}

FenXing FenXingChuLi::__right_process(Kxian1 kxian){
    FenXing tmp_fx = FenXing();
    if (this->middle.gao > this->left.gao) {
        //middle.gao > left.gao
        if (kxian.gao > this->middle.gao) {
            //继续新高
            this->left = this->middle;
            this->middle = kxian;
            this->status = FenXingChuLiStatus::RIGHT;
            return(temp_fx);
        } else {
            //顶分型
            this->right = kxian;
            this->status = FenXingChuLiStatus::FREE;
            float gao = this->middle.gao;
            float di = min(this->left.di, this->right.di);
            this->comp_fx_gao = 0;
            this->comp_fx_gao_count = 0;
            this->comp_fx_di = di;
            this->comp_fx_di_count = 2;
            this->free = Kxian1();
            tmp_fx = FenXing(FenXingType::TOP, gao, di, this->left, this->middle, this->right, this->free);
            return(tmp_fx);
        }
    } else {
        //middle.gao < left.gao
        if (kxian.di < this->middle.di){
            //继续新低
            this->left = this->middle;
            this->middle = kxian;
            this->status = FenXingChuLiStatus::RIGHT;
            return(temp_fx);
        } else {
            //底分型
            this->right = kxian;
            this->status = FenXingChuLiStatus::FREE;
            float di = FenXingChuLi::middle.di;
            float gao = max(this->left.gao, this->right.gao);
            this->comp_fx_gao = gao;
            this->comp_fx_gao_count = 2;
            this->comp_fx_di = 0;
            this->comp_fx_di_count = 0;
            this->free = FenXing();
            tmp_fx = FenXing(FenXingType::BOTTOM, gao, di, this->left, this->middle, this->right, this->free);
            return(tmp_fx);
        }
    }
}

FenXing FenXingChuLi::__free_process(Kxian1 kxian) {
    FenXing tmp_fx = FenXing();
    if (this->temp_fx.get_type() == FenXingType::TOP) {
        //顶分型处理
        if (kxian.get_high() < this->temp_fx.get_low()) {
            //确认顶分型
            tmp_fx = this->temp_fx;
            tmp_fx.set_type(FenXingType::VERIFY_TOP);
            this->left = kxian;
            this->status = FenXingChuLiStatus::MIDELL;
        } else {
            if (kxian.get_low() < this->comp_fx_di) {
                if (kxian.get_high() < this->last_bar.get_low() + 0.01) {
                    //有缺口
                    this->comp_fx_di_count = 5;
                }
                if (this->comp_fx_di_count >= 5) {
                    tmp_fx = this->temp_fx;
                    tmp_fx.set_type(FenXingType::VERIFY_TOP);
                    this->left = this->last_bar;
                    this->middle = kxian;
                    this->status = FenXingType::RIGHT;
                } else {
                    this->comp_fx_di = kxian.get_low();
                    this->comp_fx_di_count++;
                }
            }
        }
    } else {
        //底分型处理
        if (kxian.get_low() > this->temp_fx.get_high()) {
            tmp_fx = this->temp_fx;
            tmp_fx.set_type(FenXingType::VERIFY_BOTTOM);
            this->left = kxian;
            this->status = FenXingChuLiStatus::MIDDLE;
        } else {
            if (kxian.get_high() > this->comp_fx_gao) {
                if (kxian.get_low() > this->temp_fx.get_high() + 0.01) {
                    this->comp_fx_gao_count = 5;
                }
                if (this->comp_fx_gao_count >= 5) {
                    tmp_fx = this->temp_fx;
                    tmp_fx.set_type(FenXingType::VERIFY_BOTTOM);
                    this->left = this->last_bar;
                    this->middle = kxian;
                    this->status = FenXingChuLiStatus::RIGHT;
                } else {
                    this->comp_fx_gao = kxian.get_high();
                    this->comp_fx_gao_count++;
                }
            }
        }
    }
    return(tmp_fx);
}


Kxian1 FenXingChuLi::__get_last_kxian() {
    Kxian1 last_kx = Kxian1();
    switch (FenXingChuLi::status) {
        case FenXingChuLiStatus::LEFT:
            last_kx = FenXingChuLi::free;
            break;
        case FenXingChuLiStatus::MIDDLE:
            last_kx = FenXingChuLi::left;
            break;
        case FenXingChuLiStatus::RIGHT:
            last_kx = FenXingChuLi::middle;
            break;
        case FenXingChuLiStatus::FREE_FIRST:
            last_kx = FenXingChuLi::right;
            break;
        case FenXingChuLiStatus::FREE:
            last_kx = FenXingChuLi::free;
            break;
    }
    return(last_kx);
}


FenXing FenXingChuLi::__last_fx_process(Kxian1 kxian) {
    FenXing tmp_fx = FenXing();

    if (this->last_fx.get_type() == FenXingType::NONE) {
        return(tmp_fx);
    }

    if (this->fx.get_type() == FenXingType::VERIFY_TOP) {
        if (kxian.get_high() > this->fx.get_high()) {
            //创新高
            tmp_fx = this->fx;
            tmp_fx.set_type(FenXingType::FAILURE_VERIFY_TOP);
            this->left = this->last_bar;
            this->middle = kxian;
            this->status = FenXingChuLiStatus::RIGHT;
        }
    } else {
        if (this->fx.get_type() == FenXingType::VERIFY_BOTTOM) {
            if (kxian.get_low() < this->fx.get_low()) {
                //创新低
                tmp_fx = this->fx;
                tmp_fx.set_type(FenXingType::FAILURE_VERIFY_BOTTOM);
                this->left = this->last_bar;
                this->middle = kxian;
                this->status = FenXingChuLiStatus::RIGHT;
            }
        }
    }
    return(tmp_fx);
}

void FenXingChuLi::__set_fx(FenXing fx) {
    FenXingChuLi::fx = fx;
}

void FenXingChuLi::__set_temp_fx(FenXing fx) {
    FenXingChuLi::temp_fx = fx;
}
FenXing  FenXingChuLi::__get_last_fx(Kxian1 kxian) {
    FenXing fx = FenXing();
    if (this->fenXingList.size() > 1) {
        fx = this->fenXingList.back();
        this->fenXingList.pop_back();
        if (fx.get_type() == FenXingType::VERIFY_TOP)
            fx.set_type(FenXingType::FAILURE_TOP);
        else {
            if (fx.get_type() == FenXingType::VERIFY_BOTTOM)
                fx.set_type(FenXingType::FAILURE_BOTTOM);
        }
        this->fenXingList.push_back(fx);


        for (int num = this->fenXingList.size() - 1; num > 0; num--) {
            if (this->fenXingList[num].get_type() == FenXingType::VERIFY_BOTTOM || this->fenXingList[num].get_type() == FenXingType::VERIFY_TOP) {
                this->__set_fx(this->fenXingList[num]);
                break;
            }
        }
    }

    return(fx);
}

Kxian1 FenXingChuLi::get_kx_item(string pos) {
    Kxian1 kx;
    if (pos == "left") {
        kx = FenXingChuLi::left;
    } else if (pos == "middle") {
        kx = FenXingChuLi::middle;
    } else if (pos == "right") {
        kx = FenXingChuLi::right;
    } else if (pos == "free") {
        kx = FenXingChuLi::free;
    } else {
        kx = {0};
    }
    return kx;
}

