
package hlab4;

import java.util.Scanner;

public class SimUniversity {

    // the current time (0:00  - 23:59)
    private static int  time = 8;

    // you, the player
    private static Player player;

    public static void main(String[] args) {

        // hey! its a map
        Map world = new Map();

        // player needs to know about the map
        player = new Human(world);
        // not over yet!
        int gameOver = 0;

        while(gameOver==0)
        {
            // methods names should be informative enough
            clearScreen();
            world.displayMap();
            player.displayStats();
            displayTime();
            // get the action after showing a list of options
            char action = player.requestAction();
            System.out.println();
            // find out the effects of that action
            player.doAction(action, time);

            // process what happened and end the game if we lose
            gameOver = updatePlayer();

            // wait so the person can
            System.out.println("(Press enter to advance an hour)");
            Scanner input = new Scanner(System.in);
            input.nextLine();
        }
if (gameOver==1)
        System.out.println("Game Over!");
else
    System.out.println("you win by being absolute not entertaining! now go back to work.");
    }

    public static void displayTime()
    {
        // display the time
        System.out.printf("Current time: %2d:00.  ", time);

        // add an hour (and loop so 24:00 -> 0:00)
        time = (time+1)%24;
    }


    public static int updatePlayer()
    {

        // default: keep on player
        int gameOver = 0;


        // all work and no play...
        if(player.getEntertainment() == 0)
        {
            System.out.println("Guess what");
            gameOver = 2;
        }

        // dum dum dummmm!
        else if(player.getSmartness() == 0)
        {
            System.out.println("You drop out of college and are forced to drive taxis for the rest of your life.");
            gameOver = 1;
        }
        // too tired
        else if(player.getEnergy() == 0 && player instanceof Human)
        {
            System.out.println("You are extremely tired and become a mindless zombie until you get enough sleep!");
            player = new Zombie(player);
        }
        // back to normal
        else if(player.getEnergy() > 50 && player instanceof Zombie)
        {
            System.out.println("You are back to a functional level of sleep, whew!");
            player = new Human(player);
        }

        return gameOver;
    }

    // hack!
    public static void clearScreen()
    {
        System.out.println("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    }

}
