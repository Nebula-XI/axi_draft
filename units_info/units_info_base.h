#pragma once

#include <algorithm>
#include <map>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace InSys {

using unit_info_uid = std::size_t;
class make_unit_info_uid {
  std::string _str{};
  unit_info_uid operator()() { return std::hash<std::string>{}(_str); }

 public:
  template <typename Arg = std::string, typename... Args>
  unit_info_uid operator()(Arg str, Args... other_str) {
    _str += str;
    return operator()(other_str...);
  }
};

struct unit_info_base {
  unit_info_base() = delete;
  unit_info_base(const std::string_view &_name, const std::string_view &_label,
                 unit_info_uid _parent_uid, const std::string_view &_unit)
      : uid{make_unit_info_uid{}(_name, _label)},
        name{_name},
        label{_label},
        parent_uid{_parent_uid},
        unit{_unit} {}
  virtual ~unit_info_base() noexcept = default;
  const unit_info_uid uid{};
  const std::string name{};
  const std::string label{};
  const unit_info_uid parent_uid{};
  const std::string unit{};
};

struct unit_info_axi_base : unit_info_base {
  unit_info_axi_base(const std::string_view &_name,
                     const std::string_view &_label, unit_info_uid _axi_offset,
                     const std::string_view &_unit)
      : unit_info_base{_name, _label, {}, _unit}, axi_offset{_axi_offset} {}
  virtual ~unit_info_axi_base() noexcept = default;
  const unit_info_uid axi_offset{};
};

template <typename unit_info_type>
using units_info_list = std::vector<unit_info_type>;

template <template <typename> typename units_info_list, typename unit_info_type>
class units_info_base_parser {
 public:
  using list_type = units_info_list<unit_info_type>;
  using value_type = unit_info_type;
  units_info_base_parser() = default;
  virtual ~units_info_base_parser() noexcept = default;
  list_type get_info() const { return _info_list; }
  list_type find_by_name(const std::string_view &name) const {
    list_type info_list{};
    for (const auto &info : _info_list) {
      if (info.name == name) {
        info_list.push_back(info);
      }
    }
    return info_list;
  }
  std::optional<value_type> get_by_label(const std::string_view &label) const {
    std::optional<value_type> result{};
    for (const auto &info : _info_list) {
      if (info.label == label) {
        result.emplace(info);
      }
    }
    return result;
  }
  std::optional<value_type> get_by_uid(unit_info_uid uid) const {
    std::optional<value_type> result{};
    for (const auto &info : _info_list) {
      if (info.uid == uid) {
        result.emplace(info);
      }
    }
    return result;
  }

 protected:
  list_type _info_list{};
  virtual void parser(const std::string_view &config) = 0;
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
