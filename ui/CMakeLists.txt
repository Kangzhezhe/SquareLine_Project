SET(SOURCES screens/ui_Screen1.c
    ui.c
    components/ui_comp_hook.c
    ui_helpers.c
    ui_events.c
    images/ui_img_backgroud4_png.c
    images/ui_img_face_png.c
    images/ui_img_backgroud3_png.c
    images/ui_img_backgroud1_png.c
    images/ui_img_backgroud2_png.c
    images/ui_img_2_png.c
    fonts/ui_font_Fontcn15.c
    fonts/ui_font_Fontcn20.c
    fonts/ui_font_Fontcn30.c
    fonts/ui_font_Fontcnmsg.c)

add_library(ui ${SOURCES})
