#pragma once

#include "units_info.h"

namespace InSys {

/// параметры узла SPI
struct unit_info_spi : unit_info_base {
  unit_info_spi() = delete;  //< контруктор по умолчанию
  unit_info_spi(const std::string &_name, std::size_t _axi_offset,
                int _chip_select)
      : unit_info_base{_name, _axi_offset},
        chip_select{_chip_select} {}  //< размещающий
                                      //конструктор
  const int chip_select{};            //< выбор кристала
  inline static const char unit[]{"spi"};
};

/// список параметров узлов SPI
using units_info_spi_list = std::vector<unit_info_spi>;

/// класс формирователь информации об устройствах
class units_info_spi final {
  units_info_spi_list _info_list{};

 public:
  /// конструктор
  units_info_spi(const std::string &config) {
    // TODO: add configuration parser
    _info_list.emplace_back("LMX2594", 0x00000600,
                            1);  // FIXME: пример
    _info_list.emplace_back("LTC6953", 0x00000800,
                            2);  // FIXME: пример
  }
  units_info_spi_list get_info() const { return _info_list; }

  units_info_spi_list find_info(std::size_t id) const {
    units_info_spi_list info_list{};
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
