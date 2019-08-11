#include "G2PP.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <gtk/gtk.h>

GtkWidget *entry_alpha;
GtkWidget *entry_beta;
GtkWidget *entry_sigma1;
GtkWidget *entry_sigma2;
GtkWidget *entry_rho;

GtkWidget *entry_zcbp;

static G2PP * new_g2pp() {
    return new G2PP();
}

static void delete_g2pp(G2PP * ptr) {
    delete ptr;
}

static void do_calibrate (GtkWidget *widget, gpointer data) { g_print ("Calibrating...Done\n"); }
static void do_calculate (GtkWidget *widget, gpointer data) {
    double alpha = atof((char *) gtk_entry_get_text (GTK_ENTRY(entry_alpha)));
    double beta = atof((char *) gtk_entry_get_text (GTK_ENTRY(entry_beta)));
    double sigma1 = atof((char *) gtk_entry_get_text (GTK_ENTRY(entry_sigma1)));
    double sigma2 = atof((char *) gtk_entry_get_text (GTK_ENTRY(entry_sigma2)));
    double rho = atof((char *) gtk_entry_get_text (GTK_ENTRY(entry_rho)));

    char result_buffer[12];
    snprintf(result_buffer, sizeof(result_buffer), "%f", alpha + beta + sigma1 + sigma2 + rho);
    gtk_entry_set_text(GTK_ENTRY(entry_zcbp), result_buffer);
}

int main (int argc, char **argv)
{

  GtkBuilder *builder;
  GtkWidget *window;
  GError *error = NULL;

  gtk_init (&argc, &argv);

  /* Construct a GtkBuilder instance and load our UI description */
  builder = gtk_builder_new ();
  if (gtk_builder_add_from_file (builder, "../ui/calculator.ui", &error) == 0)
  {
      g_printerr ("Error loading file: %s\n", error->message);
      g_clear_error (&error);
      return 1;
  }

  /* Connect signal handlers to the constructed widgets. */
  window = (GtkWidget *)gtk_builder_get_object (builder, "window");
  g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);

  GObject *button_cali = gtk_builder_get_object (builder, "button_calibrate");
  g_signal_connect (button_cali, "clicked", G_CALLBACK (do_calibrate), NULL);

  GObject *button_calc = gtk_builder_get_object (builder, "button_calculate");
  g_signal_connect (button_calc, "clicked", G_CALLBACK (do_calculate), NULL);

  GObject *button_quit = gtk_builder_get_object (builder, "quit");
  g_signal_connect (button_quit, "clicked", G_CALLBACK (gtk_main_quit), NULL);

  entry_alpha = (GtkWidget *)gtk_builder_get_object (builder, "entry_alpha");
  entry_beta = (GtkWidget *)gtk_builder_get_object (builder, "entry_beta");
  entry_sigma1 = (GtkWidget *)gtk_builder_get_object (builder, "entry_sigma1");
  entry_sigma2 = (GtkWidget *)gtk_builder_get_object (builder, "entry_sigma2");
  entry_rho = (GtkWidget *)gtk_builder_get_object (builder, "entry_rho");
  entry_zcbp = (GtkWidget *)gtk_builder_get_object (builder, "entry_zcbp");

  gtk_widget_show_all (window);
  gtk_main ();

  return 0;
}
#ifdef __cplusplus
}
#endif
