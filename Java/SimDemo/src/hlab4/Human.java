package hlab4;

import java.util.*;

// Human inherits from Player
public class Human extends Player {

    // super call's the constructor in Player
    public Human(Map world)
    {
        super(world);
    }

    // super call's the constructor in Player
    public Human(Player p)
    {
        super(p);
    }

    // list of possible actions....
    @Override
    public void doAction(char action, int time)
    {
        // get where we are
        Room location = world.getPlayerLocation();
        String locationName = location.getName();

        // keep track of the effects of our actions...
        int entertainmentChange = 0;
        int energyChange = 0;
        int smartnessChange = 0;

        // long list of options below based on time/location! yay if/else
        if(action == 'G')
        {
            travel();
            energyChange = removeEnergy(4);
        }
        else if(locationName.equals("University"))
        {
            if(8 < time && time < 16)
            {
                if(action == 'S')
                {
                    System.out.println("You put your head down and snore very loudly.");
                    energyChange = addEnergy(10);
                    smartnessChange = addSmartness(2);
                    entertainmentChange = removeEntertainment(2);
                }
                else if(action == 'A')
                {
                    System.out.println("You take copious notes and pay close attention to the material.");
                    energyChange = removeEnergy(4);
                    smartnessChange = addSmartness(8);
                    entertainmentChange = removeEntertainment(8);
                }
                else
                {
                    System.out.println("Bad command, you do nothing for an hour.");
                    energyChange = removeEnergy(2);
                    entertainmentChange = removeEntertainment(2);
                }
            }
            else
            {
                System.out.println("There are no classes in session, you waste an hour of your time.");
                energyChange = removeEnergy(4);
                entertainmentChange = removeEntertainment(3);
            }
        }
        else if(locationName.equals("Dorm"))
        {
            if(action == 'S')
            {
                if(8 < time && time < 20)
                {
                    System.out.println("Since you are diurnal, you get an hour of unrestful sleep.");
                    energyChange = addEnergy(4);
                    entertainmentChange = removeEntertainment(5);
                }
                else
                {
                    System.out.println("You get a good hour of sleep.");
                    energyChange = addEnergy(10);
                    entertainmentChange = removeEntertainment(5);
                }
            }
            else if(action == 'H')
            {
                System.out.println("You break out the books and pound through some problems.");
                energyChange = removeEnergy(3);
                smartnessChange = addSmartness(10);
                entertainmentChange = removeEntertainment(8);
            }
            else if(action == 'W')
            {
                System.out.println("OMG!! CATS!!!!");
                energyChange = removeEnergy(2);
                smartnessChange = removeSmartness(5);
                entertainmentChange = addEntertainment(20);
            }
            else
            {
                System.out.println("You stare at the ceiling for an hour...");
                energyChange = removeEnergy(1);
                entertainmentChange = removeEntertainment(3);
            }
        }
        else if(locationName.equals("Outside"))
        {
            if(action == 'S')
            {
                System.out.println("Party hard!");
                energyChange = removeEnergy(8);
                entertainmentChange = addEntertainment(8);
            }
            else if(action == 'P')
            {
                if(8 < time && time < 20)
                {
                    System.out.println("You run for almost an hour straight (when not flattened on the ground).");
                    energyChange = removeEnergy(15);
                    smartnessChange = removeSmartness(1);
                    entertainmentChange = addEntertainment(12);
                }
                else
                {
                    System.out.println("It is too dark and you end up falling flat on your face.");
                    energyChange = removeEnergy(6);
                    smartnessChange = removeSmartness(3);
                    entertainmentChange = removeEntertainment(1);
                }
            }
            else if (action == 'D')
                {
                        System.out.println("Just kdding, you do not have a date.");
                    energyChange = removeEnergy(0);
                    entertainmentChange = removeEntertainment(10);
                }
            {
                System.out.println("You wander around outside for an hour.");
                energyChange = removeEnergy(6);
                smartnessChange = addSmartness(1);
                entertainmentChange = addEntertainment(3);
            }
        }
        else
        {
            System.out.println("You are currently lost, which is not a lot of fun");
            energyChange = removeEnergy(2);
            entertainmentChange = removeEntertainment(2);
        }

        // end list of actions based on time/location

        // tell the user the effects of actions
        System.out.println("From your actions changed your stats by... " +
                "Energy: " + energyChange + ", " +
                "Entertainment: " + entertainmentChange + ", " +
                "Smartness: " + smartnessChange + ".");
    }        
}
