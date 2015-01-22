package lab3h;
import java.util.Scanner;
// this is you!
public class Heroine extends Monster{

    public Heroine()
    {
        System.out.println("Here should be your name...");
        Scanner    keyboardInput = new Scanner(System.in);
        name = keyboardInput.next();
        hitPoints = 20;
    }

    @Override
    public int attack() // swat them with your fly-swatter!
    {
        return RandomNumbers.intInRange(0, 4);
    }

}
