// Copyright (C) 2001
// William E. Kempf
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee,
// provided that the above copyright notice appear in all copies and
// that both that copyright notice and this permission notice appear
// in supporting documentation.  William E. Kempf makes no representations
// about the suitability of this software for any purpose.  
// It is provided "as is" without express or implied warranty.

#ifndef BOOST_RECURSIVE_MUTEX_WEK070601_HPP
#define BOOST_RECURSIVE_MUTEX_WEK070601_HPP

#include <boost/thread/config.hpp>
#ifndef BOOST_HAS_THREADS
#   error	Thread support is unavailable!
#endif

#include <boost/utility.hpp>
#include <boost/thread/xlock.hpp>

#if defined(BOOST_HAS_PTHREADS)
#   include <pthread.h>
#endif

namespace boost {

class condition;
struct xtime;

class recursive_mutex : private noncopyable
{
public:
    friend class basic_lock<recursive_mutex>;
    friend class condition;
    
    typedef basic_lock<recursive_mutex> lock;
    
    recursive_mutex();
    ~recursive_mutex();
    
private:
#if defined(BOOST_HAS_WINTHREADS)
    typedef size_t cv_state;
#elif defined(BOOST_HAS_PTHREADS)
    struct cv_state
    {
        long count;
        pthread_mutex_t* pmutex;
    };
#endif
    void do_lock();
    void do_unlock();
    void do_lock(cv_state& state);
    void do_unlock(cv_state& state);

#if defined(BOOST_HAS_WINTHREADS)
    unsigned long m_mutex;
    unsigned long m_count;
#elif defined(BOOST_HAS_PTHREADS)
    pthread_mutex_t m_mutex;
    unsigned m_count;
#   if !defined(BOOST_HAS_PTHREAD_MUTEXATTR_SETTYPE)
    pthread_cond_t m_unlocked;
    pthread_t m_thread_id;
    bool m_valid_id;
#   endif
#endif
};

class recursive_try_mutex : private noncopyable
{
public:
    friend class basic_lock<recursive_try_mutex>;
    friend class basic_trylock<recursive_try_mutex>;
    friend class condition;
    
    typedef basic_lock<recursive_try_mutex> lock;
    typedef basic_trylock<recursive_try_mutex> trylock;
    
    recursive_try_mutex();
    ~recursive_try_mutex();
    
private:
#if defined(BOOST_HAS_WINTHREADS)
    typedef size_t cv_state;
#elif defined(BOOST_HAS_PTHREADS)
    struct cv_state
    {
        long count;
        pthread_mutex_t* pmutex;
    };
#endif
    void do_lock();
    bool do_trylock();
    void do_unlock();
    void do_lock(cv_state& state);
    void do_unlock(cv_state& state);
    
#if defined(BOOST_HAS_WINTHREADS)
    unsigned long m_mutex;
    unsigned long m_count;
#elif defined(BOOST_HAS_PTHREADS)
    pthread_mutex_t m_mutex;
    unsigned m_count;
#   if !defined(BOOST_HAS_PTHREAD_MUTEXATTR_SETTYPE)
    pthread_cond_t m_unlocked;
    pthread_t m_thread_id;
    bool m_valid_id;
#   endif
#endif
};

class recursive_timed_mutex : private noncopyable
{
public:
    friend class basic_lock<recursive_timed_mutex>;
    friend class basic_trylock<recursive_timed_mutex>;
    friend class basic_timedlock<recursive_timed_mutex>;
    friend class condition;
    
    typedef basic_lock<recursive_timed_mutex> lock;
    typedef basic_trylock<recursive_timed_mutex> trylock;
    typedef basic_timedlock<recursive_timed_mutex> timedlock;
    
    recursive_timed_mutex();
    ~recursive_timed_mutex();
    
private:
#if defined(BOOST_HAS_WINTHREADS)
    typedef size_t cv_state;
#elif defined(BOOST_HAS_PTHREADS)
    struct cv_state
    {
        long count;
        pthread_mutex_t* pmutex;
    };
#endif
    void do_lock();
    bool do_trylock();
    bool do_timedlock(const xtime& xt);
    void do_unlock();
    void do_lock(cv_state& state);
    void do_unlock(cv_state& state);
    
#if defined(BOOST_HAS_WINTHREADS)
    unsigned long m_mutex;
    unsigned long m_count;
#elif defined(BOOST_HAS_PTHREADS)
    pthread_mutex_t m_mutex;
    pthread_cond_t m_unlocked;
    pthread_t m_thread_id;
    bool m_valid_id;
    unsigned m_count;
#endif
};

} // namespace boost

// Change Log:
//    8 Feb 01  WEKEMPF Initial version.
//    1 Jun 01  WEKEMPF Modified to use xtime for time outs.  Factored out
//                      to three classes, mutex, try_mutex and timed_mutex.
//   11 Jun 01  WEKEMPF Modified to use PTHREAD_MUTEX_RECURSIVE if available.

#endif // BOOST_RECURSIVE_MUTEX_WEK070601_HPP
