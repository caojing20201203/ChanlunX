#pragma once
#include "Bi1.h"

class Bi_ZhongShu {
    private:
        float high, low, max_high, min_low;
        int start_pos, stop_pos;
        Bi input_bi, output_bi;
        Bi max_bi, min_bi;
        float length;
    public:
        vector<Bi> bi_list;
        Bi_ZhongShu() {
            this->input_bi = Bi();
            this->output_bi = Bi();
            this->high = 0;
            this->low = 0;
            this->max_high = 0;
            this->min_low = 0;
            this->bi_list.clear();
            this->start_pos = 0;
            this->stop_pos = 0;
            this->max_bi = Bi();
            this->min_bi = Bi();
            this->length = 0;
        }

        Bi_ZhongShu(Bi in, Bi xd1, Bi xd2, Bi xd3) {
            this->input_bi = in;
            this->high = min(xd1.get_high(), xd3.get_high());
            this->low = max(xd1.get_low(), xd3.get_low());
            this->max_high = max(xd1.get_high(), xd3.get_high());
            this->min_low = min(xd1.get_low(), xd3.get_low());
            this->bi_list.push_back(xd1);
            this->bi_list.push_back(xd2);
            this->bi_list.push_back(xd3);

            this->start_pos = in.get_stop_pos();
            this->stop_pos = xd3.get_stop_pos();
            this->length = this->high - this->low;
        } 

        void stop(Bi bi) {
            this->output_bi = bi;
            this->stop_pos = bi.get_start_pos();
        }

        float get_length() {
            return(this->length);
        }

        float get_high() {
            return(this->high);
        }

        float get_max_high() {
            return(this->max_high);
        } 

        void set_max_high(Bi bi) {
            this->max_high = bi.get_high();
            this->max_bi = bi;
        }    

        float get_low() {
            return(this->low);
        }

        float get_min_low() {
            return(this->min_low);
        }

        void set_min_low(Bi bi) {
            this->min_low = bi.get_low();
            this->min_bi = bi;
        }

        Bi get_input() {
            return(this->input_bi);
        }

        void set_stop_pos(Bi bi) {
            this->stop_pos = bi.get_stop_pos();
        }

        Bi get_output() {
            return(this->output_bi);
        }

        int get_start_pos() {
            return(this->start_pos);
        }

        int get_stop_pos() {
            return(this->stop_pos);
        }

        Bi get_max_bi() {
            return(this->max_bi);
        }

        Bi get_min_bi() {
            return(this->min_bi);
        }

        Bi find_next_bi(Bi bi) {
            int nCount = this->bi_list.size();
            for (int i = 0; i < nCount; i++) {
                if (this->bi_list[i] == bi) {
                    return(this->bi_list[i + 1]);
                }
            }
            return(Bi());
        }
};

enum class FindBiZhongShuReturnType { None, THREE_BUY, THREE_SELL, ZHONSHU_UPGRADE, ZhongShuSuccess, ZhongShuFailer };
struct FindBiZhongShuReturn {
    FindBiZhongShuReturnType type;
    Bi_ZhongShu bi_zhongshu;
};
enum class FindZhongShuBiReturnType {None, ONE, TWO};
struct FindZhongShuBiReturn {
    FindZhongShuBiReturnType type;
    Bi bi1;
    Bi bi2;
};

/*
enum class Bi_ZhongShuChuLiStatus {NONE, UP, DOWN, INSIDE, THREE_BUY, THREE_SELL};
*/
enum class Bi_ZhongShuChuLiStatus {NONE, INPUT, ZS1, ZS2, ZS3, ZS4, ZS5, ZS6, ZS7, ZS8, THREE_BUY, THREE_SELL};

class Bi_ZhongShuChuLi {
    private:
        Bi_ZhongShu bi_zhongshu;
        Bi_ZhongShuChuLiStatus status;
        Bi input, zs1, zs2, zs3, zs4, zs5, zs6, zs7, zs8, output, zs_input, zs_output;
    public:
        Bi_ZhongShuChuLi();
        Bi_ZhongShuChuLi(Bi input, Bi xd1, Bi xd2, Bi xd3);

        FindBiZhongShuReturn find_Bi_ZhongShu(Bi xd);
        FindBiZhongShuReturn __find_Bi_ZhongShu(Bi xd);
        Bi_ZhongShu get_Bi_ZhongShu();
        FindZhongShuBiReturn generate_zs_bi(Bi bi);
        Bi get_last_zs_bi();

        Bi_ZhongShuChuLiStatus get_status() {
            return(this->status);
        }
};
