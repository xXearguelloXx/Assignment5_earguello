#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Pixel.h"

//function average_colors
//adds up every r, every g, and every b value separately, then divides each total by the number of pixels
//vector is passed by reference & so it dont copy thousands of pixels

void average_colors(std::vector<Pixel> &pixel_list)
{
  if (pixel_list.empty())
  {
    std::cout << "no pixels to average" << std::endl;
    return;
  }

  //keep everything in float
  double r_total = 0.0;
  double g_total = 0.0;
  double b_total = 0.0;
	
  //create totals for every color
  for (size_t i = 0; i < pixel_list.size(); i++)
  {
    r_total += pixel_list[i].r;
    g_total += pixel_list[i].g;
    b_total += pixel_list[i].b;
  }
  //printing and average calculation
  double count = pixel_list.size();
  std::cout << "average r = " << r_total / count << std::endl;
  std::cout << "average g = " << g_total / count << std::endl;
  std::cout << "average b = " << b_total / count << std::endl;
}

//fucntion flip_vertically
//currently, the file is ordered x first, then y:
//(0,0) (0,1)...(0,255) then (1,0) (1,1)...(1,255) then (2,0)...
//every column x is height pixels in a row
//pixel at (x, y) sits at index (x * height + y) in the vector

//flipping vertically means the color at y moves to (height - 1 - y).
//for each column we swap the colors of the top and bottom pixels.
//Then step inward, stopping at the middle (like reversing an array)
//Only r, g, b move. so x and y stay put so flipped.dat keeps the same
//(0,0) (0,1) (0,2) ... order as pixels.dat, just with new colors.


void flip_vertically(std::vector<Pixel> &pixel_list)
{
  if (pixel_list.empty())
    return;

  //find the image size from the data
  int max_x = 0;
  int max_y = 0;
  for (size_t i = 0; i < pixel_list.size(); i++)
  {
    if (pixel_list[i].x > max_x) max_x = pixel_list[i].x;
    if (pixel_list[i].y > max_y) max_y = pixel_list[i].y;
  }
  int width = max_x + 1;   //512
  int height = max_y + 1;  //256

  for (int x = 0; x < width; x++)
  {
    for (int y = 0; y < height / 2; y++)   // only the top half; each swap handles two pixels
    {
      Pixel &top = pixel_list[x * height + y];
      Pixel &bottom = pixel_list[x * height + (height - 1 - y)];

      // swap the colors, leave the coordinates alone
      float temp;
      temp = top.r; top.r = bottom.r; bottom.r = temp;
      temp = top.g; top.g = bottom.g; bottom.g = temp;
      temp = top.b; top.b = bottom.b; bottom.b = temp;
    }
  }
}

int main(int argc, char *argv[])
{
  //file name comes from the command line, e.g. ./main pixels.dat
  //argv[0] is the program name, argv[1] is the first argument
  if (argc < 2)
  {
    std::cout << "usage: " << argv[0] << " pixels.dat" << std::endl;
    return 1;
  }

  std::ifstream infile(argv[1]);
  if (!infile.is_open())
  {
    std::cout << "could not open " << argv[1] << std::endl;
    return 1;
  }

  //the vector that holds every pixel
  std::vector<Pixel> pixel_list;

  //read one line at a time and split it on commas.
  // start = where the current value begins
  // comma = where the next comma is (found with .find(',', start))
  // .substr(start, comma - start) cuts out the text between them
  // then start jumps to comma + 1, just past that comma, for the next value.
  // The last value (b) has no comma after it, so find() returns
  // std::string::npos; substr(start) with no length takes the rest of the line.

  std::string line;
  while (std::getline(infile, line))
  {
    if (line.empty())
      continue;   //skip a blank line at the end of the file

    std::string values[5];   //x, y, r, g, b as text
    size_t start = 0;
    for (int i = 0; i < 5; i++)
    {
      size_t comma = line.find(',', start);
      if (comma == std::string::npos)
      {
        values[i] = line.substr(start);           //last value: rest of the line
      }
      else
      {
        values[i] = line.substr(start, comma - start);
        start = comma + 1;                         //step past the comma
      }
    }

    //convert text to numbers: stoi = string to int, stof = string to float
    Pixel p;
    p.x = std::stoi(values[0]);
    p.y = std::stoi(values[1]);
    p.r = std::stof(values[2]);
    p.g = std::stof(values[3]);
    p.b = std::stof(values[4]);

    pixel_list.push_back(p);
  }
  infile.close();

  std::cout << "read " << pixel_list.size() << " pixels" << std::endl;

  //functions
  average_colors(pixel_list);
  flip_vertically(pixel_list);

  //write the flipped pixels in the same x,y,r,g,b format
  std::ofstream outfile("flipped.dat");
  if (!outfile.is_open())
  {
    std::cout << "could not open flipped.dat for writing" << std::endl;
    return 1;
  }

  for (size_t i = 0; i < pixel_list.size(); i++)
  {
    outfile << pixel_list[i].x << ","
            << pixel_list[i].y << ","
            << pixel_list[i].r << ","
            << pixel_list[i].g << ","
            << pixel_list[i].b << std::endl;
  }
  outfile.close();

  std::cout << "wrote flipped.dat" << std::endl;
  return 0;
}
