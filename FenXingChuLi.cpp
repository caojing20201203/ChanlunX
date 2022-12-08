#include "FenXingChuLi.h"
#include "BaoHanChuLi.h"
#include "OutputDebug.h"

using namespace std;

bool debug_fenxing_status = true;


float FenXingChuLi::comp_fx_gao, FenXingChuLi::comp_fx_di;
int FenXingChuLi::comp_fx_gao_count, FenXingChuLi::comp_fx_di_count;


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
    this->firstBarStatus = true;
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
            case FenXingType::NEW_BOTTOM:
                if (!this->keyFenXingList.empty()) {
                    this->keyFenXingList.pop_back();
                }
                tmp_fx.set_type(FenXingType::BOTTOM);
                this->temp_fx = tmp_fx;
                break;

            case FenXingType::NEW_TOP:
                if (!this->keyFenXingList.empty()) {
                    this->keyFenXingList.pop_back();
                }
                tmp_fx.set_type(FenXingType::TOP);
                this->temp_fx = tmp_fx;
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
            switch (fx.get_type()) {
            case FenXingType::BOTTOM:
                this->keyFenXingList.push_back(fx);
                break;
            case FenXingType::TOP:
                this->keyFenXingList.push_back(fx);
                break;
            case FenXingType::FAILURE_BOTTOM:
                if (!this->keyFenXingList.empty())
                    this->keyFenXingList.pop_back();
                break;
            case FenXingType::FAILURE_TOP:
                if (!this->keyFenXingList.empty())
                    this->keyFenXingList.pop_back();
                break;
            }
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
    case FenXingChuLiStatus::LOWHIGH:
        return("LOWHIGH");
    case FenXingChuLiStatus::HIGHLOW:
        return("HIGHLOW");
    default:
        return("没有处理的Token");
    }
}

void FenXingChuLi::__init_fenxing(Kxian1 kx) {
    this->one = this->last_bar;
    this->two = kx;
    this->status = FenXingChuLiStatus::THREE;
}

FenXing FenXingChuLi::__high_process(Kxian1 kxian) {
    FenXing tmp_fx = FenXing();
    if (this->fx.get_type() == FenXingType::VERIFY_TOP) {
        tmp_fx = this->fx;
        tmp_fx.set_type(FenXingType::FAILURE_VERIFY_TOP);
    } else {
        if (this->temp_fx.get_type() == FenXingType::TOP) {
            tmp_fx = this->temp_fx;
            tmp_fx.set_type(FenXingType::FAILURE_TOP);
        }
        else {
            tmp_fx = this->temp_fx;
            tmp_fx.set_free(kxian);
            tmp_fx.set_type(FenXingType::VERIFY_BOTTOM);
        }
    }
    this->__init_fenxing(kxian);
    this->max_high = kxian.get_high();
    return(tmp_fx);
}

FenXing FenXingChuLi::__low_process(Kxian1 kxian){
    FenXing tmp_fx = FenXing();
    if (this->fx.get_type() == FenXingType::VERIFY_BOTTOM) {
        tmp_fx = this->fx;
        tmp_fx.set_type(FenXingType::FAILURE_VERIFY_BOTTOM);
    } else {
        if (this->temp_fx.get_type() == FenXingType::BOTTOM) {
            tmp_fx = this->temp_fx;
            tmp_fx.set_type(FenXingType::FAILURE_BOTTOM);
        }
        else {
            tmp_fx = this->temp_fx;
            tmp_fx.set_free(kxian);
            tmp_fx.set_type(FenXingType::VERIFY_TOP);
        }
    }

    this->__init_fenxing(kxian);
    this->min_low = kxian.get_low();
    return(tmp_fx);
}

FenXing FenXingChuLi::__high_low_process(Kxian1 kxian){
    FenXing tmp_fx = FenXing();
    if (kxian.get_high() > this->max_high) {
        tmp_fx = this->__high_process(kxian);
    } else {
        if (kxian.get_low() < this->min_low) {
            tmp_fx = this->__low_process(kxian);
        }
    }
    return(tmp_fx);
}

FenXing FenXingChuLi::__failure_verify_top(Kxian1 kxian) {
    FenXing tmp_fx = FenXing();
    this->one = this->two;
    this->two = kxian;
    tmp_fx = this->fx;
    tmp_fx.set_type(FenXingType::FAILURE_VERIFY_TOP);
    return(tmp_fx);
}

FenXing FenXingChuLi::__failue_verify_bottom(Kxian1 kxian) {
    FenXing tmp_fx = FenXing();
    this->one = this->last_bar;
    this->two = kxian;
    tmp_fx = this->fx;
    tmp_fx.set_type(FenXingType::FAILURE_VERIFY_BOTTOM);
    return(tmp_fx);
}

FenXing FenXingChuLi::__determ_fenxing(Kxian1 kx) {
    FenXing tmp_fx = FenXing();

    if (this->temp_fx.get_type() == FenXingType::TOP) {
        //顶分型
        if (kx.get_low() < this->temp_fx.get_low()) {
            tmp_fx = FenXing(this->one, this->two, this->three, kx);
            tmp_fx.set_type(FenXingType::VERIFY_TOP);
        }
    }
    else {
        if (this->temp_fx.get_type() == FenXingType::BOTTOM) {
            //底分型
            if (kx.get_high() > this->temp_fx.get_high()) {
                tmp_fx = FenXing(this->one, this->two, this->three, kx);
                tmp_fx.set_type(FenXingType::VERIFY_BOTTOM);
            }
        }
    }
    return(tmp_fx);
}

FenXing  FenXingChuLi::__fenxing_process(Kxian1 kx) {
    FenXing tmp_fx = FenXing();
    if (this->two.get_high() > this->one.get_high()) {
        if (kx.get_high() > this->two.get_high()) {
            //上升趋势
        }
        else {
            //顶分型
        }
    }
    else {
        if (kx.get_low() < this->two.get_low()) {
            //下降趋势
        } else{
            //底分型
        }
    }
    return(tmp_fx);
}


FenXing FenXingChuLi::__kxian_process(Kxian1 kx){
    FenXing tmp_fx;
    float comp_price = 0.0;

    if (this->temp_fx.get_type() == FenXingType::TOP) {
        comp_price = this->three.get_low();
        switch (this->status) {
        case FenXingChuLiStatus::FOUR:
            comp_price = this->three.get_low();
            break;
        case FenXingChuLiStatus::FIVE:
            comp_price = this->four.get_low();
            break;
        }
        if (kx.get_high() < comp_price - 0.01) {
            //有缺口
            tmp_fx = this->temp_fx;
            tmp_fx.set_free(kx);
            tmp_fx.set_type(FenXingType::VERIFY_TOP);
            this->__init_fenxing(kx);
        }
        else {
            if (kx.get_high() < this->temp_fx.get_low()) {
                tmp_fx = this->temp_fx;
                tmp_fx.set_free(kx);
                tmp_fx.set_type(FenXingType::VERIFY_TOP);
                this->__init_fenxing(kx);
            }
            else {
                switch (this->status) {
                case FenXingChuLiStatus::FOUR:
                    this->four = kx;
                    this->status = FenXingChuLiStatus::FIVE;
                    break;
                case FenXingChuLiStatus::FIVE:
                    this->five = kx;
                    this->status = FenXingChuLiStatus::SIX;
                    break;
                }

            }
        }
    }
    else {
        if (this->temp_fx.get_type() == FenXingType::BOTTOM) {
            comp_price = this->three.get_high();
            switch (this->status) {
            case FenXingChuLiStatus::FOUR:
                comp_price = this->three.get_high();
                break;
            case FenXingChuLiStatus::FIVE:
                comp_price = this->four.get_high();
                break;
            }
            if (kx.get_low() > comp_price + 0.01) {
                //有缺口
                tmp_fx = this->temp_fx;
                tmp_fx.set_free(kx);
                tmp_fx.set_type(FenXingType::VERIFY_BOTTOM);
                this->__init_fenxing(kx);
            }
            else {
                if (kx.get_low() >= this->temp_fx.get_high()) {
                    tmp_fx = this->temp_fx;
                    tmp_fx.set_free(kx);
                    tmp_fx.set_type(FenXingType::VERIFY_BOTTOM);
                    this->__init_fenxing(kx);
                }
                else {
                    switch (this->status) {
                    case FenXingChuLiStatus::FOUR:
                        this->four = kx;
                        this->status = FenXingChuLiStatus::FIVE;
                        break;
                    case FenXingChuLiStatus::FIVE:
                        this->five = kx;
                        this->status = FenXingChuLiStatus::SIX;
                        break;
                    case FenXingChuLiStatus::SIX:
                        break;
                    }
                }
            }
        }
    }
    return(tmp_fx);
}

FenXing FenXingChuLi::__find_fenxing(Kxian1 kxian) {
    FenXing tmp_fx = FenXing();
    float kx_gao = kxian.get_high();
    float kx_di = kxian.get_low();
    Kxian1 first_kx;

    if (debug_fenxing_status)
        OutputDebugPrintf(" % s % f % f %d ", get_fenxing_status(FenXingChuLi::status), kxian.get_high(), kx_di, kxian.get_position());

    if (this->last_bar.get_high() == 0) {
        this->last_bar = kxian;
        this->one = kxian;
        this->max_high = kx_gao;
        this->min_low = kx_di;
        this->status = FenXingChuLiStatus::TWO;
        return(tmp_fx);
    }

    switch (FenXingChuLi::status) {
    case FenXingChuLiStatus::START:
        if (this->one.get_high() == 0) {
            this->one = kxian;
        }
        else {
            if (this->two.get_high() == 0) {
                this->two = kxian;
            }
            else {
                if (this->two.get_high() > this->one.get_high()) {
                    if (kx_gao > this->two.get_high()) {
                        //上升趋势
                        float first_kx_gao = float(this->one.get_high() + 0.01);
                        float first_kx_di = float(this->one.get_low() + 0.01);
                        first_kx = Kxian1(first_kx_gao, first_kx_di, Direction::UP, -1);
                        tmp_fx = FenXing(first_kx, this->one, this->two, kxian);
                        this->four = kxian;
                        this->three = this->two;
                        this->two = this->one;
                        this->one = first_kx;
                        this->status = FenXingChuLiStatus::FIVE;
                    }
                    else {
                        //顶分型
                        this->three = kxian;
                        tmp_fx = FenXing(this->one, this->two, kxian, kxian);
                        this->status = FenXingChuLiStatus::FOUR;
                    }
                }
                else {
                    if (kx_di < this->two.get_low()) {
                        //下降趋势
                        float first_kx_gao = float(this->one.get_high() - 0.01);
                        float first_kx_di = float(this->one.get_low() - 0.01);
                        first_kx = Kxian1(first_kx_gao, first_kx_di, Direction::DOWN, -1);
                        tmp_fx = FenXing(first_kx, this->one, this->two, kxian);
                        this->four = kxian;
                        this->three = this->two;
                        this->two = this->one;
                        this->one = first_kx;
                        this->status = FenXingChuLiStatus::FIVE;

                    }
                    else {
                        //底分型
                        this->three = kxian;
                        tmp_fx = FenXing(this->one, this->two, kxian, kxian);
                        this->status = FenXingChuLiStatus::FOUR;
                    }
                }
            }
        }
        break;
    case FenXingChuLiStatus::ONE:
        this->one = kxian;
        this->status = FenXingChuLiStatus::TWO;
        break;

    case FenXingChuLiStatus::TWO:
        if (kx_gao > this->max_high) {
            this->max_high = kx_gao;
        } else {
            if (kx_di < this->min_low) {
                this->min_low = kx_di;
            }
        }
        /*
        if (this->firstBarStatus == true) {
            Kxian1 tmpKx;
            float tmpKx_gao, tmpKx_di;
            if (kx_gao > this->one.get_high()) {
                tmpKx_gao = this->one.get_high() + 0.1;
                tmpKx_di = this->one.get_low() + 0.1;
                tmpKx = Kxian1(tmpKx_gao, tmpKx_di, Direction::DOWN, 0);
                tmp_fx = FenXing(tmpKx, this->one, kxian, kxian);
                tmp_fx.set_type(FenXingType::VERIFY_BOTTOM);
            }
            else {
                tmpKx_gao = this->one.get_high() - 0.1;
                tmpKx_di = this->one.get_low() - 0.1;
                tmpKx = Kxian1(tmpKx_gao, tmpKx_di, Direction::UP, 0);
                tmp_fx = FenXing(tmpKx, this->one, kxian, kxian);
                tmp_fx.set_type(FenXingType::VERIFY_TOP);
            }
            this->firstBarStatus = false;
            this->status = FenXingChuLiStatus::THREE;
        }
        else {
            this->two = kxian;
            this->status = FenXingChuLiStatus::THREE;
        }
        */
        this->two = kxian;
        this->status = FenXingChuLiStatus::THREE;
        break;

    case FenXingChuLiStatus::THREE:
        if (this->two.get_high() > this->one.get_high()) {
            //第2根K线高于第1根
            if (kx_di < this->min_low) {
                if (this->two.get_high() == this->max_high) {
                    this->three = kxian;
                    this->min_low = kx_di;
                    this->status = FenXingChuLiStatus::HIGHLOW;
                }
                else {
                    this->min_low = kx_di;
                    if (this->fx.get_type() == FenXingType::VERIFY_BOTTOM) {
                        tmp_fx = this->fx;
                        tmp_fx.set_type(FenXingType::FAILURE_VERIFY_BOTTOM);
                    }
                    this->__init_fenxing(kxian);
                }
            }
            else {
                if (kx_gao > this->two.get_high()) {
                    //上升趋势
                    if (kx_gao > this->max_high)
                        this->max_high = kx_gao;
                    if (this->fx.get_type() == FenXingType::VERIFY_TOP && kx_gao > this->fx.get_high()) {
                        //比前高还高
                        tmp_fx = this->fx;
                        tmp_fx.set_type(FenXingType::FAILURE_VERIFY_TOP);
                    }
                    if (this->firstBarStatus) {
                        this->four = kxian;
                        this->three = this->two;
                        this->two = this->one;
                        float tmpKx_gao = this->one.get_high() + 0.1;
                        float tmpKx_di = this->one.get_low() + 0.1;
                        this->one = Kxian1(tmpKx_gao, tmpKx_di, Direction::DOWN, 0);
                        tmp_fx = FenXing(this->one, this->two, this->three, kxian);
                        this->status = FenXingChuLiStatus::FIVE;
                        this->firstBarStatus = false;
                    } else 
                        this->__init_fenxing(kxian);
                }
                else {
                    //顶分型
                    if (kx_di < this->min_low) {
                        this->min_low = kx_di;
                        if (this->fx.get_type() == FenXingType::VERIFY_BOTTOM) {
                            tmp_fx = this->fx;
                            tmp_fx.set_type(FenXingType::FAILURE_VERIFY_BOTTOM);
                        }
                        this->__init_fenxing(kxian);
                    }
                    else {
                        if (this->fx.get_type() == FenXingType::VERIFY_BOTTOM && kx_di < this->fx.get_low() && this->two.get_high() < this->max_high) {
                            tmp_fx = this->fx;
                            tmp_fx.set_type(FenXingType::FAILURE_VERIFY_BOTTOM);
                            this->__init_fenxing(kxian);
                        }
                        else {
                            tmp_fx = FenXing(this->one, this->two, kxian, kxian);
                            if (this->two.get_high() == this->max_high)
                                tmp_fx.set_high_low_type(HighLowType::NEW_HIGH);
                            this->three = kxian;
                            if (kx_gao < this->two.get_low()) {
                                //第三根K线和第二根K线有缺口
                                tmp_fx.set_type(FenXingType::VERIFY_TOP);
                                this->__init_fenxing(kxian);
                            }
                            else {
                                this->status = FenXingChuLiStatus::FOUR;
                            }
                            /*
                            if (this->fx.get_type() == FenXingType::VERIFY_TOP && kx_gao > this->fx.get_high()) {
                                tmp_fx.set_type(FenXingType::NEW_BOTTOM);
                            }
                            */
                        }
                    }
                    if (this->firstBarStatus)
                        this->firstBarStatus = false;
                }
            }
        }
        else {
            //第2根K线低于第1根
            if (kx_gao > this->max_high) {
                if (this->two.get_low() == this->min_low) {
                    this->three = kxian;
                    this->status = FenXingChuLiStatus::LOWHIGH;
                }
                else {
                    this->max_high = kx_gao;
                    if (this->fx.get_type() == FenXingType::VERIFY_TOP) {
                        tmp_fx = this->fx;
                        tmp_fx.set_type(FenXingType::FAILURE_VERIFY_TOP);
                    }
                    this->__init_fenxing(kxian);
                }
            }
            else {
                if (kx_di < this->two.get_low()) {
                    //下降趋势
                    if (kx_di < this->min_low)
                        this->min_low = kx_di;
                    if (this->fx.get_type() == FenXingType::VERIFY_BOTTOM && kx_di < this->fx.get_low()) {
                        tmp_fx = this->fx;
                        tmp_fx.set_type(FenXingType::FAILURE_VERIFY_BOTTOM);
                    }
                    if (this->firstBarStatus) {
                        this->four = kxian;
                        this->three = this->two;
                        this->two = this->one;
                        float tmpKx_gao = this->one.get_high() - 0.1;
                        float tmpKx_di = this->one.get_low() - 0.1;
                        this->one = Kxian1(tmpKx_gao, tmpKx_di, Direction::DOWN, 0);
                        tmp_fx = FenXing(this->one, this->two, this->three, kxian);
                        this->status = FenXingChuLiStatus::FIVE;
                        this->firstBarStatus = false;
                    } else
                        this->__init_fenxing(kxian);
                }
                else {
                    //底分型
                    if (kx_gao > this->max_high) {
                        this->max_high = kx_gao;
                        if (this->fx.get_type() == FenXingType::VERIFY_TOP) {
                            tmp_fx = this->fx;
                            tmp_fx.set_type(FenXingType::FAILURE_VERIFY_TOP);
                        }
                        this->__init_fenxing(kxian);
                    }
                    else {
                        if (this->fx.get_type() == FenXingType::VERIFY_TOP && kx_gao > this->fx.get_high() && this->two.get_low() > this->min_low) {
                            tmp_fx = this->fx;
                            tmp_fx.set_type(FenXingType::FAILURE_VERIFY_TOP);
                            this->__init_fenxing(kxian);
                        }
                        else {
                            tmp_fx = FenXing(this->one, this->two, kxian, kxian);
                            if (this->two.get_low() == this->min_low)
                                tmp_fx.set_high_low_type(HighLowType::NEW_LOW);
                            this->three = kxian;
                            if (kx_di > this->two.get_high()) {
                                //第三根K线和第二根K线有缺口
                                tmp_fx.set_type(FenXingType::VERIFY_BOTTOM);
                                this->__init_fenxing(kxian);
                            }
                            else
                                this->status = FenXingChuLiStatus::FOUR;
                            /*
                            if (this->fx.get_type() == FenXingType::VERIFY_TOP && kx_gao > this->fx.get_high()) {
                                tmp_fx.set_type(FenXingType::NEW_BOTTOM);
                                }
                            */
                        }
                    }
                    if (this->firstBarStatus)
                        this->firstBarStatus = false;
                }
            }
        }
        break;

    case FenXingChuLiStatus::HIGHLOW:
        if (kx_gao > this->max_high) {
            this->max_high = kx_gao;
            this->one = this->two;
            this->two = this->three;
            this->three = kxian;
            this->status = FenXingChuLiStatus::HIGHLOW;
        }
        else {
            if (kx_di < this->min_low) {
                this->four = kxian;
                this->min_low = kx_di;
                tmp_fx = FenXing(this->one, this->two, this->three, this->four);
                this->status = FenXingChuLiStatus::FIVE;
            }
            else {
                tmp_fx = FenXing(this->one, this->two, kxian, kxian);
                tmp_fx.set_high_low_type(HighLowType::NEW_LOW);
                this->three = kxian;
                this->status = FenXingChuLiStatus::FOUR;
            }
        }
        break;

    case FenXingChuLiStatus::LOWHIGH:
        if (kx_di < this->min_low) {
            this->min_low = kx_di;
            this->one = this->last_bar;
            this->two = kxian;
            this->status = FenXingChuLiStatus::THREE;
        }
        else {
            if (kx_gao > this->max_high) {
                this->max_high = kx_gao;
                this->one = this->last_bar;
                this->two = kxian;
                this->status = FenXingChuLiStatus::THREE;
            }
            else {
                tmp_fx = FenXing(this->one, this->two, kxian, kxian);
                tmp_fx.set_high_low_type(HighLowType::NEW_HIGH);
                this->three = kxian;
                this->status = FenXingChuLiStatus::FOUR;
            }
        }
        break;

    case FenXingChuLiStatus::FOUR:
        if (this->temp_fx.get_type() == FenXingType::TOP) {
            //顶分型处理
            if (kx_di < this->three.get_low()) {
                if (kx_di < this->min_low) {
                    if (this->two.get_high() == this->max_high) {
                        tmp_fx = this->temp_fx;
                        tmp_fx.set_type(FenXingType::VERIFY_BOTTOM);
                        tmp_fx.set_high_low_type(HighLowType::NEW_LOW);
                        this->__init_fenxing(kxian);
                    }
                    else {
                        this->min_low = kx_di;
                        if (this->fx.get_type() == FenXingType::VERIFY_BOTTOM) {
                            tmp_fx = this->fx;
                            tmp_fx.set_type(FenXingType::FAILURE_VERIFY_BOTTOM);
                        }
                        else {
                            tmp_fx = this->temp_fx;
                            tmp_fx.set_type(FenXingType::FAILURE_TOP);
                        }
                        this->__init_fenxing(kxian);
                    }
                }
                else {
                    if (this->fx.get_type() == FenXingType::VERIFY_BOTTOM && kx_di < this->fx.get_low() && this->fx.get_high_low_type() == HighLowType::NONE) {
                        tmp_fx = this->temp_fx;
                        tmp_fx.set_free(kxian);
                        tmp_fx.set_type(FenXingType::VERIFY_TOP);
                        this->__init_fenxing(kxian);
                    }
                    else {
                        tmp_fx = this->__kxian_process(kxian);
                    }
                }
            } else {
                //反向K线处理
                if (kx_gao > this->max_high)
                    //创新高
                    this->max_high = kx_gao;
                if (this->fx.get_type() == FenXingType::VERIFY_TOP && kx_gao > this->fx.get_high()) {
                    //高于前一个分型
                    tmp_fx = this->__failure_verify_top(kxian);
                } else {
                    if (kx_gao > this->temp_fx.get_high()) {
                        tmp_fx = this->temp_fx;
                        tmp_fx.set_type(FenXingType::FAILURE_TOP);
                        this->__init_fenxing(kxian);
                    }
                }
            }
        } else {
            //底分型处理
            if (kx_gao > this->three.get_high()) {
                if (kx_gao > this->max_high) {
                    this->max_high = kx_gao;
                    if (this->two.get_low() == this->min_low) {
                        tmp_fx = this->temp_fx;
                        tmp_fx.set_high_low_type(HighLowType::NEW_LOW);
                        tmp_fx.set_type(FenXingType::VERIFY_BOTTOM);
                        this->__init_fenxing(kxian);
                    }
                    else {
                        if (this->fx.get_type() == FenXingType::VERIFY_TOP) {
                            tmp_fx = this->fx;
                            tmp_fx.set_type(FenXingType::FAILURE_VERIFY_TOP);
                        }
                        else {
                            tmp_fx = this->temp_fx;
                            tmp_fx.set_type(FenXingType::FAILURE_BOTTOM);
                        }
                        this->__init_fenxing(kxian);
                    }
                }
                else {
                    if (this->fx.get_type() == FenXingType::VERIFY_TOP && kx_gao > this->fx.get_high() && this->fx.get_high_low_type() == HighLowType::NONE) {
                        tmp_fx = this->temp_fx;
                        tmp_fx.set_free(kxian);
                        tmp_fx.set_type(FenXingType::VERIFY_BOTTOM);
                        this->__init_fenxing(kxian);
                    }
                    else {
                        tmp_fx = this->__kxian_process(kxian);
                    }
                }
            } else {
                //反向K线处理
                if (kx_di < this->min_low) 
                    //创新低
                    this->min_low = kx_di;
                if (this->fx.get_type() == FenXingType::VERIFY_BOTTOM && kx_di < this->fx.get_low()) {
                    //低于上一个底分型
                    tmp_fx = this->__failue_verify_bottom(kxian);
                } else {
                    if (kx_di < this->temp_fx.get_low()) {
                        tmp_fx = this->temp_fx;
                        tmp_fx.set_type(FenXingType::FAILURE_BOTTOM);
                        this->__init_fenxing(kxian);
                    }
                }
            }
        }
        break;

    case FenXingChuLiStatus::FIVE:
        if (this->temp_fx.get_type() == FenXingType::TOP) {
            //顶分型处理
            if (kx_di < this->four.get_low()) {
                if (kx_di < this->min_low) {
                    this->min_low = kx_di;
                    if (this->two.get_high() == this->max_high) {
                        tmp_fx = this->temp_fx;
                        tmp_fx.set_high_low_type(HighLowType::NEW_HIGH);
                        tmp_fx.set_type(FenXingType::VERIFY_TOP);
                        this->__init_fenxing(kxian);
                    }
                    else {
                        if (this->fx.get_type() == FenXingType::VERIFY_BOTTOM) {
                            tmp_fx = this->fx;
                            tmp_fx.set_type(FenXingType::FAILURE_VERIFY_BOTTOM);
                        }
                        else {
                            tmp_fx = this->temp_fx;
                            tmp_fx.set_type(FenXingType::FAILURE_TOP);
                        }
                        this->__init_fenxing(kxian);
                    }
                }
                else {
                    if (this->fx.get_type() == FenXingType::VERIFY_BOTTOM && kx_di < this->fx.get_low() && this->fx.get_high_low_type() == HighLowType::NONE) {
                        //荣盛石化 5分钟 386
                        tmp_fx = this->temp_fx;
                        tmp_fx.set_free(kxian);
                        tmp_fx.set_type(FenXingType::VERIFY_TOP);
                        this->__init_fenxing(kxian);
                    }
                    else {
                        tmp_fx = this->__kxian_process(kxian);
                    }
                }
            } else {
                //反向K线处理
                if (kx_gao > this->max_high) 
                    //创新高
                    this->max_high = kx_gao;
                if (this->fx.get_type() == FenXingType::VERIFY_TOP && kx_gao > this->fx.get_high()) {
                    //高于前一个分型
                    tmp_fx = this->__failure_verify_top(kxian);
                }
                else {
                    if (kx_gao > this->temp_fx.get_high()) {
                        tmp_fx = this->temp_fx;
                        tmp_fx.set_type(FenXingType::FAILURE_TOP);
                        this->__init_fenxing(kxian);
                    }
                }
            }
        } else {
            //底分型处理
            if (kx_gao > this->four.get_high()) {
                if (kx_gao > this->max_high) {
                    this->max_high = kx_gao;
                    if (this->two.get_low() == this->min_low) {
                        tmp_fx = this->temp_fx;
                        tmp_fx.set_high_low_type(HighLowType::NEW_LOW);
                        tmp_fx.set_type(FenXingType::VERIFY_BOTTOM);
                        this->__init_fenxing(kxian);
                    }
                    else {
                        if (this->fx.get_type() == FenXingType::VERIFY_TOP) {
                            tmp_fx = this->fx;
                            tmp_fx.set_type(FenXingType::FAILURE_VERIFY_TOP);
                        }
                        else {
                            tmp_fx = this->temp_fx;
                            tmp_fx.set_type(FenXingType::FAILURE_BOTTOM);
                        }
                        this->__init_fenxing(kxian);
                    }
                }
                else {
                    if (this->fx.get_type() == FenXingType::VERIFY_TOP && kx_gao > this->fx.get_high() && this->fx.get_high_low_type() == HighLowType::NONE) {
                        tmp_fx = this->temp_fx;
                        tmp_fx.set_free(kxian);
                        tmp_fx.set_type(FenXingType::VERIFY_BOTTOM);
                        this->__init_fenxing(kxian);
                    }
                    else {
                        tmp_fx = this->__kxian_process(kxian);
                    }
                }
            } else {
                //反向K线处理
                if (kx_di < this->min_low) 
                    //创新低
                    this->min_low = kx_di;
                if (this->fx.get_type() == FenXingType::VERIFY_BOTTOM && kx_di < this->fx.get_low()) {
                    //低于上一个底分型
                    return(this->__failue_verify_bottom(kxian));
                }
                else {
                    if (kx_di < this->temp_fx.get_low()) {
                        tmp_fx = this->temp_fx;
                        tmp_fx.set_type(FenXingType::FAILURE_BOTTOM);
                        this->__init_fenxing(kxian);
                    }
                }
            }
        }
        break;

    case FenXingChuLiStatus::SIX:
        if (this->temp_fx.get_type() == FenXingType::TOP) {
            //顶分型
            if (kx_di < this->five.get_low()) {
                if (kx_di < this->min_low) {
                    if (this->two.get_high() == this->max_high) {
                        tmp_fx = FenXing(this->one, this->two, this->three, kxian);
                        tmp_fx.set_high_low_type(HighLowType::NEW_HIGH);
                        tmp_fx.set_type(FenXingType::VERIFY_TOP);
                        this->__init_fenxing(kxian);
                    }
                    else {
                        this->min_low = kx_di;
                        if (this->fx.get_type() == FenXingType::VERIFY_BOTTOM) {
                            tmp_fx = this->fx;
                            tmp_fx.set_type(FenXingType::FAILURE_VERIFY_BOTTOM);
                        }
                        else {
                            tmp_fx = this->temp_fx;
                            tmp_fx.set_type(FenXingType::FAILURE_TOP);
                        }
                        this->__init_fenxing(kxian);
                    }
                }
                else {
                    tmp_fx = FenXing(this->one, this->two, this->three, kxian);
                    tmp_fx.set_type(FenXingType::VERIFY_TOP);
                    this->__init_fenxing(kxian);
                }
            } else {
                //反方向处理
                if (kx_gao > this->max_high) 
                    //创新高
                    this->max_high = kx_gao;
                if (this->fx.get_type() == FenXingType::VERIFY_TOP && kx_gao > this->fx.get_high()) {
                    //高于前一个分型
                    tmp_fx = this->__failure_verify_top(kxian);
                    this->__init_fenxing(kxian);
                }
                else {
                    if (kx_gao > this->temp_fx.get_high()) {
                        tmp_fx = this->temp_fx;
                        tmp_fx.set_type(FenXingType::FAILURE_TOP);
                        this->__init_fenxing(kxian);
                    }
                }
            }
        } else {
            //底分型
            if (kx_gao > this->five.get_high()) {
                if (this->two.get_low() == this->min_low) {
                    tmp_fx = FenXing(this->one, this->two, this->three, kxian);
                    tmp_fx.set_high_low_type(HighLowType::NEW_LOW);
                    tmp_fx.set_type(FenXingType::VERIFY_BOTTOM);
                    this->__init_fenxing(kxian);
                }
                else {
                    if (kx_gao > this->max_high) {
                        this->max_high = kx_gao;
                        if (this->fx.get_type() == FenXingType::VERIFY_TOP) {
                            tmp_fx = this->fx;
                            tmp_fx.set_type(FenXingType::FAILURE_VERIFY_TOP);
                        }
                        else {
                            tmp_fx = this->temp_fx;
                            tmp_fx.set_type(FenXingType::FAILURE_BOTTOM);
                        }
                        this->__init_fenxing(kxian);
                    }
                    else {
                        tmp_fx = FenXing(this->one, this->two, this->three, kxian);
                        tmp_fx.set_free(kxian);
                        tmp_fx.set_type(FenXingType::VERIFY_BOTTOM);
                        this->__init_fenxing(kxian);
                    }
                }
            }
            else {
                //反向处理
                if (kx_di < this->min_low)
                    this->min_low = kx_di;
                if (this->fx.get_type() == FenXingType::VERIFY_BOTTOM && kx_di < this->fx.get_low()) {
                    tmp_fx = this->__failue_verify_bottom(kxian);
                    this->__init_fenxing(kxian);
                }
                else {
                    if (kx_di < this->temp_fx.get_low()) {
                        tmp_fx = this->temp_fx;
                        tmp_fx.set_type(FenXingType::FAILURE_BOTTOM);
                        this->__init_fenxing(kxian);
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
    FenXingChuLi fenXingChuLi;
    int position_start, position_stop;
    FenXing fx;

    for (int i = 0; i < nCount; i++) {
        baohanChuli.add(pHigh[i], pLow[i]);
    }
    fenXingChuLi.handle(baohanChuli.kxianList);
    unsigned int count = fenXingChuLi.keyFenXingList.size();

    for (int i = 0; i < nCount; i++) {
        pOut[i] = 0;
    }

    fx = fenXingChuLi.keyFenXingList[0];
    position_start = fx.get_start_position();
    switch (fx.get_type()) {
    case FenXingType::VERIFY_BOTTOM:
        pOut[position_start] = -3;
        break;
    case FenXingType::VERIFY_TOP:
        pOut[position_start] = 3;
        break;
    }
    for (unsigned int i = 0; i < count; i++) {
        fx = fenXingChuLi.keyFenXingList[i];
        position_stop = fx.get_stop_position();
        switch (fx.get_type()) {
        case FenXingType::BOTTOM:
            pOut[position_stop] = -1;
            break;
        case FenXingType::FAILURE_BOTTOM:
            pOut[position_stop] = -2;
            break;
        case FenXingType::VERIFY_BOTTOM:
            pOut[position_stop] = -3;
            break;
        case FenXingType::FAILURE_VERIFY_BOTTOM:
            pOut[position_stop] = -4;
            break;
        case FenXingType::TOP:
            pOut[position_stop] = 1;
            break;
        case FenXingType::FAILURE_TOP:
            pOut[position_stop] = 2;
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


void Bi3_fenxing_highlow(int nCount, float* pOut, float* pHigh, float* pLow, float* pIn) {
    BaoHanChuLi baohanChuli;
    int position_start, position_stop;
    FenXingChuLi fenXingChuLi;
    FenXing fx;

    for (int i = 0; i < nCount; i++) {
        baohanChuli.add(pHigh[i], pLow[i]);
    }

    fenXingChuLi.handle(baohanChuli.kxianList);
    for (int i = 0; i < nCount; i++) {
        pOut[i] = 0;
    }

    unsigned int count = fenXingChuLi.keyFenXingList.size();
    if (count > 0) {
        fx = fenXingChuLi.keyFenXingList[0];

        position_start = fx.get_start_position();
        switch (fx.get_type()) {
        case FenXingType::VERIFY_BOTTOM:
            pOut[position_start] = fx.get_low();
            break;
        case FenXingType::VERIFY_TOP:
            pOut[position_start] = fx.get_high();
            break;
        }
    }

    for (unsigned int i = 0; i < count; i++) {
        FenXing fx = fenXingChuLi.keyFenXingList[i];
        position_stop = fx.get_stop_position();
        switch (fx.get_type()) {
        case FenXingType::BOTTOM:
        case FenXingType::VERIFY_BOTTOM:
            pOut[position_stop] = fx.get_low();
            break;
        case FenXingType::TOP:
        case FenXingType::VERIFY_TOP:
            pOut[position_stop] = fx.get_high();
            break;
        default:
            pOut[position_stop] = 0;
        }
    }
}

//轨道上轨
void GuiDao_Gao(int nCount, float* pOut, float* pHigh, float* pLow, float* pIn) {
    BaoHanChuLi baohanChuli;
    int position_start = 0, position_stop = 0;
    FenXingChuLi fenXingChuLi;
    FenXing fx;
    float last_gao = 0, gao = 0;
    int start_count = 0;

    for (int i = 0; i < nCount; i++) {
        baohanChuli.add(pHigh[i], pLow[i]);
    }

    fenXingChuLi.handle(baohanChuli.kxianList);
    for (int i = 0; i < nCount; i++) {
        pOut[i] = 0;
    }

    unsigned int count = fenXingChuLi.keyFenXingList.size();
    if (count > 0) {
        fx = fenXingChuLi.keyFenXingList[0];
        if (fx.get_type() == FenXingType::VERIFY_TOP) {
            position_start = fx.get_start_position();
            last_gao = fx.get_high();
            start_count = 1;
        }
        else {
            if (count > 1) {
                fx = fenXingChuLi.keyFenXingList[1];
                if (fx.get_type() == FenXingType::VERIFY_TOP) {
                    position_start = fx.get_start_position();
                    last_gao = fx.get_high();
                    start_count = 2;
                }
            }
        }
    }
    for (unsigned int i = start_count; i < count; i++) {
        FenXing fx = fenXingChuLi.keyFenXingList[i];
        switch (fx.get_type()) {
        case FenXingType::VERIFY_TOP:
        case FenXingType::TOP:
            position_stop = fx.get_stop_position();
            gao = fx.get_high();
            if (gao > last_gao) {
                for (int pos = position_start; pos <= position_stop; pos++) {
                    pOut[pos] = gao;
                }
            }
            else {
                for (int pos = position_start; pos <= position_stop; pos++) {
                    pOut[pos] = last_gao;
                }
            }
            last_gao = gao;
            position_start = position_stop;
            break;
        }
    }
    for (unsigned int pos = position_stop; pos < nCount; pos++) {
        pOut[pos] = gao;
    }
}

//轨道下轨
void GuiDao_Di(int nCount, float* pOut, float* pHigh, float* pLow, float* pIn) {
    BaoHanChuLi baohanChuli;
    int position_start = 0, position_stop = 0;
    FenXingChuLi fenXingChuLi;
    FenXing fx;
    float last_di = 0, di = 0;
    int start_count = 1;

    for (int i = 0; i < nCount; i++) {
        baohanChuli.add(pHigh[i], pLow[i]);
    }

    fenXingChuLi.handle(baohanChuli.kxianList);
    for (int i = 0; i < nCount; i++) {
        pOut[i] = 0;
    }

    unsigned int count = fenXingChuLi.keyFenXingList.size();
    if (count > 0) {
        fx = fenXingChuLi.keyFenXingList[0];
        if (fx.get_type() == FenXingType::VERIFY_BOTTOM) {
            position_start = fx.get_start_position();
            last_di = fx.get_low();
            start_count = 1;
        }
        else {
            if (count > 1) {
                fx = fenXingChuLi.keyFenXingList[1];
                if (fx.get_type() == FenXingType::VERIFY_BOTTOM) {
                    position_start = fx.get_start_position();
                    last_di = fx.get_low();
                    start_count = 2;
                }
            }
        }
    }
    for (unsigned int i = start_count; i < count; i++) {
        FenXing fx = fenXingChuLi.keyFenXingList[i];
        switch (fx.get_type()) {
        case FenXingType::VERIFY_BOTTOM:
        case FenXingType::BOTTOM:
            position_stop = fx.get_stop_position();
            di = fx.get_low();
            if (di > last_di) {
                for (int pos = position_start; pos <= position_stop; pos++) {
                    pOut[pos] = last_di;
                }
            }
            else {
                for (int pos = position_start; pos <= position_stop; pos++) {
                    pOut[pos] = di;
                }
            }
            last_di = di;
            position_start = position_stop;
        }
    }
    for (unsigned int pos = position_stop; pos < nCount; pos++) {
        pOut[pos] = di;
    }
}

//轨道中轨
void GuiDao_Zhong(int nCount, float* pOut, float* pHigh, float* pLow, float* pIn) {
    BaoHanChuLi baohanChuli;
    int gao_position_start = 0, gao_position_stop = 0, di_position_start = 0, di_position_stop = 0;
    FenXingChuLi fenXingChuLi;
    FenXing fx, fx1;
    float last_di = 0, di = 0, last_gao = 0, gao = 0;
    int start_count = 1;
    float* gaoArray = new float[nCount]();
    float* diArray = new float[nCount]();

    for (int i = 0; i < nCount; i++) {
        baohanChuli.add(pHigh[i], pLow[i]);
    }

    fenXingChuLi.handle(baohanChuli.kxianList);
    for (int i = 0; i < nCount; i++) {
        pOut[i] = 0;
    }

    unsigned int count = fenXingChuLi.keyFenXingList.size();
    if (count > 0) {
        fx = fenXingChuLi.keyFenXingList[0];
        switch (fx.get_type()) {
        case FenXingType::VERIFY_BOTTOM:
        case FenXingType::BOTTOM:
            last_di = fx.get_low();
            di_position_start = fx.get_start_position();
            if (count > 1) {
                fx1 = fenXingChuLi.keyFenXingList[1];
                if (fx1.get_type() == FenXingType::VERIFY_TOP) {
                    last_gao = fx1.get_high();
                    gao_position_start = fx1.get_start_position();
                }
            }
            break;
        case FenXingType::VERIFY_TOP:
        case FenXingType::TOP:
            last_gao = fx.get_high();
            gao_position_start = fx.get_start_position();
            if (count > 1) {
                fx1 = fenXingChuLi.keyFenXingList[1];
                if (fx1.get_type() == FenXingType::VERIFY_BOTTOM) {
                    last_di = fx1.get_low();
                    di_position_start = fx1.get_start_position();
                }
            }
            break;
        }
    }
    for (unsigned int i = 2; i < count; i++) {
        fx = fenXingChuLi.keyFenXingList[i];
        switch (fx.get_type()) {
        case FenXingType::VERIFY_BOTTOM:
            di_position_stop = fx.get_stop_position();
            di = fx.get_low();
            if (di > last_di) {
                for (int pos = di_position_start; pos <= di_position_stop; pos++) {
                    diArray[pos] = last_di;
                }
            }
            else {
                for (int pos = di_position_start; pos <= di_position_stop; pos++) {
                    diArray[pos] = di;
                }
            }
            last_di = di;
            di_position_start = di_position_stop;
            break;
        case FenXingType::VERIFY_TOP:
            gao = fx.get_high();
            gao_position_stop = fx.get_stop_position();
            if (gao > last_gao) {
                for (int pos = gao_position_start; pos <= gao_position_stop; pos++) {
                    gaoArray[pos] = gao;
                }
            }
            else {
                for (int pos = gao_position_start; pos <= gao_position_stop; pos++) {
                    gaoArray[pos] = last_gao;
                }
            }
            last_gao = gao;
            gao_position_start = gao_position_stop;
            break;
        case FenXingType::BOTTOM:
            di = fx.get_low();
            di_position_stop = fx.get_stop_position();
            if (di > last_di) {
                for (int pos = di_position_start; pos <= di_position_stop; pos++) {
                    diArray[pos] = last_di;
                }
            }
            else {
                for (int pos = di_position_start; pos <= di_position_stop; pos++) {
                    diArray[pos] = di;
                }
            }
            for (int pos = gao_position_start; pos < nCount; pos++) {
                gaoArray[pos] = gao;
            }
            for (int pos = di_position_stop; pos < nCount; pos++) {
                diArray[pos] = di;
            }
            break;
        case FenXingType::TOP:
            gao = fx.get_high();
            gao_position_stop = fx.get_stop_position();
            if (gao > last_gao) {
                for (int pos = gao_position_start; pos <= gao_position_stop; pos++) {
                    gaoArray[pos] = gao;
                }
            }
            else {
                for (int pos = gao_position_start; pos <= gao_position_stop; pos++) {
                    gaoArray[pos] = last_gao;
                }
            }
            for (int pos = di_position_start; pos < nCount; pos++) {
                diArray[pos] = di;
            }
            for (int pos = gao_position_stop; pos < nCount; pos++) {
                gaoArray[pos] = gao;
            }
            break;
        }
    }
    switch (fx.get_type()) {
    case FenXingType::VERIFY_TOP:
        for (unsigned int pos = fx.get_stop_position(); pos < nCount; pos++) {
            gaoArray[pos] = gao;
        }
        for (unsigned int pos = di_position_stop; pos < nCount; pos++) {
            diArray[pos] = di;
        }
        break;
    case FenXingType::VERIFY_BOTTOM:
        for (unsigned int pos = fx.get_stop_position(); pos < nCount; pos++) {
            diArray[pos] = di;
        }
        for (unsigned int pos = gao_position_stop; pos < nCount; pos++) {
            gaoArray[pos] = gao;
        }
    }

    for (unsigned int pos = 0; pos < nCount; pos++) {
        pOut[pos] = (gaoArray[pos] + diArray[pos]) / 2;
    }
}

//轨道线状态
void GuiDao_Status(int nCount, float* pOut, float* pHigh, float* pLow, float* pClose) {
    BaoHanChuLi baohanChuli;
    int gao_position_start = 0, gao_position_stop = 0, di_position_start = 0, di_position_stop = 0;
    FenXingChuLi fenXingChuLi;
    FenXing fx, fx1;
    float last_di = 0, di = 0, last_gao = 0, gao = 0;
    int start_count = 1;
    float* gaoArray = new float[nCount]();
    float* diArray = new float[nCount]();
    float* zhongArray = new float[nCount]();

    for (int i = 0; i < nCount; i++) {
        baohanChuli.add(pHigh[i], pLow[i]);
    }

    fenXingChuLi.handle(baohanChuli.kxianList);
    for (int i = 0; i < nCount; i++) {
        pOut[i] = 0;
    }

    unsigned int count = fenXingChuLi.keyFenXingList.size();
    if (count > 0) {
        fx = fenXingChuLi.keyFenXingList[0];
        switch (fx.get_type()) {
        case FenXingType::VERIFY_BOTTOM:
        case FenXingType::BOTTOM:
            last_di = fx.get_low();
            di_position_start = fx.get_stop_position();
            if (count > 1) {
                fx1 = fenXingChuLi.keyFenXingList[1];
                if (fx1.get_type() == FenXingType::VERIFY_TOP) {
                    last_gao = fx1.get_high();
                    gao_position_start = fx1.get_stop_position();
                }
            }
            break;
        case FenXingType::VERIFY_TOP:
        case FenXingType::TOP:
            last_gao = fx.get_high();
            gao_position_start = fx.get_stop_position();
            if (count >= 1) {
                fx1 = fenXingChuLi.keyFenXingList[1];
                if (fx1.get_type() == FenXingType::VERIFY_BOTTOM) {
                    last_di = fx1.get_low();
                    di_position_start = fx1.get_stop_position();
                }
            }
            break;
        }
    }
    for (unsigned int i = 2; i < count; i++) {
        fx = fenXingChuLi.keyFenXingList[i];
        switch (fx.get_type()) {
        case FenXingType::VERIFY_BOTTOM:
            di_position_stop = fx.get_stop_position();
            di = fx.get_low();
            if (di > last_di) {
                for (int pos = di_position_start; pos <= di_position_stop; pos++) {
                    diArray[pos] = last_di;
                }
            }
            else {
                for (int pos = di_position_start; pos <= di_position_stop; pos++) {
                    diArray[pos] = di;
                }
            }
            last_di = di;
            di_position_start = di_position_stop;
            break;
        case FenXingType::VERIFY_TOP:
            gao = fx.get_high();
            gao_position_stop = fx.get_stop_position();
            if (gao > last_gao) {
                for (int pos = gao_position_start; pos <= gao_position_stop; pos++) {
                    gaoArray[pos] = gao;
                }
            }
            else {
                for (int pos = gao_position_start; pos <= gao_position_stop; pos++) {
                    gaoArray[pos] = last_gao;
                }
            }
            last_gao = gao;
            gao_position_start = gao_position_stop;
            break;
        case FenXingType::BOTTOM:
            di = fx.get_low();
            di_position_stop = fx.get_stop_position();
            if (di > last_di) {
                for (int pos = di_position_start; pos <= di_position_stop; pos++) {
                    diArray[pos] = last_di;
                }
            }
            else {
                for (int pos = di_position_start; pos <= di_position_stop; pos++) {
                    diArray[pos] = di;
                }
            }
            for (int pos = gao_position_start; pos < nCount; pos++) {
                gaoArray[pos] = gao;
            }
            for (int pos = di_position_stop; pos < nCount; pos++) {
                diArray[pos] = di;
            }
            break;
        case FenXingType::TOP:
            gao = fx.get_high();
            gao_position_stop = fx.get_stop_position();
            if (gao > last_gao) {
                for (int pos = gao_position_start; pos <= gao_position_stop; pos++) {
                    gaoArray[pos] = gao;
                }
            }
            else {
                for (int pos = gao_position_start; pos <= gao_position_stop; pos++) {
                    gaoArray[pos] = last_gao;
                }
            }
            for (int pos = di_position_start; pos < nCount; pos++) {
                diArray[pos] = di;
            }
            for (int pos = gao_position_stop; pos < nCount; pos++) {
                gaoArray[pos] = gao;
            }
            break;
        }
    }
    switch (fx.get_type()) {
    case FenXingType::VERIFY_TOP:
        for (unsigned int pos = fx.get_stop_position(); pos < nCount; pos++) {
            gaoArray[pos] = gao;
        }
        for (unsigned int pos = di_position_stop; pos < nCount; pos++) {
            diArray[pos] = di;
        }
        break;
    case FenXingType::VERIFY_BOTTOM:
        for (unsigned int pos = fx.get_stop_position(); pos < nCount; pos++) {
            diArray[pos] = di;
        }
        for (unsigned int pos = gao_position_stop; pos < nCount; pos++) {
            gaoArray[pos] = gao;
        }
    }

    for (unsigned int pos = 0; pos < nCount; pos++) {
        zhongArray[pos] = (gaoArray[pos] + diArray[pos]) / 2;
    }
    if (pClose[nCount - 1] > zhongArray[nCount - 1])
        pOut[nCount - 1] = 1;
    else
        pOut[nCount - 1] = 0;

    gao = gaoArray[nCount - 1];
    for (int pos = nCount - 2; pos > 0; pos--) {
        if (gao != gaoArray[pos]) {
            last_gao = gaoArray[pos];
            break;
        }
    }
    di = diArray[nCount - 1];
    for (int pos = nCount - 2; pos > 0; pos--) {
        if (di != diArray[pos]) {
            last_di = diArray[pos];
            break;
        }
    }
    if (gao > last_gao) {
        if (di > last_di) {
            //上行
            pOut[nCount - 2] = 1;
        }
        else {
            //扩张
            pOut[nCount - 2] = 2;
        }
    }
    else {
        if (di > last_di) {
            //收敛
            pOut[nCount - 2] = 3;
        }
        else {
            //下行
            pOut[nCount - 2] = 4;
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
        position_stop = fx.get_verify_stop_position();
        switch (fx.get_type()) {
        case FenXingType::BOTTOM:
            pOut[position_stop] = -1;
            break;
        case FenXingType::FAILURE_BOTTOM:
            pOut[position_stop] = -2;
            break;
        case FenXingType::VERIFY_BOTTOM:
            pOut[position_stop] = -3;
            break;
        case FenXingType::FAILURE_VERIFY_BOTTOM:
            pOut[position_stop] = -4;
            break;
        case FenXingType::TOP:
            pOut[position_stop] = 1;
            break;
        case FenXingType::FAILURE_TOP:
            pOut[position_stop] = 2;
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
