AT7_IMG = "at7.png"
AT7_OUTIMG = "at7uui/at7_bitimg.h"
AT7_OUT_GLYPH_METRICS = "at7uui/at7_glyph_metrics.h"

AT7_SRC_WIDTH = 596
AT7_SRC_HEIGHT = 9

AT7_WIDTH = 596 +   127-1#padding - 1px between each char
AT7_HEIGHT = 9



from PIL import Image
import math
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

glyph_mf.write("\n\n#include <uui/uui_ui.h>\n\n")

LEFT_BAR_OFFSET = 2

bitimg_width = 0
bitimg_height = 0
width, height = img.size
width -= LEFT_BAR_OFFSET
gcode = 0

o_y = 0
gcode = 0

cur_width = 0
cur_byte = 0
cur_bit = 0
y_hi = -500
y_lo = 500

glyph_bytes = bytearray()

to_repeat = 0


# font metrics
glyph_mf.write("\nconst u_font_metrics at7_font_metrics = {")
glyph_mf.write("\t.baseline = 7")
glyph_mf.write("};\n")


glyph_src_bounds = []

# glyph metrics / gen atlas
for i in range(1, width*4):
    x=i%width

    has_col = 0

    for y in range(0,9): # 9 because some go 2px lower than the baseline
        r,g,b,a = img.getpixel((x+LEFT_BAR_OFFSET,o_y+y+1))
        if (r>0.5 and g>0.5 and b>0.5):
            has_col = 1

            # cur_byte = cur_byte << 1
            # cur_byte |= 1
            # cur_bit+=1
            if (y>y_hi):
                y_hi=y
            if (y<y_lo):
                y_lo=y
        #else:
            #cur_byte << 1
            #cur_bit+=1
            
        #if (cur_bit==8):
            #glyph_bytes+=bytearray([cur_byte])
            #glyph_bytes+=bytearray([0])
            # cur_byte = cur_byte >> 8
            # cur_bit = 1

    if (has_col):
        cur_width+=1
    else:
        if (cur_width >0 and to_repeat>0):
            to_repeat-=1

    if (gcode == 128):
        break

    if (cur_width > 0 and has_col == 0 and to_repeat == 0):
        # expand image
        #out_img+=glyph_bytes
        # add spacing


        #glyph_bytes = bytearray()

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

     
        if (gcode == 127):
            glyph_mf.write(f"\n/* -- 'del' - [{gcode}] -- */")
        elif (gcode >= 32 and gcode < 128):
            glyph_mf.write(f"\n/* -- '{chr(gcode)}' - [{gcode}] -- */")

        if (gcode == 34): #"
            cur_width=3


        glyph_mf.write(f"\nu_glyph_metrics at7_gm{gcode} = ")
        glyph_mf.write("{\n")

        height = y_hi - y_lo + 1

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


        glyph_src_bounds.append([LEFT_BAR_OFFSET+x-cur_width-1, 1+o_y+y_lo, LEFT_BAR_OFFSET+x-1, 1+o_y+y_hi,    bitimg_width, y_lo, bitimg_width+cur_width, y_hi])


    
        # write UVs
        u0 = bitimg_width/AT7_WIDTH
        v0 = 0
        v1 = (bitimg_width+cur_width)/AT7_WIDTH
        v2 = height/AT7_HEIGHT

        glyph_mf.write(f"/*u0,v0,u1,v1*/\nfloat at7_uv{gcode}[] = {{")
        glyph_mf.write(f"{u0},{v0},{v1},{v2}")
        glyph_mf.write("};\n\n")



        bitimg_width+=cur_width

        y_hi = -500
        y_lo = 500

        if (gcode == 126):
            break

        cur_width=0
        if (gcode == 32):
            gcode = 127 
        elif (gcode==127):
            gcode = 33
        else:
            gcode += 1

   
        if (gcode == 34): #"
            to_repeat=2


    if (x==width-1):
        #out_img+=glyph_bytes
        #glyph_bytes = bytearray()
        o_y+=9


out_img = bytearray([0])*AT7_WIDTH*AT7_HEIGHT


gcode == 0
o_y = 0

x_dst_accum = 0

for i in range(0, 127):
    w = glyph_src_bounds[i][2]-glyph_src_bounds[i][0]+1
    h = glyph_src_bounds[i][3]-glyph_src_bounds[i][1]+1
    
    for y in range(h):
        for x in range(w):
            dstx = glyph_src_bounds[i][4]+x+x_dst_accum
            dsty = glyph_src_bounds[i][5]+y


            tgdi = math.floor((dstx+dsty*AT7_WIDTH)/8)
            tb   = (dstx+dsty*AT7_WIDTH)%8

            sx = glyph_src_bounds[i][0]+x
            sy = glyph_src_bounds[i][1]+y

            r,g,b,a = img.getpixel((sx,sy))
            if (r>0.5 and g>0.5 and b>0.5):
                byte = out_img[tgdi]
                n =  tb              
                byte ^= (1 << n)
                out_img[tgdi] = byte
    x_dst_accum+=1

'''
# generate the bit image
for i in range(1, width*4):
    x=i%width

    has_col = 0
    for y in range(0,9): # 9 because some go 2px lower than the baseline
        r,g,b,a = img.getpixel((x+LEFT_BAR_OFFSET,o_y+y+1))
        if (r>0.5 and g>0.5 and b>0.5):
            has_col = 1

            cur_byte = cur_byte << 1
            cur_byte |= 1
            cur_bit += 1
        else:
            cur_byte << 1
            cur_bit += 1

        if (cur_bit==8):
            glyph_bytes+=bytearray([cur_byte])
            cur_byte = cur_byte >> 8
            cur_bit = 0





    if (has_col):
        cur_width+=1
    else:
        if (cur_width >0 and to_repeat>0):
            to_repeat-=1

    if (gcode == 128):
        break

    if (cur_width > 0 and has_col == 0 and to_repeat == 0):
        # add spacing
        cur_bit = (cur_bit + 9) % 8
        if (cur_bit!=0):
            glyph_bytes+=bytearray([0])

        out_img+=glyph_bytes
        glyph_bytes = bytearray()

    

        



        if (gcode == 126):
            cur_width=0
        if (gcode == 32):
            gcode = 127 
        elif (gcode==127):
            gcode = 33
        else:
            gcode += 1


        if (gcode == 34): #"
            to_repeat=2
'''






# glyph table
glyph_mf.write("/*GLYPH METRIC LOOKUP TABLE*/")


glyph_mf.write("\nconst u_glyph_metrics *at7_glyph_metrics[] = {")
for i in range(0,128):
    glyph_mf.write(f"&at7_gm{i}")
    if (i!=127):
        glyph_mf.write(",")
glyph_mf.write("};\n")


glyph_mf.write("\nconst float *at7_uvs[] = {")
for i in range(0,128):
    glyph_mf.write(f"at7_uv{i}")
    if (i!=127):
        glyph_mf.write(",")
glyph_mf.write("};")

glyph_mf.close()

with open(AT7_OUTIMG, "wb") as f:

    f.write(f"#define AT7_WIDTH  {AT7_WIDTH}\n".encode())
    f.write(f"#define AT7_HEIGHT {AT7_HEIGHT}\n".encode())

    
    f.write(f"#define AT7_SRC_WIDTH  {bitimg_width}\n".encode())
    f.write(f"#define AT7_SRC_HEIGHT {AT7_HEIGHT}\n".encode())

    f.write("const unsigned char at7_bitimg[] = {".encode())
    for b in out_img:
        f.write(f"{b},".encode())
    
    f.seek(-1, 1)
    f.write("};".encode())
    f.flush()