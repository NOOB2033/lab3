#ifndef LAB3_IOCCONTAINER_H
#define LAB3_IOCCONTAINER_H


#include <functional>
#include <cstdlib>
#include <memory>
#include <map>
#include <string>


/*
Получение кода типа;
    Прежде чем мы сможем безопасно сохранить способ получения класса,
    подписывающегося на определенный интерфейс, мы должны выделить для этого код типа.
    В предложенной реализации  реализации контейнера IOC  есть статическая целочисленная переменная,
    которая определяет идентификатор следующего типа, который будет выделен,
    и экземпляр статической локальной переменной для каждого типа,
    к которому можно обратиться, вызвав метод GetTypeID.
*/


class IOCContainer
{
private:
    static int s_typeId;
public:
    /*
  Получение экземпляров объекта;
      Теперь, когда  есть идентификатор типа, мы должны иметь возможность хранить какой-то фабричный объект,
      чтобы показать тот факт, что мы не знаем, как создать объект.(так как мы не знаем как создать объект)
      Хранить все фабрики в одной коллекции, для этого выбираем абстрактный базовый класс,
      из которого будут производиться фабрики, и реализацию, которая захватывает функтор для последующего вызова.
      Можно использовать std :: map для хранения фабрик,та же можно рассмотреть  другие варианты для эффективности.
 */

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
    //one typeid per type
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

    /*
       Регистрация экземпляров
   */

    // Базовая реализация - регистрация функтора
    template<typename TInterface, typename ...TS>
    void RegisterFunctor(std::function<std::shared_ptr<TInterface>(std::shared_ptr<TS> ...ts)> functor)
    {
        factories[GetTypeID<TInterface>()] = std::make_shared<CFactory<TInterface>>([=] {return functor(GetObject<TS>()...); });
    }
    // Регистрация экземпляра объекта
    template<typename TInterface>
    void RegisterInstance(std::shared_ptr<TInterface> t) {
        factories[GetTypeID<TInterface>()] = std::make_shared<CFactory<TInterface>>([=] {return t; });
    }
    // Работаем с указателем на функцию
    template<typename TInterface, typename ...TS>
    void RegisterFunctor(std::shared_ptr<TInterface>(*functor)(std::shared_ptr<TS> ...ts))
    {
        RegisterFunctor(std::function<std::shared_ptr<TInterface>(std::shared_ptr<TS> ...ts)>(functor));
    }
    // Фабрика, которая будет вызывать необходимый конструктор для каждого экземпляра
    template<typename TInterface, typename TConcrete, typename ...TArguments>
    void RegisterFactory()
    {
        RegisterFunctor(
                std::function<std::shared_ptr<TInterface>(std::shared_ptr<TArguments> ...ts)>(
                        [](std::shared_ptr<TArguments>...arguments) -> std::shared_ptr<TInterface> {
                            return std::make_shared<TConcrete>(std::forward<std::shared_ptr<TArguments>>(arguments)...);
                        }));

    }
    // Фабрика, которая будет возвращать один экземпляр  объекта  для каждого вызова
    template<typename TInterface, typename TConcrete, typename ...TArguments>
    void RegisterInstance()
    {
        RegisterInstance<TInterface>(std::make_shared<TConcrete>(GetObject<TArguments>()...));
    }
private:
    //todo: consider sorted vector
    std::map<int, std::shared_ptr<FactoryBase>> factories;
};


#endif //LAB3_IOCCONTAINER_H