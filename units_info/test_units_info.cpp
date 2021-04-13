
#include <iostream>

#include "units_info_gpio.h"
#include "units_info_i2c.h"
#include "units_info_spi.h"

using namespace InSys;

void print(const unit_info_i2c &info) {
  std::cout << "unit name: " << info.name << '\n';
  std::cout << "unit type: " << info.unit << '\n';
  std::cout << "unit id: 0x" << std::hex << info.id << '\n';
  std::cout << "unit address: 0x" << info.address << '\n';
  std::cout << "unit frequency: " << std::dec << info.frequency << '\n';
  std::cout << "unit axi offset: 0x" << std::hex << info.axi_offset << '\n';
  std::cout << std::string(32, '~') << '\n';
  std::cout << info.to_config() << '\n';
  std::cout << std::string(32, '-') << '\n';
}

void print(const unit_info_spi &info) {
  std::cout << "unit name: " << info.name << '\n';
  std::cout << "unit type: " << info.unit << '\n';
  std::cout << "unit id: 0x" << std::hex << info.id << '\n';
  std::cout << "unit chip select: 0x" << info.chip_select << '\n';
  std::cout << "unit clock: " << std::dec << info.clock << '\n';
  std::cout << "unit axi offset: 0x" << std::hex << info.axi_offset << '\n';
  std::cout << std::string(32, '~') << '\n';
  std::cout << info.to_config() << '\n';
  std::cout << std::string(32, '-') << '\n';
}

void print(const unit_info_gpio &info) {
  std::cout << "unit name: " << info.name << '\n';
  std::cout << "unit type: " << info.unit << '\n';
  std::cout << "unit id: 0x" << std::hex << info.id << '\n';
  std::cout << "unit axi offset: 0x" << info.axi_offset << '\n';
  std::cout << std::string(32, '~') << '\n';
  std::cout << info.to_config() << '\n';
  std::cout << std::string(32, '-') << '\n';
}

int main(int argc, char *argv[]) try {
  std::cout << std::string(32, '-') << '\n';

  // описание конфигурации
  std::string config{"blablabla"};
  // делаем разбор конфигурации для узлов I2C, SPI, GPIO
  units_info_i2c units_info_i2c{config};
  units_info_spi units_info_spi{config};
  units_info_gpio units_info_gpio{config};

  // получаем описание всех узлов I2C
  auto i2c_info_list = units_info_i2c.get_info();
  // обходим все узлы I2C
  for (const auto &info : i2c_info_list) {
    // выводим описание каждого узла I2C в консоль
    print(info);
  }

  // получаем описание всех узлов SPI
  auto spi_info_list = units_info_spi.get_info();
  // обходим все узлы SPI
  for (const auto &info : spi_info_list) {
    // выводим описание каждого узла SPI в консоль
    print(info);
  }

  // получаем описание всех узлов GPIO
  auto gpio_info_list = units_info_gpio.get_info();
  // обходим все узлы GPIO
  for (const auto &info : gpio_info_list) {
    // выводим описание каждого узла GPIO в консоль
    print(info);
  }

  // формируем ID для INA218
  auto ina218_id = units_info_make_id("INA218");
  // поиск всех узлов с соответствующим ID
  auto ina218_info_list = units_info_i2c.find_info(ina218_id);
  // обходим все найденные узлы
  for (const auto &info : ina218_info_list) {
    // выводим описание узла в консоль
    print(info);
  }

  // поиск всех узлов с соответствующим именем
  auto lmx2594_info_list = units_info_spi.find_info("LMX2594");
  // обходим все найденные узлы
  for (const auto &info : lmx2594_info_list) {
    // выводим описание узла в консоль
    print(info);
  }

  return EXIT_SUCCESS;
} catch (const std::exception &e) {
  std::cerr << e.what() << std::endl;
  return EXIT_FAILURE;
} catch (...) {
  std::cerr << "unexcepted exception" << std::endl;
  return EXIT_FAILURE;
}
