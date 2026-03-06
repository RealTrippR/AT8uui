#include "at7uui.h"
#include "at7_bitimg.h"
#include "at7_glyph_metrics.h"

uint32_t at7_ref_count=0;

struct {
    #ifdef UUI_GL
    GLuint img;
    #endif
} atlas;



U_RESULT at7_ref_atlas() {
    if (at7_ref_count == UINT32_MAX-1) {
        return U_ERR_INVALID_OPERATION; 
    }
    at7_ref_count++;

    if (at7_ref_count==1) {
        // make atlas
        #ifdef UUI_GL
        glGetError(); //flush error state

        glGenTextures(1, &atlas.img);
        GLenum glerr;
        if ((glerr=glGetError())!=GL_NO_ERROR) {
            return GLenum_to_URESULT(glerr);
        }
        glBindTexture(GL_TEXTURE_2D, atlas.img);
        glTexImage2D(GL_TEXTURE_2D, 0, INTERNAL_FOR)
        
        #endif
    }

    return U_SUCCESS;
}

U_RESULT at7_deref_atlas() {
    if (at7_ref_count==0) {
        return U_ERR_INVALID_OPERATION;
    }
    at7_ref_count--;
    if (at7_ref_count==0) {
        #ifdef UUI_GL
        glGetError(); //flush error state
        #endif
    }
    return U_SUCCESS;
}

void at7_init_font_interface(ui_font_interface* intr,  char grfcc) {
    intr->get_font_height = at7_get_font_height;
    intr->glyph_deref = at7_glyph_deref;
    intr->glyph_ref = at7_glyph_ref;
    intr->glyphs_ref = at7_glyphs_ref;
    intr->glyphs_deref = at7_glyphs_deref;
    intr->glyph_get_metrics = at7_glyphs_get_metrics;
    
    intr->u_data = NULL;
    intr->upid = 0;
}

U_RESULT cdecl at7_glyph_ref(ui_font_interface* intr, uint32_t unicode, void* iudata, ug_img_data* data) {
    (void)intr;
    (void)unicode;
    (void)iudata;

    // fill image data

    return U_SUCCESS;
}

U_RESULT cdecl at7_glyph_deref(ui_font_interface* intr, uint32_t unicode, void* iudata) {
    (void)intr;
    (void)unicode;
    (void)iudata;
    return U_SUCCESS;
}

U_RESULT cdecl at7_glyphs_ref(ui_font_interface* intr, uint32_t* glyphs, uint16_t glyph_count, void* iudata, ug_img_data* data) {
    (void)intr;
    (void)glyphs;
    (void)glyph_count;
    (void)iudata;
    
    // fill image data

    
    return U_SUCCESS;
}

U_RESULT cdecl at7_glyphs_deref(ui_font_interface* intr, uint32_t* glyphs, uint16_t glyph_count, void* iudata) {
    (void)intr;
    (void)glyphs;
    (void)glyph_count;
    (void)iudata;
    return U_SUCCESS;
}