#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include "syscalls.h"
#define PERMS 0660

FILE _iob[OPEN_MAX] = { /* stdin, stdout, stderr: */
    {0, (char*)0, (char*)0, _READ, 0 },
    {0, (char*)0, (char*)0, _WRITE, 1 },
    {0, (char*)0, (char*)0, _WRITE | _UNBUF, 2 }
};

FILE *fopen(char *name, char *mode) {
    int fd;
    FILE *fp;
    
    if (*mode != 'r' && *mode != 'w' && *mode != 'a') return NULL;
    for (fp=_iob; fp<_iob+OPEN_MAX; fp++)
        if ((fp->flag&(_READ|_WRITE)) == 0)
            break; //* found free slot */
    if (fp>=_iob+OPEN_MAX) return NULL; // no free slots

    switch (*mode) {
        case 'w': // write
            fd = creat(name, PERMS);
            break;
        case 'a': // append
            if ((fd=open(name, O_RDONLY, 0)) == -1)
                fd = creat(name, PERMS);
            lseek(fd, 0L, 2); // unistd
            break;
        case 'r': // read
            fd = open(name, O_RDONLY, 0);
            break;
    }

    if (fd==-1) return NULL; // could not access file
   
    fp->fd = fd;
    fp->cnt = 0;
    fp->base = NULL;
    fp->flag = (*mode == 'r') ? _READ : _WRITE;
    return fp;
}

/* _fillbuf:  allocate and fill input buffer */
int _fillbuf(FILE *fp) {
    int bufsize;
    
    if ((fp->flag&(_READ | _EOF | _ERR)) != _READ)
        return EOF;
    bufsize = (fp->flag &_UNBUF) ? 1 : BUFSIZ;
    if (fp->base == NULL)   /* no buffer yet */
        if ((fp->base = (char*) malloc(bufsize)) == NULL)
            return EOF;  /* can′t get buffer */
    fp->ptr = fp->base;
    fp->cnt = read(fp->fd, fp->ptr, bufsize);
    if (--fp->cnt<0) {
        if (fp->cnt==-1)
            fp->flag |= _EOF;
        else
            fp->flag |= _ERR;
        fp->cnt = 0;
        return EOF;
    }
    return (unsigned char) *fp->ptr++;
}

int _flushbuf(int c,FILE* fp) {
    int character = c;

    // if buffer is full then we delete buffer and recreate it
    if (!(fp->cnt)) {
        free(fp->base); // free space previously allocated
        _fillbuf(fp); // recreate buffer
    }

    // place character
    *(fp->ptr++) = character;
    --fp->cnt; // decrease number of cnt
    return character;
}

/*
 * Function to browse around the text file in UNIX system
 * Input:
 * - FILE *fp -> pointer to the file
 * - long offset -> offset from the origin where we want to be
 * - origin -> 0=start, 1=current, 2=end
 * Returns:
 * - int -> 0 if all good, otherwise not zero
 */
int fseek(FILE *fp, long offset, int origin) {
    int result = 0; // returns 0 if good, otherwise not zero
    switch (origin) {
        // start of the file
        case SEEK_SET:
            break;
        // where we are now
        case SEEK_CUR:
            break;
        // end of file
        case SEEK_END:
            break;
    }

    return result;
}
