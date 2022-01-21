﻿#include "FenXingChuLi.h"
#include "BaoHanChuLi.h"
#include "OutputDebug.h"

using namespace std;

bool debug_fenxing_status = true;
FenXingChuLiStatus FenXingChuLi::status;
Kxian1 FenXingChuLi::one, FenXingChuLi::two, FenXingChuLi::three, FenXingChuLi::four,  FenXingChuLi::five, FenXingChuLi::six, FenXingChuLi::last_bar;
FenXing FenXingChuLi::fx, FenXingChuLi::temp_fx, FenXingChuLi::last_fx;
float FenXingChuLi::comp_fx_gao, FenXingChuLi::comp_fx_di;
int FenXingChuLi::comp_fx_gao_count, FenXingChuLi::comp_fx_di_count;
float FenXingChuLi::max_high, FenXingChuLi::min_low;

FenXingChuLi::FenXingChuLi() {
    FenXing fx = FenXing();
    FenXingChuLi::status = FenXingChuLiStatus::ONE;

    Kxian1 kx = Kxian1();
    this->one = kx;
    this->two = kx;
    this->three = kx;
    this->four = kx;
    this->five = kx;
    this->six = kx;

    this->fx = fx;
    this->temp_fx = fx;
    this->last_fx = FenXing();
    this->max_high = 0.0;
    this->min_low = 0.0;
    this->last_bar = Kxian1();
}

void debug_fenxing(FenXing fx) {
    FenXingType fx_type = fx.get_type();
    switch (fx_type) {
    case FenXingType::NONE:
        break;
    case FenXingType::TOP:
        OutputDebugPrintf("【顶分型】 %f %d", fx.get_high(), fx.get_start_position());
        break;
    case FenXingType::FAILURE_TOP:
        OutputDebugPrintf("【顶分型失败】%f %d", fx.get_high(), fx.get_stop_position());
        break;
    case FenXingType::VERIFY_TOP:
        OutputDebugPrintf("【验证顶分型】%f %d", fx.get_high(), fx.get_stop_position());
        break;
    case FenXingType::FAILURE_VERIFY_TOP:
        OutputDebugPrintf("【验证顶分型失败】%f %d", fx.get_high(), fx.get_stop_position());
        break;
    case FenXingType::BOTTOM:
        OutputDebugPrintf("【底分型】 %f %d", fx.get_low(), fx.get_start_position());
        break;
    case FenXingType::FAILURE_BOTTOM:
        OutputDebugPrintf("【底分型失败】%f %d", fx.get_low(), fx.get_stop_position());
        break;
    case FenXingType::VERIFY_BOTTOM:
        OutputDebugPrintf("【验证底分型】 %f %d", fx.get_low(), fx.get_stop_position());
        break;
    case FenXingType::FAILURE_VERIFY_BOTTOM:
        OutputDebugPrintf("【验证底分型失败】 %f %d", fx.get_low(), fx.get_stop_position());
        break;
    }
}

void FenXingChuLi::handle(vector<Kxian1>& kxianList) {
    FenXing tmp_fx;
    FenXingType fx_type;

    this->kxianList = kxianList;
    for (vector<Kxian1>::iterator iter = kxianList.begin(); iter != kxianList.end(); iter++) {
        tmp_fx = this->__find_fenxing((*iter));
        fx_type = tmp_fx.get_type();
        if (fx_type != FenXingType::NONE) {
            if (debug_fenxing_status)
                debug_fenxing(tmp_fx);
            switch (fx_type) {
            case FenXingType::TOP:
                this->temp_fx = tmp_fx;
                break;
            case FenXingType::FAILURE_TOP:
                this->temp_fx = FenXing();
                break;
            case FenXingType::BOTTOM:
                this->temp_fx = tmp_fx;
                break;
            case FenXingType::FAILURE_BOTTOM:
                this->temp_fx = FenXing();
                break;
            case FenXingType::VERIFY_BOTTOM:
                if (this->keyFenXingList.empty()) {
                    this->keyFenXingList.push_back(tmp_fx);
                } else {
                    FenXing last_fx = this->keyFenXingList.back();
                    if (last_fx.get_type() == FenXingType::VERIFY_TOP) {
                        this->keyFenXingList.push_back(tmp_fx);
                    } else {
                        //上一个分型也为底分型
                        if (last_fx.get_low() < tmp_fx.get_low()) {
                            tmp_fx = FenXing();
                        }
                        else {
                            if (!this->keyFenXingList.empty()) {
                                this->keyFenXingList.pop_back();
                                this->keyFenXingList.push_back(tmp_fx);
                            }
                        }
                    }
                }
                if (tmp_fx.get_type() != FenXingType::NONE) {
                    this->fx = tmp_fx;
                    this->fenXingList.push_back(tmp_fx);
                }
                break;
            case FenXingType::VERIFY_TOP:
                if (this->keyFenXingList.empty()) {
                    this->keyFenXingList.push_back(tmp_fx);
                }
                else {
                    FenXing last_fx = this->keyFenXingList.back();
                    if (last_fx.get_type() == FenXingType::VERIFY_BOTTOM) {
                        this->keyFenXingList.push_back(tmp_fx);
                    } else {
                        //上一个分型也为顶分型
                        if (last_fx.get_high() > tmp_fx.get_high()) {
                            tmp_fx = FenXing();
                        }
                        else {
                            if (!this->keyFenXingList.empty()) {
                                this->keyFenXingList.pop_back();
                                this->keyFenXingList.push_back(tmp_fx);
                            }
                        }
                    }
                }
                if (tmp_fx.get_type() != FenXingType::NONE) {
                    this->fx = tmp_fx;
                    this->fenXingList.push_back(tmp_fx);
                }
                break;

            case FenXingType::FAILURE_VERIFY_TOP:
                if (!this->keyFenXingList.empty()) {
                    this->keyFenXingList.pop_back();
                    if (!this->keyFenXingList.empty())
                        this->fx = this->keyFenXingList.back();
                }
                else
                    this->fx = FenXing();
                break;

            case FenXingType::FAILURE_VERIFY_BOTTOM:
                if (!this->keyFenXingList.empty()) {
                    this->keyFenXingList.pop_back();
                    if (!this->keyFenXingList.empty())
                        this->fx = this->keyFenXingList.back();
                }
                else
                    this->fx = FenXing();
                break;
            }
            if (tmp_fx.get_type() != FenXingType::NONE)
                this->fenXingList.push_back(tmp_fx);
        }
    }

    if (!this->keyFenXingList.empty()) {
        FenXing start_fx = this->keyFenXingList.back();
        int count = this->fenXingList.size();
        int start_num = 0;
        for (int i = 0; i < count; i++) {
            if (this->fenXingList[i] == start_fx) {
                start_num = i;
                break;
            }
        }
        for (int i = start_num + 1; i < count; i++) {
            FenXing fx = this->fenXingList[i];
            this->keyFenXingList.push_back(fx);
        }
    }
}

char* get_fenxing_status(FenXingChuLiStatus fx_type) {
    switch (fx_type) {
    case FenXingChuLiStatus::ONE:
        return("ONE");
    case FenXingChuLiStatus::TWO:
        return("TWO");
    case FenXingChuLiStatus::THREE:
        return("THREE");
    case FenXingChuLiStatus::FOUR:
        return("FOUR");
    case FenXingChuLiStatus::FIVE:
        return("FIVE");
    case FenXingChuLiStatus::SIX:
        return("SIX");
    default:
        return("没有处理的Token");
    }
}

void FenXingChuLi::__init_fenxing(Kxian1 kx) {
    this->one = this->last_bar;
    this->two = kx;
    this->status = FenXingChuLiStatus::THREE;
}

void FenXingChuLi::__high_process(Kxian1 kxian) {
    this->__init_fenxing(kxian);
    this->max_high = kxian.get_high();
}

void FenXingChuLi::__low_process(Kxian1 kxian){
    this->__init_fenxing(kxian);
    this->min_low = kxian.get_low();
    FenXing tmp_fx = FenXing();
}

void FenXingChuLi::__high_low_process(Kxian1 kxian){
    if (kxian.get_high() > this->max_high) {
        this->__high_process(kxian);
    } else {
        if (kxian.get_low() < this->min_low) {
            this->__low_process(kxian);
        }
    }
}

FenXing FenXingChuLi::__find_fenxing(Kxian1 kxian) {
    FenXing tmp_fx = FenXing();

    if (debug_fenxing_status)
        OutputDebugPrintf(" % s % f % f %d ", get_fenxing_status(FenXingChuLi::status), kxian.get_high(), kxian.get_low(), kxian.get_position());

    if (this->last_bar.get_high() == 0) {
        this->last_bar = kxian;
        this->one = kxian;
        this->max_high = kxian.get_high();
        this->min_low = kxian.get_low();
        this->status = FenXingChuLiStatus::TWO;
        return(tmp_fx);
    }

    switch (FenXingChuLi::status) {
    case FenXingChuLiStatus::ONE:
        this->one = kxian;
        this->status = FenXingChuLiStatus::TWO;
        break;

    case FenXingChuLiStatus::TWO:
        if (kxian.get_high() > this->max_high || kxian.get_low() < this->min_low) {
            this->__high_low_process(kxian);
        } else {
            this->two = kxian;
            this->status = FenXingChuLiStatus::THREE;
        }
        break;

    case FenXingChuLiStatus::THREE:
        //分型处理
        if (this->two.get_high() > this->one.get_high()) {
            //middle 大于 left
            if (kxian.get_high() > this->two.get_high()) {
                //上升趋势
                if (kxian.get_high() > this->max_high) {
                    //创新高
                    this->__high_process(kxian);
                } else {
                    this->__init_fenxing(kxian);
                }
            }
            else {
                //顶分型
                if (kxian.get_low() < this->min_low) {
                    //创新低
                    this->__low_process(kxian);
                } else {
                    tmp_fx = FenXing( this->one, this->two, kxian, kxian);
                    this->three = kxian;
                    this->status = FenXingChuLiStatus::FOUR;
                }
            }
        }
        else {
            //middle 小于 left
            if (kxian.get_low() < this->two.get_low()) {
                //下降趋势
                if (kxian.get_low() < this->min_low) {
                    //创新低
                    this->__low_process(kxian);
                } else {
                    this->__init_fenxing(kxian);
                }
            }
            else {
                //底分型
                if (kxian.get_high() > this->max_high) {
                    //创新高
                    this->__high_process(kxian);
                } else {
                    tmp_fx = FenXing(this->one, this->two, kxian, kxian);
                    this->three = kxian;
                    this->status = FenXingChuLiStatus::FOUR;
                }
            }
        }
        break;

    case FenXingChuLiStatus::FOUR:
        if (this->temp_fx.get_type() == FenXingType::TOP) {
            //顶分型处理
            if (kxian.get_low() < this->min_low) {
                //最低
                this->__low_process(kxian);
                tmp_fx = this->temp_fx;
                tmp_fx.set_type(FenXingType::FAILURE_TOP);
            } else {
                if (kxian.get_high() > this->temp_fx.get_high()) {
                    if (kxian.get_high() > this->max_high) {
                        //最高
                        this->__high_process(kxian);
                    } else {
                        //创新高
                        this->__init_fenxing(kxian);
                    }
                    tmp_fx = this->temp_fx;
                    tmp_fx.set_type(FenXingType::FAILURE_TOP);
                }
                else {
                    if (kxian.get_low() < this->three.get_low()) {
                        if (kxian.get_high() < this->three.get_low() - 0.01) {
                            tmp_fx = this->temp_fx;
                            tmp_fx.set_type(FenXingType::VERIFY_TOP);
                            this->__init_fenxing(kxian);
                        } else {
                            this->four = kxian;
                            this->status = FenXingChuLiStatus::FIVE;
                        }
                    }
                }
            }
        }
        else {
            if (this->temp_fx.get_type() == FenXingType::BOTTOM) {
                //底分型处理
                if (kxian.get_high() > this->max_high) {
                    //最高
                    this->__high_process(kxian);
                    tmp_fx = this->temp_fx;
                    tmp_fx.set_type(FenXingType::FAILURE_BOTTOM);
                } else {
                    if (kxian.get_low() < this->temp_fx.get_low()) {
                        if (kxian.get_low() < this->min_low) {
                            //最低
                            this->__low_process(kxian);
                        } else {
                            //创新低
                            this->__init_fenxing(kxian);
                        }
                        tmp_fx = this->temp_fx;
                        tmp_fx.set_type(FenXingType::FAILURE_BOTTOM);
                    }
                    else {
                        if (kxian.get_high() > this->three.get_high()) {
                            if (kxian.get_low() > this->three.get_high() + 0.01) {
                                //有缺口
                                tmp_fx = this->temp_fx;
                                tmp_fx.set_type(FenXingType::VERIFY_BOTTOM);
                                this->__init_fenxing(kxian);
                            } else {
                                this->four = kxian;
                                this->status = FenXingChuLiStatus::FIVE;
                            }
                        }
                    }
                }
            }
        }
        break;

    case FenXingChuLiStatus::FIVE:
        if (this->temp_fx.get_type() == FenXingType::TOP) {
            //顶分型
            if (kxian.get_low() < this->min_low) {
                //最低
                this->__low_process(kxian);
                tmp_fx = this->temp_fx;
                tmp_fx.set_type(FenXingType::FAILURE_TOP);
            } else {
                if (kxian.get_high() > this->temp_fx.get_high()) {
                    if (kxian.get_high() > this->max_high) {
                        //最高
                        this->__high_process(kxian);
                    } else {
                        //创新高
                        this->__init_fenxing(kxian);
                    }
                    tmp_fx = this->temp_fx;
                    tmp_fx.set_type(FenXingType::FAILURE_TOP);
                } else {
                    if (kxian.get_low() < this->four.get_low()) {
                        if (kxian.get_high() < this->four.get_low() - 0.01) {
                            //有缺口
                            tmp_fx = this->temp_fx;
                            tmp_fx.set_type(FenXingType::VERIFY_TOP);
                            this->__init_fenxing(kxian);
                        } else {
                            this->five = kxian;
                            this->status = FenXingChuLiStatus::SIX;
                        }

                    }
                }
            }
        } else {
            if (this->temp_fx.get_type() == FenXingType::BOTTOM) {
                //底分型
                if (kxian.get_high() > this->max_high) {
                    //最高
                    this->__high_process(kxian);
                    tmp_fx = this->temp_fx;
                    tmp_fx.set_type(FenXingType::FAILURE_BOTTOM);
                } else {
                    if (kxian.get_low() < this->temp_fx.get_low()) {
                        if (kxian.get_low() < this->min_low) {
                            //最低
                            this->__low_process(kxian);
                        } else {
                            this->__init_fenxing(kxian);
                        }
                        tmp_fx = this->temp_fx;
                        temp_fx.set_type(FenXingType::FAILURE_BOTTOM);
                    } else {
                        if (kxian.get_high() > this->four.get_high()) {
                            if (kxian.get_low() > this->four.get_high() + 0.01) {
                                tmp_fx = this->temp_fx;
                                tmp_fx.set_type(FenXingType::VERIFY_BOTTOM);
                                this->__init_fenxing(kxian);
                            } else{
                                this->five = kxian;
                                this->status = FenXingChuLiStatus::SIX;
                            }
                        }
                    }
                }

            }
        }
        break;

    case FenXingChuLiStatus::SIX:
        if (this->temp_fx.get_type() == FenXingType::TOP) {
            //顶分型
            if (kxian.get_low() < this->min_low) {
                //最低
                this->__low_process(kxian);
                tmp_fx = this->temp_fx;
                tmp_fx.set_type(FenXingType::VERIFY_TOP);

            } else {
                if (kxian.get_high() > this->temp_fx.get_high()) {
                    if (kxian.get_high() > this->max_high) {
                        this->__high_process(kxian);
                    } else {
                        this->__init_fenxing(kxian);
                    }
                    tmp_fx = this->temp_fx;
                    tmp_fx.set_type(FenXingType::FAILURE_TOP);
                } else {
                    if (kxian.get_low() < this->five.get_low()) {
                        tmp_fx = FenXing(this->one, this->two, this->three, kxian);
                        tmp_fx.set_type(FenXingType::VERIFY_TOP);
                        this->__init_fenxing(kxian);
                    }


                }
            }
        }
        else {
            if (this->temp_fx.get_type() == FenXingType::BOTTOM) {
                //底分型
                if (kxian.get_high() > this->max_high) {
                    //最高
                    this->__high_process(kxian);
                    tmp_fx = this->temp_fx;
                    tmp_fx.set_type(FenXingType::VERIFY_BOTTOM);
                } else {
                    if (kxian.get_low() < this->temp_fx.get_low()) {
                        if (kxian.get_low() < this->min_low) {
                            //最低
                            this->__low_process(kxian);
                        } else {
                            //创新低
                            this->__init_fenxing(kxian);
                        }
                        tmp_fx = this->temp_fx;
                        tmp_fx.set_type(FenXingType::FAILURE_BOTTOM);
                    } else {
                        if (kxian.get_high() > this->five.get_high()) {
                            tmp_fx = FenXing(this->one, this->two, this->three, kxian);
                            tmp_fx.set_type(FenXingType::VERIFY_BOTTOM);
                            this->__init_fenxing(kxian);
                        }
                    }
                }
            }
        }
        break;
    }

    this->last_bar = kxian;
    return(tmp_fx);
}


void Bi3_fenxing(int nCount, float* pOut, float* pHigh, float* pLow, float* pIn) {
    BaoHanChuLi baohanChuli;
    for (int i = 0; i < nCount; i++) {
        baohanChuli.add(pHigh[i], pLow[i]);
    }

    FenXingChuLi fenXingChuLi;
    fenXingChuLi.handle(baohanChuli.kxianList);

    for (int i = 0; i < nCount; i++) {
        pOut[i] = 0;
    }

    int position_start, position_stop;
    unsigned int count = fenXingChuLi.keyFenXingList.size();
    for (unsigned int i = 0; i < count; i++) {
        FenXing fx = fenXingChuLi.keyFenXingList[i];
        position_start = fx.get_start_position();
        position_stop = fx.get_stop_position();
        switch (fx.get_type()) {
        case FenXingType::BOTTOM:
            pOut[position_start] = -1;
            break;
        case FenXingType::FAILURE_BOTTOM:
            pOut[position_start] = -2;
            break;
        case FenXingType::VERIFY_BOTTOM:
            pOut[position_start] = -3;
            break;
        case FenXingType::FAILURE_VERIFY_BOTTOM:
            pOut[position_start] = -4;
            break;
        case FenXingType::TOP:
            pOut[position_start] = 1;
            break;
        case FenXingType::FAILURE_TOP:
            pOut[position_start] = 2;
            break;
        case FenXingType::VERIFY_TOP:
            pOut[position_start] = 3;
            break;
        case FenXingType::FAILURE_VERIFY_TOP:
            pOut[position_start] = 4;
            break;
        }
    }
}

void Bi4_fenxing(int nCount, float* pOut, float* pHigh, float* pLow, float* pIn) {
    BaoHanChuLi baohanChuli;
    for (int i = 0; i < nCount; i++) {
        baohanChuli.add(pHigh[i], pLow[i]);
    }

    FenXingChuLi fenXingChuLi;
    fenXingChuLi.handle(baohanChuli.kxianList);

    for (int i = 0; i < nCount; i++) {
        pOut[i] = 0;
    }

    int position_start, position_stop;
    unsigned int count = fenXingChuLi.keyFenXingList.size();
    for (unsigned int i = 0; i < count; i++) {
        FenXing fx = fenXingChuLi.keyFenXingList[i];
        position_start = fx.get_start_position();
        position_stop = fx.get_stop_position();
        switch (fx.get_type()) {
        case FenXingType::BOTTOM:
            pOut[position_start] = -1;
            break;
        case FenXingType::FAILURE_BOTTOM:
            pOut[position_start] = -2;
            break;
        case FenXingType::VERIFY_BOTTOM:
            pOut[position_stop] = -3;
            break;
        case FenXingType::FAILURE_VERIFY_BOTTOM:
            pOut[position_stop] = -4;
            break;
        case FenXingType::TOP:
            pOut[position_start] = 1;
            break;
        case FenXingType::FAILURE_TOP:
            pOut[position_start] = 2;
            break;
        case FenXingType::VERIFY_TOP:
            pOut[position_stop] = 3;
            break;
        case FenXingType::FAILURE_VERIFY_TOP:
            pOut[position_stop] = 4;
            break;
        }
    }
}
