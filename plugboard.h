#ifndef PLUGBOARD_H
#define PLUGBOARD_H

class Plugboard {
 private:
  static const int MAX_SIZE = 26;
  int config_mapping[MAX_SIZE];
  int input_number;
  int mapped_number;
  int error;
  
 public:
  Plugboard();
  ~Plugboard();
  void create_mapping(const char* filename); //Read the plugboard configuration file and create the appropriate mapping, checking for errors too
  bool is_invalid_index(int number1, int number2);
  bool is_impossible_plugboard_configuration(int number1, int number2);
  bool is_odd_plugboard_parameters(int count);
  bool is_above_possible(int count);
  bool is_error();
  int return_plugboard_error();
  void config_scrambling(); //Produce the mapped number corresponding to the input number
  void scrambling(int& number); //Receive a number (corresponding to a letter) and turn it into its correct mapping
};

#endif
