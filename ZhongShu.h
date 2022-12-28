#pragma once
#include "CompositeBi.h"

class ZhongShu {
    private:
        float high, low, max_high, min_low;
        int start_pos, stop_pos;
        CompositeBi input_bi, output_bi;
        CompositeBi max_bi, min_bi;
        float length;
    public:
        vector<CompositeBi> comp_bi_list;
        ZhongShu() {
            this->input_bi = CompositeBi();
            this->output_bi = CompositeBi();
            this->high = 0;
            this->low = 0;
            this->max_high = 0;
            this->min_low = 0;
            this->start_pos = 0;
            this->stop_pos = 0;
            this->max_bi = CompositeBi();
            this->min_bi = CompositeBi();
            this->length = 0;
            this->comp_bi_list.clear();
        }

        ZhongShu(CompositeBi in, CompositeBi bi1, CompositeBi bi2, CompositeBi bi3) {
            this->input_bi = in;
            this->high = min(bi1.get_high(), bi3.get_high());
            this->low = max(bi1.get_low(), bi3.get_low());
            this->max_high = max(bi1.get_high(), bi3.get_high());
            this->min_low = min(bi1.get_low(), bi3.get_low());
            this->comp_bi_list.push_back(bi1);
            this->comp_bi_list.push_back(bi2);
            this->comp_bi_list.push_back(bi3);

            this->start_pos = in.get_stop_pos();
            this->stop_pos = bi3.get_stop_pos();
            this->length = this->high - this->low;        
        } 

        void stop(CompositeBi comp_bi) {
            this->output_bi = comp_bi;
            this->stop_pos = comp_bi.get_stop_pos();
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

        CompositeBi get_input() {
            return(this->input_bi);
        }

        void set_stop_pos(CompositeBi comp_bi){
            this->stop_pos = comp_bi.get_stop_pos();
        }

        CompositeBi get_output() {
            return(this->output_bi);
        }

        int get_start_pos() {
            return(this->start_pos);
        }

        int get_stop_pos() {
            return(this->stop_pos);
        }

        CompositeBi get_max_bi(){
            return(this->max_bi);
        }

        CompositeBi get_min_bi(){
            return(this->min_bi);
        }

        CompositeBi find_next_xd(CompositeBi comp_bi){
            int nCount = this->comp_bi_list.size();
            for (int i = 0; i < nCount; i++){
                if (this->comp_bi_list[i] == comp_bi){
                    return(this->comp_bi_list[i+1]);
                }
            return(CompositeBi());
            }
        }
};