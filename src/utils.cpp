#include "utils.h"
#include <time.h>

namespace Utils{
    const char* ntpServer = "pool.ntp.org";
    const long gmtOffset_sec = 0;
    const int daylightOffset_sec = 3600;

    void initTime(){
        configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    }
    String getTime() {
        struct tm timeinfo;
        if(!getLocalTime(&timeinfo)){
            return "Sem hora";
        }
        char buffer[20];
        strftime(buffer, sizeof(buffer), "%H:%M:%S", &timeinfo);
        return String(buffer);
    }
}