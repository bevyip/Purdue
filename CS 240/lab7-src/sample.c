#include <gtk/gtk.h>

static void
cb_clicked( GObject     *obj,
            GtkEditable *edit )
{
    gint  pos = -1;
    gchar digit = (gchar)GPOINTER_TO_INT( g_object_get_data( obj, "num" ) );

    gtk_editable_insert_text( edit, &digit, 1, &pos );
}

int
main( int    argc,
      char **argv )
{
    GtkWidget *window,
              *table,
              *entry,
              *button;
    gint       i;

    gtk_init( &argc, &argv );

    window = gtk_window_new( GTK_WINDOW_TOPLEVEL );
    g_signal_connect( window, "destroy", gtk_main_quit, NULL );

    table = gtk_table_new( 4, 3, TRUE );
    gtk_container_add( GTK_CONTAINER( window ), table );

    entry = gtk_entry_new();
    gtk_table_attach( GTK_TABLE( table ), entry, 0, 3, 0, 1,
                      GTK_FILL, GTK_FILL, 0, 0 );

    for( i = 0; i < 9; i++ )
    {
        gint  row = i / 3,
              col = i % 3;
        gchar label[] = " ";

        label[0] = '1' + i;
        button = gtk_button_new_with_label( label );
        g_object_set_data( G_OBJECT( button ),
                           "num", GINT_TO_POINTER( '1' + i ) );
        g_signal_connect( button, "clicked", G_CALLBACK( cb_clicked ), entry );
        gtk_table_attach( GTK_TABLE( table ), button, col, col + 1,
                          row + 1 , row + 2, GTK_FILL, GTK_FILL, 0, 0 );
    }

    gtk_widget_show_all( window );

    gtk_main();

    return( 0 );
}
