#include "gio/gio.h"
#include "glib.h"
#include "include/def.h"
#include "stdio.h"

#define BUF_INIT_SIZE 100

static gchar *buffer = NULL;

void buffer_add( gchar *path )
{
        buffer = g_strconcat( buffer, path );
}

gchar* buffer_print(void)
{
        printf( "%s\t", buffer );
        g_free( buffer );
}
