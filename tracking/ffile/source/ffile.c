/*
 *  ffile.c
 */

#include "ffile.h"

static RACC_T ra;

#define ffile_prepare_access(p, o, ta, pd, pos, q) \
    ra.pf = (p); \
    ra.op = (o); \
    ra.tacc = (ta); \
    ra.ptofrom = (pd); \
    ra.currpos = (pos); \
    ra.rqty = (q)

void
ffile_init(void)
{
    flash_init();
    rfile_init_directory();
}

void
ffile_close(void)
{
    flash_close();
}

#if FF_GET_FILE_INFO == 1
FFILE_T *
ffile_get_file_info(FFD_T ffd)
{
    return rfile_get_file(ffd);
}
#endif

#if FF_FILE_FORMAT == 1
void
ffile_file_format(FFD_T ffd)
{
    FFILE_T *p;

    p = rfile_get_file(ffd);
    rfile_file_format(p);
}
#endif

#if FF_QUEUE_FILE == 1
FFUInt
ffile_queue_insert(FFD_T ffd, void *preg)
{
    FFILE_T *p;

    p = rfile_get_file(ffd);

    if (p->qty >= p->num_regs)
    {
        FFDBG_Q_FULL(p);
        return FQFILE_FULL;
    }

    ffile_prepare_access(p, WRITE_ACCESS, QFILE_ACCESS, preg, p->in, 1);
    rfile_access_register(&ra);
    FFDBG_Q_INSERT(p);

    if (++p->in >= p->num_regs)
    {
        p->in = 0;
    }

    ++p->qty;
    /*rfile_update_directory( p );*/
    return FQFILE_OK;
}
#endif

#if FF_QUEUE_FILE == 1
FFUInt
ffile_queue_remove(FFD_T ffd, void *preg)
{
    FFILE_T *p;

    p = rfile_get_file(ffd);

    if (p->qty == 0)
    {
        FFDBG_Q_EMPTY(p);
        return FQFILE_EMPTY;
    }

    ffile_prepare_access(p, READ_ACCESS, QFILE_ACCESS, preg, p->out, 1);
    rfile_access_register(&ra);
    FFDBG_Q_REMOVE(p);

    if (++p->out >= p->num_regs)
    {
        p->out = 0;
    }

    --p->qty;
    /*rfile_update_directory( p );*/
    return FQFILE_OK;
}

FFUInt
ffile_queue_delete(FFD_T ffd, NR_T *ndel)
{
    NR_T ntot, n;
    FFILE_T *p = rfile_get_file(ffd);

    if (p->qty == 0)
    {
        FFDBG_Q_EMPTY(p);
        return FQFILE_EMPTY;
    }

    if (*ndel > p->qty)     /* Limits the number of register to be deleted */
    {
        *ndel = p->qty;
    }

    ntot = *ndel;
    while (ntot != (NR_T)0)
    {
        n = (NR_T)(p->num_regs - p->out);   /* Registers until the end */

        if (n > p->qty)
        {
            n = p->qty;
        }

        if (n > ntot)
        {
            n = ntot;
        }

        p->out += n;
        p->qty -= n;
        ntot -= n;

        if (p->out == p->num_regs)
        {
            p->out = 0;
        }
    }
    /*rfile_update_directory( p );*/
    return FQFILE_OK;
}
#endif

int
ffile_random_access(FFD_T ffd, ffui8_t op, void *preg, ffui16_t rqty)
{
    FFILE_T *p;
    NR_T rregs_in_file, aqty;

    p = rfile_get_file(ffd);

    if (p->pos >= p->num_regs)
    {
        return -FRFILE_EOF;
    }

    rregs_in_file = p->num_regs - p->pos;
    aqty = rqty > rregs_in_file ? rregs_in_file : rqty;

    FFDBG_RND_ACCESS(p, op);
    ffile_prepare_access(p, op, RFILE_ACCESS, preg, p->pos, aqty);
    rfile_access_register(&ra);

    p->pos += aqty;

    /*rfile_update_directory( p );*/
    return (int)aqty;
}

#if FF_QUEUE_FILE == 1 && FF_QUEUE_RND_READ == 1
FFUInt
ffile_queue_random_read(FFD_T ffd, ffui8_t dir, void *preg)
{
    FFILE_T *p;

    p = rfile_get_file(ffd);

    if (p->pos_qty == 0)
    {
        FFDBG_Q_EMPTY(p);
        return FQFILE_EMPTY;
    }

    ffile_prepare_access(p, READ_ACCESS, RFILE_ACCESS, preg, p->pos, 1);
    rfile_access_register(&ra);
    FFDBG_Q_RND_READ(p);

    if (dir == READ_FORWARD)
    {
        if (++p->pos >= p->num_regs)
        {
            p->pos = 0;
        }
    }
    else
    {
        if ((short)--p->pos < 0)
        {
            p->pos = p->num_regs - 1;
        }
    }

    --p->pos_qty;
    return FQFILE_OK;
}
#endif

#if FF_QUEUE_FILE == 1 && FF_QUEUE_READ == 1
FFUInt
ffile_queue_read(FFD_T ffd, void *preg)
{
    FFILE_T *p;

    p = rfile_get_file(ffd);

    if (p->qty == 0)
    {
        FFDBG_Q_EMPTY(p);
        return FQFILE_EMPTY;
    }

    if (p->pos == p->in)
    {
        FFDBG_Q_EMPTY(p);
        return FQFILE_EMPTY;
    }

    ffile_prepare_access(p, READ_ACCESS, RFILE_ACCESS, preg, p->pos, 1);
    rfile_access_register(&ra);
    FFDBG_Q_READ(p);

    if (++p->pos >= p->num_regs)
    {
        p->pos = 0;
    }

    return FQFILE_OK;
}
#endif

#if FF_TELL == 1
ffui16_t
ffile_tell(FFD_T ffd)
{
    FFILE_T *p;
    NR_T size;

    p = rfile_get_file(ffd);

    size = (p->type == QFILE_TYPE) ? p->qty : p->pos;
    return (ffui16_t)size;
}
#endif

#if FF_SEEK == 1
void
ffile_seek(FFD_T ffd, ffui16_t offset)
{
    FFILE_T *p;

    p = rfile_get_file(ffd);

    p->pos = offset;
    FFDBG_SEEK(p);
    /*rfile_update_directory( p );*/
}
#endif

#if FF_QUEUE_FILE == 1
ffui16_t
ffile_queue_open_as_random(FFD_T ffd, ffui8_t from)
{
    FFILE_T *p;

    p = rfile_get_file(ffd);

    if (from == OPEN_FROM_BEGINNING)
    {
        p->pos = p->out;
    }
    else
    {
        p->pos = p->in == 0 ? p->num_regs - 1 : p->in - 1;
    }

    p->pos_qty = p->qty;
    FFDBG_Q_OPEN_AS_RND(p, from);
    /*rfile_update_directory( p );*/
    return p->pos_qty;
}
#endif

#if FF_CHECK_EOF == 1
FFUInt
ffile_is_eof(FFD_T ffd)
{
    FFILE_T *p;

    p = rfile_get_file(ffd);

    if (p->type == QFILE_TYPE)
    {
        return p->qty == 0;
    }
    else
    {
        return p->pos >= p->num_regs;
    }
}
#endif

#if FF_IS_CORRUPTED == 1
FFUInt
ffile_is_corrupted(FFD_T ffd)
{
    FFILE_T *p;

    p = rfile_get_file(ffd);
    return p->page_error == p->num_pages ? FRFILE_ERROR : FRFILE_OK;
}
#endif

void
ffile_sync(void)
{
    rfile_update_directory((FFILE_T *)0);
}
/* ------------------------------ End of file ------------------------------ */
