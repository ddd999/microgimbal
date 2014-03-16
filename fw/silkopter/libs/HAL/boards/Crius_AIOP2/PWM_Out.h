#pragma once

namespace hal
{	
namespace pwm_out
{
	extern void init();
	
	static const uint8_t MAX_CHANNEL_COUNT = 8;

	//sets the value of channel ch
	extern void set_channel(uint8_t ch, int16_t val);
	
	//sets the value for all channels up to min(size, get_channel_count())
	extern void set_channels(const int16_t* src, uint8_t size);
	
	extern void set_all_channels(int16_t val);
	
	//sets the frequency of a channel in hertz
	extern void set_frequency(uint8_t ch, uint16_t hz);

	//sets the frequency for all channels up to min(size, get_channel_count())
	extern void set_frequencies(const uint16_t* hz, uint8_t size);
	extern void set_frequencies(uint16_t hz);
	
	extern void set_all_enabled(bool enabled);
	extern void set_enabled(uint8_t ch, bool enabled);
}
}