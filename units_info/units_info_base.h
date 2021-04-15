#pragma once

#include <map>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace InSys {

class make_units_info_uid {
  std::string _str{};
  std::size_t operator()() { return std::hash<std::string>{}(_str); }

 public:
  template <typename Arg = std::string, typename... Args>
  std::size_t operator()(Arg str, Args... other_str) {
    _str += str;
    return operator()(other_str...);
  }
};

struct unit_info_base {
  unit_info_base() = delete;
  unit_info_base(const std::string_view &_name, const std::string_view &_label,
                 std::size_t _parent_uid, const std::string_view &_unit)
      : uid{make_units_info_uid{}(_name, _label)},
        name{_name},
        label{_label},
        parent_uid{_parent_uid},
        unit{_unit} {}
  virtual ~unit_info_base() noexcept = default;
  const std::size_t uid{};
  const std::string_view name{};
  const std::string_view label{};
  const std::size_t parent_uid{};
  const std::string_view unit{};
};

struct unit_info_axi_base : unit_info_base {
  unit_info_axi_base(const std::string_view &_name,
                     const std::string_view &_label, std::size_t _axi_offset,
                     const std::string_view &_unit)
      : unit_info_base{_name, _label, {}, _unit}, axi_offset{_axi_offset} {}
  virtual ~unit_info_axi_base() noexcept = default;
  const std::size_t axi_offset{};
};

template <typename unit_info_type>
using units_info_list = std::vector<unit_info_type>;

template <template <typename> typename units_info_list, typename unit_info_type>
class units_info_base {
 public:
  using list_type = units_info_list<unit_info_type>;
  using value_type = unit_info_type;
  units_info_base() = delete;
  virtual ~units_info_base() noexcept = default;
  list_type get_info() const { return _info_list; }

 protected:
  list_type _info_list{};
  units_info_base(const std::string_view &config) { parser(config); }
  virtual void parser(const std::string_view &config) {
    // TODO: add configuration parser
  }
};

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

}  // namespace InSys
