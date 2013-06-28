
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h> /* for calloc, free */
#include "header.h"

static void *local_calloc(size_t nmemb, size_t size) {
   void *p = sqlite3_malloc(nmemb*size);
   if (p == NULL)
      return NULL;
   return memset(p, 0, nmemb*size);
}

extern struct SN_env * SN_create_env(int S_size, int I_size, int B_size)
{
    struct SN_env * z = (struct SN_env *) local_calloc(1, sizeof(struct SN_env));
    if (z == NULL) return NULL;
    z->p = create_s();
    if (z->p == NULL) goto error;
    if (S_size)
    {
        int i;
        z->S = (symbol * *) local_calloc(S_size, sizeof(symbol *));
        if (z->S == NULL) goto error;

        for (i = 0; i < S_size; i++)
        {
            z->S[i] = create_s();
            if (z->S[i] == NULL) goto error;
        }
    }

    if (I_size)
    {
        z->I = (int *) local_calloc(I_size, sizeof(int));
        if (z->I == NULL) goto error;
    }

    if (B_size)
    {
        z->B = (unsigned char *) local_calloc(B_size, sizeof(unsigned char));
        if (z->B == NULL) goto error;
    }

    return z;
error:
    SN_close_env(z, S_size);
    return NULL;
}

extern void SN_close_env(struct SN_env * z, int S_size)
{
    if (z == NULL) return;
    if (S_size)
    {
        int i;
        for (i = 0; i < S_size; i++)
        {
            lose_s(z->S[i]);
        }
        sqlite3_free(z->S);
    }
    sqlite3_free(z->I);
    sqlite3_free(z->B);
    if (z->p) lose_s(z->p);
    sqlite3_free(z);
}

extern int SN_set_current(struct SN_env * z, int size, const symbol * s)
{
    int err = replace_s(z, 0, z->l, size, s, NULL);
    z->c = 0;
    return err;
}
