#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>
#include "list.h"
#include "lf.h"
#include "common.h"
#include "color.h"

lf_option LFopt;
char *LFpath;
char *LFbuf;
int LFpathsiz;
linklist LFcolorlist;

bool is_digit(const char *nm, int n)
{
    if (!nm || n <= 0)
    {
        return false;
    }
    while (n)
    {
        if (*nm < '0' || *nm > '9')
        {
            break;
        }
        --n;
    }
    return (n) ? false : true;
}
char *filter(char *s, int n)
{
    int k = 0;
    bool repeat;
    char *tmp;

    tmp = (char *)malloc(sizeof(char) * n);
    for (char *c = s; *c && k < n; ++c)
    {
        repeat = false;
        for (char *d = s; d < c; ++d)
        {
            if (*d == *c)
            {
                repeat = true;
            }
        }
        if (!repeat)
        {
            tmp[k] = *s;
            ++k;
        }
    }
    if (k < n)
    {
        tmp = (char *)realloc(tmp, sizeof(char) * (k + 1));
        tmp[k + 1] = 0;
    }
    // case k==n nothing to do.
    // case k>n doesn't exist here.
    return tmp;
}

int set_t_arg(char *s[], int n, int *i, int j)
{
    // if 't' is at  the end of string
    // and there's a argument
    // and it's a number
    if ((j == strlen(s[*i]) - 1) &&
        (*i < n - 1))
    {
        if (is_digit(s[*i + 1], strlen(s[*i])))
        {
            ++(*i);
            return strtol(s[*i], NULL, 10);
        }
    }
    return 0;
}

char set_l_arg(char *s[], int n, int *i, int j)
{
    if ((j == strlen(s[*i]) - 1) &&
        (*i < n - 1))
    {
        // its length is 1
        if (strlen(s[*i + 1]) == 1)
        {
            ++(*i);
            return s[*i][0];
        }
    }
    return '\n';
}

char set_s_arg(char *s[], int n, int *i, int j)
{
    // if 's' is at  the end of string
    // and there's a argument
    if ((j == strlen(s[*i]) - 1) &&
        (*i < n - 1))
    {
        // its length is 1
        if (strlen(s[*i + 1]) == 1)
        {
            char c = s[*i + 1][0];
            if ((c == 'i') ||
                (c == 'l') ||
                (c == 'u') ||
                (c == 'g') ||
                (c == 's') ||
                (c == 'a') ||
                (c == 'm') ||
                (c == 'c') ||
                (c == 't') ||
                (c == 'e'))
            {
                ++(*i);
                return c;
            }
        }
    }
    // by default, sort by name
    return 0;
}

m_arg *set_m_arg(char *s[], int n, int *i, int j)
{
    bool ok = true;

    if ((j == strlen(s[*i]) - 1) &&
        (*i < n - 1))
    {
        char *c = s[*i + 1];
        while (*c && ok)
        { // verify if all characters are correct.
            if ((*c != 'b') &&
                (*c != 'c') &&
                (*c != 'd') &&
                (*c != 'p') &&
                (*c != 'l') &&
                (*c != 'f') &&
                (*c != 's') &&
                (*c != 'u') &&
                (*c != 'g') &&
                (*c != 't') &&
                (*c != 'r') &&
                (*c != 'w') &&
                (*c != 'x'))
            {
                ok = false;
            }
            else
            {
                ++c;
            }
        }
        if (ok)
        {
            ++(*i);
            m_arg *m = lf_alloc(M_ARG_SIZ);
            memset(m, 0, M_ARG_SIZ);
            for (int j = 0; j < strlen(s[*i]); ++j)
            {
                switch (s[*i][j])
                {
                case 'b':
                    m->b = true;
                    break;
                case 'c':
                    m->c = true;
                    break;
                case 'd':
                    m->d = true;
                    break;
                case 'p':
                    m->f = true;
                    break;
                case 'l':
                    m->l = true;
                    break;
                case 'f':
                    m->r = true;
                    break;
                case 's':
                    m->s = true;
                    break;
                case 'u':
                    m->u = true;
                    break;
                case 'g':
                    m->g = true;
                    break;
                case 't':
                    m->t = true;
                    break;
                case 'r':
                    m->r = true;
                    break;
                case 'w':
                    m->w = true;
                    break;
                case 'x':
                    m->x = true;
                    break;
                default:
                    break;
                }
            }
            return m;
        }
    }
    return NULL;
}

i_arg *set_i_arg(char *s[], int n, int *i, int j)
{
    bool ok = true;

    if ((j == strlen(s[*i]) - 1) &&
        (*i < n - 1))
    {
        char *c = s[*i + 1];
        while (*c && ok)
        {
            if ((*c != 'i') &&
                (*c != 'l') &&
                (*c != 'u') &&
                (*c != 'g') &&
                (*c != 's') &&
                (*c != 'p') &&
                (*c != 'a') &&
                (*c != 'm') &&
                (*c != 'c'))
            {
                ok = false;
            }
            else
            {
                ++c;
            }
        }
        if (ok)
        {
            ++(*i);
            i_arg *l = (i_arg *)lf_alloc(L_ARG_SIZ);
            memset(l, 0, L_ARG_SIZ);
            for (int j = 0; j < strlen(s[*i]); ++j)
            {
                switch (s[*i][j])
                {
                case 'i':
                    l->i = true;
                    break;
                case 'l':
                    l->l = true;
                    break;
                case 'u':
                    l->u = true;
                    break;
                case 'g':
                    l->g = true;
                    break;
                case 's':
                    l->s = true;
                    break;
                case 'p':
                    l->p = true;
                    break;
                case 'a':
                    l->a = true;
                    break;
                case 'm':
                    l->m = true;
                    break;
                case 'c':
                    l->c = true;
                    break;
                default:
                    break;
                }
            }
            return l;
        }
    }
    return NULL;
}

n_arg *set_n_arg(char *s[], int n, int *i, int j)
{
    bool ok = true;

    if ((j == strlen(s[*i]) - 1) &&
        (*i < n - 1))
    {
        char *c = s[*i + 1];
        while (*c && ok)
        {
            if ((*c != 'f') &&
                (*c != 'q') &&
                (*c != 's'))
            {
                ok = false;
            }
            else
            {
                ++c;
            }
        }
        if (ok)
        {
            ++(*i);
            n_arg *nn = (n_arg *)lf_alloc(N_ARG_SIZ);
            memset(nn, 0, N_ARG_SIZ);
            for (int j = 0; j < strlen(s[*i]); ++j)
            {
                switch (s[*i][j])
                {
                case 'f':
                    nn->f = true;
                    break;
                case 'q':
                    nn->q = true;
                    break;
                case 's':
                    nn->s = true;
                    break;
                default:
                    break;
                }
            }
            return nn;
        }
    }
    return NULL;
}

void set_options(int argc, char *argv[], linklist l)
{
    int tmp;
    bool ok = true;

    for (int i = 1; i < argc; ++i)
    {
        if (argv[i][0] == '-')
        {
            tmp = strlen(argv[i]);
            for (int j = 1; j < tmp; ++j)
            {
                switch (argv[i][j])
                {
                case '!':
                    LFopt.arg = true;
                    break;
                case '0':
                    LFopt.zero = true;
                    break;
                case '1':
                    LFopt.one = true;
                    break;
                case '2':
                    LFopt.two = true;
                    break;
                case 'a':
                    LFopt.a = true;
                    break;
                case 'c':
                    LFopt.c = true;
                    break;
                case 'r':
                    LFopt.r = true;
                    break;
                case 'v':
                    LFopt.v = true;
                    break;
                case 'h':
                    LFopt.h = true;
                    break;
                case 't':
                    LFopt.t = true;
                    LFopt.td = set_t_arg(argv, argc, &i, j);
                    break;
                case 's':
                    LFopt.s = true;
                    LFopt.sc = set_s_arg(argv, argc, &i, j);
                    break;
                case 'm':
                    LFopt.m = true;
                    LFopt.ml = set_m_arg(argv, argc, &i, j);
                    break;
                case 'i':
                    LFopt.i = true;
                    LFopt.il = set_i_arg(argv, argc, &i, j);
                    break;
                case 'n':
                    LFopt.n = true;
                    LFopt.nl = set_n_arg(argv, argc, &i, j);

                    break;
                default:
                    ok = false;
                    printf("%s: Invalid command \"%c\"\n", PROGRAM, argv[i][j]);
                    break;
                }
            }
        }
        else
        {
            ladd(l, LLAST, argv[i]);
        }
    }
    if (!ok)
    {
        lf_quit();
    }
}

int main(int argc, char *argv[], char *envp[])
{
    linklist l = lopen();

    lf_init();
    set_options(argc, argv, l);
    if (lempty(l))
    {
        ladd(l, LFIRST, "./");
    }
    if (LFopt.h)
    {
        help(0);
    }
    else if (LFopt.v)
    {
        version();
    }
    else if (LFopt.t + LFopt.i + LFopt.zero + LFopt.one + LFopt.two > true)
    {
        printf("%s: The 't', 'i', '0', '1' and '2' options should be used separately.\n", PROGRAM);
        lf_quit();
    }
    else
    {
        if (!LFopt.nl)
        {
            LFopt.nl = (n_arg *)lf_alloc(N_ARG_SIZ);
            memset(LFopt.nl, 0, N_ARG_SIZ);
        }
        if (!LFopt.ml)
        {
            LFopt.ml = (m_arg *)lf_alloc(M_ARG_SIZ);
            LFopt.ml->b = true;
            LFopt.ml->c = true;
            LFopt.ml->d = true;
            LFopt.ml->p = true;
            LFopt.ml->l = true;
            LFopt.ml->f = true;
            LFopt.ml->s = true;
            LFopt.ml->u = true;
            LFopt.ml->g = true;
            LFopt.ml->t = true;
            LFopt.ml->r = true;
            LFopt.ml->w = true;
            LFopt.ml->x = true;
        }
        if (LFopt.i)
        {
            if (LFopt.il)
            {
                if (LFopt.il->i + LFopt.il->l +
                        LFopt.il->s + LFopt.il->p +
                        LFopt.il->u + LFopt.il->g +
                        LFopt.il->a + LFopt.il->m +
                        LFopt.il->c ==
                    true)
                {
                    LFopt.nl->f = false;
                }
                else
                {
                    LFopt.nl->f = true;
                }
            }
            else
            {
                LFopt.nl->f = true;
                LFopt.il = (i_arg *)lf_alloc(L_ARG_SIZ);
                LFopt.il->i = true;
                LFopt.il->l = true;
                LFopt.il->u = true;
                LFopt.il->g = true;
                LFopt.il->p = true;
                LFopt.il->s = true;
                LFopt.il->m = true;
            }
        }
        if (LFopt.t)
        {
            LFopt.nl->f = true;
        }
        if (LFopt.c)
        {
            LFcolorlist = scan_for_color();
            if (lempty(LFcolorlist))
            {
                printf("%s: warning: \"-c\" not available because the \"LS_COLORS\" environment variable is not set.\n", PROGRAM);
                LFopt.c = false;
            }
            else if (!getcolor(LFcolorlist, "rs", false))
            { // at least LS_COLORS must have value for "rs"
                printf("%s: warning: \"-c\" not available because the environment variable \"LS_COLORS\" has no value \"rs\".\n", PROGRAM);
                LFopt.c = false;
            }
        }
        run(l);
    }
    lf_quit(l);
    lclose(l);
    return 0;
}