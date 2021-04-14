#pragma once

#include <optional>
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
inline auto units_info_make_id(const std::string &name,
                               const std::string &lable) noexcept {
  return std::hash<std::string>{}(name + lable);
}

/// базовая информация присущая всем узлам
struct unit_info_base {
  unit_info_base() = delete;  //< контруктор по умолчанию
  unit_info_base(const std::string &_name, const std::string &_label,
                 std::size_t _axi_offset)
      : id{units_info_make_id(_name, _label)},
        name{_name},
        label{_label},
        axi_offset{_axi_offset} {}  //< размещающий
                                    //конструктор
  virtual ~unit_info_base() noexcept = default;
  const std::size_t id{};  //< уникальный идентификатор узла
  const std::string name{};   //< имя узла
  const std::string label{};  //< метка узла
  const std::size_t axi_offset{};  //< смещение в адресном пространстве
};

/// список параметров узлов
template <typename unit_info_type>
using units_info_list = std::vector<unit_info_type>;

/// класс формирователь информации об устройствах
// template <typename units_info_type>
template <template <typename> typename units_info_list, typename unit_info_type>
class units_info_base {
 public:
  using list_type = units_info_list<unit_info_type>;
  using value_type = unit_info_type;
  units_info_base() = delete;
  virtual ~units_info_base() noexcept = default;
  /// запрос списка параметров узлов
  list_type get_info() const { return _info_list; }
  /// запрос параметров узла по заданному оффсету
  std::optional<value_type> get_info(std::size_t axi_offset) const {
    std::optional<value_type> result{};
    for (const auto &info : _info_list) {
      if (info.axi_offset == axi_offset) {
        result.emplace(info);
        break;
      }
    }
    return result;
  }
  /// поиск узлов по заданному идентификатору
  list_type find_info(std::size_t id) const {
    list_type info_list{};
    for (const auto &info : _info_list) {
      if (info.id != id) {
        continue;
      }
      info_list.push_back(info);
    }
    return info_list;
  }
  /// поиск узлов по заданному имени
  list_type find_info(const std::string &name) const {
    list_type info_list{};
    for (const auto &info : _info_list) {
      if (info.name != name) {
        continue;
      }
      info_list.push_back(info);
    }
    return info_list;
  }

 protected:
  list_type _info_list{};
  units_info_base(const std::string &config) { parser(config); }
  virtual void parser(const std::string config) {
    // TODO: add configuration parser
  }
};

}  // namespace InSys
