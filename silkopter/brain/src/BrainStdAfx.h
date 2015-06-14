#include <type_traits>
#include <typeinfo>

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/optional.hpp>
#include <boost/noncopyable.hpp>

#define RAPIDJSON_HAS_STDSTRING 1

#include "QBase.h"
#include "QData.h"
#include "qmath.h"

#include "rapidjson/document.h"     // rapidjson's DOM-style API
#include "utils/Json_Util.h"

#include "Globals.h"
