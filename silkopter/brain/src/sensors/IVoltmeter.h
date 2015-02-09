#pragma once

#include "common/sensors/Sensor_Samples.h"

namespace silk
{
namespace sensors
{

class IVoltmeter
{
public:
    virtual ~IVoltmeter() {}

    virtual auto get_voltmeter_name() const -> q::String const& = 0;

    struct Config
    {
        float bias;
        float scale = 1;
    };

    virtual void set_voltmeter_config(Config const& config) = 0;
    virtual auto get_voltmeter_config() const -> Config const& = 0;

    virtual auto get_voltmeter_samples() const -> std::vector<Voltmeter_Sample> const& = 0;
};

DECLARE_CLASS_PTR(IVoltmeter);

}
}
