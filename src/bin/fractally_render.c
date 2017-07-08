#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <Evas.h>
#include <Elementary.h>

#include <math.h>

#include "fractally_private.h"

Evas_Object *_fractally_img;

Evas_Object *
fractally_render_init(Evas_Object *content)
{
   Evas_Coord ww, wh;

   evas_object_geometry_get(_fractally_win, NULL, NULL, &ww, &wh);
   evas_object_hide(content);

   _fractally_img = evas_object_image_add(evas_object_evas_get(content));
   evas_object_image_alpha_set(_fractally_img, EINA_FALSE);
   evas_object_image_source_set(_fractally_img, content);

   evas_object_image_filled_set(_fractally_img, EINA_TRUE);
   evas_object_show(_fractally_img);

   elm_win_resize_object_add(_fractally_win, _fractally_img);
   evas_object_image_size_set(_fractally_img, ww, wh);

   fractally_render_layout(content);
   return _fractally_img;
}

void
fractally_render_layout(Evas_Object *content)
{
   Evas_Coord ww, wh;

   evas_object_geometry_get(_fractally_win, NULL, NULL, &ww, &wh);
   evas_object_image_size_set(_fractally_img, ww, wh);

   fractally_render_refresh(content);
}

static void
_color_cell_at(float px, float py, unsigned int *pixel)
{
   double c_re = px * 4;
   double c_im = py * 4;
   int MAX_ITERATIONS = 50 / (sqrt(sqrt(_fractally_scale)));

   int i;
   double x = 0, y = 0, xsq = 0, ysq = 0;

   for (i = 0; i < MAX_ITERATIONS && (xsq + ysq <= 4); i++)
     {
        double x_new = xsq - ysq + c_re;
        y = 2 * x * y + c_im;
        x = x_new;

        xsq = x * x;
        ysq = y * y;
     }

   if (i == MAX_ITERATIONS)
     *pixel = 0;
   else
     {
        double mu = ((double) i / MAX_ITERATIONS);
        double c = sin((mu / 2) * M_PI);

        *pixel = (int)(c * 255);
        *pixel |= 56 << 8;
        *pixel |= (int)((1 - c) * 255) << 16;
      }
}

void
fractally_render_refresh(Evas_Object *content)
{
   Evas_Coord ww, wh;
   unsigned int *data, *pixel;
   int x, y;

   evas_object_geometry_get(content, NULL, NULL, &ww, &wh);
   data = evas_object_image_data_get(_fractally_img, EINA_TRUE);
   pixel = data;

   for (y = 0; y < wh; y++)
     for (x = 0; x < ww; x++)
       {
          _color_cell_at((((float)x / ww) - .625) * _fractally_scale + _fractally_x,
                          ((((float)y / wh) - .5) / 1.5) * _fractally_scale + _fractally_y, pixel);

          pixel++;
       }
   evas_object_image_data_set(_fractally_img, data);
   evas_object_image_data_update_add(_fractally_img, 0, 0, ww, wh);
}

