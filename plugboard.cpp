#include "plugboard.h"
#include "errors.h"
#include <iostream>
#include <fstream>

using namespace std;


Plugboard::Plugboard() {
  for (int i = 0; i < MAX_SIZE; i++)
    config_mapping[i] = i;
  error = NO_ERROR;
}

Plugboard::~Plugboard() {}

void Plugboard::scrambling(int& number) {
  input_number = number;
  config_scrambling();
  number = mapped_number;
}

void Plugboard::config_scrambling() { mapped_number = config_mapping[input_number] ; }

void Plugboard::create_mapping(const char* filename) {
  ifstream in_stream;
  int number1, number2, count;

  count = 0;
  
  in_stream.open(filename);

  if (in_stream.fail()) {
    in_stream.close();
    cerr << "The program encountered an error opening or reading the plugboard configuration file " << filename << endl;
    error = ERROR_OPENING_CONFIGURATION_FILE;
    return ;
  }

  in_stream >> number1;
  
  while (!in_stream.eof()) {

    count++;

    if (in_stream.fail()) {
      in_stream.close();
      cerr << "Non-numeric character in plugboard file " << filename << endl;
      error = NON_NUMERIC_CHARACTER;
      return ;
    }
    
    in_stream >> number2;

    if (!in_stream.eof()) {

      if (in_stream.fail()) {
	in_stream.close();
	cerr << "Non-numeric character in plugboard file " << filename << endl;
	error = NON_NUMERIC_CHARACTER;
      	return ;
      }

      if (is_invalid_index(number1, number2)) {
	in_stream.close();
	cerr << "The plugboard file " << filename << " contains a number that is not between 0 and 25" << endl;
	error = INVALID_INDEX;
	return ;
      }

      if (is_impossible_plugboard_configuration(number1, number2)) {
	in_stream.close();
	cerr << "There is an impossible plugboard configuration in the file " << filename << endl;
	error = IMPOSSIBLE_PLUGBOARD_CONFIGURATION;
	return ;
      }	
    
      config_mapping[number1] = number2;
      config_mapping[number2] = number1;

      count++;

      in_stream >> number1;
    }
  }
  
  if (is_above_possible(count)) {
    in_stream.close();
    cerr << "Incorrect number of parameters in plugboard file " << filename << endl;
    error = INCORRECT_NUMBER_OF_PLUGBOARD_PARAMETERS;
    return ;
  } 
  

  if (is_odd_plugboard_parameters(count)) {
    in_stream.close();
    cerr << "Incorrect number of parameters in plugboard file " << filename << endl;
    error = INCORRECT_NUMBER_OF_PLUGBOARD_PARAMETERS;
    return ;
  }   
  in_stream.close();
}

bool Plugboard::is_above_possible(int count) {
  if (count > MAX_SIZE)
    return true;

  return false;
}

bool Plugboard::is_invalid_index(int number1, int number2) {
  if ((number1 < 0) || (number1 > 25) || (number2 < 0) || (number2 > 25))
    return true;

  return false;
}

bool Plugboard::is_impossible_plugboard_configuration(int number1, int number2) {
  if ((number1 == number2) ||(config_mapping[number1] != number1) || (config_mapping[number2] != number2))
    return true;

  return false;
}

bool Plugboard::is_odd_plugboard_parameters(int count) {
  if (count % 2 == 1)
    return true;

  return false;
}

bool Plugboard::is_error() {
  if (error != NO_ERROR)
    return true;

  return false;
}

int Plugboard::return_plugboard_error() { return error; }

