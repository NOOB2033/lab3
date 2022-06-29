#ifndef LAB3_IOCCONTAINER_H
#define LAB3_IOCCONTAINER_H


#include <functional>
#include <cstdlib>
#include <memory>
#include <map>
#include <string>


class IOCContainer
{
private:
    static int s_typeId;
public:
    class FactoryBase
    {
    public:
        virtual ~FactoryBase() = default;
    };

    static IOCContainer& instance()
    {
        static IOCContainer container;
        return container;
    }

    template<typename T>
    class CFactory : public FactoryBase
    {
    public:
        ~CFactory() = default;

        CFactory(std::function<std::shared_ptr<T>()> functor)
                : functor(functor)
        {}

        std::shared_ptr<T> GetObject()
        {
            return functor();
        }

    private:
        std::function<std::shared_ptr<T>()> functor;
    };

    static IOCContainer& getIOCContainer()
    {
        static IOCContainer container;
        return container;
    }

    template<typename T>
    static int GetTypeID()
    {
        static int typeId = s_typeId++;
        return typeId;
    }

    template<typename T>
    std::shared_ptr<T> GetObject()
    {
        auto typeId = GetTypeID<T>();
        auto factoryBase = factories[typeId];
        auto factory = std::static_pointer_cast<CFactory<T>>(factoryBase);
        return factory->GetObject();
    }

    template<typename TInterface, typename ...TS>
    void RegisterFunctor(std::function<std::shared_ptr<TInterface>(std::shared_ptr<TS> ...ts)> functor)
    {
        factories[GetTypeID<TInterface>()] = std::make_shared<CFactory<TInterface>>([=] {return functor(GetObject<TS>()...); });
    }

    template<typename TInterface>
    void RegisterInstance(std::shared_ptr<TInterface> t) {
        factories[GetTypeID<TInterface>()] = std::make_shared<CFactory<TInterface>>([=] {return t; });
    }

    template<typename TInterface, typename ...TS>
    void RegisterFunctor(std::shared_ptr<TInterface>(*functor)(std::shared_ptr<TS> ...ts))
    {
        RegisterFunctor(std::function<std::shared_ptr<TInterface>(std::shared_ptr<TS> ...ts)>(functor));
    }

    template<typename TInterface, typename TConcrete, typename ...TArguments>
    void RegisterFactory()
    {
        RegisterFunctor(
                std::function<std::shared_ptr<TInterface>(std::shared_ptr<TArguments> ...ts)>(
                        [](std::shared_ptr<TArguments>...arguments) -> std::shared_ptr<TInterface> {
                            return std::make_shared<TConcrete>(std::forward<std::shared_ptr<TArguments>>(arguments)...);
                        }));

    }

    template<typename TInterface, typename TConcrete, typename ...TArguments>
    void RegisterInstance()
    {
        RegisterInstance<TInterface>(std::make_shared<TConcrete>(GetObject<TArguments>()...));
    }
private:
    std::map<int, std::shared_ptr<FactoryBase>> factories;
};


#endif //LAB3_IOCCONTAINER_H