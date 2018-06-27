#ifndef TEST_HPP
#define TEST_HPP
#include "hwlib.hpp"
#include <vector>
#include <iostream>
#include <string>
class TestSuite {
private:
	int testCount = 0;
	int passedTests = 0;
	int i = 0;
public:
	std::vector<std::vector<uint32_t>> test;
	TestSuite() {}

	// Function from StackOverflow - https://stackoverflow.com/questions/11421432/how-can-i-output-the-value-of-an-enum-class-in-c11
	template <typename Enumeration>
	auto as_integer( Enumeration const value )
		-> typename std::underlying_type<Enumeration>::type {
		return static_cast<typename std::underlying_type<Enumeration>::type>( value );
	}
	template <typename T >
	bool assertEquals( T lhs, T rhs, std::string name = "unknown" ) {
		i++;
		testCount++;
		if ( lhs == rhs ) {
			passedTests++;
		} else {
			std::vector<uint32_t> temp = { lhs, rhs };
			std::cout << name << " failed" << std::endl;
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