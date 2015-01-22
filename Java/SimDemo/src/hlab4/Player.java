package hlab4;

import java.util.*;

public abstract class Player {

    // character has 3 stats: entertainment, energy and smartness
    protected Map world = null;
    protected int entertainment;
    protected int energy;
    protected int smartness;

    // start out at 70 in each stat
    public Player()
    {
        entertainment = 70;
        energy = 70;
        smartness = 70;
    }

    // hopefully you exist in a world...
    public Player(Map world)
    {
        this();

        this.world = world;
    }

    // used when changing between Zombie and Human state to copy current stat levels
    public Player(Player other)
    {
        entertainment = other.entertainment;
        energy = other.energy;
        smartness = other.smartness;
        world = other.world;
    }

    // Tell them where they are and what actions they can take (and ask them as the method says)
    public char requestAction()
    {
        // get where you are
        Room location = world.getPlayerLocation();

        // display where you are
        System.out.println("You are at the " + location.getName() + ".");

        // if at uni
        if(location.getName().equals("University"))
        {
            System.out.println("Do you want to (A)ttend class or (S)leep through lecture?");
        }
        // if at dorm
        else if(location.getName().equals("Dorm"))
        {
            System.out.println("Do you want to (S)leep, do (H)omeowrk or (W)atch some YouTube?");
        }
        // if outside
        else if(location.getName().equals("Outside"))
        {
            System.out.println("Do you want to (S)ocialize, (D)ate or (P)lay Rugby?");
        }
        // if you are not in any of the above, we have a problem
        else if(location.getName().equalsIgnoreCase("Toliet"))
        {
            System.out.println("DO NOT PEE ON YOUR PANTS!");

        }

        else
        {
            System.out.println("You are lost in the abyss...");
            
        }
        // they can also change locations by 'G'
        System.out.println("Do you want to (G)o to a different location?");

        // make a scanner and let them enter a choice
        Scanner input = new Scanner(System.in);
        String answer = input.nextLine();

        // if they entered something, give back the first character (capitalized)
        if(answer.length() > 0)
        {
            return answer.toUpperCase().charAt(0);
        }
        // otherwise they just hit enter (trying to crash me!), so they do nothing
        else
        {
            return ' ';
        }
    }

    // chage locations
    public void travel()
    {
        // destionations the names of possible destinations
        ArrayList<Character> destinations = new ArrayList<Character>();

        // to do this, we need to copy in each name manually
        for(Room place : world.getLocations())
        {
            destinations.add(new Character(place.getLetter()));
        }

        // show the possible destinations
        System.out.println("Where do you want to travel to: " + destinations + "?");

        // read from the keyboard where they want to go
        Scanner input = new Scanner(System.in);        
        String response = input.nextLine();
        // if they didn't enter anything (shame on them) they go nowhere
        if(response.length() == 0)
        {
            System.out.println("You go nowhere...");
            return;
        }
        // otherwise pull out the first character
        char r = response.toUpperCase().charAt(0);

        // find the room that matches this character
        Room destination = null;
        for(Room place : world.getLocations())
        {
            if(place.getLetter() == r)
            {
                destination = place;
            }
        }

        // if we didnt find the room, they entered a bad character!
        if(destination == null)
        {
            System.out.println("Invalid location, you go nowhere...");
        }
        // otherwise go to that location
        else
        {
            System.out.println("You go to " + destination.getName() + ".");
            world.moveToRoom(destination);            
        }
    }


    // display stats... yes what the methods says it does...
    public void displayStats()
    {
        System.out.println("Current stats are: " +
                "Energy = " + energy +", " +
                "Entertainment = " + entertainment +", " +
                "Smartness = " + smartness +".");
    }
    
    public abstract void doAction(char action, int time);

    // !!! WARNING !!! proceed past here at your own risk
    // below are methods for adding, removing and getting the value the 3 stats

    // get the value of entertainment
    public int getEntertainment()
    {
        return entertainment;
    }

    // adds amount to "entertainment" and returns the change
    public int addEntertainment(int amount)
    {
        // remember the old value
        int oldEntertainment = entertainment;

        // add the entertainment amount
        entertainment = entertainment + amount;

        // don't let entertainment go over 100
        entertainment = Math.min(entertainment, 100);

        return entertainment - oldEntertainment;
    }

    //subtracts amount from "entertainment" and returns the change
    public int removeEntertainment(int amount)
    {
        // remember the old value
        int oldEntertainment = entertainment;

        // add the entertainment amount
        entertainment = entertainment - amount;

        // don't let entertainment go under 0
        entertainment = Math.max(entertainment, 0);

        return entertainment - oldEntertainment;
    }


    // get the value of energy
    public int getEnergy()
    {
        return energy;
    }

    // adds amount to "energy" and returns the change
    public int addEnergy(int amount)
    {
        // remember the old value
        int oldEnergy = energy;

        // add the energy amount
        energy = energy + amount;

        // don't let energy go over 100
        energy = Math.min(energy, 100);

        return energy - oldEnergy;
    }

    //subtracts amount from "energy" and returns the change
    public int removeEnergy(int amount)
    {
        // remember the old value
        int oldEnergy = energy;

        // add the energy amount
        energy = energy - amount;

        // don't let energy go under 0
        energy = Math.max(energy, 0);

        return energy - oldEnergy;
    }

    // get the value of entertainment
    public int getSmartness()
    {
        return smartness;
    }

    // adds amount to "smartness" and returns the change
    public int addSmartness(int amount)
    {
        // remember the old value
        int oldSmartness = smartness;

        // add the smartness amount
        smartness = smartness + amount;

        // don't let smartness go over 100
        smartness = Math.min(smartness, 100);

        return smartness - oldSmartness;
    }

    //subtracts amount from "smartness" and returns the change
    public int removeSmartness(int amount)
    {
        // remember the old value
        int oldSmartness = smartness;

        // add the smartness amount
        smartness = smartness - amount;

        // don't let smartness go under 0
        smartness = Math.max(smartness, 0);

        return smartness - oldSmartness;
    }
}
