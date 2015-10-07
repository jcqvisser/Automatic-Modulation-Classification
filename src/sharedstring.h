#ifndef SHAREDSTRING_H
#define SHAREDSTRING_H

#include <boost/smart_ptr.hpp>
#include <boost/thread.hpp>
#include <string>

class SharedString
{
public:
    /**
     * @brief SharedString default constructor, initializes default std::string object.
     */
    SharedString();

    /**
     * @brief SharedString copy constructor, initializes a std::string object from the given std::string object
     * @param The given std::string object to be copied.
     */
    SharedString(std::string & copyStr);

    /**
     * @brief Function to get the mutex object from this shared string.
     * @return Shared pointer to the mutex object.
     */
    boost::shared_ptr < boost::shared_mutex > getMutex();

    /**
     * @brief getData function used to get the std::string that stores the data, so that it can be modified, or read.
     * @return A reference to the stored std::string object.
     */
    std::string & getString();

private:
    std::string _string;

    boost::shared_ptr < boost::shared_mutex > _stringMutex;
};

#endif // SHAREDSTRING_H
