#define BOOST_TEST_MODULE basic
#include <boost/test/included/unit_test.hpp>

#include <string>
#include <cerrno>

#include <web/web.hpp>

using namespace boost;

//____________________________________________________________________________//

BOOST_AUTO_TEST_CASE (test_throw_system_exception)
{
	struct
	{
		void operator()()
		{
			FILE * file = fopen("I_HOPE_THIS_FILE_DOES_NOT_EXISTS.txt", "r");
			if (!file)
			{
				__throw_system_exception();
			}
		}
	} bad;
	BOOST_CHECK_THROW(bad(), std::runtime_error);
}

//____________________________________________________________________________//
