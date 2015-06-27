#include <bikebrain/App.h>

#include <stingraykit/log/Logger.h>

using namespace bikebrain;

int main()
{
	try
	{
		App app;
		app.Run();
	}
	catch(const std::exception& ex)
	{ stingray::Logger::Error() << "bikebrain failed: " << ex; }
}
