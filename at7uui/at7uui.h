#ifndef AT8_UUI_H99
#define AT8_UUI_H99

#include <uui/uui_core.h>
#include <uui/uui_ui.h>

extern uint32_t at7_ref_count;

extern const unsigned char at7_bitimg[];

U_RESULT at7_ref_atlas();

U_RESULT at7_deref_atlas();

void at7_init_font_interface(ui_font_interface*, char grfcc);

U_RESULT cdecl at7_glyph_ref(ui_font_interface* intr, uint32_t unicode, void* iudata, ug_img_data* data);

U_RESULT cdecl at7_glyph_deref(ui_font_interface* intr, uint32_t unicode, void* iudata);

U_RESULT cdecl at7_glyphs_ref(ui_font_interface* intr, uint32_t* glyphs, uint16_t glyph_count, void* iudata, ug_img_data* data);

U_RESULT cdecl at7_glyphs_deref(ui_font_interface* intr, uint32_t* glyphs, uint16_t glyph_count, void* iudata);

uint16_t cdecl at7_get_font_height(ui_font_interface* intr, void* iudata);

void cdecl at7_get_glyph_metrics(ui_font_interface* intr, uint32_t unicode, void* iudata, u_glyph_metrics** gmetrics);

void cdecl at7_get_font_metrics(ui_font_interface* intr, uint32_t unicode, void* iudata, u_font_metrics** gmetrics);

void cdecl ui_fptr_get_font_height(ui_font_interface* intr, void* iudata);

#endif