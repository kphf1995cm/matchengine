/**********************************************
* \version   0.0.1
* \author    ubiquant
* \created   20150101
* \modified  20150101
* \brief:
* Contact:
* Note:
**********************************************/

#include <vector>
#include "engine.h"

//****** following items are for test purpose, please do not modify ***************
MatchEngine engine;
Reporter reporter;
std::vector<int> orderid;
static int num_test = 0;

#define TEST(d, p, v, n, ap) {\
    reporter.Clear();\
    orderid.push_back( engine.LimitOrder(d, p, v) );\
    if (reporter.num_events == n && reporter.total_price == ap)\
        std::cout<<"SUCC: TEST " << ++num_test<<" MATCH"<<std::endl; \
    else \
        std::cout<<"FAIL: TEST " << ++num_test<<" MISMATCH"<<std::endl;\
}

#define TEST_CANCEL(ind) {\
    reporter.Clear();\
    engine.CancelOrder(orderid[ind]);\
}


#define TEST_MARKET(d, v, n, ap) {\
    reporter.Clear();\
    orderid.push_back( engine.MarketOrder(d, v) );\
    if (reporter.num_events == n && reporter.total_price == ap)\
        std::cout<<"SUCC: TEST " << ++num_test<<" MATCH"<<std::endl; \
    else \
        std::cout<<"FAIL: TEST " << ++num_test<<" MISMATCH"<<std::endl;\
}

//*********************************************************************************

int main(int argc, char** argv)
{
	std::cout << "************ MATCH ENGINE *************" << std::endl;
	TEST(SELL, 100, 100, 0, 0);
	TEST(SELL, 101, 10, 0, 0);
	TEST(BUY, 99, 8, 0, 0);
	TEST(BUY, 100, 10, 1, 100);
	TEST(BUY, 101, 95, 2, 201);
	TEST(SELL, 101, 5, 0, 0);
	TEST(BUY, 100, 10, 0, 0);
	TEST(BUY, 99, 10, 0, 0);
	TEST_CANCEL(7);
	TEST(BUY, 99, 6, 0, 0);
	TEST(SELL, 97, 30, 3, 298);
	TEST(BUY, 98, 1, 1, 97);
	TEST_MARKET(BUY, 8, 2, 198);
	return 0;
};


