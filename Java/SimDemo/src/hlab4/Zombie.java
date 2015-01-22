package hlab4;

import java.util.*;

public class Zombie extends Player{

    // too lazy to comment this, see Human.java
    // only difference here is some actions are not possible

    public Zombie(Map world)
    {
        super(world);
    }

    public Zombie(Player p)
    {
        super(p);
    }

    @Override
    public void doAction(char action, int time)
    {
        Room location = world.getPlayerLocation();
        String locationName = location.getName();
        int entertainmentChange = 0;
        int energyChange = 0;
        int smartnessChange = 0;

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
                    System.out.println("You try to focus but you are too tired.");
                    energyChange = removeEnergy(3);
                    smartnessChange = addSmartness(1);
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
                System.out.println("You try to study but you end up staring at the paper (and nodding off).");
                energyChange = removeEnergy(1);
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
                System.out.println("You are unable to form complete sentances and people ignore you.");
                energyChange = removeEnergy(4);
                entertainmentChange = removeEntertainment(4);
            }
            else if(action == 'P')
            {                
                System.out.println("You show up and just sit on the sidelines.");
                energyChange = removeEnergy(2);
                entertainmentChange = removeEntertainment(1);
            }
            else
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

        System.out.println("From your actions changed your stats by... " +
                "Energy: " + energyChange + ", " +
                "Entertainment: " + entertainmentChange + ", " +
                "Smartness: " + smartnessChange + ".");
    }

}
