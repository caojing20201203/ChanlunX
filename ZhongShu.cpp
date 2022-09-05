#include "ZhongShu.h"

using namespace std;

ZhongShuChuLi::ZhongShuChuLi(){
    this->zhongshu = ZhongShu();
    this->last_xd = XianDuan();
}

bool ZhongShuChuLi::is_level_xd(XianDuan xd){
    float length;
    float xd_high = xd.get_high();
    float xd_low = xd.get_low();
    XianDuanType xd_type = xd.get_type();
    float min_length = this->zhongshu.get_length() / 3;

    switch(xd_type){
        case XianDuanType::UP:
        if (xd_high > this->zhongshu.get_high() ) {
            length = xd_high - this->zhongshu.get_high();
            if (length > min_length) {
                return(true);
            } 
        }
        break;
        case XianDuanType::DOWN:
        if (xd_low < this->zhongshu.get_low()){
            length = this->zhongshu.get_length() - xd_low;
            if (length > min_length)
                return(true);
        }
        break;
    }
    return(false);
}

ZhongShuChuLi::ZhongShuChuLi(XianDuan input, XianDuan xd1, XianDuan xd2, XianDuan xd3) {
    this->zhongshu = ZhongShu(input, xd1, xd2, xd3);
    if (xd3.get_type() == XianDuanType::UP && xd3.get_high() > this->zhongshu.get_high()) {
        this->status = ZhongShuChuLiStatus::UP;
    } else {
        if (xd3.get_type() == XianDuanType::DOWN && xd3.get_low() < this->zhongshu.get_low())
            this->status = ZhongShuChuLiStatus::DOWN;
        else
            this->status = ZhongShuChuLiStatus::INSIDE;
    }
}

ZhongShu ZhongShuChuLi::get_zhongshu() {
    return(this->zhongshu);
}

FindZhongShuReturn ZhongShuChuLi::find_zhongshu(XianDuan xd) {
    FindZhongShuReturn ret_zhongshu = FindZhongShuReturn();
    this->zhongshu.xd_list.push_back(xd);

    float xd_high = xd.get_high();
    float xd_low = xd.get_low();
    XianDuanType xd_type = xd.get_type();
    if (this->zhongshu.xd_list.size() >= 10) {
        //TODO: 中枢升级
        printf("TODO 中枢升级");
        XianDuan first_xd = xd.generate_xd(this->zhongshu.xd_list[1], this->zhongshu.xd_list[2], this->zhongshu.xd_list[3]);
        XianDuan second_xd = xd.generate_xd(this->zhongshu.xd_list[4], this->zhongshu.xd_list[5], this->zhongshu.xd_list[6]);
        XianDuan three_xd = xd.generate_xd(this->zhongshu.xd_list[7], this->zhongshu.xd_list[8], this->zhongshu.xd_list[9]);
        XianDuan input = this->zhongshu.get_input();
        ret_zhongshu.zhongshu = ZhongShu(input, first_xd, second_xd, three_xd);
        ret_zhongshu.type = FindZhongShuReturnType::ZhongShu_upgrade;
        return(ret_zhongshu);
    } else {
        switch(xd_type){
            case XianDuanType::UP:
            if (xd_high < this->zhongshu.get_low()) {
                //3卖
                if (this->zhongshu.get_input().get_type() == XianDuanType::DOWN){
                    this->zhongshu.stop(this->last_xd);
                    ret_zhongshu.zhongshu = this->zhongshu;
                    ret_zhongshu.type = FindZhongShuReturnType::THREE_SELL;
                } else {
                    
                }
           } else {
                if (this->is_level_xd(xd)){
                    this->zhongshu.stop(xd);
                    ret_zhongshu.zhongshu = this->zhongshu;
                    ret_zhongshu.type = FindZhongShuReturnType::ZhongShuSuccess;
                }
            }
            break;
            case XianDuanType::DOWN:
            break;
        }
    }
    return(ret_zhongshu);
}
