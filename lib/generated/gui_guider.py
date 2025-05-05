# Copyright 2025 NXP
# NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
# accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
# activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
# comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
# terms, then you may not retain, install, activate or otherwise use the software.

import SDL
import utime as time
import usys as sys
import lvgl as lv
import lodepng as png
import ustruct
import fs_driver

lv.init()
SDL.init(w=480,h=320)

# Register SDL display driver.
disp_buf1 = lv.disp_draw_buf_t()
buf1_1 = bytearray(480*320*4)
disp_buf1.init(buf1_1, None, len(buf1_1)//4)
disp_drv = lv.disp_drv_t()
disp_drv.init()
disp_drv.draw_buf = disp_buf1
disp_drv.flush_cb = SDL.monitor_flush
disp_drv.hor_res = 480
disp_drv.ver_res = 320
disp_drv.register()

# Regsiter SDL mouse driver
indev_drv = lv.indev_drv_t()
indev_drv.init()
indev_drv.type = lv.INDEV_TYPE.POINTER
indev_drv.read_cb = SDL.mouse_read
indev_drv.register()

fs_drv = lv.fs_drv_t()
fs_driver.fs_register(fs_drv, 'Z')

# Below: Taken from https://github.com/lvgl/lv_binding_micropython/blob/master/driver/js/imagetools.py#L22-L94

COLOR_SIZE = lv.color_t.__SIZE__
COLOR_IS_SWAPPED = hasattr(lv.color_t().ch,'green_h')

class lodepng_error(RuntimeError):
    def __init__(self, err):
        if type(err) is int:
            super().__init__(png.error_text(err))
        else:
            super().__init__(err)

# Parse PNG file header
# Taken from https://github.com/shibukawa/imagesize_py/blob/ffef30c1a4715c5acf90e8945ceb77f4a2ed2d45/imagesize.py#L63-L85

def get_png_info(decoder, src, header):
    # Only handle variable image types

    if lv.img.src_get_type(src) != lv.img.SRC.VARIABLE:
        return lv.RES.INV

    data = lv.img_dsc_t.__cast__(src).data
    if data == None:
        return lv.RES.INV

    png_header = bytes(data.__dereference__(24))

    if png_header.startswith(b'\211PNG\r\n\032\n'):
        if png_header[12:16] == b'IHDR':
            start = 16
        # Maybe this is for an older PNG version.
        else:
            start = 8
        try:
            width, height = ustruct.unpack(">LL", png_header[start:start+8])
        except ustruct.error:
            return lv.RES.INV
    else:
        return lv.RES.INV

    header.always_zero = 0
    header.w = width
    header.h = height
    header.cf = lv.img.CF.TRUE_COLOR_ALPHA

    return lv.RES.OK

def convert_rgba8888_to_bgra8888(img_view):
    for i in range(0, len(img_view), lv.color_t.__SIZE__):
        ch = lv.color_t.__cast__(img_view[i:i]).ch
        ch.red, ch.blue = ch.blue, ch.red

# Read and parse PNG file

def open_png(decoder, dsc):
    img_dsc = lv.img_dsc_t.__cast__(dsc.src)
    png_data = img_dsc.data
    png_size = img_dsc.data_size
    png_decoded = png.C_Pointer()
    png_width = png.C_Pointer()
    png_height = png.C_Pointer()
    error = png.decode32(png_decoded, png_width, png_height, png_data, png_size)
    if error:
        raise lodepng_error(error)
    img_size = png_width.int_val * png_height.int_val * 4
    img_data = png_decoded.ptr_val
    img_view = img_data.__dereference__(img_size)

    if COLOR_SIZE == 4:
        convert_rgba8888_to_bgra8888(img_view)
    else:
        raise lodepng_error("Error: Color mode not supported yet!")

    dsc.img_data = img_data
    return lv.RES.OK

# Above: Taken from https://github.com/lvgl/lv_binding_micropython/blob/master/driver/js/imagetools.py#L22-L94

decoder = lv.img.decoder_create()
decoder.info_cb = get_png_info
decoder.open_cb = open_png

def anim_x_cb(obj, v):
    obj.set_x(v)

def anim_y_cb(obj, v):
    obj.set_y(v)

def anim_width_cb(obj, v):
    obj.set_width(v)

def anim_height_cb(obj, v):
    obj.set_height(v)

def anim_img_zoom_cb(obj, v):
    obj.set_zoom(v)

def anim_img_rotate_cb(obj, v):
    obj.set_angle(v)

global_font_cache = {}
def test_font(font_family, font_size):
    global global_font_cache
    if font_family + str(font_size) in global_font_cache:
        return global_font_cache[font_family + str(font_size)]
    if font_size % 2:
        candidates = [
            (font_family, font_size),
            (font_family, font_size-font_size%2),
            (font_family, font_size+font_size%2),
            ("montserrat", font_size-font_size%2),
            ("montserrat", font_size+font_size%2),
            ("montserrat", 16)
        ]
    else:
        candidates = [
            (font_family, font_size),
            ("montserrat", font_size),
            ("montserrat", 16)
        ]
    for (family, size) in candidates:
        try:
            if eval(f'lv.font_{family}_{size}'):
                global_font_cache[font_family + str(font_size)] = eval(f'lv.font_{family}_{size}')
                if family != font_family or size != font_size:
                    print(f'WARNING: lv.font_{family}_{size} is used!')
                return eval(f'lv.font_{family}_{size}')
        except AttributeError:
            try:
                load_font = lv.font_load(f"Z:MicroPython/lv_font_{family}_{size}.fnt")
                global_font_cache[font_family + str(font_size)] = load_font
                return load_font
            except:
                if family == font_family and size == font_size:
                    print(f'WARNING: lv.font_{family}_{size} is NOT supported!')

global_image_cache = {}
def load_image(file):
    global global_image_cache
    if file in global_image_cache:
        return global_image_cache[file]
    try:
        with open(file,'rb') as f:
            data = f.read()
    except:
        print(f'Could not open {file}')
        sys.exit()

    img = lv.img_dsc_t({
        'data_size': len(data),
        'data': data
    })
    global_image_cache[file] = img
    return img

def calendar_event_handler(e,obj):
    code = e.get_code()

    if code == lv.EVENT.VALUE_CHANGED:
        source = e.get_current_target()
        date = lv.calendar_date_t()
        if source.get_pressed_date(date) == lv.RES.OK:
            source.set_highlighted_dates([date], 1)

def spinbox_increment_event_cb(e, obj):
    code = e.get_code()
    if code == lv.EVENT.SHORT_CLICKED or code == lv.EVENT.LONG_PRESSED_REPEAT:
        obj.increment()
def spinbox_decrement_event_cb(e, obj):
    code = e.get_code()
    if code == lv.EVENT.SHORT_CLICKED or code == lv.EVENT.LONG_PRESSED_REPEAT:
        obj.decrement()

def digital_clock_cb(timer, obj, current_time, show_second, use_ampm):
    hour = int(current_time[0])
    minute = int(current_time[1])
    second = int(current_time[2])
    ampm = current_time[3]
    second = second + 1
    if second == 60:
        second = 0
        minute = minute + 1
        if minute == 60:
            minute = 0
            hour = hour + 1
            if use_ampm:
                if hour == 12:
                    if ampm == 'AM':
                        ampm = 'PM'
                    elif ampm == 'PM':
                        ampm = 'AM'
                if hour > 12:
                    hour = hour % 12
    hour = hour % 24
    if use_ampm:
        if show_second:
            obj.set_text("%d:%02d:%02d %s" %(hour, minute, second, ampm))
        else:
            obj.set_text("%d:%02d %s" %(hour, minute, ampm))
    else:
        if show_second:
            obj.set_text("%d:%02d:%02d" %(hour, minute, second))
        else:
            obj.set_text("%d:%02d" %(hour, minute))
    current_time[0] = hour
    current_time[1] = minute
    current_time[2] = second
    current_time[3] = ampm

def analog_clock_cb(timer, obj):
    datetime = time.localtime()
    hour = datetime[3]
    if hour >= 12: hour = hour - 12
    obj.set_time(hour, datetime[4], datetime[5])

def datetext_event_handler(e, obj):
    code = e.get_code()
    target = e.get_target()
    if code == lv.EVENT.FOCUSED:
        if obj is None:
            bg = lv.layer_top()
            bg.add_flag(lv.obj.FLAG.CLICKABLE)
            obj = lv.calendar(bg)
            scr = target.get_screen()
            scr_height = scr.get_height()
            scr_width = scr.get_width()
            obj.set_size(int(scr_width * 0.8), int(scr_height * 0.8))
            datestring = target.get_text()
            year = int(datestring.split('/')[0])
            month = int(datestring.split('/')[1])
            day = int(datestring.split('/')[2])
            obj.set_showed_date(year, month)
            highlighted_days=[lv.calendar_date_t({'year':year, 'month':month, 'day':day})]
            obj.set_highlighted_dates(highlighted_days, 1)
            obj.align(lv.ALIGN.CENTER, 0, 0)
            lv.calendar_header_arrow(obj)
            obj.add_event_cb(lambda e: datetext_calendar_event_handler(e, target), lv.EVENT.ALL, None)
            scr.update_layout()

def datetext_calendar_event_handler(e, obj):
    code = e.get_code()
    target = e.get_current_target()
    if code == lv.EVENT.VALUE_CHANGED:
        date = lv.calendar_date_t()
        if target.get_pressed_date(date) == lv.RES.OK:
            obj.set_text(f"{date.year}/{date.month}/{date.day}")
            bg = lv.layer_top()
            bg.clear_flag(lv.obj.FLAG.CLICKABLE)
            bg.set_style_bg_opa(lv.OPA.TRANSP, 0)
            target.delete()

# Create main_page
main_page = lv.obj()
main_page.set_size(480, 320)
main_page.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for main_page, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
main_page.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page.set_style_bg_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
main_page.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create main_page_label_5
main_page_label_5 = lv.label(main_page)
main_page_label_5.set_text("期望电流(mA)")
main_page_label_5.set_long_mode(lv.label.LONG.WRAP)
main_page_label_5.set_width(lv.pct(100))
main_page_label_5.set_pos(140, 259)
main_page_label_5.set_size(111, 20)
# Set style for main_page_label_5, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
main_page_label_5.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_label_5.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_label_5.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_label_5.set_style_text_font(test_font("OPPOSans_Medium", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_label_5.set_style_text_opa(187, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_label_5.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_label_5.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_label_5.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_label_5.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_label_5.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_label_5.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_label_5.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_label_5.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_label_5.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create main_page_measure_power_label
main_page_measure_power_label = lv.label(main_page)
main_page_measure_power_label.set_text("0.000")
main_page_measure_power_label.set_long_mode(lv.label.LONG.WRAP)
main_page_measure_power_label.set_width(lv.pct(100))
main_page_measure_power_label.set_pos(164, 161)
main_page_measure_power_label.set_size(130, 30)
# Set style for main_page_measure_power_label, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
main_page_measure_power_label.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_measure_power_label.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_measure_power_label.set_style_text_color(lv.color_hex(0xf98aff), lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_measure_power_label.set_style_text_font(test_font("IntelOneMono_Medium", 30), lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_measure_power_label.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_measure_power_label.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_measure_power_label.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_measure_power_label.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_measure_power_label.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_measure_power_label.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_measure_power_label.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_measure_power_label.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_measure_power_label.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_measure_power_label.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create main_page_measure_current_label
main_page_measure_current_label = lv.label(main_page)
main_page_measure_current_label.set_text("0.000")
main_page_measure_current_label.set_long_mode(lv.label.LONG.WRAP)
main_page_measure_current_label.set_width(lv.pct(100))
main_page_measure_current_label.set_pos(28, 68)
main_page_measure_current_label.set_size(130, 31)
# Set style for main_page_measure_current_label, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
main_page_measure_current_label.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_measure_current_label.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_measure_current_label.set_style_text_color(lv.color_hex(0xff9600), lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_measure_current_label.set_style_text_font(test_font("IntelOneMono_Medium", 30), lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_measure_current_label.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_measure_current_label.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_measure_current_label.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_measure_current_label.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_measure_current_label.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_measure_current_label.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_measure_current_label.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_measure_current_label.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_measure_current_label.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_measure_current_label.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create main_page_measure_voltage_label
main_page_measure_voltage_label = lv.label(main_page)
main_page_measure_voltage_label.set_text("0.000")
main_page_measure_voltage_label.set_long_mode(lv.label.LONG.WRAP)
main_page_measure_voltage_label.set_width(lv.pct(100))
main_page_measure_voltage_label.set_pos(164, 68)
main_page_measure_voltage_label.set_size(130, 30)
# Set style for main_page_measure_voltage_label, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
main_page_measure_voltage_label.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_measure_voltage_label.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_measure_voltage_label.set_style_text_color(lv.color_hex(0xa6ff00), lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_measure_voltage_label.set_style_text_font(test_font("IntelOneMono_Medium", 30), lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_measure_voltage_label.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_measure_voltage_label.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_measure_voltage_label.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_measure_voltage_label.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_measure_voltage_label.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_measure_voltage_label.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_measure_voltage_label.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_measure_voltage_label.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_measure_voltage_label.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_measure_voltage_label.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create main_page_set_current_label
main_page_set_current_label = lv.label(main_page)
main_page_set_current_label.set_text("0")
main_page_set_current_label.set_long_mode(lv.label.LONG.WRAP)
main_page_set_current_label.set_width(lv.pct(100))
main_page_set_current_label.set_pos(156, 284)
main_page_set_current_label.set_size(76, 18)
# Set style for main_page_set_current_label, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
main_page_set_current_label.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_set_current_label.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_set_current_label.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_set_current_label.set_style_text_font(test_font("IntelOneMono_Medium", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_set_current_label.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_set_current_label.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_set_current_label.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_set_current_label.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_set_current_label.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_set_current_label.set_style_bg_color(lv.color_hex(0x505050), lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_set_current_label.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_set_current_label.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_set_current_label.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_set_current_label.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_set_current_label.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_set_current_label.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create main_page_switch2chart
main_page_switch2chart = lv.switch(main_page)
main_page_switch2chart.set_pos(47, 219)
main_page_switch2chart.set_size(58, 27)
# Set style for main_page_switch2chart, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
main_page_switch2chart.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_switch2chart.set_style_bg_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_switch2chart.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_switch2chart.set_style_border_width(2, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_switch2chart.set_style_border_opa(153, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_switch2chart.set_style_border_color(lv.color_hex(0x00bdff), lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_switch2chart.set_style_border_side(lv.BORDER_SIDE.FULL, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_switch2chart.set_style_radius(10, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_switch2chart.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Set style for main_page_switch2chart, Part: lv.PART.INDICATOR, State: lv.STATE.CHECKED.
main_page_switch2chart.set_style_bg_opa(223, lv.PART.INDICATOR|lv.STATE.CHECKED)
main_page_switch2chart.set_style_bg_color(lv.color_hex(0x13359a), lv.PART.INDICATOR|lv.STATE.CHECKED)
main_page_switch2chart.set_style_bg_grad_dir(lv.GRAD_DIR.HOR, lv.PART.INDICATOR|lv.STATE.CHECKED)
main_page_switch2chart.set_style_bg_grad_color(lv.color_hex(0x2195f6), lv.PART.INDICATOR|lv.STATE.CHECKED)
main_page_switch2chart.set_style_bg_main_stop(0, lv.PART.INDICATOR|lv.STATE.CHECKED)
main_page_switch2chart.set_style_bg_grad_stop(255, lv.PART.INDICATOR|lv.STATE.CHECKED)
main_page_switch2chart.set_style_border_width(0, lv.PART.INDICATOR|lv.STATE.CHECKED)

# Set style for main_page_switch2chart, Part: lv.PART.KNOB, State: lv.STATE.DEFAULT.
main_page_switch2chart.set_style_bg_opa(255, lv.PART.KNOB|lv.STATE.DEFAULT)
main_page_switch2chart.set_style_bg_color(lv.color_hex(0xffffff), lv.PART.KNOB|lv.STATE.DEFAULT)
main_page_switch2chart.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.KNOB|lv.STATE.DEFAULT)
main_page_switch2chart.set_style_border_width(0, lv.PART.KNOB|lv.STATE.DEFAULT)
main_page_switch2chart.set_style_radius(10, lv.PART.KNOB|lv.STATE.DEFAULT)

# Create main_page_btn_1
main_page_btn_1 = lv.btn(main_page)
main_page_btn_1_label = lv.label(main_page_btn_1)
main_page_btn_1_label.set_text("")
main_page_btn_1_label.set_long_mode(lv.label.LONG.WRAP)
main_page_btn_1_label.set_width(lv.pct(100))
main_page_btn_1_label.align(lv.ALIGN.CENTER, 0, 0)
main_page_btn_1.set_style_pad_all(0, lv.STATE.DEFAULT)
main_page_btn_1.set_pos(29, 255)
main_page_btn_1.set_size(100, 50)
# Set style for main_page_btn_1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
main_page_btn_1.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_btn_1.set_style_bg_color(lv.color_hex(0x292929), lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_btn_1.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_btn_1.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_btn_1.set_style_radius(5, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_btn_1.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_btn_1.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_btn_1.set_style_text_font(test_font("montserratMedium", 18), lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_btn_1.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_btn_1.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create main_page_label_1
main_page_label_1 = lv.label(main_page)
main_page_label_1.set_text("电流(mA)")
main_page_label_1.set_long_mode(lv.label.LONG.WRAP)
main_page_label_1.set_width(lv.pct(100))
main_page_label_1.set_pos(28, 31)
main_page_label_1.set_size(130, 25)
# Set style for main_page_label_1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
main_page_label_1.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_label_1.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_label_1.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_label_1.set_style_text_font(test_font("OPPOSans_Medium", 25), lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_label_1.set_style_text_opa(164, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_label_1.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_label_1.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_label_1.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_label_1.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_label_1.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_label_1.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_label_1.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_label_1.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_label_1.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create main_page_label_2
main_page_label_2 = lv.label(main_page)
main_page_label_2.set_text("电压(V)")
main_page_label_2.set_long_mode(lv.label.LONG.WRAP)
main_page_label_2.set_width(lv.pct(100))
main_page_label_2.set_pos(164, 31)
main_page_label_2.set_size(130, 25)
# Set style for main_page_label_2, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
main_page_label_2.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_label_2.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_label_2.set_style_text_color(lv.color_hex(0xfafafa), lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_label_2.set_style_text_font(test_font("OPPOSans_Medium", 25), lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_label_2.set_style_text_opa(171, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_label_2.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_label_2.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_label_2.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_label_2.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_label_2.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_label_2.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_label_2.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_label_2.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_label_2.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create main_page_label_4
main_page_label_4 = lv.label(main_page)
main_page_label_4.set_text("负载功率(W)")
main_page_label_4.set_long_mode(lv.label.LONG.WRAP)
main_page_label_4.set_width(lv.pct(100))
main_page_label_4.set_pos(164, 124)
main_page_label_4.set_size(130, 25)
# Set style for main_page_label_4, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
main_page_label_4.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_label_4.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_label_4.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_label_4.set_style_text_font(test_font("OPPOSans_Medium", 22), lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_label_4.set_style_text_opa(171, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_label_4.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_label_4.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_label_4.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_label_4.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_label_4.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_label_4.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_label_4.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_label_4.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_label_4.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create main_page_line_1
main_page_line_1 = lv.line(main_page)
main_page_line_1_line_points = [{"x":0, "y":0},{"x":0, "y":178},]
main_page_line_1.set_points(main_page_line_1_line_points, 2)
main_page_line_1.set_pos(161, 31)
main_page_line_1.set_size(1, 177)
# Set style for main_page_line_1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
main_page_line_1.set_style_line_width(2, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_line_1.set_style_line_color(lv.color_hex(0x757575), lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_line_1.set_style_line_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_line_1.set_style_line_rounded(True, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create main_page_line_2
main_page_line_2 = lv.line(main_page)
main_page_line_2_line_points = [{"x":0, "y":0},{"x":250, "y":0},]
main_page_line_2.set_points(main_page_line_2_line_points, 2)
main_page_line_2.set_pos(37, 116)
main_page_line_2.set_size(250, 1)
# Set style for main_page_line_2, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
main_page_line_2.set_style_line_width(2, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_line_2.set_style_line_color(lv.color_hex(0x757575), lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_line_2.set_style_line_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_line_2.set_style_line_rounded(True, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create main_page_tileview_1
main_page_tileview_1 = lv.tileview(main_page)
main_page_tileview_1_main = main_page_tileview_1.add_tile(0, 0, lv.DIR.BOTTOM)
main_page_tileview_1_setting = main_page_tileview_1.add_tile(0, 1, lv.DIR.TOP | lv.DIR.BOTTOM)
main_page_tileview_1_about = main_page_tileview_1.add_tile(0, 2, lv.DIR.TOP)
main_page_tileview_1.set_pos(333, 0)
main_page_tileview_1.set_size(151, 320)
main_page_tileview_1.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for main_page_tileview_1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
main_page_tileview_1.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_tileview_1.set_style_bg_color(lv.color_hex(0x0b0b0b), lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_tileview_1.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_tileview_1.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_tileview_1.set_style_shadow_width(46, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_tileview_1.set_style_shadow_color(lv.color_hex(0xe7ffd7), lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_tileview_1.set_style_shadow_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_tileview_1.set_style_shadow_spread(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_tileview_1.set_style_shadow_ofs_x(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_tileview_1.set_style_shadow_ofs_y(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Set style for main_page_tileview_1, Part: lv.PART.SCROLLBAR, State: lv.STATE.DEFAULT.
main_page_tileview_1.set_style_bg_opa(255, lv.PART.SCROLLBAR|lv.STATE.DEFAULT)
main_page_tileview_1.set_style_bg_color(lv.color_hex(0xeaeff3), lv.PART.SCROLLBAR|lv.STATE.DEFAULT)
main_page_tileview_1.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.SCROLLBAR|lv.STATE.DEFAULT)
main_page_tileview_1.set_style_radius(0, lv.PART.SCROLLBAR|lv.STATE.DEFAULT)

# Create main_page_spinbox_1
main_page_spinbox_1 = lv.spinbox(main_page_tileview_1_about)
main_page_spinbox_1.set_width(85)
main_page_spinbox_1.set_height(23)
main_page_spinbox_1.set_digit_format(4, 4)
main_page_spinbox_1.set_range(-9999, 9999)
main_page_spinbox_1_height = main_page_spinbox_1.get_height()
main_page_spinbox_1_btn_plus = lv.btn(main_page_tileview_1_about)
main_page_spinbox_1_btn_plus.set_size(main_page_spinbox_1_height, main_page_spinbox_1_height)
main_page_spinbox_1_btn_plus.set_style_bg_img_src(lv.SYMBOL.PLUS, 0)
main_page_spinbox_1_btn_plus.add_event_cb(lambda e: spinbox_increment_event_cb(e, main_page_spinbox_1), lv.EVENT.ALL, None)
main_page_spinbox_1_btn_minus = lv.btn(main_page_tileview_1_about)
main_page_spinbox_1_btn_minus.set_size(main_page_spinbox_1_height, main_page_spinbox_1_height)
main_page_spinbox_1_btn_minus.set_style_bg_img_src(lv.SYMBOL.MINUS, 0)
main_page_spinbox_1_btn_minus.add_event_cb(lambda e: spinbox_decrement_event_cb(e, main_page_spinbox_1), lv.EVENT.ALL, None)
main_page_spinbox_1.set_pos(33, 83)
main_page_spinbox_1_btn_plus.align_to(main_page_spinbox_1, lv.ALIGN.OUT_RIGHT_MID, 5, 0)
main_page_spinbox_1_btn_minus.align_to(main_page_spinbox_1, lv.ALIGN.OUT_LEFT_MID, -5, 0)
# Set style for main_page_spinbox_1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
main_page_spinbox_1.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_spinbox_1.set_style_bg_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_spinbox_1.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_spinbox_1.set_style_border_width(2, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_spinbox_1.set_style_border_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_spinbox_1.set_style_border_color(lv.color_hex(0xdbdbdb), lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_spinbox_1.set_style_border_side(lv.BORDER_SIDE.FULL, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_spinbox_1.set_style_pad_top(30, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_spinbox_1.set_style_pad_right(10, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_spinbox_1.set_style_pad_bottom(10, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_spinbox_1.set_style_pad_left(8, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_spinbox_1.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_spinbox_1.set_style_text_font(test_font("montserratMedium", 10), lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_spinbox_1.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_spinbox_1.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_spinbox_1.set_style_radius(5, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_spinbox_1.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Set style for main_page_spinbox_1, Part: lv.PART.CURSOR, State: lv.STATE.DEFAULT.
main_page_spinbox_1.set_style_text_color(lv.color_hex(0xffffff), lv.PART.CURSOR|lv.STATE.DEFAULT)
main_page_spinbox_1.set_style_text_font(test_font("montserratMedium", 12), lv.PART.CURSOR|lv.STATE.DEFAULT)
main_page_spinbox_1.set_style_text_opa(255, lv.PART.CURSOR|lv.STATE.DEFAULT)
main_page_spinbox_1.set_style_bg_opa(255, lv.PART.CURSOR|lv.STATE.DEFAULT)
main_page_spinbox_1.set_style_bg_color(lv.color_hex(0x929292), lv.PART.CURSOR|lv.STATE.DEFAULT)
main_page_spinbox_1.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.CURSOR|lv.STATE.DEFAULT)
# Set style for main_page_spinbox_1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
style_main_page_spinbox_1_extra_btns_main_default = lv.style_t()
style_main_page_spinbox_1_extra_btns_main_default.init()
style_main_page_spinbox_1_extra_btns_main_default.set_text_color(lv.color_hex(0xffffff))
style_main_page_spinbox_1_extra_btns_main_default.set_text_font(test_font("montserratMedium", 9))
style_main_page_spinbox_1_extra_btns_main_default.set_text_opa(255)
style_main_page_spinbox_1_extra_btns_main_default.set_bg_opa(255)
style_main_page_spinbox_1_extra_btns_main_default.set_bg_color(lv.color_hex(0x66686a))
style_main_page_spinbox_1_extra_btns_main_default.set_bg_grad_dir(lv.GRAD_DIR.NONE)
style_main_page_spinbox_1_extra_btns_main_default.set_border_width(0)
style_main_page_spinbox_1_extra_btns_main_default.set_radius(3)
style_main_page_spinbox_1_extra_btns_main_default.set_shadow_width(0)
main_page_spinbox_1_btn_plus.add_style(style_main_page_spinbox_1_extra_btns_main_default, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_spinbox_1_btn_minus.add_style(style_main_page_spinbox_1_extra_btns_main_default, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create main_page_label_3
main_page_label_3 = lv.label(main_page_tileview_1_about)
main_page_label_3.set_text("负载调整率测量")
main_page_label_3.set_long_mode(lv.label.LONG.WRAP)
main_page_label_3.set_width(lv.pct(100))
main_page_label_3.set_pos(13, 163)
main_page_label_3.set_size(130, 24)
# Set style for main_page_label_3, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
main_page_label_3.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_label_3.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_label_3.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_label_3.set_style_text_font(test_font("OPPOSans_Medium", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_label_3.set_style_text_opa(177, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_label_3.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_label_3.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_label_3.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_label_3.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_label_3.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_label_3.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_label_3.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_label_3.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_label_3.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create main_page_measure_resistance_label
main_page_measure_resistance_label = lv.label(main_page_tileview_1_about)
main_page_measure_resistance_label.set_text("0.000")
main_page_measure_resistance_label.set_long_mode(lv.label.LONG.WRAP)
main_page_measure_resistance_label.set_width(lv.pct(100))
main_page_measure_resistance_label.set_pos(12, 184)
main_page_measure_resistance_label.set_size(130, 30)
# Set style for main_page_measure_resistance_label, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
main_page_measure_resistance_label.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_measure_resistance_label.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_measure_resistance_label.set_style_text_color(lv.color_hex(0x00bdff), lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_measure_resistance_label.set_style_text_font(test_font("IntelOneMono_Medium", 30), lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_measure_resistance_label.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_measure_resistance_label.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_measure_resistance_label.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_measure_resistance_label.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_measure_resistance_label.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_measure_resistance_label.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_measure_resistance_label.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_measure_resistance_label.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_measure_resistance_label.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_measure_resistance_label.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create main_page_digital_clock_1
main_page_digital_clock_1_time = [int(0), int(00), int(00), ""]
main_page_digital_clock_1 = lv.dclock(main_page, "0:00:00")
main_page_digital_clock_1_timer = lv.timer_create_basic()
main_page_digital_clock_1_timer.set_period(1000)
main_page_digital_clock_1_timer.set_cb(lambda src: digital_clock_cb(main_page_digital_clock_1_timer, main_page_digital_clock_1, main_page_digital_clock_1_time, True, False ))
main_page_digital_clock_1.set_pos(27, 154)
main_page_digital_clock_1.set_size(130, 36)
# Set style for main_page_digital_clock_1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
main_page_digital_clock_1.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_digital_clock_1.set_style_text_color(lv.color_hex(0x00d9ff), lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_digital_clock_1.set_style_text_font(test_font("IntelOneMono_Medium", 26), lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_digital_clock_1.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_digital_clock_1.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_digital_clock_1.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_digital_clock_1.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_digital_clock_1.set_style_pad_top(7, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_digital_clock_1.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_digital_clock_1.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_digital_clock_1.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_digital_clock_1.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create main_page_label_6
main_page_label_6 = lv.label(main_page)
main_page_label_6.set_text("运行时间")
main_page_label_6.set_long_mode(lv.label.LONG.WRAP)
main_page_label_6.set_width(lv.pct(100))
main_page_label_6.set_pos(24, 124)
main_page_label_6.set_size(130, 25)
# Set style for main_page_label_6, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
main_page_label_6.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_label_6.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_label_6.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_label_6.set_style_text_font(test_font("OPPOSans_Medium", 22), lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_label_6.set_style_text_opa(171, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_label_6.set_style_text_letter_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_label_6.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_label_6.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_label_6.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_label_6.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_label_6.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_label_6.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_label_6.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
main_page_label_6.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

main_page.update_layout()
# Create chart_page
chart_page = lv.obj()
chart_page.set_size(480, 320)
chart_page.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for chart_page, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
chart_page.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create chart_page_sw_1
chart_page_sw_1 = lv.switch(chart_page)
chart_page_sw_1.set_pos(29, 224)
chart_page_sw_1.set_size(66, 22)
# Set style for chart_page_sw_1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
chart_page_sw_1.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
chart_page_sw_1.set_style_bg_color(lv.color_hex(0xe6e2e6), lv.PART.MAIN|lv.STATE.DEFAULT)
chart_page_sw_1.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
chart_page_sw_1.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
chart_page_sw_1.set_style_radius(10, lv.PART.MAIN|lv.STATE.DEFAULT)
chart_page_sw_1.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Set style for chart_page_sw_1, Part: lv.PART.INDICATOR, State: lv.STATE.CHECKED.
chart_page_sw_1.set_style_bg_opa(255, lv.PART.INDICATOR|lv.STATE.CHECKED)
chart_page_sw_1.set_style_bg_color(lv.color_hex(0x2195f6), lv.PART.INDICATOR|lv.STATE.CHECKED)
chart_page_sw_1.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.INDICATOR|lv.STATE.CHECKED)
chart_page_sw_1.set_style_border_width(0, lv.PART.INDICATOR|lv.STATE.CHECKED)

# Set style for chart_page_sw_1, Part: lv.PART.KNOB, State: lv.STATE.DEFAULT.
chart_page_sw_1.set_style_bg_opa(255, lv.PART.KNOB|lv.STATE.DEFAULT)
chart_page_sw_1.set_style_bg_color(lv.color_hex(0xffffff), lv.PART.KNOB|lv.STATE.DEFAULT)
chart_page_sw_1.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.KNOB|lv.STATE.DEFAULT)
chart_page_sw_1.set_style_border_width(0, lv.PART.KNOB|lv.STATE.DEFAULT)
chart_page_sw_1.set_style_radius(10, lv.PART.KNOB|lv.STATE.DEFAULT)

# Create chart_page_btn_1
chart_page_btn_1 = lv.btn(chart_page)
chart_page_btn_1_label = lv.label(chart_page_btn_1)
chart_page_btn_1_label.set_text("Button")
chart_page_btn_1_label.set_long_mode(lv.label.LONG.WRAP)
chart_page_btn_1_label.set_width(lv.pct(100))
chart_page_btn_1_label.align(lv.ALIGN.CENTER, 0, 0)
chart_page_btn_1.set_style_pad_all(0, lv.STATE.DEFAULT)
chart_page_btn_1.set_pos(29, 255)
chart_page_btn_1.set_size(100, 50)
# Set style for chart_page_btn_1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
chart_page_btn_1.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
chart_page_btn_1.set_style_bg_color(lv.color_hex(0x2195f6), lv.PART.MAIN|lv.STATE.DEFAULT)
chart_page_btn_1.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
chart_page_btn_1.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
chart_page_btn_1.set_style_radius(5, lv.PART.MAIN|lv.STATE.DEFAULT)
chart_page_btn_1.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
chart_page_btn_1.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
chart_page_btn_1.set_style_text_font(test_font("montserratMedium", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
chart_page_btn_1.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
chart_page_btn_1.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create chart_page_chart_1
chart_page_chart_1 = lv.chart(chart_page)
chart_page_chart_1.set_type(lv.chart.TYPE.LINE)
chart_page_chart_1.set_div_line_count(3, 5)
chart_page_chart_1.set_point_count(5)
chart_page_chart_1.set_range(lv.chart.AXIS.PRIMARY_Y, 0, 100)
chart_page_chart_1.set_axis_tick(lv.chart.AXIS.PRIMARY_Y, 10, 5, 5, 10, True, 40)
chart_page_chart_1.set_range(lv.chart.AXIS.SECONDARY_Y, 0, 100)
chart_page_chart_1.set_zoom_x(256)
chart_page_chart_1.set_zoom_y(256)
chart_page_chart_1_series_0 = chart_page_chart_1.add_series(lv.color_hex(0x2F35DA), lv.chart.AXIS.PRIMARY_Y)
chart_page_chart_1.set_next_value(chart_page_chart_1_series_0, 1)
chart_page_chart_1.set_next_value(chart_page_chart_1_series_0, 20)
chart_page_chart_1.set_next_value(chart_page_chart_1_series_0, 30)
chart_page_chart_1.set_next_value(chart_page_chart_1_series_0, 40)
chart_page_chart_1.set_next_value(chart_page_chart_1_series_0, 5)
chart_page_chart_1.set_pos(117, 10)
chart_page_chart_1.set_size(356, 238)
chart_page_chart_1.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for chart_page_chart_1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
chart_page_chart_1.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
chart_page_chart_1.set_style_bg_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
chart_page_chart_1.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
chart_page_chart_1.set_style_border_width(1, lv.PART.MAIN|lv.STATE.DEFAULT)
chart_page_chart_1.set_style_border_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
chart_page_chart_1.set_style_border_color(lv.color_hex(0xe8e8e8), lv.PART.MAIN|lv.STATE.DEFAULT)
chart_page_chart_1.set_style_border_side(lv.BORDER_SIDE.FULL, lv.PART.MAIN|lv.STATE.DEFAULT)
chart_page_chart_1.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
chart_page_chart_1.set_style_line_width(2, lv.PART.MAIN|lv.STATE.DEFAULT)
chart_page_chart_1.set_style_line_color(lv.color_hex(0xe8e8e8), lv.PART.MAIN|lv.STATE.DEFAULT)
chart_page_chart_1.set_style_line_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
chart_page_chart_1.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Set style for chart_page_chart_1, Part: lv.PART.TICKS, State: lv.STATE.DEFAULT.
chart_page_chart_1.set_style_text_color(lv.color_hex(0x151212), lv.PART.TICKS|lv.STATE.DEFAULT)
chart_page_chart_1.set_style_text_font(test_font("montserratMedium", 12), lv.PART.TICKS|lv.STATE.DEFAULT)
chart_page_chart_1.set_style_text_opa(255, lv.PART.TICKS|lv.STATE.DEFAULT)
chart_page_chart_1.set_style_line_width(2, lv.PART.TICKS|lv.STATE.DEFAULT)
chart_page_chart_1.set_style_line_color(lv.color_hex(0xe8e8e8), lv.PART.TICKS|lv.STATE.DEFAULT)
chart_page_chart_1.set_style_line_opa(255, lv.PART.TICKS|lv.STATE.DEFAULT)

chart_page.update_layout()

def main_page_measure_current_label_event_handler(e):
    code = e.get_code()
    if (code == lv.EVENT.CLICKED):
        pass
        main_page_measure_current_label.add_flag(lv.obj.FLAG.HIDDEN)
        
main_page_measure_current_label.add_event_cb(lambda e: main_page_measure_current_label_event_handler(e), lv.EVENT.ALL, None)

def main_page_switch2chart_event_handler(e):
    code = e.get_code()
    if (code == lv.EVENT.VALUE_CHANGED):
        pass
        lv.scr_load_anim(chart_page, lv.SCR_LOAD_ANIM.OVER_BOTTOM, 200, 200, False)
main_page_switch2chart.add_event_cb(lambda e: main_page_switch2chart_event_handler(e), lv.EVENT.ALL, None)

def main_page_btn_1_event_handler(e):
    code = e.get_code()
    if (code == lv.EVENT.CLICKED):
        pass
        lv.scr_load_anim(chart_page, lv.SCR_LOAD_ANIM.MOVE_RIGHT, 200, 200, False)
main_page_btn_1.add_event_cb(lambda e: main_page_btn_1_event_handler(e), lv.EVENT.ALL, None)

def chart_page_sw_1_event_handler(e):
    code = e.get_code()
    if (code == lv.EVENT.VALUE_CHANGED):
        pass
        lv.scr_load_anim(main_page, lv.SCR_LOAD_ANIM.MOVE_BOTTOM, 200, 200, False)
chart_page_sw_1.add_event_cb(lambda e: chart_page_sw_1_event_handler(e), lv.EVENT.ALL, None)

def chart_page_btn_1_event_handler(e):
    code = e.get_code()
    if (code == lv.EVENT.CLICKED):
        pass
        lv.scr_load_anim(main_page, lv.SCR_LOAD_ANIM.FADE_ON, 200, 200, False)
chart_page_btn_1.add_event_cb(lambda e: chart_page_btn_1_event_handler(e), lv.EVENT.ALL, None)

# content from custom.py

# Load the default screen
lv.scr_load(main_page)

while SDL.check():
    time.sleep_ms(5)

