#include "utils.h"

void fatalError(const char *why) {
    fprintf(stderr, "%s", why);
    exit(0x666);
}

int isExtensionSupported(const char *extList, const char *extension) {

    const char *start;
    const char *where, *terminator;

    /* Extension names should not have spaces. */
    where = strchr(extension, ' ');
    if (where || *extension == '\0')
        return 0;

    /* It takes a bit of care to be fool-proof about parsing the
       OpenGL extensions string. Don't be fooled by sub-strings,
       etc. */
    for (start = extList;;) {
        where = strstr(start, extension);

        if (!where)
            break;

        terminator = where + strlen(extension);

        if (where == start || *(where - 1) == ' ')
            if (*terminator == ' ' || *terminator == '\0')
                return 1;

        start = terminator;
    }
    return 0;
}

int msleep(long msec) {
    struct timespec ts;
    int res;

    if (msec < 0) {
        errno = EINVAL;
        return -1;
    }

    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec % 1000) * 1000000;

    do {
        res = nanosleep(&ts, &ts);
    } while (res && errno == EINTR);

    return res;
}

uint64_t getTimeMS() {
    struct timespec ts;
    if (clock_gettime(CLOCK_MONOTONIC, &ts) == 0)
        return (uint64_t) ((ts.tv_sec * 1000)+(ts.tv_nsec / 1000000));
    else
        return 0;
}