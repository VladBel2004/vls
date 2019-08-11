#include "gio/gio.h"
#include "include/def.h"
#include "stdio.h"
#include "glib/gprintf.h"

GQueue *queue_path = NULL;

void queue_foreach( gpointer data, gpointer user_data )
{
        if( g_strcmp0( (gchar*)data, "/" ) == 0 )
        {
                printf( "\e[1;38;05;%sm%s\e[0m", dirc, (gchar*)data );
        }
        else
        {
                printf( "\e[1;38;05;%sm%s/\e[0m", dirc, (gchar*)data );
        }
}

static void queue_free_item( gpointer data )
{
        g_free( data );
}

int main( gint argc, gchar **argv )
{
        usr_init( &argc, argv );
        GFile **input = get_g_files_from_input( g_strv_length( argv ), argv );
        GError *error = NULL;

        for( gint i = 0; i < g_strv_length(argv)-1; i++ )
        {
                GFileInfo *info = g_file_query_info( input[i], "standard::*,unix::*,time::*,access::*,owner::*",\
                                G_FILE_QUERY_INFO_NOFOLLOW_SYMLINKS, NULL, &error );

                if( error != NULL )
                {
                        if( error->domain == G_IO_ERROR && error->code == G_IO_ERROR_NOT_FOUND )
                        {
                                gchar *file_path = g_file_get_path( input[i] );
                                USR_WARNIG( "File \e[38;05;9m%s\e[0m not found.", file_path );
                                g_free( file_path );
                        }

                        g_error_free( error );
                        error = NULL;
                        continue;
                }

                queue_path = g_queue_new();
                print_file_info( info, input[i] );
                g_queue_free( queue_path );
        }

        usr_deinit();

        return 0;
}
