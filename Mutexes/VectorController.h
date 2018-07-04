#ifndef _VECTOR_CONTROLLER_H_GUARD_
#define _VECTOR_CONTROLLER_H_GUARD_

#include <vector>
#include <iostream>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>
#include <boost/core/noncopyable.hpp>

template<class T>
class VectorController
{
public:
    VectorController( const boost::shared_ptr<std::vector<T>> &rw_vector )
        :vec( rw_vector ) {}
        
    T read() const
    {
        boost::shared_lock<boost::shared_mutex> lock{mutex};
        if( !vec->empty() )
            return vec->back();
        
        return -100;
    }
    
    void write( const T& val )
    {
        boost::unique_lock<boost::shared_mutex> lock{mutex};
        vec->push_back( val );
    }
    
private:
    mutable boost::shared_mutex mutex;
    boost::shared_ptr<std::vector<T>> vec;
};


class VectorThread
{
public:
    explicit VectorThread( const boost::shared_ptr<VectorController<int>> &_vc )
        :vc( _vc )
    {
        srand( time( NULL ) );
    }
    
    void operator()()
    {
        for (int i = 0; i < 1000000000; ++i)
        {
            makeAction();
            boost::this_thread::sleep_for( boost::chrono::milliseconds{1} );
        }
    }
    
private:
    // Записываем или считываем значение с пропорцией 95/5.
    void makeAction()
    {
        int distr = rand() % 100;
        if( distr < 95 )
        {
            std::cout << "Reading: " << vc->read() << std::endl;
        }
        else
        {
            std::cout << "Writing: " << distr << std::endl;
            vc->write( distr );
        }
    }
    
private:
    boost::shared_ptr<VectorController<int>> vc;
};


#endif
    // _VECTOR_CONTROLLER_H_GUARD_
