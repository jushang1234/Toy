package lab3h;

public class Randomonster extends Monster {

    // mighty ogre
    public Randomonster()
    {
        name = "Some Randomonster";
        hitPoints = 10;
    }

    @Override
    public int attack()
    {
        // average attack =  15
        return RandomNumbers.intInRange(5,10);
    }

}
