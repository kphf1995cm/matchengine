/**********************************************
* \version   0.0.1
* \author    ubiquant
* \created   20150101
* \modified  20150101
* \brief:
* Contact:
* Note:
**********************************************/

#include "engine.h"

#define MATCH(a, b) \
  { reporter.MatchEvent(a, b); };

// BUY 0; SELL 1
int MatchEngine::LimitOrder(int direction, long price, long vol) {
	
	bool success = false;
	if (direction == BUY){ //buy
		for (std::map<long, std::vector<OrderItem>>::iterator iter = price_orderitems.begin(); iter != price_orderitems.end()&&success==false; ++iter) {
			if (iter->first > price)
				break;
			for (std::vector<OrderItem>::iterator oiter = price_orderitems[iter->first].begin(); oiter != price_orderitems[iter->first].end();++oiter) {
				if (oiter->direction == SELL) {
					if (oiter->vol>vol) {
						oiter->vol = oiter->vol - vol;
						MATCH(price, vol);
						success = true;
						break;
					}
					else {
						if (oiter->vol == vol) {
							price_orderitems[iter->first].erase(oiter);
							MATCH(price, vol);
							success = true;
							break;
						}
						else {
							MATCH(price, oiter->vol);
							vol = vol - oiter->vol;
							price_orderitems[iter->first].erase(oiter);
						}
					}
				}
			}
		}
		if (success == false){ // do not buy successfully 
			OrderItem order_item;
			order_item.direction = BUY;
			order_item.orderid = orderid;
			order_item.ordertype = LIMIT;
			order_item.vol = vol;
			price_orderitems[price].push_back(order_item);
			buy_orderids.push_back(orderid);
			orderid_price[orderid] = price;
		}
	}
	else { //SELL
		// check buy_orderids (从小到大)
		for (std::vector<int>::iterator iter = buy_orderids.begin(); iter != buy_orderids.end()&&success==false; ++iter) {
			for (std::vector<OrderItem>::iterator oiter = price_orderitems[orderid_price[*iter]].begin(); oiter != price_orderitems[orderid_price[*iter]].end(); ++oiter) {
				if (oiter->orderid == *iter) { //找到对应orderid
					if (oiter->ordertype == MARKET||(oiter->ordertype==LIMIT&&orderid_price[*iter]>=price)) {
						if (oiter->vol > vol) {
							MATCH(price, vol);
							oiter->vol = oiter->vol - vol;
							success = true;
							break;
						}
						else {
							if (oiter->vol == vol) {
								MATCH(price, vol);
								success = true;
								price_orderitems[orderid_price[*iter]].erase(oiter);
								buy_orderids.erase(iter);
								break;
							}
							else {
								MATCH(price, oiter->vol);
								vol = vol - oiter->vol;
								price_orderitems[orderid_price[*iter]].erase(oiter);
								buy_orderids.erase(iter); //删除迭代器后，iter失效？
							}
						}
					}
				}
			}
		}
		if (success == false) {  // do not sell successfully 
			OrderItem order_item;
			order_item.direction = SELL;
			order_item.orderid = orderid;
			order_item.ordertype = LIMIT;
			order_item.vol = vol;
			price_orderitems[price].push_back(order_item);
			orderid_price[orderid] = price;
		}
	}
	orderid++;
	// this is where you should handle the incoming limit orders

	// Whenever the matchengine makes an order match, you should
	// use the MATCH( PRICE, VOL ) macro to report the order execution.
	// e.g. the new order is a Buy order with price 100 and vol 120, current price
	// level has two counterpart orders (SELL order) as vol 20 and 80,
	// then you will make two mathes:
	// MATCH(100, 20)
	// MATCH(100, 80)

	// remember that you should return an orderid, which is generated and stored
	// by your limit order book and matchengine
	return orderid-1;
}

int MatchEngine::MarketOrder(int direction, long vol) {

	bool success = false;
	long price_level = 0;
	if (price_orderitems.size() > 0)
		price_level = price_orderitems.begin()->first;
	if (direction == BUY) {
		for (std::map<long, std::vector<OrderItem>>::iterator iter = price_orderitems.begin(); iter != price_orderitems.end() && success == false; ++iter) {
			for (std::vector<OrderItem>::iterator oiter = price_orderitems[iter->first].begin(); oiter != price_orderitems[iter->first].end(); ++oiter) {
				if (oiter->direction == SELL) {
					if (oiter->vol > vol) {
						oiter->vol = oiter->vol - vol;
						MATCH(iter->first, vol);
						success = true;
						break;
					}
					else {
						if (oiter->vol == vol) {
							MATCH(iter->first, vol);
							success = true;
							price_orderitems[iter->first].erase(oiter);
							break;
						}
						else{
							MATCH(iter->first, oiter->vol);
							price_level = iter->first;
							vol = vol - oiter->vol;
							price_orderitems[iter->first].erase(oiter);// oiter迭代器失效？
						}
					}
				}
			}
		}
		if (success == false) {
			OrderItem order_item;
			order_item.direction = BUY;
			order_item.orderid = orderid;
			order_item.ordertype =MARKET;
			order_item.vol = vol;
			price_orderitems[price_level].push_back(order_item);
			buy_orderids.push_back(orderid);
			orderid_price[orderid] = price_level;
		}
	}
	// this is where you should handle the incoming market orders

	// remember marketorder should also return an orderid
	orderid++;
	return orderid-1;
}

void MatchEngine::CancelOrder(int orderid) {
	// this is where you should handle the order cancelling request
	long price = orderid_price[orderid];
	for (std::vector<OrderItem>::iterator oiter = price_orderitems[price].begin(); oiter != price_orderitems[price].end(); ++oiter) {
		if (oiter->orderid == orderid)
		{
			price_orderitems[price].erase(oiter);
			break;
		}
	}
}
