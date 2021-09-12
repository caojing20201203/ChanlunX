#include "ZhongShu1.h"

using namespace std;

ZhongShuChuLi::ZhongShuChuLi(ZhongShu1 zs) {
    this->zhongshu = zs;
}

void ZhongShuChuLi::set_status(ZhongShuChuLiStatus status) {
    this->status = status;
}

ZhongShu1 ZhongShuChuLi::find_zhongshu(Bi1 bi) {
    ZhongShu1 zs = ZhongShu1();
    if (bi.get_high() > this->zhongshu.get_max_high()) {
        this->zhongshu.set_max_high(bi.get_high());
    }
    if (bi.get_low() < this->zhongshu.get_min_low()) {
        this->zhongshu.set_min_low(bi.get_low());
    }
    switch (this->status) {
    case ZhongShuChuLiStatus::NONE:
        break;
    case ZhongShuChuLiStatus::IN:
        if (bi.get_type() == BiType::UP) {
            if (bi.get_high() <= this->zhongshu.get_zhongshu_low()) {
                //3卖
            }
            
        }
        else {
            if (bi.get_type() == BiType::DOWN) {
                if (bi.get_low() >= this->zhongshu.get_zhongshu_high()) {
                    //3买
                    if (!this->zhongshu.biList.empty()) {
                        Bi1 out_bi = this->zhongshu.biList.back();
                        this->zhongshu.set_output_bi(out_bi);
                    }
                }
            }
        }
        break;
    case ZhongShuChuLiStatus::OUT:
        break;
    }
    if (bi.get_high() > this->zhongshu.get_zhongshu_high()) {
        this->set_status(ZhongShuChuLiStatus::IN);
    }
    else {
        if (bi.get_low() < this->zhongshu.get_zhongshu_low()) {
            this->set_status(ZhongShuChuLiStatus::IN);
        }
        else {
            this->set_status(ZhongShuChuLiStatus::OUT);
        }
    }
    this->zhongshu.biList.push_back(bi);
    return(zs);
}
