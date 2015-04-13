/**
 * COPYRIGHT NOTICE
 * Copyright (c) 2012, Computational Science Laboratory, Stony Brook University.
 * All rights reserved.
 *
 * @file mtpi_coder.h
 * @version 1.0
 * @author Peng Zhang <Peng.Zhang@StonyBrook.edu>
 * @date 2012-11-8
 * @brief the Coder class.
 *
**/
#ifndef _MTPI_CODRE_H
#define _MTPI_CODRE_H

#include "mtpi_common.h"


/**
 * A template for converting a string to a number, integer/float/double/long int.
 */
template <class T>
class String2Number {
public:
	T convert (char * str) {
		string type (typeid(T).name());
		if (type.compare(typeid(int).name())==0)
			return atoi (str);
		if (type.compare(typeid(long).name())==0)
			return atol (str);
		if (type.compare(typeid(double).name())==0 || type.compare(typeid(float).name())==0)
			return atof (str);
		stringstream ss;
		ss << "Not support the type [" << type.data () << "]";
		MTPI_Write_Info (ERR_INFO, ss);
		return 0;
	}
};


template <class T, int T_SIZE>
class CoderBase {
private:
	static const int datum_size = T_SIZE;
	String2Number<T> _convertor;

	T * data;
	int nelems;

	string _encoded_string;

public:
	CoderBase () {
		data = NULL;
		clear ();
	}

	~CoderBase () {
		clear ();
	}

	/**
	 * @return Size of an encoded string.
	 */
	int size () {
		return (int) _encoded_string.size ();
	}

	/**
	 * @brief clear data
	 */
	void clear () {
		_encoded_string.clear ();
		if (data!=NULL )
			delete [] data;
		data = NULL;
	}

	/**
	 * @brief Encode the data array
	 * @param [in] d Pointer to the data array to be encoded
	 * @param [in] n Number of data to be encoded
	 * @return Pointer to the encoded string.
	 */
	const char * encode (T * d, int n) {
		int s;
		clear ();
		return encode (d, n, &s);
	}

	/**
	 * @brief Encode the data array
	 * @param [in] d Pointer to the data array to be encoded
	 * @param [in] n Number of data to be encoded
	 * @param [out] encoded_length Length of the encoded string
	 * @return Pointer to the encoded string.
	 */
	const char * encode (T * d, int n, int * encoded_length) {
		stringstream ss;
		for (int i=0; i<n; i++)  
			ss << setw(datum_size) << setfill (' ') << d[i];
		clear ();
		_encoded_string.assign (ss.str().c_str());
		ss.clear();
		*encoded_length = size ();
		return _encoded_string.c_str ();
	}

	/**
	 * @brief Decode a string
	 * @param [in] str Pointer to an encoded string
	 * @param [in] n Number of expected data
	 * @return Pointer to the decoded string
	 */
	T * decode (const char * str, int n) {
		char buffer [T_SIZE+1];
		buffer [T_SIZE] = '\0';
		clear ();
		nelems = n;
		data = new T [nelems];
		for (int i=0; i<nelems; i++) {
			strncpy (buffer, &(str[i*T_SIZE]), T_SIZE);
			data [i] = _convertor.convert (buffer);
		}
		return data;
	}

};


typedef CoderBase<int, INT_WIDTH> IntCoder;
typedef CoderBase<long, LONG_INT_WIDTH> LongCoder;
typedef CoderBase<float, FLOAT_WIDTH> FloatCoder;
typedef CoderBase<double, DOUBLE_WIDTH> DoubleCoder;

#endif
