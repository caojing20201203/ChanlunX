#include "XianDuan.h"

using namespace std;

XianDuanChuLi::XianDuanChuLi() {
    this->last_xd = XianDuan();
    this->last_bi = Bi();
    this->bicl = BiChuLi();
    this->status = XianDuanChuLiStatus::START;

}

void XianDuanChuLi::handle(vector<Kxian1>& kxianList) {
    FindXianDuanReturn ret_fd;
    XianDuan xd = XianDuan();
    Bi tmp_bi = Bi();
    Bi bi = Bi();
    vector<Bi> bi_list;
    this->bicl.handle(kxianList);

    bi_list = this->bicl.biList;
    int count = bi_list.size();
    for (int i = 0; i < count; i++) {
        bi = bi_list[i];
        ret_fd = this->find_xianduan(bi);
        if (ret_fd.type != FindXianDuanReturnType::None) {
            switch (ret_fd.type) {
            case FindXianDuanReturnType::One:
                debug_xianduan(ret_fd.xd1);
                this->xianDuanList.push_back(ret_fd.xd1);
                if (ret_fd.xd1.get_type() == XianDuanType::UP || ret_fd.xd1.get_type() == XianDuanType::DOWN)
                    this->key_xianduan_list.push_back(ret_fd.xd1);
                break;
            case FindXianDuanReturnType::Two:
                debug_xianduan(ret_fd.xd1);
                this->xianDuanList.push_back(ret_fd.xd1);
                if (ret_fd.xd1.get_type() == XianDuanType::UP || ret_fd.xd1.get_type() == XianDuanType::DOWN)
                    this->key_xianduan_list.push_back(ret_fd.xd1);
                debug_xianduan(ret_fd.xd2);
                this->xianDuanList.push_back(ret_fd.xd2);
                if (ret_fd.xd2.get_type() == XianDuanType::UP || ret_fd.xd2.get_type() == XianDuanType::DOWN)
                    this->key_xianduan_list.push_back(ret_fd.xd2);
                break;
            case FindXianDuanReturnType::Three:
                debug_xianduan(ret_fd.xd1);
                this->xianDuanList.push_back(ret_fd.xd1);
                if (ret_fd.xd1.get_type() == XianDuanType::UP || ret_fd.xd1.get_type() == XianDuanType::DOWN)
                    this->key_xianduan_list.push_back(ret_fd.xd1);
                debug_xianduan(ret_fd.xd2);
                this->xianDuanList.push_back(ret_fd.xd2);
                if (ret_fd.xd2.get_type() == XianDuanType::UP || ret_fd.xd2.get_type() == XianDuanType::DOWN)
                    this->key_xianduan_list.push_back(ret_fd.xd2);
                debug_xianduan(ret_fd.xd3);
                this->xianDuanList.push_back(ret_fd.xd3);
                if (ret_fd.xd3.get_type() == XianDuanType::UP || ret_fd.xd3.get_type() == XianDuanType::DOWN)
                    this->key_xianduan_list.push_back(ret_fd.xd3);
                break;
            case FindXianDuanReturnType::Failure:
                if (ret_fd.xd1.get_type() == XianDuanType::UP)
                    OutputDebugPrintf("FAILURE XD: UP %f %f", ret_fd.xd1.get_low(), ret_fd.xd1.get_high());
                else {
                    if (ret_fd.xd1.get_type() == XianDuanType::DOWN)
                        OutputDebugPrintf("FAILURE XD: DOWN %f %f", ret_fd.xd1.get_high(), ret_fd.xd1.get_low());
                }
                if (!this->key_xianduan_list.empty()) {
                    this->key_xianduan_list.pop_back();
                    this->key_xianduan_list.push_back(ret_fd.xd1);
                    this->xianDuanList.push_back(ret_fd.xd1);
                }
                else {
                    this->xianDuanList.push_back(ret_fd.xd1);
                    this->key_xianduan_list.push_back(ret_fd.xd1);
                }
                break;
            case FindXianDuanReturnType::NewXianDuan:
                OutputDebugPrintf("NewXianDuan: %f %f", ret_fd.xd1.get_low(), ret_fd.xd1.get_high());
                if (!this->key_xianduan_list.empty()) {
                    this->key_xianduan_list.pop_back();
                    this->key_xianduan_list.push_back(ret_fd.xd1);
                    this->xianDuanList.push_back(ret_fd.xd1);
                }
                break;
            case FindXianDuanReturnType::XianDuanUpgrade:
                OutputDebugPrintf("XianDuanUpgrade: %f %f", ret_fd.xd1.get_low(), ret_fd.xd1.get_high());
                if (!this->key_xianduan_list.empty()) {
                    this->key_xianduan_list.pop_back();
                    this->key_xianduan_list.pop_back();
                    this->key_xianduan_list.push_back(ret_fd.xd1);
                    this->xianDuanList.push_back(ret_fd.xd1);
                }
                break;
            case FindXianDuanReturnType::None:
                break;
            }
        }
    }

    int ncount = this->xianDuanList.size();
    int start_num = 0;
    if (!this->key_xianduan_list.empty()) {
        XianDuan comp_xd = this->key_xianduan_list.back();
        for (int i = 0; i < ncount; i++) {
            xd = this->xianDuanList[i];
            if (xd == comp_xd) {
                start_num = i + 1;
                break;
            }
        }
        for (int i = start_num; i < ncount; i++) {
            xd = this->xianDuanList[i];
            this->key_xianduan_list.push_back(xd);
        }
    }

}

FindXianDuanReturn XianDuanChuLi::failure_xd(Bi start_bi, Bi stop_bi) {
    FindXianDuanReturn ret_fd = FindXianDuanReturn();
    XianDuan tmp_xd = XianDuan();
    Bi xd_start_bi = Bi();

    if (!this->key_xianduan_list.empty()) {
        XianDuan last_xd = this->key_xianduan_list.back();
        if (last_xd.get_type() == XianDuanType::UP) {
            //上升笔处理
            if (start_bi.get_low() < last_xd.get_low()) {
                ret_fd.xd1 = XianDuan(start_bi, start_bi);
                ret_fd.xd2 = XianDuan(stop_bi, stop_bi);
                ret_fd.type = FindXianDuanReturnType::Two;
            }
            else {
                ret_fd.xd1 = XianDuan(last_xd.get_start_bi(), stop_bi);
                ret_fd.xd1.save_bi_list(this->get_xd_bi_list());
                ret_fd.type = FindXianDuanReturnType::Failure;
            }
        }
        else {
            //下降笔处理
            if (start_bi.get_high() > last_xd.get_high()) {
                ret_fd.xd1 = XianDuan(start_bi, start_bi);
                ret_fd.xd2 = XianDuan(stop_bi, stop_bi);
                ret_fd.type = FindXianDuanReturnType::Two;
            }
            else {
                ret_fd.xd1 = XianDuan(last_xd.get_start_bi(), stop_bi);
                ret_fd.xd1.save_bi_list(this->get_xd_bi_list());
                ret_fd.type = FindXianDuanReturnType::Failure;
            }
        }
    }
    else {
        ret_fd.xd1 = XianDuan(start_bi, start_bi);
        ret_fd.xd2 = XianDuan(stop_bi, stop_bi);
        ret_fd.type = FindXianDuanReturnType::Two;
    }
    this->status = XianDuanChuLiStatus::START;
    return(ret_fd);
}


vector<Bi>  XianDuanChuLi::get_xd_bi_list() {
    vector<Bi> bi_list;
    switch (this->status) {
    case XianDuanChuLiStatus::START:
        break;
    case XianDuanChuLiStatus::LEFT:
        bi_list.push_back(this->start);
        break;
    case XianDuanChuLiStatus::AFTER_LEFT:
        bi_list.push_back(this->start);
        bi_list.push_back(this->left);
        break;
    case XianDuanChuLiStatus::MIDDLE_HIGHLOW:
    case XianDuanChuLiStatus::MIDDLE_NORMAL:
        bi_list.push_back(this->start);
        bi_list.push_back(this->left);
        bi_list.push_back(this->after_left);
        break;
    case XianDuanChuLiStatus::LEFT_INCLUDE_MIDDLE:
    case XianDuanChuLiStatus::LEFT_INCLUDE_MIDDLE_NORMAL:
    case XianDuanChuLiStatus::LEFT_INCLUDE_MIDDLE_INCLUDE_RIGHT:
    case XianDuanChuLiStatus::LEFT_INCLUDE_MIDDLE_INCLUDE_RIGHT_NORMAL:
    case XianDuanChuLiStatus::LEFT_INCLUDE_MIDDLE_INCLUDE_RIGHT_INCLUDE_FREE:
        bi_list.push_back(this->start);
        bi_list.push_back(this->left);
        bi_list.push_back(this->after_left);
        bi_list.push_back(this->middle);
        bi_list.push_back(this->after_middle);
        break;

    case XianDuanChuLiStatus::LONGXIANDUAN:
        bi_list.push_back(this->start);
        bi_list.push_back(this->left);
        bi_list.push_back(this->after_left);
        bi_list.push_back(this->middle);
        break;
    case XianDuanChuLiStatus::LONGXIANDUAN_RIGHT:
        bi_list.push_back(this->start);
        bi_list.push_back(this->left);
        bi_list.push_back(this->after_left);
        bi_list.push_back(this->middle);
        bi_list.push_back(this->after_middle);
        break;
    case XianDuanChuLiStatus::LONGXIANDUAN_AFTER_RIGHT:
        bi_list.push_back(this->start);
        bi_list.push_back(this->left);
        bi_list.push_back(this->after_left);
        bi_list.push_back(this->middle);
        bi_list.push_back(this->after_middle);
        bi_list.push_back(this->right);
        break;
    case XianDuanChuLiStatus::LONGXIANDUAN_FREE:
        bi_list.push_back(this->start);
        bi_list.push_back(this->left);
        bi_list.push_back(this->after_left);
        bi_list.push_back(this->middle);
        bi_list.push_back(this->after_middle);
        bi_list.push_back(this->right);
        break;
    case XianDuanChuLiStatus::LONGXIANDUAN_AFTER_FREE:
        bi_list.push_back(this->start);
        bi_list.push_back(this->left);
        bi_list.push_back(this->after_left);
        bi_list.push_back(this->middle);
        bi_list.push_back(this->after_middle);
        bi_list.push_back(this->right);
        bi_list.push_back(this->after_right);
        bi_list.push_back(this->free);
        break;
    case XianDuanChuLiStatus::LONGXIANDUAN_AFTER_FREE_1:
        bi_list.push_back(this->start);
        bi_list.push_back(this->left);
        bi_list.push_back(this->after_left);
        bi_list.push_back(this->middle);
        bi_list.push_back(this->after_middle);
        bi_list.push_back(this->right);
        bi_list.push_back(this->after_right);
        bi_list.push_back(this->free);
        bi_list.push_back(this->after_free);
        break;

    case XianDuanChuLiStatus::AFTER_MIDDLE:
        bi_list.push_back(this->start);
        bi_list.push_back(this->left);
        bi_list.push_back(this->after_left);
        bi_list.push_back(this->middle);
        break;
    case XianDuanChuLiStatus::RIGHT:
    case XianDuanChuLiStatus::RIGHT_NORMAL:
    case XianDuanChuLiStatus::RIGHT_NORMAL_NORMAL:
    case XianDuanChuLiStatus::RIGHT_NORMAL_NORMAL_NORMAL:
        bi_list.push_back(this->start);
        bi_list.push_back(this->left);
        bi_list.push_back(this->after_left);
        bi_list.push_back(this->middle);
        bi_list.push_back(this->after_middle);
        break;
    case XianDuanChuLiStatus::AFTER_RIGHT:
        bi_list.push_back(this->start);
        bi_list.push_back(this->left);
        bi_list.push_back(this->after_left);
        bi_list.push_back(this->middle);
        bi_list.push_back(this->after_middle);
        bi_list.push_back(this->right);
        break;
    case XianDuanChuLiStatus::FREE:
        bi_list.push_back(this->start);
        bi_list.push_back(this->left);
        bi_list.push_back(this->after_left);
        bi_list.push_back(this->middle);
        bi_list.push_back(this->after_middle);
        bi_list.push_back(this->right);
        bi_list.push_back(this->after_right);
        break;
    case XianDuanChuLiStatus::AFTER_FREE:
        bi_list.push_back(this->start);
        bi_list.push_back(this->left);
        bi_list.push_back(this->after_left);
        bi_list.push_back(this->middle);
        bi_list.push_back(this->after_middle);
        bi_list.push_back(this->right);
        bi_list.push_back(this->after_right);
        bi_list.push_back(this->free);
        break;

    case XianDuanChuLiStatus::AFTER_FREE_1:
        bi_list.push_back(this->start);
        bi_list.push_back(this->left);
        bi_list.push_back(this->after_left);
        bi_list.push_back(this->middle);
        bi_list.push_back(this->after_middle);
        bi_list.push_back(this->right);
        bi_list.push_back(this->after_right);
        bi_list.push_back(this->free);
        bi_list.push_back(this->after_free);
        break;

    case XianDuanChuLiStatus::AFTER_FREE_2:
        bi_list.push_back(this->start);
        bi_list.push_back(this->left);
        bi_list.push_back(this->after_left);
        bi_list.push_back(this->middle);
        bi_list.push_back(this->after_middle);
        bi_list.push_back(this->right);
        bi_list.push_back(this->after_right);
        bi_list.push_back(this->free);
        bi_list.push_back(this->after_free);
        bi_list.push_back(this->after_free_1);
        break;

    }
    return(bi_list);
}

FindXianDuanReturn  XianDuanChuLi::find_xianduan(Bi bi) {
    FindXianDuanReturn ret_fd;
    BiType bi_type;
    XianDuan xd = XianDuan();
    if (this->last_bi.get_type() == BiType::NONE) {
        if (bi.get_type() == BiType::UP || bi.get_type() == BiType::DOWN) {
            this->last_bi = bi;
            this->start = bi;
            XianDuanChuLi::status = XianDuanChuLiStatus::LEFT;
        }
    }
    else {
        bi_type = bi.get_type();
        switch (bi_type) {
        case BiType::UP:
        case BiType::DOWN:
            return(this->__find_xianduan(bi));
        case BiType::TEMP_DOWN:
        case BiType::TEMP_UP:
            break;
        default:
            this->__backroll(bi);
        }
    }
    ret_fd.type = FindXianDuanReturnType::None;
    ret_fd.xd1.save_bi_list(this->get_xd_bi_list());
    return(ret_fd);
}

void XianDuanChuLi::__backroll(Bi bi) {
    switch (this->status) {
    case XianDuanChuLiStatus::START:
        break;
    case XianDuanChuLiStatus::LEFT:
        this->status = XianDuanChuLiStatus::START;
        break;
    case XianDuanChuLiStatus::AFTER_LEFT:
        this->status = XianDuanChuLiStatus::LEFT;
        break;
    case XianDuanChuLiStatus::MIDDLE_HIGHLOW:
        this->status = XianDuanChuLiStatus::AFTER_LEFT;
        break;
    case XianDuanChuLiStatus::MIDDLE_NORMAL:
        this->status = XianDuanChuLiStatus::AFTER_LEFT;
    case XianDuanChuLiStatus::AFTER_MIDDLE:
        //this->status = XianDuanChuLiStatus::MIDDLE;
        break;
    case XianDuanChuLiStatus::RIGHT:
        this->status = XianDuanChuLiStatus::AFTER_MIDDLE;
    case XianDuanChuLiStatus::AFTER_RIGHT:
        // this->status = XianDuanChuLiStatus::RIGHT;
        break;
    case XianDuanChuLiStatus::FREE:
        this->status = XianDuanChuLiStatus::AFTER_RIGHT;
        break;
    case XianDuanChuLiStatus::AFTER_FREE:
        this->status = XianDuanChuLiStatus::FREE;
        break;
    }
}

void XianDuanChuLi::debug_xianduan(XianDuan xd) {
    XianDuanType xd_type = xd.get_type();
    switch (xd_type) {
    case XianDuanType::UP:
        OutputDebugPrintf("【上升线段】 %f %f", xd.get_low(), xd.get_high());
        break;
    case XianDuanType::FAILURE_UP:
        OutputDebugPrintf("【失败上升线段】 %f %f", xd.get_low(), xd.get_high());
        break;
    case XianDuanType::DOWN:
        OutputDebugPrintf("【下降线段】 %f %f", xd.get_high(), xd.get_low());
        break;
    case XianDuanType::FAILURE_DOWN:
        OutputDebugPrintf("【失败下降线段】 %f %f", xd.get_high(), xd.get_low());
        break;
    }
}

char* get_xianduan_status(XianDuanChuLiStatus status) {
    switch (status) {
    case XianDuanChuLiStatus::START:
        return("START");
    case XianDuanChuLiStatus::LEFT:
        return("LEFT");
    case XianDuanChuLiStatus::AFTER_LEFT:
        return("AFTER_LEFT");
    case XianDuanChuLiStatus::MIDDLE_HIGHLOW:
        return("MIDDLE_HIGHLOW");
    case XianDuanChuLiStatus::MIDDLE_NORMAL:
        return("MIDDLE_NORMAL");
    case XianDuanChuLiStatus::AFTER_MIDDLE:
        return("AFTER_MIDDLE");
    case XianDuanChuLiStatus::RIGHT:
        return("RIGHT_NORMAL");
    case XianDuanChuLiStatus::AFTER_RIGHT:
        return("AFTER_RIGHT");
    case XianDuanChuLiStatus::FREE:
        return("FREE");
    case XianDuanChuLiStatus::AFTER_FREE:
        return("AFTER_RIGHT");
    case XianDuanChuLiStatus::LEFT_INCLUDE_MIDDLE:
        return("LEFT_INCLUDE_MIDDLE");
    default:
        return("Not Process Item");
    }
}


bool is_bi_equal(Bi bi1, Bi bi2) {
    float radio;
    float bi1_len = bi1.get_length();
    float bi2_len = bi2.get_length();
    if (bi1_len < bi2_len) {
        radio = bi1_len / bi2_len;
    }
    else {
        radio = bi2_len / bi1_len;
    }

    if (radio > 0.618)
        return(true);
    else
        return(false);
}

FindXianDuanReturn XianDuanChuLi::__find_xianduan(Bi bi) {
    XianDuan xd = XianDuan();
    FindXianDuanReturn ret_xd;
    Bi_ZhongShu A_zs, B_zs;
    int ret_cnt;
    XianDuan last_xd, before_last_xd;

    switch (this->status) {
    case XianDuanChuLiStatus::START:
        ret_cnt = this->key_xianduan_list.size();
        if (ret_cnt >= 2) {
            last_xd = this->key_xianduan_list[ret_cnt - 1];
            before_last_xd = this->key_xianduan_list[ret_cnt - 2];
        }
        else {
            if (ret_cnt > 0) {
                before_last_xd = XianDuan();
                last_xd = this->key_xianduan_list[0];
            }
            else {
                last_xd = XianDuan();
                before_last_xd = XianDuan();
            }
        }
        if (bi.get_type() == BiType::UP) {
            if (this->last_xd.get_type() == XianDuanType::DOWN && bi.get_high() > this->last_xd.get_high()) {
                if (before_last_xd.get_type() == XianDuanType::UP && bi.get_low() > before_last_xd.get_low()) {
                    ret_xd.xd1 = XianDuan(before_last_xd.get_start_bi(), bi);
                    ret_xd.type = FindXianDuanReturnType::XianDuanUpgrade;
                    this->status = XianDuanChuLiStatus::START;
                }
                else {
                    this->start = bi;
                    this->status = XianDuanChuLiStatus::LEFT;
                }
            }
            else {
                this->start = bi;
                this->status = XianDuanChuLiStatus::LEFT;
            }
        }
        else {
            if (bi.get_type() == BiType::DOWN) {
                if (last_xd.get_type() == XianDuanType::UP && bi.get_low() < last_xd.get_low()) {
                    if (before_last_xd.get_type() == XianDuanType::DOWN && before_last_xd.get_high() > bi.get_high()) {
                        ret_xd.xd1 = XianDuan(before_last_xd.get_start_bi(), bi);
                        ret_xd.type = FindXianDuanReturnType::XianDuanUpgrade;
                        this->status = XianDuanChuLiStatus::START;
                    }
                    else {
                        this->start = bi;
                        this->status = XianDuanChuLiStatus::LEFT;
                    }
                }
                else {
                    this->start = bi;
                    this->status = XianDuanChuLiStatus::LEFT;
                }
            }
        }
        break;

    case XianDuanChuLiStatus::LEFT:
        if (this->start.get_type() == BiType::UP && bi.get_low() < this->start.get_low() || (this->start.get_type() == BiType::DOWN && bi.get_high() > this->start.get_high())) {
            return(this->failure_xd(this->start, bi));
        }
        else {
            this->left = bi;
            this->status = XianDuanChuLiStatus::AFTER_LEFT;
        }
        break;

    case XianDuanChuLiStatus::AFTER_LEFT:
        this->after_left = bi;
        last_xd = XianDuan();
        before_last_xd = XianDuan();
        ret_cnt = this->key_xianduan_list.size();
        if (ret_cnt >= 2) {
            before_last_xd = this->key_xianduan_list[ret_cnt - 2];
            last_xd = this->key_xianduan_list[ret_cnt - 1];
        }
        else {
            if (ret_cnt > 0) {
                before_last_xd = XianDuan();
                last_xd = this->key_xianduan_list[0];
            }
            else {
                before_last_xd = XianDuan();
                last_xd = XianDuan();
            }
        }
        if (bi.get_type() == BiType::UP) {
            if (last_xd.get_type() == XianDuanType::DOWN && bi.get_high() > last_xd.get_high()) {
                if (before_last_xd.get_type() == XianDuanType::UP && bi.get_low() < before_last_xd.get_low()) {
                    ret_xd.xd1 = XianDuan(before_last_xd.get_start_bi(), bi);
                    ret_xd.type = FindXianDuanReturnType::XianDuanUpgrade;
                    this->status = XianDuanChuLiStatus::START;
                    return(ret_xd);
                }
            }
            if (bi.get_high() > this->start.get_high()) {
                this->status = XianDuanChuLiStatus::MIDDLE_HIGHLOW;
                ret_xd.xd1 = XianDuan(this->start, bi);
                ret_xd.xd1.set_type(XianDuanType::TEMP_UP);
                ret_xd.type = FindXianDuanReturnType::One;
                return(ret_xd);
            }
            else {
                this->status = XianDuanChuLiStatus::MIDDLE_NORMAL;
            }
        }
        else {
            //下降笔
            if (last_xd.get_type() == XianDuanType::UP && bi.get_low() < last_xd.get_low()) {
                if (before_last_xd.get_type() == XianDuanType::DOWN && bi.get_high() < before_last_xd.get_high()) {
                    ret_xd.xd1 = XianDuan(before_last_xd.get_start_bi(), bi);
                    ret_xd.type = FindXianDuanReturnType::XianDuanUpgrade;
                    this->status = XianDuanChuLiStatus::START;
                    return(ret_xd);
                }
            }
            if (bi.get_low() < this->start.get_low()){
                this->status = XianDuanChuLiStatus::MIDDLE_HIGHLOW;
                ret_xd.xd1 = XianDuan(this->start, bi);
                ret_xd.xd1.set_type(XianDuanType::TEMP_DOWN);
                ret_xd.type = FindXianDuanReturnType::One;
                return(ret_xd);
            }
            else {
                this->status = XianDuanChuLiStatus::MIDDLE_NORMAL;
            }
        }
        break;

    case XianDuanChuLiStatus::MIDDLE_HIGHLOW:
        if (bi.get_type() == BiType::UP) {
            //上升笔
            if (bi.get_high() > this->start.get_high()) {
                //创新高
                if (!this->key_xianduan_list.empty()) {
                    last_xd = this->key_xianduan_list.back();
                    if (last_xd.get_type() == XianDuanType::UP) {
                        ret_xd.xd1 = XianDuan(last_xd.get_start_bi(), bi);
                        ret_xd.type = FindXianDuanReturnType::NewXianDuan;
                        this->status = XianDuanChuLiStatus::START;
                    }
                    else {
                        ret_xd.xd1 = XianDuan(this->start, this->after_left);
                        ret_xd.type = FindXianDuanReturnType::One;
                        this->start = bi;
                        this->status = XianDuanChuLiStatus::LEFT;
                    }
                }
                else {
                    ret_xd.xd1 = XianDuan(this->start, this->after_left);
                    ret_xd.type = FindXianDuanReturnType::One;
                    this->start = bi;
                    this->status = XianDuanChuLiStatus::LEFT;
                }
            }
            else {
                if (bi.get_high() < this->start.get_low()) {
                    this->middle = bi;
                    this->status = XianDuanChuLiStatus::LONGXIANDUAN;
                }
                else {
                    this->__middle_process(bi);
                }
            }
        }
        else {
            //下降笔
            if (bi.get_low() < this->start.get_low()) {
                //创新低
                if (!this->key_xianduan_list.empty()) {
                    last_xd = this->key_xianduan_list.back();
                    if (last_xd.get_type() == XianDuanType::DOWN) {
                        ret_xd.xd1 = XianDuan(last_xd.get_start_bi(), bi);
                        ret_xd.type = FindXianDuanReturnType::NewXianDuan;
                        this->status = XianDuanChuLiStatus::START;
                    }
                    else {
                        ret_xd.xd1 = XianDuan(this->start, this->after_left);
                        ret_xd.type = FindXianDuanReturnType::One;
                        this->start = bi;
                        this->status = XianDuanChuLiStatus::LEFT;
                    }
                }
                else {
                    ret_xd.xd1 = XianDuan(this->start, this->after_left);
                    ret_xd.type = FindXianDuanReturnType::One;
                    this->start = bi;
                    this->status = XianDuanChuLiStatus::LEFT;
                }
            }
            else {
                if (bi.get_low() > this->start.get_high()) {
                    this->middle = bi;
                    this->status = XianDuanChuLiStatus::LONGXIANDUAN;
                }
                else {
                    this->__middle_process(bi);
                }
            }
        }
        break;

    case XianDuanChuLiStatus::MIDDLE_NORMAL:
        if (bi.get_type() == BiType::UP) {
            if (bi.get_high() > this->start.get_high()) {
                //创新高
                this->left = bi.generate_bi(this->left, this->after_left, bi);
                return(this->failure_xd(this->start, this->left));
            }
            if (bi.get_high() <= this->left.get_high()) {
                //left 包含middle
                this->middle = bi;
                this->status = XianDuanChuLiStatus::LEFT_INCLUDE_MIDDLE;
            }
            else {
                this->left = bi.generate_bi(this->left, this->after_left, bi);
                this->status = XianDuanChuLiStatus::AFTER_LEFT;
            }
        }
        else {
            //下降笔
            if (bi.get_low() < this->start.get_low()) {
                //创新低
                this->left = bi.generate_bi(this->left, this->after_left, bi);
                return(this->failure_xd(this->start, this->left));
            }
            if (bi.get_low() >= this->left.get_low()) {
                //left 包含middle
                this->middle = bi;
                this->status = XianDuanChuLiStatus::LEFT_INCLUDE_MIDDLE;
            }
            else {
                this->left = bi.generate_bi(this->left, this->after_left, bi);
                this->status = XianDuanChuLiStatus::AFTER_LEFT;
            }
        }
        break;

    case XianDuanChuLiStatus::LEFT_INCLUDE_MIDDLE:
        if (bi.get_type() == BiType::UP) {
            //上升笔
            if (bi.get_high() > this->start.get_high()) {
                this->after_left = bi.generate_bi(this->after_left, this->middle, bi);
                ret_xd.xd1 = XianDuan(this->start, this->after_left);
                ret_xd.xd1.set_type(XianDuanType::TEMP_UP);
                ret_xd.type = FindXianDuanReturnType::One;
                this->status = XianDuanChuLiStatus::MIDDLE_HIGHLOW;
            }
            else {
                if (bi.get_high() > this->after_left.get_high()) {
                    this->after_left = bi.generate_bi(this->after_left, this->middle, bi);
                    this->status = XianDuanChuLiStatus::MIDDLE_NORMAL;
                }
                else {
                    this->after_middle = bi;
                    this->status = XianDuanChuLiStatus::LEFT_INCLUDE_MIDDLE_NORMAL;
                }
            }
        }
        else {
            //下降笔
            if (bi.get_low() < this->start.get_low()) {
                this->after_left = bi.generate_bi(this->after_left, this->middle, bi);
                ret_xd.xd1 = XianDuan(this->start, this->after_left);
                ret_xd.xd1.set_type(XianDuanType::TEMP_DOWN);
                ret_xd.type = FindXianDuanReturnType::One;
                this->status = XianDuanChuLiStatus::MIDDLE_HIGHLOW;
            }
            else {
                if (bi.get_low() < this->after_left.get_low()) {
                    this->after_left = bi.generate_bi(this->after_left, this->middle, bi);
                    this->status = XianDuanChuLiStatus::MIDDLE_NORMAL;
                }
                else {
                    this->after_middle = bi;
                    this->status = XianDuanChuLiStatus::LEFT_INCLUDE_MIDDLE_NORMAL;
                }
            }
        }
        break;

    case XianDuanChuLiStatus::LEFT_INCLUDE_MIDDLE_NORMAL:
        if (bi.get_type() == BiType::UP) {
            if (bi.get_high() > this->start.get_high()) {
                this->left = bi.generate_bi(this->left, this->after_left, bi);
                return(this->failure_xd(this->start, this->left));
            }
            else {
                if (bi.get_high() > this->left.get_high()) {
                    this->left = bi.generate_bi(this->left, this->after_left, bi);
                    this->status = XianDuanChuLiStatus::AFTER_LEFT;
                }
                else {
                    if (bi.get_high() > this->middle.get_high()) {
                        this->middle = bi.generate_bi(this->middle, this->after_middle, bi);
                        this->status = XianDuanChuLiStatus::LEFT_INCLUDE_MIDDLE;
                    }
                    else {
                        //LEFT_INCLUDE_MIDDLE_NORMAL_NORMAL
                        this->right = bi;
                        this->status = XianDuanChuLiStatus::LEFT_INCLUDE_MIDDLE_INCLUDE_RIGHT;
                    }
                }
            }
        }
        else {
            //下降笔
            if (bi.get_low() < this->start.get_low()) {
                this->left = bi.generate_bi(this->left, this->after_left, bi);
                return(this->failure_xd(this->start, this->left));
            }
            else {
                if (bi.get_low() < this->left.get_low()) {
                    this->left = bi.generate_bi(this->left, this->after_left, bi);
                    this->status = XianDuanChuLiStatus::AFTER_LEFT;
                }
                else {
                    if (bi.get_low() < this->middle.get_low()) {
                        this->middle = bi.generate_bi(this->middle, this->after_middle, bi);
                        this->status = XianDuanChuLiStatus::LEFT_INCLUDE_MIDDLE;
                    }
                    else {
                        this->right = bi;
                        this->status = XianDuanChuLiStatus::LEFT_INCLUDE_MIDDLE_INCLUDE_RIGHT;
                    }
                }
            }
        }
        break;

    case XianDuanChuLiStatus::LEFT_INCLUDE_MIDDLE_INCLUDE_RIGHT:
        if (bi.get_type() == BiType::UP) {
            if (bi.get_high() > this->start.get_high()) {
                this->after_left = bi.generate_bi(this->after_left, this->middle, bi);
                ret_xd.xd1 = XianDuan(this->start, this->after_left);
                ret_xd.type = FindXianDuanReturnType::One;
                this->status = XianDuanChuLiStatus::MIDDLE_HIGHLOW;
            }
            else {
                if (bi.get_high() > this->after_left.get_high()) {
                    this->after_left = bi.generate_bi(this->after_left, this->middle, bi);
                    this->status = XianDuanChuLiStatus::MIDDLE_NORMAL;
                }
                else {
                    if (bi.get_high() > this->after_middle.get_high()) {
                        this->after_middle = bi.generate_bi(this->after_middle, this->right, bi);
                        this->status = XianDuanChuLiStatus::LEFT_INCLUDE_MIDDLE_NORMAL;
                    }
                    else {
                        this->after_right = bi;
                        this->status = XianDuanChuLiStatus::LEFT_INCLUDE_MIDDLE_INCLUDE_RIGHT_NORMAL;
                    }
                }
            }
        }
        else {
            //下降笔
            if (bi.get_low() < this->start.get_low()) {
                this->after_left = bi.generate_bi(this->after_left, this->middle, bi);
                this->after_left = bi.generate_bi(this->after_left, this->middle, bi);
                ret_xd.xd1 = XianDuan(this->start, this->after_left);
                ret_xd.type = FindXianDuanReturnType::One;
                this->status = XianDuanChuLiStatus::MIDDLE_HIGHLOW;
            }
            else {
                if (bi.get_low() < this->after_left.get_low()) {
                    this->after_left = bi.generate_bi(this->after_left, this->middle, bi);
                    this->status = XianDuanChuLiStatus::MIDDLE_NORMAL;
                }
                else {
                    if (bi.get_low() < this->after_middle.get_low()) {
                        this->after_middle = bi.generate_bi(this->after_middle, this->right, bi);
                        this->status = XianDuanChuLiStatus::LEFT_INCLUDE_MIDDLE_NORMAL;
                    }
                    else {
                        this->after_right = bi;
                        this->status = XianDuanChuLiStatus::LEFT_INCLUDE_MIDDLE_INCLUDE_RIGHT_NORMAL;
                    }
                }
            }
        }
        break;

    case XianDuanChuLiStatus::LEFT_INCLUDE_MIDDLE_INCLUDE_RIGHT_NORMAL:
        if (bi.get_type() == BiType::UP) {
            if (bi.get_high() > this->start.get_high()) {
                this->left = bi.generate_bi(this->left, this->after_left, bi);
                return(this->failure_xd(this->start, this->left));
            }
            else {
                if (bi.get_high() > this->left.get_high()) {
                    this->left = bi.generate_bi(this->left, this->after_left, bi);
                    this->status = XianDuanChuLiStatus::AFTER_LEFT;
                }
                else {
                    if (bi.get_high() > this->middle.get_high()) {
                        this->middle = bi.generate_bi(this->middle, this->after_middle, bi);
                        this->status = XianDuanChuLiStatus::LEFT_INCLUDE_MIDDLE;
                    }
                    else {
                        if (bi.get_high() > this->right.get_high()) {
                            this->right = bi.generate_bi(this->right, this->after_right, bi);
                            this->status = XianDuanChuLiStatus::LEFT_INCLUDE_MIDDLE_INCLUDE_RIGHT;
                        }
                        else {
                            this->free = bi;
                            this->status = XianDuanChuLiStatus::LEFT_INCLUDE_MIDDLE_INCLUDE_RIGHT_INCLUDE_FREE;
                        }
                    }
                }
            }
        }
        else {
            //下降笔
            if (bi.get_low() < this->start.get_low()) {
                this->left = bi.generate_bi(this->left, this->after_left, bi);
                return(this->failure_xd(this->start, this->left));
            }
            else {
                if (bi.get_low() < this->left.get_low()) {
                    this->left = bi.generate_bi(this->left, this->after_left, bi);
                    this->status = XianDuanChuLiStatus::AFTER_LEFT;
                }
                else {
                    if (bi.get_low() < this->middle.get_low()) {
                        this->middle = bi.generate_bi(this->middle, this->after_middle, bi);
                        this->status = XianDuanChuLiStatus::LEFT_INCLUDE_MIDDLE;
                    }
                    else {
                        if (bi.get_low() < this->right.get_low()) {
                            this->right = bi.generate_bi(this->right, this->after_right, bi);
                            this->status = XianDuanChuLiStatus::LEFT_INCLUDE_MIDDLE_INCLUDE_RIGHT;
                        }
                        else {
                            this->free = bi;
                            this->status = XianDuanChuLiStatus::LEFT_INCLUDE_MIDDLE_INCLUDE_RIGHT_INCLUDE_FREE;
                        }
                    }
                }
            }
        }
        break;

    case XianDuanChuLiStatus::LEFT_INCLUDE_MIDDLE_INCLUDE_RIGHT_INCLUDE_FREE:
        OutputDebugPrintf("ToDO: LEFT_INCLUDE_MIDDLE_INCLUDE_RIGHT_INCLUDE_FREE 未处理");
        break;

    case XianDuanChuLiStatus::LONGXIANDUAN:
        if (bi.get_type() == BiType::UP) {
            if (bi.get_high() > this->middle.get_high()) {
                ret_xd.xd1 = XianDuan(this->start, bi);
                ret_xd.type = FindXianDuanReturnType::One;
                this->status = XianDuanChuLiStatus::START;
            }
            else {
                this->after_middle = bi;
                this->status = XianDuanChuLiStatus::LONGXIANDUAN_RIGHT;
            }
        }
        else {
            //下降笔
            if (bi.get_low() < this->middle.get_low()) {
                ret_xd.xd1 = XianDuan(this->start, bi);
                ret_xd.type = FindXianDuanReturnType::One;
                this->status = XianDuanChuLiStatus::START;
            }
            else {
                this->after_middle = bi;
                this->status = XianDuanChuLiStatus::LONGXIANDUAN_RIGHT;
            }
        }
        break;

    case XianDuanChuLiStatus::LONGXIANDUAN_RIGHT:
        if (bi.get_type() == BiType::UP) {
            if (bi.get_high() > this->start.get_high()) {
                this->start = bi.generate_bi(this->start, this->left, this->after_left);
                this->left = bi.generate_bi(this->middle, this->after_middle, bi);
                return(this->failure_xd(this->start, this->left));
            }
            else {
                if (bi.get_high() > this->middle.get_high()) {
                    if (bi.get_high() < this->start.get_low()) {
                        this->middle = bi.generate_bi(this->middle, this->after_middle, bi);
                        this->status = XianDuanChuLiStatus::LONGXIANDUAN;
                    }
                    else {
                        ret_xd.xd1 = XianDuan(this->start, this->after_left);
                        ret_xd.xd1.set_type(XianDuanType::TEMP_DOWN);
                        ret_xd.xd2 = XianDuan(this->middle, bi);
                        ret_xd.xd2.set_type(XianDuanType::TEMP_UP);
                        ret_xd.type = FindXianDuanReturnType::Two;
                        this->right = bi;
                        this->status = XianDuanChuLiStatus::AFTER_RIGHT;
                    }
                }
                else {
                    this->right = bi;
                    this->status = XianDuanChuLiStatus::LONGXIANDUAN_AFTER_RIGHT;
                }
            }
        }
        else {
            //下降笔
            if (bi.get_low() < this->start.get_low()) {
                this->start = bi.generate_bi(this->start, this->left, this->after_left);
                this->left = bi.generate_bi(this->middle, this->after_middle, bi);
                return(this->failure_xd(this->start, this->left));
            }
            else {
                if (bi.get_low() < this->middle.get_low()) {
                    if (bi.get_low() > this->start.get_high()) {
                        this->middle = bi.generate_bi(this->middle, this->after_middle, bi);
                        this->status = XianDuanChuLiStatus::LONGXIANDUAN;
                    }
                    else {
                        this->right = bi;
                        ret_xd.xd1 = XianDuan(this->start, this->after_left);
                        ret_xd.xd1.set_type(XianDuanType::TEMP_DOWN);
                        ret_xd.xd2 = XianDuan(this->middle, bi);
                        ret_xd.xd2.set_type(XianDuanType::TEMP_UP);
                        ret_xd.type = FindXianDuanReturnType::Two;
                        this->right = bi;
                        this->status = XianDuanChuLiStatus::AFTER_RIGHT;
                    }
                }
                else {
                    this->right = bi;
                    this->status = XianDuanChuLiStatus::LONGXIANDUAN_AFTER_RIGHT;
                }
            }
        }
        break;

    case XianDuanChuLiStatus::LONGXIANDUAN_AFTER_RIGHT:
        if (bi.get_type() == BiType::UP) {
            if (bi.get_high() > this->middle.get_high()) {
                this->start = bi.generate_bi(this->start, this->left, this->after_left);
                this->left = this->middle;
                this->after_left = bi.generate_bi(this->after_middle, this->right, bi);
                this->status = XianDuanChuLiStatus::MIDDLE_HIGHLOW;
            }
            else {
                if (bi.get_high() > this->right.get_high()) {
                    this->after_middle = bi.generate_bi(this->after_middle, this->right, bi);
                    this->status = XianDuanChuLiStatus::LONGXIANDUAN_RIGHT;
                }
                else {
                    this->after_right = bi;
                    this->status = XianDuanChuLiStatus::LONGXIANDUAN_FREE;
                }
            }
        }
        else {
            //下降笔
            if (bi.get_low() < this->middle.get_low()) {
                this->start = bi.generate_bi(this->start, this->left, this->after_left);
                this->left = this->middle;
                this->after_left = bi.generate_bi(this->after_middle, this->right, bi);
                this->status = XianDuanChuLiStatus::MIDDLE_HIGHLOW;
            }
            else {
                if (bi.get_low() < this->right.get_low()) {
                    this->after_middle = bi.generate_bi(this->after_middle, this->right, bi);
                    this->status = XianDuanChuLiStatus::LONGXIANDUAN_RIGHT;
                }
                else {
                    this->after_right = bi;
                    this->status = XianDuanChuLiStatus::LONGXIANDUAN_FREE;
                }
            }
        }
        break;

    case XianDuanChuLiStatus::LONGXIANDUAN_FREE:
        if (bi.get_type() == BiType::UP) {
            if (bi.get_high() > this->start.get_high()) {
                this->start = bi.generate_bi(this->start, this->left, this->after_left);
                this->left = bi.generate_bi(this->middle, this->after_middle, bi);
                return(this->failure_xd(this->start, this->left));
            }
            else {
                if (bi.get_high() > this->middle.get_high()) {
                    this->middle = bi.generate_bi(this->middle, this->after_middle, bi);
                    if (bi.get_high() < this->start.get_low()) {
                        this->status = XianDuanChuLiStatus::LONGXIANDUAN;
                    }
                    else {
                        this->__middle_process(this->middle);
                    }
                }
                else {
                    if (bi.get_high() > this->right.get_high()) {
                        this->right = bi.generate_bi(this->right, this->after_right, bi);
                        this->status = XianDuanChuLiStatus::LONGXIANDUAN_AFTER_RIGHT;
                    }
                    else {
                        this->free = bi;
                        this->status = XianDuanChuLiStatus::LONGXIANDUAN_AFTER_FREE;
                        break;
                    }
                }
            }
        }
        else {
            //下降笔
            if (bi.get_low() < this->start.get_low()) {
                this->start = bi.generate_bi(this->start, this->left, this->after_left);
                this->left = bi.generate_bi(this->middle, this->after_middle, bi);
                return(this->failure_xd(this->start, this->left));
            }
            else {
                if (bi.get_low() < this->middle.get_low()) {
                    if (bi.get_low() > this->start.get_high()) {
                        this->middle = bi.generate_bi(this->middle, this->after_middle, bi);
                        this->status = XianDuanChuLiStatus::LONGXIANDUAN;
                    }
                    else {
                        this->right = bi.generate_bi(this->right, this->after_right, bi);
                        this->status = XianDuanChuLiStatus::AFTER_RIGHT;
                    }
                }
                else {
                    this->free = bi;
                    this->status = XianDuanChuLiStatus::LONGXIANDUAN_AFTER_FREE;
                }
            }
            break;

    case XianDuanChuLiStatus::LONGXIANDUAN_AFTER_FREE:
        if (bi.get_type() == BiType::UP) {
            if (bi.get_high() > this->middle.get_high()) {
                this->start = bi.generate_bi(this->start, this->left, this->after_left);
                this->left = this->middle;
                this->after_left = bi.generate_bi(this->after_middle, this->right, bi);
                this->status = XianDuanChuLiStatus::MIDDLE_HIGHLOW;
            }
            else {
                if (bi.get_high() > this->right.get_high()) {
                    this->after_middle = bi.generate_bi(this->after_middle, this->right, bi);
                    this->status = XianDuanChuLiStatus::RIGHT;
                }
                else {
                    if (bi.get_high() > this->free.get_high()) {
                        this->after_right = bi.generate_bi(this->after_right, this->free, bi);
                        this->status = XianDuanChuLiStatus::LONGXIANDUAN_FREE;
                    }
                    else {
                        this->after_free = bi;
                        this->status = XianDuanChuLiStatus::LONGXIANDUAN_AFTER_FREE_1;
                    }
                }
            }
        }
        else {
            //下降笔
            if (bi.get_low() < this->middle.get_low()) {
                this->start = bi.generate_bi(this->start, this->left, this->after_left);
                this->left = this->middle;
                this->after_left = bi.generate_bi(this->after_middle, this->right, bi);
                this->status = XianDuanChuLiStatus::MIDDLE_HIGHLOW;
            }
            else {
                if (bi.get_low() < this->right.get_low()) {
                    this->after_middle = bi.generate_bi(this->after_middle, this->right, bi);
                    this->status = XianDuanChuLiStatus::RIGHT;
                }
                else {
                    if (bi.get_low() < this->free.get_low()) {
                        this->after_right = bi.generate_bi(this->after_right, this->free, bi);
                        this->status = XianDuanChuLiStatus::LONGXIANDUAN_FREE;
                    }
                    else {
                        this->after_free = bi;
                        this->status = XianDuanChuLiStatus::LONGXIANDUAN_AFTER_FREE_1;
                    }
                }
            }
        }
        break;

    case XianDuanChuLiStatus::LONGXIANDUAN_AFTER_FREE_1:
        if (bi.get_type() == BiType::UP) {
            if (bi.get_high() > this->start.get_high()) {
                this->start = bi.generate_bi(this->start, this->left, this->after_left);
                this->left = bi.generate_bi(this->middle, this->after_middle, bi);
                return(this->failure_xd(this->start, this->left));
            }
            else {
                if (bi.get_high() > this->middle.get_high()) {
                    if (bi.get_high() < this->start.get_low()) {
                        this->middle = bi.generate_bi(this->middle, this->after_middle, bi);
                        this->status = XianDuanChuLiStatus::LONGXIANDUAN;
                    }
                    else {
                        this->right = bi.generate_bi(this->right, this->after_right, bi);
                        this->status = XianDuanChuLiStatus::LONGXIANDUAN_AFTER_RIGHT;
                    }
                }
                else {
                    if (bi.get_high() > this->right.get_high()) {
                        this->right = bi.generate_bi(this->right, this->after_right, bi);
                        this->status = XianDuanChuLiStatus::LONGXIANDUAN_AFTER_RIGHT;
                    }
                    else {
                        if (bi.get_high() > this->free.get_high()) {
                            this->free = bi.generate_bi(this->free, this->after_free, bi);
                            this->status = XianDuanChuLiStatus::LONGXIANDUAN_AFTER_FREE;
                        }
                        else {
                            this->after_free_1 = bi;
                            OutputDebugPrintf("LONGXIANDUAN_AFTER_FREE_2 未处理");
                        }
                    }
                }
            }
        }
        else {
            //下降笔
            if (bi.get_low() < this->start.get_low()) {
                this->start = bi.generate_bi(this->start, this->left, this->after_left);
                this->left = bi.generate_bi(this->middle, this->after_middle, bi);
                return(this->failure_xd(this->start, this->left));
            }
            else {
                if (bi.get_low() > this->middle.get_low()) {
                    if (bi.get_low() > this->start.get_high()) {
                        this->middle = bi.generate_bi(this->middle, this->after_middle, bi);
                        this->status = XianDuanChuLiStatus::LONGXIANDUAN;
                    }
                    else {
                        this->right = bi.generate_bi(this->right, this->after_right, bi);
                        this->status = XianDuanChuLiStatus::AFTER_RIGHT;
                    }
                }
                else {
                    if (bi.get_low() < this->right.get_low()) {
                        this->right = bi.generate_bi(this->right, this->after_right, bi);
                        this->status = XianDuanChuLiStatus::LONGXIANDUAN_AFTER_RIGHT;
                    }
                    else {
                        if (bi.get_low() < this->free.get_low()) {
                            this->free = bi.generate_bi(this->free, this->after_free, bi);
                            this->status = XianDuanChuLiStatus::LONGXIANDUAN_AFTER_FREE;
                        }
                        else {
                            this->after_free_1 = bi;
                            OutputDebugPrintf("LONGXIANDUAN_AFTER_FREE_2 未处理");
                        }
                    }
                }
            }
        }
        break;

    case XianDuanChuLiStatus::AFTER_MIDDLE:
        this->after_middle = bi;
        if (bi.get_type() == BiType::UP) {
            if (bi.get_high() > this->middle.get_high()) {
                this->start = bi.generate_bi(this->start, this->left, this->after_left);
                this->left = this->middle;
                this->after_left = this->after_middle;
                this->status = XianDuanChuLiStatus::MIDDLE_HIGHLOW;
            }
            else {
                this->status = XianDuanChuLiStatus::RIGHT;
            }
        }
        else {
            //下降笔
            if (bi.get_low() < this->middle.get_low()) {
                this->start = bi.generate_bi(this->start, this->left, this->after_left);
                this->left = this->middle;
                this->after_left = this->after_middle;
                this->status = XianDuanChuLiStatus::MIDDLE_HIGHLOW;
            }
            else {
                this->status = XianDuanChuLiStatus::RIGHT;
            }
        }
        break;

    case XianDuanChuLiStatus::RIGHT:
        if (bi.get_type() == BiType::UP) {
            //上升笔
            if (bi.get_high() > this->start.get_high()) {
                this->start = bi.generate_bi(this->start, this->left, this->after_left);
                this->left = bi.generate_bi(this->middle, this->after_middle, bi);
                return(this->failure_xd(this->start, this->left));
            }
            else {
                if (bi.get_high() > this->middle.get_high()) {
                    ret_xd.xd1 = XianDuan(this->start, this->after_left);
                    ret_xd.xd2 = XianDuan(this->middle, bi);
                    ret_xd.xd2.set_type(XianDuanType::TEMP_UP);
                    ret_xd.type = FindXianDuanReturnType::Two;
                    this->start = this->middle;
                    this->left = this->after_middle;
                    this->after_left = bi;
                    this->status = XianDuanChuLiStatus::MIDDLE_HIGHLOW;
                }
                else {
                    if (this->middle.get_high() < this->start.get_low()) {
                        this->right = bi;
                        this->status = XianDuanChuLiStatus::AFTER_RIGHT;
                    }
                    else {
                        this->right = bi;
                        this->status = XianDuanChuLiStatus::RIGHT_NORMAL;
                    }
                }
            }
        }
        else {
            //下降笔
            if (bi.get_low() < this->start.get_low()) {
                this->start = bi.generate_bi(this->start, this->left, this->after_left);
                this->left = bi.generate_bi(this->middle, this->after_middle, bi);
                return(this->failure_xd(this->start, this->left));
            }
            else {
                if (bi.get_low() < this->middle.get_low()) {
                    ret_xd.xd1 = XianDuan(this->start, this->after_left);
                    ret_xd.xd2 = XianDuan(this->middle, bi);
                    ret_xd.xd2.set_type(XianDuanType::TEMP_DOWN);
                    ret_xd.type = FindXianDuanReturnType::Two;
                    this->start = this->middle;
                    this->left = this->after_middle;
                    this->after_left = bi;
                    this->status = XianDuanChuLiStatus::MIDDLE_HIGHLOW;
                }
                else {
                    if (this->middle.get_low() > this->start.get_high()) {
                        this->right = bi;
                        this->status = XianDuanChuLiStatus::AFTER_RIGHT;
                    }
                    else {
                        this->right = bi;
                        this->status = XianDuanChuLiStatus::RIGHT_NORMAL;
                    }
                }
            }
        }
        break;
    case XianDuanChuLiStatus::RIGHT_NORMAL:
        if (bi.get_type() == BiType::UP) {
            //上升笔
            if (bi.get_high() > this->middle.get_high()){
                this->start = bi.generate_bi(this->start, this->left, this->after_left);
                this->left = this->middle;
                this->after_left = bi.generate_bi(this->after_middle, this->right, bi);
                this->status = XianDuanChuLiStatus::MIDDLE_HIGHLOW;
            }
            else {
                if (bi.get_high() > this->right.get_high()) {
                    this->after_middle = bi.generate_bi(this->after_middle, this->right, bi);
                    this->status = XianDuanChuLiStatus::RIGHT;
                }
                else {
                    this->after_right = bi;
                    this->status = XianDuanChuLiStatus::RIGHT_NORMAL_NORMAL;
                }
            }

        }
        else {
            //下降笔
            if (bi.get_low() < this->middle.get_low()) {
                this->start = bi.generate_bi(this->start, this->left, this->after_left);
                this->left = this->middle;
                this->after_left = bi.generate_bi(this->after_middle, this->right, bi);
                this->status = XianDuanChuLiStatus::MIDDLE_HIGHLOW;
            }
            else {
                if (bi.get_low() < this->right.get_low()) {
                    this->after_middle = bi.generate_bi(this->after_middle, this->right, bi);
                    this->status = XianDuanChuLiStatus::RIGHT;
                }
                else {
                    this->after_right = bi;
                    this->status = XianDuanChuLiStatus::RIGHT_NORMAL_NORMAL;
                }
            }
        }
        break;

    case XianDuanChuLiStatus::RIGHT_NORMAL_NORMAL:
        if (bi.get_type() == BiType::UP) {
            if (bi.get_high() > this->middle.get_high()) {
                ret_xd.xd1 = XianDuan(this->start, this->after_left);
                ret_xd.xd2 = XianDuan(this->middle, bi);
                ret_xd.type = FindXianDuanReturnType::Two;
                this->status = XianDuanChuLiStatus::START;
            }
            else {
                if (bi.get_high() > this->right.get_high()) {
                    this->right = bi.generate_bi(this->right, this->after_right, bi);
                    this->status = XianDuanChuLiStatus::RIGHT_NORMAL;
                }
                else {
                    this->free = bi;
                    this->status = XianDuanChuLiStatus::RIGHT_NORMAL_NORMAL_NORMAL;
                }
            }
        }
        else {
            //下降笔
            if (bi.get_low() < this->middle.get_low()) {
                ret_xd.xd1 = XianDuan(this->start, this->after_left);
                ret_xd.xd2 = XianDuan(this->middle, bi);
                ret_xd.type = FindXianDuanReturnType::Two;
                this->status = XianDuanChuLiStatus::START;
            }
            else {
                if (bi.get_low() < this->right.get_low()) {
                    this->right = bi.generate_bi(this->right, this->after_right, bi);
                    this->status = XianDuanChuLiStatus::RIGHT_NORMAL;
                }
                else {
                    this->free = bi;
                    this->status = XianDuanChuLiStatus::RIGHT_NORMAL_NORMAL_NORMAL;
                }
            }
        }
        break;

    case XianDuanChuLiStatus::RIGHT_NORMAL_NORMAL_NORMAL:
        OutputDebugPrintf("RIGHT_NORMAL_NORMAL_NORMAL 未处理");
        break;

    case XianDuanChuLiStatus::AFTER_RIGHT:
        if (bi.get_type() == BiType::UP) {
            //上升笔
            if (bi.get_high() > this->middle.get_high()) {
                //创新高
                ret_xd.xd1 = XianDuan(this->start, this->after_left);
                ret_xd.xd2 = XianDuan(this->middle, this->right);
                ret_xd.type = FindXianDuanReturnType::Two;
                this->start = bi;
                this->status = XianDuanChuLiStatus::LEFT;
            }
            else {
                if (this->middle.get_low() > this->start.get_high()) {
                    this->after_right = bi;
                    this->status = XianDuanChuLiStatus::FREE;
                }
                else {
                    this->after_middle = bi.generate_bi(this->after_middle, this->right, bi);
                    this->status = XianDuanChuLiStatus::RIGHT;
                }
            }
        }
        else {
            //下降笔
            if (bi.get_low() < this->middle.get_low()) {
                //创新低
                ret_xd.xd1 = XianDuan(this->start, this->after_left);
                ret_xd.xd2 = XianDuan(this->middle, this->right);
                ret_xd.type = FindXianDuanReturnType::Two;
                this->start = bi;
                this->status = XianDuanChuLiStatus::LEFT;
            }
            else {
                if (this->middle.get_high() < this->start.get_low()) {
                    this->after_right = bi;
                    this->status = XianDuanChuLiStatus::FREE;
                }
                else {
                    this->after_middle = bi.generate_bi(this->after_middle, this->right, bi);
                    this->status = XianDuanChuLiStatus::RIGHT;
                }
            }
        }
        break;

    case XianDuanChuLiStatus::FREE:
        if (bi.get_type() == BiType::UP) {
            //上升笔
            if (bi.get_high() > this->start.get_high()) {
                //创新高
                this->start = bi.generate_bi(this->start, this->left, this->after_left);
                this->left = bi.generate_bi(this->middle, this->after_middle, bi);
                return(this->failure_xd(this->start, this->left));
            }
            if (bi.get_high() > this->after_right.get_high()) {
                this->middle = bi.generate_bi(this->middle, this->after_middle, this->right);
                if (this->__middle_process(this->middle)) {
                    this->left = bi.generate_bi(this->left, this->after_left, bi);
                    this->status = XianDuanChuLiStatus::AFTER_LEFT;
                }
                else {
                    this->after_middle = this->after_right;
                    return(this->__right_process(bi));
                }
            }
            else {
                this->free = bi;
                this->status = XianDuanChuLiStatus::AFTER_FREE;
            }
        }
        else {
            //下降笔
            if (bi.get_low() < this->start.get_low()) {
                //创新低
                this->start = bi.generate_bi(this->start, this->left, this->after_left);
                this->left = bi.generate_bi(this->middle, this->after_middle, bi);
                return(this->failure_xd(this->start, this->left));
            }
            if (bi.get_low() < this->after_right.get_low()) {
                this->middle = bi.generate_bi(this->middle, this->after_middle, this->right);
                if (this->__middle_process(this->middle)) {
                    this->left = bi.generate_bi(this->left, this->after_left, bi);
                    this->status = XianDuanChuLiStatus::AFTER_LEFT;
                }
                else {
                    this->after_middle = this->after_right;
                    return(this->__right_process(bi));
                }
            }
            else {
                this->free = bi;
                this->status = XianDuanChuLiStatus::AFTER_FREE;
            }
        }
        break;

    case XianDuanChuLiStatus::AFTER_FREE:
        if (bi.get_type() == BiType::UP) {
            //上升笔
            if (bi.get_high() > this->middle.get_high()) {
                this->start = bi.generate_bi(this->start, this->left, this->after_left);
                this->left = bi.generate_bi(this->middle, this->after_middle, this->right);
                this->after_left = bi.generate_bi(this->after_right, this->free, bi);
                this->status = XianDuanChuLiStatus::MIDDLE_HIGHLOW;
            }
            else {
                if (bi.get_high() < this->after_right.get_high()) {
                    this->after_free = bi;
                    this->status = XianDuanChuLiStatus::AFTER_FREE_1;
                }
                else {
                    ret_xd.xd1 = XianDuan(this->start, this->after_left);
                    ret_xd.xd2 = XianDuan(this->middle, this->right);
                    if (this->after_middle.get_low() >= this->after_right.get_high() + 0.01) {
                        //after_middle和after_right有缺口
                        ret_xd.xd2.set_type(XianDuanType::TEMP_DOWN);
                        ret_xd.type = FindXianDuanReturnType::Two;
                        this->start = this->middle;
                        this->left = this->after_middle;
                        this->after_left = this->right;
                        this->middle = this->after_right;
                        this->after_middle = this->free;
                        this->right = bi;
                        this->status = XianDuanChuLiStatus::AFTER_RIGHT;
                        return(ret_xd);
                    }
                    else {
                        ret_xd.xd3 = XianDuan(this->after_right, bi);
                        if (this->after_right.get_type() == BiType::UP)
                            ret_xd.xd3.set_type(XianDuanType::TEMP_UP);
                        else
                            ret_xd.xd3.set_type(XianDuanType::TEMP_DOWN);
                        ret_xd.type = FindXianDuanReturnType::Three;
                        this->start = this->after_right;
                        this->left = this->free;
                        this->after_left = bi;
                        this->status = XianDuanChuLiStatus::MIDDLE_HIGHLOW;
                        return(ret_xd);
                    }
                }
            }
        }
        else {
            //下降笔
            if (bi.get_low() < this->middle.get_low()) {
                this->start = bi.generate_bi(this->start, this->left, this->after_left);
                this->left = bi.generate_bi(this->middle, this->after_middle, this->right);
                this->after_left = bi.generate_bi(this->after_right, this->free, bi);
                this->status = XianDuanChuLiStatus::MIDDLE_HIGHLOW;
            }
            else {
                if (bi.get_low() > this->after_right.get_low()) {
                    //after_right 包含 after_free
                    this->after_free = bi;
                    this->status = XianDuanChuLiStatus::AFTER_FREE_1;
                }
                else {
                    ret_xd.xd1 = XianDuan(this->start, this->after_left);
                    ret_xd.xd1.save_bi_list(this->get_xd_bi_list());
                    ret_xd.xd2 = XianDuan(this->middle, this->right);
                    if (this->after_right.get_low() > this->after_middle.get_high() + 0.01) {
                        //有缺口
                        ret_xd.xd2.set_type(XianDuanType::TEMP_UP);
                        ret_xd.type = FindXianDuanReturnType::Two;
                        this->start = this->middle;
                        this->left = this->after_middle;
                        this->after_left = this->right;
                        this->middle = this->after_right;
                        this->after_middle = this->free;
                        this->right = bi;
                        this->status = XianDuanChuLiStatus::AFTER_RIGHT;
                        return(ret_xd);
                    }
                    else {
                        ret_xd.xd3 = XianDuan(this->after_right, bi);
                        if (this->after_right.get_type() == BiType::UP)
                            ret_xd.xd3.set_type(XianDuanType::TEMP_UP);
                        else
                            ret_xd.xd3.set_type(XianDuanType::TEMP_DOWN);
                        ret_xd.type = FindXianDuanReturnType::Three;
                        this->start = this->after_right;
                        this->left = this->free;
                        this->after_left = bi;
                        this->status = XianDuanChuLiStatus::MIDDLE_HIGHLOW;
                        return(ret_xd);
                    }
                }
            }
        }
        break;

    case XianDuanChuLiStatus::AFTER_FREE_1:
        if (bi.get_type() == BiType::UP) {
            if (bi.get_high() > this->start.get_high()) {
                this->start = bi.generate_bi(this->start, this->left, this->after_left);
                this->left = bi.generate_bi(this->middle, this->after_middle, bi);
                return(this->failure_xd(this->start, this->left));
            }
            else {
                if (bi.get_high() > this->right.get_high()) {
                    this->start = bi.generate_bi(this->start, this->left, this->after_left);
                    this->left = bi.generate_bi(this->middle, this->after_middle, bi);
                    this->status = XianDuanChuLiStatus::AFTER_LEFT;
                }
                else {
                    if (bi.get_high() > this->free.get_high()) {
                        this->free = bi.generate_bi(this->free, this->after_free, bi);
                        this->status = XianDuanChuLiStatus::AFTER_FREE;
                    }
                    else {
                        this->after_free_1 = bi;
                        this->status = XianDuanChuLiStatus::AFTER_FREE_2;
                    }
                }
            }
        }
        else {
            //下降笔
            if (bi.get_low() < this->start.get_low()) {
                this->start = bi.generate_bi(this->start, this->left, this->after_left);
                this->left = bi.generate_bi(this->middle, this->after_middle, bi);
                return(this->failure_xd(this->start, this->left));
            }
            else {
                if (bi.get_low() < this->right.get_low()) {
                    this->start = bi.generate_bi(this->start, this->left, this->after_left);
                    this->left = bi.generate_bi(this->middle, this->after_middle, bi);
                    this->status = XianDuanChuLiStatus::AFTER_LEFT;
                }
                else {
                    if (bi.get_low() < this->free.get_low()) {
                        this->free = bi.generate_bi(this->free, this->after_free, bi);
                        this->status = XianDuanChuLiStatus::AFTER_FREE;
                    }
                    else {
                        this->after_free_1 = bi;
                        this->status = XianDuanChuLiStatus::AFTER_FREE_2;
                    }
                }
            }
        }
        break;

    case XianDuanChuLiStatus::AFTER_FREE_2:
        if (bi.get_type() == BiType::UP) {
            if (bi.get_high() > this->middle.get_high()) {
                this->start = bi.generate_bi(this->start, this->left, this->after_left);
                this->left = bi.generate_bi(this->middle, this->after_middle, this->right);
                this->after_left = bi.generate_bi(this->right, this->after_right, bi);
                this->status = XianDuanChuLiStatus::MIDDLE_HIGHLOW;
            }
            else {
                if (bi.get_high() > this->after_right.get_high()) {
                    this->after_right = bi.generate_bi(this->after_right, this->free, bi);
                    this->status = XianDuanChuLiStatus::FREE;
                }
                else {
                    if (bi.get_high() > this->after_free.get_high()) {
                        this->after_free = bi.generate_bi(this->after_free, this->after_free_1, bi);
                        this->status = XianDuanChuLiStatus::AFTER_FREE_1;
                    }
                    else {
                        this->after_free_2 = bi;
                        this->status = XianDuanChuLiStatus::AFTER_FREE_3;
                    }
                }
            }
        }
        else {
            //下降笔
            if (bi.get_low() < this->middle.get_low()) {
                this->start = bi.generate_bi(this->start, this->left, this->after_left);
                this->left = bi.generate_bi(this->middle, this->after_middle, this->right);
                this->after_left = bi.generate_bi(this->right, this->after_right, bi);
                this->status = XianDuanChuLiStatus::MIDDLE_HIGHLOW;
            }
            else {
                if (bi.get_low() < this->after_right.get_low()) {
                    this->after_right = bi.generate_bi(this->after_right, this->free, bi);
                    this->status = XianDuanChuLiStatus::FREE;
                }
                else {
                    if (bi.get_low() < this->after_free.get_low()) {
                        this->after_free = bi.generate_bi(this->after_free, this->after_free_1, bi);
                        this->status = XianDuanChuLiStatus::AFTER_FREE_1;
                    }
                    else {
                        this->after_free_2 = bi;
                        this->status = XianDuanChuLiStatus::AFTER_FREE_3;
                    }
                }
            }
        }
        break;

    case XianDuanChuLiStatus::AFTER_FREE_3:
        if (bi.get_type() == BiType::UP) {
            if (bi.get_high() > this->start.get_high()) {
                this->start = bi.generate_bi(this->start, this->left, this->after_left);
                this->left = bi.generate_bi(this->middle, this->after_middle, bi);
                return(this->failure_xd(this->start, this->left));
            }
            else {
                if (bi.get_high() > this->right.get_high()) {
                    this->start = bi.generate_bi(this->start, this->left, this->after_left);
                    this->left = bi.generate_bi(this->middle, this->after_middle, bi);
                    this->status = XianDuanChuLiStatus::AFTER_LEFT;
                }
                else {
                    if (bi.get_high() > this->free.get_high()) {
                        this->free = bi.generate_bi(this->free, this->after_free, bi);
                        this->status = XianDuanChuLiStatus::AFTER_FREE;
                    }
                    else {
                        if (bi.get_high() > this->after_free_1.get_high()) {
                            this->after_free_1 = bi.generate_bi(this->after_free_1, this->after_free_2, bi);
                            this->status = XianDuanChuLiStatus::AFTER_FREE_1;
                        }
                        else {
                            OutputDebugPrintf("AFTER_FREE_4未处理");
                        }
                    }
                }
            }
        }
        else {
            //下降笔
            if (bi.get_low() < this->start.get_low()) {
                this->start = bi.generate_bi(this->start, this->left, this->after_left);
                this->left = bi.generate_bi(this->middle, this->after_middle, bi);
                return(this->failure_xd(this->start, this->left));
            }
            else {
                if (bi.get_low() < this->right.get_low()) {
                    this->start = bi.generate_bi(this->start, this->left, this->after_left);
                    this->left = bi.generate_bi(this->middle, this->after_middle, bi);
                    this->status = XianDuanChuLiStatus::AFTER_LEFT;
                }
                else {
                    if (bi.get_low() < this->free.get_low()) {
                        this->free = bi.generate_bi(this->free, this->after_free, bi);
                        this->status = XianDuanChuLiStatus::AFTER_FREE;
                    }
                    else {
                        if (bi.get_low() < this->after_free_1.get_low()) {
                            this->after_free_1 = bi.generate_bi(this->after_free_1, this->after_free_2, bi);
                            this->status = XianDuanChuLiStatus::AFTER_FREE_1;
                        }
                        else {
                            OutputDebugPrintf("AFTER_FREE_4 未处理");
                        }
                    }
                }
            }
        }
        break;
        }
    }
    return(ret_xd);
}

bool XianDuanChuLi::__middle_process(Bi bi) {
    FindXianDuanReturn ret_fd = FindXianDuanReturn();
    if (bi.get_high() >= this->left.get_high() && bi.get_low() <= this->left.get_low()) {
        //middle 包含 left
        this->start = bi.generate_bi(this->start, this->left, this->after_left);
        this->left = bi;
        this->status = XianDuanChuLiStatus::AFTER_LEFT;
        return(true);
    }
    else {
        this->middle = bi;
        this->status = XianDuanChuLiStatus::AFTER_MIDDLE;
        return(false);
    }
}

FindXianDuanReturn XianDuanChuLi::__right_process(Bi bi) {
    FindXianDuanReturn ret_xd = FindXianDuanReturn();
    if (this->middle.get_high() > this->left.get_high()) {
        if (bi.get_high() > this->middle.get_high()) {
            //上升处理
            if (this->start.get_type() == BiType::DOWN) {
                if (bi.get_high() > this->start.get_high()) {
                    ret_xd.xd1 = XianDuan(this->start, this->start);
                    ret_xd.xd2 = XianDuan(this->left, bi);
                    ret_xd.xd2.set_type(XianDuanType::TEMP_UP);
                    ret_xd.type = FindXianDuanReturnType::Two;
                    this->start = bi.generate_bi(this->left, this->after_left, this->middle);
                    this->left = this->middle;
                    this->after_left = bi;
                    this->status = XianDuanChuLiStatus::MIDDLE_HIGHLOW;
                    return(ret_xd);
                }
                else {
                    this->left = bi.generate_bi(this->left, this->after_left, this->middle);
                    this->after_left = this->after_middle;
                    this->__middle_process(bi);
                }
            }
            else {
                //顶分型
                if (this->middle.get_low() > this->left.get_high() + 0.01) {
                    //left 和 middle 有缺口
                    this->right = bi;
                    this->status = XianDuanChuLiStatus::AFTER_RIGHT;
                }
                else {
                    if (this->start.get_type() == BiType::UP) {
                        ret_xd.xd1 = XianDuan(this->start, this->after_left);
                        ret_xd.xd1.save_bi_list(this->get_xd_bi_list());
                        ret_xd.xd2 = XianDuan(this->middle, bi);
                        ret_xd.xd2.set_type(XianDuanType::TEMP_DOWN);
                        ret_xd.type = FindXianDuanReturnType::Two;
                        this->start = this->middle;
                        this->left = this->after_middle;
                        this->after_left = bi;
                        this->status = XianDuanChuLiStatus::MIDDLE_HIGHLOW;
                        return(ret_xd);
                    }
                    else {
                        ret_xd.xd1 = XianDuan(this->start, this->start);
                        ret_xd.xd1.save_bi_list(this->get_xd_bi_list());
                        ret_xd.xd2 = XianDuan(this->left, this->middle);
                        ret_xd.type = FindXianDuanReturnType::Two;
                        this->start = this->after_middle;
                        this->left = bi;
                        this->status = XianDuanChuLiStatus::AFTER_LEFT;
                        return(ret_xd);
                    }
                }

            }
        }
    }
    else {
        if (bi.get_low() < this->middle.get_low()) {
            //下降处理
            if (this->start.get_type() == BiType::UP) {
                if (bi.get_low() < this->start.get_low()) {
                    //创新低
                    ret_xd.xd1 = XianDuan(this->start, this->start);
                    ret_xd.xd2 = XianDuan(this->left, bi);
                    ret_xd.xd2.set_type(XianDuanType::TEMP_DOWN);
                    this->start = bi.generate_bi(this->left, this->after_left, this->middle);
                    this->left = this->after_middle;
                    this->after_left = bi;
                    this->status = XianDuanChuLiStatus::MIDDLE_HIGHLOW;
                    return(ret_xd);
                }
                else {
                    this->left = bi.generate_bi(this->left, this->after_left, this->middle);
                    this->after_left = this->after_middle;
                    this->__middle_process(bi);
                }
            }
            else {
                this->left = bi.generate_bi(this->left, this->after_left, this->middle);
                this->after_left = this->after_middle;
                this->__middle_process(bi);
            }
        }
        else {
            //底分型
            if (this->left.get_low() > this->middle.get_high() + 0.01) {
                //有缺口
                this->right = bi;
                this->status = XianDuanChuLiStatus::AFTER_RIGHT;
            }
            else {
                if (this->start.get_type() == BiType::DOWN) {
                    ret_xd.xd1 = XianDuan(this->start, this->after_left);
                    ret_xd.xd1.save_bi_list(this->get_xd_bi_list());
                    ret_xd.xd2 = XianDuan(this->middle, bi);
                    ret_xd.xd2.set_type(XianDuanType::TEMP_UP);
                    ret_xd.type = FindXianDuanReturnType::Two;
                    this->start = this->middle;
                    this->left = this->after_middle;
                    this->after_left = bi;
                    this->status = XianDuanChuLiStatus::MIDDLE_HIGHLOW;
                    return(ret_xd);
                }
                else {
                    ret_xd.xd1 = XianDuan(this->start, this->start);
                    ret_xd.xd1.save_bi_list(this->get_xd_bi_list());
                    ret_xd.xd2 = XianDuan(this->left, this->middle);
                    ret_xd.type = FindXianDuanReturnType::Two;
                    this->start = this->after_middle;
                    this->left = bi;
                    this->status = XianDuanChuLiStatus::AFTER_LEFT;
                    return(ret_xd);
                }
            }
        }
    }
    ret_xd.type = FindXianDuanReturnType::None;
    return(ret_xd);
}


void Bi3_xianduan(int nCount, float* pOut, float* pHigh, float* pLow, float* pIn) {
    BaoHanChuLi baohanChuli;
    XianDuanChuLi xdchuli;
    XianDuan xd;
    Bi bi;
    unsigned bi_status = 0;
    int pos, start_pos, stop_pos;

    for (int i = 0; i < nCount; i++) {
        baohanChuli.add(pHigh[i], pLow[i]);
    }

    for (int i = 0; i < nCount; i++) {
        pOut[i] = 0;
    }

    xdchuli.handle(baohanChuli.kxianList);

    unsigned int count = xdchuli.key_xianduan_list.size();

    if (!xdchuli.key_xianduan_list.empty()) {
        XianDuan start_xd = xdchuli.key_xianduan_list[0];
        start_pos = start_xd.get_start_pos();
        stop_pos = start_xd.get_stop_pos();

        if (start_xd.get_type() == XianDuanType::UP) {
            pOut[start_pos] = -3;
            pOut[stop_pos] = 3;
        }
        else {
            pOut[start_pos] = +3;
            pOut[stop_pos] = -3;
        }
        for (unsigned int i = 1; i < count; i++) {
            xd = xdchuli.key_xianduan_list[i];
            stop_pos = xd.get_stop_pos();
            switch (xd.get_type()) {
            case XianDuanType::TEMP_UP:
                pOut[stop_pos] = 1;
                break;
            case XianDuanType::FAILURE_TEMP_UP:
                pOut[stop_pos] = 2;
                break;
            case XianDuanType::UP:
                pOut[stop_pos] = 3;
                break;
            case XianDuanType::FAILURE_UP:
                pOut[stop_pos] = 4;
                break;
            case XianDuanType::TEMP_DOWN:
                pOut[stop_pos] = -1;
                break;
            case XianDuanType::FAILURE_TEMP_DOWN:
                pOut[stop_pos] = -2;
                break;
            case XianDuanType::DOWN:
                pOut[stop_pos] = -3;
                break;
            case XianDuanType::FAILURE_DOWN:
                pOut[stop_pos] = -4;
                break;
            }
        }
    }

    vector<Bi> bi_list = xdchuli.get_xd_bi_list();
    if (!bi_list.empty()) {
        bi = bi_list[0];
        pos = bi.get_start_pos();
        if (bi.get_type() == BiType::UP) {
            bi_status = 30;
            pOut[pos] = bi_status;
        }
        else {
            bi_status = 60;
            pOut[pos] = bi_status;
        }
    }
    for (unsigned int i = 1; i < bi_list.size(); i++) {
        bi = bi_list[i];
        pos = bi.get_start_pos();
        bi_status++;
        pOut[pos] = bi_status;
    }
    pos = bi.get_stop_pos();
    bi_status++;
    pOut[pos] = bi_status;

}

void Bi4_xianduan(int nCount, float* pOut, float* pHigh, float* pLow, float* pIn) {
    BaoHanChuLi baohanChuli;
    XianDuanChuLi xdchuli;
    XianDuan xd;
    int stop_pos;

    for (int i = 0; i < nCount; i++) {
        baohanChuli.add(pHigh[i], pLow[i]);
    }

    for (int i = 0; i < nCount; i++) {
        pOut[i] = 0;
    }

    xdchuli.handle(baohanChuli.kxianList);

    unsigned int count = xdchuli.key_xianduan_list.size();
    if (count == 0) {
        return;
    }

    XianDuan start_xd = xdchuli.key_xianduan_list[0];
    int start_pos = start_xd.get_start_pos();
    stop_pos = start_xd.get_stop_verify_pos();
    if (start_xd.get_type() == XianDuanType::UP) {
        pOut[start_pos] = -3;
        pOut[stop_pos] = +3;
    }
    else {
        pOut[start_pos] = +3;
        pOut[stop_pos] = -3;
    }
    for (unsigned int i = 1; i < count; i++) {
        xd = xdchuli.key_xianduan_list[i];
        stop_pos = xd.get_stop_verify_pos();
        switch (xd.get_type()) {
        case XianDuanType::TEMP_UP:
            pOut[stop_pos] = 1;
            break;
        case XianDuanType::FAILURE_TEMP_UP:
            pOut[stop_pos] = 2;
            break;
        case XianDuanType::UP:
            pOut[stop_pos] = 3;
            break;
        case XianDuanType::FAILURE_UP:
            pOut[stop_pos] = 4;
            break;
        case XianDuanType::TEMP_DOWN:
            pOut[stop_pos] = -1;
            break;
        case XianDuanType::FAILURE_TEMP_DOWN:
            pOut[stop_pos] = -2;
            break;
        case XianDuanType::DOWN:
            pOut[stop_pos] = -3;
            break;
        case XianDuanType::FAILURE_DOWN:
            pOut[stop_pos] = -4;
            break;
        }
    }
}
