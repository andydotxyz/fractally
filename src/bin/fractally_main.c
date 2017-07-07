#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

/* NOTE: Respecting header order is important for portability.
 * Always put system first, then EFL, then your public header,
 * and finally your private one. */

#if ENABLE_NLS
# include <libintl.h>
#endif

#include <Ecore_Getopt.h>
#include <Elementary.h>

#include "fractally_private.h"

#define COPYRIGHT "Copyright © 2017 Andy Williams <andy@andywilliams.me> and various contributors (see AUTHORS)."

static void
_fractally_win_del(void *data EINA_UNUSED, Evas_Object *obj EINA_UNUSED, void *event_info EINA_UNUSED)
{
   elm_exit();
}

static void
_fractally_win_resize(void *data EINA_UNUSED, Evas *e EINA_UNUSED, Evas_Object *obj,
                      void *event_info EINA_UNUSED)
{
   fractally_render_layout(obj);
}

static Evas_Object *
fractally_win_setup(void)
{
   Evas_Object *win;

   win = elm_win_util_standard_add("main", "Factally");
   if (!win) return NULL;

   elm_win_focus_highlight_enabled_set(win, EINA_TRUE);
   evas_object_resize(win, 300 * elm_config_scale_get(),
                           200 * elm_config_scale_get());

   evas_object_smart_callback_add(win, "delete,request", _fractally_win_del, NULL);
   evas_object_event_callback_add(win, EVAS_CALLBACK_RESIZE, _fractally_win_resize, NULL);

   fractally_render_init(win);
   evas_object_show(win);

   return win;
}

static const Ecore_Getopt optdesc = {
  "fractally",
  "%prog [options]",
  PACKAGE_VERSION,
  COPYRIGHT,
  "3 clause BSD license",
  "An EFL fractally program",
  0,
  {
    ECORE_GETOPT_LICENSE('L', "license"),
    ECORE_GETOPT_COPYRIGHT('C', "copyright"),
    ECORE_GETOPT_VERSION('V', "version"),
    ECORE_GETOPT_HELP('h', "help"),
    ECORE_GETOPT_SENTINEL
  }
};

EAPI_MAIN int
elm_main(int argc EINA_UNUSED, char **argv EINA_UNUSED)
{
   Evas_Object *win;
   int args;
   Eina_Bool quit_option = EINA_FALSE;

   Ecore_Getopt_Value values[] = {
     ECORE_GETOPT_VALUE_BOOL(quit_option),
     ECORE_GETOPT_VALUE_BOOL(quit_option),
     ECORE_GETOPT_VALUE_BOOL(quit_option),
     ECORE_GETOPT_VALUE_BOOL(quit_option),
     ECORE_GETOPT_VALUE_NONE
   };

#if ENABLE_NLS
   setlocale(LC_ALL, "");
   bindtextdomain(PACKAGE, LOCALEDIR);
   bind_textdomain_codeset(PACKAGE, "UTF-8");
   textdomain(PACKAGE);
#endif

   args = ecore_getopt_parse(&optdesc, values, argc, argv);
   if (args < 0)
     {
	EINA_LOG_CRIT("Could not parse arguments.");
	goto end;
     }
   else if (quit_option)
     {
	goto end;
     }

   elm_app_info_set(elm_main, "fractally", "images/fractally.png");

   if (!(win = fractally_win_setup()))
     goto end;

   fractally_render_layout(win);
   elm_run();

 end:
   elm_shutdown();

   return 0;
}
ELM_MAIN()
