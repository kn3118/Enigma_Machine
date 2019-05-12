#include "enigma.h"
#include "errors.h"
#include <iostream>

using namespace std;

int main(int argc, char** argv) {

  Enigma en;
  
  en.set_components(argc, argv); //Read the command line arguments and set up the right components
  if (en.is_error())
    return en.return_error();

  en.encrypt_message(); //Start encrypting message

  return en.return_error();
}
