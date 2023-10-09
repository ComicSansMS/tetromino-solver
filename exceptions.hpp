#include <stdexcept>

#define PROTOCOL_VIOLATION(msg) throw std::logic_error(msg)
