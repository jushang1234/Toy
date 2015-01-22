package lab3h;

public abstract class Monster { // generic monster type

    // Every good monster has a name
    public String name;

    // and some hit points... since you are going to swat them
    public int hitPoints;

    // how much damage does the monster do?
    public abstract int attack();

    // modify the monster's hit points when you attack it
    public void takeDamage(int amount)
    {
        hitPoints = hitPoints - amount;
    }

    // override toString to make it easier to show names in println
    @Override
    public String toString()
    {
        return name;
    }
}
