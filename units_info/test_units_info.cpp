
#include "units_info.h"

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
  // делаем разбор конфигурации
  units_info units{"blablabla"};

  std::cout << std::string(32, '-') << '\n' << std::hex;

  // получаем описание всех узлов I2C
  auto i2c_info_list = units.get_i2c_info();
  // обходим все узлы I2C
  for (const auto &i2c_info : i2c_info_list) {
    // выводим описание каждого узла в консоль
    print(i2c_info);
  }

  // получаем описание всех узлов SPI
  auto spi_info_list = units.get_spi_info();
  // обходим все узлы SPI
  for (const auto &spi_info : spi_info_list) {
    // выводим описание каждого узла в консоль
    print(spi_info);
  }

  // получаем описание всех узлов GPIO
  auto gpio_info_list = units.get_gpio_info();
  // обходим все узлы GPIO
  for (const auto &gpio_info : gpio_info_list) {
    // выводим описание каждого узла в консоль
    print(gpio_info);
  }

  // получаем описание всех узлов
  auto units_info_list = units.get_info();
  // обходим все узлы
  for (const auto &unit_info : units_info_list) {
    // если узел имее тип I2C
    if (unit_info.type() == typeid(unit_info_i2c)) {
      auto i2c_info = unit_info_cast<unit_info_i2c>(unit_info);
      // выводим описание узла в консоль
      print(i2c_info);
    }
    // если узел имее тип SPI
    if (unit_info.type() == typeid(unit_info_spi)) {
      auto spi_info = unit_info_cast<unit_info_spi>(unit_info);
      // выводим описание узла в консоль
      print(spi_info);
    }
    // если узел имее тип GPIO
    if (unit_info.type() == typeid(unit_info_gpio)) {
      auto gpio_info = unit_info_cast<unit_info_gpio>(unit_info);
      // выводим описание узла в консоль
      print(gpio_info);
    }
  }

  // формируем ID для INA218
  auto ina218_id = units_info_make_id("INA218");
  // поиск всех узлов с соответствующим ID
  auto ina218_info_list = units.find_info(ina218_id);
  // обходим все найденные узлы
  for (const auto &ina218_info : ina218_info_list) {
    // если тип не соответствует ожидаемому, то пропускаем
    if (ina218_info.type() != typeid(unit_info_i2c)) {
      continue;
    }
    auto i2c_info = unit_info_cast<unit_info_i2c>(ina218_info);
    // выводим описание узла в консоль
    print(i2c_info);
  }
  return EXIT_SUCCESS;
} catch (const std::exception &e) {
  std::cerr << e.what() << std::endl;
  return EXIT_FAILURE;
} catch (...) {
  std::cerr << "unexcepted exception" << std::endl;
  return EXIT_FAILURE;
}
