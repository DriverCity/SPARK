
#include "Initializer.h"
#include "Application.h"
#include <iostream>

int main(int argc, char** argv)
{
    // Instantiate and initialize components.
    spark::Initializer initializer;
    initializer.InitializeConfiguration(argv[0]);
    initializer.InitializeLogger(spark::GlobalConfiguration::instance(), std::cout);
    spark::Application::Setup setup = initializer.createSetup(spark::GlobalConfiguration::instance());

    // Start application.
    spark::Application app(setup);
    return app.exec();
}
