#include <cstdlib>
#include <iostream>

#include "units_dev.h"
#include "units_dev_gpio.h"
#include "units_dev_i2c.h"
#include "units_dev_sdram.h"
#include "units_dev_spi.h"

using namespace InSys;

int main(int argc, char *argv[]) try {
  return EXIT_SUCCESS;
} catch (const std::exception &e) {
  std::cerr << e.what() << std::endl;
  return EXIT_FAILURE;
} catch (...) {
  std::cerr << "unexcepted exception" << std::endl;
  return EXIT_FAILURE;
}
