#pragma once
#include "bi1.h"
#include<vector>

using namespace std;

enum class CompositeBiType {NONE, UP, DOWN, TEMP_UP, TEMP_DOWN};
enum class CompositeBiKide {None, SignlBi, ThreeBi, FiveBi, SevenBi, NineBi};
class CompositeBi{
    private:
        CompositeBiType type = CompositeBiType::NONE;
        vector<Bi> bi_list;
        Bi start_bi, stop_bi;
        int start_pos, stop_pos, verify_pos;
        float high, low, length;
        CompositeBiKide kind;

    public:
        CompositeBi(){
            this->type = CompositeBiType::NONE;
            this->high = 0;
            this->low = 0;
            this->length = 0;
            this->start_bi = Bi();
            this->start_pos = 0;
            this->stop_bi = Bi();
            this->stop_pos = 0;
            this->verify_pos = 0;
            this->kind = CompositeBiKide::None;
        }

        CompositeBi(Bi start_bi, Bi stop_bi) {
            this->start_bi = start_bi;
            this->stop_bi = stop_bi;
            this->high = 0;
            this->low = 0;
            this->length = 0;
            this->start_pos = start_bi.get_start_fx().get_middle().get_position();
            this->stop_pos = stop_bi.get_stop_fx().get_middle().get_position();
            this->verify_pos = stop_bi.get_stop_fx().get_free().get_position();

            if (start_bi.get_type() == BiType::DOWN) {
                this->high = start_bi.get_high();
                this->low = stop_bi.get_low();
                this->length = this->high - this->low;
                this->type = CompositeBiType::DOWN;
            }
            else {
                this->low = start_bi.get_low();
                this->high = stop_bi.get_high();
                this->length = this->high - this->low;
                this->type = CompositeBiType::UP;
            }
        }
    
        CompositeBiType get_type() {
            return(this->type);
        }

        void set_type(CompositeBiType cb_type) {
            this->type = cb_type;
        }

        CompositeBiKide get_kind() {
            return(this->kind);
        }

        void set_kind(CompositeBiKide kind) {
            this->kind = kind;
        }

        float get_high() {
            return(this->high);
        }

        float get_low() {
            return(this->low);
        }

        Bi get_start_bi() {
            return(this->start_bi);
        }

        Bi get_stop_bi() {
            return(this->stop_bi);
        }

        int get_start_pos() {
            return(this->start_pos);
        }

        int get_stop_pos() {
            return(this->stop_pos);
        }

        int get_stop_verify_pos() {
            return(this->verify_pos);
        }

        float get_length() {
            return(this->length);
        }

        bool operator==(CompositeBi that) {
            if (this->start_bi == that.start_bi && this->stop_bi == that.stop_bi && this->type == that.type)
                return(true);
            else
                return(false);
        }

        void save_bi_list(vector<Bi> bilist) {
            this->bi_list = bilist;
        }

        vector<Bi> get_bi_list() {
            return(this->bi_list);
        }
};

enum class FindCompositeBiReturnType {None, Failure, One, Two, Three};
struct FindCompositeBiReturn {
    FindCompositeBiReturnType type;
    CompositeBi csBi1;
    CompositeBi csBi2;
    CompositeBi csBi3;
};

enum class CompositeBiChuLiStatus { START, LEFT, AFTER_LEFT, AFTER_LEFT_HIGHLOW, AFTER_LEFT_NORMAL, AFTER_LEFT_NORMAL_NORMAL};
class CompositeBiChuLi {
    private:
        CompositeBiChuLiStatus status;
        BiChuLi bicl;
        Bi start, left, after_left, middle, after_middle, right;
        FindCompositeBiReturn __find_CompositeBi(Bi bi);
    public:
        CompositeBiChuLi();
        void handle(vector<Kxian1>& kxlist);
        vector<CompositeBi> composite_bi_list;
};

void Bi3_CompositeBi(int nCount, float *pOut, float *pHigh, float *pLow, float *pIn);
