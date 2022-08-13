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

bool is_bi_equal_1(float bi1_len, float bi2_len) {
    float radio;
    if (bi1_len < bi2_len) {
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


FindBiZhongShuReturn Bi_ZhongShuChuLi::find_Bi_ZhongShu(Bi bi) {
    FindBiZhongShuReturn ret_bi_zhongshu = FindBiZhongShuReturn();
    Bi last_bi = this->bi_zhongshu.bi_list.back();
    this->bi_zhongshu.bi_list.push_back(bi);
    Bi_ZhongShu ret_Bi_ZhongShu = Bi_ZhongShu();
    float bi_high = bi.get_high();
    float bi_low = bi.get_low();

    if (this->bi_zhongshu.bi_list.size() >= 10) {
        //TODO: 中枢升级
        Bi first_bi = bi.generate_bi(this->bi_zhongshu.bi_list[1], this->bi_zhongshu.bi_list[2], this->bi_zhongshu.bi_list[3]);
        Bi second_bi = bi.generate_bi(this->bi_zhongshu.bi_list[4], this->bi_zhongshu.bi_list[5], this->bi_zhongshu.bi_list[6]);
        Bi three_bi = bi.generate_bi(this->bi_zhongshu.bi_list[7], this->bi_zhongshu.bi_list[8], this->bi_zhongshu.bi_list[9]);
        Bi input = this->bi_zhongshu.get_input();
        
        ret_bi_zhongshu.bi_zhongshu = Bi_ZhongShu(input, first_bi, second_bi, three_bi);
        ret_bi_zhongshu.type = FindBiZhongShuReturnType::ZHONSHU_UPGRADE;
        OutputDebugPrintf("ToDo 中枢升级");
        return(ret_bi_zhongshu);
    }
    switch (bi.get_type()) {
    case BiType::UP:
        if (bi_high < this->bi_zhongshu.get_low()) {
            //3卖
            this->bi_zhongshu.stop(last_bi);
            ret_bi_zhongshu.bi_zhongshu = this->bi_zhongshu;
            ret_bi_zhongshu.type = FindBiZhongShuReturnType::THREE_SELL;
        }
        else {
            if (bi_high > this->bi_zhongshu.get_high()) {
                if (is_bi_equal_1(bi.get_length(), this->bi_zhongshu.get_length()) == false) {
                    if (this->bi_zhongshu.get_input().get_type() == BiType::UP) {
                        this->bi_zhongshu.stop(bi);
                        ret_bi_zhongshu.bi_zhongshu = this->bi_zhongshu;
                        ret_bi_zhongshu.type = FindBiZhongShuReturnType::ZhongShuSuccess;
                    }
                    else {
                        this->bi_zhongshu.stop(last_bi);
                        ret_bi_zhongshu.bi_zhongshu = this->bi_zhongshu;
                        ret_bi_zhongshu.type = FindBiZhongShuReturnType::ZhongShuFailer;
                    }
                }
            }
        }
        break;
    case BiType::DOWN:
        if (bi_low > this->bi_zhongshu.get_high()) {
            //3买
            this->bi_zhongshu.stop(last_bi);
            ret_bi_zhongshu.bi_zhongshu = this->bi_zhongshu;
            ret_bi_zhongshu.type = FindBiZhongShuReturnType::THREE_BUY;
        }
        else {
            if (bi_low < this->bi_zhongshu.get_low()) {
                if (is_bi_equal_1(bi.get_length(), this->bi_zhongshu.get_length()) == false) {
                    if (this->bi_zhongshu.get_input().get_type() == BiType::DOWN) {
                        this->bi_zhongshu.stop(bi);
                        ret_bi_zhongshu.bi_zhongshu = this->bi_zhongshu;
                        ret_bi_zhongshu.type = FindBiZhongShuReturnType::ZhongShuSuccess;
                    }
                    else {
                        this->bi_zhongshu.stop(last_bi);
                        ret_bi_zhongshu.bi_zhongshu = this->bi_zhongshu;
                        ret_bi_zhongshu.type = FindBiZhongShuReturnType::ZhongShuFailer;
                    }
                }
            }
        }
        break;
    }

    if (ret_bi_zhongshu.type == FindBiZhongShuReturnType::None)
        this->bi_zhongshu.set_stop_pos(bi);

    return(ret_bi_zhongshu);
}
