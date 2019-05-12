#include "reflector.h"
#include "errors.h"
#include <iostream>
#include <fstream>

using namespace std;


Reflector::Reflector() {
  for (int i = 0; i < MAX_SIZE; i++)
    mapping[i] = -1;
  error = NO_ERROR;
}

Reflector::~Reflector() {}

void Reflector::create_mapping(const char* filename) {
  ifstream in_stream;
  int number1, number2, count;

  count = 0;

  in_stream.open(filename);

  if (in_stream.fail()) {
    in_stream.close();
    cerr << "The program encountered an error opening or reading the reflector configuration file " << filename << endl;
    error = ERROR_OPENING_CONFIGURATION_FILE;
    return ;
  } 

  in_stream >> number1;

  while (!in_stream.eof()) {
    
    count++;

    if (in_stream.fail()) {
      in_stream.close();
      cerr << "Non-numeric character in reflector file " << filename << endl;
      error = NON_NUMERIC_CHARACTER;
      return ;
    }

    in_stream >> number2;

    if (!in_stream.eof()) {

      if (in_stream.fail()) {
	in_stream.close();
	cerr << "Non-numeric character in reflector file " << filename << endl;
	error = NON_NUMERIC_CHARACTER;
	return ;
      }

      if (is_invalid_reflector_mapping(number1, number2)) {
	in_stream.close();
	cerr << "The reflector configuration file " << filename << " attempts to map an input to itself or pair each index with more than one other" << endl;
	error = INVALID_REFLECTOR_MAPPING;
	return ;
      }

      if (is_invalid_index(number1, number2)) {
	in_stream.close();
	cerr << "The reflector configuration file " << filename << " contains at least one number that is not between 0 and 25" << endl;
	error = INVALID_INDEX;
	return ;
      }

      mapping[number1] = number2;
      mapping[number2] = number1;

      count++;

      in_stream >> number1;
    }
  }

  if (is_incorrect_number_of_reflector_pairs(count)) {
    in_stream.close();
    cerr << "Incorrect (odd) number of parameters in reflector file " << filename << endl;
    error = INCORRECT_NUMBER_OF_REFLECTOR_PARAMETERS;
    return ;
  }

  if (count < MAX_SIZE) {
    in_stream.close();
    cerr << "Insufficient number of mappings in reflector file: " << filename << endl;
    error = INCORRECT_NUMBER_OF_REFLECTOR_PARAMETERS;
    return ;
  }
  in_stream.close();
}

void Reflector::scrambling(int& number) {
  input_number = number;

  config_scrambling();

  number = output_number;
}

void Reflector::config_scrambling() { output_number = mapping[input_number]; }

bool Reflector::is_invalid_reflector_mapping(int number1, int number2) {
  if ((number1 == number2) || (mapping[number1] != -1) || (mapping[number2] != -1))
    return true;

  return false;
}

bool Reflector::is_invalid_index(int number1, int number2) {
  if ((number1 < 0) || (number1 > 25) || (number2 < 0) || (number2 > 25))
    return true;

  return false;
}

bool Reflector::is_incorrect_number_of_reflector_pairs(int count) {
  if (count % 2 == 1)
    return true;

  return false;
}  

bool Reflector::is_error() {
  if (error != NO_ERROR)
    return true;

  return false;
}

int Reflector::return_reflector_error() { return error; }
