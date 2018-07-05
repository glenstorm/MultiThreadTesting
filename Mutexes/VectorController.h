#ifndef _VECTOR_CONTROLLER_H_GUARD_
#define _VECTOR_CONTROLLER_H_GUARD_

#include <vector>
#include <iostream>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>
#include <boost/core/noncopyable.hpp>

template<class T>
class VectorController : private boost::noncopyable
{
public:
    VectorController( VectorController&& other )
        : vec( other.vec )
    {}
    
    VectorController( std::vector<T> &rw_vector )
        : vec( rw_vector ) {}

    T read() const
    {
        boost::shared_lock<boost::shared_mutex> lock{mutex};
        if( !vec.empty() )
            return vec.back();

        return -100;
    }

    void write( const T& val )
    {
        boost::unique_lock<boost::shared_mutex> lock{mutex};
        vec.push_back( val );
    }

private:
    mutable boost::shared_mutex mutex;
    std::vector<T>& vec;
};

static boost::mutex io_mutex;

class VectorThread : private boost::noncopyable
{
public:
    VectorThread( VectorThread&& other )
        : vc( std::move( other.vc ) )
    {}
    
    VectorThread( const boost::shared_ptr<VectorController<int>> &_vc )
        :vc( _vc )
    {
        srand( time( NULL ) );
    }

    void operator()()
    {
        for (int i = 0; i < 1000000; ++i)
        {
            makeAction();
        }
    }

private:
    // Записываем или считываем значение с пропорцией 95/5.
    void makeAction()
    {
        boost::lock_guard<boost::mutex>{ io_mutex };
        int distr = rand() % 100;
        if( distr < 95 )
        {
            int read_val = vc->read();
            io_mutex.lock();
            std::cout << "Reading: " << read_val << std::endl;
            io_mutex.unlock();
        }
        else
        {
            vc->write( distr );
            io_mutex.lock();
            std::cout << "Writing: " << distr << std::endl;
            io_mutex.unlock();
        }
    }

private:
    boost::shared_ptr<VectorController<int>> vc;
};


#endif
    // _VECTOR_CONTROLLER_H_GUARD_
