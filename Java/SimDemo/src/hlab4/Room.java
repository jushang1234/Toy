package hlab4;

import java.util.*;
import java.io.*;
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

public class Room {

    // letters outline where the location is
    private char mapLetter;

    // locations are rectangles....
    private int topLeftX;
    private int topLeftY;
    private int bottomRightX;
    private int bottomRightY;

    // all good things have a name, right?
    private String roomName;

    // constructor to make a room!  need all the above information
    public Room(String name, char symbol, int tlx, int tly, int brx, int bry)
    {
        roomName=name;
        mapLetter=symbol;
        topLeftX=tlx;
        topLeftY=tly;
        bottomRightX=brx;
        bottomRightY=bry;
    }

    // Below: get methods to get all the information about a room

    public String getName()
    {
        return roomName;
    }

    public char getLetter()
    {
        return mapLetter;
    }

    public int getTopLeftX()
    {
        return topLeftX;
    }

    public int getTopLeftY()
    {
        return topLeftY;
    }

    public int getBottomRightX()
    {
        return bottomRightX;
    }

    public int getBottomRightY()
    {
        return bottomRightY;
    }

}
