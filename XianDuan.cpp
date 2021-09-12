#include "XianDuan.h"
#include "ZhongShu1.h"

using namespace std;

XianDuan XianDuanChuLi::last_xd = XianDuan();
XianDuanChuLiStatus  XianDuanChuLi::status = XianDuanChuLiStatus::LEFT;
Bi1 XianDuanChuLi::last_bi = Bi1();

XianDuanChuLi::XianDuanChuLi() {
    this->last_xd = XianDuan();
    this->last_bi = Bi1();
    this->bicl = BiChuLi1();
}

void XianDuanChuLi::handle(vector<Kxian1>& kxianList) {
    Bi1 bi = Bi1();
    this->bicl.handle(kxianList);
    /*
    int count = this->bicl.biList.size();
    for (int i = 0; i < count; i++) {
        bi = this->bicl.biList[i];
        this->find_xianduan(bi);
    }
    */
}


Bi1 XianDuanChuLi::generate_bi(Bi1 first_bi, Bi1 second_bi,Bi1 three_bi) {
    FenXing start_fx, stop_fx;

    start_fx = first_bi.get_start_fx();
    stop_fx = three_bi.get_stop_fx();
    return(Bi1(start_fx, stop_fx));
}

Bi1 XianDuanChuLi::update_stop_bi(Bi1 bi, FenXing stop_fx) {
    FenXing start_fx = bi.get_start_fx();
    return(Bi1(start_fx, stop_fx));
}

XianDuan XianDuanChuLi::failure_xd(Bi1 start_bi, Bi1 stop_bi) {
    XianDuanType xd_type = this->last_xd.get_type();
    XianDuan xd = XianDuan();
    Bi1 xd_start_bi = Bi1();

    if (xd_type != XianDuanType::NONE) {
        xd_start_bi = this->last_xd.get_start_bi();
        xd = XianDuan(xd_start_bi, stop_bi);
        if (xd_type == XianDuanType::UP)
            xd.set_type(XianDuanType::FAILURE_UP);
        else {
            if (xd_type == XianDuanType::DOWN) {
                xd.set_type(XianDuanType::FAILURE_DOWN);
            }
        }
    }
    return(xd);
}

XianDuan XianDuanChuLi::find_xianduan(Bi1 bi) {
    XianDuan xd = XianDuan();
    if (this->last_bi.get_type() == BiType::NONE) {
        this->last_bi = bi;
        this->left = bi;
        XianDuanChuLi::status = XianDuanChuLiStatus::AFTER_LEFT;
    }
    else {
        switch (XianDuanChuLi::status) {
        case XianDuanChuLiStatus::LEFT:
            this->left = bi;
            XianDuanChuLi::status = XianDuanChuLiStatus::AFTER_LEFT;
            break;
        case XianDuanChuLiStatus::AFTER_LEFT:
            if (this->left.get_type() == BiType::UP && bi.get_low() < this->left.get_low()) {
                xd = this->failure_xd(this->left, bi);
                XianDuanChuLi::status = XianDuanChuLiStatus::LEFT;
                return(xd);
            }
            else {
                if (this->left.get_type() == BiType::DOWN && bi.get_high() > this->left.get_high()) {
                    xd = this->failure_xd(this->left, bi);
                    XianDuanChuLi::status = XianDuanChuLiStatus::LEFT;
                    return(xd);
                }
            }
            this->after_left = bi;
            XianDuanChuLi::status = XianDuanChuLiStatus::MIDDLE;
            break;
        case XianDuanChuLiStatus::MIDDLE:
            if (bi.get_type() == BiType::UP && bi.get_high() > this->left.get_high()) {
                XianDuanChuLi::status = XianDuanChuLiStatus::MIDDLE_HIGH_LOW;
            }
            else {
                if (bi.get_type() == BiType::DOWN && bi.get_low() < this->left.get_low()) {
                    XianDuanChuLi::status = XianDuanChuLiStatus::MIDDLE_HIGH_LOW;
                }
                else {
                    XianDuanChuLi::status = XianDuanChuLiStatus::MIDDLE_NORMAL;
                }
            }
            this->middle = bi;
            break;
        case XianDuanChuLiStatus::MIDDLE_HIGH_LOW:
            if (bi.get_type() == BiType::UP) {
                if (bi.get_high() > this->left.get_high()){
                    Bi1 start_bi = this->generate_bi(this->left, this->after_left, this->middle);
                    xd = this->failure_xd(start_bi, bi);
                    XianDuanChuLi::status = XianDuanChuLiStatus::LEFT;
                }
                else {
                    if (bi.get_high() >= this->after_left.get_high()){
                        this->left = this->generate_bi(this->left, this->after_left, this->middle);
                        this->after_left = this->update_stop_bi(bi, this->middle.get_start_fx());
                       this->status = XianDuanChuLiStatus::MIDDLE;
                    }
                    else {
                        if (bi.get_high() <= this->left.get_low()) {
                            this->left = this->generate_bi(this->left, this->after_left, this->middle);
                            this->after_left = bi;
                            XianDuanChuLi::status = XianDuanChuLiStatus::MIDDLE;
                        }
                        else {
                            this->after_middle = bi;
                            this->zhongshu_a = ZhongShu1(this->left, this->after_left, this->middle, bi);
                        }
                    }
                }
            }
            else {
                if (bi.get_type() == BiType::DOWN) {
                    if (bi.get_low() < this->left.get_low()) {
                        Bi1 start_bi = this->generate_bi(this->left, this->after_left, this->middle);
                        xd = this->failure_xd(start_bi, bi);
                        XianDuanChuLi::status = XianDuanChuLiStatus::LEFT;
                    }
                    else {
                        if (bi.get_low() <= this->after_left.get_low()) {
                            this->left = this->generate_bi(this->left, this->after_left, this->middle);
                            this->after_left = update_stop_bi(bi, this->middle.get_start_fx());
                            XianDuanChuLi::status = XianDuanChuLiStatus::MIDDLE;
                        }
                        else {
                            if (bi.get_low() >= this->left.get_high()) {
                                this->left = this->generate_bi(this->left, this->after_left, this->middle);
                                this->after_left = bi;
                                XianDuanChuLi::status = XianDuanChuLiStatus::MIDDLE;
                            }
                            else {
                                this->after_middle = bi;
                                this->zhongshu_a = ZhongShu1(this->left, this->after_left, this->middle, bi);
                                XianDuanChuLi::status = XianDuanChuLiStatus::ZHONGSHU_A;
                            }
                        }
                    }
                }
            }
            break;
        case XianDuanChuLiStatus::MIDDLE_NORMAL:
            if (bi.get_type() == BiType::DOWN) {
                if (bi.get_low() < this->left.get_low()) {
                    Bi1 stop_bi = generate_bi(this->after_left, this->middle, bi);
                    xd = this->failure_xd(this->left, stop_bi);
                    XianDuanChuLi::status = XianDuanChuLiStatus::LEFT;
                }
                else {
                    if (bi.get_low() >= this->after_left.get_low()) {
                        this->after_left = generate_bi(this->after_left, this->middle, bi);
                        XianDuanChuLi::status = XianDuanChuLiStatus::MIDDLE;
                    }
                    else {
                        this->after_middle = bi;
                        this->zhongshu_a = ZhongShu1(this->left, this->after_left, this->middle, bi);
                        XianDuanChuLi::status = XianDuanChuLiStatus::ZHONGSHU_A;
                    }
                }
            }
            else {
                if (bi.get_type() == BiType::UP) {
                    if (bi.get_high() > this->left.get_high()) {
                        Bi1 stop_bi = generate_bi(this->after_left, this->middle, bi);
                        xd = this->failure_xd(this->left, stop_bi);
                        XianDuanChuLi::status = XianDuanChuLiStatus::LEFT;
                    }
                    else {
                        if (bi.get_high() <= this->after_left.get_high()) {
                            this->after_left = generate_bi(this->after_left, this->middle, bi);
                            XianDuanChuLi::status = XianDuanChuLiStatus::MIDDLE;
                        }
                        else {
                            this->after_middle = bi;
                            this->zhongshu_a = ZhongShu1(this->left, this->after_left, this->middle, bi);
                            XianDuanChuLi::status = XianDuanChuLiStatus::ZHONGSHU_A;
                        }
                    }
                }
            }
            break;
        case XianDuanChuLiStatus::ZHONGSHU_A:
            break;
        }
    }
}

void Bi3(int nCount, float* pOut, float* pHigh, float* pLow, float* pIn) {
    BaoHanChuLi baohanChuli;
    XianDuanChuLi xdchuli;
    XianDuan xd;
    //XianDuanType xd_type;

    for (int i = 0; i < nCount; i++) {
        baohanChuli.add(pHigh[i], pLow[i]);
    }

    for (int i = 0; i < nCount; i++) {
        pOut[i] = 0;
    }

    xdchuli.handle(baohanChuli.kxianList);
    /*
    unsigned int count = bichuli.biList.size();
    for (unsigned int i = 0; i < count; i++) {
        bi = bichuli.biList[i];
        bi_type = bi.get_type();
        if (bi_type != BiType::NONE) {
            switch (bi_type) {
            case BiType::UP:
                pOut[bi.get_stop_fx().get_middle().position] = 1;
                break;
            case BiType::DOWN:
                pOut[bi.get_stop_fx().get_middle().position] = -1;
                break;
            case BiType::NEW_DOWN:
                pOut[bi.get_stop_fx().get_middle().position] = -2;
                break;
            case BiType::NEW_UP:
                pOut[bi.get_stop_fx().get_middle().position] = 2;
                break;
            }
        }
    }
    */
}


