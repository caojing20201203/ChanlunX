#include "Bi_ZhongShu.h"

using namespace std;

Bi_ZhongShuChuLi::Bi_ZhongShuChuLi(){
    this->bi_zhongshu = Bi_ZhongShu();
}

Bi_ZhongShuChuLi::Bi_ZhongShuChuLi(Bi input, Bi xd1, Bi xd2, Bi xd3) {
    this->bi_zhongshu = Bi_ZhongShu(input, xd1, xd2, xd3);

    if (xd3.get_type() == BiType::UP && xd3.get_high() > this->bi_zhongshu.get_high()) {
        this->status = Bi_ZhongShuChuLiStatus::UP;
    } else {
        if (xd3.get_type() == BiType::DOWN && xd3.get_low() < this->bi_zhongshu.get_low())
            this->status = Bi_ZhongShuChuLiStatus::DOWN;
        else
            this->status = Bi_ZhongShuChuLiStatus::INSIDE;
    }
}

Bi_ZhongShu Bi_ZhongShuChuLi::get_Bi_ZhongShu() {
    return(this->bi_zhongshu);
}

bool determ_bi_radio(Bi bi1, Bi bi2) {
    float radio = get_bi_radio(bi1, bi2);
    if (radio > 0.618)
        return(true);
    else
        return(false);
}


float get_bi_radio(Bi bi1, Bi bi2) {
    float radio = 0.0;
    float bi1_len = bi1.get_length();
    float bi2_len = bi2.get_length();
    if (bi2_len < bi1_len)
        radio = bi2_len / bi1_len;
    else
        radio = bi1_len / bi2_len * -1;
    return(radio);
}

Bi_ZhongShu Bi_ZhongShuChuLi::find_Bi_ZhongShu(Bi bi) {
    Bi last_bi = this->bi_zhongshu.bi_list.back();
    this->bi_zhongshu.bi_list.push_back(bi);
    Bi_ZhongShu ret_Bi_ZhongShu = Bi_ZhongShu();
    float bi_high = bi.get_high();
    float bi_low = bi.get_low();
    float bi_radio = 0.3 * this->bi_zhongshu.get_length();
    float bi_cha;

    if (this->bi_zhongshu.bi_list.size() >= 9) {
        //TODO: 中枢升级
        OutputDebugPrintf("ToDo 中枢升级");
    }
    else {
        if (bi.get_type() == BiType::UP) {
            if (bi_high > this->bi_zhongshu.get_high()) {
                bi_cha = bi_high - this->bi_zhongshu.get_high();
                if (bi_cha > bi_radio) {
                    this->bi_zhongshu.stop();
                    ret_Bi_ZhongShu = this->bi_zhongshu;
                }
            }
        }
        else {
            if (bi.get_type() == BiType::DOWN) {
                if (bi_low < this->bi_zhongshu.get_low()) {
                    bi_cha = this->bi_zhongshu.get_low() - bi_low;
                    if (bi_cha > bi_radio) {
                        this->bi_zhongshu.stop();
                        ret_Bi_ZhongShu = this->bi_zhongshu;
                    }
                }

            }
        }
    }
    return(ret_Bi_ZhongShu);
}
