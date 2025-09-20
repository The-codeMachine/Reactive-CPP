// main.cpp
#include "observable.hpp"

int main()
{
    using namespace observe;

    Observable<int> a(5);
    Observable<int> b(10);

    Watch watcher([&]()
                  { std::cout << "a = " << a.get() << ", b = " << b.get() << std::endl; });

    a.set(7);  // triggers watcher
    b.set(12); // triggers watcher

    std::cin.get();

    return 0;
}
