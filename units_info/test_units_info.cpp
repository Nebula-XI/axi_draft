
#include <iostream>

#include "units_info_i2c.h"
#include "units_info_spi.h"
#include "units_info_gpio.h"

using namespace InSys;

void print(const unit_info_i2c &i2c_info) {
  std::cout << "unit name: " << i2c_info.name << '\n';
  std::cout << "unit type: " << i2c_info.unit << '\n';
  std::cout << "unit id: 0x" << i2c_info.id << '\n';
  std::cout << "unit address: 0x" << i2c_info.address << '\n';
  std::cout << "unit axi offset: 0x" << i2c_info.axi_offset << '\n';
  std::cout << std::string(32, '-') << '\n';
}

void print(const unit_info_spi &spi_info) {
  std::cout << "unit name: " << spi_info.name << '\n';
  std::cout << "unit type: " << spi_info.unit << '\n';
  std::cout << "unit id: 0x" << spi_info.id << '\n';
  std::cout << "unit chip select: 0x" << spi_info.chip_select << '\n';
  std::cout << "unit axi offset: 0x" << spi_info.axi_offset << '\n';
  std::cout << std::string(32, '-') << '\n';
}

void print(const unit_info_gpio &gpio_info) {
  std::cout << "unit name: " << gpio_info.name << '\n';
  std::cout << "unit type: " << gpio_info.unit << '\n';
  std::cout << "unit id: 0x" << gpio_info.id << '\n';
  std::cout << "unit axi offset: 0x" << gpio_info.axi_offset << '\n';
  std::cout << std::string(32, '-') << '\n';
}

int main(int argc, char *argv[]) try {

  std::string config{"blablabla"};

  // делаем разбор конфигурации
  units_info_i2c units_i2c{config};
  units_info_spi units_spi{config};
  units_info_gpio units_gpio{config};

  std::cout << std::string(32, '-') << '\n' << std::hex;

  // получаем описание всех узлов I2C
  auto i2c_info_list = units_i2c.get_info();
  // обходим все узлы I2C
  for (const auto &i2c_info : i2c_info_list) {
    // выводим описание каждого узла в консоль
    print(i2c_info);
  }

  // получаем описание всех узлов SPI
  auto spi_info_list = units_spi.get_info();
  // обходим все узлы SPI
  for (const auto &spi_info : spi_info_list) {
    // выводим описание каждого узла в консоль
    print(spi_info);
  }

  // получаем описание всех узлов GPIO
  auto gpio_info_list = units_gpio.get_info();
  // обходим все узлы GPIO
  for (const auto &gpio_info : gpio_info_list) {
    // выводим описание каждого узла в консоль
    print(gpio_info);
  }

  // формируем ID для INA218
  auto ina218_id = units_info_make_id("INA218");
  // поиск всех узлов с соответствующим ID
  auto ina218_info_list = units_i2c.find_info(ina218_id);
  // обходим все найденные узлы
  for (const auto &ina218_info : ina218_info_list) {
    // выводим описание узла в консоль
    print(ina218_info);
  }
  return EXIT_SUCCESS;
} catch (const std::exception &e) {
  std::cerr << e.what() << std::endl;
  return EXIT_FAILURE;
} catch (...) {
  std::cerr << "unexcepted exception" << std::endl;
  return EXIT_FAILURE;
}
