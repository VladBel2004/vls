#ifndef __DEF
#define __DFE

#define USR_WARNIG( message, ... ) ( g_log( "USR", G_LOG_LEVEL_WARNING, message, __VA_ARGS__ ) )

// File Info 
typedef struct {
        GFile *file;
        GFileInfo *info;
} file_and_info;

// App options
extern gboolean show_all_files;
extern gboolean show_only_first_input;
extern gint32 max_depth;
extern gchar *info_format;
extern gchar *color_file;

// .ini config
extern gchar *dirc;
extern gchar *regc;
extern gchar *spec;
extern gchar *slkc;
extern gchar *sltc;

// Queue of Path
extern GQueue *queue_path;

GFile** get_g_files_from_input( gint argc, gchar **argv );
void usr_init( gint *argc, gchar **argv );
void usr_deinit(void);
void print_file_info( GFileInfo *info, GFile *file );
gchar* str_reverse( gchar *str, guint64 len );

// Queue Functions
void queue_foreach( gpointer data, gpointer user_data );

void buffer_add( gchar *path );
gchar* buffer_print(void);

#endif
