#pragma once
#include "XianDuan.h"

class ZhongShu {
    private:
        float high, low, max_high, min_low;
        int start_pos, stop_pos;
        XianDuan input_xd, output_xd;
        XianDuan max_xd, min_xd;
        float length;
    public:
        vector<XianDuan> xd_list;
        ZhongShu() {
            this->input_xd = XianDuan();
            this->output_xd = XianDuan();
            this->high = 0;
            this->low = 0;
            this->max_high = 0;
            this->min_low = 0;
            this->start_pos = 0;
            this->stop_pos = 0;
            this->max_xd = XianDuan();
            this->min_xd = XianDuan();
            this->length = 0;
            this->xd_list.clear();
        }

        ZhongShu(XianDuan in, XianDuan xd1, XianDuan xd2, XianDuan xd3) {
            this->input_xd = in;
            this->high = min(xd1.get_high(), xd3.get_high());
            this->low = max(xd1.get_low(), xd3.get_low());
            this->max_high = max(xd1.get_high(), xd3.get_high());
            this->min_low = min(xd1.get_low(), xd3.get_low());
            this->xd_list.push_back(xd1);
            this->xd_list.push_back(xd2);
            this->xd_list.push_back(xd3);

            this->start_pos = in.get_stop_pos();
            this->stop_pos = xd3.get_stop_pos();
            this->length = this->high - this->low;        
        } 

        void stop(XianDuan xd) {
            this->output_xd = xd;
            this->stop_pos = xd.get_stop_pos();
        }

        float get_length(){
            return(this->length);
        }

        float get_high() {
            return(this->high);
        }

        float get_max_high() {
            return(this->max_high);
        } 

        void set_max_high(float max_high) {
            this->max_high = max_high;
        }    

        float get_low() {
            return(this->low);
        }

        float get_min_low() {
            return(this->min_low);
        }

        void set_min_low(float min_low) {
            this->min_low = min_low;
        }

        XianDuan get_input() {
            return(this->input_xd);
        }

        void set_stop_pos(XianDuan xd){
            this->stop_pos = xd.get_stop_pos();
        }

        XianDuan get_output() {
            return(this->output_xd);
        }

        int get_start_pos() {
            return(this->start_pos);
        }

        int get_stop_pos() {
            return(this->stop_pos);
        }

        XianDuan get_max_xd(){
            return(this->max_xd);
        }

        XianDuan get_min_xd(){
            return(this->min_xd);
        }

        XianDuan find_next_xd(XianDuan xd){
            int nCount = this->xd_list.size();
            for (int i = 0; i < nCount; i++){
                if (this->xd_list[i] == xd){
                    return(this->xd_list[i+1]);
                }
            return(XianDuan());
            }
        }
};

enum class FindZhongShuReturnType {None, THREE_BUY, THREE_SELL, ZhongShu_upgrade, ZhongShuSuccess, ZhongShuFailer };
struct FindZhongShuReturn {
    FindZhongShuReturnType type;
    ZhongShu zhongshu;
};

enum class ZhongShuChuLiStatus {NONE, UP, DOWN, INSIDE, THREE_BUY, THREE_SELL};
class ZhongShuChuLi {
    private:
        ZhongShu zhongshu;
        ZhongShuChuLiStatus status;
    public:
        ZhongShuChuLi();
        ZhongShuChuLi(XianDuan input, XianDuan xd1, XianDuan xd2, XianDuan xd3);
        FindZhongShuReturn find_zhongshu(XianDuan xd);
        ZhongShu get_zhongshu();
        bool is_level_xd(XianDuan xd);
        XianDuan last_xd;

        ZhongShuChuLiStatus get_status(){
            return(this->status);
        }
};