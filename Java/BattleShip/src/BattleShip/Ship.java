
package hlab8;


public class Ship {

    private String name; // name of the piece
    private int length; // length of the piece
    // positions are the top left point of the piece
    private int rowPosition;
    private int columnPosition;

    private boolean isHorizontal; // whether it is going left-right or up-down
    private char boardCharacter; // character representing piece on board

    // useful constructor
    public Ship(String s, int i, char c)
    {
        name = s;
        length = i;
        boardCharacter = c;
    }

    // fill in the information of the location of the piece when placed
    public void setPiece(int row, int column, char direction)
    {
        if(direction == 'u')
        {
            rowPosition = row-(length-1);
            columnPosition = column;
            isHorizontal = false;
        }
        else if(direction == 'd')
        {
            rowPosition = row;
            columnPosition = column;
            isHorizontal = false;
        }
        else if(direction == 'l')
        {
            rowPosition = row;
            columnPosition = column - (length-1);
            isHorizontal = true;
        }
        else if(direction == 'r')
        {
            rowPosition = row;
            columnPosition = column;
            isHorizontal = true;
        }
        else
        {
            rowPosition = -1;
            columnPosition = -1;
        }
    }

    // "get" method
    public boolean isHorizontal()
    {
        return isHorizontal;
    }

    // another "get" method...
    public int getTopLeftRow()
    {
        return rowPosition;
    }

    // yeah...
    public int getTopLeftColumn()
    {
        return columnPosition;
    }

    // ...
    public int getLength()
    {
        return length;
    }

    public String getName()
    {
        return name;
    }

    public char getBoardCharacter()
    {
        return boardCharacter;
    }
}
