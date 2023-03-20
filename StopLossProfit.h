///////////////////////////////////////////////////////////
//  StopLossProfit.h
//  Implementation of the Class StopLossProfit
//  Created on:      08-1ÔÂ-2023 12:36:34
//  Original author: ldj
///////////////////////////////////////////////////////////

#if !defined(EA_855DC049_3609_4b2c_986D_041DA6FF9556__INCLUDED_)
#define EA_855DC049_3609_4b2c_986D_041DA6FF9556__INCLUDED_

class StopLossProfit
{

public:
	StopLossProfit();
	virtual ~StopLossProfit();

	buy(var self, var buy_datetime, var price);
	calculate_yingkui(var self, var price);
	get_cost(var self, var price);
	get_kuisun(var self, var price);
	get_lirun(var self, var price);
	None(var self, var code, var amount);
	sell(var self, var sell_datetime, var price);
	set_zhisun(var self, var price);
	set_zhiying(var self, var price);

};
#endif // !defined(EA_855DC049_3609_4b2c_986D_041DA6FF9556__INCLUDED_)
