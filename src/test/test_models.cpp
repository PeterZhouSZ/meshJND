#include <iostream>
using namespace std;

#include "contrastmasking.h"
#include "contrastsensitivity.h"
#include "psychometricfunction.h"

int main()
{
  SarkisonCSF csf;

  std::cout << "sarkison csf;" << std::endl;
  for(unsigned int i=0; i<30; ++i)
    std::cout << csf(double(i)) << std::endl;

  return 0;
}
