#include "bunsan/base64.hpp"

#include <algorithm>

#include <cstdlib>
#include <cassert>

using namespace std;
using namespace bunsan::base64;

template <typename T>
void test(const T &data)
{
	string b = encode(data);
	auto data2 = decode(b);
	T x(data2.begin(), data2.end());
	assert(data==x);
}

int main()
{
	vector<unsigned char> data;
	data.resize(1000000);
	generate(data.begin(), data.end(), [](){return char((rand()&1)*(rand()%256));});
	test(data);
	generate(data.begin(), data.end(), [](){return char(rand()%256);});
	test(data);
	data.resize(0);
	test(data);
	generate(data.begin(), data.end(), [](){return char(rand()%256);});
	test(data);
	string data_(1000000, '\0');
	generate(data_.begin(), data_.end(), [](){return char((rand()&1)*(rand()%256));});
	test(data_);
	generate(data_.begin(), data_.end(), [](){return char(rand()%256);});
	test(data_);
	data_.resize(0);
	test(data_);
}

