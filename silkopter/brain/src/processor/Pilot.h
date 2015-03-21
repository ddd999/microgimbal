#pragma once

#include "common/node/processor/IPilot.h"
#include "common/node/stream/IAngular_Velocity_Stream.h"
#include "common/node/stream/IFrame_Stream.h"
#include "common/node/stream/IBattery_State_Stream.h"
#include "common/node/stream/ICommands_Stream.h"
#include "Comms.h"
#include "HAL.h"

namespace sz
{
namespace Pilot
{
struct Init_Params;
struct Config;
}
}

namespace silk
{
namespace node
{

class Pilot : public IPilot
{
public:
    Pilot(HAL& hal);

    auto init(rapidjson::Value const& init_params) -> bool;
    auto get_init_params() const -> rapidjson::Document const&;

    auto set_config(rapidjson::Value const& json) -> bool;
    auto get_config() const -> rapidjson::Document;

    auto get_inputs() const -> std::vector<Input>;
    auto get_outputs() const -> std::vector<Output>;

    void process();

private:
    auto init() -> bool;

    HAL& m_hal;

    rapidjson::Document m_init_paramsj;
    std::shared_ptr<sz::Pilot::Init_Params> m_init_params;
    std::shared_ptr<sz::Pilot::Config> m_config;

    q::Clock::duration m_dt = q::Clock::duration(0);

    IAngular_Velocity_Stream_wptr m_angular_velocity_stream;
    //ILocation_wptr m_location_stream;
    IBattery_State_Stream_wptr m_battery_state_stream;
    ICommands_Stream_wptr m_commands_stream;

    std::vector<IAngular_Velocity_Stream::Sample> m_angular_velocity_samples;
    //std::vector<ILocation::Sample> m_location_samples;
    std::vector<IBattery_State_Stream::Sample> m_battery_state_samples;
    std::vector<ICommands_Stream::Sample> m_commands_samples;

    struct Stream : public IFrame_Stream
    {
        auto get_samples() const -> std::vector<Sample> const& { return samples; }
        auto get_rate() const -> uint32_t { return rate; }


        Sample last_sample;
        std::vector<Sample> samples;
        uint32_t rate = 0;
    };
    std::shared_ptr<Stream> m_output_stream;
};



}
}
