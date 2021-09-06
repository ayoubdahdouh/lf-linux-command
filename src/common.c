#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/stat.h>
#include <errno.h>
#include "common.h"
#include "list.h"

void help(char h)
{
    if (!h)
    {
        printf("Usage:\n");
        printf("     %s -[OPTION]=[ARGUMENT],... [FILE]...\n\n", PROGRAM);
        printf("Options:\n");
        printf("    it's possible to combine those lettre together\n\n");
        printf("    -0  separate files with a space.\n");
        printf("    -1  lists one file per line.\n");
        printf("    -2  separate files with commas.\n");
        printf("    -3  separate files with a semicolon.\n");
        printf("    -a  show hidden files.\n");
        printf("    -[...]m=[bcdflrsugtrwx],[...]\n");
        printf("          File's mode, choose the files to list from the list below:\n");
        printf("            b  block device\n");
        printf("            c  character device\n");
        printf("            d  directory\n");
        printf("            p  FIFO/pipe\n");
        printf("            l  symlink\n");
        printf("            f  regular file\n");
        printf("            s  socket\n");
        printf("            u  set-user identification (SUID)\n");
        printf("            g  set-group identification (SGID)\n");
        printf("            t  sticky bit\n");
        printf("            r  read by owner\n");
        printf("            w  write by owner\n");
        printf("            x  execute by owner\n");
        printf("          By default, \"-m\" displays all files and folders except the hidden ones.\n");
        printf("          wich has the same effect as not using the \"-m\" option.\n");
        printf("    -[...]l=[inpsugamc],[...]\n");
        printf("          File's information, choose the information to display from the list below:\n");
        printf("            i  inode number\n");
        printf("            n  number of hard links\n");
        printf("            p  permissions\n");
        printf("            s  size\n");
        printf("            u  file owner\n");
        printf("            g  file group\n");
        printf("            a  last access\n");
        printf("            m  last modification\n");
        printf("            c  last status change\n");
        printf("          By default, \"-l\" show i,n,p,s,m if no argument is set\n");
        printf("    -[...]s=[insugamcte],[...]\n");
        printf("          Sort the output\n");
        printf("            i  inode number\n");
        printf("            n  number of hard links\n");
        printf("            s  size\n");
        printf("            u  file owner\n");
        printf("            g  file group\n");
        printf("            a  last access\n");
        printf("            m  last modification\n");
        printf("            c  last status change\n");
        printf("            t  file type\n");
        printf("            e  file extension\n");
        printf("          By default, %s will sort the output by name,\n", PROGRAM);
        printf("          But if you invoke \"-s\" without giving any arguments, you disable sorting of the output.\n");
        printf("    -[...]n=[bfqs],[...]\n");
        printf("            b  adds a backslash to the string containing spaces\n");
        printf("            f  follow link\n");
        printf("            q  display the name in quotes\n");
        printf("            s  display the folder name with a slash\n");
        printf("    -[...]t=[DEPTH]\n");
        printf("             DEPTH  tree depth\n");
        printf("          By default (without DEPTH), the depth is unlimited.\n");
        printf("    -r  make the size readable like 7K, 423M, 2G etc.\n");
        printf("    -c  color the output.\n");
        printf("    -h  print help.\n");
        printf("    -v  print version information.\n");
    }
}

void version()
{
    printf("Version: %s %s.\n", PROGRAM, VERSION);
}

void *lf_alloc(long int size)
{
    void *b;
    b = malloc(size);
    if (!b)
    {
        printf("%s: %s\n", PROGRAM, strerror(errno));
        exit(EXIT_FAILURE);
    }
    return b;
}
// places the contents of the symbolic link pathname in the buffer "buf" (global variable).
// if failed, also print error in the buffer "buf"
bool lf_link(const char *nm)
{
    int cnt;
    if (!nm)
    {
        return false;
    }
    cnt = readlink(nm, LFbuf, PATH_MAX - 1);
    if (cnt == -1)
    {
        strcpy(LFbuf, strerror(errno));
        return false;
    }
    LFbuf[cnt] = 0;
    return true;
}
// if failed, also print error in the buffer "buf"
bool lf_stat(const char *nm, struct stat *s)
{
    if (lstat(nm, s) == -1)
    {
        // strcpy(buf, strerror(errno));
        return false;
    }
    return true;
}

void lf_init()
{
    setbuf(stdout, NULL);
    // init PATH
    LFpath = (char *)lf_alloc(sizeof(char) * PATH_MAX);
    // init BUF
    LFbuf = (char *)lf_alloc(sizeof(char) * PATH_MAX);
}

void lf_quit()
{
    free(LFpath);
    free(LFbuf);
    exit(EXIT_SUCCESS);
}

int has_space(char *nm)
{
    int i = 0;
    for (; *nm; ++nm)
    {
        if (*nm == ' ')
        {
            ++i;
        }
    }
    return i;
}

bool is_absolute_path(const char *p)
{
    if (p)
    {
        return (p[0] == '/');
    }
    return false;
}

char *fileextension(char *s)
{
    // not null and not empty
    if (s && *s)
    {
        char *c = s + strlen(s) - 1;
        while (c >= s && *c != '.' && *c != '/')
        {
            --c;
        }
        if (*c == '.')
        {
            return c;
        }
    }
    return NULL;
}
char filetype(mode_t *m)
{
    char c;
    switch (*m & S_IFMT)
    {
    case S_IFBLK:
        c = 'b';
        break;
    case S_IFCHR:
        c = 'c';
        break;
    case S_IFDIR:
        c = 'd';
        break;
    case S_IFIFO:
        c = 'p';
        break;
    case S_IFLNK:
        c = 'l';
        break;
    case S_IFREG:
        c = '~';
        break;
    case S_IFSOCK:
        c = 's';
        break;
    default:
        c = 0;
        break;
    }
    return c;
}