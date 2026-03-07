#include "at7uui.h"
#include "at7_bitimg.h"
#include "at7_glyph_metrics.h"

#ifdef _WIN32
#include <malloc.h>
#define ALLOCA(X) _alloca(X)
#else
#include <alloca.h>
#define ALLOCA(X) alloca(X)
#endif

#include <stdio.h>


uint32_t at7_ref_count = 0;

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


        unsigned char* imgbuf = ALLOCA(AT7_WIDTH*AT7_HEIGHT);

        uint16_t x,y;
        for (y=0; y<AT7_HEIGHT; ++y) {
            for (x=0; x<AT7_WIDTH; ++x) {
                uint16_t byte_i = (x+y*AT7_WIDTH) / 8;
                uint8_t  bit_i  = (x+y*AT7_WIDTH) % 8;
                char val = at7_bitimg[byte_i] >> bit_i & 0x1;

                imgbuf[x+y*AT7_WIDTH] = val*255;
                //printf("byte_i: %d, bit i: %d, val: %d\n", byte_i, bit_i, val);
            }
        }
        
        
        // fill the image

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glPixelStorei(GL_UNPACK_ROW_LENGTH,         AT7_WIDTH); // cpu -> gpu/
        glTexImage2D(GL_TEXTURE_2D, 0, ug_gl_sized_fmt(GL_RED, GL_UNSIGNED_BYTE),    AT7_WIDTH, AT7_HEIGHT, 0, GL_RED, GL_UNSIGNED_BYTE, imgbuf);

        if ((glerr=glGetError())) {
            return GLenum_to_URESULT(glerr);
        }


        // mag/min filters are MANDATORY!!!
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

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
        glDeleteTextures(1, &atlas.img);
        GLenum glerr;
        if ((glerr=glGetError())!=GL_NO_ERROR) {
            return GLenum_to_URESULT(glerr);
        }
        #endif
    }
    return U_SUCCESS;
}

void at7_init_font_interface(ui_font_interface* intr,  char grfcc) {
    intr->get_font_height = at7_font_get_height;
    intr->glyph_deref = at7_glyph_deref;
    intr->glyph_ref = at7_glyph_ref;
    intr->glyphs_ref = at7_glyphs_ref;
    intr->glyphs_deref = at7_glyphs_deref;
    intr->glyph_get_metrics = at7_glyph_get_metrics;
    intr->uv_get = at7_glyph_get_uv;
    intr->uv_and_img_get = at7_glyph_get_uv_and_img;
    
    intr->u_data = (void*)(uint64_t)grfcc;
    intr->upid = 0;
}

U_RESULT cdecl at7_glyph_ref(ui_font_interface* intr, uint32_t unicode, void* iudata, ug_img_data* data) {
    (void)intr;
    (void)unicode;
    (void)iudata;

    // fill image data

    data->fmt = 0x0;
    #ifdef UUI_GL
    data->gl.img = atlas.img;
    #endif

    data->height = AT7_HEIGHT;
    data->width = AT7_WIDTH;

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
    (void)data;

    // TODO
    
    return U_SUCCESS;
}

U_RESULT cdecl at7_glyphs_deref(ui_font_interface* intr, uint32_t* glyphs, uint16_t glyph_count, void* iudata) {
    (void)intr;
    (void)glyphs;
    (void)glyph_count;
    (void)iudata;    
    return U_SUCCESS;
}


U_RESULT cdecl at7_glyph_get_uv(ui_font_interface* intr, uint32_t unicode, void* iudata, float* u_0, float* v_0, float* u_1, float* v_1) {
    (void)intr;
    (void)iudata;    
    float* uv = (float*)at7_uvs[unicode&0x7F];
    *u_0 = uv[0];
    *v_0 = uv[1];
    *u_1 = uv[2];
    *v_1 = uv[3];

    return U_SUCCESS;
}

U_RESULT cdecl at7_glyph_get_uv_and_img(ui_font_interface* intr, uint32_t unicode, void* iudata, float* u_0, float* v_0, float* u_1, float* v_1, ug_img_data* data) {
    (void)intr;
    (void)iudata;    
    float* uv = (float*)at7_uvs[unicode&0x7F];
    *u_0 = uv[0];
    *v_0 = uv[1];
    *u_1 = uv[2];
    *v_1 = uv[3];

    // fill image data

    data->fmt = 0x0;
    #ifdef UUI_GL
    data->gl.img = atlas.img;
    #endif

    data->height = AT7_HEIGHT;
    data->width = AT7_WIDTH;

    return U_SUCCESS;
}

void cdecl at7_glyph_get_metrics(ui_font_interface* intr, uint32_t unicode, void* iudata, u_glyph_metrics** gmetrics) {
    (void)intr;
    (void)iudata;    
    *gmetrics = (u_glyph_metrics*)at7_glyph_metrics[unicode&0x7F];
}

void cdecl at7_font_get_metrics(ui_font_interface* intr, uint32_t unicode, void* iudata, u_font_metrics** fmetrics) {
    (void)intr;
    (void)unicode;
    (void)iudata;    
    *fmetrics = (u_font_metrics*)&at7_font_metrics;
}

uint16_t cdecl at7_font_get_height(ui_font_interface* intr, void* iudata) {
    (void)intr;
    (void)iudata;
    return 7;
}