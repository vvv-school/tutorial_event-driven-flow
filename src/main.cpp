#include "vOptFlow.h"
#include <yarp/os/all.h>
#include <yarp/sig/all.h>


int main(int argc, char * argv[])
{
    /* initialize yarp network */
    yarp::os::Network yarp;

    /* prepare and configure the resource finder */
    yarp::os::ResourceFinder rf;
    rf.setDefaultConfigFile("opticalflow.ini");
    rf.setDefaultContext("eventdriven");
    rf.configure(argc, argv);

    /* instantiate the module */
    vFlowModule module;
    return module.runModule(rf);
}
