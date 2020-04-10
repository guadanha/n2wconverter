#ifndef LOG_LOG_H_
#define LOG_LOG_H_

#include <set>
#include <sstream>
#include <cstdio>
#include <string.h>
#include <stdio.h>
#include <sys/time.h>

#include <unistd.h>
#include <sys/syscall.h>
#define LOG_GETTID() syscall(__NR_gettid)

#ifndef LOG_MAX_LEVEL
#define LOG_MAX_LEVEL LOGLog::DEBUG4
#endif


class LOGLog
{
private:
public:
    enum Level {
            NONE,
            ALWAYS,
            ERROR,
            WARNING,
            INFO,
            DEBUG,
            DEBUG1,
            DEBUG2,
            DEBUG3,
            DEBUG4
    };

public:
    LOGLog()
    :_output(output_stream()), _is_tty(isatty(fileno(_output)))
    {
    }

    ~LOGLog()
    {
        print(os.str());
    }

    static std::string now()
    {
        struct timeval tv;
        gettimeofday(&tv, 0);

        //Initializing it fully.
        struct tm r = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
        localtime_r(&(tv.tv_sec), &r);

        char result[15] = {0};
        std::sprintf(result, "%02d:%02d:%02d.%03d",
            r.tm_hour, r.tm_min, r.tm_sec, (int)(tv.tv_usec / 1000));
        return result;
    }

    static std::string moment()
    {
        struct timeval tv;
        gettimeofday(&tv, 0);

        //Initializing it fully.
        struct tm r = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
        localtime_r(&(tv.tv_sec), &r);

        char result[26] = {0};
        std::sprintf(result, "%04d-%02d-%02d %02d:%02d:%02d",
            r.tm_year + 1900, r.tm_mon + 1, r.tm_mday, r.tm_hour, r.tm_min, r.tm_sec);
        return result;
    }

    std::ostringstream& get(LOGLog::Level level, const std::string& domain)
    {
        if (_is_tty)
        {
            if (level == LOGLog::ERROR)
            {
                os << "\033[1;31m"; // SETCOLOR_FAILURE
            }
            else if (level == LOGLog::WARNING)
            {
                os << "\033[1;33m"; // SETCOLOR_WARNING
            }
            else
            {
                os << "\033[0;39m"; // SETCOLOR_NORMAL
            }
        }
        os << moment() << " [" << (int)(LOG_GETTID()) << "] <" << domain
        << "> " << to_string(level) << ((level != ALWAYS)? ": " : "");
        os.clear();
        return os;
    }

    void print(const std::string& str)
    {
        FILE* out = output_stream();
        if (!out) return; // Log completely silenced

        fprintf(out, "%s", str.c_str());

        if (_is_tty) fprintf(out, "\033[0;39m"); // SETCOLOR_NORMAL

        fflush(out);
    }

    static void reporting_level(LOGLog::Level lvl)
    {
        reporting_level() = lvl;
    }

    static void output_stream(FILE* stream)
    {
        output_stream() = stream;
    }

    static FILE*& output_stream()
    {
        static FILE* pStream = stdout;
        return pStream;
    }

    static LOGLog::Level& reporting_level()
    {
        static LOGLog::Level reportingLevel = LOGLog::DEBUG4;
        return reportingLevel;
    }

#if defined(LOG_LOG_NO_FILTERING)

    static bool can_log(const std::string&)
    {
        return true;
    }

    static bool can_log(const char *)
    {
        return true;
    }

#else

    static void unblock_tid(unsigned int tid = 0)
    {
        if(!tid) tid = LOG_GETTID();
        data_locker lock;
        data()->_blocked_tids.erase(tid);
    }

    static void block_tid(unsigned int tid)
    {
        data_locker lock;
        data()->_blocked_tids.insert(tid);
    }

    static std::set<unsigned int> blocked_tids()
    {
        data_locker lock;
        return data()->_blocked_tids;
    }

    static const std::set<std::string> blocked_domains()
    {
        data_locker lock;
        return data()->_blkd;
    }

    static const std::set<std::string> whitelisted_domains()
    {
        data_locker lock;
        return data()->_wld;
    }

    static void block_domain(const std::string& domain)
    {
        data_locker lock;
        if (domain == "*")
        {
            data()->_block_all_domains = true;
        }
        else
        {
            data()->_blkd.insert(domain);
            data()->_wld.erase(domain);
        }
    }

    static void unblock_domain(const std::string& domain)
    {
        data_locker lock;
        if (domain == "*")
        {
            data()->_block_all_domains = false;
        }
        else
        {
            data()->_wld.insert(domain);
            data()->_blkd.erase(domain);
        }
    }

    static bool can_log(const std::string& domain)
    {
        int tid = LOG_GETTID();

        data_locker lock;
        if (data()->_blocked_tids.find(tid) != data()->_blocked_tids.end())
        {
            return false;
        }

        // Optimize the most usual case
        if (!(data()->_block_all_domains) && data()->_blkd.empty())
        {
            return true;
        }

        // If we block all, check the whitelist to see if we can enable it.
        if (data()->_block_all_domains)
        {
            return data()->_wld.count(domain) != 0;
        }

        return data()->_blkd.count(domain) == 0;
    }


private:

    struct data_locker
    {
        data_locker()
        {
            pthread_mutex_lock(data_locker::get_mutex());
        }
        ~data_locker()
        {
            pthread_mutex_unlock(data_locker::get_mutex());
        }
        static pthread_mutex_t* get_mutex()
        {
            static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
            return &mutex;
        }
    };

#endif // defined(LOG_LOG_NO_FILTERING)

private:

    struct log_detail
    {
        std::set<std::string> _wld;
        std::set<std::string> _blkd;
        std::set<unsigned int> _blocked_tids;
        bool _block_all_domains;
    };

    static log_detail* data()
    {
        static log_detail* _detail = NULL;
        if (!_detail) _detail = new log_detail();
        return _detail;
    }

    LOGLog(const LOGLog&);
    LOGLog& operator =(const LOGLog&);

    static std::string to_string(LOGLog::Level level)
    {
      static const char* const buffer[] = { "NONE", "", "ERROR",
          "WARNING", "INFO", "DEBUG", "DEBUG1", "DEBUG2", "DEBUG3", "DEBUG4"};
      return buffer[level];
    }

    FILE* _output;
    bool _is_tty;
    std::ostringstream os;

};


#define GENERIC_LOG(domain, level) \
    if (level > LOG_MAX_LEVEL) ; \
    else if (level > LOGLog::reporting_level() || !LOGLog::output_stream()) ; \
    else if (!LOGLog::can_log(domain)) ; \
    else LOGLog().get(level, domain)

#define FILE_BASENAME ((strrchr(__FILE__, '/') ?: __FILE__ - 1) + 1)
#define FILE_LOG(domain, level) GENERIC_LOG(domain, level) << FILE_BASENAME \
                                             << ":" << __LINE__ << " - "


#define LOG_DEBUG(domain)   FILE_LOG(domain, LOGLog::DEBUG)
#define LOG_DEBUG1(domain)  FILE_LOG(domain, LOGLog::DEBUG1)
#define LOG_DEBUG2(domain)  FILE_LOG(domain, LOGLog::DEBUG2)
#define LOG_DEBUG3(domain)  FILE_LOG(domain, LOGLog::DEBUG3)
#define LOG_DEBUG4(domain)  FILE_LOG(domain, LOGLog::DEBUG4)
#define LOG_ERROR(domain)   FILE_LOG(domain, LOGLog::ERROR)
#define LOG_ALWAYS(domain)  FILE_LOG(domain, LOGLog::ALWAYS)
#define LOG_WARNING(domain) FILE_LOG(domain, LOGLog::WARNING)
#define LOG_INFO(domain)    FILE_LOG(domain, LOGLog::INFO)
#define LOG_TRACE()         LOG_INFO("trace") << __FUNCTION__ << std::endl 

#endif // LOG_LOG_H_
