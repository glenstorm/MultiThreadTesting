// Есть некий объект (например контейнер STL) из которого производятся конкурентные (из нескольких потоков) чтение и запись. Соотношение операций чтение/запись = 95/5.
// Приведите несколько способов организации доступа к данным объекта, опишите плюсы и минусы каждого способа.

#include <VectorController.h>
#include <boost/make_shared.hpp>

int main()
{    
    boost::shared_ptr<std::vector<int>> test_vec = boost::make_shared<std::vector<int>>();
    boost::shared_ptr<VectorController<int>> vc = boost::make_shared<VectorController<int>>( test_vec );
    
    VectorThread v1( vc );
    boost::thread t1{ boost::ref( v1 ) };
    
    VectorThread v2( vc );
    boost::thread t2{ boost::ref( v2 ) };
    
    t1.join();
    t2.join();
}
