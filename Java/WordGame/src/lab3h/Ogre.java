package lab3h;

public class Ogre extends Monster {

    // mighty ogre
    public Ogre()
    {
        name = "Blood-thirsty ogre";
        hitPoints = 9001;
    }

    @Override
    public int attack()
    {
        // average attack =  15
        return RandomNumbers.intInRange(10, 20);
    }

}
