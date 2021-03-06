// Есть некий объект (например контейнер STL) из которого производятся конкурентные (из нескольких потоков) чтение и запись. Соотношение операций чтение/запись = 95/5.
// Приведите несколько способов организации доступа к данным объекта, опишите плюсы и минусы каждого способа.

#include <vector>
#include <VectorController.h>
#include <boost/make_shared.hpp>

int main()
{
    std::vector<int> test_vec;
    boost::shared_ptr<VectorController<int>> vc = boost::make_shared<VectorController<int>>( test_vec );

    const int thread_num = 8;

    std::vector<boost::thread> t;
    for( int i = 0; i < thread_num; ++i )
    {
        t.emplace_back( VectorThread( vc ) );
    }

 	for( int i = 0; i < thread_num; ++i )
    {
        t[i].join();
    }
}
