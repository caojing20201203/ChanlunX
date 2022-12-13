#include "CompositeBi.h"

CompositeBiChuLi::CompositeBiChuLi(){
    this->bicl = BiChuLi();
    this->status = CompositeBiChuLiStatus::START;
    this->start = Bi();
    this->left = Bi();
    this->after_left = Bi();
    this->middle = Bi();
    this->after_middle = Bi();
    this->right = Bi();
    this->bi_count = 0;
}

void CompositeBiChuLi::CompositeBiChuLi::handle(vector<Kxian1>& kxianList, bool IncludeTempBi){
    FindCompositeBiReturn ret_fd;
    CompositeBi com_bi = CompositeBi();
    Bi bi = Bi();
    vector<Bi> bi_list;
    this->bicl.handle(kxianList);
    bi_list = this->bicl.biList;
    int count = bi_list.size();

    for (int i = 0; i < count; i++){
        bi = bi_list[i];
        ret_fd = this->__find_CompositeBi(bi);
        switch(ret_fd.type){
            case FindCompositeBiReturnType::One:
                this->composite_bi_list.push_back(ret_fd.csBi1);
                break;
            case FindCompositeBiReturnType::Two:
                this->composite_bi_list.push_back(ret_fd.csBi1);
                this->composite_bi_list.push_back(ret_fd.csBi2);
                break;
            case FindCompositeBiReturnType::Three:
                this->composite_bi_list.push_back(ret_fd.csBi1);
                this->composite_bi_list.push_back(ret_fd.csBi2);
                this->composite_bi_list.push_back(ret_fd.csBi3);
                break;
            case FindCompositeBiReturnType::None:
                break;
        }
    }
    if (IncludeTempBi == true) {
        switch (this->status) {
        case CompositeBiChuLiStatus::START:
            break;
        case CompositeBiChuLiStatus::LEFT:
            ret_fd.type = FindCompositeBiReturnType::One;
            ret_fd.csBi1 = CompositeBi(this->start, this->start);
            if (this->start.get_type() == BiType::UP)
                ret_fd.csBi1.set_type(CompositeBiType::TEMP_UP);
            else
                ret_fd.csBi1.set_type(CompositeBiType::TEMP_DOWN);
            this->composite_bi_list.push_back(ret_fd.csBi1);
            break;
        case CompositeBiChuLiStatus::AFTER_LEFT:
            ret_fd.type = FindCompositeBiReturnType::Two;
            ret_fd.csBi1 = CompositeBi(this->start, this->start);
            ret_fd.csBi2 = CompositeBi(this->left, this->left);
            if (this->start.get_type() == BiType::UP) {
                ret_fd.csBi1.set_type(CompositeBiType::TEMP_UP);
                ret_fd.csBi2.set_type(CompositeBiType::TEMP_DOWN);
            }
            else {
                ret_fd.csBi1.set_type(CompositeBiType::TEMP_DOWN);
                ret_fd.csBi2.set_type(CompositeBiType::TEMP_UP);
            }
            this->composite_bi_list.push_back(ret_fd.csBi1);
            this->composite_bi_list.push_back(ret_fd.csBi2);
            break;
        case CompositeBiChuLiStatus::AFTER_LEFT_HIGHLOW:
        case CompositeBiChuLiStatus::AFTER_LEFT_NORMAL:
            ret_fd.type = FindCompositeBiReturnType::Three;
            ret_fd.csBi1 = CompositeBi(this->start, this->start);
            ret_fd.csBi2 = CompositeBi(this->left, this->left);
            ret_fd.csBi3 = CompositeBi(this->after_left, this->after_left);
            if (this->start.get_type() == BiType::UP) {
                ret_fd.csBi1.set_type(CompositeBiType::TEMP_UP);
                ret_fd.csBi2.set_type(CompositeBiType::TEMP_DOWN);
                ret_fd.csBi3.set_type(CompositeBiType::TEMP_UP);
            }
            else {
                ret_fd.csBi1.set_type(CompositeBiType::TEMP_DOWN);
                ret_fd.csBi2.set_type(CompositeBiType::TEMP_UP);
                ret_fd.csBi3.set_type(CompositeBiType::TEMP_DOWN);
            }
            this->composite_bi_list.push_back(ret_fd.csBi1);
            this->composite_bi_list.push_back(ret_fd.csBi2);
            this->composite_bi_list.push_back(ret_fd.csBi3);
            break;
        }
    }
}

char* getCompositebiChuLiStatus(CompositeBiChuLiStatus status) {
    switch (status) {
    case CompositeBiChuLiStatus::START:
        return("START");
    case CompositeBiChuLiStatus::LEFT:
        return("LEFT");
    case CompositeBiChuLiStatus::AFTER_LEFT:
        return("AFTER_LEFT");
    case CompositeBiChuLiStatus::AFTER_LEFT_HIGHLOW:
        return("AFTER_LEFT_HIGHLOW");
    case CompositeBiChuLiStatus::AFTER_LEFT_NORMAL:
        return("AFTER_LEFT_NORMAL");
    case CompositeBiChuLiStatus::AFTER_LEFT_NORMAL_NORMAL:
        return("AFTER_LEFT_NORMAL_NORMAL");

    }
}


FindCompositeBiReturn CompositeBiChuLi::__find_CompositeBi(Bi bi){
    BiType bi_type = bi.get_type();
    FindCompositeBiReturn ret_comp_bi = FindCompositeBiReturn();
    float bi_high = bi.get_high();
    float bi_low = bi.get_low();
    float bi_length = bi.get_length();
    CompositeBi last_comp_bi, before_last_comp_bi;

    if (bi_type == BiType::UP) {
        OutputDebugPrintf("上升组合笔 %f %f %d %d %f %s ", bi.get_low(), bi.get_high(), bi.get_start_pos(), bi.get_stop_pos(), bi.get_length(), getCompositebiChuLiStatus(this->status));
    }
    else {
        OutputDebugPrintf("下降组合笔 %f %f %d %d %f %s ", bi.get_high(), bi.get_low(), bi.get_start_pos(), bi.get_stop_pos(), bi.get_length(), getCompositebiChuLiStatus(this->status));
    }

    switch(this->status){
        case CompositeBiChuLiStatus::START:
            if (!this->composite_bi_list.empty()) {
                int count = this->composite_bi_list.size();
                last_comp_bi = this->composite_bi_list.back();
                if ((bi_type == BiType::UP && bi_high > last_comp_bi.get_high()) ||
                    (bi_type == BiType::DOWN && bi_low < last_comp_bi.get_low())) {
                    if (count >= 2) {
                        before_last_comp_bi = this->composite_bi_list[count - 2];
                        if ((bi_type == BiType::UP && bi_high > before_last_comp_bi.get_high() && bi_low > before_last_comp_bi.get_low()) ||
                            (bi_type == BiType::DOWN && bi_low < before_last_comp_bi.get_low() && bi_high < before_last_comp_bi.get_high())) {
                            this->composite_bi_list.pop_back();
                            this->composite_bi_list.pop_back();
                            this->bi_count = before_last_comp_bi.get_count() + last_comp_bi.get_count() + 1;
                            ret_comp_bi.type = FindCompositeBiReturnType::One;
                            ret_comp_bi.csBi1 = CompositeBi(before_last_comp_bi.get_start_bi(), bi);
                            ret_comp_bi.csBi1.set_count(this->bi_count);
                            status = CompositeBiChuLiStatus::START;
                            return(ret_comp_bi);
                        }
                    }
                }
            }
            this->start = bi;
            this->status = CompositeBiChuLiStatus::LEFT;
            this->bi_count = 1;
            break;
        case CompositeBiChuLiStatus::LEFT:
            if ((bi_type == BiType::UP && bi_high > this->start.get_high()) || (bi_type == BiType::DOWN && bi_low < this->start.get_low())){
                if (!this->composite_bi_list.empty()) {
                    last_comp_bi = this->composite_bi_list.back();
                    if ((this->start.get_type() == BiType::UP && last_comp_bi.get_high() > this->start.get_high())
                        || (this->start.get_type() == BiType::DOWN && last_comp_bi.get_low() < this->start.get_low())) {
                        this->composite_bi_list.pop_back();
                        ret_comp_bi.type = FindCompositeBiReturnType::One;
                        ret_comp_bi.csBi1 = CompositeBi(last_comp_bi.get_start_bi(), bi);
                        ret_comp_bi.csBi1.set_kind(CompositeBiKide::FiveBi);
                        this->status = CompositeBiChuLiStatus::START;
                    }
                    else {
                        ret_comp_bi.type = FindCompositeBiReturnType::One;
                        ret_comp_bi.csBi1 = CompositeBi(this->start, this->start);
                        ret_comp_bi.csBi1.set_kind(CompositeBiKide::SignlBi);
                        this->start = bi;
                        this->status = CompositeBiChuLiStatus::LEFT;
                    }
                }
                else {
                    ret_comp_bi.type = FindCompositeBiReturnType::One;
                    ret_comp_bi.csBi1 = CompositeBi(this->start, this->start);
                    ret_comp_bi.csBi1.set_kind(CompositeBiKide::SignlBi);
                    this->start = bi;
                    this->status = CompositeBiChuLiStatus::LEFT;
                }
            } else {
                this->left = bi;
                this->status = CompositeBiChuLiStatus::AFTER_LEFT;
            }
            break;
        case CompositeBiChuLiStatus::AFTER_LEFT:
            switch(bi_type){
                case BiType::UP:
                    if (bi_high > this->start.get_high()) {
                        ret_comp_bi.type = FindCompositeBiReturnType::One;
                        ret_comp_bi.csBi1 = CompositeBi(this->start, bi);
                        ret_comp_bi.csBi1.set_kind(CompositeBiKide::ThreeBi);
                        this->status = CompositeBiChuLiStatus::START;
                    } else {
                        this->after_left = bi;
                        this->status = CompositeBiChuLiStatus::AFTER_LEFT_NORMAL;
                    }
                    break;
                case BiType::DOWN:
                    if (bi_low < this->start.get_low()) {
                        ret_comp_bi.type = FindCompositeBiReturnType::One;
                        ret_comp_bi.csBi1 = CompositeBi(this->start, bi);
                        ret_comp_bi.csBi1.set_kind(CompositeBiKide::ThreeBi);
                        this->status = CompositeBiChuLiStatus::START;
                    } else {
                        this->after_left = bi;
                        this->status = CompositeBiChuLiStatus::AFTER_LEFT_NORMAL;
                    }
                    break;
            }
            break;

        case CompositeBiChuLiStatus::AFTER_LEFT_NORMAL:
            switch(bi_type){
                case BiType::UP:
                    if (bi_high > this->start.get_high()) {
                        if (this->start.get_type() == BiType::DOWN) {
                            if (!this->composite_bi_list.empty()) {
                                last_comp_bi = this->composite_bi_list.back();
                                if (last_comp_bi.get_low() < this->start.get_low()) {
                                    this->composite_bi_list.pop_back();
                                    ret_comp_bi.type = FindCompositeBiReturnType::One;
                                    ret_comp_bi.csBi1 = CompositeBi(last_comp_bi.get_start_bi(), bi);
                                    ret_comp_bi.csBi1.set_kind(CompositeBiKide::FiveBi);
                                    this->status = CompositeBiChuLiStatus::START;
                                }
                                else {
                                    ret_comp_bi.type = FindCompositeBiReturnType::Two;
                                    ret_comp_bi.csBi1 = CompositeBi(this->start, this->start);
                                    ret_comp_bi.csBi1.set_kind(CompositeBiKide::SignlBi);
                                    ret_comp_bi.csBi2 = CompositeBi(this->left, bi);
                                    ret_comp_bi.csBi2.set_kind(CompositeBiKide::ThreeBi);
                                    this->status = CompositeBiChuLiStatus::START;
                                }
                            }
                            else {
                                ret_comp_bi.type = FindCompositeBiReturnType::One;
                                ret_comp_bi.csBi1 = CompositeBi(this->start, this->start);
                                ret_comp_bi.csBi1.set_kind(CompositeBiKide::SignlBi);
                                this->start = bi.generate_bi(this->left, this->after_left, bi);
                                this->status = CompositeBiChuLiStatus::LEFT;
                            }
                        }
                        else {
                            ret_comp_bi.type = FindCompositeBiReturnType::One;
                            ret_comp_bi.csBi1 = CompositeBi(this->start, this->start);
                            ret_comp_bi.csBi1.set_kind(CompositeBiKide::SignlBi);
                            this->start = bi.generate_bi(this->left, this->after_left, bi);
                            this->status = CompositeBiChuLiStatus::LEFT;
                        }
                    }
                    else {
                        if (bi_high > this->left.get_high()) {
                            this->status = CompositeBiChuLiStatus::START;
                            ret_comp_bi.type = FindCompositeBiReturnType::Two;
                            ret_comp_bi.csBi1 = CompositeBi(this->start, this->start);
                            ret_comp_bi.csBi1.set_kind(CompositeBiKide::SignlBi);
                            ret_comp_bi.csBi2 = CompositeBi(this->left, bi);
                            ret_comp_bi.csBi2.set_kind(CompositeBiKide::ThreeBi);
                        }
                        else {
                            this->middle = bi;
                            this->status = CompositeBiChuLiStatus::AFTER_LEFT_NORMAL_NORMAL;
                        }

                    }
                    break;
                case BiType::DOWN:
                        if (bi_low < this->start.get_low()){
                            if (this->start.get_type() == BiType::UP) {
                                if (!this->composite_bi_list.empty()) {
                                    last_comp_bi = this->composite_bi_list.back();
                                    if (last_comp_bi.get_high() > this->start.get_high()) {
                                        this->composite_bi_list.pop_back();
                                        ret_comp_bi.type = FindCompositeBiReturnType::One;
                                        ret_comp_bi.csBi1 = CompositeBi(last_comp_bi.get_start_bi(), bi);
                                        ret_comp_bi.csBi1.set_kind(CompositeBiKide::FiveBi);
                                        this->status = CompositeBiChuLiStatus::START;
                                    }
                                    else {
                                        ret_comp_bi.type = FindCompositeBiReturnType::Two;
                                        ret_comp_bi.csBi1 = CompositeBi(this->start, this->start);
                                        ret_comp_bi.csBi1.set_kind(CompositeBiKide::SignlBi);
                                        ret_comp_bi.csBi2 = CompositeBi(this->left, bi);
                                        ret_comp_bi.csBi2.set_kind(CompositeBiKide::ThreeBi);
                                        this->status = CompositeBiChuLiStatus::START;
                                    }
                                }
                                else {
                                    ret_comp_bi.type = FindCompositeBiReturnType::One;
                                    ret_comp_bi.csBi1 = CompositeBi(this->start, this->start);
                                    ret_comp_bi.csBi1.set_kind(CompositeBiKide::SignlBi);
                                    this->start = bi.generate_bi(this->left, this->after_left, bi);
                                    this->status = CompositeBiChuLiStatus::LEFT;
                                }
                            }
                            else {
                                ret_comp_bi.type = FindCompositeBiReturnType::One;
                                ret_comp_bi.csBi1 = CompositeBi(this->start, this->start);
                                ret_comp_bi.csBi1.set_kind(CompositeBiKide::SignlBi);
                                this->start = bi.generate_bi(this->left, this->after_left, bi);
                                this->status = CompositeBiChuLiStatus::LEFT;
                            }
                        } else {
                            if (bi_low < this->left.get_low()){
                                this->status = CompositeBiChuLiStatus::START;
                                ret_comp_bi.type = FindCompositeBiReturnType::Two;
                                ret_comp_bi.csBi1 = CompositeBi(this->start, this->start);
                                ret_comp_bi.csBi1.set_kind(CompositeBiKide::SignlBi);
                                ret_comp_bi.csBi2 = CompositeBi(this->left, bi);
                                ret_comp_bi.csBi2.set_kind(CompositeBiKide::ThreeBi);
                             }
                            else {
                                this->middle = bi;
                                this->status = CompositeBiChuLiStatus::AFTER_LEFT_NORMAL_NORMAL;
                            }
                        }
                    break;
            }
            break;

        case CompositeBiChuLiStatus::AFTER_LEFT_NORMAL_NORMAL:
            switch (bi_type) {
            case BiType::UP:
                if (this->start.get_type() == BiType::UP) {
                    if (bi_high > this->start.get_high()) {
                        ret_comp_bi.type = FindCompositeBiReturnType::One;
                        ret_comp_bi.csBi1 = CompositeBi(this->start, bi);
                        ret_comp_bi.csBi1.set_kind(CompositeBiKide::FiveBi);
                        this->status = CompositeBiChuLiStatus::START;
                    }
                    else {
                        if (bi_high > this->after_left.get_high()) {
                            this->after_left = bi.generate_bi(this->after_left, this->middle, bi);
                            this->status = CompositeBiChuLiStatus::AFTER_LEFT_NORMAL;
                        }
                    }
                }
                else {
                    if (bi_high > this->start.get_high()) {
                        ret_comp_bi.type = FindCompositeBiReturnType::Two;
                        ret_comp_bi.csBi1 = CompositeBi(this->start, this->start);
                        ret_comp_bi.csBi1.set_kind(CompositeBiKide::SignlBi);
                        ret_comp_bi.csBi2 = CompositeBi(this->left, bi);
                        ret_comp_bi.csBi2.set_kind(CompositeBiKide::FiveBi);
                        this->status = CompositeBiChuLiStatus::START;
                    }
                    else {
                        if (bi_high > this->left.get_high()) {
                            ret_comp_bi.type = FindCompositeBiReturnType::Two;
                            ret_comp_bi.csBi1 = CompositeBi(this->start, this->start);
                            ret_comp_bi.csBi2 = CompositeBi(this->left, bi);
                            this->status = CompositeBiChuLiStatus::START;
                        }
                    }
                }
                break;
            case BiType::DOWN:
                if (this->start.get_type() == BiType::DOWN) {
                    if (bi_low < this->start.get_low()) {
                        ret_comp_bi.type = FindCompositeBiReturnType::One;
                        ret_comp_bi.csBi1 = CompositeBi(this->start, bi);
                        ret_comp_bi.csBi1.set_kind(CompositeBiKide::FiveBi);
                        this->status = CompositeBiChuLiStatus::START;
                    }
                    else {
                        if (bi_low < this->left.get_low()) {
                            this->left = bi.generate_bi(this->left, Bi(), bi);
                            this->status = CompositeBiChuLiStatus::AFTER_LEFT;
                        }
                    }
                }
                else {
                    if (bi_low < this->start.get_low()) {
                        ret_comp_bi.type = FindCompositeBiReturnType::Two;
                        ret_comp_bi.csBi1 = CompositeBi(this->start, this->start);
                        ret_comp_bi.csBi1.set_kind(CompositeBiKide::SignlBi);
                        ret_comp_bi.csBi2 = CompositeBi(this->left, bi);
                        ret_comp_bi.csBi2.set_kind(CompositeBiKide::FiveBi);
                        this->status = CompositeBiChuLiStatus::START;
                    }
                    else {
                        if (bi_low < this->left.get_low()) {
                            ret_comp_bi.type = FindCompositeBiReturnType::Two;
                            ret_comp_bi.csBi1 = CompositeBi(this->start, this->start);
                            ret_comp_bi.csBi2 = CompositeBi(this->left, bi);
                            this->status = CompositeBiChuLiStatus::START;
                        }
                    }
                }
                break;
            }
            break;
    }
    return(ret_comp_bi);
}

void Bi3_CompositeBi(int nCount, float *pOut, float *pHigh, float *pLow, float *pIn) {
    BaoHanChuLi baoHanChuLi;
    CompositeBiChuLi composite_bi_chuli;
    CompositeBi composite_bi;
    Bi bi;
    int start_pos, stop_pos;
    int temp_start_num = 0;

    for (int i = 0; i < nCount; i++){
        baoHanChuLi.add(pHigh[i], pLow[i]);
    }

    for (int i = 0; i < nCount; i++) {
        pOut[i] = 0;
    }

    composite_bi_chuli.handle(baoHanChuLi.kxianList, true);
    int count = composite_bi_chuli.composite_bi_list.size();
    if (!composite_bi_chuli.composite_bi_list.empty()){
        composite_bi = composite_bi_chuli.composite_bi_list[0];
        start_pos = composite_bi.get_start_pos();
        if (composite_bi.get_type() == CompositeBiType::UP)
            pOut[start_pos] = -3;
        else
            pOut[start_pos] = 3;

        for (int i = 0; i < count; i++) {
            composite_bi = composite_bi_chuli.composite_bi_list[i];
            stop_pos = composite_bi.get_stop_pos();
            switch(composite_bi.get_type()){
                case CompositeBiType::UP:
                    pOut[stop_pos] = 3;
                    break;
                case CompositeBiType::DOWN:
                    pOut[stop_pos] = -3;
                    break;
                case CompositeBiType::TEMP_DOWN:
                    if (temp_start_num == 0) {
                        temp_start_num = 60;
                        pOut[stop_pos] = temp_start_num;
                    }
                    else {
                        temp_start_num++;
                        pOut[stop_pos] = temp_start_num;
                    }
                    break;
                case CompositeBiType::TEMP_UP:
                    if (temp_start_num == 0) {
                        temp_start_num = 30;
                        pOut[stop_pos] = temp_start_num;
                    }
                    else {
                        temp_start_num++;
                        pOut[stop_pos] = temp_start_num;
                    }
                    break;
            }
        }
    }
}

void Bi4_CompositeBi(int nCount, float* pOut, float* pHigh, float* pLow, float* pIn) {
    BaoHanChuLi baoHanChuLi;
    CompositeBiChuLi composite_bi_chuli;
    CompositeBi composite_bi;
    Bi bi;
    int start_pos, stop_pos;
    int temp_start_num = 0;

    for (int i = 0; i < nCount; i++) {
        baoHanChuLi.add(pHigh[i], pLow[i]);
    }

    for (int i = 0; i < nCount; i++) {
        pOut[i] = 0;
    }

    composite_bi_chuli.handle(baoHanChuLi.kxianList, true);
    int count = composite_bi_chuli.composite_bi_list.size();
    if (!composite_bi_chuli.composite_bi_list.empty()) {
        composite_bi = composite_bi_chuli.composite_bi_list[0];
        start_pos = composite_bi.get_start_pos();
        if (composite_bi.get_type() == CompositeBiType::UP)
            pOut[start_pos] = -3;
        else
            pOut[start_pos] = 3;

        for (int i = 0; i < count; i++) {
            composite_bi = composite_bi_chuli.composite_bi_list[i];
            stop_pos = composite_bi.get_stop_verify_pos();
            switch (composite_bi.get_type()) {
            case CompositeBiType::UP:
                pOut[stop_pos] = 3;
                break;
            case CompositeBiType::DOWN:
                pOut[stop_pos] = -3;
                break;
            case CompositeBiType::TEMP_DOWN:
                if (temp_start_num == 0) {
                    temp_start_num = 60;
                    pOut[stop_pos] = temp_start_num;
                }
                else {
                    temp_start_num++;
                    pOut[stop_pos] = temp_start_num;
                }
                break;
            case CompositeBiType::TEMP_UP:
                if (temp_start_num == 0) {
                    temp_start_num = 30;
                    pOut[stop_pos] = temp_start_num;
                }
                else {
                    temp_start_num++;
                    pOut[stop_pos] = temp_start_num;
                }
                break;
            }
        }
    }
}

void Bi3_Composite_GuiDaoGao(int nCount, float* pOut, float* pHigh, float* pLow, float* pIn) {
    BaoHanChuLi baoHanChuLi;
    CompositeBiChuLi composite_bi_chuli;
    CompositeBi composite_bi;
    Bi bi;
    int start_pos, stop_pos;
    int temp_start_num = 0;
    float gao;

    for (int i = 0; i < nCount; i++) {
        baoHanChuLi.add(pHigh[i], pLow[i]);
    }

    for (int i = 0; i < nCount; i++) {
        pOut[i] = 0;
    }

    composite_bi_chuli.handle(baoHanChuLi.kxianList, false);
    int count = composite_bi_chuli.composite_bi_list.size();
    if (!composite_bi_chuli.composite_bi_list.empty()) {
        for (int i = 0; i < count; i++) {
            composite_bi = composite_bi_chuli.composite_bi_list[i];
            gao = composite_bi.get_high();
            start_pos = composite_bi.get_start_pos();
            stop_pos = composite_bi.get_stop_pos();
            for (int j = start_pos; j <= stop_pos; j++) {
                pOut[j] = gao;
            }
        }
        for (int i = stop_pos; i < nCount; i++)
            pOut[i] = gao;
    }
}


void Bi3_Composite_GuiDaoDi(int nCount, float* pOut, float* pHigh, float* pLow, float* pIn) {
    BaoHanChuLi baoHanChuLi;
    CompositeBiChuLi composite_bi_chuli;
    CompositeBi composite_bi;
    Bi bi;
    int start_pos, stop_pos;
    int temp_start_num = 0;
    float di;

    for (int i = 0; i < nCount; i++) {
        baoHanChuLi.add(pHigh[i], pLow[i]);
    }

    for (int i = 0; i < nCount; i++) {
        pOut[i] = 0;
    }

    composite_bi_chuli.handle(baoHanChuLi.kxianList, false);
    int count = composite_bi_chuli.composite_bi_list.size();
    if (!composite_bi_chuli.composite_bi_list.empty()) {
        for (int i = 0; i < count; i++) {
            composite_bi = composite_bi_chuli.composite_bi_list[i];
            di = composite_bi.get_low();
            start_pos = composite_bi.get_start_pos();
            stop_pos = composite_bi.get_stop_pos();
            for (int j = start_pos; j <= stop_pos; j++) {
                pOut[j] = di;
            }
        }
        for (int i = stop_pos; i < nCount; i++) {
            pOut[i] = di;
        }
    }
}


void Bi3_Composite_GuiDaoZhong(int nCount, float* pOut, float* pHigh, float* pLow, float* pIn) {
    BaoHanChuLi baoHanChuLi;
    CompositeBiChuLi composite_bi_chuli;
    CompositeBi composite_bi;
    Bi bi;
    int start_pos, stop_pos;
    int temp_start_num = 0;
    float gao, di, zhong;

    for (int i = 0; i < nCount; i++) {
        baoHanChuLi.add(pHigh[i], pLow[i]);
    }

    for (int i = 0; i < nCount; i++) {
        pOut[i] = 0;
    }

    composite_bi_chuli.handle(baoHanChuLi.kxianList, false);
    int count = composite_bi_chuli.composite_bi_list.size();
    if (!composite_bi_chuli.composite_bi_list.empty()) {
        composite_bi = composite_bi_chuli.composite_bi_list[0];
        for (int i = 0; i < count; i++) {
            composite_bi = composite_bi_chuli.composite_bi_list[i];
            gao = composite_bi.get_high();
            di = composite_bi.get_low();
            zhong = (gao + di) / 2;
            start_pos = composite_bi.get_start_pos();
            stop_pos = composite_bi.get_stop_pos();
            for (int j = start_pos; j <= stop_pos; j++) {
                if (zhong > 0)
                    pOut[j] = zhong;
            }
        }
        for (int i = stop_pos; i < nCount; i++)
            pOut[i] = zhong;
    }
}
