
#pragma once

#include <string>
#include <vector>
#include <cstring>
#include <memory>

namespace InSys
{

#define ALIGN_UP(value, align) (((value) & (align - 1)) ? (((value) + (align - 1)) & ~(align - 1)) : (value))

//-----------------------------------------------------------------------------
//  Класс для работы с физическим PCIe устройством. Выполняет отоборажение BAR
//  в адресное пространство процесса и запись чтение регистров из этих областей
//-----------------------------------------------------------------------------
    class dev_axi
    {
    public:
        dev_axi(const std::string &name);
        virtual ~dev_axi();
        bool is_valid() { return _valid; }

        //TODO: Добавить проверку на диапазон и т.д Все смещения в байтах.
        bool write_xdma(uint32_t offset, uint32_t reg, uint32_t value) { _xdma_bar[(offset + reg) >> 2] = value; };
        bool read_xdma(uint32_t offset, uint32_t reg, uint32_t& value) { value = _xdma_bar[(offset + reg) >> 2]; };

        bool write_axi(uint32_t offset, size_t reg, size_t value) { _axi_bar[(offset + reg) >> 2] = value; };
        bool read_axi(uint32_t offset, uint32_t reg, uint32_t& value){ value = _axi_bar[(offset + reg) >> 2]; };

        template<typename T> T* axi_region_address() { return static_cast<T*>(_axi_bar); }
        uint32_t axi_region_size() { return _axi_bar_size; }

        template<typename T> T* xdma_region_address() { return static_cast<T*>(_xdma_bar); }
        uint32_t xdma_region_size() { return _xdma_bar_size; }

    private:
        dev_axi() = default;
        std::string _name;
        bool _valid;
        uint32_t* _axi_bar;
        uint32_t _axi_bar_size;
        uint32_t* _xdma_bar;
        uint32_t _xdma_bar_size;
    };
    using devaxi_t = std::shared_ptr<dev_axi>;
    inline devaxi_t get_axi_devce(const std::string &name) { return std::make_shared<dev_axi>(name); }

//-----------------------------------------------------------------------------
//  Запись и чтение конфигурации BRAM блока MAIN физического PCIe устройства.
//-----------------------------------------------------------------------------
    class axi_config
    {
    public:
        axi_config(devaxi_t hw) { _hw = hw; };
        virtual ~axi_config();
        bool is_valid() { return _valid; }

        bool write_configuration(const std::string &config_file);
        bool save_configuration(const std::string &config_file);
        bool hw_configuration(std::string& bram_config);

    private:
        bool process_config_file(const std::string& fname, std::string& config_data);
        axi_config() = default;
        std::string _space;
        bool _valid;
        devaxi_t _hw;
    };
    using axi_config_t = std::shared_ptr<axi_config>;
    inline axi_config_t get_axi_config(devaxi_t hw) { return std::make_shared<axi_config>(hw); }    
}