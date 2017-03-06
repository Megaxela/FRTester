//
// Created by megaxela on 03.03.17.
//

#ifndef FRTESTER_FABRICREGISTRATOR_H
#define FRTESTER_FABRICREGISTRATOR_H

#define REGISTER(Fabric, Object, Name)\
static FabricRegistrator<Fabric, Object> __registrator(Name);

template<typename Fabric, typename Object>
class FabricRegistrator
{
public:
    FabricRegistrator(const typename Fabric::NameType& name)
    {
        Fabric::instance().template registerObject<Object>(name);
    }
};


#endif //FRTESTER_FABRICREGISTRATOR_H
