Nishith Agarwal
ID: 1624-574-357
CSCI 480 HW1


Description:

The program uses OpenGL to extract height fields from a 2D image (using the PICK_PIXEL module) and renders the height field along the Z-axis to create a three-dimensional image. 

The program creates a three-dimensional image using three methods:

1. Points: The program creates two points for each pixel. One point lies in the x-y plane with z=0. The other point lies in x-y-z with the z-co-ordinate equal to the height field extracted. It is possible to switch to the points mode by pressing "p" key.

2. Lines: The program uses the two points created in points method earlier, and renders a line to connect them. It is possible to switch to the lines mode by pressing "l" key.

3. Triangles: The program connects three pixels using triangles. The z-coordinate for each point corresponds to the height field extracted from that pixel. It is possible to switch to the lines mode by pressing "t" key.

ROTATIONS: The program also supports translations to rotate the image around the X-axis or the Z-axis using the mouse drag operation.

TRANSLATIONS: The program also supports translations to move the image on the screen using the CTRL key and mouse drag operation.

SCALE: The program also supports scaling to zoom in or zoom out of the image using the SHIFT key and mouse drag operation.

Execute Instructions:
> tar -xvf nishith_agarwal_csci480_hw1.zip
> cd pic
> make
> cd ..
> cd assign1
> make
> ./assign1 SantaMonicaMountains-512.jpg
> ./assign1 color.jpg

Please find the files 000.jpg - 299.jpg under the output_files folder

Extra Features:

The program supports images with color (bpp=3). The RGB values are extracted for each pixel and used to display the three-dimensional output image in color. 

Animation:

The animation is 20 seconds long (300 frames at 15fps). The animation has been divided into two parts.

Part1: Displays an image with bpp=1. The animation shows the image rotation and translation. It then switches to point mode and shows the zoom in and zoom out operations. Lastly, it switches to line mode.

Part2: Shows the extra credit operation to display an image with bpp=3 in full color.

