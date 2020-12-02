#include <iostream>

int main(int argc, const char* argv[])
{
  std::cout << "Hello World!" << std::endl;

  for (size_t i = 1; i < argc; i++)
    std::cout << argv[i] << std::endl;

  return 0;
}
