package hlab4;

import java.util.ArrayList;

public class Map {

    // size of the world
    private static final int MAX_ROWS = 20;
    private static final int MAX_COLUMNS = 40;

    // characters for blank space and you, the player
    private static final char BLANK = ' ';
    private static final char PLAYER = 'Y';

    // stores all the characters on the world
    private char[][] grid = new char[MAX_ROWS][MAX_COLUMNS];

    // list of the possible locations to go to
    private ArrayList<Room> mapRooms = new ArrayList<Room>();

    // current place you are
    private Room currentLocation = null;

    public Map()
    {
        // initialize the map to be blank
        for(int i=0; i < grid.length; i++)
        {
            for(int j=0; j < grid[i].length; j++)
            {
                grid[i][j] = BLANK;
            }
        }

        // make 3 rooms
        Room outside = new Room("Outside", 'O', 0,0, 39,19);
        Room university = new Room("University", 'U', 5,5, 15,15);
        Room dorm = new Room("Dorm", 'D', 25,5, 30,10);
        Room toliet = new Room("Toliet",'T', 10,10, 12,12);

        // add them to our arrayList
        mapRooms.add(outside);
        mapRooms.add(university);
        mapRooms.add(dorm);
        mapRooms.add(toliet);

        // start out in your room
        currentLocation = dorm;
        moveToRoom(currentLocation);

        // generate the map (rooms)
        generateMap();
    }

    private void generateMap()
    {
        // for all the rooms we have in our list...
        for(Room place : mapRooms)
        {
            // Fill in top and bottom of rectangle with characters
            for(int i = place.getTopLeftX(); i <= place.getBottomRightX(); i++)
            {
                grid[place.getTopLeftY()][i] = place.getLetter();
                grid[place.getBottomRightY()][i] = place.getLetter();
            }
            // fill in left and right side...
            for(int i = place.getTopLeftY(); i <= place.getBottomRightY(); i++)
            {
                grid[i][place.getTopLeftX()] = place.getLetter();
                grid[i][place.getBottomRightX()] = place.getLetter();
            }
        }
    }

    // this simply shows the map on the screen...
    public void displayMap()
    {
        for(int i=0; i < grid.length; i++)
        {
            for(int j=0; j < grid[i].length; j++)
            {
                System.out.printf("%2s", grid[i][j]);
            }
            System.out.println();
        }
    }

    // get where we are
    public Room getPlayerLocation()
    {
        return currentLocation;
    }

    // get the possible locations to go to
    public ArrayList<Room> getLocations()
    {
        return mapRooms;
    }

    // update the map so the player symbol moves to the a new location
    public void moveToRoom(Room destination)
    {
        // if were somewhere before
        if(currentLocation != null)
        {
            // remove the mark from the center of the previous location
            grid[currentLocation.getTopLeftY() + (currentLocation.getBottomRightY() - currentLocation.getTopLeftY())/2]
                    [currentLocation.getTopLeftX() +(currentLocation.getBottomRightX() - currentLocation.getTopLeftX())/2]
                    = BLANK;
        }

        // update where we are
        currentLocation = destination;

        // add player mark to center of new location
        grid[currentLocation.getTopLeftY() + (currentLocation.getBottomRightY() - currentLocation.getTopLeftY())/2]
                [currentLocation.getTopLeftX() + (currentLocation.getBottomRightX() - currentLocation.getTopLeftX())/2]
                = PLAYER;
    }


}
