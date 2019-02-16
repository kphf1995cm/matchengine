/**********************************************
* \version   0.0.1
* \author    ubiquant
* \created   20150101
* \modified  20150101
* \brief:
* Contact:
* Note:
**********************************************/

#ifndef _engine_H_lSpJLFkhxa_
#define _engine_H_lSpJLFkhxa_

#include <iostream>
#include <string>
#include <vector>
#include <map>

#define  BUY  0
#define  SELL 1
#define LIMIT 0
#define MARKET 1

//! this is the matchengine
class MatchEngine {
protected:
	struct OrderItem {
		int ordertype;// LIMIT 0; MARKET 1
		int direction; // BUY 0; SELL 1
		long vol;
		int orderid; // represent order
	};
	int orderid;
	// this is where you should put your data structure
	// holding the limit order book; You could design the limit
	// order book as you wish.
	// NOTE that the limit order book has price range of 100-200

	std::map<long, std::vector<OrderItem>> price_orderitems; // price OrderItems£¬ represents limit order
	std::map<int, long> orderid_price; //orderid price
	std::vector<int> buy_orderids;

public:
	MatchEngine() { orderid = 0; }
	// LimitOrder handles the incoming order, direction could
	// be BUY/SELL, price and volume are long integer
	// return:  orderid (this id is managed by your limit order boook,
	// and will be possibly used by CancelOrder )
	int LimitOrder(int direction, long price, long vol);

	// CancelOrder handles order cancelling request, you should
	// input orderid, which is returned by LimitOrder beforehand.
	void CancelOrder(int orderid);

	// MarketOrder handles incomming Market order
	// return: orderid
	int MarketOrder(int direction, long vol);

};


//! this is the class used in test, you can safely ignore it.
class Reporter {
private:
	struct EVENT {
		long price;
		long vol;
	};
public:
	std::vector<EVENT> events;
public:
	int num_events;
	long total_price;

	void MatchEvent(long price, long vol) {
		EVENT event;
		event.price = price;
		event.vol = vol;
		++num_events;
		total_price += price;
		events.push_back(event);
	}

	void Clear() {
		num_events = 0;
		total_price = 0;
	}

};

extern Reporter reporter;

#endif
