// observable.hpp
#pragma once
#include <functional>
#include <vector>
#include <unordered_set>
#include <memory>
#include <iostream>

namespace observe
{

    // Forward declaration
    class WatcherBase;

    // Internal dependency tracker (1 per thread)
    namespace internal
    {
        thread_local WatcherBase *currentWatcher = nullptr;
    }

    // Base class for all watchers (Watch and later Computed)
    class WatcherBase
    {
    public:
        virtual void run() = 0;
        virtual ~WatcherBase() = default;
    };

    // Observable<T>
    template <typename T>
    class Observable
    {
    public:
        Observable(const T &initial) : value(initial) {}

        T get() const
        {
            if (internal::currentWatcher)
            {
                dependents.insert(internal::currentWatcher); // now allowed
            }
            return value;
        }

        void set(const T &newValue)
        {
            if (value != newValue)
            {
                value = newValue;
                notify();
            }
        }

    private:
        T value;
        mutable std::unordered_set<WatcherBase *> dependents;

        void notify()
        {
            for (WatcherBase *w : dependents)
            {
                if (w)
                    w->run();
            }
        }
    };

    // Watch: reacts to observable changes
    class Watch : public WatcherBase
    {
    public:
        Watch(std::function<void()> fn) : effect(std::move(fn))
        {
            run(); // Initial run and dependency registration
        }

        void run() override
        {
            internal::currentWatcher = this;
            effect();
            internal::currentWatcher = nullptr;
        }

    private:
        std::function<void()> effect;
    };

} // namespace observe
