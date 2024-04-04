//https://github.com/doctest/doctest

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN //To set up the testing framework
#include "doctest.h" //Same comment as above

#include <iostream>
#include <vector>
#include <future>
#include <chrono>



/*Sources
https://www.cppstories.com/2014/01/tasks-with-stdfuture-and-stdasync/
https://www.geeksforgeeks.org/vector-in-cpp-stl/?ref=lbp
https://cplusplus.com/reference/future/future/
https://cplusplus.com/reference/future/async/
https://www.geeksforgeeks.org/measure-execution-time-function-cpp/
*/


using std::vector;
using std::cout;
using std::endl;

using std::future;
using std::async;

using namespace std::chrono;

int async_version(vector<int> vec, int key);
int count_keys(vector<int> vec, int key);

int factorial(int number) { return number <= 1 ? number : factorial(number - 1) * number; }

int async_version(vector<int> vec, int key) {
	//Split vector
	auto temp1 = vector<int>(vec.begin(), vec.begin() + (vec.size() / 2));
	auto temp2 = vector<int>(vec.begin() + (vec.size() / 2), vec.end());

	//Create 2 futures
	future<int> countKeyFirstHalf = async(count_keys, temp1, key);
	future<int> countKeySecondHalf = async(count_keys, temp2, key);

	//Return the sum of the counts
	return countKeyFirstHalf.get() + countKeySecondHalf.get();
}

int count_keys(vector<int> vec, int key) {
	int count = 0;
	for (auto i = vec.begin(); i < vec.end(); i++) {
		count += (*i == key) ? 1 : 0;
	}
	return count;
}






TEST_CASE("Testing the async function") {
	vector<int> vec1 = vector<int>({ 1,2,3,4,5 });


    CHECK(async_version(vec1,1) == 1);

	/*
    CHECK(factorial(2) == 2);
    CHECK(factorial(3) == 6);
    CHECK(factorial(10) == 3628800);*/
}
