#include "rotors.h"
#include "errors.h"
#include <iostream>
#include <fstream>

using namespace std;


/* We set rot[0] to be the right-most rotor, rot[1] to be the rotor to its left etc. */

Rotor::Rotor() {
  for (int i = 0; i < MAX_SIZE; i++) {
    config_forward_mapping[i] = -1;
    config_backward_mapping[i] = -1;
    notch[i] = -1;
  }
  rotations_count = 0;
  error = NO_ERROR;
}

Rotor::~Rotor() {}

void Rotor::create_mapping(const char* filename) {
  ifstream in_stream;
  int number, count, notch_count;

  count = 0;

  in_stream.open(filename);

  if (in_stream.fail()) {
    in_stream.close();
    cerr << "The program encountered an error opening or reading the rotor configuration file " << filename << endl;
    error = ERROR_OPENING_CONFIGURATION_FILE;
    return ;
  }

  in_stream >> number;

  while (!in_stream.eof() && count < MAX_SIZE) {
    
    if (in_stream.fail()) {
      in_stream.close();
      cerr << "Non-numeric character for mapping in rotor file " << filename << endl;
      error = NON_NUMERIC_CHARACTER;
      return ;
    }

    if (is_invalid_index(number)) {
      in_stream.close();
      cerr << "The rotor file " << filename << " contains a number that is not between 0 and 25" << endl;
      error = INVALID_INDEX;
      return ;
    }

    if (is_invalid_rotor_mapping(number, count)) {
      in_stream.close();
      cerr << "The rotor file " << filename << " attempts to map more than one input to the same output" << endl;
      error = INVALID_ROTOR_MAPPING;
      return ;
    }

    config_forward_mapping[count] = number;
    config_backward_mapping[number] = count;
    count++;
    in_stream >> number;
  }

  if (count < MAX_SIZE) {
    in_stream.close();
    cerr << "Not all inputs mapped in rotor file: " << filename << endl;
    error = INVALID_ROTOR_MAPPING;
    return ;
  }

  notch_count = 0;
  
  while (!in_stream.eof()) {

    if (in_stream.fail()) {
      in_stream.close();
      cerr << "Non-numeric character for mapping in rotor file " << filename << endl;
      error = NON_NUMERIC_CHARACTER;
      return ;
    }

    if (is_invalid_index(number)) {
      in_stream.close();
      cerr << "The rotor file " << filename << " contains a number for a notch that is not between 0 and 25" << endl;
      error = INVALID_INDEX;
      return ;
    }

    if (is_repeated_notch(number)) {
      in_stream.close();
      cerr << "Repeated notch number (" << number << ") in the rotor file " << filename << endl;
      error = INVALID_ROTOR_MAPPING;
      return ;
    }

    if (notch_count < MAX_SIZE) {
      notch[number] = number;
      notch_count++;
    }
    else {
      notch_count++;
      break;
    } 
    in_stream >> number;
  }

  if (notch_count > MAX_SIZE) {
    in_stream.close();
    cerr << "The rotor file " << filename << " contains too many notches" << endl;
    return ;
  }
  in_stream.close();
}

bool Rotor::is_invalid_index(int number) {
  if ((number < 0) || (number > 25))
    return true;

  return false;
}

bool Rotor::is_invalid_rotor_mapping(int number, int count) {
  while (count >= 1) {
    if (number == config_forward_mapping[count - 1])
      return true;
    count--;
  }
  return false;
}

bool Rotor::is_repeated_notch(int number) {
  if (notch[number] != -1)
    return true;

  return false;
}

bool Rotor::is_error() {
  if (error != NO_ERROR)
    return true;

  return false;
}

int Rotor::return_rotor_error() { return error; }

bool Rotor::is_notch_hit() {
  if (notch[rotations_count] != -1 )
    return true;

  return false;
}

void Rotor::rotor_rotate() {
  update_difference_mapping();
  update_config_mapping();
  rotations_count = (rotations_count + 1) % MAX_SIZE;
}

void Rotor::rotation_difference_mapping() {
  for (int i = 0; i < MAX_SIZE; i++)
    difference[i] = config_forward_mapping[i] - i;
}

void Rotor::update_difference_mapping() {
  int tmp;
  tmp = difference[0];
  for (int i = 0; i < MAX_SIZE - 1 ; i++) {
    difference[i] = difference[i+1];
  }
  difference[MAX_SIZE - 1] = tmp;
}

void Rotor::update_config_mapping() {
  for (int i = 0; i < MAX_SIZE; i++) {
    if (i + difference[i] >= 0)
      config_forward_mapping[i] = (i + difference[i]) % MAX_SIZE ;
    else
      config_forward_mapping[i] = MAX_SIZE + (i + difference[i]);
  }
  for (int i = 0; i < MAX_SIZE; i++)
    config_backward_mapping[config_forward_mapping[i]] = i ;
}

void Rotor::forward_scrambling(int& number) {
  input_number = number;

  config_forward_scrambling();

  number = mapped_number;
}

void Rotor::backward_scrambling(int& number) {
  input_number = number;

  config_backward_scrambling();

  number = mapped_number;
}

void Rotor::config_forward_scrambling() { mapped_number = config_forward_mapping[input_number] ; }

void Rotor::config_backward_scrambling() { mapped_number = config_backward_mapping[input_number] ; }

void Rotor::set_starting_position(int number) { starting_position = number ; }

int Rotor::get_starting_position() { return starting_position ; }
