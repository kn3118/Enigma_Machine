#ifndef ENIGMA_H
#define ENIGMA_H

class Plugboard;
class Rotor;
class Reflector;

class Enigma {
 private:
  Plugboard* pb;
  Reflector* ref;
  int number_of_rotors;
  Rotor** rot;
  int error;

 public:
  Enigma();
  ~Enigma();
  void set_components(int argc, char** argv); //Set up the right components for the enigma machine based on the command line arguments provided
  void create_starting_positions(const char* filename); //Read the starting positions file and save the right starting position for each rotor
  bool is_error();
  bool is_invalid_index(int number);
  int return_error();
  void encrypt_message(); //Start reading and encrypting the letters provided in a loop
  void encrypt_letter(const char ch); //Receive an input letter, perform the necessary scrambling and output the encrypted letter
  void rotors_rotate(); //Make one rotors' rotation accounting for notches too
  void set_initial_rotors_position(); //Set the initial starting position for each rotor
};

#endif
