#include "sharedstring.h"

SharedString::SharedString() :
    _string(),
    _stringMutex(new boost::shared_mutex())
{

}

SharedString::SharedString(std::string & copyStr) :
    _string(copyStr),
    _stringMutex(new boost::shared_mutex())
{

}

boost::shared_ptr < boost::shared_mutex > SharedString::getMutex()
{
    return _stringMutex;
}

std::string & SharedString::getString()
{
    return _string;
}
