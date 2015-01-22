package lab3h;

import java.util.Scanner;

public class HeroineQuest {

    public static Heroine player;

    public static void main(String[] args) {
        // The "player" is who we are playing!
        player = new Heroine();

        System.out.println(player+" enters the Dark Woods of the Serpent, " +
                "armed with just her trusty fly swatter...");

        // waits for the user to hit enter (see below)
        pause();

        // The heroine does only leaves the forest in a body bag =(
        while(player.hitPoints>0)
        {            
            // combat sequence
            beginCombat();
        }

        System.out.println("Your strength is exhausted and you collapse to the ground");
    }

    public static Monster randomMonster()
    {
        // Math.random gives a random number in the range [0, 1)
        double random = Math.random();
if (random <=0.8)
        {
        return new Randomonster();
        }
else {
        
                
        // 90% of the time we get a rabbit monster
        if (random < 0.8)
        {
            return new Rabbit();
        }
        // 10% of the time we get an ogre (rawr)
        else
        {
            return new Ogre();
        }
    }
    }

    public static void pause()
    {
        Scanner anotherKeyboard = new Scanner(System.in);
        System.out.println("(Press enter to continue)");
        anotherKeyboard.nextLine();
    }

    public static void beginCombat()
    {
        // get a random monster (see the method below)
        // 90% chance rabbit, 10% chance ogre
        Monster enemy = randomMonster();

        System.out.println(player + " encounters a " + enemy + "!");

        pause();

        while(enemy.hitPoints > 0)
        {
            int yourAttack = player.attack();
            int enemyAttack = enemy.attack();

            System.out.println("The enemy smacks " + player
                    + " for " + enemyAttack + " points of damage!");
            player.takeDamage(enemyAttack);
            
            if(player.hitPoints <= 0)
            {
                return;
            }

            System.out.println("You have " + player.hitPoints + " hit points left!");

            
            System.out.println("You swat the " + enemy
                    + " for " + yourAttack + " points of damage!");
            enemy.takeDamage(yourAttack);

            pause();
        }
        
        System.out.println("You have slain the mighty " + enemy
                + " and continue onwards.");

        pause();
    }

}
