#include <stingraykit/log/Logger.h>


int main()
{
	try
	{
		App app;
		app.Run();
	}
	catch(const std::exception& ex)
	{ Logger::Error() << "bikebrain failed: " << ex; }
}
