# hex-to-image
A simple program to convert a bunch of hex digits to a png file.

# Usage
By default hex-to-image looks for a file called `hex-input.txt` in the directory you are executing it in as the intput file.<br/>
hex-to-image uses the size of the input file to define the size of the output image, unless specified with the commandline options for width and height.<br/>
The default output is the file `hex-to-image.png` in the execution directory.

## Commandline options
These are all the starting arguments you can give hex-to-image to configure stuff.
|argument   |short  |default            |explanation|
|-----------|-------|-------------------|-----------|
|--help     |       |                   |Prints this help text and then quits.|
|--info     |       |                   |Prints some info about this program and then quits.|
|--input    |-i     |hex-input.txt      |Sets the file with the hex values to convert to an image.|
|--width    |-w     |0                  |Sets the width of the output image. Set to 0 to use the length of the lines in the input file.|
|--height   |-h     |0                  |Sets the height of the output image. Set to 0 to use the line count from the input file.|
|--output   |-o     |hex-to-image.png   |Sets the location of the output image.|
|--override |       |                   |Makes this program override the output file. If not specified it will append a number instead if the output already exists.|
|--digits   |-d     |6                  |Sets the number of hex digits to use for each pixel. Valid values are 3, 4, 6, and 8. If the values is 4 or 8 the output image will include transparency.|

## Examples
To convert the hex digits from `in.txt` to an image with a width of 12 pixels use this:<br/>
`./hex-to-image -i in.txt -w 12`<br/>
This will automatically calculate the height from the size of the input file.

`./hex-to-image -o out.png -h 15`<br/>
This will convert the hex digits from `hex-input.txt` to an image with a height of 15 pixels and a width calculated to fit the full input file to the file `out.png`.<br/>
Should the output file already exist this will instead write to `out-1.png`.

` ./hex-to-image -i in/input.txt -w 25 -h 25 -o output.png --override -d 8`<br/>
This converts the hexadecimal digits from the file `input.txt` in the directory `in` to a png with transparency named `output.png`.<br/>
Should the output file already exist this will override it.
