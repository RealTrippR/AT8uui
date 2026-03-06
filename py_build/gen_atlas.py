AT7_IMG = "at7.png"
AT7_OUTIMG = "at7uui/at7_bitimg.h"
AT7_OUT_GLYPH_METRICS = "at7uui/glyph_metrics.h"


from PIL import Image
img = Image.open(AT7_IMG)

out_img = bytearray()


glyph_mf = open(AT7_OUT_GLYPH_METRICS, "w")
glyph_mf.write("/***********************************************************************\n")
glyph_mf.write("* -- -- -- -- -- -- AUTO GENERATED  BY  GEN_ATLAS.PY -- -- -- -- -- -- *\n")
glyph_mf.write("*           A COMPONENT OF THE UNIVERSAL USER INTERFACE SYSTEM         *\n")
glyph_mf.write("*                                                                      *\n")
glyph_mf.write("*            COPYRIGHT (C) TRIPP R. 2026, ALL RIGHTS RESERVED.         *\n")
glyph_mf.write("* -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- *\n")
glyph_mf.write("***********************************************************************/\n")

glyph_mf.write("\n\n#include <uui/uui_ui.h>")

LEFT_BAR_OFFSET = 2

bitimg_width = 0
bitimg_height = 0
width, height = img.size
width -= LEFT_BAR_OFFSET
gcode = 0

#o_y = 0
o_y = 18
gcode = 54

cur_width = 0
cur_byte = 0
cur_bit = 0
y_hi = -500
y_lo = 500
glyph_bytes = bytearray()

for i in range(1, width*4):
    x=i%width

    has_col = 0

    for y in range(0,8): # 8 because some go lower than they probably should
        r,g,b,a = img.getpixel((x+LEFT_BAR_OFFSET,o_y+y+1))
        if (r>0.5 and g>0.5 and b>0.5):
            has_col = 1

            cur_byte = cur_byte << 1
            cur_byte |= 1
            cur_bit+=1
            if (y>y_hi):
                y_hi=y
            if (y<y_lo):
                y_lo=y
        else:
            cur_byte << 1
            cur_bit+=1
        if (cur_bit==8):
            glyph_bytes+=bytearray([cur_byte])
            cur_byte = cur_byte >> 8
            cur_bit = 0
    if (has_col):
        cur_width+=1
    
    
    if (cur_width > 0 and has_col == 0):
        # expand image
        out_img+=glyph_bytes
        glyph_bytes = bytearray()
        bitimg_width+=cur_width

        #add font metrics

        # typedef struct u_glyph_metrics {
        #     uint8_t ascender;
        #     uint8_t descender;
        #     float bearing;
        #     float advance_vert;
        #     float advance_horz;
        #     uint8_t  height;
        #     uint8_t  width;
        # } u_glyph_metrics;

        if (gcode >= 32 and gcode < 128):
            glyph_mf.write(f"\n/* -- '{chr(gcode)}' - [{gcode}] -- */")
            
        glyph_mf.write(f"\nu_glyph_metrics at7_gm{gcode} = ")
        glyph_mf.write("{\n")

        height = y_hi - y_lo

        glyph_mf.write(f"\t.bearing = 2,\n")
        if (y_hi<0 or y_lo > 100):
            glyph_mf.write(f"\t.advance_vert = {0},\n")
            glyph_mf.write(f"\t.ascender = {0},\n")
            glyph_mf.write(f"\t.descender = {0},\n")
            height=0
        else:
            glyph_mf.write(f"\t.advance_vert = {y_hi-y_lo + 1},\n")
            glyph_mf.write(f"\t.ascender = {y_lo},\n")
            glyph_mf.write(f"\t.descender = {7-(y_hi+1)},\n")

        glyph_mf.write(f"\t.advance_horz = {cur_width + 1},\n")
        glyph_mf.write(f"\t.height = {height},\n")
        glyph_mf.write(f"\t.width = {cur_width}\n")

        glyph_mf.write("};\n")


        cur_width=0
        gcode+=1


    if (x==width-1):
        out_img+=glyph_bytes
        glyph_bytes = bytearray()
        o_y+=9
        break


glyph_mf.close()

with open(AT7_OUTIMG, "wb") as f:

    f.write(f"#define AT7_WIDTH  {bitimg_width}\n".encode())
    f.write(f"#define AT7_HEIGHT {7}\n".encode())
    f.write("const unsigned char at7_bitimg[] = {".encode())
    for b in out_img:
        f.write(f"{b},".encode())
    f.seek(-1, 1)
    f.write("};".encode())
    f.flush()