
#if defined(APPLE) || defined(__APPLE__) || defined(DARWIN) || defined(__linux__) || defined(BSD)

#    include <errno.h>
#    include <stdio.h>
#    include <unistd.h>

int get_cpu_count(void) {
    long nprocs = sysconf(_SC_NPROCESSORS_ONLN);
    if(nprocs == -1) {
        perror("sysconf");
        return -1;  // Indicate an error
    }
    return (int)nprocs;
}

#elif defined(_WIN32) || defined(_WIN64)

#    define WIN32_LEAN_AND_MEAN
#    include <windows.h>

int get_cpu_count(void) {
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    return (int)sysinfo.dwNumberOfProcessors;
}

#elif
#    error "Unsupported platform!"
#endif
