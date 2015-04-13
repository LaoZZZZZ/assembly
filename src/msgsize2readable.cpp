/**
 * COPYRIGHT NOTICE
 * Copyright (c) 2012, Computational Science Laboratory, Stony Brook University.
 * All rights reserved.
 *
 * @file msgsize2readable.cpp
 * @version 1.0
 * @author Peng Zhang <Peng.Zhang@StonyBrook.edu>
 * @date 2013-06-11
 * @brief convert a message size to a human readable formatted string
 *
**/
#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <string>
#include <iostream>
#include <iomanip>


const char * msgsize2readable (
	int size, std::string & msg)
{
	msg.clear ();
	double number = size;
	double base = 1024.0;
	int unit = 0;
	
	std::stringstream ss;
	while (number > base && unit < 4) {
		number /= base; unit ++;
	}
	ss << std::fixed << std::setprecision (2) << number;

	switch (unit) {
	  case 0: ss << " B"; break;
	  case 1: ss << " KB"; break;
	  case 2: ss << " MB"; break;
	  case 3: ss << " GB"; break;
	  default: break;
	}
	msg.assign (ss.str().c_str());
	return msg.c_str();
}

#ifdef MAIN_MSGSIZE2READABLE

using namespace std;

void test_msgsize2readable (int size) {
	string msg;
	msgsize2readable (size, msg);
	cout << "INFO: testing size = [" << size << "] -> ["
	     << msg.c_str() << "]" << endl;
	cout.flush ();
}

int main (int argc, char * argv []) {

	cout << "INFO: Testing msgsize2readable" << endl;

	string msg;
	int size = 102223;

	test_msgsize2readable (1022);
	test_msgsize2readable (12345678);
	test_msgsize2readable (9239933);
	test_msgsize2readable (102);
	test_msgsize2readable (102223);
	cout << "INFO: testing size = [" << size << "] -> ["
	     << msgsize2readable (102223, msg) << "]" << endl;

	cout << "INFO: Testing msgsize2readable done!" << endl;
	return 0;
}

#endif // end of MAIN_MSGSIZE2READABLE
