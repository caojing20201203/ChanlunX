#include "Bi_ZhongShu.h"

using namespace std;

Bi_ZhongShuChuLi::Bi_ZhongShuChuLi(){
    this->bi_zhongshu = Bi_ZhongShu();
    this->zs_input = Bi();
    this->zs_output = Bi();
}

Bi_ZhongShuChuLi::Bi_ZhongShuChuLi(Bi input, Bi xd1, Bi xd2, Bi xd3) {
    this->bi_zhongshu = Bi_ZhongShu(input, xd1, xd2, xd3);
    this->input = input;
    this->zs1 = xd1;
    this->zs2 = xd2;
    this->zs3 = xd3;
    this->status = Bi_ZhongShuChuLiStatus::ZS4;
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

Bi Bi_ZhongShuChuLi::get_last_zs_bi() {
    switch (this->status) {
    case Bi_ZhongShuChuLiStatus::ZS8:
        this->status = Bi_ZhongShuChuLiStatus::ZS7;
        return(this->zs7);
        break;
    case Bi_ZhongShuChuLiStatus::ZS7:
        this->status = Bi_ZhongShuChuLiStatus::ZS6;
        return(this->zs6);
        break;
    case Bi_ZhongShuChuLiStatus::ZS6:
        this->status = Bi_ZhongShuChuLiStatus::ZS5;
        return(this->zs5);
        break;
    case Bi_ZhongShuChuLiStatus::ZS5:
        this->status = Bi_ZhongShuChuLiStatus::ZS4;
        return(this->zs4);
        break;
    case Bi_ZhongShuChuLiStatus::ZS4:
        this->status = Bi_ZhongShuChuLiStatus::ZS3;
        return(this->zs3);
        break;
    case Bi_ZhongShuChuLiStatus::ZS3:
        this->status = Bi_ZhongShuChuLiStatus::ZS2;
        return(this->zs2);
        break;
    case Bi_ZhongShuChuLiStatus::ZS2:
        this->status = Bi_ZhongShuChuLiStatus::ZS1;
        return(this->zs1);
        break;
    case Bi_ZhongShuChuLiStatus::ZS1:
        this->status = Bi_ZhongShuChuLiStatus::INPUT;
        return(this->input);
        break;
        
    }
}

FindZhongShuBiReturn Bi_ZhongShuChuLi::generate_zs_bi(Bi bi) {
    FindZhongShuBiReturn ret_bi = FindZhongShuBiReturn();
    BiType bi_type = bi.get_type();
    float bi_high = bi.get_high();
    float bi_low = bi.get_low();
    Bi_ZhongShu zhongshu = this->get_Bi_ZhongShu();
    float zs_high = zhongshu.get_high();
    float zs_low = zhongshu.get_low();
    BiType zs_input_type = this->zs_input.get_type();

    switch (zs_input_type) {
    case BiType::NONE:
        if (bi_type == BiType::UP && bi_high < zs_high && bi_low < zs_low) {
            this->zs_input = bi;
        }
        else {
            if (bi_type == BiType::DOWN && bi_low > zs_low && bi_high > zs_high) {
                this->zs_input = bi;
            }
            else {
                ret_bi.bi1 = bi;
                ret_bi.type = FindZhongShuBiReturnType::ONE;
            }
        }
        break;
    case BiType::UP:
        if (bi_type == BiType::UP && bi_high > zs_high) {
            ret_bi.bi1 = bi.generate_bi(this->zs_input, Bi(), bi);
            ret_bi.type = FindZhongShuBiReturnType::ONE;
            this->zs_input = Bi();
            this->zs_output = Bi();
        }
        else {
            if (bi_type == BiType::DOWN && bi_low < zs_low) {
                ret_bi.bi1 = this->zs_input;
                ret_bi.bi2 = bi;
                ret_bi.type = FindZhongShuBiReturnType::TWO;
                this->zs_input = Bi();
                this->zs_output = Bi();
            }
            else {
                this->bi_zhongshu.bi_list.push_back(this->zs_input);
                this->zs_output = bi;
            }
        }
        break;
    case BiType::DOWN:
        if (bi_type == BiType::DOWN && bi_low < zs_low) {
            ret_bi.bi1 = bi.generate_bi(this->zs_input, Bi(), bi);
            ret_bi.type = FindZhongShuBiReturnType::ONE;
            this->zs_input = Bi();
            this->zs_output = Bi();
        }
        else {
            if (bi_type == BiType::UP && bi_high > zs_high) {
                ret_bi.bi1 = this->zs_input;
                ret_bi.bi2 = bi;
                ret_bi.type = FindZhongShuBiReturnType::TWO;
                this->zs_input = Bi();
                this->zs_output = Bi();
            }
            else {
                this->bi_zhongshu.bi_list.push_back(this->zs_input);
                this->zs_output = bi;
            }
        }
        break;
    }
    return(ret_bi);
}

FindBiZhongShuReturn Bi_ZhongShuChuLi::find_Bi_ZhongShu(Bi bi) {
    FindBiZhongShuReturn ret_zs = FindBiZhongShuReturn();
    FindZhongShuBiReturn ret_bi;

    ret_bi = this->generate_zs_bi(bi);
    switch (ret_bi.type) {
    case FindZhongShuBiReturnType::None:
        break;
    case FindZhongShuBiReturnType::TWO:
        switch (this->status) {
        case Bi_ZhongShuChuLiStatus::ZS4:
            this->zs4 = bi;
            this->status = Bi_ZhongShuChuLiStatus::ZS5;
            break;
        case Bi_ZhongShuChuLiStatus::ZS5:
            this->zs5 = bi;
            this->status = Bi_ZhongShuChuLiStatus::ZS6;
            break;
        case Bi_ZhongShuChuLiStatus::ZS6:
            this->zs6 = bi;
            this->status = Bi_ZhongShuChuLiStatus::ZS7;
            break;
        case Bi_ZhongShuChuLiStatus::ZS7:
            this->zs7 = bi;
            this->status = Bi_ZhongShuChuLiStatus::ZS8;
        }

    case FindZhongShuBiReturnType::ONE:
        ret_zs = this->__find_Bi_ZhongShu(bi);
        break;
    }
    return(ret_zs);
}

FindBiZhongShuReturn Bi_ZhongShuChuLi::__find_Bi_ZhongShu(Bi bi) {
    FindBiZhongShuReturn ret_bi_zhongshu = FindBiZhongShuReturn();
    Bi last_bi;
    Bi inside_input, inside_output;

    if (this->bi_zhongshu.bi_list.empty())
        last_bi = Bi();
    else {
        last_bi = this->bi_zhongshu.bi_list.back();
        this->bi_zhongshu.bi_list.push_back(bi);
        switch (bi.get_type()) {
        case BiType::UP:
            if (bi.get_high() > this->bi_zhongshu.get_max_high()) {
                this->bi_zhongshu.set_max_high(bi);
                if (bi.get_high() - this->bi_zhongshu.get_high() > this->bi_zhongshu.get_length()){
                    this->bi_zhongshu.stop(bi);
                    ret_bi_zhongshu.bi_zhongshu = this->bi_zhongshu;
                    ret_bi_zhongshu.type = FindBiZhongShuReturnType::ZhongShuSuccess;
                    return(ret_bi_zhongshu);
                }
            }
            break;
        case BiType::DOWN:
            if (bi.get_low() < this->bi_zhongshu.get_min_low()) {
                this->bi_zhongshu.set_min_low(bi);
                if (this->bi_zhongshu.get_low() - bi.get_low() > this->bi_zhongshu.get_length()){
                    this->bi_zhongshu.stop(bi);
                    ret_bi_zhongshu.bi_zhongshu = this->bi_zhongshu;
                    ret_bi_zhongshu.type = FindBiZhongShuReturnType::ZhongShuSuccess;
                    return(ret_bi_zhongshu);
                }
            }
            break;
        }
    }
    Bi_ZhongShu ret_Bi_ZhongShu = Bi_ZhongShu();
    float bi_high = bi.get_high();
    float bi_low = bi.get_low();

    if (this->bi_zhongshu.bi_list.size() >= 10) {
        //TODO: 中枢升级
        Bi first_bi = bi.generate_bi(this->bi_zhongshu.bi_list[0], this->bi_zhongshu.bi_list[1], this->bi_zhongshu.bi_list[2]);
        Bi second_bi = bi.generate_bi(this->bi_zhongshu.bi_list[3], this->bi_zhongshu.bi_list[4], this->bi_zhongshu.bi_list[5]);
        Bi three_bi = bi.generate_bi(this->bi_zhongshu.bi_list[6], this->bi_zhongshu.bi_list[7], this->bi_zhongshu.bi_list[8]);
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
                if (bi_high - this->bi_zhongshu.get_high() > this->bi_zhongshu.get_length()) {
                    this->bi_zhongshu.stop(bi);
                    ret_bi_zhongshu.bi_zhongshu = this->bi_zhongshu;
                    ret_bi_zhongshu.type = FindBiZhongShuReturnType::ZhongShuSuccess;
                }
                else {
                    if (bi_low < this->bi_zhongshu.get_low()) {
                        if (inside_input.get_type() == BiType::NONE) {
                            inside_input = last_bi;
                        }
                    }
                }
            }
            else {

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
            if (bi_low < this->bi_zhongshu.get_low() && (this->bi_zhongshu.get_low() - bi_low > this->bi_zhongshu.get_length())) {
                this->bi_zhongshu.stop(bi);
                ret_bi_zhongshu.bi_zhongshu = this->bi_zhongshu;
                ret_bi_zhongshu.type = FindBiZhongShuReturnType::ZhongShuSuccess;
            }
        }
        break;
    }

    if (ret_bi_zhongshu.type == FindBiZhongShuReturnType::None)
        this->bi_zhongshu.set_stop_pos(bi);

    return(ret_bi_zhongshu);
}
