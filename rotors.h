#ifndef ROTORS_H
#define ROTORS_H

class Rotor {
 private:
  static const int MAX_SIZE = 26;
  int config_forward_mapping[MAX_SIZE];
  int config_backward_mapping[MAX_SIZE];
  int notch[MAX_SIZE];
  int starting_position;
  int difference[MAX_SIZE];
  int input_number;
  int mapped_number; 
  int rotations_count;
  int error;

 public:
  Rotor();
  ~Rotor();
  void create_mapping(const char* filename); //Read the rotors configuration file and create the appropriate mapping, checking for errors too
  bool is_invalid_index(int number);
  bool is_invalid_rotor_mapping(int number, int count);
  bool is_repeated_notch(int number);
  bool is_error();
  int return_rotor_error();
  void set_starting_position(int number);
  int get_starting_position();
  void rotation_difference_mapping(); //Create the difference[26] matrix by subtracting the index (input) number from the mapped number
  void update_difference_mapping(); //Shift all differences in the difference[26] matrix one place up to enable for the new rotors' mapping
  void update_config_mapping(); //Update the forward and backward mapping after a rotors' rotation
  void rotor_rotate(); //Perform a rotors' rotation, by updating the difference mapping, updating the configuration mapping and incrementing the rotations count by 1
  void config_forward_scrambling(); //Produce the mapped number corresponding to the input number as the letter travels forward (towards the reflector)
  void config_backward_scrambling(); //Produce the mapped number corresponding to the input number as the letter travels backwards (towards the plugboard)
  void forward_scrambling(int& number); //Receive a number (corresponding to a letter) and turn it into its correct forward mapping
  void backward_scrambling(int& number); //Receive a number (corresponding to a letter) and turn it into its correct backward mapping
  bool is_notch_hit(); //Return true if a rotor hits a notch
};

#endif
