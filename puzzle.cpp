#include <FL/Fl.H>        // needed in every fltk program
#include <FL/Fl_Window.H> // needed to use the Fl_Window class
#include <FL/Fl_Box.H>
#include <FL/Fl_JPEG_Image.H>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string>
using namespace std;

char *int2charstar(int v)
{
    char *s = new char[sizeof(int)];
    sprintf(s, "%d", v);
    return s;
};

Fl_JPEG_Image *blank = new Fl_JPEG_Image("row-1-col-4.jpg");

class MyButton : public Fl_Box
{

public:
    int neighbourcount;
    MyButton *neighbours[4];
    MyButton(int x, int y, int w, int h, const char *l);
    int handle(int e); // e is incoming mouse event of different kinds
};

MyButton ::MyButton(int x, int y, int w, int h, const char *l = 0) : Fl_Box(x, y, w, h, l)
{
}

int MyButton ::handle(int e)
{
    if (e == FL_PUSH)
    {
        if (image() != blank)
        {
            for (int i = 0; i < neighbourcount + 1; i++)
            {
                if (neighbours[i]->image() == blank)
                {
                    neighbours[i]->image(image());
                    neighbours[i]->redraw();
                    neighbours[i]->color(FL_GREEN);
                    image(blank);
                    redraw();
                    color(FL_RED);
                }
            }
        }
    }

    return 1;
};

void allocateNeighbours(MyButton *buttons[4][4])
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            int k = -1;
            if (j != 0)
            {
                k++;
                buttons[i][j]->neighbours[k] = buttons[i][j - 1];
            }
            if (j != 3)
            {
                k++;
                buttons[i][j]->neighbours[k] = buttons[i][j + 1];
            }
            if (i != 0)
            {
                k++;
                buttons[i][j]->neighbours[k] = buttons[i - 1][j];
            }
            if (i != 3)
            {
                k++;
                buttons[i][j]->neighbours[k] = buttons[i + 1][j];
            }
            for (int l = k + 1; l < 4; l++)
            {
                buttons[i][j]->neighbours[l] = NULL;
            }
            buttons[i][j]->neighbourcount = k;
        }
    }
    return;
}

void randomize(MyButton *buttons[4][4])
{
    srand(time(0));
    for (int i = 0; i < 500; i++)
    {
        int a = (rand() % 4);
        int b = (rand() % 4);
        buttons[a][b]->handle(1);
    }
    return;
}

int main(int argc, char *argv[])
{
    ifstream infile("image.txt");
    Fl_Window *window;

    window = new Fl_Window(600, 600, "ALLU ARJUN"); // outer window
    window->color(FL_BLACK);

    MyButton *buttons[4][4];
    Fl_JPEG_Image *images[4][4];
    string names[4][4];
    const char *filename[4][4];

    for (int j = 0; j < 4 && infile.good(); j++)
    {
        for (int i = 0; i < 4 && infile.good(); i++)
        {
            getline(infile, names[i][j]);
            filename[i][j] = names[i][j].c_str();
            images[i][j] = new Fl_JPEG_Image(filename[i][j]);
        }
    }

    for (int j = 0; j < 4; j++)
    {
        for (int i = 0; i < 4; i++)
        {
            if (i != 3 || j != 0)
            {
                buttons[i][j] = new MyButton(75 * (2 * i), 75 * (2 * j), images[i][j]->w(), images[i][j]->h());
                buttons[i][j]->image(images[i][j]);
                buttons[i][j]->color(FL_BLACK);
            }
            else
            {
                buttons[i][j] = new MyButton(75 * (2 * i), 75 * (2 * j), images[i][j]->w(), images[i][j]->h());
                buttons[i][j]->image(blank);
                buttons[i][j]->color(FL_BLACK);
            }
        }
    }

    allocateNeighbours(buttons);
    randomize(buttons);

    window->end();
    window->show();
    int s = (Fl::run()); // the process waits from here on for events
    return s;
}