#include "gio/gio.h"
#include "glib.h"
#include "include/def.h"
#include "string.h"
#include "math.h"
#include "glib/gprintf.h"

gboolean show_all_files = FALSE;
gboolean show_only_first_input = FALSE;
gint32 max_depth = 2;
gchar *info_format = "p";
gchar *color_file = NULL;

// .ini config 
gchar *dirc = "111";
gchar *spec = "115";
gchar *regc = "159";
gchar *slkc = "208";
gchar *sltc = "214";

gchar *str_reverse( gchar *str, guint64 len )
{
        guint64 dlen = len;
        gchar *nstr = g_malloc( len*sizeof(gchar)+1 );
        guint64 count = 0;

        for( len; len > 0; len-- )
        {
                nstr[count] = str[len-1];
                count++;
        }
        nstr[dlen] = '\0';

        g_free( str );
        return nstr;
}

static gint list_comp_func( gconstpointer a, gconstpointer b )
{
        file_and_info *fai_a = (file_and_info*)a;
        file_and_info *fai_b = (file_and_info*)b;

        GFileType type_a = g_file_info_get_file_type( fai_a->info );
        GFileType type_b = g_file_info_get_file_type( fai_b->info );

        if( type_a == type_b )
        {
                return 0;
        }
        else if( type_a != G_FILE_TYPE_DIRECTORY && type_b == G_FILE_TYPE_DIRECTORY )
        {
                return -1;
        }
        else if( type_a == G_FILE_TYPE_DIRECTORY && type_b != G_FILE_TYPE_DIRECTORY )
        {
                return 1;
        }
}

static void list_foreach( gpointer data, gpointer user_data )
{
        file_and_info *fai = (file_and_info*)data;
        print_file_info( fai->info, fai->file );
}

static void print_mode( guint32 mode )
{
        gchar *smode = g_malloc(9);
        if( mode & 1 == 1 )
        {
                smode[0] = 'x';
        }
        else if( !( mode & 1 ) )
        {
                smode[0] = '-';
        }
        
        mode>>=1;
        if( mode & 1 == 1 )
        {
                smode[1] = 'w';
        }
        else if( !( mode & 1 ) )
        {
                smode[1] = '-';
        }
        
        mode>>=1;
        if( mode & 1 == 1 )
        {
                smode[2] = 'r';
        }
        else if( !( mode & 1 ) )
        {
                smode[2] = '-';
        }
        
        mode>>=1;
        if( mode & 1 == 1 )
        {
                smode[3] = 'x';
        }
        else if( !( mode &1 ) )
        {
                smode[3] = '-';
        }
        
        mode>>=1;
        if( mode & 1 == 1 )
        {
                smode[4] = 'w';
        }
        else if( !( mode & 1 ) )
        {
                smode[4] = '-';
        }
        
        mode>>=1;
        if( mode & 1 == 1 )
        {
                smode[5] = 'r';
        }
        else if( !( mode & 1 ) )
        {
                smode[5] = '-';
        }
        
        mode>>=1;
        if( mode & 1 == 1 )
        {
                smode[6] = 'x';
        }
        else if( !( mode & 1 ) )
        {
                smode[6] = '-';
        }
        
        mode>>=1;
        if( mode & 1 == 1 )
        {
                smode[7] = 'w';
        }
        else if( !( mode & 1 ) )
        {
                smode[7] = '-';
        }
        
        mode>>=1;
        if( mode & 1 == 1 )
        {
                smode[8] = 'r';
        }
        else if( !( mode & 1 ) )
        {
                smode[8] = '-';
        }
        
        gchar *nstr = str_reverse( smode, 9 );
        g_printf( "%s\t", nstr );
        g_free( nstr );
}

void print_file_info( GFileInfo *info, GFile *file )
{
        guint32 type = g_file_info_get_file_type( info ); 
        guint64 inode = g_file_info_get_attribute_uint64( info, "unix::inode" );
        const gchar *user = g_file_info_get_attribute_string( info, "owner::user" );
        const gchar *group = g_file_info_get_attribute_string( info, "owner::group" );
        guint64 time_modif = g_file_info_get_attribute_uint64( info, "time::modified" );
        guint64 size = g_file_info_get_attribute_uint64( info, "standard::size" );
        guint32 mode = g_file_info_get_attribute_uint32( info, "unix::mode" );
        GError *error = NULL;

        if( show_all_files == FALSE && g_file_info_get_name( info )[0] == '.' )
        {
                return;
        }               
        if( show_only_first_input == TRUE && g_queue_get_length( queue_path ) >= 1 )
        {
                return;
        }
        else if( g_queue_get_length( queue_path ) >= max_depth )
        {
                return;
        }


        for( gint i = 0; i < strlen( info_format ); i++ )
        {
                if( info_format[i] == 'i' )
                {
                        g_printf( "%lu\t", inode );
                }
                else if( info_format[i] == 's' )
                {
#ifdef DEBUG
                        G_BREAKPOINT();
#endif
                        if( (long double)size/pow( 2, 10 ) < 1 )
                        {
                                g_printf( "%i\t", size );
                        }
                        else if( (long double)size/pow( 2, 10 ) < pow( 2, 10 ) )
                        {
                                if( fmod( (double)size, pow( 2, 10 ) ) == 0 )
                                {
                                        g_printf( "%iK\t", (int)( size/pow( 2, 10 ) ) );
                                        continue;
                                }

                                g_printf( "%.2fK\t", (float)size/pow( 2, 10 ) );
                        }
                        else if( (long double)size/pow( 2, 20 ) < pow( 2, 10 ) )
                        {
                                if( fmod( (double)size, pow( 2, 20 ) ) == 0 )
                                {
                                        g_printf( "%iM\t", (int)( size/pow( 2, 20 ) ) );
                                        continue;
                                }

                                g_printf( "%.2fM\t", (float)size/pow( 2, 20 ) );
                        }
                        else if( (long double)size/pow(2, 30) < pow( 2, 10 ) )
                        {
                                if( fmod( (double)size, pow( 2, 30 ) ) == 0 )
                                {
                                        g_printf( "%iG\t", (int)( size/pow( 2, 30 ) ) );
                                        continue;
                                }

                                g_printf( "%.2fG\t", (float)size/pow( 2, 30 ) );
                        }
                        else 
                        {
                                if( fmod( (double)size, pow( 2, 40 ) ) == 0 )
                                {
                                        g_printf( "%iT\t", (int)( size/pow( 2, 40 ) )  );
                                        continue;
                                }

                                g_printf( "%.2fT\t", (float)size/pow( 2, 40 ) );
                        }
                }
                else if( info_format[i] == 'u' )
                {
                        g_printf( "%s\t", user );
                }
                else if( info_format[i] == 'g' )
                {
                        g_printf( "%s\t", group );
                }
                else if( info_format[i] == 'r' )
                {
                        print_mode( mode );
                }
                else if( info_format[i] == 't' )
                {
                        if( type == G_FILE_TYPE_REGULAR )
                        {
                                g_printf( "\e[1;38;05;%smreg\e[0m\t", regc );
                        }
                        else if( type == G_FILE_TYPE_DIRECTORY )
                        {
                                g_printf( "\e[1;38;05;%smdir\e[0m\t", dirc );
                        }
                        else if( type == G_FILE_TYPE_SYMBOLIC_LINK )
                        {
                                g_printf( "\e[1;38;05;%smslk\e[0m\t", slkc );
                        }
                        else if( type == G_FILE_TYPE_SPECIAL )
                        {
                                g_printf( "\e[1;38;05;%smspc\e[0m\t", spec );
                        }
                }
                else if( info_format[i] == 'm' )
                {
                        GDateTime *gdt = g_date_time_new_from_unix_local( time_modif );
                        gchar *ft = g_date_time_format( gdt, "%H:%M %d-%m-%Y" );

                        g_printf( "%s\t", ft );

                        g_free( ft );
                        g_date_time_unref( gdt );
                }
                
        }

        if( type == G_FILE_TYPE_REGULAR )
        {
                if( g_queue_get_length( queue_path ) != 0 )
                {
                        g_queue_foreach( queue_path, queue_foreach, NULL );
                }
                g_printf( "\e[1;38;05;%sm%s\e[0m\n", regc,g_file_info_get_name( info ) );
                // Make unref for info and file intanseces.
        }
        else if( type == G_FILE_TYPE_SYMBOLIC_LINK )
        {
                if( g_queue_get_length( queue_path ) != 0 )
                {
                        g_queue_foreach( queue_path, queue_foreach, NULL );
                }
                g_printf( "\e[1;38;05;%sm%s\e[0m \e[1;38;05;231m->\e[0m \e[1;38;05;%sm%s\e[0m\n", slkc,
                                g_file_info_get_name( info ), sltc, g_file_info_get_symlink_target( info ) );
        }
        else if( type == G_FILE_TYPE_SPECIAL )
        {
                if( g_queue_get_length( queue_path ) != 0 )
                {
                        g_queue_foreach( queue_path, queue_foreach, NULL );
                }
                g_printf( "\e[1;38;05;%sm%s\e[0m\n", spec, g_file_info_get_name( info ) );
        }
        else if( type == G_FILE_TYPE_DIRECTORY )
        {
                g_queue_push_tail( queue_path, (gpointer)g_file_info_get_name( info ) );
                g_queue_foreach( queue_path, queue_foreach, NULL );
                g_printf( "\n" );

                guint64 count = 0;
                GList *file_list = NULL;
                GFileEnumerator *file_enum = g_file_enumerate_children( file, 
                                "standard::*,unix::*,time::*,access::*,owner::*",
                                1, NULL, &error );
                if( error != NULL )
                {
                        g_printf( "%s\n", error->message );
                        g_error_free( error );
                        error = NULL;
                }

                while( TRUE )
                {
                        GFile *child_file = NULL;
                        GFileInfo *child_info = NULL;

                        if( g_file_enumerator_iterate( file_enum, &child_info, &child_file, NULL, NULL ) == 1 &&
                                        child_info == NULL && child_file == NULL )
                        {
                                break;
                        }
                        
                        count++;
                        file_and_info *fai = g_malloc( sizeof( file_and_info ) );
                        fai->file = g_file_dup( child_file );
                        fai->info = g_file_info_dup( child_info );

                        if( file_list == NULL )
                        {
                                file_list = g_list_append( NULL, fai );
                        }
                        else 
                        {
                                file_list = g_list_append( file_list, fai );
                        }
                }

                if( count != 0 )
                {
                        file_list = g_list_sort( file_list, list_comp_func );
                        g_list_foreach( file_list, list_foreach, NULL );
                        g_list_free( file_list );
                }

                g_queue_pop_tail( queue_path );
                g_object_unref( file_enum );
        }
}
