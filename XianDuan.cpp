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
                    if (!this->key_xianduan_list.empty())
                        this->key_xianduan_list.pop_back();
                    this->key_xianduan_list.push_back(ret_fd.xd1);
                    this->xianDuanList.push_back(ret_fd.xd1);
                }
                break;

            case FindXianDuanReturnType::FindZhongShu:
                OutputDebugPrintf("Find ZhongShu:%d, %d, %f %f", ret_fd.zhongshu.get_start_pos(), ret_fd.zhongshu.get_stop_pos(), ret_fd.zhongshu.get_low(), ret_fd.zhongshu.get_high());
                this->zhongshu_list.push_back(ret_fd.zhongshu);
                break;

            case FindXianDuanReturnType::ZhongShuSuccess:
                OutputDebugPrintf("ZhongShu Success:%d, %d, %f %f", ret_fd.zhongshu.get_start_pos(), ret_fd.zhongshu.get_stop_pos(), ret_fd.zhongshu.get_low(), ret_fd.zhongshu.get_high());
                if (!this->zhongshu_list.empty()) {
                    this->zhongshu_list.pop_back();
                }
                this->zhongshu_list.push_back(ret_fd.zhongshu);
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

bool is_bi_equal(float bi1_len, float bi2_len) {
    float radio;

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

bool __determ_zhongshu(float start_len, float left_len, float after_len, float middle_len) {
    if (is_bi_equal(start_len, left_len) == false && is_bi_equal(left_len, after_len) && is_bi_equal(after_len, middle_len)) {
        return(true);
    }
    return(false);
}

FindXianDuanReturn XianDuanChuLi::failure_xd() {
    FindXianDuanReturn ret_fd = FindXianDuanReturn();
    XianDuan last_xd, before_last_xd;
    Bi xd_start_bi = Bi(), bi;

    vector<Bi> xd_bi_list(20);

    last_xd = this->get_last_xd(-1);
    if (last_xd.get_type() == XianDuanType::NONE) {
        ret_fd = this->set_xianduan(FindXianDuanReturnType::Two_Bi);
        this->status = XianDuanChuLiStatus::START;
    }
    else {
        switch (last_xd.get_type()) {
        case XianDuanType::UP:
            if (this->start.get_low() < last_xd.get_low()) {
                before_last_xd = this->get_last_xd(-2);
                if (before_last_xd.get_type() == XianDuanType::NONE) {
                    ret_fd = this->set_xianduan(FindXianDuanReturnType::Two_Bi);
                    this->status = XianDuanChuLiStatus::START;
                }
                else {
                    if (before_last_xd.get_type() == XianDuanType::UP && before_last_xd.get_high() < this->start.get_high()) {
                        ret_fd = this->set_xianduan(FindXianDuanReturnType::Two_Bi);
                        this->status = XianDuanChuLiStatus::START;

                    }
                    else {
                        xd_bi_list = before_last_xd.get_bi_list();
                        vector<Bi> last_xd_bi_list = last_xd.get_bi_list();
                        xd_bi_list.insert(xd_bi_list.end(), last_xd_bi_list.begin(), last_xd_bi_list.end());
                        xd_start_bi = before_last_xd.get_start_bi();
                        ret_fd.xd1 = XianDuan(xd_start_bi, this->start);
                        ret_fd.xd1.set_kind(XianDuanKind::ThreeBi);
                        ret_fd.xd1.save_bi_list(xd_bi_list);
                        xd_bi_list.clear();
                        xd_bi_list.push_back(this->left);
                        ret_fd.xd2 = XianDuan(this->left, this->left);
                        ret_fd.xd2.set_kind(XianDuanKind::SigleBi);
                        ret_fd.xd2.save_bi_list(xd_bi_list);
                        if (!this->key_xianduan_list.empty()) {
                            this->key_xianduan_list.pop_back();
                            if (!this->key_xianduan_list.empty())
                                this->key_xianduan_list.pop_back();
                        }
                        ret_fd.type = FindXianDuanReturnType::Two;
                        this->status = XianDuanChuLiStatus::START;
                    }
                }
            }
            else {
                this->after_left = this->left;
                this->left = this->start;
                this->start = bi.generate_bi(last_xd.get_start_bi(), Bi(), last_xd.get_stop_bi());
                if (!this->key_xianduan_list.empty())
                    this->key_xianduan_list.pop_back();
                if (is_bi_equal(this->start.get_length(), this->left.get_length()) == false && is_bi_equal(this->left.get_length(), this->after_left.get_length())) 
                    this->status = XianDuanChuLiStatus::MIDDLE_EQUAL;
                else 
                  this->status = XianDuanChuLiStatus::MIDDLE_HIGHLOW;
            }
            break;
        case XianDuanType::DOWN:
            if (this->start.get_high() > last_xd.get_high()) {
                before_last_xd = this->get_last_xd(-2);
                if (before_last_xd.get_type() == XianDuanType::NONE) {
                    ret_fd = this->set_xianduan(FindXianDuanReturnType::Two_Bi);
                    this->status = XianDuanChuLiStatus::START;
                }
                else {
                    if (before_last_xd.get_type() == XianDuanType::UP && before_last_xd.get_low() > this->start.get_low()) {
                        ret_fd = this->set_xianduan(FindXianDuanReturnType::Two_Bi);
                        this->status = XianDuanChuLiStatus::START;
                    }
                    else {
                        xd_bi_list.clear();
                        xd_bi_list = before_last_xd.get_bi_list();
                        vector<Bi> last_xd_bi_list = last_xd.get_bi_list();
                        xd_bi_list.insert(xd_bi_list.end(), last_xd_bi_list.begin(), last_xd_bi_list.end());
                        xd_start_bi = before_last_xd.get_start_bi();
                        ret_fd.xd1 = XianDuan(xd_start_bi, this->start);
                        ret_fd.xd1.set_kind(XianDuanKind::ThreeBi);
                        ret_fd.xd1.save_bi_list(xd_bi_list);
                        xd_bi_list.clear();
                        xd_bi_list.push_back(this->left);
                        ret_fd.xd2 = XianDuan(this->left, this->left);
                        ret_fd.xd2.set_kind(XianDuanKind::SigleBi);
                        ret_fd.xd2.save_bi_list(xd_bi_list);
                        if (!this->key_xianduan_list.empty()) {
                            this->key_xianduan_list.pop_back();
                            if (!this->key_xianduan_list.empty())
                                this->key_xianduan_list.pop_back();
                        }
                        ret_fd.type = FindXianDuanReturnType::Two;
                        this->status = XianDuanChuLiStatus::START;
                    }
                }
            }
            else {
                this->after_left = this->left;
                this->left = this->start;
                this->start = bi.generate_bi(last_xd.get_start_bi(), Bi(), last_xd.get_stop_bi());
                if (!this->key_xianduan_list.empty())
                    this->key_xianduan_list.pop_back();
                if (is_bi_equal(this->start.get_length(), this->left.get_length()) == false && is_bi_equal(this->left.get_length(), this->after_left.get_length()))
                    this->status = XianDuanChuLiStatus::MIDDLE_EQUAL;
                else
                    this->status = XianDuanChuLiStatus::MIDDLE_HIGHLOW;
            }
            break;
        }
    }
    return(ret_fd);
}

void XianDuanChuLi::push_bi_list() {
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
    case XianDuanChuLiStatus::LONGXIANDUAN_RIGHT_NORMAL:
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

FindXianDuanReturn XianDuanChuLi::set_xianduan(FindXianDuanReturnType type) {
    FindXianDuanReturn ret_xd;
    vector<Bi> xd_bi_list;

    switch (type) {
    case FindXianDuanReturnType::Two_Bi:
        xd_bi_list.clear();
        ret_xd.xd1 = XianDuan(this->start, this->start);
        xd_bi_list.push_back(this->start);
        ret_xd.xd1.set_kind(XianDuanKind::SigleBi);
        ret_xd.xd1.save_bi_list(xd_bi_list);
        ret_xd.xd2 = XianDuan(this->left, this->left);
        xd_bi_list.clear();
        xd_bi_list.push_back(this->left);
        ret_xd.xd2.set_kind(XianDuanKind::SigleBi);
        ret_xd.xd2.save_bi_list(xd_bi_list);
        ret_xd.type = FindXianDuanReturnType::Two;
        break;
    case FindXianDuanReturnType::Two:
        xd_bi_list.clear();
        ret_xd.xd1 = XianDuan(this->start, this->after_left);
        xd_bi_list.push_back(this->start);
        xd_bi_list.push_back(this->left);
        xd_bi_list.push_back(this->after_left);
        ret_xd.xd1.set_kind(XianDuanKind::LONGXIANDUAN);
        ret_xd.xd1.save_bi_list(xd_bi_list);
        ret_xd.xd2 = XianDuan(this->middle, this->right);
        xd_bi_list.clear();
        xd_bi_list.push_back(this->middle);
        xd_bi_list.push_back(this->after_middle);
        xd_bi_list.push_back(this->right);
        ret_xd.xd2.set_kind(XianDuanKind::LONGXIANDUAN);
        ret_xd.xd2.save_bi_list(xd_bi_list);
        ret_xd.type = FindXianDuanReturnType::Two;
        break;
    case FindXianDuanReturnType::One:
        ret_xd.xd1 = XianDuan(this->start, this->after_middle);
        xd_bi_list.clear();
        xd_bi_list.push_back(this->start);
        xd_bi_list.push_back(this->left);
        xd_bi_list.push_back(this->after_left);
        xd_bi_list.push_back(this->middle);
        xd_bi_list.push_back(this->after_middle);
        ret_xd.xd1.set_kind(XianDuanKind::LONGXIANDUAN);
        ret_xd.xd1.save_bi_list(xd_bi_list);
        ret_xd.type = FindXianDuanReturnType::One;
        break;
    }
    return(ret_xd);
}

FindXianDuanReturn  XianDuanChuLi::find_xianduan(Bi bi) {
    FindXianDuanReturn ret_fd;
    BiType bi_type;
    XianDuan xd = XianDuan();

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
    ret_fd.type = FindXianDuanReturnType::None;
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
    case XianDuanChuLiStatus::LEFT_INCLUDE_MIDDLE:
        return("LEFT_INCLUDE_MIDDLE");
    case XianDuanChuLiStatus::AFTER_MIDDLE:
        return("AFTER_MIDDLE");
    case XianDuanChuLiStatus::RIGHT:
        return("RIGHT");
    case XianDuanChuLiStatus::RIGHT_NORMAL:
        return("RIGHT_NORMAL");
    case XianDuanChuLiStatus::RIGHT_NORMAL_NORMAL:
        return("RIGHT_NORMAL_NORMAL");
    case XianDuanChuLiStatus::RIGHT_NORMAL_NORMAL_NORMAL:
        return("RIGHT_NORMAL_NORMAL_NORMAL");
    case XianDuanChuLiStatus::AFTER_RIGHT:
        return("AFTER_RIGHT");
    case XianDuanChuLiStatus::FREE:
        return("FREE");
    case XianDuanChuLiStatus::AFTER_FREE:
        return("AFTER_FREE");
    case XianDuanChuLiStatus::AFTER_FREE_1:
        return("AFTER_FREE_1");
    case XianDuanChuLiStatus::AFTER_FREE_2:
        return("AFTER_FREE_2");
    case XianDuanChuLiStatus::AFTER_FREE_3:
        return("AFTER_FREE_3");
    case XianDuanChuLiStatus::LONGXIANDUAN:
        return("LONGXIANDUAN");
    case XianDuanChuLiStatus::LONGXIANDUAN_RIGHT:
        return("LONGXIANDUAN_RIGHT");
    case XianDuanChuLiStatus::LONGXIANDUAN_RIGHT_NORMAL:
        return("LONGXIANDUAN_AFTER_RIGHT");
    case XianDuanChuLiStatus::LONGXIANDUAN_FREE:
        return("LONGXIANDUAN_FREE");
    case XianDuanChuLiStatus::LONGXIANDUAN_AFTER_FREE:
        return("LONGXIANDUAN_AFTER_FREE");
    case XianDuanChuLiStatus::LONGXIANDUAN_AFTER_FREE_1:
        return("LONGXIANDUAN_AFTER_FREE_1");
    case XianDuanChuLiStatus::A:
        return("A");
    case XianDuanChuLiStatus::B:
        return("B");
    default:
        return("Not Process Item");
    }
}


XianDuan XianDuanChuLi::get_last_xd(int num) {
    XianDuan ret_xd = XianDuan();
    int count = this->key_xianduan_list.size();
    if (count > 0) {
        if (num < 0 && abs(num) <= count) {
            ret_xd = this->key_xianduan_list[count + num];
        }
    }
    return(ret_xd);
}

FindXianDuanReturn XianDuanChuLi::__find_xianduan(Bi bi) {
    XianDuan xd = XianDuan();
    FindXianDuanReturn ret_xd;
    Bi_ZhongShu A_zs, B_zs;
    FindBiZhongShuReturn ret_zhongshu;
    int ret_cnt;
    XianDuan last_xd, before_last_xd;
    bool process_status;
    Bi tmp_bi = Bi();

    BiType bi_type = bi.get_type();
    float bi_high = bi.get_high();
    float bi_low = bi.get_low();
    vector<Bi> xd_bi_list;

    if (bi.get_type() == BiType::UP) {
        OutputDebugPrintf("上升笔 %f %f %d %d %s ", bi.get_low(), bi.get_high(), bi.get_start_pos(), bi.get_stop_pos(), get_xianduan_status(this->status));
    }
    else {
        OutputDebugPrintf("下降笔 %f %f %d %d %s ", bi.get_high(), bi.get_low(), bi.get_start_pos(), bi.get_stop_pos(), get_xianduan_status(this->status));
    }
    
    switch (this->status) {
    case XianDuanChuLiStatus::START:
        process_status = false;
        last_xd = this->get_last_xd(-1);
        switch (bi_type) {
        case BiType::UP:
            if (last_xd.get_type() == XianDuanType::DOWN) {
                before_last_xd = this->get_last_xd(-2);
                if (before_last_xd.get_type() == XianDuanType::UP) {
                    if (is_bi_equal(last_xd.get_length(), bi.get_length())) {
                        if (is_bi_equal(before_last_xd.get_length(), last_xd.get_length()) == false) {
                            this->start = bi.generate_bi(before_last_xd.get_start_bi(), Bi(), before_last_xd.get_stop_bi());
                            this->left = bi.generate_bi(last_xd.get_start_bi(), Bi(), last_xd.get_stop_bi());
                            this->after_left = bi;
                            this->status = XianDuanChuLiStatus::MIDDLE_EQUAL;
                            process_status = true;
                        }
                    }
                    else {
                        if (before_last_xd.get_low() < last_xd.get_low()) {
                            this->start = bi.generate_bi(before_last_xd.get_start_bi(), Bi(), before_last_xd.get_stop_bi());
                            this->left = bi.generate_bi(last_xd.get_start_bi(), Bi(), last_xd.get_stop_bi());
                            this->after_left = bi;
                            this->status = XianDuanChuLiStatus::MIDDLE_HIGHLOW;
                            process_status = true;
                        }
                    }
                }
                else {
                    if (last_xd.get_kind() == XianDuanKind::PANZHENG) {
                        if (bi.get_high() < this->A_zscl.get_Bi_ZhongShu().get_low()) {
                            this->b_1 = this->A_zscl.get_Bi_ZhongShu().get_output();
                            this->b_2 = bi;
                            this->status = XianDuanChuLiStatus::b_3;
                            process_status = true;
                        }
                    }
                }
            }
            break;
        case BiType::DOWN:
            if (last_xd.get_type() == XianDuanType::UP) {
                before_last_xd = this->get_last_xd(-2);
                if (before_last_xd.get_type() == XianDuanType::DOWN) {
                    if (is_bi_equal(last_xd.get_length(), bi.get_length())) {
                        if (is_bi_equal(before_last_xd.get_length(), last_xd.get_length() == false)) {
                            this->start = bi.generate_bi(before_last_xd.get_start_bi(), Bi(), before_last_xd.get_stop_bi());
                            this->left = bi.generate_bi(last_xd.get_start_bi(), Bi(), last_xd.get_stop_bi());
                            this->after_left = bi;
                            this->status = XianDuanChuLiStatus::MIDDLE_EQUAL;
                            process_status = true;
                        }
                        else {
                            if (before_last_xd.get_high() > last_xd.get_high()) {
                                this->start = bi.generate_bi(before_last_xd.get_start_bi(), Bi(), before_last_xd.get_stop_bi());
                                this->left = bi.generate_bi(last_xd.get_start_bi(), Bi(), last_xd.get_stop_bi());
                                this->after_left = bi;
                                this->status = XianDuanChuLiStatus::MIDDLE_HIGHLOW;
                                process_status = true;
                            }
                        }
                    }
                    else {
                        if (last_xd.get_kind() == XianDuanKind::PANZHENG) {
                            if (bi.get_high() < this->A_zscl.get_Bi_ZhongShu().get_low()) {
                                this->b_1 = this->A_zscl.get_Bi_ZhongShu().get_output();
                                this->b_2 = bi;
                                this->status = XianDuanChuLiStatus::b_3;
                                process_status = true;
                            }
                        }
                    }
                }
            }
            break;
        }
        if (process_status == false) {
            this->start = bi;
            this->status = XianDuanChuLiStatus::LEFT;
        }
        break;

    case XianDuanChuLiStatus::LEFT:
        if (this->start.get_type() == BiType::UP && bi.get_low() < this->start.get_low() || (this->start.get_type() == BiType::DOWN && bi.get_high() > this->start.get_high())) {
            if (this->key_xianduan_list.empty()) {
                xd_bi_list.clear();
                xd_bi_list.push_back(this->start);
                ret_xd.xd1 = XianDuan(this->start, this->start);
                ret_xd.xd1.save_bi_list(xd_bi_list);
                ret_xd.xd1.set_kind(XianDuanKind::SigleBi);
                this->left = bi;
                xd_bi_list.clear();
                xd_bi_list.push_back(this->left);
                ret_xd.xd2 = XianDuan(this->left, this->left);
                ret_xd.xd2.save_bi_list(xd_bi_list);
                ret_xd.type = FindXianDuanReturnType::Two;
                this->status = XianDuanChuLiStatus::START;
            }
            else {
                this->left = bi;
                return(this->failure_xd());
            }
        }
        else {
            this->left = bi;
            this->status = XianDuanChuLiStatus::AFTER_LEFT;
        }
        break;

    case XianDuanChuLiStatus::AFTER_LEFT:
        this->after_left = bi;
        if (is_bi_equal(this->start.get_length(), this->left.get_length()) == false && is_bi_equal(this->left.get_length(), bi.get_length())) {
            this->status = XianDuanChuLiStatus::MIDDLE_EQUAL;
        }
        else {
            switch (bi_type) {
            case BiType::UP:
                if (bi.get_high() > this->start.get_high()) {
                    this->status = XianDuanChuLiStatus::MIDDLE_HIGHLOW;
                    last_bi = bi;
                    return(ret_xd);
                }
                else {
                    this->status = XianDuanChuLiStatus::MIDDLE_NORMAL;
                }
                break;
            case BiType::DOWN:
                if (bi.get_low() < this->start.get_low()) {
                    this->status = XianDuanChuLiStatus::MIDDLE_HIGHLOW;
                    last_bi = bi;
                    return(ret_xd);
                }
                else {
                    this->status = XianDuanChuLiStatus::MIDDLE_NORMAL;
                }
                break;
            }
        }
        break;

    case XianDuanChuLiStatus::MIDDLE_EQUAL:
        if (is_bi_equal(bi.get_length(), this->after_left.get_length())) {
            this->A_zscl = Bi_ZhongShuChuLi(this->start, this->left, this->after_left, bi);
            this->status = XianDuanChuLiStatus::A;
        }
        else {
            switch (bi_type) {
            case BiType::UP:
                if (bi.get_high() > this->start.get_high()) {
                    if (this->start.get_low() < bi.get_low())
                        this->left = bi.generate_bi(this->left, this->after_left, bi);
                    else {
                        this->start = bi.generate_bi(this->start, this->left, this->after_left);
                        last_bi = bi;
                    }
                    return(this->failure_xd());
                }
                else {
                    this->middle = bi;
                    this->status = XianDuanChuLiStatus::MIDDLE_EQUAL_NORMAL;
                }
                break;
            case BiType::DOWN:
                if (bi.get_low() < this->start.get_low()) {
                    if (this->start.get_high() > bi.get_high())
                        this->left = bi.generate_bi(this->left, this->after_left, bi);
                    else {
                        this->start = bi.generate_bi(this->start, this->left, this->after_left);
                        last_bi = bi;
                    }
                    return(this->failure_xd());
                }
                else {
                    this->middle = bi;
                    this->status = XianDuanChuLiStatus::MIDDLE_EQUAL_NORMAL;
                }
                break;
            }
        }
        break;

    case XianDuanChuLiStatus::MIDDLE_EQUAL_NORMAL:
        switch (bi_type) {
        case BiType::UP:
            if (this->start.get_type() == BiType::UP) {
                if (bi_high > this->middle.get_high()) {
                    this->after_left = bi.generate_bi(this->after_left, this->middle, bi);
                    if (is_bi_equal(this->left.get_length(), this->after_left.get_length())) {
                        this->status = XianDuanChuLiStatus::MIDDLE_EQUAL;
                    }
                    else {
                        this->status = XianDuanChuLiStatus::MIDDLE_HIGHLOW;
                    }
                }
            }
            else {
                if (bi_high > this->start.get_high()) {
                    this->right = bi;
                    ret_xd = this->set_xianduan(FindXianDuanReturnType::Two);
                    this->status = XianDuanChuLiStatus::START;
                }
                else {
                    if (bi_high > this->middle.get_high()) {
                        this->middle = bi.generate_bi(this->middle, Bi(), bi);
                        if (is_bi_equal(this->after_left.get_length(), this->middle.get_length())) {
                            this->A_zscl = Bi_ZhongShuChuLi(this->start, this->left, this->after_left, this->middle);
                            this->status = XianDuanChuLiStatus::A;
                        }
                        else {
                            if (this->middle.get_high() > this->left.get_high()) {
                                this->start = bi.generate_bi(this->start, this->left, this->after_left);
                                this->left = this->middle;
                                this->status = XianDuanChuLiStatus::AFTER_LEFT;
                            }
                        }
                    }
                }
            }
            break;
        case BiType::DOWN:
            if (this->start.get_type() == BiType::DOWN) {
                if (bi_low < this->middle.get_low()) {
                    this->after_left = bi.generate_bi(this->after_left, this->middle, bi);
                    if (is_bi_equal(this->left.get_length(), this->after_left.get_length())) {
                        this->status = XianDuanChuLiStatus::MIDDLE_EQUAL;
                    }
                    else {
                        this->status = XianDuanChuLiStatus::MIDDLE_HIGHLOW;
                    }
                }
            }
            else {
                if (bi_low < this->start.get_low()) {
                    this->right = bi;
                    ret_xd = this->set_xianduan(FindXianDuanReturnType::Two);
                    this->status = XianDuanChuLiStatus::START;
                }
                else {
                    if (bi_low < this->middle.get_low()) {
                        this->middle = bi.generate_bi(this->middle, Bi(), bi);
                        if (is_bi_equal(this->after_left.get_length(), this->middle.get_length())) {
                            this->A_zscl = Bi_ZhongShuChuLi(this->start, this->left, this->after_left, this->middle);
                            this->status = XianDuanChuLiStatus::A;
                        }
                        else {
                            if (this->middle.get_low() < this->left.get_low()) {
                                this->start = bi.generate_bi(this->start, this->left, this->after_left);
                                this->left = this->middle;
                                this->status = XianDuanChuLiStatus::AFTER_LEFT;
                            }
                        }

                    }

                }
            }
        }
        break;

    case XianDuanChuLiStatus::MIDDLE_HIGHLOW:
        tmp_bi = bi.generate_bi(this->start, this->left, this->after_left);
        if (is_bi_equal(tmp_bi.get_length(), bi.get_length())) {
            XianDuan tmp_xd = this->get_last_xd(-1);
            if (tmp_xd.get_type() != XianDuanType::NONE && is_bi_equal(tmp_xd.get_length(), tmp_bi.get_length()) == false) {
                this->start = bi.generate_bi(tmp_xd.get_start_bi(), Bi(), tmp_xd.get_stop_bi());
                this->left = tmp_bi;
                this->after_left = bi;
                this->status = XianDuanChuLiStatus::MIDDLE_EQUAL;
                this->key_xianduan_list.pop_back();
            }
            else {
                ret_xd.xd1 = XianDuan(this->start, this->after_left);
                xd_bi_list = ret_xd.xd1.get_bi_list();
                xd_bi_list.push_back(this->start);
                xd_bi_list.push_back(this->left);
                xd_bi_list.push_back(this->after_left);
                ret_xd.xd1.set_kind(XianDuanKind::LONGXIANDUAN);
                ret_xd.xd2 = XianDuan(bi, bi);
                xd_bi_list = ret_xd.xd2.get_bi_list();
                xd_bi_list.push_back(bi);
                ret_xd.xd2.set_kind(XianDuanKind::LONGXIANDUAN);
                ret_xd.type = FindXianDuanReturnType::Two;
                this->status = XianDuanChuLiStatus::START;
            }
        }
        else {
            switch (bi_type) {
            case BiType::UP:
                if (bi_high > this->start.get_high()) {
                    ret_xd.xd1 = XianDuan(this->start, this->after_left);
                    xd_bi_list = ret_xd.xd1.get_bi_list();
                    xd_bi_list.push_back(this->start);
                    xd_bi_list.push_back(this->left);
                    xd_bi_list.push_back(this->after_left);
                    ret_xd.xd1.set_kind(XianDuanKind::LONGXIANDUAN);
                    ret_xd.type = FindXianDuanReturnType::One;
                    this->start = bi;
                    this->status = XianDuanChuLiStatus::LEFT;
                }
                else {
                    if (bi_high < this->start.get_low()) {
                        this->middle = bi;
                        this->status = XianDuanChuLiStatus::LONGXIANDUAN;
                    }
                    else {
                        this->__middle_process(bi);
                    }
                }
                break;
            case BiType::DOWN:
                if (bi_low < this->start.get_low()) {
                    ret_xd.xd1 = XianDuan(this->start, this->after_left);
                    xd_bi_list = ret_xd.xd1.get_bi_list();
                    xd_bi_list.push_back(this->start);
                    xd_bi_list.push_back(this->left);
                    xd_bi_list.push_back(this->after_left);
                    ret_xd.xd1.set_kind(XianDuanKind::LONGXIANDUAN);
                    ret_xd.type = FindXianDuanReturnType::One;
                    this->start = bi;
                    this->status = XianDuanChuLiStatus::LEFT;
                }
                else {
                    if (bi_low > this->start.get_high()) {
                        this->middle = bi;
                        this->status = XianDuanChuLiStatus::LONGXIANDUAN;
                    }
                    else {
                        this->__middle_process(bi);
                    }
                }
                break;
            }
        }
        break;

    case XianDuanChuLiStatus::MIDDLE_NORMAL:
        if (bi.get_type() == BiType::UP) {
            if (bi.get_high() > this->start.get_high()) {
                //创新高
                this->left = bi.generate_bi(this->left, this->after_left, bi);
                return(this->failure_xd());
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
                return(this->failure_xd());
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
        switch (bi.get_type()) {
        case BiType::UP:
            switch (this->start.get_type()) {
            case BiType::UP:
                if (bi.get_high() > this->start.get_high()) {
                    this->after_left = bi.generate_bi(this->after_left, this->middle, bi);
                    this->status = XianDuanChuLiStatus::MIDDLE_HIGHLOW;
                }
                else {
                    if (bi.get_high() > this->after_left.get_high()) {
                        this->after_left = bi.generate_bi(this->after_left, this->middle, bi);
                        this->status = XianDuanChuLiStatus::MIDDLE_NORMAL;
                    }
                }
                break;
            case BiType::DOWN:
                if (bi.get_high() > this->start.get_high()) {
                    this->left = bi.generate_bi(this->left, this->after_left, bi);
                    return(this->failure_xd());
                }
                else {
                    if (bi.get_high() > this->after_left.get_high()) {
                        this->after_left = bi.generate_bi(this->after_left, bi, bi);
                        this->status = XianDuanChuLiStatus::MIDDLE_NORMAL;
                    }
                }
                break;
            }
            break;

        case BiType::DOWN:
            switch (this->start.get_type()) {
            case BiType::UP:
                if (bi.get_low() < this->start.get_low()) {
                    this->left = bi.generate_bi(this->left, this->after_left, bi);
                    return(this->failure_xd());
                }
                else {
                    if (bi.get_low() < this->left.get_low()) {
                        this->after_left = bi.generate_bi(this->left, this->after_left, bi);
                        this->status = XianDuanChuLiStatus::MIDDLE_NORMAL;
                    }
                }
                break;
            case BiType::DOWN:
                if (bi.get_low() < this->start.get_low()) {
                    this->after_left = bi.generate_bi(this->after_left, this->middle, bi);
                    this->status = XianDuanChuLiStatus::MIDDLE_HIGHLOW;
                }
                else {
                    if (bi.get_low() < this->after_left.get_low()) {
                        this->after_left = bi.generate_bi(this->after_left, this->middle, bi);
                        this->status = XianDuanChuLiStatus::MIDDLE_NORMAL;
                    }
                }
                break;
            }
            break;
        }
        break;

    case XianDuanChuLiStatus::A:
        ret_zhongshu =  this->A_zscl.find_Bi_ZhongShu(bi);
        switch (ret_zhongshu.type) {
        case FindBiZhongShuReturnType::THREE_BUY:
            this->a = ret_zhongshu.bi_zhongshu.get_input();
            this->b_1 = ret_zhongshu.bi_zhongshu.get_output();
            this->A_zscl.get_Bi_ZhongShu().stop(this->b_1);
            this->b_2 = bi;
            ret_xd.zhongshu = this->A_zscl.get_Bi_ZhongShu();
            ret_xd.type = FindXianDuanReturnType::ZhongShuSuccess;
            this->status = XianDuanChuLiStatus::b_3;
            break;
        case FindBiZhongShuReturnType::THREE_SELL:
            this->a = ret_zhongshu.bi_zhongshu.get_input();
            this->b_1 = ret_zhongshu.bi_zhongshu.get_output();
            this->b_2 = bi;
            ret_xd.zhongshu = this->A_zscl.get_Bi_ZhongShu();
            ret_xd.type = FindXianDuanReturnType::ZhongShuSuccess;
            this->status = XianDuanChuLiStatus::b_3;
            break;
        case FindBiZhongShuReturnType::ZhongShuSuccess:
            this->a = ret_zhongshu.bi_zhongshu.get_input();
            this->b = bi;
            ret_xd.zhongshu = this->A_zscl.get_Bi_ZhongShu();
            ret_xd.type = FindXianDuanReturnType::ZhongShuSuccess;
            last_bi = this->b;
            this->status = XianDuanChuLiStatus::START;
            break;
        case FindBiZhongShuReturnType::ZhongShuFailer:
            this->a = ret_zhongshu.bi_zhongshu.get_input();
            this->b = ret_zhongshu.bi_zhongshu.get_output();
            ret_xd.zhongshu = this->A_zscl.get_Bi_ZhongShu();
            ret_xd.type = FindXianDuanReturnType::ZhongShuSuccess;
            this->start = bi;
            last_bi = bi;
            this->status = XianDuanChuLiStatus::LEFT;
            break;
        case FindBiZhongShuReturnType::ZHONSHU_UPGRADE:
            ret_xd.zhongshu = this->A_zscl.get_Bi_ZhongShu();
            ret_xd.type = FindXianDuanReturnType::ZhongShuUpgrade;
            this->status = XianDuanChuLiStatus::START;
            break;
        }
        break;

    case XianDuanChuLiStatus::b_3:
        switch (bi.get_type()) {
        case BiType::UP:
            if (bi.get_high() > this->b_1.get_high()) {
                this->b = bi.generate_bi(this->b_1, this->b_2, bi);
                if (this->b.get_length() > this->a.get_length())
                    this->status = XianDuanChuLiStatus::B_b1;
                else {
                    ret_xd.xd1 = XianDuan(this->a, this->b);
                    ret_xd.xd1.set_kind(XianDuanKind::PANZHENG);
                    ret_xd.xd1.get_bi_list().push_back(this->a);
                    ret_xd.xd1.get_bi_list().push_back(this->b);
                    ret_xd.type = FindXianDuanReturnType::One;
                    this->status = XianDuanChuLiStatus::START;
                }
            }
            else {
                this->b_3 = bi;
                this->status = XianDuanChuLiStatus::b_3_normal;
            }
            break;

        case BiType::DOWN:
            if (bi.get_low() < this->b_1.get_low()) {
                this->b = bi.generate_bi(this->b_1, this->b_2, bi);
                if (this->b.get_length() > this->a.get_length())
                    this->status = XianDuanChuLiStatus::B_b1;
                else {
                    ret_xd.xd1 = XianDuan(this->a, this->b);
                    ret_xd.xd1.set_kind(XianDuanKind::PANZHENG);
                    ret_xd.xd1.get_bi_list().push_back(this->a);
                    ret_xd.xd1.get_bi_list().push_back(this->b);
                    ret_xd.type = FindXianDuanReturnType::One;
                    this->status = XianDuanChuLiStatus::START;
                }
            }
            else {
                this->b_3 = bi;
                this->status = XianDuanChuLiStatus::b_3_normal;
            }
            break;
        }
        break;

    case XianDuanChuLiStatus::b_3_normal:
        switch (bi.get_type()) {
        case BiType::UP:
            if (this->b_2.get_type() == BiType::UP) {
                if (bi.get_high() > this->b_2.get_high()) {
                    if (bi.get_high() < this->A_zscl.get_Bi_ZhongShu().get_low()) {
                        this->b_2 = bi.generate_bi(this->b_2, this->b_3, bi);
                        this->status = XianDuanChuLiStatus::b_3;
                    }
                    else {
                        this->b = bi.generate_bi(this->b_1, this->b_2, bi);
                        ret_xd.xd1 = XianDuan(this->a, this->b);
                        ret_xd.xd1.set_kind(XianDuanKind::PANZHENG);
                        ret_xd.xd1.get_bi_list().push_back(this->a);
                        ret_xd.xd1.get_bi_list().push_back(this->b);
                        ret_xd.type = FindXianDuanReturnType::One;
                        this->start = this->b_2;
                        this->left = this->b_3;
                        this->after_left = bi;
                        this->status = XianDuanChuLiStatus::MIDDLE_HIGHLOW;
                    }
                }
            }
            else {
                //b_2是下降笔
                if (bi.get_high() > this->b_2.get_high()) {
                    this->b = bi.generate_bi(this->b_1, this->b_2, bi);
                    this->status = XianDuanChuLiStatus::B_b1;
                }
                else {
                    if (bi.get_high() > this->b_3.get_high()) {
                        this->b_3 = bi.generate_bi(this->b_3, bi, bi);
                        this->status = XianDuanChuLiStatus::b_3_normal;
                    }
                }
            }
            break;
        case BiType::DOWN:
            if (this->b_2.get_type() == BiType::DOWN) {
                if (bi.get_low() < this->b_2.get_low()) {
                    if (bi.get_low() > this->A_zscl.get_Bi_ZhongShu().get_high()) {
                        this->b_2 = bi.generate_bi(this->b_2, this->b_3, bi);
                        this->status = XianDuanChuLiStatus::b_3;
                    }
                    else {
                        this->b = bi.generate_bi(this->b_1, this->b_2, bi);
                        ret_xd.xd1 = XianDuan(this->a, this->b);
                        ret_xd.xd1.set_kind(XianDuanKind::PANZHENG);
                        ret_xd.xd1.get_bi_list().push_back(this->a);
                        ret_xd.xd1.get_bi_list().push_back(this->b);
                        ret_xd.type = FindXianDuanReturnType::One;
                        this->start = this->b_2;
                        this->left = this->b_3;
                        this->after_left = bi;
                        this->status = XianDuanChuLiStatus::MIDDLE_HIGHLOW;
                    }
                }
                else {
                    //b_2 是上升笔
                    if (bi.get_low() < this->b_2.get_low()) {
                        this->b = bi.generate_bi(this->b_1, this->b_2, bi);
                        this->status = XianDuanChuLiStatus::B_b1;
                    }
                    else {
                        if (bi.get_low() < this->b_3.get_low()) {
                            this->b_3 = bi.generate_bi(this->b_3, bi, bi);
                            this->status = XianDuanChuLiStatus::b_3_normal;
                        }
                    }
                }
            }
            break;
        }
        break;

    case XianDuanChuLiStatus::LONGXIANDUAN:
        if (bi.get_type() == BiType::UP) {
            if (bi.get_high() > this->middle.get_high()) {
                this->after_middle = bi;
                ret_xd = this->set_xianduan(FindXianDuanReturnType::One);
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
                this->after_middle = bi;
                ret_xd = this->set_xianduan(FindXianDuanReturnType::One);
                this->status = XianDuanChuLiStatus::START;
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
                return(this->failure_xd());
            }
            else {
                if (bi.get_high() > this->middle.get_high()) {
                    if (bi.get_high() < this->start.get_low()) {
                        this->middle = bi.generate_bi(this->middle, this->after_middle, bi);
                        this->status = XianDuanChuLiStatus::LONGXIANDUAN;
                    }
                    else {
                        this->right = bi;
                        this->status = XianDuanChuLiStatus::AFTER_RIGHT;
                    }
                }
                else {
                    this->right = bi;
                    this->status = XianDuanChuLiStatus::LONGXIANDUAN_RIGHT_NORMAL;
                }
            }
        }
        else {
            //下降笔
            if (bi.get_low() < this->start.get_low()) {
                this->start = bi.generate_bi(this->start, this->left, this->after_left);
                this->left = bi.generate_bi(this->middle, this->after_middle, bi);
                return(this->failure_xd());
            }
            else {
                if (bi.get_low() < this->middle.get_low()) {
                    if (bi.get_low() > this->start.get_high()) {
                        this->middle = bi.generate_bi(this->middle, this->after_middle, bi);
                        this->status = XianDuanChuLiStatus::LONGXIANDUAN;
                    }
                    else {
                        this->right = bi;
                        this->status = XianDuanChuLiStatus::AFTER_RIGHT;
                    }
                }
                else {
                    this->right = bi;
                    this->after_right = Bi();
                    this->status = XianDuanChuLiStatus::LONGXIANDUAN_RIGHT_NORMAL;
                }
            }
        }
        break;

    case XianDuanChuLiStatus::LONGXIANDUAN_RIGHT_NORMAL:
        switch (bi_type) {
        case BiType::UP:
            if (this->start.get_type() == BiType::UP) {
                if (bi_high > this->middle.get_high()) {
                    this->after_middle = bi.generate_bi(this->after_middle, this->right, bi);
                    ret_xd = this->set_xianduan(FindXianDuanReturnType::One);
                    this->status = XianDuanChuLiStatus::START;
                }
                else {
                    if (bi_high > this->after_middle.get_high()) {
                        this->after_middle = bi.generate_bi(this->after_middle, this->right, bi);
                        this->status = XianDuanChuLiStatus::LONGXIANDUAN_RIGHT;
                    }
                }
            }
            else {
                if (bi_high > this->start.get_high()) {
                    this->right = bi.generate_bi(this->right, Bi(), bi);
                    ret_xd = this->set_xianduan(FindXianDuanReturnType::Two);
                    this->status = XianDuanChuLiStatus::START;
                }
                else {
                    if (bi_high > this->middle.get_high()) {
                        if (bi_high < this->start.get_low()) {
                            this->middle = bi.generate_bi(this->middle, this->after_middle, bi);
                            this->status = XianDuanChuLiStatus::LONGXIANDUAN;
                        }
                        else {
                            this->start = bi.generate_bi(this->start, this->left, this->after_left);
                            this->left = bi.generate_bi(this->middle, this->after_middle, bi);
                            this->status = XianDuanChuLiStatus::AFTER_LEFT;
                        }
                    }
                }
            }
            break;
        case BiType::DOWN:
            if (this->start.get_type() == BiType::DOWN) {
                if (bi_low < this->middle.get_low()) {
                    this->after_middle = bi.generate_bi(this->after_middle, this->right, bi);
                    ret_xd = this->set_xianduan(FindXianDuanReturnType::One);
                    this->status = XianDuanChuLiStatus::START;
                }
                else {
                    if (bi_low < this->after_middle.get_low()) {
                        this->after_middle = bi.generate_bi(this->after_middle, this->right, bi);
                        this->status = XianDuanChuLiStatus::LONGXIANDUAN_RIGHT;
                    }
                }
            }
            else {
                if (bi_low < this->start.get_low()) {
                    this->right = bi.generate_bi(this->right, Bi(), bi);
                    ret_xd = this->set_xianduan(FindXianDuanReturnType::Two);
                    this->status = XianDuanChuLiStatus::START;
                }
                else {
                    if (bi_low < this->middle.get_low()) {
                        if (bi_low > this->start.get_high()) {
                            this->middle = bi.generate_bi(this->middle, this->after_middle, bi);
                            this->status = XianDuanChuLiStatus::LONGXIANDUAN;
                        }
                        else {
                            this->start = bi.generate_bi(this->start, this->left, this->after_left);
                            this->left = bi.generate_bi(this->middle, this->after_middle, bi);
                            this->status = XianDuanChuLiStatus::AFTER_LEFT;
                        }
                    }
                }
            }
            break;
        }

    case XianDuanChuLiStatus::LONGXIANDUAN_FREE:
        if (bi.get_type() == BiType::UP) {
            if (bi.get_high() > this->start.get_high()) {
                this->start = bi.generate_bi(this->start, this->left, this->after_left);
                this->left = bi.generate_bi(this->middle, this->after_middle, bi);
                return(this->failure_xd());
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
                        this->status = XianDuanChuLiStatus::LONGXIANDUAN_RIGHT_NORMAL;
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
                return(this->failure_xd());
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
                return(this->failure_xd());
            }
            else {
                if (bi.get_high() > this->middle.get_high()) {
                    if (bi.get_high() < this->start.get_low()) {
                        this->middle = bi.generate_bi(this->middle, this->after_middle, bi);
                        this->status = XianDuanChuLiStatus::LONGXIANDUAN;
                    }
                    else {
                        this->right = bi.generate_bi(this->right, this->after_right, bi);
                        this->status = XianDuanChuLiStatus::LONGXIANDUAN_RIGHT_NORMAL;
                    }
                }
                else {
                    if (bi.get_high() > this->right.get_high()) {
                        this->right = bi.generate_bi(this->right, this->after_right, bi);
                        this->status = XianDuanChuLiStatus::LONGXIANDUAN_RIGHT_NORMAL;
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
                return(this->failure_xd());
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
                        this->status = XianDuanChuLiStatus::LONGXIANDUAN_RIGHT_NORMAL;
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
                return(this->failure_xd());
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
                return(this->failure_xd());
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
        if (bi.get_type() == BiType::UP) {
            if (bi.get_high() > this->start.get_high()) {
                ret_xd.xd1 = XianDuan(this->start, this->after_left);
                ret_xd.xd2 = XianDuan(this->middle, bi);
                ret_xd.type = FindXianDuanReturnType::Two;
                this->status = XianDuanChuLiStatus::START;
            }
            else {
                if (bi.get_high() > this->start.get_low()) {
                    this->start = bi.generate_bi(this->start, this->left, this->after_left);
                    this->left = bi.generate_bi(this->middle, this->after_middle, bi);
                    this->status = XianDuanChuLiStatus::AFTER_LEFT;
                }
                else {
                    if (bi.get_high() > this->middle.get_high()) {
                        this->right = bi.generate_bi(this->right, this->after_right, bi);
                        this->status = XianDuanChuLiStatus::AFTER_RIGHT;
                    }
                    else {
                        if (bi.get_high() > this->right.get_high()) {
                            this->right = bi.generate_bi(this->right, this->after_right, bi);
                            this->status = XianDuanChuLiStatus::RIGHT_NORMAL_NORMAL;
                        } else {
                            this->after_right = bi;
                            this->status = XianDuanChuLiStatus::RIGHT_NORMAL_NORMAL_NORMAL_NORMAL;
                        }
                    }
                }
            }
        }
        else {
            if (bi.get_type() == BiType::DOWN) {
                if (bi.get_low() < this->start.get_low()){
                    ret_xd.xd1 = XianDuan(this->start, this->after_left);
                    ret_xd.xd2 = XianDuan(this->middle, bi);
                    ret_xd.type = FindXianDuanReturnType::Two;
                    this->status = XianDuanChuLiStatus::START;
                } else {
                    if (bi.get_low() < this->start.get_high()){
                        this->start = bi.generate_bi(this->start, this->left, this->after_left);
                        this->left = bi.generate_bi(this->middle, this->after_middle, bi);
                        this->status = XianDuanChuLiStatus::AFTER_LEFT;
                    } else {
                        if (bi.get_low() < this->middle.get_low()){
                            this->right = bi.generate_bi(this->right, this->after_right, bi);
                            this->status = XianDuanChuLiStatus::AFTER_RIGHT;
                        } else {
                            if (bi.get_low() < this->after_right.get_low()) {
                                                            this->after_right = bi;
                            this->status = XianDuanChuLiStatus::RIGHT_NORMAL_NORMAL_NORMAL_NORMAL;
                            }
                        }
                    }
                }
            }
        }
        break;
        
    case XianDuanChuLiStatus::RIGHT_NORMAL_NORMAL_NORMAL_NORMAL:
        OutputDebugPrintf("RIGHT_NORMAL_NORMAL_NORMAL_NORMAL 未处理");
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
                return(this->failure_xd());
            }
            if (bi.get_high() > this->after_right.get_high()) {
                ret_xd.xd1 = XianDuan(this->start, this->after_left);
                ret_xd.type = FindXianDuanReturnType::One;
                this->start = bi.generate_bi(this->middle, this->after_middle, this->right);
                this->left = this->after_right;
                this->after_left = bi;
                this->status = XianDuanChuLiStatus::MIDDLE_HIGHLOW;
            }
            else {
                ret_xd.xd1 = XianDuan(this->start, this->after_left);
                ret_xd.type = FindXianDuanReturnType::One;
                this->start = this->middle;
                this->left = this->after_middle;
                this->after_left = this->right;
                this->middle = this->after_right;
                this->after_middle = bi;
                this->status = XianDuanChuLiStatus::RIGHT;
            }
        }
        else {
            //下降笔
            if (bi.get_low() < this->start.get_low()) {
                //创新低
                this->start = bi.generate_bi(this->start, this->left, this->after_left);
                this->left = bi.generate_bi(this->middle, this->after_middle, bi);
                return(this->failure_xd());
            }
            if (bi.get_low() < this->after_right.get_low()) {
                ret_xd.xd1 = XianDuan(this->start, this->after_left);
                ret_xd.type = FindXianDuanReturnType::One;
                this->start = bi.generate_bi(this->middle, this->after_middle, this->right);
                this->left = this->after_right;
                this->after_left = bi;
                this->status = XianDuanChuLiStatus::MIDDLE_HIGHLOW;
            }
            else {
                ret_xd.xd1 = XianDuan(this->start, this->after_left);
                ret_xd.type = FindXianDuanReturnType::One;
                this->start = this->middle;
                this->left = this->after_middle;
                this->after_left = this->right;
                this->middle = this->after_right;
                this->after_middle = bi;
                this->status = XianDuanChuLiStatus::RIGHT;
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
                return(this->failure_xd());
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
                return(this->failure_xd());
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
                return(this->failure_xd());
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
                return(this->failure_xd());
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
    float bi_status = 0;
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
}

void show_xd_bi_list(vector<Bi> bi_list, float* pOut) {
    Bi bi;
    int pos, bi_status = 30;

    if (!bi_list.empty()) {
        bi = bi_list[0];
        pos = bi.get_start_pos();
        pOut[pos] = -4;

        for (unsigned int i = 0; i < bi_list.size(); i++) {
            bi = bi_list[i];
            pos = bi.get_stop_pos();
            switch (bi.get_type()) {
            case BiType::UP:
                pOut[pos] = 4;
                break;
            case BiType::DOWN:
                pOut[pos] = -4;
                break;
            }
        }
    }
}

void Bi3_xianduan_bi(int nCount, float* pOut, float* pHigh, float* pLow, float* pIn) {
    BaoHanChuLi baohanChuli;
    XianDuanChuLi xdchuli;
    XianDuan xd;
    Bi bi;
    float bi_status = 0;
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
        for (unsigned int i = 1; i < count; i++) {
            xd = xdchuli.key_xianduan_list[i];
            if (xd.get_type() == XianDuanType::UP || xd.get_type() == XianDuanType::DOWN) {
                show_xd_bi_list(xd.get_bi_list(), pOut);
            }
        }
    }
    show_xd_bi_list(xdchuli.get_xd_bi_list(), pOut);
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

void Bi3_bi_zhongshu(int nCount, float* pOut, float* pHigh, float* pLow, float* pIn) {
    BaoHanChuLi baohanChuli;
    XianDuanChuLi xdchuli;
    XianDuan xd;
    Bi bi;
    Bi_ZhongShu bi_zhongshu;

    float bi_status = 0;
    int pos, start_pos, stop_pos;

    for (int i = 0; i < nCount; i++) {
        baohanChuli.add(pHigh[i], pLow[i]);
    }

    for (int i = 0; i < nCount; i++) {
        pOut[i] = 0;
    }

    xdchuli.handle(baohanChuli.kxianList);

    unsigned int count = xdchuli.zhongshu_list.size();

    for (unsigned int i = 0; i < count; i++) {
        bi_zhongshu = xdchuli.zhongshu_list[i];
        pOut[bi_zhongshu.get_start_pos()] = 1;
        if (bi_zhongshu.get_output().get_type() != BiType::NONE)
            pOut[bi_zhongshu.get_stop_pos()] = 2;
    }
}

void Bi3_bi_zhongshu_high(int nCount, float* pOut, float* pHigh, float* pLow, float* pIn) {
    BaoHanChuLi baohanChuli;
    XianDuanChuLi xdchuli;
    XianDuan xd;
    Bi bi;
    Bi_ZhongShu bi_zhongshu;

    float bi_status = 0;
    int pos, start_pos, stop_pos;

    for (int i = 0; i < nCount; i++) {
        baohanChuli.add(pHigh[i], pLow[i]);
    }

    for (int i = 0; i < nCount; i++) {
        pOut[i] = 0;
    }

    xdchuli.handle(baohanChuli.kxianList);

    unsigned int count = xdchuli.zhongshu_list.size();

    for (unsigned int i = 0; i < count; i++) {
        bi_zhongshu = xdchuli.zhongshu_list[i];
        int start_pos = bi_zhongshu.get_start_pos();
        int stop_pos = bi_zhongshu.get_stop_pos();
        float high = bi_zhongshu.get_high();
        for (unsigned int j = start_pos; j < stop_pos; j++)
            pOut[j] = high;
    }
}

void Bi3_bi_zhongshu_low(int nCount, float* pOut, float* pHigh, float* pLow, float* pIn) {
    BaoHanChuLi baohanChuli;
    XianDuanChuLi xdchuli;
    XianDuan xd;
    Bi bi;
    Bi_ZhongShu bi_zhongshu;

    float bi_status = 0;
    int pos, start_pos, stop_pos;

    for (int i = 0; i < nCount; i++) {
        baohanChuli.add(pHigh[i], pLow[i]);
    }

    for (int i = 0; i < nCount; i++) {
        pOut[i] = 0;
    }

    xdchuli.handle(baohanChuli.kxianList);

    unsigned int count = xdchuli.zhongshu_list.size();

    for (unsigned int i = 0; i < count; i++) {
        bi_zhongshu = xdchuli.zhongshu_list[i];
        int start_pos = bi_zhongshu.get_start_pos();
        int stop_pos = bi_zhongshu.get_stop_pos();
        float low = bi_zhongshu.get_low();
        for (unsigned int j = start_pos; j < stop_pos; j++)
            pOut[j] = low;
    }
}
