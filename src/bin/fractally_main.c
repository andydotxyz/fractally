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

Evas_Object *_canvas, *_fractally_win;
double _fractally_x = -0.15, _fractally_y = 0.0, _fractally_scale = 1.0;
Evas_Coord _mouse_x, _mouse_y;
Eina_Bool _mouse_down;

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

static void
_fractally_mouse_down(void *data EINA_UNUSED, Evas *e EINA_UNUSED, Evas_Object *obj EINA_UNUSED,
                      void *event_info)
{
   Evas_Event_Mouse_Down *event;

   event = event_info;

   if (event->button != 1)
     return;

   _mouse_down = EINA_TRUE;
   _mouse_x = event->canvas.x;
   _mouse_y = event->canvas.y;
}

static void
_fractally_mouse_up(void *data EINA_UNUSED, Evas *e EINA_UNUSED, Evas_Object *obj EINA_UNUSED,
                      void *event_info)
{
   Evas_Event_Mouse_Up *event;
   Evas_Coord ww, wh;
   double deltax, deltay;

   event = (Evas_Event_Mouse_Up *) event_info;

   if (event->button != 1)
     return;

   _mouse_down = EINA_FALSE;
   evas_object_geometry_get(obj, NULL, NULL, &ww, &wh);

   deltax = (float)(_mouse_x - event->canvas.x) / ww * _fractally_scale;
   deltay = (float)(_mouse_y - event->canvas.y) / (wh * 1.5) * _fractally_scale;

   _fractally_x += deltax;
   _fractally_y += deltay;

   evas_object_map_enable_set(_canvas, EINA_FALSE);
   fractally_render_refresh(obj);
}

static void
_fractally_mouse_move(void *data EINA_UNUSED, Evas *e EINA_UNUSED, Evas_Object *obj EINA_UNUSED,
                      void *event_info)
{
   Evas_Event_Mouse_Move *event;
   Evas_Map *m = evas_map_new(4);
   Evas_Coord ww, wh;
   int deltax, deltay;

   event = (Evas_Event_Mouse_Move *)event_info;

   if (!_mouse_down)
     return;

   evas_object_geometry_get(_fractally_win, NULL, NULL, &ww, &wh);
   deltax = _mouse_x - event->cur.canvas.x;
   deltay = _mouse_y - event->cur.canvas.y;

   evas_map_util_points_populate_from_geometry(m, 0 - deltax, 0 - deltay, ww, wh, 0);

   evas_object_map_set(_canvas, m);
   evas_object_map_enable_set(_canvas, EINA_TRUE);
   evas_map_free(m);
}

static void
_fractally_key_down(void *data EINA_UNUSED, Evas *e EINA_UNUSED, Evas_Object *obj,
                    void *event_info)
{
   Evas_Event_Key_Down *ev;

   ev = event_info;

   if (!strcmp(ev->key, "Left"))
     _fractally_x += .18  * _fractally_scale;
   else if (!strcmp(ev->key, "Right"))
     _fractally_x -= .18 * _fractally_scale;
   else if (!strcmp(ev->key, "Up"))
     _fractally_y += .12 * _fractally_scale;
   else if (!strcmp(ev->key, "Down"))
     _fractally_y -= .12 * _fractally_scale;
   else if (!strcmp(ev->key, "plus"))
     _fractally_scale *= .8;
   else if (!strcmp(ev->key, "minus"))
     _fractally_scale *= 1.25;

   fractally_render_refresh(obj);
}

static Evas_Object *
fractally_win_setup(void)
{
   Evas_Object *win, *content;

   win = elm_win_util_standard_add("main", "Factally");
   if (!win) return NULL;

   _fractally_win = win;
   elm_win_focus_highlight_enabled_set(win, EINA_TRUE);
   evas_object_resize(win, 300 * elm_config_scale_get(),
                           200 * elm_config_scale_get());

   evas_object_smart_callback_add(win, "delete,request", _fractally_win_del, NULL);
   evas_object_event_callback_add(win, EVAS_CALLBACK_RESIZE, _fractally_win_resize, NULL);
   evas_object_event_callback_add(win, EVAS_CALLBACK_MOUSE_DOWN, _fractally_mouse_down, NULL);
   evas_object_event_callback_add(win, EVAS_CALLBACK_MOUSE_UP, _fractally_mouse_up, NULL);
   evas_object_event_callback_add(win, EVAS_CALLBACK_MOUSE_MOVE, _fractally_mouse_move, NULL);
   evas_object_event_callback_add(win, EVAS_CALLBACK_KEY_DOWN, _fractally_key_down, NULL);

   content = evas_object_rectangle_add(win);
   elm_win_resize_object_add(win, content);
   evas_object_show(content);

   _canvas = fractally_render_init(content);
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
