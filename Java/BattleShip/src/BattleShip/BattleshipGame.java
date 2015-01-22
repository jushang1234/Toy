
package hlab8;

import java.util.*;

public class BattleshipGame {
    
    private final int SIZE = 9; // size of the board
    private final char BLANK = '~'; // DO NOT SET THIS TO A SHIP CHARACTER
    private final char UNKNOWN = '?'; // where have you not shot yet?
    private final char HIT = 'X'; // hit indicator
    private final char MISS = 'O'; // miss indicator
    
    private boolean AI = false;
    private char[][] playerBoard; // your piece and computer's hits/misses
    private char[][] computerBoard; // computer's pieces and your hit/misses
    private Ship[] playerShips; // your ships
    private Ship[] computerShips; // computer's ships

    private BattleshipGame()
    {
        // First initialize both player's and computer's boards to blank
        playerBoard = new char[SIZE][SIZE];
        for(int i =0; i < playerBoard.length; i++)
        {
            for(int j=0; j < playerBoard[i].length; j++)
            {
                playerBoard[i][j] = BLANK;
            }
        }

        computerBoard = new char[SIZE][SIZE];
        for(int i =0; i < computerBoard.length; i++)
        {
            for(int j=0; j < computerBoard[i].length; j++)
            {
                computerBoard[i][j] = UNKNOWN;
            }
        }

        // use one ship of each type
        playerShips = new Ship[5];
        playerShips[4] = new Destroyer();
        playerShips[3] = new Cruiser();
        playerShips[2] = new Submarine();
        playerShips[1] = new BattleShip();
        playerShips[0] = new AircraftCarrier();

        // computer too!
        computerShips = new Ship[5];
        computerShips[4] = new Destroyer();
        computerShips[3] = new Cruiser();
        computerShips[2] = new Submarine();
        computerShips[1] = new BattleShip();
        computerShips[0] = new AircraftCarrier();
    }

    public static void main(String[] args)
    {
        BattleshipGame game = new BattleshipGame(); // all right, lets make one of these classes

        System.out.println("Welcome to Battleship v23.1");
        game.placePlayerPieces(); // ask the player to place all their pieces
        game.placeComputerPieces(); // computer automatically places...

        System.out.println("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"); // cheat used to bump old text off screen
        game.displayBoards(); // show the starting state of both boards

        // keep alternating turns until someone wins....
        do{
            String playerMove = game.playerMove(); // you go
            String computerMove = game.computerMove(); // computer goes
            System.out.println("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
            game.displayBoards(playerMove, computerMove); // show results (after bumping old text off)
        }while(game.gameOver() == 0);

        // game over conditions, 1 = you lose, 2 = you win, 3 = tie
        int gameOver = game.gameOver();
        if(gameOver == 1)
        {
            System.out.println("You lost!  Practice more.");
        }
        else if(gameOver == 2)
        {
            System.out.println("You won!  Have a cookie.");
        }
        else if(gameOver == 3)
        {
            System.out.println("Oh my gosh, a tie!");
        }
    }

    // This method goes through and makes you place all your pieces
    private void placePlayerPieces()
    {
        // loop over every piece
        for(int i=0; i<playerShips.length; i++)
        {
            boolean correctPlacement;
            // loop until you give a correct placement (i.e. no conflicting ships or off the board)
            do{
                displayBoards(); // show the board
                correctPlacement = placePiece(playerShips[i]); // see if the placement is valid
                if(!correctPlacement) // if not valid, tell them they are terrible
                {
                    System.out.println("\nPlease enter a valid position and direction");
                }
            }while(!correctPlacement);

            // valid placement, so place this piece
            addPieceToBoard(playerBoard, playerShips[i]);
        }
    }


    // computer is more automated
    private void placeComputerPieces()
    {
        // we need to make a tempBoard, since we do not want to modify computerBoard
        // (otherwise you could see where their ships are... that would be cheating)
        char[][] tempBoard = new char[SIZE][SIZE];
        for(int i=0; i<tempBoard.length; i++)
        {
            for(int j=0; j<tempBoard[i].length; j++)
            {
                tempBoard[i][j] = BLANK;
            }
        }

        // after tempBoard is blank, randomly generate spots until you find a valid configuration
        // NOTE: this will cause an infinite loop if the board is too small and unlucky positions
        // a 9x9 should never loop infinitly

        // for every piece
        for(int i=0; i<computerShips.length; i++)
        {
            boolean correctPlacement;

            // attempt placement until valid
            do{
                // get random coordinate and direction
                int randomColumn = (int)(Math.random()*SIZE);
                int randomRow = (int)(Math.random()*SIZE);
                int randomDirectionInt = (int)(Math.random()*4);
                String randomDirection = "";

                switch(randomDirectionInt)
                {
                    case 0:
                        randomDirection = "up";
                        break;
                    case 1:
                        randomDirection = "down";
                        break;
                    case 2:
                        randomDirection = "left";
                        break;
                    case 3:
                        randomDirection = "right";
                        break;
                    default:
                        randomDirection = "";
                        break;
                }

                // See if placement is valid
                computerShips[i].setPiece(randomRow, randomColumn, getFirstChar(randomDirection));
                correctPlacement = isValidPlacement(tempBoard, computerShips[i]);
            }while(!correctPlacement);

            // update tempBoard (so ships cannot overlap)
            addPieceToBoard(tempBoard, computerShips[i]);
        }
    }

    // Player's place piece method
    private boolean placePiece(Ship piece)
    {
        // where do you want to put it?
        System.out.println("What coordinate do you want to place the "+piece.getName().toLowerCase()+" (length "+piece.getLength()+") at?");
        String coordinate = requestString().toLowerCase();
        System.out.println("What direction do you want the piece to go? (up, down, left or right)");
        String direction = requestString().toLowerCase();

        // convert what they entered into useable information and place the piece
        // this sets the information inside Ship.java
        piece.setPiece(rowCharToInt(getFirstChar(coordinate)), getFirstInt(coordinate)-1, getFirstChar(direction));

        return isValidPlacement(playerBoard, piece); // return whether this was valid or not
    }

    // actually modify the board to display the piece
    // NOTE: does not actually change any data in the piece
    private void addPieceToBoard(char[][] board, Ship piece)
    {
        // get the top left spot
        int row = piece.getTopLeftRow();
        int column = piece.getTopLeftColumn();

        // go over the whole length
        for(int i =0; i < piece.getLength(); i++)
        {
            // update board character
            board[row][column] = piece.getBoardCharacter();

            // and continue right
            if(piece.isHorizontal())
            {
                column++;
            }
            else // or down
            {
                row++;
            }
        }
    }

    // Ask player where they want to shoot
    private String playerMove()
    {
        int attackRow;
        int attackColumn;
        String coordinate;

        // loop until they give a valid location to bombard
        // You can bombard the same coordinate twice, but that is not smart
        do
        {
            System.out.println("Which coordinate would you like to bombard?");
            coordinate = requestString().toLowerCase();
            attackRow = rowCharToInt(getFirstChar(coordinate));
            attackColumn = getFirstInt(coordinate)-1;
        }while( (attackRow < 0 || attackRow > SIZE-1) || (attackColumn < 0 || attackColumn > SIZE-1));

        // Once we know the shot we need to check every enemy ship....
        for(int k=0; k<computerShips.length; k++)
        {
            // ... and see if it hit any of it's positions
            int row = computerShips[k].getTopLeftRow();
            int column = computerShips[k].getTopLeftColumn();

            // (loop is over all the positions of each ship)
            for(int i = 0; i < computerShips[k].getLength(); i++)
            {
                // if we found a ship, we hit something! (update board)
                if(row == attackRow && column == attackColumn)
                {
                    computerBoard[attackRow][attackColumn] = HIT;
                    // check to see if this is the last hit needed to shink a ship
                    if(isSunk(computerBoard,computerShips[k]))
                    {
                        return computerShips[k].getName()+" sunk at "+getFirstChar(coordinate.toUpperCase())+getFirstInt(coordinate);
                    }
                    return "Hit at "+getFirstChar(coordinate.toUpperCase())+getFirstInt(coordinate);
                }

                // if we haven't hit yet, continue moving right
                if(computerShips[k].isHorizontal())
                {
                    column++;
                }
                else // ... or down
                {
                    row++;
                }
            }
        }

        // if we checked all ships (and every position on each ship), then we missed
        computerBoard[attackRow][attackColumn] = MISS;
        return "Miss at "+getFirstChar(coordinate.toUpperCase())+getFirstInt(coordinate);
    }

    // Computer's move
    // TODO: make the comptuer smarter!
    private String computerMove()
    {
        int attackRow=0;
        int attackColumn=0;
        String coordinate ="";
        while (!AI)
        {
        // find random position
        attackRow = (int)(Math.random()*SIZE);
        attackColumn = (int)(Math.random()*SIZE);
        
        // shoot there
        coordinate = ""+((char)(attackRow+65))+(attackColumn+1);

        // smilar logic to PlayerMove()...
        // for each ship, check each position and see if we hit/sunk or missed
        for(int k=0; k<playerShips.length; k++)
        {
            int row = playerShips[k].getTopLeftRow();
            int column = playerShips[k].getTopLeftColumn();
            for(int i = 0; i < playerShips[k].getLength(); i++)
            {
                if(row == attackRow && column == attackColumn)
                {
                    playerBoard[attackRow][attackColumn] = HIT;
                    if(isSunk(playerBoard,playerShips[k]))
                    {
                        return playerShips[k].getName()+" sunk at "+coordinate;
                    }
//                    AI = true;
                    return "Hit at "+coordinate;
                }

                if(playerShips[k].isHorizontal())
                {
                    column++;
                }
                else
                {
                    row++;
                }
            }
        }
        }
        
        playerBoard[attackRow][attackColumn] = MISS;
        return "Miss at "+coordinate;
        
       
    }

    // check to see if the game is over...
    // 0 = continue
    // 1 = you lose (you have to try hard to lose against this computer)
    // 2 = you win
    // 3 = tie
    private int gameOver()
    {
        int result = 0; // return value (indicates states listed above)

        // check to see how many total ship squares have been placed
        int computerShipCoordinateTotal = 0;
        for(int i=0; i<computerShips.length; i++)
        {
            computerShipCoordinateTotal += computerShips[i].getLength();
        }

        // same as above but for player this time, not computer
        int playerShipCoordinateTotal = 0;
        for(int i=0; i<playerShips.length; i++)
        {
            playerShipCoordinateTotal += playerShips[i].getLength();
        }


        // count up the number of actual hits on the board
        int playerBoardHitTotal = 0;
        for(int i=0; i<playerBoard.length; i++)
        {
            for(int j=0; j<playerBoard[i].length; j++)
            {
                if(playerBoard[i][j] == HIT)
                {
                    playerBoardHitTotal++;
                }
            }
        }
        // if number of hits = number of squares occupied by ships, everything is sunk
        if(playerBoardHitTotal == playerShipCoordinateTotal)
        {
            result+=1;
        }

        // again same logic but for computer
        int computerBoardHitTotal = 0;
        for(int i=0; i<computerBoard.length; i++)
        {
            for(int j=0; j<computerBoard[i].length; j++)
            {
                if(computerBoard[i][j] == HIT)
                {
                    computerBoardHitTotal++;
                }
            }
        }
        if(computerBoardHitTotal == computerShipCoordinateTotal)
        {
            result+=2;
        }

        return result;
    }

    // starts at top left and checks to see if every square is hit
    // (if it finds non-hit square, immediately stop)
    private boolean isSunk(char[][] board, Ship piece)
    {
        int row = piece.getTopLeftRow();
        int column = piece.getTopLeftColumn();
        for(int i=0; i<piece.getLength(); i++)
        {
            if(board[row][column] != HIT)
            {
                return false;
            }

            if(piece.isHorizontal())
            {
                column++;
            }
            else
            {
                row++;
            }
        }

        return true;
    }

    // don't use...
    private void displayBoards()
    {
        displayBoards(null, null, playerBoard, computerBoard);
    }

    // Use this one in the code above
    private void displayBoards(String move1, String move2)
    {
        displayBoards(move1, move2, playerBoard, computerBoard);
    }

    // used by one above
    private void displayBoards(String move1, String move2, char[][] leftBoard, char[][] rightBoard)
    {
        // first row(
        System.out.printf("%2s ", "");
        for(int i = 0; i < SIZE; i++)
        {
            System.out.printf("%2s ", i+1);
        }
        System.out.printf("%20s","");
        System.out.printf("%2s ", "");
        for(int i = 0; i < SIZE; i++)
        {
            System.out.printf("%2s ", i+1);
        }
        System.out.printf("\n");

        //rest of rows
        // (int)'A' = 65
        for(int i = 0; i < SIZE; i++)
        {
            char row = (char)(65+i);
            System.out.printf("%2s ", row);
            for(int j = 0; j < SIZE; j++)
            {
                System.out.printf("%2s ", leftBoard[i][j]);
            }
            System.out.printf("%20s","");
            System.out.printf("%2s ", row);
            for(int j = 0; j < SIZE; j++)
            {
                System.out.printf("%2s ", rightBoard[i][j]);
            }
            System.out.printf("\n");
        }

        if(move1 != null)
        {
            System.out.println("Your move:");
            System.out.println(move1);
        }
        if(move2 != null)
        {
            System.out.println("Computer's move:");
            System.out.println(move2);
        }
    }

    // magic
    private int rowCharToInt(char c)
    {
        // (int)'a' = 97
        return ((int)c) - 97;
    }

    // checks to see if in bounds and not overlapping other ships
    private boolean isValidPlacement(char[][] board, Ship piece)
    {
        int row = piece.getTopLeftRow();
        int column = piece.getTopLeftColumn();

        // check and see if the piece is actually on the board
        if(column<0 || (column+piece.getLength()-1>SIZE-1 && piece.isHorizontal()))
        {
            return false;
        }
        if(row<0 || (row+piece.getLength()-1>SIZE-1 && !piece.isHorizontal()))
        {
            return false;
        }

        // check to see if another ship is present
        for(int i =0; i < piece.getLength(); i++)
        {
            if(board[row][column] != BLANK)
            {
                return false;
            }

            if(piece.isHorizontal())
            {
                column++;
            }
            else
            {
                row++;
            }
        }

        return true;
    }

    // cause I'm lazy
    private static String requestString()
    {
        Scanner keyboardInput = new Scanner(System.in);
        String temp = keyboardInput.nextLine();

        return temp;
    }

    // String parsing....
    private static char getFirstChar(String temp)
    {
        int i=0;
        char c;
        // if the string is blank (i.e. ""), give bogus value
        // this ensure they will be prompted again
        if(temp.length()==0)
        {
            c = (char)0;
        }
        else
        {
            // skip all letters
            while(i < temp.length() && !Character.isLetter(temp.charAt(i)))
            {
                i++;
            }
            // if we found a letter, pull it out
            if(i < temp.length())
            {
                c = temp.charAt(i);
            }
            // otherwise bogus return (all non-letters)
            else
            {
                c = (char)0;
            }
        }

        return c;
    }

    // similar to one above... replace "letter" with "number"
    private static int getFirstInt(String temp)
    {        
        String number = "";
        int i =0;
        if(temp.length()==0)
        {
            return 0;
        }

        while(i < temp.length() && !Character.isDigit(temp.charAt(i)))
        {
            i++;
        }

        while(i < temp.length() && Character.isDigit(temp.charAt(i)))
        {
            number += temp.charAt(i);
            i++;
        }

        if(number.equals(""))
        {
            return 0;
        }

        return Integer.parseInt(number);
    }
}
