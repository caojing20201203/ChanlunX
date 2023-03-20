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
    this->status = ZouShiChuLiStatus::START;
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

FindZouShiReturn ZouShiChuLi::find_zoushi(CompositeBi comp_bi) {
    FindZouShiReturn ret_zoushi;
    ZouShi last_zoushi;

    switch (this->status) {
    case ZouShiChuLiStatus::START:
        this->a_0 = comp_bi;
        this->status = ZouShiChuLiStatus::LEFT;
        break;

    case ZouShiChuLiStatus::LEFT:
        if (this->match_zhongshu_xianduan(comp_bi, this->a_0)) {
            //和this->a相差不多
            this->a_1 = comp_bi;
            this->status = ZouShiChuLiStatus::LEFT_EQUAL;
        }
        else {
            if (comp_bi.get_type() == CompositeBiType::DOWN && comp_bi.get_low() < this->a_0.get_low()) {
                //创新低
                this->status = ZouShiChuLiStatus::START;
                ret_zoushi.type = FindZouShiReturnType::Failure;
                ret_zoushi.zoushi1 = ZouShi(ZouShiType::DOWN, this->a_0, this->a_1);
            }
            else {
                if (comp_bi.get_type() == CompositeBiType::UP && comp_bi.get_high() > this->a_0.get_high()) {
                    //创新高
                    this->status = ZouShiChuLiStatus::START;
                    ret_zoushi.type = FindZouShiReturnType::Failure;
                    ret_zoushi.zoushi1 = ZouShi(ZouShiType::UP, this->a_0, this->a_1);
                }
                else {
                    this->a_1 = comp_bi;
                    this->status = ZouShiChuLiStatus::AFTER_LEFT;
                }
            }
        }
        break;

    case ZouShiChuLiStatus::LEFT_EQUAL:
        if (this->match_zhongshu_xianduan(this->a_1, comp_bi)) {
            //如果存在上一个走势
            last_zoushi = this->get_last_zoushi();
            if (last_zoushi.get_type() == ZouShiType::NONE) {
                this->A = ZhongShu(CompositeBi(), this->a_0, this->a_1, comp_bi);
                this->status = ZouShiChuLiStatus::A;
            }
            else {
                //上一个走势处理
                OutputDebugPrintf(" 上一个走势未处理！");
            }
        }
        else {
            if (comp_bi.get_type() == CompositeBiType::UP) {
                if (this->a_0.get_low() < this->a_1.get_low()) {
                    if (comp_bi.get_high() > this->a_1.get_high()) {
                        this->a_0 = comp_bi.generate_bi(this->a_0, comp_bi);
                        this->status = ZouShiChuLiStatus::LEFT;
                    }
                    else {
                        this->a_2 = comp_bi;
                        this->status = ZouShiChuLiStatus::AFTER_LEFT_EQUAL;
                    }
                }
                else {
                    last_zoushi = this->get_last_zoushi();
                    OutputDebugPrintf("LEFT_EQUAL 上一个走势 未处理");
                }
            }
            else {
                //下降笔
                if (this->a_0.get_high() > this->a_1.get_high()) {
                    if (comp_bi.get_low() < this->a_1.get_low()) {
                        this->a_0 = comp_bi.generate_bi(this->a_0, comp_bi);
                        this->status = ZouShiChuLiStatus::LEFT;
                    }
                    else {
                        this->a_2 = comp_bi;
                        this->status = ZouShiChuLiStatus::AFTER_LEFT_EQUAL;
                    }
                }
                else {
                    last_zoushi = this->get_last_zoushi();
                    OutputDebugPrintf("LEFT_EQUAL 上一个走势 未处理");
                }
            }
        }
        break;

    case ZouShiChuLiStatus::AFTER_LEFT_EQUAL:
        //**************************************************************
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
