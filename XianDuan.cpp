#include "XianDuan.h"

bool is_bi_equal(Bi bi1, Bi bi2) {
    float radio;
    float bi1_len = bi1.get_length();
    float bi2_len = bi2.get_length();

    if (bi1_len <= bi2_len) {
        radio = bi1_len / bi2_len;
    }
    else {
        radio = bi2_len / bi1_len;
    }

    if (radio >= 0.618) {
        return(true);
    }
    else {
        return(false);
    }
}

XianDuanChuLi::XianDuanChuLi() {
    this->last_xd = XianDuan();
    this->last_bi = Bi();
    this->bicl = BiChuLi();
    this->status = XianDuanChuLiStatus::a_1;
}


void XianDuanChuLi::handle(vector<Kxian1>& kxianList) {
    FindXianDuanReturn ret_fd;
    XianDuan xd = XianDuan();
    Bi tmp_bi = Bi();
    Bi bi = Bi();
    vector<Bi> bi_list;
    vector <Bi> pre_bi_list;
    vector<Bi_ZhongShu> zhongshu_list;
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

bool __is_zhongshu(Bi bi1, Bi bi2, Bi bi3) {
    float radio_bi1_bi2, radio_bi2_bi3;
    float bi1_len = bi1.get_length();
    float bi2_len = bi2.get_length();
    float bi3_len = bi3.get_length();
    if (bi1_len < bi2_len)
        radio_bi1_bi2 = bi1_len / bi2_len;
    else
        radio_bi1_bi2 = bi2_len / bi1_len;

    if (bi2_len < bi3_len)
        radio_bi2_bi3 = bi2_len / bi3_len;
    else
        radio_bi2_bi3 = bi3_len / bi2_len;

    if (radio_bi1_bi2 >= 0.618 && radio_bi2_bi3 >= 0.618) {
        if (bi1.get_type() == BiType::UP) {
            if (bi3.get_low() > bi1.get_high()) {
                return(false);
            }
        }
        else {
            if (bi3.get_high() < bi1.get_low()) {
                return(false);
            }
        }
        return(true);
    }
    else {
        return(false);
    }
}


bool __isMiddleEquale(Bi start, Bi left, Bi after_left) {
    float start_radio, left_radio;

    float start_len = start.get_length();
    float left_len = left.get_length();
    float after_len = after_left.get_length();

    if (start_len > left_len) {
        start_radio = left_len / start_len;
        if (start_radio < 0.5) {
            if (left_len < after_len)
                left_radio = left_len / after_len;
            else
                left_radio = after_len / left_len;
            if (left_radio >= 0.618) {
                if (start.get_type() == BiType::UP && start.get_low() < left.get_low())
                    return(true);
                if (start.get_type() == BiType::DOWN && start.get_high() > left.get_high())
                    return(true);
            }
        }
    }
    return(false);
}


FindXianDuanReturn XianDuanChuLi::failure_xd(Bi bi1, Bi bi2) {
    FindXianDuanReturn ret_fd = FindXianDuanReturn();
    XianDuan last_xd, before_last_xd;
    Bi xd_start_bi = Bi(), bi;

    vector<Bi> xd_bi_list(20);
    /*
    last_xd = this->get_last_xd(-1);
    if (last_xd.get_type() == XianDuanType::NONE) {
        ret_fd = this->set_xianduan(FindXianDuanReturnType::Two_Bi);
        this->status = XianDuanChuLiStatus::a_1;
    }
    else {
        switch (last_xd.get_type()) {
        case XianDuanType::UP:
            if (this->a_1.get_low() < last_xd.get_low()) {
                before_last_xd = this->get_last_xd(-2);
                if (before_last_xd.get_type() == XianDuanType::NONE) {
                    ret_fd = this->set_xianduan(FindXianDuanReturnType::Two_Bi);
                    this->status = XianDuanChuLiStatus::a_1;
                }
                else {
                    if ((before_last_xd.get_type() == XianDuanType::UP && before_last_xd.get_high() < this->start.get_high())
                         || (before_last_xd.get_type() == XianDuanType::DOWN && before_last_xd.get_low() > this->start.get_low())) {
                        ret_fd = this->set_xianduan(FindXianDuanReturnType::Two_Bi);
                        this->status = XianDuanChuLiStatus::START;
                    } else {
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
                xd_bi_list = last_xd.get_bi_list();
                xd_bi_list.push_back(this->start);
                xd_bi_list.push_back(this->left);
                ret_fd.xd1 = XianDuan(last_xd.get_start_bi(), this->left);
                ret_fd.xd1.save_bi_list(xd_bi_list);
                ret_fd.xd1.set_kind(XianDuanKind::LONGXIANDUAN);
                ret_fd.type = FindXianDuanReturnType::NewXianDuan;
                this->status = XianDuanChuLiStatus::START;
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
                xd_bi_list = last_xd.get_bi_list();
                xd_bi_list.push_back(this->start);
                xd_bi_list.push_back(this->left);
                ret_fd.xd1 = XianDuan(last_xd.get_start_bi(), this->left);
                ret_fd.xd1.save_bi_list(xd_bi_list);
                ret_fd.xd1.set_kind(XianDuanKind::LONGXIANDUAN);
                ret_fd.type = FindXianDuanReturnType::NewXianDuan;
                this->status = XianDuanChuLiStatus::START;
            }
            break;
        }
    }
    */
    return(ret_fd);
}

void XianDuanChuLi::push_bi_list() {
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
   }
    ret_fd.type = FindXianDuanReturnType::None;
    return(ret_fd);
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

vector<Bi> XianDuanChuLi::normal_process(Bi bi1, Bi bi2, Bi bi3){
    vector<Bi> ret_bi_list;
    Bi new_bi = Bi();
    switch (bi3.get_type()) {
    case BiType::UP:
        if (bi3.get_high() > bi2.get_high()) {
            new_bi = bi3.generate_bi(bi2, Bi(), bi3);
            ret_bi_list.push_back(bi1);
            ret_bi_list.push_back(new_bi);
        }
        break;
    case BiType::DOWN:
        if (bi3.get_low() < bi2.get_low()) {
            new_bi = bi3.generate_bi(bi1, bi2, bi3);
            ret_bi_list.push_back(new_bi);
        }
        break;
    }
    return(ret_bi_list);
}

vector<Bi> XianDuanChuLi::pre_zhongshu_process(Bi input, Bi bi1, Bi bi2, Bi bi3) {
    vector<Bi> ret_bi_list;
    Bi new_bi = Bi();
    switch (bi3.get_type()) {
        if (is_bi_equal(bi2, bi3)) {
            ret_bi_list.push_back(input);
            ret_bi_list.push_back(bi1);
            ret_bi_list.push_back(bi2);
            ret_bi_list.push_back(bi3);
        }
        else {
            switch (bi3.get_type()) {
            case BiType::UP:
                if (bi3.get_high() > bi2.get_high()) {
                    if (bi1.get_low() < bi3.get_low()) {
                        ret_bi_list.push_back(input);
                        new_bi = bi3.generate_bi(bi1, bi2, bi3);
                        ret_bi_list.push_back(new_bi);
                    }
                    else {
                        if (input.get_high() == 0) {
                            ret_bi_list.push_back(bi3);
                        }
                        else {
                            if (input.get_high() > bi1.get_high()) {
                                new_bi = bi3.generate_bi(input, bi1, bi2);
                                ret_bi_list.push_back(new_bi);
                                ret_bi_list.push_back(bi3);
                            }
                            else {
                                OutputDebugPrintf("pre_zhongshu_process 出现异常情况！");
                            }
                        }
                    }
                }
                break;
            case BiType::DOWN:
                if (bi3.get_low() < bi2.get_low()) {
                    if (bi1.get_high() > bi2.get_high()) {
                        ret_bi_list.push_back(input);
                        new_bi = bi3.generate_bi(bi1, bi2, bi3);
                        ret_bi_list.push_back(new_bi);
                    }
                    else {
                        if (input.get_low() == 0) {
                            ret_bi_list.push_back(bi3);
                        }
                        else {
                            if (input.get_low() < bi1.get_low()) {
                                new_bi = bi3.generate_bi(input, bi1, bi2);
                                ret_bi_list.push_back(new_bi);
                                ret_bi_list.push_back(bi3);
                            }
                            else {
                                OutputDebugPrintf("pre_zhongshu_process 出现异常情况！");
                            }
                        }
                    }
                }
                break;
            }
        }
    }
    return(ret_bi_list);
}

bool is_zhongshu_equal(ZhongShu zs, Bi bi) {
    float zs_length = zs.get_high() - zs.get_low();
    float radio;
    if (zs_length >= bi.get_length())
        radio = bi.get_length() / zs_length;
    else
        radio = zs_length / bi.get_length();
    if (radio >= 0.618)
        return(true);
    else
        return(false);
}

 FindZhongShuReturn XianDuanChuLi::zhongshu_process(ZhongShu zs, Bi bi) {
     FindZhongShuReturn ret_zs;

    if (!is_zhongshu_equal(zs, bi)) {
        switch (bi.get_type()) {
        case BiType::UP:
            if (bi.get_high() <= zs.get_low()) {
                //三卖
                if (zs.get_input().get_type() == BiType::UP) {

                }
            }
            break;
        case BiType::DOWN:
            if (bi.get_low() >= zs.get_high()) {
                //三买
                this->b_1 = this->last_bi;
                this->b_2 = bi;
                this->status = XianDuanChuLiStatus::b_3;
            }
            break;
        }
        this->last_bi = bi;
    }
    if (bi.get_high() > zs.get_max_high()) {
        zs.set_max_bi(bi);
    }
    else {
        if (bi.get_low() < zs.get_min_low()) {
            zs.set_min_bi(bi);
        }
    }
}

FindXianDuanReturn XianDuanChuLi::__find_xianduan(Bi bi) {
    FindXianDuanReturn ret_xd;
    BiType bi_type = bi.get_type();
    float bi_high = bi.get_high();
    float bi_low = bi.get_low();
    float bi_length = bi.get_length();
    vector<Bi> xd_bi_list;
    vector<Bi> normal_bi_list;
    vector<Bi> pre_zhongshu_list;
    Bi input;
    int count;
    Bi tmp_bi;
    XianDuan xd;

    if (bi_type == BiType::UP) {
        OutputDebugPrintf("上升笔 %f %f %d %d %f %s ", bi.get_low(), bi.get_high(), bi.get_start_pos(), bi.get_stop_pos(), bi.get_length(), this->status);
    }
    else {
        OutputDebugPrintf("下降笔 %f %f %d %d %f %s ", bi.get_high(), bi.get_low(), bi.get_start_pos(), bi.get_stop_pos(), bi.get_length(), this->status);
    }

    switch(this->status) {
        case XianDuanChuLiStatus::a_1:
            XianDuanType last_xd_type;
            this->a_1 = bi;
            this->status = XianDuanChuLiStatus::a_2;
            break;

        case XianDuanChuLiStatus::a_2:
            if (is_bi_equal(this->a_1, bi)) {
                this->a_2 = bi;
                this->status = XianDuanChuLiStatus::a1_equal_a2;
            }
            else {
                switch (bi_type) {
                case BiType::UP:
                    if (bi_high > this->a_1.get_high()) {
                        return(this->failure_xd(this->a_1, bi));
                    }
                    else {
                        this->a_2 = bi;
                        this->status = XianDuanChuLiStatus::a_3;
                    }
                    break;
                case BiType::DOWN:
                    if (bi_low < this->a_1.get_low()) {
                        return(this->failure_xd(this->a_1, bi));
                    }
                    else {
                        this->a_2 = bi;
                        this->status = XianDuanChuLiStatus::a_3;
                    }
                    break;
                }
            }
            break;

        case XianDuanChuLiStatus::a1_equal_a2:
            pre_zhongshu_list = this->pre_zhongshu_process(Bi(), this->a_1, this->a_2, bi);
            count = pre_zhongshu_list.size();
            switch (count) {
            case 4:
                this->A = ZhongShu(pre_zhongshu_list[0], pre_zhongshu_list[1], pre_zhongshu_list[2], pre_zhongshu_list[3], Bi());
                this->status = XianDuanChuLiStatus::A;
                break;
            case 2:
                tmp_bi = pre_zhongshu_list[0];
                if (tmp_bi.get_high() == 0) {
                    this->a_1 = pre_zhongshu_list[1];
                    this->status = XianDuanChuLiStatus::a_2;
                }
                else {
                    this->a_1 = tmp_bi;
                    this->a_2 = pre_zhongshu_list[2];
                    switch (this->a_2.get_type()) {
                    case BiType::UP:
                        if (this->a_2.get_high() > this->a_1.get_high()) {
                            return(this->failure_xd(this->a_1, this->a_2));
                        }
                        else {
                            this->status = XianDuanChuLiStatus::a_3;
                        }
                        break;
                    case BiType::DOWN:
                        if (this->a_2.get_low() < this->a_1.get_low()) {
                            return(this->failure_xd(this->a_1, this->a_2));
                        }
                        else {
                            this->status = XianDuanChuLiStatus::a_3;
                        }
                        break;
                    }
                }
                break;
            case 1:
                this->a_1 = pre_zhongshu_list[0];
                this->status = XianDuanChuLiStatus::a_2;
                break;
            }
            break;

        case XianDuanChuLiStatus::a2_equal_a3:
            pre_zhongshu_list = this->pre_zhongshu_process(Bi(), this->a_1, this->a_2, bi);
            count = pre_zhongshu_list.size();
            switch (count) {
            case 4:
                this->A = ZhongShu(pre_zhongshu_list[0], pre_zhongshu_list[1], pre_zhongshu_list[2], pre_zhongshu_list[3], Bi());
                this->status = XianDuanChuLiStatus::A;
                break;
            case 2:
                tmp_bi = pre_zhongshu_list[0];
                if (tmp_bi.get_high() == 0) {
                    this->a_1 = pre_zhongshu_list[1];
                    this->status = XianDuanChuLiStatus::a_2;
                }
                else {
                    this->a_1 = tmp_bi;
                    this->a_2 = pre_zhongshu_list[2];
                    switch (this->a_2.get_type()) {
                    case BiType::UP:
                        if (this->a_2.get_high() > this->a_1.get_high()) {
                            return(this->failure_xd(this->a_1, this->a_2));
                        }
                        else {
                            this->status = XianDuanChuLiStatus::a_3;
                        }
                        break;
                    case BiType::DOWN:
                        if (this->a_2.get_low() < this->a_1.get_low()) {
                            return(this->failure_xd(this->a_1, this->a_2));
                        }
                        else {
                            this->status = XianDuanChuLiStatus::a_3;
                        }
                        break;
                    }
                }
                break;
            case 1:
                this->a_1 = pre_zhongshu_list[0];
                this->status = XianDuanChuLiStatus::a_2;
                break;
            }
            break;

        case XianDuanChuLiStatus::a_3:
            this->a_3 = bi;
            if (is_bi_equal(this->a_2, bi)) {
                this->status = XianDuanChuLiStatus::a2_equal_a3;
            }
            else {
                switch (bi_type) {
                case BiType::UP:
                    if (bi_high > this->a_1.get_high())
                        this->status = XianDuanChuLiStatus::a3_highlow_a1;
                    else
                        this->status = XianDuanChuLiStatus::a3_normal_a1;
                    break;
                case BiType::DOWN:
                    if (bi_low < this->a_1.get_low())
                        this->status = XianDuanChuLiStatus::a3_highlow_a1;
                    else
                        this->status = XianDuanChuLiStatus::a3_normal_a1;
                    break;
                }
            }
            break;

        case XianDuanChuLiStatus::a3_highlow_a1:
            switch (bi_type) {
            case BiType::UP:
                if (bi_high < this->a_1.get_low()) {
                    this->a_4 = bi;
                    this->status = XianDuanChuLiStatus::longxianduan;
                }
                else {
                    this->a_1 = bi.generate_bi(this->a_1, this->a_2, this->a_3);
                    if (is_bi_equal(this->a_1, bi)) {
                        this->a_2 = bi;
                        this->status = XianDuanChuLiStatus::a1_equal_a2;
                    }
                    else {
                        if (bi.get_high() > this->a_1.get_high()) {
                            return(this->failure_xd(this->a_1, bi));
                        }
                        else {
                            this->a_2 = bi;
                            this->status = XianDuanChuLiStatus::a_3;
                        }
                    }
                }
                break;
            case BiType::DOWN:
                if (bi_low > this->a_1.get_high()) {
                    this->a_4 = bi;
                    this->status = XianDuanChuLiStatus::longxianduan;
                }
                else {
                    this->a_1 = bi.generate_bi(this->a_1, this->a_2, this->a_3);
                    if (is_bi_equal(this->a_1, bi)) {
                        this->a_2 = bi;
                        this->status = XianDuanChuLiStatus::a1_equal_a2;
                    }
                    else {
                        if (bi.get_low() < this->a_1.get_low()) {
                            return(this->failure_xd(this->a_1, bi));
                        }
                        else {
                            this->a_2 = bi;
                            this->status = XianDuanChuLiStatus::a_3;
                        }
                    }
                }
                break;
            }
            break;

        case XianDuanChuLiStatus::a3_normal_a1:
            switch (bi_type) {
            case BiType::UP:
                if (bi_high > this->a_2.get_high()) {
                    this->a_2 = bi.generate_bi(this->a_2, this->a_3, bi);
                    if (is_bi_equal(this->a_1, this->a_2)) {
                        this->status = XianDuanChuLiStatus::a1_equal_a2;
                    }
                    else {
                        if (this->a_2.get_high() > this->a_1.get_high()) {
                            return(this->failure_xd(this->a_1, this->a_2));
                        }
                        else {
                            this->status = XianDuanChuLiStatus::a_2;
                        }
                    }
                }
                else {
                    normal_bi_list = this->normal_process(this->a_2, this->a_3, bi);
                    count = normal_bi_list.size();
                    switch (count) {
                    case 2:
                        this->a_2 = normal_bi_list[0];
                        this->a_3 = normal_bi_list[1];
                        if (is_bi_equal(this->a_2, this->a_3)) {
                            this->status = XianDuanChuLiStatus::a2_equal_a3;
                        }
                        else {
                            switch (this->a_3.get_type()) {
                            case BiType::UP:
                                if (this->a_3.get_high() > this->a_1.get_high()) {
                                    this->status = XianDuanChuLiStatus::a3_highlow_a1;
                                }
                                else {
                                    this->status = XianDuanChuLiStatus::a3_normal_a1;
                                }
                                break;
                            case BiType::DOWN:
                                if (this->a_3.get_low() < this->a_1.get_low()) {
                                    this->status = XianDuanChuLiStatus::a3_highlow_a1;
                                }
                                else {
                                    this->status = XianDuanChuLiStatus::a3_normal_a1;
                                }
                                break;
                            }
                        }
                        break;
                    case 1:
                        this->a_2 = normal_bi_list[0];
                        switch (this->a_2.get_type()) {
                        case BiType::UP:
                            if (this->a_2.get_high() > this->a_1.get_high()) {
                                return(this->failure_xd(this->a_1, this->a_2));
                            }
                            else {
                                this->status = XianDuanChuLiStatus::a_2;
                            }
                            break;
                        case BiType::DOWN:
                            if (this->a_2.get_low() < this->a_1.get_low()) {
                                return(this->failure_xd(this->a_1, this->a_2));
                            }
                            else {
                                this->status = XianDuanChuLiStatus::a_2;
                            }
                            break;
                        }
                        break;
                    default:
                        break;
                    }
                }
                break;
            case BiType::DOWN:
                if (bi_low < this->a_2.get_low()) {
                    this->a_2 = bi.generate_bi(this->a_2, this->a_3, bi);
                    if (is_bi_equal(this->a_1, this->a_2)) {
                        this->status = XianDuanChuLiStatus::a1_equal_a2;
                    }
                    else {
                        if (bi.get_low() < this->a_1.get_low()) {
                            return(this->failure_xd(this->a_1, this->a_2));
                        }
                        else {
                            this->status = XianDuanChuLiStatus::a_2;
                        }
                    }
                }
                else {
                    normal_bi_list = this->normal_process(this->a_2, this->a_3, bi);
                    count = normal_bi_list.size();
                    switch (count) {
                    case 2:
                        this->a_2 = normal_bi_list[0];
                        this->a_3 = normal_bi_list[1];
                        if (is_bi_equal(this->a_2, this->a_3)) {
                            this->status = XianDuanChuLiStatus::a2_equal_a3;
                        }
                        else {
                            switch (this->a_3.get_type()) {
                            case BiType::UP:
                                if (this->a_3.get_high() > this->a_1.get_high()) {
                                    this->status = XianDuanChuLiStatus::a3_highlow_a1;
                                }
                                else {
                                    this->status = XianDuanChuLiStatus::a3_normal_a1;
                                }
                                break;
                            case BiType::DOWN:
                                if (this->a_3.get_low() < this->a_1.get_low()) {
                                    this->status = XianDuanChuLiStatus::a3_highlow_a1;
                                }
                                else {
                                    this->status = XianDuanChuLiStatus::a3_normal_a1;
                                }
                                break;
                            }
                        }
                        break;
                    case 1:
                        this->a_2 = normal_bi_list[0];
                        switch (this->a_2.get_type()) {
                        case BiType::UP:
                            if (this->a_2.get_high() > this->a_1.get_high()) {
                                return(this->failure_xd(this->a_1, this->a_2));
                            }
                            else {
                                this->status = XianDuanChuLiStatus::a_2;
                            }
                            break;
                        case BiType::DOWN:
                            if (this->a_2.get_low() < this->a_1.get_low()) {
                                return(this->failure_xd(this->a_1, this->a_2));
                            }
                            else {
                                this->status = XianDuanChuLiStatus::a_2;
                            }
                            break;
                        }
                        break;
                    default:
                        break;
                    }
                }
                break;
            }
            break;

        case XianDuanChuLiStatus::longxianduan:
            if (is_bi_equal(this->a_4, bi)) {
                this->a_1 = bi.generate_bi(this->a_1, this->a_2, this->a_3);
                this->a_2 = this->a_4;
                this->a_3 = bi;
                this->status = XianDuanChuLiStatus::a2_equal_a3;
            }
            else {
                switch (bi_type) {
                case BiType::UP:
                    if (bi_high > this->a_3.get_high()) {
                        //长线段处理
                        last_xd = XianDuan(this->a_1, bi);
                        last_xd.set_kind(XianDuanKind::LONGXIANDUAN);
                        ret_xd.type = FindXianDuanReturnType::One;
                        ret_xd.xd1 = last_xd;
                        this->status = XianDuanChuLiStatus::a_1;
                    }
                    else {
                        this->a_5 = bi;
                        this->status = XianDuanChuLiStatus::longxianduan_normal;
                    }
                    break;
                case BiType::DOWN:
                    if (bi_low < this->a_3.get_low()) {
                        //长线段处理
                        last_xd = XianDuan(this->a_1, bi);
                        last_xd.set_kind(XianDuanKind::LONGXIANDUAN);
                        ret_xd.type = FindXianDuanReturnType::One;
                        ret_xd.xd1 = last_xd;
                        this->status = XianDuanChuLiStatus::a_1;
                    }
                    else {
                        this->a_5 = bi;
                        this->status = XianDuanChuLiStatus::longxianduan_normal;
                    }
                    break;
                }
            }
            break;

        case XianDuanChuLiStatus::longxianduan_normal:
            this->a_1 = bi.generate_bi(this->a_1, this->a_2, this->a_3);
            switch (bi_type) {
            case BiType::UP:
                if (bi_high > this->a_4.get_high()) {
                    tmp_bi = bi.generate_bi(this->a_4, this->a_5, bi);
                    if (is_bi_equal(this->a_1, tmp_bi)) {
                        this->a_2 = tmp_bi;
                        this->status = XianDuanChuLiStatus::a1_equal_a2;
                    }
                    else {
                        if (tmp_bi.get_high() > this->a_1.get_high()) {
                            return(this->failure_xd(this->a_1, tmp_bi));
                        }
                        else {
                            this->a_2 = tmp_bi;
                            this->status = XianDuanChuLiStatus::a_3;
                        }
                    }
                }
                else {
                    normal_bi_list = this->normal_process(this->a_4, this->a_5, bi);
                    this->a_1 = bi.generate_bi(this->a_1, this->a_2, this->a_3);
                    count = normal_bi_list.size();
                    switch (count) {
                    case 2:
                        this->a_2 = normal_bi_list[0];
                        this->a_3 = normal_bi_list[1];
                        if (is_bi_equal(this->a_2, this->a_3)) {
                            this->status = XianDuanChuLiStatus::a2_equal_a3;
                        }
                        else {
                            switch (this->a_3.get_type()) {
                            case BiType::UP:
                                if (this->a_3.get_high() > this->a_1.get_high()) {
                                    this->status = XianDuanChuLiStatus::a3_highlow_a1;
                                }
                                else {
                                    this->status = XianDuanChuLiStatus::a3_normal_a1;
                                }
                                break;
                            case BiType::DOWN:
                                if (this->a_3.get_low() < this->a_1.get_low()) {
                                    this->status = XianDuanChuLiStatus::a3_highlow_a1;
                                }
                                else {
                                    this->status = XianDuanChuLiStatus::a3_normal_a1;
                                }
                                break;
                            }
                        }
                        break;
                    case 1:
                        this->a_2 = normal_bi_list[0];
                        switch (this->a_2.get_type()) {
                        case BiType::UP:
                            if (this->a_2.get_high() > this->a_1.get_high()) {
                                return(this->failure_xd(this->a_1, this->a_2));
                            }
                            else {
                                this->status = XianDuanChuLiStatus::a_2;
                            }
                            break;
                        case BiType::DOWN:
                            if (this->a_2.get_low() < this->a_1.get_low()) {
                                return(this->failure_xd(this->a_1, this->a_2));
                            }
                            else {
                                this->status = XianDuanChuLiStatus::a_2;
                            }
                            break;
                        }
                    }

                }
                break;
            case BiType::DOWN:
                if (bi_low < this->a_4.get_low()) {
                    tmp_bi = bi.generate_bi(this->a_4, this->a_5, bi);
                    if (is_bi_equal(this->a_1, tmp_bi)) {
                        this->a_2 = tmp_bi;
                        this->status = XianDuanChuLiStatus::a1_equal_a2;
                    }
                    else {
                        if (tmp_bi.get_low() < this->a_1.get_low()) {
                            return(this->failure_xd(this->a_1, tmp_bi));
                        }
                        else {
                            this->a_2 = tmp_bi;
                            this->status = XianDuanChuLiStatus::a_3;
                        }
                    }
                }
                else {
                    normal_bi_list =this->normal_process(this->a_4, this->a_5, bi);
                    this->a_1 = bi.generate_bi(this->a_1, this->a_2, this->a_3);
                    count = normal_bi_list.size();
                    switch (count) {
                    case 2:
                        this->a_2 = normal_bi_list[0];
                        this->a_3 = normal_bi_list[1];
                        if (is_bi_equal(this->a_2, this->a_3)) {
                            this->status = XianDuanChuLiStatus::a2_equal_a3;
                        }
                        else {
                            switch (this->a_3.get_type()) {
                            case BiType::UP:
                                if (this->a_3.get_high() > this->a_1.get_high()) {
                                    this->status = XianDuanChuLiStatus::a3_highlow_a1;
                                }
                                else {
                                    this->status = XianDuanChuLiStatus::a3_normal_a1;
                                }
                                break;
                            case BiType::DOWN:
                                if (this->a_3.get_low() < this->a_1.get_low()) {
                                    this->status = XianDuanChuLiStatus::a3_highlow_a1;
                                }
                                else {
                                    this->status = XianDuanChuLiStatus::a3_normal_a1;
                                }
                                break;
                            }
                        }
                        break;
                    case 1:
                        this->a_2 = normal_bi_list[0];
                        switch (this->a_2.get_type()) {
                        case BiType::UP:
                            if (this->a_2.get_high() > this->a_1.get_high()) {
                                return(this->failure_xd(this->a_1, this->a_2));
                            }
                            else {
                                this->status = XianDuanChuLiStatus::a_2;
                            }
                            break;
                        case BiType::DOWN:
                            if (this->a_2.get_low() < this->a_1.get_low()) {
                                return(this->failure_xd(this->a_1, this->a_2));
                            }
                            else {
                                this->status = XianDuanChuLiStatus::a_2;
                            }
                            break;
                        }
                        break;
                    default:
                        break;
                    }
                }
                break;
            }
            break;

        case XianDuanChuLiStatus::A:
            this->zhongshu_process(this->A, bi);
            break;

        case XianDuanChuLiStatus::b_3:
            if (is_bi_equal(this->b_2, bi)) {
                this->b_3 = bi;
                this->status = XianDuanChuLiStatus::b2_equal_b3;
            }
            else {
                switch (bi_type) {
                case BiType::UP:
                    if (bi_high > this->b_1.get_high()) {
                        this->b = bi.generate_bi(this->b_1, this->b_2, bi);
                        this->A.set_output(this->b);
                        this->status = XianDuanChuLiStatus::b;
                    }
                    else {
                        this->b_3 = bi;
                        this->status = XianDuanChuLiStatus::b_3_normal;

                    }
                    break;
                case BiType::DOWN:
                    if (bi_low < this->b_1.get_low()) {
                        //中枢完成
                        this->b = bi.generate_bi(this->b_1, this->b_2, bi);
                        this->A.set_output(this->b);
                        this->status = XianDuanChuLiStatus::b;
                    }
                    else {
                        this->b_3 = bi;
                        this->status = XianDuanChuLiStatus::b_3_normal;
                    }
                    break;
                }
            }
            break;

        case XianDuanChuLiStatus::b_3_normal:
            normal_bi_list = this->normal_process(this->b_2, this->b_3, bi);
            count = normal_bi_list.size();
            switch (count) {
            case 2:
                this->b = this->b_1;
                this->b_2 = normal_bi_list[0];
                this->b_3 = normal_bi_list[1];
                if (is_bi_equal(this->b_2, this->b_3)) {
                    this->status = XianDuanChuLiStatus::b2_equal_b3;
                }
                else {
                    switch (this->b_3.get_type()) {
                    case BiType::UP:
                        if (this->b_3.get_high() > this->b.get_high()) {
                            this->b = bi.generate_bi(this->b, this->b_2, this->b_3);
                            this->status = XianDuanChuLiStatus::b;
                        }
                        else {
                            this->status = XianDuanChuLiStatus::b_3_normal;
                        }
                        break;
                    case BiType::DOWN:
                        if (this->b_3.get_low() < this->b.get_low()) {
                            this->b = bi.generate_bi(this->b, this->b_2, this->b_3);
                            this->status = XianDuanChuLiStatus::b;
                        }
                        else {
                            this->status = XianDuanChuLiStatus::b_3_normal;
                        }
                        break;
                    }
                }
                break;
            case 1:
                this->b_2 = normal_bi_list[0];
                switch (this->b_2.get_type()) {
                case BiType::UP:
                    tmp_bi = bi.generate_bi(this->a_1, this->a_2, this->b);
                    if (is_bi_equal(tmp_bi, this->b_2)) {
                        this->a_1 = tmp_bi;
                        this->a_2 = this->b_2;
                        this->status = XianDuanChuLiStatus::a1_equal_a2;
                    }
                    else {
                        if (this->b_2.get_high() < this->A.get_low()) {
                            this->b_1 = this->b;
                            this->status = XianDuanChuLiStatus::b_3;
                        }
                        else {
                            last_xd = XianDuan(this->a_1, this->b);
                            last_xd.set_kind(XianDuanKind::PANZHENG);
                            ret_xd.xd1 = last_xd;
                            ret_xd.type = FindXianDuanReturnType::One;
                            this->a_1 = this->b_2;
                            this->status = XianDuanChuLiStatus::a_2;
                        }
                    }
                    break;
                case BiType::DOWN:
                    tmp_bi = bi.generate_bi(this->a_1, this->b_2, this->b);
                    if (is_bi_equal(tmp_bi, this->b_2)) {
                        this->a_1 = tmp_bi;
                        this->a_2 = this->b_2;
                        this->status = XianDuanChuLiStatus::a1_equal_a2;
                    }
                    else {
                        if (this->b_2.get_low() > this->A.get_high()) {
                            this->b_1 = this->b;
                            this->status = XianDuanChuLiStatus::b_3;
                        }
                        else {
                            last_xd = XianDuan(this->a_1, this->b);
                            last_xd.set_kind(XianDuanKind::PANZHENG);
                            ret_xd.xd1 = last_xd;
                            ret_xd.type = FindXianDuanReturnType::One;
                            this->a_1 = this->b_2;
                            this->status = XianDuanChuLiStatus::a_2;
                        }
                    }
                    break;
                }
                break;
            }
            break;

        case XianDuanChuLiStatus::b:
            switch (bi_type) {
            case BiType::UP:
                tmp_bi = bi.generate_bi(this->a_1, this->a_2, bi);
                if (is_bi_equal(tmp_bi, bi)) {
                    this->a_1 = tmp_bi;
                    this->a_2 = bi;
                    this->status = XianDuanChuLiStatus::a1_equal_a2;
                }
                else {
                    if (bi_high < this->A.get_low()) {
                        this->b_2 = bi;
                        this->status = XianDuanChuLiStatus::b_3;
                    }
                    else {
                        last_xd = XianDuan(this->a_1, this->b);
                        last_xd.set_kind(XianDuanKind::PANZHENG);
                        ret_xd.xd1 = last_xd;
                        ret_xd.type = FindXianDuanReturnType::One;
                        this->a_1 = this->b_2;
                        this->status = XianDuanChuLiStatus::a_2;
                    }
                }
                break;
            case BiType::DOWN:
                tmp_bi = bi.generate_bi(this->a_1, this->a_2, bi);
                if (is_bi_equal(tmp_bi, bi)) {
                    this->a_1 = tmp_bi;
                    this->a_2 = bi;
                    this->status = XianDuanChuLiStatus::a1_equal_a2;
                }
                else {
                    if (bi_low > this->A.get_high()) {
                        this->b_2 = bi;
                        this->status = XianDuanChuLiStatus::b_3;
                    }
                    else {
                        last_xd = XianDuan(this->a_1, this->b);
                        last_xd.set_kind(XianDuanKind::PANZHENG);
                        ret_xd.xd1 = last_xd;
                        ret_xd.type = FindXianDuanReturnType::One;
                        this->a_1 = this->b_2;
                        this->status = XianDuanChuLiStatus::a_2;
                    }
                }
                break;
            }
            break;

        case XianDuanChuLiStatus::b2_equal_b3:
            pre_zhongshu_list = this->pre_zhongshu_process(this->b_1, this->b_2, this->b_3, bi);
            count = pre_zhongshu_list.size();
            switch (count) {
            case 4:
                this->B = ZhongShu(pre_zhongshu_list[0], pre_zhongshu_list[1], pre_zhongshu_list[2], pre_zhongshu_list[3], Bi());
                this->status = XianDuanChuLiStatus::B;
                break;
            case 2:
                this->b_1 = pre_zhongshu_list[0];
                this->b_2 = pre_zhongshu_list[1];
                switch (this->b_2.get_type()) {
                case BiType::UP:
                    if (this->b_2.get_high() > this->A.get_low()) {
                        last_xd = XianDuan(this->a_1, this->b_1);
                        ret_xd.type = FindXianDuanReturnType::One;
                        ret_xd.xd1 = last_xd;
                        this->a_1 = this->b_2;
                        this->status = XianDuanChuLiStatus::a_2;
                    }
                    else {
                        this->status = XianDuanChuLiStatus::a_3;
                    }
                    break;
                case BiType::DOWN:
                    if (this->b_2.get_low() < this->A.get_high()) {
                        last_xd = XianDuan(this->a_1, this->b_1);
                        ret_xd.type = FindXianDuanReturnType::One;
                        ret_xd.xd1 = last_xd;
                        this->a_1 = this->b_2;
                        this->status = XianDuanChuLiStatus::a_2;
                    }
                    else {
                        this->status = XianDuanChuLiStatus::a_3;
                    }
                    break;
                }
                break;
            case 1:
                this->b_1 = pre_zhongshu_list[0];
                this->status = XianDuanChuLiStatus::b;
                break;
            }
            break;

        case XianDuanChuLiStatus::B:
            if (is_bi_equal(this->b_3, bi)) {
                
            }
            else {
                switch (bi_type) {
                case BiType::UP:
                    if (bi_high <= this->B.get_low()) {

                    }
                    else {
                        if (bi_high < this->B.get_high()) {

                        }
                        else {

                        }
                    }
                    break;
                case BiType::DOWN:
                    break;
                }
            }
            break;
    }
    this->last_bi = bi;
    return(ret_xd);
}



void Bi3_xianduan(int nCount, float* pOut, float* pHigh, float* pLow, float* pIn) {
    BaoHanChuLi baohanChuli;
    XianDuanChuLi xdchuli;
    XianDuan xd;
    Bi bi;
    float bi_status = 0;
    int start_pos, stop_pos;

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

    if (bi_list.size() > 0) {
        bi = bi_list[0];
        pos = bi.get_start_pos();
        if (bi.get_type() == BiType::UP)
            pOut[pos] = -4;
        else
            pOut[pos] = 4;

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
        unsigned int start_pos = bi_zhongshu.get_start_pos();
        unsigned int stop_pos = bi_zhongshu.get_stop_pos();
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
        unsigned int start_pos = bi_zhongshu.get_start_pos();
        unsigned int stop_pos = bi_zhongshu.get_stop_pos();
        float low = bi_zhongshu.get_low();
        for (unsigned int j = start_pos; j < stop_pos; j++)
            pOut[j] = low;
    }
}
