#include "gtk/gtkcssprovider.h"
#include "xdg.hpp"
#include <cstdio>
#include <cstdlib>
#include <gtk-layer-shell.h>
#include <gtk/gtk.h>
#include <string>
#include <unistd.h>
#include <vte-2.91/vte/vte.h>
#include <webkit2/webkit2.h>

auto static app_startup(GtkApplication *_app) -> void {
	GtkCssProvider *css_provider;
	css_provider = gtk_css_provider_new();
	std::string css_data = R"%(
		* {
			font-family: "DejaVu Sans", serif;
		}

		window {
			background-color: rgba(47, 47, 47, 0.90);
		}

		.menu_window {
			background-color: rgba(27, 27, 27, 0.90);
		}

		vte-terminal {
			background-color: white;
		}

		.menu_window button {
			background: none;
			border: none;
			border-radius: 0;
			box-shadow: none;
			text-shadow: none;
		}

		.wallpaper_window {
			background: none;
			background-image: url("comes_wallpaper.png");
		}

		.menu_window button:hover {
			background-color: rgba(0, 0, 0, 0.2);
		}

		.menu_window button label {
			color: black;
			font-size: 1.2em;
			margin-right: 60px;
			box-shadow: none;
			text-shadow: none;
		}

		.clock_window {
			background-color: transparent;
			padding: 0;
			margin: 0;
		}

		.clock_window label {
			color: black;
			font-weight: bold;
			font-size: 6em;
			padding: 0;
			margin: 0;
			margin-top: 0;
			margin-bottom: 0;
			text-shadow: none;
		}

		button.toggle {
			background: none;
			background-color: transparent;
			border-radius: 0;
			border: none;
			box-shadow: none;
			text-shadow: none;
			-gtk-icon-shadow: none;
		}

		button.toggle:hover {
			background-color: rgba(0, 0, 0, 0.2);
		}

		button.toggle:checked {
			background-color: rgba(0, 0, 0, 0.2);
		}

		.comes_titlebar {
			background: none;
			background-color: white;
			border-radius: 0;
			border: none;

			min-height: 34px;

			border: 2px solid #ff80d7;
			font-family: "DejaVu Sans", serif;
			font-size: 1.5em;
			font-weight: 400;
		}

		.comes_titlebar label {
			color: black;
			margin-bottom: 10px;
			margin-right: 5px;
		}

		.comes_titlebar .close {
			background: none;
			background-color: red;
			border-radius: 0;
			box-shadow: none;
			text-shadow: none;
			-gtk-icon-shadow: none;
			padding-right: 10px;
			padding-bottom: 10px;
		}

		.comes_titlebar .close:hover {
			background-color: pink;
		}

		.comes_titlebar .minimize {
						background: none;
						border-radius: 0;
			box-shadow: none;
			text-shadow: none;
			-gtk-icon-shadow: none;
			background-color: #ffeb89;
						padding-right: 10px;
			padding-bottom: 10px;
		}

		.comes_titlebar .minimize:hover {
			background-color: #ff964b;
		}

		.comes_titlebar .maximize {
						background: none;
						border-radius: 0;
			box-shadow: none;
			text-shadow: none;
			-gtk-icon-shadow: none;
			background-color: #ffeb89;
						padding-right: 10px;
			padding-bottom: 10px;
		}

		.comes_titlebar .maximize:hover {
			background-color: #ff964b;
		}


		.comes_titlebar box {
			background: none;
		}
	)%";
	gtk_css_provider_load_from_data(css_provider, css_data.c_str(), css_data.length(), NULL);
	gtk_style_context_add_provider_for_screen(gdk_screen_get_default(), GTK_STYLE_PROVIDER(css_provider),
											  GTK_STYLE_PROVIDER_PRIORITY_USER);

	auto settings = gtk_settings_get_default();

	GValue theme_name = G_VALUE_INIT;
	g_value_init(&theme_name, G_TYPE_STRING);
	g_value_set_string(&theme_name, "Adwaita");

	g_object_set_property(G_OBJECT(settings), "gtk-theme-name", &theme_name);
}

auto create_comes_titlebar(std::string title) -> GtkWidget * {
	GtkHeaderBar *hb = GTK_HEADER_BAR(gtk_header_bar_new());
	gtk_header_bar_set_show_close_button(hb, true);

	GtkBox *box = GTK_BOX(gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0));
	gtk_container_add(GTK_CONTAINER(box), gtk_label_new(title.c_str()));

	gtk_widget_set_hexpand(GTK_WIDGET(box), true);

	gtk_header_bar_set_custom_title(hb, GTK_WIDGET(box));

	GtkStyleContext *context;
	context = gtk_widget_get_style_context(GTK_WIDGET(hb));
	gtk_style_context_add_class(context, "comes_titlebar");

	return GTK_WIDGET(hb);
}

auto Comes_Przeglądarka_WWW_Do_Dupy() -> GtkWindow * {
	GtkWindow *window = GTK_WINDOW(gtk_window_new(GTK_WINDOW_TOPLEVEL));
	gtk_window_set_title(window, "Comes Przeglądarka WWW Do Dupy");
	gtk_window_set_titlebar(window, create_comes_titlebar( "Comes Przeglądarka WWW Do Dupy"));
	gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);

	WebKitWebView *webView = WEBKIT_WEB_VIEW(webkit_web_view_new());

	GtkBox *box = GTK_BOX(gtk_box_new(GTK_ORIENTATION_VERTICAL, 0));
	GtkBox *kontrolki = GTK_BOX(gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0));

	GtkEntry *url_entry = GTK_ENTRY(gtk_entry_new());
	GtkButton *przejdź_do_strony = GTK_BUTTON(gtk_button_new_with_label("Przejdź Do Strony"));

	gtk_entry_set_text(url_entry, "https://github.com/comes-group");

	struct PrzejdzDoStronyData {
		WebKitWebView *view;
		GtkEntry *url_entry;
	};

	auto pds_data = new PrzejdzDoStronyData;
	pds_data->view = webView;
	pds_data->url_entry = url_entry;

	g_signal_connect(przejdź_do_strony, "clicked", G_CALLBACK(+[](GtkButton *btn, PrzejdzDoStronyData *dt) {
						 auto text = gtk_entry_get_text(dt->url_entry);
						 webkit_web_view_load_uri(dt->view, text);
					 }),
					 pds_data);

	gtk_box_pack_start(kontrolki, GTK_WIDGET(url_entry), true, true, 0);
	gtk_box_pack_start(kontrolki, GTK_WIDGET(przejdź_do_strony), false, true, 0);

	gtk_box_pack_start(box, GTK_WIDGET(kontrolki), false, true, 0);
	gtk_box_pack_start(box, GTK_WIDGET(webView), true, true, 0);

	gtk_container_add(GTK_CONTAINER(window), GTK_WIDGET(box));

	g_signal_connect(window, "destroy", G_CALLBACK(+[]() {}), NULL);
	g_signal_connect(webView, "close", G_CALLBACK(+[](WebKitWebView *webView, GtkWidget *window) {
						 gtk_widget_destroy(window);
						 return true;
					 }),
					 window);

	// Load a web page into the browser instance
	webkit_web_view_load_uri(webView, "https://github.com/comes-group");

	gtk_widget_grab_focus(GTK_WIDGET(webView));
	gtk_widget_show_all(GTK_WIDGET(window));

	return GTK_WINDOW(window);
}

auto Comes_Wiersz_Poleceń_Do_Dupy() -> GtkWindow * {
	GtkWindow *window = GTK_WINDOW(gtk_window_new(GTK_WINDOW_TOPLEVEL));
	gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
	gtk_window_set_title(window, "Comes Wiersz Poleceń Do Dupy");

	gtk_window_set_titlebar(window, create_comes_titlebar("Comes Wiersz Poleceń Do Dupy"));

	VteTerminal *term = VTE_TERMINAL(vte_terminal_new());
	VtePty *pty = VTE_PTY(vte_terminal_pty_new_sync(term, VTE_PTY_DEFAULT, NULL, NULL));

	GdkRGBA white_color = (GdkRGBA) { .red = 1.0, .green = 1.0, .blue = 1.0, .alpha = 1.0 };
	GdkRGBA black_color = (GdkRGBA) { .red = 0.0, .green = 0.0, .blue = 0.0, .alpha = 1.0 };

	vte_terminal_set_color_background(term, &white_color);
	vte_terminal_set_color_foreground(term, &black_color);

	gchar **argv = g_new0(gchar *, 4);
	argv[0] = std::getenv("SHELL");
	argv[1] = "-l";

	gchar **envv = g_new0(gchar *, 2);
	envv[0] = "PROMPT_COMMAND=echo -ne \"\\033]0;${USER}@${HOSTNAME%%.*}:${PWD/#$HOME/\\~}\\007\"";

	gtk_container_add(GTK_CONTAINER(window), GTK_WIDGET(term));

	int flags = G_SPAWN_DEFAULT;
	flags |= G_SPAWN_SEARCH_PATH;
	flags |= G_SPAWN_FILE_AND_ARGV_ZERO;

	vte_terminal_spawn_async(term, VTE_PTY_DEFAULT, g_get_home_dir(), argv, envv, (GSpawnFlags)flags, NULL, NULL, NULL,
							 -1, NULL, NULL, NULL);

	vte_terminal_set_pty(term, pty);
	gtk_widget_show_all(GTK_WIDGET(window));

	return window;
}

auto create_wallpaper_window() -> GtkWindow * {
	GtkWindow *window = GTK_WINDOW(gtk_window_new(GTK_WINDOW_TOPLEVEL));

	gtk_window_set_title(window, "comes-desktop-shell-wallpaper");
	gtk_window_set_decorated(window, false);
	gtk_window_set_resizable(window, false);

	GtkStyleContext *context;
	context = gtk_widget_get_style_context(GTK_WIDGET(window));
	gtk_style_context_add_class(context, "wallpaper_window");

	gtk_layer_init_for_window(window);

	gtk_layer_set_layer(window, GTK_LAYER_SHELL_LAYER_BACKGROUND);
	gtk_layer_set_keyboard_mode(window, GTK_LAYER_SHELL_KEYBOARD_MODE_NONE);

	gtk_layer_set_anchor(window, GTK_LAYER_SHELL_EDGE_LEFT, true);
	gtk_layer_set_anchor(window, GTK_LAYER_SHELL_EDGE_RIGHT, true);
	gtk_layer_set_anchor(window, GTK_LAYER_SHELL_EDGE_TOP, true);
	gtk_layer_set_anchor(window, GTK_LAYER_SHELL_EDGE_BOTTOM, true);

	gtk_widget_show_all(GTK_WIDGET(window));

	return window;
}

auto create_main_window(GtkWindow *menu_window) -> GtkWindow * {
	// Main window
	GtkWindow *window = GTK_WINDOW(gtk_window_new(GTK_WINDOW_TOPLEVEL));

	gtk_window_set_title(window, "comes-desktop-shell-panel");
	gtk_window_set_decorated(window, false);
	gtk_window_set_resizable(window, false);

	gtk_layer_init_for_window(window);

	gtk_layer_set_layer(window, GTK_LAYER_SHELL_LAYER_TOP);
	gtk_layer_set_keyboard_mode(window, GTK_LAYER_SHELL_KEYBOARD_MODE_NONE);

	// Repell other windows.
	// gtk_layer_auto_exclusive_zone_enable(window);

	// Resize window to maximum possible width.
	gtk_layer_set_anchor(window, GTK_LAYER_SHELL_EDGE_LEFT, true);
	gtk_layer_set_anchor(window, GTK_LAYER_SHELL_EDGE_RIGHT, true);
	gtk_layer_set_anchor(window, GTK_LAYER_SHELL_EDGE_TOP, false);
	gtk_layer_set_anchor(window, GTK_LAYER_SHELL_EDGE_BOTTOM, true);

	gtk_widget_set_size_request(GTK_WIDGET(window), 0, 48);

	GtkBox *box = GTK_BOX(gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0));
	GtkToggleButton *button = GTK_TOGGLE_BUTTON(gtk_toggle_button_new());

	g_signal_connect(button, "toggled", G_CALLBACK(+[](GtkToggleButton *button, GtkWindow *men) {
						 if (gtk_toggle_button_get_active(button)) {
							 gtk_widget_show_all(GTK_WIDGET(men));
						 } else {
							 gtk_widget_hide(GTK_WIDGET(men));
						 }
						 return true;
					 }),
					 menu_window);

	gtk_button_set_image(GTK_BUTTON(button), gtk_image_new_from_file("comes.png"));

	gtk_container_add(GTK_CONTAINER(box), GTK_WIDGET(button));

	gtk_container_add(GTK_CONTAINER(window), GTK_WIDGET(box));

	gtk_widget_show_all(GTK_WIDGET(window));

	return window;
}

auto create_clock_window() -> GtkWindow * {
	// Main window
	GtkWindow *window = GTK_WINDOW(gtk_window_new(GTK_WINDOW_TOPLEVEL));

	gtk_window_set_title(window, "comes-desktop-shell-clock");
	gtk_window_set_decorated(window, false);
	gtk_window_set_resizable(window, false);

	GtkStyleContext *context;
	context = gtk_widget_get_style_context(GTK_WIDGET(window));
	gtk_style_context_add_class(context, "clock_window");

	gtk_layer_init_for_window(window);

	gtk_layer_set_layer(window, GTK_LAYER_SHELL_LAYER_OVERLAY);
	gtk_layer_set_keyboard_mode(window, GTK_LAYER_SHELL_KEYBOARD_MODE_NONE);

	// Resize window to maximum possible width.
	gtk_layer_set_anchor(window, GTK_LAYER_SHELL_EDGE_RIGHT, true);
	gtk_layer_set_anchor(window, GTK_LAYER_SHELL_EDGE_BOTTOM, true);

	gtk_layer_set_margin(window, GTK_LAYER_SHELL_EDGE_BOTTOM, -15);

	gtk_widget_set_size_request(GTK_WIDGET(window), 200, 32);

	GtkWidget *time_label = gtk_label_new("12:30");
	gint gtk_timeout = g_timeout_add(
		1000,
		+[](void *timlabl) -> int {
			time_t m_unixtime = time(NULL);
			struct tm m_time_conv;
			char *m_time_format = "%H:%M";
			char m_str_time[256];

			if (localtime_r(&m_unixtime, &m_time_conv) == NULL) { return 1; }

			strftime(m_str_time, sizeof(m_str_time), m_time_format, &m_time_conv);
			gtk_label_set_text((GtkLabel *)timlabl, m_str_time);

			return 1;
		},
		time_label);

	gtk_container_add(GTK_CONTAINER(window), time_label);

	gtk_widget_show_all(GTK_WIDGET(window));

	return window;
}

auto create_menu_window(GtkApplication *_app) -> GtkWindow * {
	// Main window
	GtkWindow *window = GTK_WINDOW(gtk_window_new(GTK_WINDOW_TOPLEVEL));

	gtk_window_set_title(window, "comes-desktop-shell-menu");
	gtk_window_set_decorated(window, false);
	gtk_window_set_resizable(window, false);

	GtkStyleContext *context;
	context = gtk_widget_get_style_context(GTK_WIDGET(window));
	gtk_style_context_add_class(context, "menu_window");

	gtk_layer_init_for_window(window);

	gtk_layer_set_layer(window, GTK_LAYER_SHELL_LAYER_OVERLAY);
	gtk_layer_set_keyboard_mode(window, GTK_LAYER_SHELL_KEYBOARD_MODE_NONE);

	// Resize window to maximum possible width.
	gtk_layer_set_anchor(window, GTK_LAYER_SHELL_EDGE_LEFT, true);
	gtk_layer_set_anchor(window, GTK_LAYER_SHELL_EDGE_BOTTOM, true);

	gtk_layer_set_margin(window, GTK_LAYER_SHELL_EDGE_BOTTOM, 48);

	GtkBox *box = GTK_BOX(gtk_box_new(GTK_ORIENTATION_VERTICAL, 0));

	GtkButton *uruchom_polecenie = GTK_BUTTON(gtk_button_new_with_label("Uruchom\npolecenie"));
	GtkButton *przeglądarka_plików = GTK_BUTTON(gtk_button_new_with_label("Przeglądarka\nplików"));
	GtkButton *wiersz_poleceń = GTK_BUTTON(gtk_button_new_with_label("Wiersz poleceń"));
	GtkButton *comes_przeglądarka_www = GTK_BUTTON(gtk_button_new_with_label("Przeglądarka WWW"));

	g_signal_connect(comes_przeglądarka_www, "clicked", G_CALLBACK(+[](GtkButton *btn, GtkApplication *_app) {
						 gtk_application_add_window(_app, Comes_Przeglądarka_WWW_Do_Dupy());
					 }),
					 _app);

	g_signal_connect(wiersz_poleceń, "clicked", G_CALLBACK(+[](GtkButton *btn, GtkApplication *_app) {
						 gtk_application_add_window(_app, Comes_Wiersz_Poleceń_Do_Dupy());
					 }),
					 _app);

	gtk_box_pack_end(box, GTK_WIDGET(comes_przeglądarka_www), true, true, 0);
	gtk_box_pack_end(box, GTK_WIDGET(wiersz_poleceń), true, true, 0);
	gtk_box_pack_end(box, GTK_WIDGET(przeglądarka_plików), true, true, 0);
	gtk_box_pack_end(box, GTK_WIDGET(uruchom_polecenie), true, true, 0);

	gtk_container_add(GTK_CONTAINER(window), GTK_WIDGET(box));

	gtk_widget_set_size_request(GTK_WIDGET(window), 200, 300);
	return window;
}

auto static app_activate(GtkApplication *_app) -> void {
	auto menu_window = create_menu_window(_app);

	gtk_application_add_window(_app, create_wallpaper_window());
	gtk_application_add_window(_app, menu_window);
	gtk_application_add_window(_app, create_main_window(menu_window));
	gtk_application_add_window(_app, create_clock_window());
}
auto static app_shutdown(GtkApplication *_app) -> void {}

auto main(int argc, char **argv) -> int {
	GtkApplication *app;
	app = gtk_application_new("comes.desktop.shell", G_APPLICATION_FLAGS_NONE);

	g_signal_connect(app, "startup", G_CALLBACK(app_startup), NULL);
	g_signal_connect(app, "activate", G_CALLBACK(app_activate), NULL);
	g_signal_connect(app, "shutdown", G_CALLBACK(app_shutdown), NULL);

	int status = g_application_run(G_APPLICATION(app), argc, argv);
	g_object_unref(app);

	return status;
}