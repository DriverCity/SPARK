#include "Application.h"
#include <signal.h>
#include <unistd.h>
#include <functional>
#include "Logger/Logger.h"

namespace
{

void signalHandler(int)
{
    LOG_DEBUG("Received signal to quit.");
    spark::Application::stop();
}

}


namespace spark
{

std::atomic_bool Application::m_stop(false);

Application::Application(const Setup& setup) :
    m_setup(setup)
{
}


Application::~Application()
{
}



int Application::exec()
{
    // Set signal handler
    signal(SIGINT, signalHandler);
    signal(SIGABRT, signalHandler);

    m_setup.bleService->start();

    while (m_stop.load() == false)
    {
        // Wait for end.
        sleep(2);
    }

    m_setup.bleService->stop();
    return 0;
}


void Application::stop()
{
    m_stop.store(true);
}


} // spark
