#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    GtkWidget *length_scale;
    GtkWidget *uppercase_cb;
    GtkWidget *lowercase_cb;
    GtkWidget *numbers_cb;
    GtkWidget *symbols_cb;
    GtkWidget *num_passwords_entry;
    GtkWidget *open_text_cb;
} Widgets;

void generate_passwords(Widgets *widgets) {
    int length = (int)gtk_range_get_value(GTK_RANGE(widgets->length_scale));
    gboolean uppercase = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widgets->uppercase_cb));
    gboolean lowercase = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widgets->lowercase_cb));
    gboolean numbers = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widgets->numbers_cb));
    gboolean symbols = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widgets->symbols_cb));

    const char *num_passwords_str = gtk_entry_get_text(GTK_ENTRY(widgets->num_passwords_entry));
    int num_passwords = atoi(num_passwords_str);

    if (num_passwords <= 0) {
        GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, "Please enter a positive integer.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }

    gboolean open_text = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widgets->open_text_cb));

    const char *upper_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const char *lower_chars = "abcdefghijklmnopqrstuvwxyz";
    const char *number_chars = "0123456789";
    const char *symbol_chars = "!@#$%^&*()-_=+[]{}|;:,.<>?/";

    char available_chars[128];
    strcpy(available_chars, "");

    if (uppercase) {
        strcat(available_chars, upper_chars);
    }
    if (lowercase) {
        strcat(available_chars, lower_chars);
    }
    if (numbers) {
        strcat(available_chars, number_chars);
    }
    if (symbols) {
        strcat(available_chars, symbol_chars);
    }

    int available_chars_len = strlen(available_chars);

    if (available_chars_len == 0) {
        GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, "Please select at least one character type.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }

    FILE *output_file = fopen("passwords.txt", "w");

    for (int i = 0; i < num_passwords; i++) {
        for (int j = 0; j < length; j++) {
            int random_index = rand() % available_chars_len;
            fprintf(output_file, "%c", available_chars[random_index]);
        }
        fprintf(output_file, "\n");
    }

    fclose(output_file);

    if (open_text) {
        if (system("xdg-open passwords.txt") == -1) {
            GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, "Error opening the output text file.");
            gtk_dialog_run(GTK_DIALOG(dialog));
            gtk_widget_destroy(dialog);
        }
    }
}

void on_generate_button_clicked(GtkWidget *widget, Widgets *widgets) {
    generate_passwords(widgets);
}

int main(int argc, char *argv[]) {
    srand(time(NULL));

    GtkWidget *window;
    GtkWidget *layout;
    GtkWidget *length_label;
    GtkWidget *num_passwords_label;
    GtkWidget *generate_button;

    Widgets *widgets = g_slice_new(Widgets);

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Secure Password Generator");
    gtk_window_set_default_size(GTK_WINDOW(window), 700, 400);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

    layout = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), layout);

    length_label = gtk_label_new("Password Length:");
    gtk_grid_attach(GTK_GRID(layout), length_label, 0, 0, 1, 1);

    widgets->length_scale = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 10, 128, 1);
    gtk_grid_attach(GTK_GRID(layout), widgets->length_scale, 1, 0, 1, 1);

    widgets->uppercase_cb = gtk_check_button_new_with_label("Uppercase Letters");
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widgets->uppercase_cb), TRUE);
    gtk_grid_attach(GTK_GRID(layout), widgets->uppercase_cb, 0, 1, 1, 1);

    widgets->lowercase_cb = gtk_check_button_new_with_label("Lowercase Letters");
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widgets->lowercase_cb), TRUE);
    gtk_grid_attach(GTK_GRID(layout), widgets->lowercase_cb, 1, 1, 1, 1);

    widgets->numbers_cb = gtk_check_button_new_with_label("Numbers");
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widgets->numbers_cb), TRUE);
    gtk_grid_attach(GTK_GRID(layout), widgets->numbers_cb, 0, 2, 1, 1);

    widgets->symbols_cb = gtk_check_button_new_with_label("Symbols");
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widgets->symbols_cb), TRUE);
    gtk_grid_attach(GTK_GRID(layout), widgets->symbols_cb, 1, 2, 1, 1);

    num_passwords_label = gtk_label_new("Number of Passwords:");
    gtk_grid_attach(GTK_GRID(layout), num_passwords_label, 0, 3, 1, 1);

    widgets->num_passwords_entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(widgets->num_passwords_entry), "1");
    gtk_grid_attach(GTK_GRID(layout), widgets->num_passwords_entry, 1, 3, 1, 1);

    widgets->open_text_cb = gtk_check_button_new_with_label("Open Text");
    gtk_grid_attach(GTK_GRID(layout), widgets->open_text_cb, 0, 4, 1, 1);

    generate_button = gtk_button_new_with_label("Generate");
    gtk_grid_attach(GTK_GRID(layout), generate_button, 1, 4, 1, 1);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(G_OBJECT(generate_button), "clicked", G_CALLBACK(on_generate_button_clicked), widgets);

    gtk_widget_show_all(window);

    gtk_main();

    g_slice_free(Widgets, widgets);

    return 0;
}