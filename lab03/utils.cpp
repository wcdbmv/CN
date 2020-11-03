#include "utils.hpp"

#include "sys/socket.h"

std::string string_to_hex(const std::string& input) {
	constexpr char HEX_ALPHABET[] = "0123456789ABCDEF";

	std::string output;
	output.reserve(input.length() * 2);
	for (unsigned char c : input) {
		output.push_back(HEX_ALPHABET[c >> 4]);
		output.push_back(HEX_ALPHABET[c & 15]);
	}

	return output;
}

int reuse(int sd) {
	const linger sl = {
		.l_onoff = 1,
		.l_linger = 0,
	};
	return setsockopt(sd, SOL_SOCKET, SO_LINGER, &sl, sizeof sl);
}
