#include <iostream>
using namespace std;

#include "contrastmasking.h"
#include "contrastsensitivity.h"
#include "psychometricfunction.h"

int main()
{
  SarkisonCSF::ParameterType pcsf;
  pcsf.resize(3); pcsf << -15.13, 0.0096, 0.64;

  SarkisonCSF csf;
  csf.setParameters(pcsf);

  std::cout << "sarkison csf;" << std::endl;
  for(unsigned int i=0; i<30; ++i)
    std::cout << csf(double(i)) << std::endl;

  return 0;
}
