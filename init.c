#include "glib.h"
#include "gio/gio.h"
#include "glib/gprintf.h"
#include "include/def.h"

static void usr_log_handler( const gchar *log_domain,
                                    GLogLevelFlags log_level,
                                    const gchar *message, 
                                    gpointer usre_data )
{
        if( log_level == G_LOG_LEVEL_WARNING )
        {
                g_printf( "\e[38;05;13;1mWARNING\e[0m: %s\n", message );
        }
}

static GOptionEntry entries[] = {
        { "all", 'a', 0, G_OPTION_ARG_NONE, &show_all_files, "Don't hide hidden files.", NULL },
        { "depth", 'D', 0, G_OPTION_ARG_INT, &max_depth, "Sets the recursion depth.", "N" },
        { "no-depth", 'd', 0, G_OPTION_ARG_NONE, &show_only_first_input, "Equivalent --depth=0", NULL },
        { "info", 'i', 0, G_OPTION_ARG_STRING, &info_format, "Sets format of vls output", "STR" },
        { "color-file", 'c', 0, G_OPTION_ARG_STRING, &color_file, "Set file from which colors will taken.", "STR" },
        { NULL }
};

void usr_init( gint *argc, gchar **argv )
{
        GOptionContext *parse_context = g_option_context_new( "Usage" );
        GError *error = NULL;
        g_option_context_set_summary( parse_context, "Some summary." );
        g_option_context_set_description( parse_context, "Some info about --info:\n"
                                                         "\ti - i-node\ts - file size\n"
                                                         "\tg - group\tr - rigth\n"
                                                         "\tu - user\tt - file type\n"
                                                         "\tm - modification time\n" );
        g_option_context_set_help_enabled( parse_context, TRUE );
        g_option_context_add_main_entries( parse_context, entries, NULL );

        if( g_option_context_parse( parse_context, argc, &argv, &error ) == FALSE )
        {
                g_printf( "\e[1;38;05;124mERROR\e[0m:%s\n", error->message );
                g_error_free( error );
                error = NULL;
        }

        g_option_context_free( parse_context );
                
        g_log_set_handler( "USR", G_LOG_LEVEL_WARNING, usr_log_handler, NULL );

        if( color_file != NULL )
        {
                GKeyFile *kf = g_key_file_new();
                GFile *key_file = g_file_new_for_path( color_file );
                gchar *key_value = NULL;

                if( g_file_query_exists( key_file, NULL ) )
                {
                        if( !g_key_file_load_from_file( kf, color_file, 0, &error ) )
                        {
                                if( error != NULL )
                                {
                                        g_printf( "\e[1;38;05;196mERROR\e[0m: %s\n", error->message );
                                        g_error_free( error );
                                        error = NULL;
                                }
                        }

                        if( g_key_file_has_group( kf, "colors" ) )
                        {
                                if( g_key_file_has_key( kf, "colors", "regc", NULL ) )
                                {
                                        regc = g_key_file_get_string( kf, "colors", "regc", NULL );
                                }

                                if( g_key_file_has_key( kf, "colors", "dirc", NULL ) )
                                {
                                        dirc = g_key_file_get_string( kf, "colors", "dirc", NULL );
                                }

                                if( g_key_file_has_key( kf, "colors", "spec", NULL ) )
                                {
                                        spec = g_key_file_get_string( kf, "colors", "spec", NULL );
                                }

                                if( g_key_file_has_key( kf, "colors", "slkc", NULL ) )
                                {
                                        slkc = g_key_file_get_string( kf, "colors", "slkc", NULL );
                                }

                                if( g_key_file_has_key( kf, "colors", "sltc", NULL ) )
                                {
                                        sltc = g_key_file_get_string( kf, "colors", "sltc", NULL );
                                }
                        }
                        else
                        {
                                USR_WARNIG( "file %s has not \"file colors\" group.", color_file );
                        }
                }
                else 
                {
                        g_printf( "\e[1;38;05;196mERROR\e[0m: file %s net exist.\n", color_file );
                }

                g_key_file_unref( kf );
                g_object_unref( key_file );
        }
}

void usr_deinit()
{
        if( g_strcmp0( regc, "159" ) != 0 )
        {
                g_free( regc );
        }

        if( g_strcmp0( dirc, "111" ) != 0 )
        {
                g_free( dirc );
        }

        if( g_strcmp0( slkc, "208" ) != 0 )
        {
                g_free( slkc );
        }

        if( g_strcmp0( sltc, "214" ) != 0 )
        {
                g_free( sltc );
        }

        if( g_strcmp0( spec, "115" ) != 0 )
        {
                g_free( spec );
        }
}

GFile** get_g_files_from_input( gint argc, gchar **argv )
{
        GFile **input = g_slice_alloc0( sizeof(GFile*)*argc-1 );

        for( gint i = 0; i < argc-1; i++ )
        {
                input[i] = g_file_new_for_path( argv[i+1] );
        }

        return input;
}
