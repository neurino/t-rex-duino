# Python >= 3.7.1

# This script converts bitmap image *.png into two c-array: one
# containing bitmap data (*_bitmap) and other containing transparency data (*_mask).
# First two bytes in arrays are width and height in pixels.
# The rest is bitmap data. Data is stored in row-major order.
# Each row element (byte) represents a *column* of 8 pixels.

import argparse
from pathlib import Path
from typing import Callable, List, Tuple

from PIL import Image

Pixel = Tuple[int, ...]
Pixels = List[Pixel]
PixelTestFunc = Callable[[Pixel], bool]

def to_bitmap_array(pixels: Pixels, width: int, is_pixel_set: PixelTestFunc) -> List[int]:
    buffer: List[int] = []
    height = len(pixels) // width
    byte_height = (height + 7) // 8

    for byte_y in range(byte_height):
        for x in range(width):
            byte = 0
            for bit_y in range(8):
                index = (byte_y * 8 + bit_y) * width + x
                if index < len(pixels) and is_pixel_set(pixels[index]):
                    byte |= 1 << bit_y
            buffer.append(byte)

    return buffer

def to_sprite_format(width: int, height: int, bitmap: List[int]) -> List[int]:
    if width > 255:
        raise ValueError(f"width is too big: {width}")
    if height > 255:
        raise ValueError(f"height is too big: {height}")
    return [width, height] + bitmap

def to_hex_str(data: List[int]) -> str:
    buffer = ""
    for index, value in enumerate(data):
        buffer += f"0x{value:02x}"
        if index != len(data) - 1:
            buffer += ", "
            if index != 0 and (index + 1) % 16 == 0:
                buffer += "\n"
    return buffer

def to_bitmap_str(array_name: str, pixels: Pixels, width: int, is_pixel_set: PixelTestFunc) -> str:
    height = len(pixels) // width
    bitmap = to_bitmap_array(pixels, width, is_pixel_set)
    sprite = to_sprite_format(width, height, bitmap)
    return f"static const uint8_t {array_name}[] PROGMEM = {{\n{to_hex_str(sprite)}\n}};\n"

def is_not_white(pixel: Pixel) -> bool:
    is_white = pixel[0] > 127 and pixel[1] > 127 and pixel[2] > 127
    is_transparent = len(pixel) >= 4 and pixel[3] == 0
    return not (is_white or is_transparent)

def is_not_transparent(pixel: Pixel) -> bool:
    return len(pixel) < 4 or pixel[3] != 0

def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser()
    parser.add_argument("imgfile")
    parser.add_argument("outdir")
    return parser.parse_args()

def main() -> None:
    args = parse_args()
    image_path = Path(args.imgfile)
    output_dir = Path(args.outdir)

    with Image.open(image_path) as image:
        width, _ = image.size
        pixels = list(image.getdata())

    array_name = image_path.stem.replace("-", "_")
    array_data_name = f"{array_name}_bitmap"
    array_mask_name = f"{array_name}_mask"
    output_path = output_dir / f"{image_path.stem}.h"

    output_path.write_text(
        to_bitmap_str(array_data_name, pixels, width, is_not_white)
        + "\n"
        + to_bitmap_str(array_mask_name, pixels, width, is_not_transparent),
        encoding="ascii",
    )

if __name__ == "__main__":
    main()
