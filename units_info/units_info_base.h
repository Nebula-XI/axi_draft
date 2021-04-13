#pragma once

#include <sstream>
#include <string>
#include <utility>
#include <vector>

namespace InSys {

constexpr long double operator"" _kHz(long double value) {
  return value * 1000;
}
constexpr long double operator"" _MHz(long double value) {
  return value * 1000000;
}
constexpr long double operator"" _GHz(long double value) {
  return value * 1000000000;
}

constexpr unsigned long long operator"" _kHz(unsigned long long value) {
  return value * 1000;
}
constexpr unsigned long long operator"" _MHz(unsigned long long value) {
  return value * 1000000;
}
constexpr unsigned long long operator"" _GHz(unsigned long long value) {
  return value * 1000000000;
}

/// функция формирования уникального идентификатора узла
inline auto units_info_make_id(const std::string &name) noexcept {
  return std::hash<std::string>{}(name);
}

/// базовая информация присущая всем узлам
struct unit_info_base {
  unit_info_base() = delete;  //< контруктор по умолчанию
  unit_info_base(const std::string &_name, std::size_t _axi_offset)
      : id{units_info_make_id(_name)},
        name{_name},
        axi_offset{_axi_offset} {}  //< размещающий
                                    //конструктор
  virtual ~unit_info_base() noexcept = default;
  const std::size_t id{};  //< уникальный идентификатор узла
  const std::string name{};  //< имя узла
  const std::size_t axi_offset{};  //< смещение в адресном пространстве
  std::string to_config() const {
    std::stringstream config{};
    config << '[' << name << "]\n";
    config << "name = " << name << '\n';
    config << "id = 0x" << std::hex << id << '\n';
    config << "axi_offset = 0x" << axi_offset << '\n';
    return config.str();
  }
};

/// класс формирователь информации об устройствах
template <typename units_info_type>
class units_info_base {
 public:
  using value_type = units_info_type;
  virtual ~units_info_base() noexcept = default;
  value_type get_info() const { return _info_list; }
  value_type find_info(std::size_t id) const {
    value_type info_list{};
    for (const auto &info : _info_list) {
      if (info.id != id) {
        continue;
      }
      info_list.push_back(info);
    }
    return info_list;
  }
  value_type find_info(const std::string &name) const {
    return find_info(units_info_make_id(name));
  }

 protected:
  value_type _info_list{};
  units_info_base() = default;
};

}  // namespace InSys
