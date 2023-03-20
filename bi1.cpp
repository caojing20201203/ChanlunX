#include <iostream>
#include <fstream>
#include "Bi1.h"
#include <vector>

using namespace std;

bool debug_bi_status = false;

#define PRE_START           0
#define PRE_LEFT            1
#define PRE_AFTER_LEFT      2
#define PRE_AFTER_LEFT_HIGHLOW    3
#define PRE_AFTER_LEFT_NORMAL     4

using namespace std;

char* get_pre_status(int status) {
    switch (status) {
    case PRE_START:
        return("PRE_STATUS");
    case PRE_LEFT:
        return("PRE_LEFT");
    case PRE_AFTER_LEFT:
        return("PRE_AFTER_LEFT");
    case PRE_AFTER_LEFT_HIGHLOW:
        return("PRE_AFTER_LEFT_HIGHLOW");
    case PRE_AFTER_LEFT_NORMAL:
        return("PRE_AFTER_LEFT_NORMAL");
    }
}


vector<Bi> pre_bi_process(vector<Bi> biList) {
    vector<Bi> ret_list;
    int count, i, j;
    int status = PRE_START;
    count = biList.size();
    Bi first, second, three, bi;
    Bi bi1, bi2;

    for (int i = 0; i < count; i++) {
        bi = biList[i];
        if (bi.get_type() == BiType::UP)
            OutputDebugPrintf("上升笔  %s %f %f 【%f】", get_pre_status(status), bi.get_low(), bi.get_high(), bi.get_length());
        else
            OutputDebugPrintf("下降笔  %s %f %f 【%f】", get_pre_status(status), bi.get_high(), bi.get_low(), bi.get_length());

        switch (status) {
        case PRE_START:
            first = bi;
            status = PRE_LEFT;
            break;
        case PRE_LEFT:
            switch (bi.get_type()) {
            case BiType::UP:
                if (bi.get_high() > first.get_high()) {
                    ret_list.push_back(first);
                    first = bi;
                    status = PRE_LEFT;
                }
                else {
                    second = bi;
                    status = PRE_AFTER_LEFT;
                }
                break;
            case BiType::DOWN:
                if (bi.get_low() < first.get_low()) {
                    ret_list.push_back(first);
                    first = bi;
                    status = PRE_LEFT;
                }
                else {
                    second = bi;
                    status = PRE_AFTER_LEFT;
                }
                break;
            }
            break;
        case PRE_AFTER_LEFT:
            switch (bi.get_type()) {
            case BiType::UP:
                if (bi.get_high() > first.get_high()) {
                    three = bi;
                    status = PRE_AFTER_LEFT_HIGHLOW;
                }
                else {
                    three = bi;
                    status = PRE_AFTER_LEFT_NORMAL;
                }
                break;
            case BiType::DOWN:
                if (bi.get_low() < first.get_low()) {
                    three = bi;
                    status = PRE_AFTER_LEFT_HIGHLOW;
                }
                else {
                    three = bi;
                    status = PRE_AFTER_LEFT_NORMAL;
                }
                break;
            }
            break;
        case PRE_AFTER_LEFT_HIGHLOW:
            switch (bi.get_type()) {
            case BiType::UP:
                if (bi.get_high() > second.get_high()) {
                    first = bi.generate_bi(first, second, three);
                    ret_list.push_back(first);
                    first = bi;
                    second = Bi();
                    three = Bi();
                    status = PRE_LEFT;
                }
                else {
                    first = bi.generate_bi(first, second, three);
                    second = bi;
                    three = Bi();
                    status = PRE_AFTER_LEFT;
                }
                break;
            case BiType::DOWN:
                if (bi.get_low() < second.get_low()) {
                    first = bi.generate_bi(first, second, three);
                    ret_list.push_back(first);
                    first = bi;
                    second = Bi();
                    three = Bi();
                    status = PRE_LEFT;
                }
                else {
                    first = bi.generate_bi(first, second, three);
                    second = bi;
                    three = Bi();
                    status = PRE_AFTER_LEFT;
                }
                break;
            }
        case PRE_AFTER_LEFT_NORMAL:
            switch (bi.get_type()) {
            case BiType::UP:
                if (first.get_type() == BiType::DOWN) {
                    if (bi.get_high() > first.get_high()) {
                        ret_list.push_back(first);
                        first = bi.generate_bi(second, three, bi);
                        second = Bi();
                        three = Bi();
                        status = PRE_LEFT;
                    }
                    else {
                        if (bi.get_high() > second.get_high()) {
                            second = bi.generate_bi(second, three, bi);
                            status = PRE_AFTER_LEFT;
                        }
                    }
                }
                else {
                    if (bi.get_high() > first.get_high()) {
                        three = bi.generate_bi(three, Bi(), bi);
                        status = PRE_AFTER_LEFT_HIGHLOW;
                    }
                    else {
                        if (bi.get_high() > three.get_high()) {
                            three = bi.generate_bi(three, Bi(), bi);
                            status = PRE_AFTER_LEFT_NORMAL;
                        }
                    }
                }
                break;
            case BiType::DOWN:
                if (first.get_type() == BiType::UP) {
                    if (bi.get_low() < first.get_low()) {
                        ret_list.push_back(first);
                        first = bi.generate_bi(second, three, bi);
                        second = Bi();
                        three = Bi();
                        status = PRE_LEFT;
                    }
                    else {
                        if (bi.get_low() < second.get_low()) {
                            second = bi.generate_bi(second, three, bi);
                            status = PRE_AFTER_LEFT;
                        }
                    }
                }
                else {
                    if (bi.get_low() < first.get_low()) {
                        three = bi.generate_bi(three, Bi(), bi);
                        status = PRE_AFTER_LEFT_HIGHLOW;
                    }
                    else {
                        if (bi.get_low() < three.get_low()) {
                            three = bi.generate_bi(three, Bi(), bi);
                            status = PRE_AFTER_LEFT_NORMAL;
                        }
                    }
                }
                break;
            }
            break;
        }
    }
    switch (status) {
    case PRE_LEFT:
        if (first.get_type() == BiType::UP)
            first.set_type(BiType::TEMP_UP);
        else
            first.set_type(BiType::TEMP_DOWN);
        ret_list.push_back(first);
        break;
    case PRE_AFTER_LEFT:
        if (first.get_type() == BiType::UP) {
            first.set_type(BiType::TEMP_UP);
            second.set_type(BiType::TEMP_DOWN);
        }
        else {
            first.set_type(BiType::TEMP_DOWN);
            second.set_type(BiType::TEMP_UP);
        }
        ret_list.push_back(first);
        ret_list.push_back(second);
        break;
    case PRE_AFTER_LEFT_HIGHLOW:
        if (first.get_type() == BiType::UP) {
            first.set_type(BiType::TEMP_UP);
            second.set_type(BiType::TEMP_DOWN);
            three.set_type(BiType::TEMP_UP);
        }
        else {
            first.set_type(BiType::TEMP_DOWN);
            second.set_type(BiType::TEMP_UP);
            three.set_type(BiType::TEMP_DOWN);
        }
        ret_list.push_back(first);
        ret_list.push_back(second);
        ret_list.push_back(three);
        break;
    case PRE_AFTER_LEFT_NORMAL:
        if (first.get_type() == BiType::UP) {
            first.set_type(BiType::TEMP_UP);
            second.set_type(BiType::TEMP_DOWN);
            three.set_type(BiType::TEMP_UP);
        }
        else {
            first.set_type(BiType::TEMP_DOWN);
            second.set_type(BiType::TEMP_UP);
            three.set_type(BiType::TEMP_DOWN);
        }
        ret_list.push_back(first);
        ret_list.push_back(second);
        ret_list.push_back(three);
        break;
    }
    return(ret_list);
}



BiChuLi::BiChuLi() {
    this->fxcl = FenXingChuLi();
    this->status = BiChuLiStatus::START;
}

void debug_bi(Bi bi){
    if (debug_bi_status) {
        BiType bi_type = bi.get_type();
        switch(bi_type) {
            case BiType::NONE:
                break;
            case BiType::UP:
                OutputDebugPrintf("【上升笔】 %f %f %d %d", bi.get_low(), bi.get_high(), bi.get_start_pos(), bi.get_stop_pos());
                break;
            case BiType::FAILURE_UP:
                OutputDebugPrintf("【失败上升笔】 %f %f %d %d", bi.get_low(), bi.get_high(), bi.get_start_pos(), bi.get_stop_pos());
                break;        
            case BiType::TEMP_UP: 
                OutputDebugPrintf("【临时上升笔】 %f %f %d %d", bi.get_low(), bi.get_high(), bi.get_start_pos(), bi.get_stop_pos());
                break;
            case BiType::FAILURE_TEMP_UP:
                OutputDebugPrintf("【失败 临时上升笔】 %f %f %d %d", bi.get_low(), bi.get_high(), bi.get_start_pos(), bi.get_stop_pos());
                break;        
            case BiType::NEW_UP:
                OutputDebugPrintf("【新的 上升笔】 %f %f %d %d", bi.get_low(), bi.get_high(), bi.get_start_pos(), bi.get_stop_pos());
                break;            
            case BiType::DOWN:
                OutputDebugPrintf("【下降笔】 %f %f %d %d", bi.get_high(), bi.get_low(), bi.get_start_pos(), bi.get_stop_pos());
                break;
            case BiType::FAILURE_DOWN:
                OutputDebugPrintf("【失败 下降笔】 %f %f %d %d", bi.get_high(), bi.get_low(), bi.get_start_pos(), bi.get_stop_pos());
                break;        
            case BiType::TEMP_DOWN:
                OutputDebugPrintf("【临时下降笔】 %f %f %d %d", bi.get_high(), bi.get_low(), bi.get_start_pos(), bi.get_stop_pos());
                break;  
            case BiType::FAILURE_TEMP_DOWN:
                OutputDebugPrintf("【失败 临时下降笔】 %f %f %d %d", bi.get_high(), bi.get_low(), bi.get_start_pos(), bi.get_stop_pos());
                break;
            case BiType::NEW_DOWN:
                OutputDebugPrintf("【新的 下降笔】 %f %f %d %d", bi.get_high(), bi.get_low(), bi.get_start_pos(), bi.get_stop_pos());
                break;
            default:
                OutputDebugPrintf("【未处理的选项】%d ", bi_type);
                break;          
        }
    }

}

vector<Bi> BiChuLi::__baohan_process(int start, int stop) {
    vector<Bi> tmp_list;
    Bi start_bi, stop_bi;
    float comp_bi_high, comp_bi_low, bi_high, bi_low;
    int end_num;

    start_bi = this->biList[start];
    stop_bi = this->biList[stop];
    if (start_bi.get_type() == BiType::UP) {
        comp_bi_high = stop_bi.get_high();
        comp_bi_low = stop_bi.get_low();
        for (int num = start; num < stop; num++) {
            bi_high = this->biList[num].get_high();
            bi_low = this->biList[num].get_low();
            if (comp_bi_low < bi_low) {
                if (num == start) {
                    start_bi = this->biList[num + 1];
                }
                else {
                    for (int ij = start; ij < num; ij++) {
                        tmp_list.push_back(this->biList[ij]);
                    }
                    start_bi = this->biList[num];
                    start_bi = start_bi.generate_bi(start_bi, Bi(), stop_bi);
                    tmp_list.push_back(start_bi);
                }
                if (start_bi.get_type() != stop_bi.get_type()) {
                    start_bi = this->biList[num+1];
                    stop_bi = this->biList[stop - 1];
                    start_bi = start_bi.generate_bi(start_bi, Bi(), stop_bi);
                    tmp_list.push_back(this->biList[start]);
                    tmp_list.push_back(start_bi);
                }

                break;
            }
            else {
                if (comp_bi_high > bi_high) {
                    end_num = num;
                    for (int ij = start; ij < num; ij++) {
                        tmp_list.push_back(this->biList[ij]);
                    }
                    start_bi = this->biList[num];
                    start_bi = start_bi.generate_bi(start_bi, Bi(), stop_bi);
                    tmp_list.push_back(start_bi);
                    break;
                }
            }
        }
    }
    else {
        if (start_bi.get_type() == BiType::DOWN) {
            comp_bi_high = stop_bi.get_high();
            comp_bi_low = stop_bi.get_low();
            for (int num = start; num < stop; num++) {
                bi_high = this->biList[num].get_high();
                bi_low = this->biList[num].get_low();
                if (bi_high < comp_bi_high) {
                    if (num == start) {
                        start_bi = this->biList[num + 1];
                    }
                    else {
                        for (int ij = start; ij < num; ij++) {
                            tmp_list.push_back(this->biList[ij]);
                        }
                        start_bi = this->biList[num];
                    }
                    if (start_bi.get_type() != stop_bi.get_type()) {
                        stop_bi = this->biList[stop - 1];
                    }
                    start_bi = start_bi.generate_bi(start_bi, Bi(), stop_bi);
                    tmp_list.push_back(start_bi);
                    break;
                }
                else {
                    if (comp_bi_low < bi_low) {
                        end_num = num;
                        for (int ij = start; ij < num; ij++) {
                            tmp_list.push_back(this->biList[ij]);
                        }
                        start_bi = this->biList[num];
                        start_bi = start_bi.generate_bi(start_bi, Bi(), stop_bi);
                        tmp_list.push_back(start_bi);
                        break;
                    }
                }
            }
        }
    }
    return(tmp_list);
}


bool is_baohan(Bi bi1, Bi bi2) {
    if (bi1.get_high() >= bi2.get_high() && bi1.get_low() <= bi2.get_low())
        return(true);
    else
        return(false);
}


vector<Bi> BiChuLi::__remove_baohan_bi() {
    int i, j, k;
    int start = 0, stop = 0, cnt;
    vector<Bi> ret_list, tmp_list;
    Bi start_bi, stop_bi, bi, last_bi;

    int count = this->keyBiList.size();
    if (count <= 1) {
        ret_list = this->keyBiList;
    }
    else {
        start = 1;
        start_bi = this->keyBiList[0];
        last_bi = this->keyBiList[1];
        for (i = start + 2; i < count; i += 2) {
            bi = this->keyBiList[i];
            if (is_baohan(last_bi, bi)) {
                last_bi = bi;
                for (j = i + 2; j < count; j += 2) {
                    bi = this->keyBiList[j];
                    if (is_baohan(last_bi, bi)) {
                        last_bi = bi;
                    }
                    else {
                        if (start == 1)
                            ret_list.push_back(this->keyBiList[0]);

                        stop = j;
                        tmp_list = this->__baohan_process(start, stop);
                        cnt = tmp_list.size();
                        for (k = 0; k < cnt; k++) {
                            ret_list.push_back(tmp_list[k]);
                        }
                        i = j;
                        if (tmp_list[cnt - 1].get_stop_fx() == this->keyBiList[j].get_stop_fx()) {
                            last_bi = tmp_list[cnt - 1];
                        }
                        else {
                            last_bi = this->biList[j];
                        }
                        start = stop;
                        break;
                    }
                }
            }
            else {
                stop = i;
                if (start == 1)
                    ret_list.push_back(this->keyBiList[0]);
                if (last_bi.get_start_fx() == this->keyBiList[start].get_start_fx()) {
                    cnt = start;
                }
                else {
                    cnt = start + 1;
                }
                for (k = cnt; k < stop; k++)
                    ret_list.push_back(this->keyBiList[k]);
                start = stop;
                last_bi = this->keyBiList[stop];
            }
        }
    }
    for (k = stop+1; k < count; k++)
        ret_list.push_back(this->keyBiList[k]);

    return(ret_list);
}

/*
vector<Bi> BiChuLi::__remove_baohan_bi() {
    vector<Bi> ret_bi_list;
    vector<Bi> tmp_list;
    Bi bi, last_bi, first_bi;
    Bi start_bi, stop_bi;
    int baohan_num = 0, start_num, stop_num, tmp_list_num;
    int i, j, k;
    int count = this->keyBiList.size();
    if (count > 0) {
        start_num = 0;
        first_bi = this->keyBiList[0];
        last_bi = this->keyBiList[1];
        if ((first_bi.get_type() == BiType::UP && last_bi.get_low() < first_bi.get_low())
          || (first_bi.get_type() == BiType::DOWN && last_bi.get_high() > first_bi.get_high())) {
            ret_bi_list.push_back(first_bi);
            start_num++;
            first_bi = last_bi;
            if (start_num < count)
                last_bi = this->biList[start_num];
        }

        for (i = start_num + 2; i < count; i += 2) {
            bi = this->keyBiList[i];
            if (is_baohan(last_bi, bi)) {
                last_bi = bi;
                for (j = i + 2; j < count; j += 2) {
                    bi = this->biList[j];
                    if (is_baohan(last_bi, bi)) {
                        last_bi = bi;
                    }
                    else {
                        tmp_list = __baohan_process(start_num, j);
                        tmp_list_num = tmp_list.size();
                        for (int ij = 0; ij < tmp_list_num; ij++) {
                            ret_bi_list.push_back(tmp_list[ij]);
                        }
                        i = j;
                        if (tmp_list[tmp_list_num - 1].get_stop_fx() == this->biList[j].get_stop_fx()) {
                            last_bi = tmp_list[tmp_list_num - 1];
                        }
                        else {
                            last_bi = this->biList[j];
                        }
                        start_num = j;
                        break;
                    }
                }
            }
            else {
                stop_num = i;
                for (int l = start_num; l < stop_num; l++) {
                    ret_bi_list.push_back(this->biList[l]);
                }
                last_bi = bi;
                start_num = i;
            }
        }
    }
    return(ret_bi_list);
}
*/

void BiChuLi::handle(vector<Kxian1>& kxianList) {
    Bi bi = Bi();
    this->fxcl.handle(kxianList);
    int count = this->fxcl.keyFenXingList.size();

    for (int i = 0; i < count; i++) {
        FenXing fx = this->fxcl.keyFenXingList[i];
        bi = this->__find_bi(fx);
        switch (bi.get_type()) {
        case BiType::UP:
        case BiType::DOWN:
            debug_bi(bi);
            this->biList.push_back(bi);
            break;
        case BiType::TEMP_DOWN:
        case BiType::TEMP_UP:
            this->biList.push_back(bi);
            break;
        case BiType::FAILURE_TEMP_DOWN:
        case BiType::FAILURE_TEMP_UP:
            if (!this->biList.empty())
                this->biList.pop_back();
            break;
        }
    }
    //this->biList = pre_bi_process(this->biList);
    //this->biList = this->__remove_baohan_bi();
}

Bi BiChuLi::find_new_bi(FenXing stop_fx) {
    Bi last_bi = Bi();
    Bi bi = Bi();
    if (!this->keyBiList.empty()) {
        last_bi = this->keyBiList.back();
        if (!this->keyBiList.empty()) {
            this->keyBiList.pop_back();
            bi = Bi(last_bi.get_start_fx(), stop_fx);
            this->keyBiList.push_back(bi);
            if (bi.get_type() == BiType::UP)
                bi.set_type(BiType::NEW_UP);
            else
                bi.set_type(BiType::NEW_DOWN);
            this->last_fx = stop_fx;
        }
    }
    return(bi);
}


Bi BiChuLi::__find_bi(FenXing fx) {
    FenXingType last_fx_type = this->last_fx.get_type();
    Bi bi = Bi();
    Bi last_bi;

    if (last_fx_type == FenXingType::NONE) {
        if (fx.get_type() == FenXingType::VERIFY_TOP || fx.get_type() == FenXingType::VERIFY_BOTTOM)
            this->last_fx = fx;
    } else {
        switch (fx.get_type()) {
            case FenXingType::VERIFY_BOTTOM:
                if (last_fx_type == FenXingType::VERIFY_BOTTOM) {
                    if (fx.get_low() < this->last_fx.get_low())
                        bi = this->find_new_bi(fx);
                }
                else {
                    bi = Bi(this->last_fx, fx);
                    this->keyBiList.push_back(bi);
                    this->last_fx = fx;
                }
                break;

            case FenXingType::VERIFY_TOP:
                if (last_fx_type == FenXingType::VERIFY_TOP) {
                    if (fx.get_high() <= this->last_fx.get_high()) {
                        bi = this->find_new_bi(fx);
                    }
                }
                else {
                    bi = Bi(this->last_fx, fx);
                    this->last_fx = fx;
                    this->keyBiList.push_back(bi);
                }
                break;

            case FenXingType::FAILURE_VERIFY_BOTTOM:
                if (!this->keyBiList.empty()) {
                    this->keyBiList.pop_back();
                    if (!this->keyBiList.empty()) {
                        this->last_bi = this->keyBiList.back();
                        if (this->last_bi.get_type() != BiType::NONE) {
                            this->last_fx = this->last_bi.get_stop_fx();
                        }
                    }
                    else {
                        this->last_bi = Bi();
                        this->last_fx = FenXing();
                    }
                }
                else {
                    this->last_bi = Bi();
                    this->last_fx = FenXing();
                }
                bi.set_type(BiType::FAILURE_DOWN);
                return(bi);
                break;

            case FenXingType::FAILURE_VERIFY_TOP:
                if (!this->keyBiList.empty()) {
                    this->keyBiList.pop_back();
                    if (!this->keyBiList.empty()) {
                        this->last_bi = this->keyBiList.back();
                        if (this->last_bi.get_type() != BiType::NONE) {
                            this->last_fx = this->last_bi.get_stop_fx();
                        }
                    }
                    else {
                        this->last_bi = Bi();
                        this->last_fx = FenXing();
                    }
                }
                else {
                    this->last_bi = Bi();
                    this->last_fx = FenXing();
                }
                bi.set_type(BiType::FAILURE_UP);
                return(bi);
                break;

            case FenXingType::BOTTOM:
                if (last_fx_type == FenXingType::VERIFY_TOP) {
                    bi = Bi(this->last_fx, fx);
                    bi.set_type(BiType::TEMP_DOWN);
                    return(bi);
                }
                break;

            case FenXingType::TOP:
                if (last_fx_type == FenXingType::VERIFY_BOTTOM) {
                    bi = Bi(this->last_fx, fx);
                    bi.set_type(BiType::TEMP_UP);
                }
                break;

            case FenXingType::FAILURE_BOTTOM:
                if (last_fx_type == FenXingType::VERIFY_TOP) {
                    bi = Bi(this->last_fx, fx);
                    bi.set_type(BiType::FAILURE_TEMP_DOWN);
                }
                break;

            case FenXingType::FAILURE_TOP:
                if (last_fx_type == FenXingType::VERIFY_BOTTOM) {
                    bi = Bi(this->last_fx, fx);
                    bi.set_type(BiType::FAILURE_TEMP_UP);
                }
                break;
            }
        }
    return(bi);
}


void Bi3_bi(int nCount, float* pOut, float* pHigh, float* pLow, float* pIn) {
    Bi bi;
    int start_pos, stop_pos;

    BaoHanChuLi baohanChuli;
    for (int i = 0; i < nCount; i++) {
        OutputDebugPrintf(" %d %f %f ", i, pHigh[i], pLow[i]);
        baohanChuli.add(pHigh[i], pLow[i]);
    }

    BiChuLi bichuli = BiChuLi();
    bichuli.handle(baohanChuli.kxianList);

    for (int i = 0; i < nCount; i++) {
        pOut[i] = 0;
    }

    unsigned int count = bichuli.biList.size();
    if (count > 0) {
        bi = bichuli.biList[0];
        start_pos = bi.get_start_pos();
        stop_pos = bi.get_stop_pos();
        if (bi.get_type() == BiType::UP) {
            pOut[start_pos] = -3;
            pOut[stop_pos] = 3;
        }
        else {
            pOut[start_pos] = 3;
            pOut[stop_pos] = -3;
        }


        for (unsigned int i = 1; i < count; i++) {
            bi = bichuli.biList[i];
            stop_pos = bi.get_stop_pos();
            switch (bi.get_type()) {
            case BiType::UP:
                pOut[stop_pos] = 3;
                break;
            case BiType::DOWN:
                pOut[stop_pos] = -3;
                break;
            case BiType::FAILURE_UP:
                pOut[stop_pos] = 4;
                break;
            case BiType::FAILURE_DOWN:
                pOut[stop_pos] = -4;
                break;
            case BiType::TEMP_DOWN:
                pOut[stop_pos] = -1;
                break;
            case BiType::FAILURE_TEMP_DOWN:
                pOut[stop_pos] = -2;
                break;
            case BiType::TEMP_UP:
                pOut[stop_pos] = 1;
                break;
            case BiType::FAILURE_TEMP_UP:
                pOut[stop_pos] = 2;
                break;
            }
        }
    }
}

void Bi4_bi(int nCount, float* pOut, float* pHigh, float* pLow, float* pIn) {
    BaoHanChuLi baohanChuli;
    for (int i = 0; i < nCount; i++) {
        baohanChuli.add(pHigh[i], pLow[i]);
    }

    BiChuLi bichuli;
    bichuli.handle(baohanChuli.kxianList);

    for (int i = 0; i < nCount; i++) {
        pOut[i] = 0;
    }

    unsigned int count = bichuli.biList.size();
    for (int i = count - 1; i >= 0; i--) {
        Bi bi = bichuli.biList[i];
        int start_pos = bi.get_stop_fx().get_stop_position();
        int stop_pos = bi.get_stop_fx().get_verify_stop_position();
        switch (bi.get_type()) {
        case BiType::UP:
            pOut[stop_pos] = 3;
            break;
        case BiType::DOWN:
            pOut[stop_pos] = -3;
            break;
        case BiType::FAILURE_UP:
            pOut[stop_pos] = 4;
            break;
        case BiType::FAILURE_DOWN:
            pOut[stop_pos] = -4;
            break;
        case BiType::TEMP_DOWN:
            pOut[stop_pos] = -1;
            break;
        case BiType::FAILURE_TEMP_DOWN:
            pOut[stop_pos] = -2;
            break;
        case BiType::TEMP_UP:
            pOut[stop_pos] = 1;
            break;
        case BiType::FAILURE_TEMP_UP:
            pOut[stop_pos] = 2;
            break;
        }
    }
}
