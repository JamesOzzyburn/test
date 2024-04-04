/*Sources
https://www.geeksforgeeks.org/vector-in-cpp-stl/?ref=lbp //Vector reference
https://cplusplus.com/reference/future/future/ //How to use futures
https://cplusplus.com/reference/future/async/ //How to use async

https://www.geeksforgeeks.org/measure-execution-time-function-cpp/ //Only used for timing

https://www.geeksforgeeks.org/rand-and-srand-in-ccpp/ //Only used for the randomized testing
https://www.geeksforgeeks.org/find-all-occurrences-of-element-in-vector/ //Only used for the randomized testing

https://github.com/doctest/doctest //Testing framework
*/

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN //To set up the testing framework
#include "doctest.h" //Same comment as above

#include <vector> //Vector is effectivly the same as an array I just used it as you can get iterators from it
#include <future> //For the async stuff

//Only used if you want to time
#include <chrono>
#include <iostream>

using namespace std;

int async_version(vector<int> vec, int key);
int count_keys(vector<int> vec, int key);

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

//Testing stuff
TEST_CASE("testing the async function with fixed values") {
	vector<int> vec1 = vector<int>({ 1,2,3,4,5 });
	vector<int> vec2 = vector<int>({ 1,1,1,1,1,1,1,1,1,1,1,1 });
	vector<int> vec3 = vector<int>({});

    CHECK(async_version(vec1,1) == 1);
	CHECK(async_version(vec1, 6) == 0);
	CHECK(async_version(vec3, 210) == 0);

}

TEST_CASE("testing the async function with randomized values") {
	vector<int> vec1; //make vectors
	vector<int> vec2;
	vector<int> vec3;

	int randKey = rand(); //get rand key

	int count1 = 0; 
	int count2 = 0;
	int count3 = 0;

	for (int i = 0; i < 1000000; i++) { //fill vectors with random numbers
		vec1.push_back(rand());
		vec2.push_back(rand());
		vec3.push_back(rand());
	}

	auto it1 = find(vec1.begin(), vec1.end(), randKey); //make iterators to run over the vectors
	auto it2 = find(vec2.begin(), vec2.end(), randKey);
	auto it3 = find(vec3.begin(), vec3.end(), randKey);

	//Search through them
	while (it1 != vec1.end()) {
		count1 += 1;
		it1 = find(it1 + 1, vec1.end(), randKey);
	}

	while (it2 != vec2.end()) {
		count2 += 1;
		it2 = find(it2 + 1, vec2.end(), randKey);
	}

	while (it3 != vec3.end()) {
		count3 += 1;
		it3 = find(it3 + 1, vec3.end(), randKey);
	}

	CHECK(async_version(vec1, randKey) == count1);
	CHECK(async_version(vec2, randKey) == count2);
	CHECK(async_version(vec3, randKey) == count3);
}



/* Just in case you want to time it
* //using namespace std::chrono; uncomment this as well
int main(vector<int> vec, int key) {
	//First run "regularly"
	cout << "First run 'regularly'" << endl;
	auto start = high_resolution_clock::now();
	int count = count_keys(vec, key);
	cout << count << " matches" << endl;
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(stop - start);

	cout << "Finished in: " << duration.count() << " microseconds" << endl;

	//Second run async with 2 futures
	cout << "\nSecond run async with 2 futures" << endl;
	start = high_resolution_clock::now();
	count = async_version(vec, key);
	cout << count << " matches" << endl;
	stop = high_resolution_clock::now();
	duration = duration_cast<microseconds>(stop - start);
	cout << "Finished in: " << duration.count() << " microseconds" << endl;

	return 0;
}*/
