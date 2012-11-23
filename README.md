# web

Minimalistic web framework for C++11.

## Introduction

Even wondered if it is possible to write web apps in pure C++?

## Features

* HTTP verbs included.
* No external dependencies - relying only on standard library.
* High test coverage.

## Requirements

* CMake (2.6+)
* C++11 compliant compiler.

## Example

	int main(int argc, char* argv[])
	{
	        web::application app(argc, argv);
	        try
	        {
	                app.get("/", [](web::request & req, web::response & res) {
	                        res.stream() << "Hello world!";
	                });
	                app.listen(3333);
	        } catch (std::exception const & e)
	        {
	                std::cerr << "Caught exception: " << e.what() << std::endl;
	                return 1;
	        }
	}

Now just compile this code and point your web browser at http://localhost:3333/.

