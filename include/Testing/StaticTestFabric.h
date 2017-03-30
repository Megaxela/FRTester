//
// Created by megaxela on 06.03.17.
//

#ifndef FRTESTER_STATICTESTFABRIC_H
#define FRTESTER_STATICTESTFABRIC_H

#include <Testing/ManualTests/Fabric.h>
#include <Testing/ManualTests/FabricRegistrator.h>
#include "AbstractTest.h"

#define REGISTER_STATIC_TEST(OBJECT)\
static FabricRegistrator<StaticTestFabric, OBJECT> __registrator(#OBJECT);

typedef Fabric<AbstractTest, std::string> StaticTestFabric;


#endif //FRTESTER_STATICTESTFABRIC_H
