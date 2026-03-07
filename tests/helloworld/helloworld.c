#include <uui/uui_core.h>
#include <uui/uui_ui.h>
#include <at7uui/at7uui.h>


#include <twis/twis.h>
#include <twis/twisgl.h>

#include <stdio.h>

void on_window_resize(TW_WINDOW* wnd, uint32_t ww, uint32_t wh) {
    glViewport(0,0,ww,wh);
    TW_WINDOW_GET_CLIENT_SIZE(wnd, &ww, &wh);
    u_begin_info* begin_info = TW_WINDOW_GET_USERDATA(wnd);
    begin_info->render_area.x = ww;
    begin_info->render_area.y = wh;
    (void)wnd;
}

int main() 
{
    TW_RESULT rs = 0;
    U_RESULT  rsu = 0;
    TW_INSTANCE instance;
    TW_INSTANCE_CREATE_INFO instance_create_info = {NULL};
    printf("Creating TW instance... ");
    if ((rs=TW_CREATE_INSTANCE(&instance, &instance_create_info)) < 0) {
        printf("FAIL\n");
        goto fail;
    }
    printf("OK\n");

    TW_WINDOW* window;
    TW_WINDOW_BURDEN window_burden;


    TW_WINDOW_CALLBACKS_A win_callbacks_A = {NULL};
    TW_WINDOW_CALLBACKS_A_DEFAULT_NULLS(&win_callbacks_A);
    
    TW_WINDOW_CREATE_INFO win_create_info = {
        .WIDTH  = 500,
        .HEIGHT = 500,
        .POSX   = TW_USE_DEFAULT,
        .POSY   = TW_USE_DEFAULT,
        .TITLE  = L"UI Test",
        .CALLBACKS_A = &win_callbacks_A
    };
    
    printf("Creating TW window... ");
    if ((rs = TW_CREATE_WINDOW(&instance, &window, &win_create_info, &window_burden)) < 0) {
        printf("FAIL\n");
        goto fail;
    }
    printf("OK\n");

    
    TW_GL_INSTANCE gl_instance;
    if ((rs = TW_CREATE_GL_INSTANCE(&gl_instance)) < 0) {
        printf("FAIL\n");
        goto fail;
    }
    printf("OK\n");
   

    printf("Creating GL Context... ");
    TW_GL_CONTEXT gl_window_context;
    TW_GL_PIXEL_DESCRIPTION gl_pfd = {
        .PIXEL_TYPE = TW_PIXEL_T_RGBA,
        .COLOR_BITS = 32,
        .DEPTH_BITS = 24,
        .STENCIL_BITS = 8,
        .PLANE = TW_PFD_PLANE_MAIN
    };
    if ((rs = TW_CREATE_GL_CONTEXT(window, &gl_instance, &gl_window_context, &gl_pfd, 4, 6, NULL, NULL)) < 0) {
        printf("FAIL\n");
        goto fail;
    }
    printf("OK\n");

    printf("Setting as current GL context... ");

    if ((rs = TW_GL_MAKE_CONTEXT_CURRENT(&gl_window_context)) < 0) {
        printf("FAIL\n");
        goto fail;
    }
    printf("OK\n");


    u_thread_context thread_context;
    u_set_current_context(&thread_context);
    u_register(U_ENGINE_PART_SHAPES_BIT | U_ENGINE_PART_UI_BIT);

    

    at7_ref_atlas();





    ui_font_interface font_interface;
    at7_init_font_interface(&font_interface, 0);

    
    ui_style style_white = {
        .color = {1,1,1,1},
        .origin = UI_ORIGIN_LEFT_BIT | UI_ORIGIN_TOP_BIT
    };
    ui_style_new(&style_white);



    ui_text_style text_style = {
        .word_spacing =   {1, UI_TEXT_SPACING_ADD}, // ??? really? just use int8_t
        .letter_spacing = {1, UI_TEXT_SPACING_ADD},
    };
    
    ui_text text = {
        .font_interface = &font_interface,
        .str = U_STRNT("Hello World"),
        .style = &style_white,
        .text_style = &text_style,
        .height = 25,
        .encoding = UI_TEXT_ENCODING_ASCII
    };
    printf("Creating text... ");
    if ((rsu = ui_text_new(&text))<0) {printf("FAIL\n");}
    printf("OK\n");

    uint32_t winw,winh;
    TW_WINDOW_GET_CLIENT_SIZE(window, &winw, &winh);
    u_begin_info begin_info = {.render_area = {winw, winh}};
    TW_WINDOW_SET_USERDATA(window, &begin_info);
    glClearColor(0.2f, 0.3f, 0.5f, 1.0f);

    // now that openGL and the begin info is setup, it's safe to hook the resize callback
    win_callbacks_A.ON_RESIZE = on_window_resize;

    TW_EVENT event;
    while (1) {
        if (TW_POLL_EVENT(window, &event) == TW_SUCCESS) {
            TW_DISPATCH_EVENT(&event);
        }
        if (event.LAST_DISPATCHED==TW_EVENT_CLOSE) {
            break;
        }

        glClear(GL_COLOR_BUFFER_BIT);
        u_begin(&begin_info);
        ui_text_draw(&text, 50, 50);
        u_end();
        TW_GL_SWAP_BUFFERS(window);
    }


    ui_text_delete(&text);
    ui_style_delete(&style_white);


    at7_deref_atlas();

    u_unregister(U_ENGINE_PART_SHAPES_BIT | U_ENGINE_PART_UI_BIT);

    TW_DESTROY_GL_CONTEXT(&gl_window_context);
    TW_DESTROY_GL_INSTANCE(&gl_instance);

    return 0;
fail:

    printf("[rs_err]: %d\n", rs);
    printf("[rsu_err]: %d\n", rsu);

    printf("\n\nPress any key to quit the program.");
    getc(stdin);


    if (window) {TW_WINDOW_CLOSE(window);}
    TW_DESTROY_GL_CONTEXT(&gl_window_context);
    TW_DESTROY_GL_INSTANCE(&gl_instance);

  
    return -1;
}