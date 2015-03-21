#pragma once

#include "common/node/processor/IProcessor.h"
#include "common/node/stream/IFrame_Stream.h"
#include "common/node/stream/IAngular_Velocity_Stream.h"
#include "HAL.h"


namespace sz
{
namespace Stability_Processor
{
struct Init_Params;
struct Config;
}
}



namespace silk
{
namespace node
{

class Stability_Processor : public IProcessor
{
public:
    Stability_Processor(HAL& hal);

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
    std::shared_ptr<sz::Stability_Processor::Init_Params> m_init_params;
    std::shared_ptr<sz::Stability_Processor::Config> m_config;

    q::Clock::duration m_dt = q::Clock::duration(0);

    IFrame_Stream_wptr m_input_stream;
    IFrame_Stream_wptr m_target_stream;

    std::vector<IFrame_Stream::Sample> m_input_samples;
    std::vector<IFrame_Stream::Sample> m_target_samples;

    struct Stream : public IAngular_Velocity_Stream
    {
        auto get_samples() const -> std::vector<Sample> const& { return samples; }
        auto get_rate() const -> uint32_t { return rate; }

        Sample last_sample;
        std::vector<Sample> samples;
        uint32_t rate = 0;
    };
    mutable std::shared_ptr<Stream> m_output_stream;
};



}
}
