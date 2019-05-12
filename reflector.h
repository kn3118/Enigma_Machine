#ifndef REFLECTOR_H
#define REFLECTOR_H

class Reflector {
 private:
  static const int MAX_SIZE = 26;
  int mapping[MAX_SIZE];
  int input_number;
  int output_number;
  int error;

 public:
  Reflector();
  ~Reflector();
  void create_mapping(const char* filename); //Read the reflector configuration file and create the appropriate mapping, checking for errors too
  bool is_invalid_index(int number1, int number2);
  bool is_invalid_reflector_mapping(int number1, int number2);
  bool is_incorrect_number_of_reflector_pairs(int count);
  bool is_error();
  int return_reflector_error();
  void config_scrambling(); //Produce the mapped number corresponding to the input number
  void scrambling(int& number); //Receive a number (corresponding to a letter) and turn it into its correct mapping
};

#endif
