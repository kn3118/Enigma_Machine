#include "enigma.h"
#include "plugboard.h"
#include "rotors.h"
#include "reflector.h"
#include "errors.h"
#include <iostream>
#include <fstream>

using namespace std;


/* We set rot[0] to be the right-most rotor, rot[1] to be the rotor to its left etc. */

Enigma::Enigma() {}

Enigma::~Enigma() {
  delete pb;
  delete ref;
  if (number_of_rotors > 0) {
    for (int i = 0; i < number_of_rotors; i++)
      delete rot[i];
    delete[] rot;
  }
}

void Enigma::set_components(int argc, char** argv) {
  error = NO_ERROR;
  number_of_rotors = argc - 4;
  pb = new (nothrow) Plugboard () ;
  ref = new (nothrow) Reflector () ;
  if (number_of_rotors > 0) {
    rot = new (nothrow) Rotor* [number_of_rotors];
    for (int i = 0; i < number_of_rotors; i++)
      rot[i] = new (nothrow) Rotor ();
  }

  if ((argc < 3) || (argc == 4)) {
    cerr << "usage:enigma plugboard-file reflector-file (<rotor-file>* rotor-positions)?" << endl;
    error = INSUFFICIENT_NUMBER_OF_PARAMETERS;
    return ;
  }

  pb->create_mapping(argv[1]);
  if (pb->is_error()) {
    error = pb->return_plugboard_error();
    return ;
  }

  ref->create_mapping(argv[2]);
  if (ref->is_error()) {
    error = ref->return_reflector_error();
    return ;
  }

  for (int i = 0; i < number_of_rotors; i++) {
    rot[number_of_rotors - i - 1]->create_mapping(argv[i+3]);
    rot[number_of_rotors - i - 1]->rotation_difference_mapping();
    if (rot[number_of_rotors - i - 1]->is_error()) {
      error = rot[number_of_rotors - i - 1]->return_rotor_error();
      return ;
    }
  }
  if (number_of_rotors > 0)
    create_starting_positions(argv[argc-1]);
}

bool Enigma::is_error() {
  if (error != NO_ERROR)
    return true;

  return false;
}

int Enigma::return_error() { return error; }

void Enigma::encrypt_letter(const char ch) {

  int encrypted_letter = ch - 'A';

  if (number_of_rotors > 0)
    rotors_rotate();

  pb->scrambling(encrypted_letter);

  if (number_of_rotors > 0)
    for (int i = 0; i < number_of_rotors; i++)
      rot[i]->forward_scrambling(encrypted_letter);

  ref->scrambling(encrypted_letter);
  if (number_of_rotors > 0)
    for (int i = number_of_rotors - 1; i >= 0; i--)
      rot[i]->backward_scrambling(encrypted_letter);

  pb->scrambling(encrypted_letter);

  cout << static_cast<char>(encrypted_letter + 'A');
}

void Enigma::rotors_rotate() {
  rot[0]->rotor_rotate();
  for(int i = 0; i < number_of_rotors - 1; i++) {
    if (rot[i]->is_notch_hit())
      rot[i+1]->rotor_rotate();
    else
      break ;
  }
}

void Enigma::set_initial_rotors_position() {
  for (int i = 0; i < number_of_rotors; i++)
    for (int j = 0; j < rot[i]->get_starting_position(); j++)
      rot[i]->rotor_rotate();
}

void Enigma::encrypt_message() {
  set_initial_rotors_position();
  char ch;
  cin >> ch;
  while (!cin.eof()) {
    if (cin.fail() || (ch < 'A') || (ch > 'Z')) {
      cerr << ch << " is not a valid input character (input characters must be upper case letters A-Z)!" << endl;
      error = INVALID_INPUT_CHARACTER;
      return ;
    }
    encrypt_letter(ch);
    cin >> ch;
  }
}

void Enigma::create_starting_positions(const char* filename) {
  ifstream in_stream;
  int number, count;

  count = 0;

  in_stream.open(filename);

  if (in_stream.fail()) {
    in_stream.close();
    cerr << "The program encountered an error opening or reading the rotors' starting position configuration file " << filename << endl;
    error = ERROR_OPENING_CONFIGURATION_FILE;
    return ;
  }

  in_stream >> number;

  while (!in_stream.eof()) {

    if (in_stream.fail()) {
      in_stream.close();
      cerr << "Non-numeric character in rotor positions file " << filename << endl;
      error = NON_NUMERIC_CHARACTER;
      return ;
    }

    if (is_invalid_index(number)) {
      in_stream.close();
      cerr << "The rotors' starting position file " << filename << " contains a number that is not between 0 and 25" << endl;
      error = INVALID_INDEX;
      return ;
    }

    if (number_of_rotors > count) {
      rot[number_of_rotors - 1 - count]->set_starting_position(number);
      count++;
    }
    else {
      count++;
      break;
    }
    in_stream >> number;
  }

  if (number_of_rotors > count) {
    in_stream.close();
    cerr << "No starting position for rotor " << number_of_rotors - count - 1 << " in rotor position file: " << filename << endl;
    error = NO_ROTOR_STARTING_POSITION;
    return ;
  }

  if (number_of_rotors < count) {
    in_stream.close();
    cerr << "More positions specified than number of rotors in the rotor potion file " << filename << endl;
    error = NO_ROTOR_STARTING_POSITION;
    return ;
  }
  in_stream.close();
}

bool Enigma::is_invalid_index(int number) {
  if ((number < 0) || (number > 25))
    return true;

  return false;
}
