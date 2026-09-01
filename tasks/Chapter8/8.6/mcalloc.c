#include <stdio.h>

#define NALLOC 1024
typedef long Align;

union header 
{
    struct
    {
        union header *ptr; 
        unsigned size;
    } s;
    Align x; 
};
typedef union header Header;

static Header base;
static Header *freep = NULL; 

void *mycalloc(unsigned n, size_t size);
void *mymalloc(unsigned nbytes);
static Header *morecore(unsigned nunits);
void myfree(void *ap);
char *sbrk(int);

int main()
{
    long *a = mycalloc(1000, sizeof(long));
    for (int i = 0; i < 1000; i++)
        if (a[i] != 0)
        {
            printf("memory was not allocated\n");
            myfree(a);
            return -1;
        }
    myfree(a);
    printf("memory was allocated\n");
    return 0;
}

void *mycalloc(unsigned n, size_t size)
{
    char *p;
    if ((p = mymalloc(n * size)) == NULL) 
        return NULL;
    for (unsigned i = 0; i < n * size; i++) 
        p[i] = '\0';
    return p;
}

void *mymalloc(unsigned nbytes)
{
    Header *p, *prevp;
    unsigned nunits = (nbytes + sizeof(Header) - 1) / sizeof(Header) + 1;
    if ((prevp = freep) == NULL)
    {
        base.s.ptr = freep = prevp = &base;
        base.s.size = 0;
    }
    for (p = prevp->s.ptr; ; prevp = p, p = p->s.ptr)
    {
        if (p->s.size >= nunits)
        {
            if (p->s.size == nunits) 
                prevp->s.ptr = p->s.ptr; 
            else
            {
                p->s.size -= nunits; 
                p += p->s.size; 
                p->s.size = nunits; 
            }
            freep = prevp;
            return (void *)(p + 1);
        }
        if (p == freep) 
            if ((p = morecore(nunits)) == NULL) 
                return NULL;
    }
}


static Header *morecore(unsigned nunits)
{
    char *cp;
    Header *up; 
    if (nunits < NALLOC) 
        nunits = NALLOC;
    cp = sbrk(nunits * sizeof(Header)); 
    if (cp == (char *) -1) 
        return NULL;
    up = (Header*) cp; 
    up->s.size = nunits; 
    myfree((void *) (up + 1)); 
    return freep; 
}

void myfree(void *ap)
{
    Header *bp, *p;
    bp = (Header *) ap - 1;
    for (p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
        if (p >= p->s.ptr && (bp > p || bp < p->s.ptr))
            break;
    if (bp + bp->s.size == p->s.ptr)
    {
        bp->s.size += p->s.ptr->s.size; 
        bp->s.ptr = p->s.ptr->s.ptr; 
    }
    else
        bp->s.ptr = p->s.ptr;

    if (p + p->s.size == bp) 
    {
        p->s.size += bp->s.size; 
        p->s.ptr = bp->s.ptr;
    }
    else
        p->s.ptr = bp;
    freep = p; 
}
