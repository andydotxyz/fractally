#ifndef FRACTALLY_PRIVATE_H_
# define FRACTALLY_PRIVATE_H_

#include <Evas.h>

extern float _fractally_x, _fractally_y, _fractally_scale;
extern Evas_Object *_fractally_win;

Evas_Object *fractally_render_init(Evas_Object *win);
void fractally_render_layout(Evas_Object *win);
void fractally_render_refresh(Evas_Object *win);

#endif
