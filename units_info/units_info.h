#pragma once

#include <string>
#include <utility>
#include <vector>

namespace InSys {

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
  const std::size_t unit_id{};  //< уникальный идентификатор узла
  const std::string name{};  //< имя узла
  const std::size_t axi_offset{};  //< смещение в адресном пространстве
};

}  // namespace InSys
