#include "bunsan/base64.hpp"

#include <algorithm>

#include <cstdlib>
#include <cassert>

using namespace std;
using namespace bunsan::base64;

void test(const vector<unsigned char> &data)
{
	string b = encode(data);
	vector<unsigned char> data2 = decode(b);
	assert(data==data2);
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
}

