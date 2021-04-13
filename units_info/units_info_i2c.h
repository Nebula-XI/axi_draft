#pragma once

#include "units_info.h"

namespace InSys {

/// параметры узла I2C
struct unit_info_i2c : unit_info_base {
  unit_info_i2c() = delete;  //< контруктор по умолчанию
  unit_info_i2c(const std::string &_name, std::size_t _axi_offset, int _address,
                double _frequency)
      : unit_info_base{_name, _axi_offset},
        address{_address},
        frequency{_frequency} {}  //< размещающий
                                  //конструктор
  const int address{};            //< I2C адрес
  const double frequency{};       //< I2C частота
  inline static const char unit[]{"i2c"};
};

/// список параметров узлов I2C
using units_info_i2c_list = std::vector<unit_info_i2c>;

/// класс формирователь информации об устройствах
class units_info_i2c final {
  units_info_i2c_list _info_list{};

 public:
  /// конструктор
  units_info_i2c(const std::string &config) {
    // TODO: add configuration parser
    _info_list.emplace_back("INA218", 0x00000200, 0x48,
                            200'000);  // FIXME: пример
    _info_list.emplace_back("LTC2991", 0x00000400, 0x53,
                            200'000);  // FIXME: пример
  }
  units_info_i2c_list get_info() const { return _info_list; }
  units_info_i2c_list find_info(std::size_t id) const {
    units_info_i2c_list info_list{};
    for (const auto &info : _info_list) {
      if (info.id != id) {
        continue;
      }
      info_list.push_back(info);
    }
    return info_list;
  }
};

}  // namespace InSys
