//
// Created by megaxela on 03.03.17.
//

#ifndef FRTESTER_ACTIONSFABRIC_H
#define FRTESTER_ACTIONSFABRIC_H

#include "AbstractTestAction.h"
#include "Fabric.h"
#include <Testing/ManualTests/FabricRegistrator.h>

#define REGISTER_ACTION(OBJECT)\
static FabricRegistrator<TestActionFabric, OBJECT> __registrator(#OBJECT);

typedef Fabric<AbstractTestAction, std::string> TestActionFabric;

#endif //FRTESTER_ACTIONSFABRIC_H
