#pragma once

#include "units_info_base.h"

namespace InSys {

/// параметры узла SPI
struct unit_info_spi : unit_info_base {
  unit_info_spi() = delete;  //< контруктор по умолчанию
  unit_info_spi(const std::string &_name, std::size_t _axi_offset,
                int _chip_select, double _clock)
      : unit_info_base{_name, _axi_offset},
        chip_select{_chip_select},
        clock{_clock} {}    //< размещающий
                            //конструктор
  const int chip_select{};  //< выбор кристала
  const double clock{};     //< частота
  inline static const std::string unit{"spi"};
  std::string to_config() const {
    std::stringstream config{};
    config << unit_info_base::to_config();
    config << "unit = " << unit << '\n';
    config << "cs = 0x" << std::hex << chip_select << '\n';
    config << "clk = " << std::dec << clock << '\n';
    return config.str();
  }
};

/// список параметров узлов SPI
using units_info_spi_list = std::vector<unit_info_spi>;

/// класс формирователь информации об узлах SPI
class units_info_spi : public units_info_base<units_info_spi_list> {
 public:
  /// конструктор
  units_info_spi(const std::string &config) {
    // TODO: add configuration parser
    _info_list.emplace_back("LMX2594", 0x00000600, 1,
                            10_MHz);  // FIXME: пример
    _info_list.emplace_back("LTC6953", 0x00000800, 2,
                            10_MHz);  // FIXME: пример
    _info_list.emplace_back("LMX2594", 0x00001200, 3,
                            20_MHz);  // FIXME: пример
  }
};

}  // namespace InSys
