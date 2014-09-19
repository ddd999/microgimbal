#pragma once

namespace silk
{

class IO_Board : q::util::Noncopyable
{
public:
    virtual ~IO_Board() {}

    enum class Connection_Result : uint8_t
    {
        OK,
        FAILED
    };

    enum class PWM_Frequency : uint8_t
    {
        SERVO_50HZ,
        SERVO_100HZ,
        SERVO_250HZ,
        SERVO_500HZ,
        PWM_1000Hz,
    };

    virtual auto connect() -> Connection_Result = 0;
    virtual void disconnect() = 0;

    virtual bool is_disconnected() const = 0;
    virtual bool is_running() const = 0;

    //----------------------------------------------------------------------
    //motors

    virtual void set_motor_throttles(float const* throttles, size_t count) = 0;

    //----------------------------------------------------------------------
    //camera

    virtual void set_camera_rotation(math::quatf const& rot) = 0;

    //----------------------------------------------------------------------
    //calibration
    virtual void set_accelerometer_calibration_data(math::vec3f const& bias, math::vec3f const& scale) = 0;
    virtual void get_accelerometer_calibration_data(math::vec3f& bias, math::vec3f& scale) const = 0;

    virtual void set_gyroscope_calibration_data(math::vec3f const& bias) = 0;
    virtual void get_gyroscope_calibration_data(math::vec3f& bias) const = 0;

    virtual void set_compass_calibration_data(math::vec3f const& bias) = 0;
    virtual void get_compass_calibration_data(math::vec3f& bias) const = 0;

    //----------------------------------------------------------------------
    //data interface

    template<class T> struct Data
    {
        Data() : value() {}
        T value;
        uint32_t sample_idx = 0;
    };

    //----------------------------------------------------------------------
    //sensors

    struct Accelerometer
    {
        math::vec3f acceleration; //meters / second^2
        q::Clock::duration dt;
    };
    typedef Data<Accelerometer> Accelerometer_Data;
    struct Gyroscope
    {
        math::vec3f angular_velocity; //radians per second
        q::Clock::duration dt;
    };
    typedef Data<Gyroscope> Gyroscope_Data;
    typedef Data<math::vec3f> Compass_Data; //NOT normalized
    typedef Data<float> Barometer_Data; //kp
    typedef Data<float> Sonar_Data; //meters
    typedef Data<float> Thermometer_Data; //degrees celsius
    typedef Data<float> Voltage_Data; //volts
    typedef Data<float> Current_Data; //amperes / second

    struct Sensor_Sample
    {
        Accelerometer_Data accelerometer;
        Gyroscope_Data gyroscope;
        Compass_Data compass;
        Barometer_Data barometer;
        Thermometer_Data thermometer;
        Sonar_Data sonar;
        Voltage_Data voltage;
        Current_Data current;
    };
    virtual auto get_sensor_samples() const -> std::vector<Sensor_Sample> const& = 0;

    struct GPS
    {
        uint8_t fix_count = 0;
        float precision = 0;
        double latitude = 0;
        double longitude = 0;
    };
    typedef Data<GPS> GPS_Data;
//    virtual auto get_gps_data() -> std::vector<GPS_Data> const& = 0;


    //----------------------------------------------------------------------
    //returns how many comm errors we've got
    virtual size_t get_error_count() const = 0;

    //----------------------------------------------------------------------
    virtual void process() = 0;
};

}
