
#include "dev_axi.h"
#include "dev_gpio.h"
#include "dev_i2c.h"
#include "dev_sdram.h"
#include "dev_spi.h"
#include "ina21x.h"
#include "test_info.h"

using namespace InSys;

static const std::string_view g_config{"TODO: add configuration"};

int main(int argc, char *argv[]) try {
  return EXIT_SUCCESS;
} catch (const std::exception &e) {
  std::cerr << e.what() << std::endl;
  return EXIT_FAILURE;
} catch (...) {
  std::cerr << "unexcepted exception" << std::endl;
  return EXIT_FAILURE;
}
