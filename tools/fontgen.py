from PIL import Image, ImageDraw, ImageFont
from fontTools.ttLib import TTFont
from pathlib import Path

output_dir = Path("output")
output_dir.mkdir(exist_ok=True)

WIDTH = 5
HEIGHT = 7
SCALE = 5

# Get the font file for drawing
font = ImageFont.truetype("fonts/ti-83-font.otf", HEIGHT * SCALE)

# create an image and draw object for drawing
img = Image.new("1", (WIDTH * SCALE, HEIGHT * SCALE), 0)
draw = ImageDraw.Draw(img)

# Get font in TTFont for looping through
ttfont = TTFont("fonts/ti-83-font.otf")
cmap = ttfont.getBestCmap()

firstChar = min(cmap.keys())
lastChar = max(cmap.keys())

# Write the output to a file
with open(f"fonts/TI83Font{WIDTH}x{HEIGHT}.cpp", "w") as f:
    f.write('#include "fonts.h"\n\n')
    f.write(f"const uint8_t Font{WIDTH}x{HEIGHT}Bitmap[] = {{\n")

    # For each character in the font
    for codepoint in sorted(cmap.keys()):
        draw.rectangle((0,0,WIDTH * SCALE,HEIGHT * SCALE), fill=0)
        f.write(f"\t// {repr(chr(codepoint))}\n")

        draw.text((0, 0), chr(codepoint), fill=1, font=font)
        # Log the rasterization as 1s and 0s
        for y in range(HEIGHT):
            f.write("\t0b")
            for x in range(WIDTH):
                f.write(str(img.getpixel((x * SCALE, y * SCALE))))
            f.write(",\n")
        f.write("\n")
        # try: # Save an image for each font char for debugging
        #     img.save(output_dir / f"{codepoint}_{chr(codepoint)}.png")
        # except:
        #     pass


    # Finish off file
    f.write("};\n\n")
    f.write(f"const Font TI83Font{WIDTH}x{HEIGHT} {{\n\t{WIDTH},\n\t{HEIGHT},\n\t{firstChar}, {min(lastChar, 126)},\n\tFont{WIDTH}x{HEIGHT}Bitmap\n}};")
    f.close()