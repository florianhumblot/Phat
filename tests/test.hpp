#ifndef TEST_HPP
#define TEST_HPP
#include "hwlib.hpp"
#include <vector>
#include <iostream>

class TestSuite {
private:
	int testCount = 0;
	int passedTests = 0;

public:
	std::vector<std::vector<uint32_t>> test;
	TestSuite() {}
	template <typename T >
	bool assertEquals( T lhs, T rhs ) {
		testCount++;
		if ( lhs == rhs ) {
			passedTests++;
		} else {
			std::vector<uint32_t> temp = { lhs, rhs };
			test.emplace_back( temp );
		}
		return ( lhs == rhs );
	}
	int getTestCount() {
		return testCount;
	}
	int getPassedTests() {
		return passedTests;
	}

};

#endif