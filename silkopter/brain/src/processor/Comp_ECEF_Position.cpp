#include "BrainStdAfx.h"
#include "Comp_ECEF_Position.h"

#include "sz_math.hpp"
#include "sz_Comp_ECEF_Position.hpp"

namespace silk
{
namespace node
{

Comp_ECEF_Position::Comp_ECEF_Position(HAL& hal)
    : m_hal(hal)
    , m_init_params(new sz::Comp_ECEF_Position::Init_Params())
    , m_config(new sz::Comp_ECEF_Position::Config())
{
    m_position_output_stream = std::make_shared<Position_Output_Stream>();
    m_velocity_output_stream = std::make_shared<Velocity_Output_Stream>();
//    m_enu_frame_output_stream = std::make_shared<ENU_Frame_Stream>();
}

auto Comp_ECEF_Position::init(rapidjson::Value const& init_params) -> bool
{
    QLOG_TOPIC("comp_position::init");

    sz::Comp_ECEF_Position::Init_Params sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, init_params, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize Comp_Position data: {}", ss.str());
        return false;
    }
    *m_init_params = sz;
    return init();
}
auto Comp_ECEF_Position::init() -> bool
{
    if (m_init_params->rate == 0)
    {
        QLOGE("Bad rate: {}Hz", m_init_params->rate);
        return false;
    }
    m_position_output_stream->set_rate(m_init_params->rate);
    m_position_output_stream->set_tp(q::Clock::now());

    m_velocity_output_stream->set_rate(m_init_params->rate);
    m_velocity_output_stream->set_tp(q::Clock::now());

    return true;
}

auto Comp_ECEF_Position::get_inputs() const -> std::vector<Input>
{
    std::vector<Input> inputs =
    {{
        { stream::IECEF_Position::TYPE, m_init_params->rate, "Position (ecef)", m_accumulator.get_stream_path(0) },
        { stream::IECEF_Velocity::TYPE, m_init_params->rate, "Velocity (ecef)", m_accumulator.get_stream_path(1) },
        { stream::IENU_Linear_Acceleration::TYPE, m_init_params->rate, "Linear Acceleration (enu)", m_accumulator.get_stream_path(2) },
        { stream::IPressure::TYPE, m_init_params->rate, "Pressure", m_accumulator.get_stream_path(3) }
    }};
    return inputs;
}
auto Comp_ECEF_Position::get_outputs() const -> std::vector<Output>
{
    std::vector<Output> outputs =
    {{
        { "Position (ecef)", m_position_output_stream },
        { "Velocity (ecef)", m_velocity_output_stream },
    }};
    return outputs;
}

void Comp_ECEF_Position::process()
{
    QLOG_TOPIC("comp_position::process");


    m_position_output_stream->clear();
    m_velocity_output_stream->clear();

    double dts = q::Seconds(m_position_output_stream->get_dt()).count();

    m_accumulator.process([this, dts](
                          size_t,
                          stream::IECEF_Position::Sample const& pos_sample,
                          stream::IECEF_Velocity::Sample const& vel_sample,
                          stream::IENU_Linear_Acceleration::Sample const& la_sample,
                          stream::IPressure::Sample const& p_sample)
    {
        auto last_pos_sample = m_position_output_stream->get_last_sample();

        if (pos_sample.is_healthy)
        {
            //if too far away, reset
            if (math::distance_sq(m_last_gps_position, pos_sample.value) > math::square(10.0))
            {
                last_pos_sample.value = pos_sample.value;
                m_last_gps_position = pos_sample.value;
                m_velocity.set(0, 0, 0);
            }
            else
            {
                auto lla_position = util::coordinates::ecef_to_lla(last_pos_sample.value);
                auto enu_to_ecef_rotation = util::coordinates::enu_to_ecef_rotation(lla_position);
                auto ecef_la = math::transform(enu_to_ecef_rotation, math::vec3d(la_sample.value));

                last_pos_sample.value = math::lerp(last_pos_sample.value, pos_sample.value, 3.0 * dts) + ecef_la * dts * dts * 0.5;
                //m_last_gps_position = last_pos_sample.value;

                QLOGI("acc: {}", ecef_la * dts * dts * 0.5);
            }
        }

        m_position_output_stream->push_sample(last_pos_sample.value, last_pos_sample.is_healthy);
        m_velocity_output_stream->push_sample(m_velocity, last_pos_sample.is_healthy);
    });
}

void Comp_ECEF_Position::set_input_stream_path(size_t idx, q::Path const& path)
{
    m_accumulator.set_stream_path(idx, path, m_init_params->rate, m_hal);
}

auto Comp_ECEF_Position::set_config(rapidjson::Value const& json) -> bool
{
    QLOG_TOPIC("comp_position::set_config");

    sz::Comp_ECEF_Position::Config sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, json, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize Comp_Position config data: {}", ss.str());
        return false;
    }

    *m_config = sz;

    return true;
}
auto Comp_ECEF_Position::get_config() const -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(*m_config, json);
    return std::move(json);
}

auto Comp_ECEF_Position::get_init_params() const -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(*m_init_params, json);
    return std::move(json);
}

auto Comp_ECEF_Position::send_message(rapidjson::Value const& /*json*/) -> rapidjson::Document
{
    return rapidjson::Document();
}

}
}
