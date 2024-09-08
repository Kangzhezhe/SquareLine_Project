#include "ui.h"
#include "string.h"
lv_obj_t* table;
lv_obj_t* table1;
void ui_event_Switch1_update(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        _ui_flag_modify(table, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_TOGGLE);
    }
}

void ui_event_Switch2_update(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        _ui_flag_modify(table1, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_TOGGLE);
    }
}

void ui_event_Button1(lv_event_t * e){
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        if (strcmp(lv_label_get_text(ui_Label19), "暂停") == 0) {
            lv_label_set_text(ui_Label19, "继续");
        }else {
            lv_label_set_text(ui_Label19, "暂停");
        }
    }
}

char  logstr[1024];
#include "stdio.h"
void ui_event_Button4(lv_event_t * e){
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
       lv_dropdown_get_selected_str(ui_Dropdown2,logstr,1024);
        printf("%s\n",logstr);
    }
}

uint16_t img_data[256 * 256];
lv_obj_t * canvas_cam;
void lv_cam_canvas(void){
    // ��������
    canvas_cam = lv_canvas_create(ui_Container1);
    lv_canvas_set_buffer(canvas_cam, img_data, 256, 256, LV_COLOR_FORMAT_RGB565);
    lv_canvas_fill_bg(canvas_cam, lv_color_hex(0x000000), 255);
    lv_obj_set_align(canvas_cam, LV_ALIGN_CENTER);
    // lv_obj_set_pos(canvas_cam, 0, 0);

    lv_obj_set_size(canvas_cam, 256, 256);
    lv_obj_set_scrollbar_mode(canvas_cam, LV_SCROLLBAR_MODE_OFF);

    lv_obj_set_style_shadow_color(canvas_cam, lv_color_hex(0x2EC1D3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(canvas_cam, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(canvas_cam, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(canvas_cam, 10, LV_PART_MAIN | LV_STATE_DEFAULT);


    // lv_layer_t layer;
    // lv_canvas_init_layer(canvas_cam, &layer); 
    // lv_draw_rect_dsc_t dsc;
    // lv_draw_rect_dsc_init(&dsc);
    // dsc.border_color = lv_palette_main(LV_PALETTE_RED);
    // dsc.border_width = 1;
    // dsc.bg_opa = LV_OPA_TRANSP;
    lv_area_t coords = {50, 10, 216, 245};

    // lv_draw_rect(&layer, &dsc, &coords);

    // lv_canvas_finish_layer(canvas_cam, &layer);

    uint32_t x;
    uint32_t y;
    y = 10;
    for(x = 50; x < 216; x++) {
            lv_canvas_set_px(canvas_cam, x, y, lv_palette_main(LV_PALETTE_BLUE), LV_OPA_50);
    }
    y = 245;
    for(x = 50; x < 216; x++) {
            lv_canvas_set_px(canvas_cam, x, y, lv_palette_main(LV_PALETTE_BLUE), LV_OPA_20);
    }
    x = 50;
    for(y = 10; y < 245; y++) {
            lv_canvas_set_px(canvas_cam, x, y, lv_palette_main(LV_PALETTE_BLUE), LV_OPA_0);
    }
    x = 216;
    for(y = 10; y < 245; y++) {
            lv_canvas_set_px(canvas_cam, x, y, lv_palette_main(LV_PALETTE_BLUE), LV_OPA_0);
    }
}

void parseAndGetValue(char *buffer, char *keyword, char *result) {
    char *q = strstr(buffer, keyword);
    if (q != NULL) {
        char *start = strchr(q, ':') + 1;
        char *end = strchr(start, ',');
        if (end == NULL) {
            end = strchr(start, '\"');
            if(end==NULL)
                end = strchr(start, '\0');
        }
        int length = end - start;
        strncpy(result, start, length);
        result[length] = '\0';
    } else {
        result[0] = '\0'; // Set result to empty string if keyword not found
    }
}

void my_ui_init(void){
    lv_label_set_text(ui_Label16, LV_SYMBOL_PLUS);
    lv_label_set_text(ui_Label15, LV_SYMBOL_MINUS);


    table = lv_table_create(ui_TabPage2);
    lv_table_set_col_cnt(table, 4);
    lv_obj_set_width(table, 444);
    lv_obj_set_height(table, 212);
    lv_obj_set_x(table, -1);
    lv_obj_set_y(table, 13);
    lv_obj_set_align(table, LV_ALIGN_TOP_MID);
    lv_obj_set_style_text_font(table, &ui_font_Fontcnmsg, LV_PART_MAIN | LV_STATE_DEFAULT);

    /*Fill the first column*/
    lv_table_set_cell_value(table, 0, 0, "时间");
    lv_table_set_cell_value(table, 0, 1, "体温(*C)");
    lv_table_set_cell_value(table, 0, 2, "心率(bpm)");
    lv_table_set_cell_value(table, 0, 3, "血�?(%)");
    // lv_table_set_cell_value_fmt(table, 1, 0, "%d�?%d�? %02d:%02d", 7,4,23,59);
    // lv_table_set_cell_value_fmt(table, 1, 1, "%d.%d", 36,8);
    // lv_table_set_cell_value_fmt(table, 1, 2, "%d", 75);
    // lv_table_set_cell_value_fmt(table, 1, 3, "%d", 75);

      // 解析并填充表格数�?
    char RxBuffer[] = "tiwen:53.6,xueyang:79,xinlv:23,time:7 12 19 06;tiwen:12.2,xueyang:12,xinlv:83,time:7 12 19 06;tiwen:12.2,xueyang:12,xinlv:86,time:7 12 19 06;";
    int row = 1;
    char *record = strtok(RxBuffer, ";");
    while (record != NULL) {
        char tiwen[10], xueyang[10], xinlv[10], time[20];
        
        parseAndGetValue(record, "tiwen", tiwen);
        parseAndGetValue(record, "xueyang", xueyang);
        parseAndGetValue(record, "xinlv", xinlv);
        parseAndGetValue(record, "time", time);
        
        // �?充表格数�?
        int month, day, hour, minute;
        sscanf(time, "%d %d %d %d", &month, &day, &hour, &minute);
        
        lv_table_set_cell_value_fmt(table, row, 0, "%d�?%d�? %02d:%02d", month, day, hour, minute);
        lv_table_set_cell_value_fmt(table, row, 1, "%s", tiwen);
        lv_table_set_cell_value_fmt(table, row, 2, "%s", xinlv);
        lv_table_set_cell_value_fmt(table, row, 3, "%s", xueyang);
        
        row++;
        record = strtok(NULL, ";");
    }


     table1 = lv_table_create(ui_TabPage3);
    lv_table_set_col_cnt(table1, 2);
    lv_obj_set_width(table1, 444);
    lv_obj_set_height(table1, 212);
    lv_obj_set_x(table1, -1);
    lv_obj_set_y(table1, 13);
    lv_obj_set_align(table1, LV_ALIGN_TOP_MID);
    lv_obj_set_style_text_font(table1, &ui_font_Fontcnmsg, LV_PART_MAIN | LV_STATE_DEFAULT);


    /*Fill the first column*/
    lv_table_set_cell_value(table1, 0, 0, "时间");
    lv_table_set_cell_value(table1, 0, 1, "�?�?");
    lv_table_set_cell_value_fmt(table1, 1, 0, "%d�?%d�? %02d:%02d", 7,4,23,59);
    lv_table_set_cell_value(table1, 1, 1, "阿莫西林");


    // lv_table_set_row_cnt(table, 1);
    // lv_table_set_row_cnt(table1, 1);
    lv_cam_canvas();
    lv_obj_add_event_cb(ui_Switch1, ui_event_Switch1_update, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_Switch2, ui_event_Switch2_update, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_Button1, ui_event_Button1, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_Button4, ui_event_Button4, LV_EVENT_ALL, NULL);
}