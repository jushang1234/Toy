package lab3h;

public class Rabbit extends Monster{

    // (not so) mighty rabbit
    public Rabbit()
    {
        name = "Rabbit";
        hitPoints = 3;
    }

    @Override
    public int attack()
    {
        // average attack = 0.5
        return RandomNumbers.intInRange(0, 1);
    }
}
