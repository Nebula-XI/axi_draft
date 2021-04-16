#include "test_units_info.h"
#include "units_dev.h"

using namespace InSys;

static const std::string_view g_config{"TODO: add configuration"};

int main(int argc, char *argv[]) try {
  unit_info_axi_i2c_parser info_axi_i2c_parser{g_config};
  auto info_axi_i2c_list = info_axi_i2c_parser.get_info();
  unit_dev_axi_i2c::list_type dev_axi_i2c_list{};
  for (const auto &info : info_axi_i2c_list) {
    dev_axi_i2c_list.emplace_back(unit_dev_axi_i2c{info});
  }
  for (auto &dev_axi_i2c : dev_axi_i2c_list) {
    print_line();
    print_info(dev_axi_i2c.get_info());
    dev_axi_i2c.read();
    dev_axi_i2c.write();
  }

  return EXIT_SUCCESS;
} catch (const std::exception &e) {
  std::cerr << e.what() << std::endl;
  return EXIT_FAILURE;
} catch (...) {
  std::cerr << "unexcepted exception" << std::endl;
  return EXIT_FAILURE;
}
