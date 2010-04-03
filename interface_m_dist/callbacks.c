#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <gtk/gtk.h>


////////////////////////////////////////////
///////////CALLBACKS-INTERFAZ///////////////
////////////////////////////////////////////

//* Our callback.
// * The data passed to this function is printed to stdout 
static void callback( GtkWidget *widget, gpointer   data ) {
    g_print ("Hello again - %s was pressed\n", (char *) data);
}

//* This callback quits the program 
bool on_quit_clicked( GtkButton *button, gpointer   data ) {
    gtk_main_quit ();
    return FALSE;
}


