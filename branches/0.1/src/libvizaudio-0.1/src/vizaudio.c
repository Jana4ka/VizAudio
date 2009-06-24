#include <vizaudio.h>


/* Global Variables */
gboolean timer = TRUE;

void vizaudio_display(int id) {
    GtkWidget *window;
    switch(id) {
        case 4:;        
            window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
            gtk_widget_show  (window);

            break;

        case 2:;
            window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
            gtk_window_set_decorated(GTK_WINDOW(window), FALSE);
            gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
            
            GtkWidget* image;
            image = gtk_image_new_from_file("seaslug.jpg");
            gtk_container_add (GTK_CONTAINER (window), image);
            
            gtk_widget_show(image);    
            gtk_widget_show(window);        
            
            /* This function takes the function endFlash and calls it every
             * 1 second. When endFlash returns false, this loop ends.
             * This serves to make the screen flash last only one second. */
            g_timeout_add(250, (GSourceFunc)endFlash, (gpointer)window);
            break;

        case 3:;
            window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
            gtk_window_set_decorated(GTK_WINDOW(window), FALSE);
            gtk_window_fullscreen(GTK_WINDOW(window));
            
            /* Modify the color of the window */
            GdkColor color2;
            gdk_color_parse("light blue", &color2);
            gtk_widget_modify_bg(window, GTK_STATE_NORMAL, &color2);
            
            
            gtk_widget_show(window);
            
            /* This function takes the function endFlash and calls it every
             * 1 second. When endFlash returns false, this loop ends.
             * This serves to make the screen flash last only one second. */
            g_timeout_add(250, (GSourceFunc)endFlash, (gpointer)window);
            break;
            
        case 1:;
            window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
            gtk_window_set_title(GTK_WINDOW(window), "Audio Event Alert!");
            gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
            gtk_widget_set_app_paintable(window, TRUE);
            g_signal_connect(G_OBJECT(window), "delete-event", gtk_main_quit, NULL);
            
            /* Link the callbacks */
            g_signal_connect(G_OBJECT(window), "expose-event", G_CALLBACK(textDisplay), NULL);
            g_signal_connect(G_OBJECT(window), "screen-changed", G_CALLBACK(screen_changed), NULL);
            
            gtk_window_set_decorated(GTK_WINDOW(window), FALSE);
            
            //gboolean supports_alpha = FALSE;
            
            screen_changed(window, NULL, NULL);
            g_timeout_add(14, (GSourceFunc) time_handler, (gpointer) window);
            gtk_widget_show(window);
            break;

        default:
            printf("Flashing the screen\n");
            //display_flash();
            break;
    }
}

static gboolean endFlash(GtkWidget *window){
    gtk_object_destroy(GTK_OBJECT(window));
    return FALSE;
}

static void screen_changed(GtkWidget *widget, GdkScreen *old_screen, gpointer user_data){
    /* To check if the display supports alpha channels, get the colormap */
    GdkScreen *screen = gtk_widget_get_screen(widget);
    GdkColormap *colormap = gdk_screen_get_rgba_colormap(screen);
    
    /*if (!colormap)
    {
        printf("Your screen doesn't support alpha channels. Refer to the README for instructions\n");
        
        colormap = gdk_screen_get_rgb_colormap(screen);
        supports_alpha = FALSE;
    }
    else
    {
        supports_alpha = TRUE;
    }*/
    
    gtk_widget_set_colormap(widget, colormap);
}

static gboolean
time_handler (GtkWidget *widget)
{
  if (widget->window == NULL) return FALSE;

  if (!timer) return FALSE;

  gtk_widget_queue_draw(widget);

  return TRUE;
}

static gboolean textDisplay(GtkWidget *widget, GdkEventExpose *event, gpointer user_data){
    cairo_t *cr;
    cairo_text_extents_t extents;

    static gdouble alpha = 1.0;
    static gdouble size = 1;
    cr = gdk_cairo_create(widget->window);

    cairo_set_source_rgba(cr, 1.0, 1.0, 1.0, 0.0); 

    gint x = widget->allocation.width / 2;
    gint y = widget->allocation.height / 2;
    cairo_set_operator (cr, CAIRO_OPERATOR_SOURCE);

    cairo_paint(cr);

    cairo_select_font_face(cr, "Courier",
      CAIRO_FONT_SLANT_NORMAL,
      CAIRO_FONT_WEIGHT_BOLD);

    size += 0.8;

    if (size > 20) {
      alpha -= 0.01;
    }

    cairo_set_font_size(cr, size);

    cairo_set_source_rgb(cr, 0.5, 0, 0); 

    cairo_text_extents(cr, "SEASLUGS", &extents);
    cairo_move_to(cr, x - extents.width/2, y);
    cairo_text_path(cr, "SEASLUGS");
    cairo_clip(cr);
    cairo_stroke(cr);
    cairo_paint_with_alpha(cr, alpha);

    if (alpha <= 0) {
      timer = FALSE;
    }

    cairo_destroy(cr);

    return FALSE;
}

