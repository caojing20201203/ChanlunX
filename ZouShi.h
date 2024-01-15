#ifndef __ZOUSHI_H
#define __ZOUSHI_H

#include "XianDuan.h"
#include "Level.h"

using namespace std;
#pragma pack(push, 1)

class ZhongShu {
    private:
        Level level;
        XianDuan input;
        XianDuan output;
        float max_high, high, min_low, low;
        XianDuan max_xd, min_xd, after_max_xd, after_min_xd;
        XianDuan last_xd;
        int xd_count;

    public:
        ZhongShu() {
            this->input = XianDuan();
            this->output = XianDuan();
            this->max_high = 0;
            this->min_low = 0;
            this->high = 0;
            this->low = 0;
            this->max_xd = XianDuan();
            this->min_xd = XianDuan();
            this->after_max_xd = XianDuan();
            this->after_min_xd = XianDuan();
            this->last_xd = XianDuan();
            this->xd_count = 0;
        }

        ZhongShu(XianDuan input, XianDuan xd1, XianDuan xd2, XianDuan xd3, XianDuan output){
            this->input = input;
            this->max_high = max(xd1.get_high(), xd3.get_high());
            this->min_low = min(xd1.get_low(), xd3.get_low());
            this->high = min(xd1.get_high(), xd3.get_high());
            this->low = max(xd1.get_low(), xd3.get_low());
            if (xd1.get_high() == this->max_high) {
                this->max_xd = xd1;
                this->after_max_xd = xd2;
            } else {
                this->max_xd = xd3;
                this->after_max_xd = XianDuan();
            }
            if (xd1.get_low() == this->min_low){
                this->min_xd = xd1;
                this->after_min_xd = xd2;
            } else {
                this->min_xd = xd3;
                this->after_min_xd = XianDuan();
            }
            this->last_xd = xd3;
            this->xd_count = 3;
        }

        void set_output(XianDuan xd) {
            this->output = xd;
        }

        XianDuan get_after_max_xd() {
            return(this->after_max_xd);
        }

        void set_after_max_xd(XianDuan xd) {
            this->after_max_xd = xd;
        }

        XianDuan get_after_min_xd() {
            return(this->after_min_xd);
        }

        void set_after_min_xd(XianDuan xd) {
            this->after_min_xd = xd;
        }

        XianDuan get_max_xd() {
            return(this->max_xd);
        }

        void set_max_xd(XianDuan xd) {
            this->max_xd = xd;
            this->max_high = xd.get_high();
            this->after_max_xd = XianDuan();
        }

        XianDuan get_min_xd(XianDuan xd) {
            return(this->min_xd);
        }

        void set_min_xd(XianDuan xd) {
            this->min_xd = xd;
            this->min_low = xd.get_low();
            this->after_min_xd = XianDuan();
        }

        XianDuan get_last_xd() {
            return(this->last_xd);
        }

        void set_last_xd(XianDuan xd) {
            this->last_xd = xd;
        }

        float get_high() {
            return(this->high);
        }

        float get_low() {
            return(this->low);
        }

        float get_max_high() {
            return(this->max_high);
        }

        float get_min_low() {
            return(this->min_low);
        }

        int get_xd_count() {
            return(this->xd_count);
        }

        void set_xd_count(int count){
            this->xd_count = count;
        }
};

enum class ZouShiType {NONE, DAXIANDUAN_UP, UPDATE_DAXIANDUAN_UP, DAXIANDUAN_DOWN, UPDATE_DAXIANDUAN_DOWN, PANZHENG, UPDATE_PANZHENG, QUSHI_UP, UPDATE_QUSHI_UP, QUSHI_DOWN, UPDATE_QUSHI_DOWN};
enum class ZouShiDirection {NONE, UP, DOWN};
enum class ZouShiLevel {MONTH, WEEK, DAY, MIN_30, MIN_5, MIN_1};
class ZouShi {
    private:
        ZouShiType type;
        float high;
        float low;
        XianDuan a, b, c;
        ZhongShu A, B;
        XianDuan start_xd, stop_xd;
        float length;
        int count;

    public:
        ZouShi() {
            this->type = ZouShiType::NONE;
            this->high = 0;
            this->low = 0;
            this->a = XianDuan();
            this->b = XianDuan();
            this->c = XianDuan();
            this->A = ZhongShu();
            this->B = ZhongShu();
            this->start_xd = XianDuan();
            this->stop_xd = XianDuan(); 
            this->length = 0;
            this->count = 0;
        }

        ZouShi(XianDuan xd1, XianDuan xd2, XianDuan xd3, XianDuan xd4, XianDuan xd5){
            if (xd1.get_type() == XianDuanType::UP) {
                this->type = ZouShiType::DAXIANDUAN_UP;
                this->length = xd5.get_high() - xd1.get_low();
            } else {
                this->type = ZouShiType::DAXIANDUAN_DOWN;
                this->length = xd1.get_high() - xd5.get_low();
            }
            this->start_xd = xd1;
            this->stop_xd = xd5;
            //this->count = xd5.get_count() - xd1.get_count();
        }

        ZouShi(XianDuan a, ZhongShu A, XianDuan b){
            this->type = ZouShiType::PANZHENG;
            this->start_xd = a;
            this->A = A;
            this->stop_xd = b;
            this->a = a;
            this->b = b;
            if (a.get_type() == XianDuanType::UP){
                this->length = b.get_high() - a.get_low();
            } else {
                this->length = a.get_high() - b.get_low();
            }
        }

        ZouShi(XianDuan a, ZhongShu A, XianDuan b, ZhongShu B, XianDuan c){
            if (a.get_type() == XianDuanType::UP){
                this->type = ZouShiType::QUSHI_UP;
                this->length = c.get_high() - a.get_low();
            } else {
                this->type = ZouShiType::QUSHI_DOWN;
                this->length = a.get_high() - c.get_low();
            }
            this->a = a;
            this->A = A;
            this->b = b;
            this->B = B;
            this->c = c;
            this->start_xd = a;
            this->stop_xd = c;
        }

        ZouShiType get_type(){
            return(this->type);
        }

        void set_type(ZouShiType type){
            this->type = type;
        }

        float get_high(){
            return(this->high);
        }

        float get_low() {
            return(this->low);
        }

        float get_length(){
            return(this->length);
        }

        int get_start_pos() {
            return(this->start_xd.get_start_pos());
        }

        int get_stop_pos(){
            return(this->stop_xd.get_stop_verify_pos());
        }

        XianDuan get_start_xd(){
            return(this->start_xd);
        }

        XianDuan get_stop_xd() {
            return(this->stop_xd);
        }

        void set_stop_xd(XianDuan xd) {
            this->stop_xd = xd;
            if (this->start_xd.get_type() == XianDuanType::UP){
                this->length = xd.get_high() - this->start_xd.get_low();
            } else {
                this->length = this->start_xd.get_high() - xd.get_low();
            }
        }

        bool operator==(ZouShi comp){
            if (this->type == comp.type && this->start_xd == comp.start_xd && this->stop_xd == comp.stop_xd)
                return(true);
            else
                return(false);
        }
};

enum class ZouShiChuLiStatus {a1, a2, a1_equal_a2, a1_equal_a2_normal, a1_equal_a2_normal_normal, a2_normal, a2_equal_a3, a2_normal_normal, a3, a3_normal, a4, A, b3, b, B, c};
class ZouShiChuLi {
    private:
        ZouShiChuLiStatus status;
        XianDuan a1, a2, a3, a4;
        XianDuan b1, b2, b3;
        XianDuanChuLi xdcl;
        ZouShi last_zoushi;
        ZhongShu A, B;
    public:
        ZouShiChuLi();
        void handle(vector<Kxian1>& xdlist);
        ZouShi __find_zoushi(XianDuan xd);
        ZouShi failure_zoushi(XianDuan xd1, XianDuan xd2, XianDuan xd3);
        void zhongshu_process(ZhongShu zs, XianDuan xd);
        vector<ZouShi> zoushi_list;
};

void ZouShi_process(int count, float *pOut, float *pHigh, float *pLow, float *pIn);

#endif