#ifndef nanoStamp_hxx_seen
#define nanoStamp_hxx_seen
#include <ctime>

namespace CP {

    // A typedef (i.e. long long or int64_t) to handle a NanoStamp, which is
    // just the count of nanoseconds since January 1, 1970.  This has a time
    // range of just a bit more than 145 years, or until at least 2115.  The
    // type is signed so all the usual arithmetic and comparisons work.  It's
    // defined as long long since I don't want to exclude older compilers.
    typedef long long NanoStamp;

    const NanoStamp kNanoStampMicrosecond = 1000LL;
    const NanoStamp kNanoStampMillisecond = kNanoStampMicrosecond*1000LL;
    const NanoStamp kNanoStampSecond = kNanoStampMillisecond*1000LL;
    
    // An inline function to convert a nanostamp (i.e. a long long) into a
    // std::time_t.  This just truncates the ns part of the stamp.
    inline std::time_t NanoStampToTime(CP::NanoStamp t) {
        return t/kNanoStampSecond;
    }

    // An inline function to get the ns part of a nanostamp.  This is just a
    // mod 1000000000.
    inline int NanoStampFromLastSecond(CP::NanoStamp t) {
        return t%kNanoStampSecond;
    }

    // An easy way to create a nanostamp from a time.
    inline NanoStamp TimeToNanoStamp(std::time_t s, int ns = 0) {
        return s*kNanoStampSecond+ns;
    }
}
#endif
