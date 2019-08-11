#include "G2PP.h"

static const size_t N = 11;
static double stl[N]={1./12., 0.25, .5, 1., 2., 3., 5., 7., 10., 20., 30.};
static double price [N]={
    exp(-.0193*stl[0]), exp(-.0203*stl[1]), exp(-.0222*stl[2]),
    exp(-.0245*stl[3]), exp(-.0267*stl[4]), exp(-.0278*stl[5]),
    exp(-.0287*stl[6]), exp(-.0296*stl[7]), exp(-.0300*stl[8]),
    exp(-.0307*stl[9]), exp(-.0313*stl[10])
};

static Curve yc(stl, price, N);
static G2PP * g2pp_ptr = nullptr;

#ifdef __cplusplus
extern "C" {
#endif

#include <gtk/gtk.h>

static GtkWidget *entry_alpha;
static GtkWidget *entry_beta;
static GtkWidget *entry_sigma1;
static GtkWidget *entry_sigma2;
static GtkWidget *entry_rho;

static GtkWidget *entry_zcbp;
static GtkWidget *entry_stl;
static GtkWidget *entry_mat;


static void initiate_g2pp() { if (!g2pp_ptr) g2pp_ptr = new G2PP(); }
static void terminate_g2pp() { if (g2pp_ptr) delete g2pp_ptr; }
static void configure_g2pp(double a, double b, double s1, double s2, double ro)
{
    if (g2pp_ptr) {
        g2pp_ptr->setYieldCurve(&yc);
        g2pp_ptr->setParameter(G2::A, a);
        g2pp_ptr->setParameter(G2::B, b);
        g2pp_ptr->setParameter(G2::SIGMA_1, s1);
        g2pp_ptr->setParameter(G2::SIGMA_2, s2);
        g2pp_ptr->setParameter(G2::RHO, ro);
        g2pp_ptr->setPeriphery(G2::NTERMS, 1);
        g2pp_ptr->setPeriphery(G2::NPATHS, 977777);
        g2pp_ptr->setPeriphery(G2::NDIMS, 2);
        g2pp_ptr->setPeriphery(G2::NTHREADS, 12);
        g_print ("configuring...Done\n");
    }
}

static void calculate (GtkWidget *widget, gpointer data) {
    double alpha = atof((char *) gtk_entry_get_text (GTK_ENTRY(entry_alpha)));
    double beta = atof((char *) gtk_entry_get_text (GTK_ENTRY(entry_beta)));
    double sigma1 = atof((char *) gtk_entry_get_text (GTK_ENTRY(entry_sigma1)));
    double sigma2 = atof((char *) gtk_entry_get_text (GTK_ENTRY(entry_sigma2)));
    double rho = atof((char *) gtk_entry_get_text (GTK_ENTRY(entry_rho)));
    double stl = atof((char *) gtk_entry_get_text (GTK_ENTRY(entry_stl)));
    double mat = atof((char *) gtk_entry_get_text (GTK_ENTRY(entry_mat)));

    configure_g2pp(alpha, beta, sigma1, sigma2, rho);

    char result_buffer[12];
    g_print ("calculating...\n");
    double val = g2pp_ptr->getZCBP(stl, mat);
    snprintf(result_buffer, sizeof(result_buffer), "%f", val);
    g_print ("calculating...Done\n");
    gtk_entry_set_text(GTK_ENTRY(entry_zcbp), result_buffer);
}

static void reset_g2pp() {
    g_print ("resetting...\n");
    g2pp_ptr->clearSimulation();
    char result_buffer[1];
    snprintf(result_buffer, sizeof(result_buffer), "%s", "");
    gtk_entry_set_text(GTK_ENTRY(entry_zcbp), result_buffer);
    g_print ("resetting...Done\n");
}

static void calibrate (GtkWidget *widget, gpointer data) { g_print ("Calibrating...Done\n"); }

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
    g_signal_connect (button_cali, "clicked", G_CALLBACK (calibrate), NULL);

    GObject *button_calc = gtk_builder_get_object (builder, "button_calculate");
    g_signal_connect (button_calc, "clicked", G_CALLBACK (calculate), NULL);

    GObject *button_reset = gtk_builder_get_object (builder, "button_reset");
    g_signal_connect (button_reset, "clicked", G_CALLBACK (reset_g2pp), NULL);

    GObject *button_quit = gtk_builder_get_object (builder, "button_quit");
    g_signal_connect (button_quit, "clicked", G_CALLBACK (gtk_main_quit), NULL);

    entry_alpha = (GtkWidget *)gtk_builder_get_object (builder, "entry_alpha");
    entry_beta = (GtkWidget *)gtk_builder_get_object (builder, "entry_beta");
    entry_sigma1 = (GtkWidget *)gtk_builder_get_object (builder, "entry_sigma1");
    entry_sigma2 = (GtkWidget *)gtk_builder_get_object (builder, "entry_sigma2");
    entry_rho = (GtkWidget *)gtk_builder_get_object (builder, "entry_rho");

    entry_zcbp = (GtkWidget *)gtk_builder_get_object (builder, "entry_zcbp");
    entry_stl = (GtkWidget *)gtk_builder_get_object (builder, "entry_stl");
    entry_mat = (GtkWidget *)gtk_builder_get_object (builder, "entry_mat");

    gtk_widget_show_all (window);

    initiate_g2pp();
    gtk_main ();

    terminate_g2pp();
    return 0;
}
#ifdef __cplusplus
}
#endif
