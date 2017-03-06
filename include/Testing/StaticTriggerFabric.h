//
// Created by megaxela on 06.03.17.
//

#ifndef FRTESTER_STATICTRIGGERFABRIC_H
#define FRTESTER_STATICTRIGGERFABRIC_H

#include <include/Testing/ManualTests/Fabric.h>
#include <include/Testing/ManualTests/FabricRegistrator.h>
#include <include/Testing/AbstractTriggerTest.h>

#define REGISTER_STATIC_TRIGGER(OBJECT)\
static FabricRegistrator<StaticTriggerFabric, OBJECT> __registrator(#OBJECT);

typedef Fabric<AbstractTriggerTest, std::string> StaticTriggerFabric;

#endif //FRTESTER_STATICTRIGGERFABRIC_H
