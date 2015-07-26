#include <iostream>
#include <web/web.hpp>

using namespace std;
using namespace web;

int
main(int argc, char * argv[])
{
	application app(argc, argv);
	try
	{
		app.get("/", [](request & req, response & res) {
			res.write("Hello world!");
		});
		app.listen(3333);
	} catch (exception const & e)
	{
		cerr << "Caught exception: " << e.what() << endl;
		return 1;
	}
}