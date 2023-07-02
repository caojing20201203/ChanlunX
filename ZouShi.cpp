#include "ZouShi.h"
#include "ZhongShu.h"

using namespace std;

 ZouShi ZouShiChuLi::get_last_zoushi() {
    if (!this->zoushi_list.empty())
        return(this->zoushi_list.back());
    return(ZouShi());
 }

 CompositeBi ZouShiChuLi::generate_bi(CompositeBi bi1, CompositeBi bi3) {
    CompositeBi ret_bi = CompositeBi();
    Bi start_bi, stop_bi;
    start_bi = bi1.get_start_bi();
    stop_bi = bi3.get_stop_bi();
    ret_bi = CompositeBi(start_bi, stop_bi);
    return(ret_bi);
}

bool ZouShiChuLi::match_zhongshu_xianduan(CompositeBi bi1, CompositeBi bi2) {
    float bilv = 0;

    if (bi1.get_length() > bi2.get_length()) {
        bilv = bi1.get_length() / bi2.get_length();
    } else {
        bilv = bi2.get_length() / bi1.get_length();
    }

    if (bilv > 0.618)
        return(true);
    else
        return(false);
}

ZouShiChuLi::ZouShiChuLi(){
    this->status = ZouShiChuLiStatus::a0;
    this->compbicl = CompositeBiChuLi();
}

void ZouShiChuLi::handle(vector<Kxian1>& kxianList) {
    FindZouShiReturn ret_zoushi;
    ZouShi zoushi = ZouShi();
    CompositeBi comp_bi = CompositeBi();
    this->compbicl.handle(kxianList, true);

    int count = this->compbicl.composite_bi_list.size();
    for (int i = 0; i < count; i++) {
        comp_bi = this->compbicl.composite_bi_list[i];
        ret_zoushi = this->find_zoushi(comp_bi);
    }
}

bool is_composite_bi_equal(CompositeBi bi1, CompositeBi bi2) {
    float radio;
    float bi1_len, bi2_len;

    bi1_len = bi1.get_length();
    bi2_len = bi2.get_length();
    if (bi1_len < bi2_len) {
        radio = bi1_len / bi2_len;
    } else {
        radio = bi2_len / bi1_len;
    }
    if (radio >= 0.618) {
        return(true);
    } else {
        return(false);
    }
}


CompositeBi ZouShiChuLi::normal_process(CompositeBi bi) {
    CompositeBi ret_bi = CompositeBi();

    switch(this->normal_bi.get_type()){
        case CompositeBiType::UP:
            if (bi.get_high() > this->normal_bi.get_high()){
                ret_bi = bi.generate_bi(this->normal_bi, bi);
            } else {
                if (bi.get_low() < this->normal_bi.get_low()){
                    if (this->normal_first_bi.get_type() == CompositeBiType::NONE) {
                        ret_bi = bi;
                    } else {
                        ret_bi = bi.generate_bi(this->normal_first_bi, bi);
                    }
                } else {
                    if (this->normal_first_bi.get_type() == CompositeBiType::NONE) {
                        this->normal_first_bi = bi;
                    }
                }
            }
            break;
        case CompositeBiType::DOWN:
            if (bi.get_low() < this->normal_bi.get_low()){
                ret_bi = bi.generate_bi(this->normal_bi, bi);
            } else {
                if (bi.get_high() > this->normal_bi.get_high()) {
                    if (this->normal_first_bi.get_type() == CompositeBiType::NONE) {
                        ret_bi = bi;
                    } else {
                        ret_bi = bi.generate_bi(this->normal_first_bi, bi);
                    }                    
                } else {
                    if (this->normal_first_bi.get_type() == CompositeBiType::NONE) {
                        this->normal_first_bi = bi;
                    }
                }
            }
            break;
    }
    return(ret_bi);
}

void ZouShiChuLi::last_zoushi_process(CompositeBi bi1, CompositeBi bi2){
    ZouShi last_zoushi;
    if (this->zoushi_list.empty()) {
        this->a_0 = bi2;
        this->status = ZouShiChuLiStatus::a1;
    }
    else {
        last_zoushi = this->zoushi_list.back();
        if (last_zoushi.get_type() == ZouShiType::UP && bi2.get_high() > last_zoushi.get_high()) {
            last_zoushi = ZouShi(ZouShiType::UP, last_zoushi.get_start_xd(), bi2);
            this->zoushi_list.pop_back();
            this->zoushi_list.push_back(last_zoushi);
            this->status = ZouShiChuLiStatus::a0;
        }
        else {
            if (last_zoushi.get_type() == ZouShiType::DOWN && bi2.get_low() < last_zoushi.get_low()) {
                last_zoushi = ZouShi(ZouShiType::UP, last_zoushi.get_start_xd(), bi2);
                this->zoushi_list.pop_back();
                this->zoushi_list.push_back(last_zoushi);
                this->status == ZouShiChuLiStatus::a0;
            }
            else {
                OutputDebugPrintf("last_zoushi未处理");
            }
        }
    }

}

FindZouShiReturn ZouShiChuLi::find_zoushi(CompositeBi comp_bi) {
    FindZouShiReturn ret_zoushi;
    ZouShi last_zoushi, zoushi;

    float bi_high = comp_bi.get_high();
    float bi_low = comp_bi.get_low();
    float bi_len = comp_bi.get_length();
    CompositeBiType bi_type = comp_bi.get_type();
    CompositeBi ret_bi;
    FindZhongShuReturn ret_zs_value;

    switch (this->status) {
    case ZouShiChuLiStatus::a0:
        //style 1(2)
        this->a_0 = comp_bi;
        this->status = ZouShiChuLiStatus::a1;
        break;

    case ZouShiChuLiStatus::a1:
        if (is_composite_bi_equal(this->a_0, comp_bi)) {
            this->a_1 = comp_bi;
            this->status = ZouShiChuLiStatus::a1_equal_a0;
        } else {
            switch(bi_type) {
                case CompositeBiType::UP:
                    if (bi_high > this->a_0.get_high()){
                        this->last_zoushi_process(this->a_0, comp_bi);
                        this->status = ZouShiChuLiStatus::a0;
                    } else {
                        this->a_1 = comp_bi;
                        this->status = ZouShiChuLiStatus::a2;
                    }
                    break;
                case CompositeBiType::DOWN:
                    if (bi_low < this->a_0.get_low()) {
                        this->last_zoushi_process(this->a_0, this->a_1);
                        this->status = ZouShiChuLiStatus::a0;
                    } else {
                        this->a_1 = comp_bi;
                        this->status = ZouShiChuLiStatus::a2;
                    }
                    break;
            }
        }
        break;

    case ZouShiChuLiStatus::a1_equal_a0:
        if (is_composite_bi_equal(this->a_1, comp_bi)) {
            this->a_2 = comp_bi;
            this->A = ZhongShu(CompositeBi(), this->a_0, this->a_1, comp_bi);
            this->status = ZouShiChuLiStatus::A;
        } else {
            switch (bi_type) {
            case CompositeBiType::UP:
                if (bi_high > this->a_0.get_high()){
                    if (this->a_0.get_low() < this->a_1.get_low()){
                        this->a_0 = comp_bi.generate_bi(this->a_0, comp_bi);
                        this->status = ZouShiChuLiStatus::a1;
                    } else {
                        this->last_zoushi_process(this->a_0, this->a_1);
                        this->a_0 = comp_bi;
                        this->status = ZouShiChuLiStatus::a1;
                    }
                } else {
                    this->a_2 = comp_bi;
                    this->status = ZouShiChuLiStatus::a1_equal_a0_normal;
                }
                break;
            case CompositeBiType::DOWN:
                if (bi_low < this->a_0.get_low()) {
                    if (this->a_0.get_high() > this->a_1.get_high()){
                        this->a_0 = comp_bi.generate_bi(this->a_0, comp_bi);
                        this->status = ZouShiChuLiStatus::a1;                        
                    } else {
                        this->last_zoushi_process(this->a_0, this->a_1);
                        this->a_0 = comp_bi;
                        this->status = ZouShiChuLiStatus::a1;                        
                    }
                } else {
                    this->normal_bi = comp_bi;
                    this->normal_first_bi = CompositeBi();
                    this->status = ZouShiChuLiStatus::a1_equal_a0_normal;
                }
                break;
            }
        }
        break;

    case ZouShiChuLiStatus::a1_equal_a0_normal:
        ret_bi = this->normal_process(comp_bi);
        bi_type = ret_bi.get_type();
        if (bi_type == this->normal_bi.get_type()) {
            this->a_2 = ret_bi;
            if (is_composite_bi_equal(this->a_2, this->a_1)){
                this->A = ZhongShu(CompositeBi(), this->a_0, this->a_1, this->a_2);
                this->status = ZouShiChuLiStatus::A;
            } else {
                switch(bi_type){
                    case CompositeBiType::UP:
                        if (this->a_2.get_high() > this->a_0.get_high()){
                            if (this->a_0.get_low() < this->a_1.get_low()){
                                this->a_0 = comp_bi.generate_bi(this->a_1, this->a_3);
                                this->status = ZouShiChuLiStatus::a1;
                            } else {
                                this->last_zoushi_process(this->a_0, this->a_1);
                                this->a_0 = this->a_2;
                                this->status = ZouShiChuLiStatus::a1;
                            }
                        } else {
                            this->status = ZouShiChuLiStatus::a1_equal_a0_normal;
                        }
                        break;
                    case CompositeBiType::DOWN:
                        if (this->a_2.get_low() < this->a_0.get_low()) {
                            if (this->a_0.get_high() > this->a_1.get_high()){
                                this->a_0 = comp_bi.generate_bi(this->a_0, this->a_2);
                                this->status = ZouShiChuLiStatus::a1;
                            } else {
                                this->last_zoushi_process(this->a_0, this->a_1);
                                this->a_0 = this->a_2;
                                this->status = ZouShiChuLiStatus::a1;
                            }
                         } else {
                            this->status = ZouShiChuLiStatus::a1_equal_a0_normal;
                         }
                        break;
                    }
                }
            } else {
                if (bi_type == CompositeBiType::NONE) {
                    this->status = ZouShiChuLiStatus::a1_equal_a0_normal;
                } else {
                    //返回两笔
                    this->a_1 = comp_bi.generate_bi(this->a_1, ret_bi);
                    if (is_composite_bi_equal(this->a_0, this->a_1)){
                        this->status = ZouShiChuLiStatus::a1_equal_a0;
                    } else {
                        switch(bi_type) {
                            case CompositeBiType::UP:
                                if (this->a_1.get_high() > this->a_0.get_high()){
                                    this->last_zoushi_process(this->a_0, this->a_1);
                                    this->status = ZouShiChuLiStatus::a0;
                                }else {
                                    this->status = ZouShiChuLiStatus::a2;
                                }
                                break;
                            case CompositeBiType::DOWN:
                                if (this->a_1.get_low() < this->a_0.get_low()) {
                                    this->last_zoushi_process(this->a_0, this->a_1);
                                    this->status = ZouShiChuLiStatus::a0;
                                } else {
                                    this->status = ZouShiChuLiStatus::a2;                            
                                }
                                break;
                            }
                    }
                }
            }
        break;
        
    case ZouShiChuLiStatus::a2:
        if (is_composite_bi_equal(this->a_1, comp_bi)) {
            this->a_2 = comp_bi;
            this->status = ZouShiChuLiStatus::a2_equal_a1;
        } else {
            switch(bi_type) {
                case CompositeBiType::UP:
                    if (bi_high > this->a_0.get_high()) {
                        this->a_0 = comp_bi.generate_bi(this->a_0, comp_bi);
                        this->status = ZouShiChuLiStatus::a1;
                    } else {
                        this->normal_bi = comp_bi;
                        this->normal_first_bi = CompositeBi();
                        this->status = ZouShiChuLiStatus::a2_normal;
                    }
                    break;
                case CompositeBiType::DOWN:
                    if (bi_low < this->a_0.get_low()) {
                        this->a_0 = comp_bi.generate_bi(this->a_0, comp_bi);
                        this->status = ZouShiChuLiStatus::a1;
                    } else {
                        this->normal_bi = comp_bi;
                        this->normal_first_bi = CompositeBi();
                        this->status = ZouShiChuLiStatus::a2_normal;
                    }
                    break;
            }
        }
        break;
    case ZouShiChuLiStatus::a2_normal:
            ret_bi = this->normal_process(comp_bi);
            bi_type = ret_bi.get_type();
            if (bi_type == this->normal_bi.get_type()){
                //返回1笔
                this->a_2 = comp_bi.generate_bi(this->a_2, comp_bi);
                if (is_composite_bi_equal(this->a_1, comp_bi)){
                    this->status = ZouShiChuLiStatus::a1_equal_a0;
                } else {
                    switch(bi_type) {
                        case CompositeBiType::UP:
                            if (this->a_2.get_high() > this->a_0.get_high()){
                                this->a_0 = comp_bi.generate_bi(this->a_0, this->a_2);
                                this->status = ZouShiChuLiStatus::a1;
                            } else {
                                this->status = ZouShiChuLiStatus::a2;
                            }
                            break;
                        case CompositeBiType::DOWN:
                            if (this->a_2.get_low() < this->a_0.get_low()){
                                this->a_0 = comp_bi.generate_bi(this->a_0, this->a_2);
                                this->status = ZouShiChuLiStatus::a1;
                            } else {
                                this->status = ZouShiChuLiStatus::a2;
                            }
                            break;
                    }
                }
            } else {
                if (bi_type == CompositeBiType::NONE) {
                    this->status = ZouShiChuLiStatus::a2_normal;
                } else {
                    //返回两笔
                    this->a_1 = comp_bi.generate_bi(this->a_1, ret_bi);
                    if (is_composite_bi_equal(this->a_0, this->a_1)) {
                        this->status = ZouShiChuLiStatus::a1_equal_a0;
                    } else {
                        switch(bi_type) {
                            case CompositeBiType::UP:
                                if (this->a_1.get_high() > this->a_0.get_high()){
                                    this->last_zoushi_process(this->a_0, this->a_1);
                                    this->status = ZouShiChuLiStatus::a0;
                                } else {
                                    this->status = ZouShiChuLiStatus::a2;
                                }
                                break;
                            case CompositeBiType::DOWN:
                                if (this->a_1.get_low() < this->a_0.get_low()){
                                    this->last_zoushi_process(this->a_0, this->a_1);
                                    this->status = ZouShiChuLiStatus::a0;
                                } else {
                                    this->status = ZouShiChuLiStatus::a2;
                                }
                                break;
                        }
                    }
                }
            }
            break;

    case ZouShiChuLiStatus::a2_equal_a1:
        if (is_composite_bi_equal(this->a_2, comp_bi)) {
            //中枢
            this->A = ZhongShu(this->a_0, this->a_1, this->a_2, comp_bi);
            this->status = ZouShiChuLiStatus::A;
        } else {
            switch (comp_bi.get_type()) {
            case CompositeBiType::UP:
                if (bi_high > this->a_1.get_high()){
                    if (this->a_0.get_low() < this->a_2.get_low()){
                        this->a_1 = comp_bi.generate_bi(this->a_1, comp_bi);
                        if (is_composite_bi_equal(this->a_0, this->a_1)) {
                            this->status = ZouShiChuLiStatus::a1_equal_a0;
                        } else {
                            if (bi_high > this->a_0.get_high()){
                                this->last_zoushi_process(this->a_0, this->a_1);
                                this->status = ZouShiChuLiStatus::a0;
                            } else {
                                this->status = ZouShiChuLiStatus::a2;
                            }
                        }
                    } else {
                        this->a_0 = comp_bi.generate_bi(this->a_0, this->a_2);
                        this->a_1 = comp_bi;
                        if (is_composite_bi_equal(this->a_0, this->a_1)){
                            this->status = ZouShiChuLiStatus::a1_equal_a0;
                        } else {
                            if (this->a_1.get_high() > this->a_0.get_high()){
                                this->last_zoushi_process(this->a_0, this->a_1);
                                this->status = ZouShiChuLiStatus::a0;
                            } else {
                                this->status = ZouShiChuLiStatus::a2;
                            }
                        }
                    }
                } else {
                    this->normal_bi = comp_bi;
                    this->normal_first_bi = CompositeBi();
                    this->status = ZouShiChuLiStatus::a2_equal_a1_normal;
                }
                break;
            case CompositeBiType::DOWN:
                if (bi_low < this->a_1.get_low()){
                    if (this->a_0.get_high() > this->a_2.get_high()){
                        this->a_0 = comp_bi.generate_bi(this->a_0, this->a_2);
                        this->a_1 = comp_bi;
                        if (is_composite_bi_equal(this->a_0, this->a_1)) {
                            this->status = ZouShiChuLiStatus::a1_equal_a0;
                        } else {
                            if (this->a_1.get_low() < this->a_0.get_low()){
                                this->last_zoushi_process(this->a_0, this->a_1);
                                this->status = ZouShiChuLiStatus::a0;
                            } else {
                                this->status = ZouShiChuLiStatus::a2;
                            }
                        }
                    } else {
                        this->a_1 = comp_bi.generate_bi(this->a_1, comp_bi);
                        if (is_composite_bi_equal(this->a_0, this->a_1)){
                            this->status = ZouShiChuLiStatus::a1_equal_a0;
                        } else {
                            if (this->a_1.get_low() < this->a_0.get_low()) {
                                this->last_zoushi_process(this->a_0, this->a_1);
                                this->status = ZouShiChuLiStatus::a0;
                            } else {
                                this->status = ZouShiChuLiStatus::a2;
                            }
                        }
                    }
                } else {
                    this->normal_bi = comp_bi;
                    this->normal_first_bi = CompositeBi();
                    this->status = ZouShiChuLiStatus::a2_equal_a1_normal;
                }
                break;
            }
        }
        break;

    case ZouShiChuLiStatus::a2_equal_a1_normal:
        ret_bi = this->normal_process(comp_bi);
        bi_type = ret_bi.get_type();
        if (bi_type == this->normal_bi.get_type()) {
            //返回一笔
            this->a_3 = ret_bi;
            if (is_composite_bi_equal(this->a_2, this->a_3)){
                this->A = ZhongShu(this->a_0, this->a_1, this->a_2, this->a_3);
                this->status = ZouShiChuLiStatus::A;
            } else {
                switch(bi_type){
                    case CompositeBiType::UP:
                        if (this->a_3.get_high() > this->a_1.get_high()){
                            if (this->a_0.get_low() < this->a_2.get_low()) {
                                this->a_1 = comp_bi.generate_bi(this->a_1, this->a_3);
                                if (is_composite_bi_equal(this->a_0, this->a_1)){
                                    this->status = ZouShiChuLiStatus::a1_equal_a0;
                                } else {
                                    if (this->a_1.get_high() > this->a_0.get_high()){
                                        this->last_zoushi_process(this->a_0, this->a_1);
                                        this->status = ZouShiChuLiStatus::a0;
                                    } else {
                                        this->status = ZouShiChuLiStatus::a2;
                                    }
                                }
                            } else {
                                this->a_0 = comp_bi.generate_bi(this->a_0, this->a_2);
                                this->a_1 = ret_bi;
                                if (is_composite_bi_equal(this->a_0, this->a_1)){
                                    this->status = ZouShiChuLiStatus::a1_equal_a0;
                                } else {
                                    if (this->a_1.get_high() > this->a_0.get_high()){
                                        this->last_zoushi_process(this->a_0, this->a_1);
                                        this->status = ZouShiChuLiStatus::a0;
                                    } else {
                                        this->status = ZouShiChuLiStatus::a2;
                                    }
                                }
                            }
                        } else {
                            this->status = ZouShiChuLiStatus::a2_equal_a1_normal;
                        }
                        break;
                    case CompositeBiType::DOWN:
                        if (this->a_3.get_low() < this->a_1.get_low()){
                            if (this->a_0.get_high() > this->a_1.get_high()){
                                this->a_1 = comp_bi.generate_bi(this->a_0, this->a_1);
                                if (is_composite_bi_equal(this->a_0, this->a_1)){
                                    this->status = ZouShiChuLiStatus::a1_equal_a0;
                                } else {
                                    if (this->a_1.get_low() < this->a_0.get_low()){
                                        this->last_zoushi_process(this->a_0, this->a_1);
                                        this->status = ZouShiChuLiStatus::a0;
                                    } else {
                                        this->status = ZouShiChuLiStatus::a2_equal_a1_normal;
                                    }
                                }
                            } else {
                                this->a_0 = comp_bi.generate_bi(this->a_0, this->a_2);
                                this->a_1 = comp_bi;
                                if (is_composite_bi_equal(this->a_0, this->a_1)){
                                    this->status = ZouShiChuLiStatus::a1_equal_a0;
                                } else {
                                    if (this->a_1.get_low() < this->a_0.get_low()){
                                        this->last_zoushi_process(this->a_0, this->a_1);
                                        this->status = ZouShiChuLiStatus::a0;
                                    } else {
                                        this->status = ZouShiChuLiStatus::a2;
                                    }
                                }
                            }

                        } else {
                            this->status = ZouShiChuLiStatus::a2_equal_a1_normal;
                        }
                        break;
                    }
                }
            } else {
                if (bi_type == CompositeBiType::NONE) {
                    this->status = ZouShiChuLiStatus::a2_equal_a1_normal;
                } else {
                    //返回两笔
                    this->a_2 = comp_bi.generate_bi(this->a_2, ret_bi);
                    if (is_composite_bi_equal(this->a_2, this->a_1)){
                        this->status = ZouShiChuLiStatus::a2_equal_a1;
                    } else {
                        switch(bi_type) {
                            case CompositeBiType::UP:
                                if (this->a_2.get_high() > this->a_0.get_high()){
                                    this->a_0 = comp_bi.generate_bi(this->a_0, this->a_2);
                                    this->status = ZouShiChuLiStatus::a1;
                                }else {
                                    this->status = ZouShiChuLiStatus::a2_normal;
                                }
                                break;
                            case CompositeBiType::DOWN:
                                if (this->a_2.get_low() < this->a_0.get_low()) {
                                    this->a_0 = comp_bi.generate_bi(this->a_0, this->a_2);
                                    this->status = ZouShiChuLiStatus::a1;
                                } else {
                                    this->status = ZouShiChuLiStatus::a2_normal;                            
                                }
                                break;
                            }
                    }
                }
            }
        break;
    case ZouShiChuLiStatus::A:
        ret_zs_value = this->A.find_zhongshu(comp_bi);
        switch(ret_zs_value.type) {
            case FindZhongShuReturnType::MAX_HIGH:
                this->b_0 = comp_bi;
                this->status = ZouShiChuLiStatus::A_MAX_HIGH;                
                break;
            case FindZhongShuReturnType::MIN_LOW:
                this->b_0 = comp_bi;
                this->status = ZouShiChuLiStatus::A_MIN_LOW;
                break;
            case FindZhongShuReturnType::THREE_BUY:
                this->b_1 = ret_zs_value.b_1;
                this->b_2 = ret_zs_value.b_2;
                this->status = ZouShiChuLiStatus::b3;
                break;
            case FindZhongShuReturnType::THREE_SELL:
                this->b_1 = ret_zs_value.b_1;
                this->b_2 = ret_zs_value.b_2;
                this->status = ZouShiChuLiStatus::b3;
                break;
        }
        break;
    case ZouShiChuLiStatus::A_MAX_HIGH:
        this->A.find_zhongshu(comp_bi);
        if (bi_low >= this->A.get_high()){
            this->b_1 = comp_bi;
            this->status = ZouShiChuLiStatus::THREE_BUY;
        }else {
            if (bi_low > this->A.get_low()) {
                this->status = ZouShiChuLiStatus::A;
            } else {
                this->A.stop(this->b_0);
                ret_zoushi.type = FindZouShiReturnType::One;
                ret_zoushi.zoushi1 = ZouShi(ZouShiType::UP, this->A.get_input(), this->A.get_output());
                this->a_0 = comp_bi;
                this->status = ZouShiChuLiStatus::a1;
            }
        }
        break;
    case ZouShiChuLiStatus::A_MIN_LOW:
        this->A.find_zhongshu(comp_bi);
        if (bi_high <= this->A.get_low()){
            this->b_1 = comp_bi;
            this->status = ZouShiChuLiStatus::THREE_SELL;
        }else {
            if (bi_high> this->A.get_low()) {
                this->status = ZouShiChuLiStatus::A;
            } else {
                this->A.stop(this->b_0);
                ret_zoushi.type = FindZouShiReturnType::One;
                ret_zoushi.zoushi1 = ZouShi(ZouShiType::DOWN, this->A.get_input(), this->A.get_output());
                this->a_0 = comp_bi;
                this->status = ZouShiChuLiStatus::a1;
            }
        }    
        break;
    case ZouShiChuLiStatus::b3:
        break;
    }

    return(ret_zoushi);
}

void Bi3_zoushi(int nCount, float* pOut, float* pHigh, float* pLow, float* pIn){
    BaoHanChuLi baohanChuli;
    ZouShiChuLi zoushichuli;
    ZouShi zoushi;
    int start_pos, stop_pos;

    for (int i = 0; i < nCount; i++) {
        baohanChuli.add(pHigh[i], pLow[i]);
    }

    for (int i = 0; i < nCount; i++) {
        pOut[i] = 0;
    }

    zoushichuli.handle(baohanChuli.kxianList);

    unsigned int count = zoushichuli.zoushi_list.size() - 1;
    if (count == 0)
        return;

    if (!zoushichuli.zoushi_list.empty()) {
        ZouShi start_zoushi = zoushichuli.zoushi_list[0];
        start_pos = start_zoushi.get_start_pos();
        stop_pos = start_zoushi.get_stop_pos();
        if (start_zoushi.get_start_xd().get_type() == CompositeBiType::UP) {
            pOut[start_pos] = -3;
            pOut[stop_pos] = 3;
        } else {
            pOut[start_pos] = 3;
            pOut[start_pos] = -3;
        }

        for (unsigned int i = count; i > 0; i--) {
            zoushi = zoushichuli.zoushi_list[i - 1];
            stop_pos = zoushi.get_stop_pos();
            if (zoushi.get_start_xd().get_type() == CompositeBiType::UP)
                pOut[stop_pos] = 3;
            else
                pOut[stop_pos] = -3;
        }
        
    }

}

void Bi4_zoushi(int nCount, float* pOut, float* pHigh, float* pLow, float* pIn){
    BaoHanChuLi baohanChuli;
    ZouShiChuLi zoushichuli;
    ZouShi zoushi;
    int start_pos, stop_pos;

    for (int i = 0; i < nCount; i++) {
        baohanChuli.add(pHigh[i], pLow[i]);
    }

    for (int i = 0; i < nCount; i++) {
        pOut[i] = 0;
    }

    zoushichuli.handle(baohanChuli.kxianList);

    unsigned int count = zoushichuli.zoushi_list.size() - 1;
    if (count == 0)
        return;

    if (!zoushichuli.zoushi_list.empty()) {
        ZouShi start_zoushi = zoushichuli.zoushi_list[0];
        start_pos = start_zoushi.get_start_pos();
        stop_pos = start_zoushi.get_stop_pos();
        if (start_zoushi.get_start_xd().get_type() == CompositeBiType::UP) {
            pOut[start_pos] = -3;
            pOut[stop_pos] = 3;
        } else {
            pOut[start_pos] = 3;
            pOut[start_pos] = -3;
        }

        for (unsigned int i = count; i > 0; i--) {
            zoushi = zoushichuli.zoushi_list[i - 1];
            stop_pos = zoushi.get_stop_pos();
            if (zoushi.get_start_xd().get_type() == CompositeBiType::UP)
                pOut[stop_pos] = 3;
            else
                pOut[stop_pos] = -3;
        }
        
    }

}
