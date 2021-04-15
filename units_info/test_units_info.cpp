
#include <iostream>

#include "units_info.h"
#include "units_info_ddr.h"
#include "units_info_gpio.h"
#include "units_info_i2c.h"
#include "units_info_spi.h"

using namespace InSys;
using namespace std::string_literals;

void print_line(char ch = '-', int size = 40) {
  std::cout << std::string(size, ch) << '\n';
}

void print_info_list_empty() { std::cout << "unit list is empty" << '\n'; }

void print_info(const unit_info_base &info) {
  std::cout << "name: " << info.name << '\n';
  std::cout << "label: " << info.label << '\n';
  std::cout << "unit: " << info.unit << '\n';
  std::cout << "uid: 0x" << std::hex << info.uid << '\n';
  std::cout << "parent uid: 0x" << std::hex << info.parent_uid << '\n';
}

void print_info(const unit_info_axi_i2c &info) {
  print_info(static_cast<unit_info_base>(info));
  std::cout << "offset: 0x" << std::hex << info.axi_offset << '\n';
}

void print_info(const unit_info_i2c_dev &info) {
  print_info(static_cast<unit_info_base>(info));
  std::cout << "address: 0x" << std::hex << info.address << '\n';
  std::cout << "frequency: " << std::dec << info.frequency << '\n';
}

void print_info(const unit_info_i2c_mux &info) {
  print_info(static_cast<unit_info_i2c_dev_base>(info));
  for (const auto segment : info.segments) {
    std::cout << "segment uid[" << std::dec << segment.second << "]: 0x"
              << std::hex << segment.first << '\n';
  }
}

template <typename parser>
void example(std::string_view &&desc, const std::string_view &config) {
  print_line();
  std::cout << desc << ":\n";
  parser unit_info{config};
  auto unit_info_list = unit_info.get_info();
  if (unit_info_list.empty()) {
    print_line();
    print_info_list_empty();
  } else
    for (const auto &info : unit_info_list) {
      print_line();
      print_info(info);
    }
}

// описание конфигурации
static const std::string_view g_config{"TODO: add configuration"};

int main(int argc, char *argv[]) try {
  example<unit_info_axi_i2c_parser>("EXAMPLE [1] - get info axi-i2c units",
                                    g_config);
  example<unit_info_i2c_mux_parser>("EXAMPLE [2] - get info i2c-mux units",
                                    g_config);
  example<unit_info_i2c_dev_parser>("EXAMPLE [3] - get info i2c-dev units",
                                    g_config);
  return EXIT_SUCCESS;
} catch (const std::exception &e) {
  std::cerr << e.what() << std::endl;
  return EXIT_FAILURE;
} catch (...) {
  std::cerr << "unexcepted exception" << std::endl;
  return EXIT_FAILURE;
}
