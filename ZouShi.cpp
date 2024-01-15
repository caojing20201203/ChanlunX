#include "BaoHanChuLi.h"
#include "ZouShi.h"

ZouShiChuLi::ZouShiChuLi(){
    this->status = ZouShiChuLiStatus::a1;
    this->xdcl = XianDuanChuLi();
    this->last_zoushi = ZouShi();
}

void ZouShiChuLi::handle(vector<Kxian1>& kxlist){
    vector<XianDuan> xd_list;
    ZouShi zoushi;
    XianDuan xd;

    this->xdcl.handle(kxlist);
    xd_list = this->xdcl.xianDuanList;

    int count = xd_list.size();
    if (count == 0)
        return;

    for (int i = 0; i < count; i++){
        xd = xd_list[i];
        zoushi = this->__find_zoushi(xd);
        if (zoushi.get_type() != ZouShiType::NONE) {
            this->zoushi_list.push_back(zoushi);
        }
    }
}

bool is_xd_equal(XianDuan xd1, XianDuan xd2){
    float radio;
    float xd1_len = xd1.get_length();
    float xd2_len = xd2.get_length();

    if (xd1_len <= xd2_len)
        radio = xd1_len / xd2_len;
    else 
        radio = xd2_len / xd1_len;
    
    if (radio >= 0.618)
        return(true);
    else
        return(false);
}

ZouShi ZouShiChuLi::failure_zoushi(XianDuan xd1, XianDuan xd2, XianDuan xd3){
    ZouShi ret_zoushi = ZouShi();
    XianDuan start_xd, stop_xd;

    if (this->last_zoushi.get_type() == ZouShiType::NONE){
        if (xd3.get_type() == XianDuanType::NONE) {
            this->a1 = xd2;
            this->status = ZouShiChuLiStatus::a2;
        } else {
            this->a1 = xd3;
            this->status = ZouShiChuLiStatus::a2;
        }
    } else {
        ret_zoushi = this->last_zoushi;
        ret_zoushi.set_stop_xd(xd2);

        switch (ret_zoushi.get_type()) {
            case ZouShiType::DAXIANDUAN_UP:
                ret_zoushi.set_type(ZouShiType::UPDATE_DAXIANDUAN_UP);
                break;
            case ZouShiType::DAXIANDUAN_DOWN:
                ret_zoushi.set_type(ZouShiType::UPDATE_DAXIANDUAN_DOWN);
                break;
            case ZouShiType::PANZHENG:
                ret_zoushi.set_type(ZouShiType::UPDATE_PANZHENG);
                break;
            case ZouShiType::QUSHI_UP:
                ret_zoushi.set_type(ZouShiType::UPDATE_QUSHI_UP);
                break;
            case ZouShiType::QUSHI_DOWN:
                ret_zoushi.set_type(ZouShiType::UPDATE_QUSHI_DOWN);
                break;
        }
        if (xd3.get_type() == XianDuanType::NONE) {
            this->status = ZouShiChuLiStatus::a1;
        } else {
            this->a1 = xd3;
            this->status = ZouShiChuLiStatus::a2;
        }
    }
    return(ret_zoushi);
}

void ZouShiChuLi::zhongshu_process(ZhongShu zs, XianDuan xd){
    XianDuanType xd_type = xd.get_type();
    float xd_high = xd.get_high();
    float xd_low = xd.get_low();
    int xd_count = zs.get_xd_count();

    if (zs.get_after_max_xd().get_type() == XianDuanType::NONE) {
        zs.set_after_max_xd(xd);
    }
    if (zs.get_after_min_xd().get_type() == XianDuanType::NONE) {
        zs.set_after_min_xd(xd);
    } 
    
    if ((xd_type == XianDuanType::UP && xd_high < zs.get_low()) || (xd_type == XianDuanType::DOWN && xd_low > zs.get_high())) {
        zs.set_output(zs.get_last_xd());
        this->b1 = zs.get_last_xd();
        this->b2 = xd;
        this->status = ZouShiChuLiStatus::b3;
    } else {
        if (xd_type == XianDuanType::UP && xd_high > zs.get_max_high()){
                zs.set_max_xd(xd);
            }
        if (xd_type == XianDuanType::DOWN && xd_low < zs.get_min_low()){
            zs.set_min_xd(xd);
        }
    }

    zs.set_xd_count(xd_count + 1);
    if (xd_count == 9) {
        //中枢升级
    }
    zs.set_last_xd(xd);
}


ZouShi ZouShiChuLi::__find_zoushi(XianDuan xd){
    ZouShi ret_zoushi = ZouShi();
    float xd_high = xd.get_high();
    float xd_low = xd.get_low();
    XianDuanType xd_type = xd.get_type();

    switch(this->status) {
        case ZouShiChuLiStatus::a1:
            this->a1 = xd;
            this->status = ZouShiChuLiStatus::a2;
            break;

        case ZouShiChuLiStatus::a2:
            if (is_xd_equal(this->a1, xd)){
                this->a2 = xd;
                this->status = ZouShiChuLiStatus::a1_equal_a2;
            } else {
                switch (xd_type){
                    case XianDuanType::UP:
                        if (xd_high > this->a1.get_high()){
                            ret_zoushi = this->failure_zoushi(this->a1, xd, XianDuan());
                        } else {
                            this->a2 = xd;
                            this->status = ZouShiChuLiStatus::a2_normal;
                        }
                        break;
                    case XianDuanType::DOWN:
                        if (xd_low < this->a1.get_low()){
                            ret_zoushi = this->failure_zoushi(this->a1, xd, XianDuan());
                        } else {
                            this->a2 = xd;
                            this->status = ZouShiChuLiStatus::a2_normal;
                        }
                        break;
                }
            }
            break;
        
        case ZouShiChuLiStatus::a1_equal_a2:
            if (is_xd_equal(this->a2, xd)) {
                this->A = ZhongShu(XianDuan(), this->a1, this->a2, xd, XianDuan());
                this->status = ZouShiChuLiStatus::A;
            } else {
                switch ((xd_type)) {
                    case XianDuanType::UP:
                        if (xd_high > this->a1.get_high()){
                            if (this->a1.get_low() < this->a2.get_low()){
                                this->a1 = xd.generate_xd(this->a1, this->a2, xd);
                                this->status = ZouShiChuLiStatus::a2;
                            } else {
                                ret_zoushi = this->failure_zoushi(this->a1, this->a2, xd);
                            }
                        }  else {
                            this->a3 = xd;
                            this->status = ZouShiChuLiStatus::a1_equal_a2_normal;
                        }
                        break;
                    case XianDuanType::DOWN:
                        if (xd_low < this->a1.get_low()){
                            if (this->a1.get_high() > this->a2.get_high()){
                                this->a1 = xd.generate_xd(this->a1, this->a2, xd);
                                this->status = ZouShiChuLiStatus::a2;
                            } else {
                                ret_zoushi = this->failure_zoushi(this->a1, this->a2, xd);
                            }
                        } else {
                            this->a3 = xd;
                            this->status = ZouShiChuLiStatus::a1_equal_a2_normal;
                        }
                        break;
                }
            }
            break;

        case ZouShiChuLiStatus::a1_equal_a2_normal:
            switch (xd_type) {
                case XianDuanType::UP:
                    if (this->a1.get_high() > this->a2.get_high()){
                        if (xd_high > this->a3.get_high()){
                            if (this->a1.get_high() > this->a2.get_high()){
                                this->a2 = xd.generate_xd(this->a2, this->a3, xd);
                                if (is_xd_equal(this->a1, this->a2)){
                                    this->status = ZouShiChuLiStatus::a1_equal_a2;
                                } else {
                                    ret_zoushi = this->failure_zoushi(this->a1, this->a2, XianDuan());
                                }
                            } else {
                                this->a4 = xd;
                                this->status = ZouShiChuLiStatus::a1_equal_a2_normal_normal;
                            }
                        }
                    } else {
                        if (xd_high > this->a2.get_high()){
                            this->a2 = xd.generate_xd(this->a2, this->a3, xd);
                            if (is_xd_equal(this->a1, this->a2)) {
                                this->status = ZouShiChuLiStatus::a1_equal_a2;
                            } else {
                                ret_zoushi = this->failure_zoushi(this->a1, this->a2, XianDuan());
                            }
                        } else {
                            this->a4 = xd;
                            this->status = ZouShiChuLiStatus::a1_equal_a2_normal_normal;
                        }
                    }
                    break;
                case XianDuanType::DOWN:
                    if (this->a1.get_low() < this->a2.get_low()){
                        if (xd_low < this->a2.get_low()){
                            this->a2 = xd.generate_xd(this->a2, this->a3, xd);
                            if (is_xd_equal(this->a1, this->a2)) {
                                this->status = ZouShiChuLiStatus::a1_equal_a2;
                            } else {
                                ret_zoushi = this->failure_zoushi(this->a1, this->a2, XianDuan());
                            }
                        } else {
                            this->a4 = xd;
                            this->status = ZouShiChuLiStatus::a1_equal_a2_normal_normal;
                        }
                    } else {
                        if (xd_low < this->a2.get_low()){
                            this->a2 = xd.generate_xd(this->a2, this->a3, xd);
                            if (is_xd_equal(this->a1, this->a2)) {
                                this->status = ZouShiChuLiStatus::a1_equal_a2;
                            } else {
                                ret_zoushi = this->failure_zoushi(this->a1, this->a2, XianDuan());
                            }
                        } else {
                            this->a4 = xd;
                            this->status = ZouShiChuLiStatus::a1_equal_a2_normal_normal;
                        }
                    }
                    break;
            }
            break;

        case ZouShiChuLiStatus::a1_equal_a2_normal_normal:
            switch(xd_type) {
                case XianDuanType::UP:
                    if (this->a1.get_type() == XianDuanType::UP){
                        if (xd_high > this->a3.get_high()){
                            this->a3 = xd.generate_xd(this->a3, this->a4, xd);
                            if (is_xd_equal(this->a2, this->a3)) {
                                this->A = ZhongShu(XianDuan(), this->a1, this->a2, this->a3, XianDuan());
                                this->status = ZouShiChuLiStatus::A;
                            } else {
                                if (this->a1.get_low() < this->a2.get_low()){
                                    this->a1 = xd.generate_xd(this->a1, this->a2, this->a3);
                                    this->status = ZouShiChuLiStatus::a2;
                                } else {
                                    ret_zoushi = this->failure_zoushi(this->a1, this->a2, this->a3);
                                }
                            }
                        } 
                    } else {
                        if (xd_high > this->a2.get_high()){
                            this->a2 = xd.generate_xd(this->a2, this->a3, xd);
                            if (is_xd_equal(this->a1, this->a2)){
                                this->status = ZouShiChuLiStatus::a1_equal_a2;
                            } else {
                                if (this->a1.get_high() > this->a2.get_high()){
                                    this->status = ZouShiChuLiStatus::a3;
                                } else {
                                    ret_zoushi = this->failure_zoushi(this->a1, this->a2, XianDuan());
                                }
                            }
                        }
                    }
                    break;
                case XianDuanType::DOWN:
                    if (this->a1.get_type() == XianDuanType::DOWN) {
                        if (xd_low < this->a3.get_low()){
                            this->a3 = xd.generate_xd(this->a3, this->a4, xd);
                            if (is_xd_equal(this->a2, this->a3)) {
                                this->A = ZhongShu(XianDuan(), this->a1, this->a2, this->a3, XianDuan());
                                this->status = ZouShiChuLiStatus::A;
                            } else {
                                if (this->a1.get_high() > this->a2.get_high()){
                                    this->a1 = xd.generate_xd(this->a1, this->a2, this->a3);
                                    this->status = ZouShiChuLiStatus::a2;
                                } else {
                                    ret_zoushi = this->failure_zoushi(this->a1, this->a2, this->a3);
                                }
                            }
                        }
                    } else {
                        if (xd_low < this->a2.get_low()){
                            this->a2 = xd.generate_xd(this->a2, this->a3, xd);
                            if (is_xd_equal(this->a1, this->a2)){
                                this->status = ZouShiChuLiStatus::a1_equal_a2;
                            } else {
                                if (this->a1.get_low() < this->a2.get_low()){
                                    this->status = ZouShiChuLiStatus::a3;
                                } else {
                                    ret_zoushi = this->failure_zoushi(this->a1, this->a2, XianDuan());
                                }
                            }
                        }
                    }
                    break;
            }
            break;

        case ZouShiChuLiStatus::a2_normal:
            switch(xd_type) {
                case XianDuanType::UP:
                    if (is_xd_equal(this->a2, xd)){
                        this->a3 = xd;
                        this->status = ZouShiChuLiStatus::a2_equal_a3;
                    } else {
                        if (xd_high > this->a1.get_high()){
                            this->a1 = xd.generate_xd(this->a1, this->a2, xd);
                            this->status = ZouShiChuLiStatus::a2;
                        } else {
                            this->a3 = xd;
                            this->status = ZouShiChuLiStatus::a2_normal_normal;
                        }
                    }
                    break;
                case XianDuanType::DOWN:
                    if (is_xd_equal(this->a2, xd)){
                        this->a3 = xd;
                        this->status = ZouShiChuLiStatus::a2_equal_a3;
                    } else {
                        if (xd_low < this->a1.get_low()){
                            this->a1 = xd.generate_xd(this->a1, this->a2, xd);
                            this->status = ZouShiChuLiStatus::a2;
                        } else {
                            this->a3 = xd;
                            this->status = ZouShiChuLiStatus::a2_normal_normal;
                        }
                    }
                    break;
            }
            break;

        case ZouShiChuLiStatus::a2_equal_a3:
            switch(xd_type) {
                case XianDuanType::UP:
                    break;
                case XianDuanType::DOWN:
                    break;
            }
            break;
        case ZouShiChuLiStatus::A:
            this->zhongshu_process(this->A, xd);
            break;

        case ZouShiChuLiStatus::b3:
            break;
    }
    return(ret_zoushi);
}

void ZouShi_process(int nCount, float *pOut, float *pHigh, float *pLow, float *pIn){
    BaoHanChuLi baohanChuli;
    ZouShiChuLi zoushiChuLi;
    ZouShi zoushi;
    int start_pos, stop_pos;

    for (int i = 0; i < nCount; i++){
        baohanChuli.add(pHigh[i], pLow[i]);
    }

    for (int i = 0; i < nCount; i++){
        pOut[i] = 0;
    }

    zoushiChuLi.handle(baohanChuli.kxianList);

    int count = zoushiChuLi.zoushi_list.size();
    if (count == 0)
        return;

    zoushi = zoushiChuLi.zoushi_list[0];
    start_pos = zoushi.get_start_pos();
    stop_pos = zoushi.get_stop_pos();
    if (zoushi.get_start_xd().get_type() == XianDuanType::UP) {
        pOut[start_pos] = -3;
        pOut[stop_pos] = 3;
    } else {
        pOut[start_pos] = 3;
        pOut[stop_pos] = -3;
    }

    for (int i = 1; i < count; i++){
        zoushi = zoushiChuLi.zoushi_list[i];
        stop_pos = zoushi.get_stop_pos();
        switch(zoushi.get_type()){
            if (zoushi.get_start_xd().get_type() == XianDuanType::UP)
                pOut[stop_pos] = 3;
            else
                pOut[stop_pos] = -3;
        }
    }
}