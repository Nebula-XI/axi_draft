#include "test_units_info.h"
#include "units_dev.h"

using namespace InSys;

static const std::string_view g_config{"TODO: add configuration"};

int main(int argc, char *argv[]) try {
  unit_info_i2c_parser info_i2c_parser{g_config};
  auto info_axi_i2c_list =
      info_i2c_parser.get_info<unit_info_i2c_parser::axi_parser>();
  auto info_i2c_mux_list =
      info_i2c_parser.get_info<unit_info_i2c_parser::mux_parser>();
  auto info_i2c_dev_list =
      info_i2c_parser.get_info<unit_info_i2c_parser::dev_parser>();

  unit_dev_axi_i2c dev_axi_i2c{info_axi_i2c_list[0]};
  unit_dev_i2c_mux dev_i2c_mux{info_i2c_mux_list[0], 0, dev_axi_i2c};
  unit_dev_i2c dev_i2c{info_i2c_dev_list[0], dev_i2c_mux};

  dev_i2c.read();
  dev_i2c.write();

  return EXIT_SUCCESS;
} catch (const std::exception &e) {
  std::cerr << e.what() << std::endl;
  return EXIT_FAILURE;
} catch (...) {
  std::cerr << "unexcepted exception" << std::endl;
  return EXIT_FAILURE;
}
