package lab3h;

public class RandomNumbers {

    public static int intInRange(int min, int max)
    {
        // Math.random returns a number in [0,1)
        // the (int) drops the factional part, so (int)Math.random is always zero
        // this is why we need the extra +1 which looks weird
        return min + (int)(Math.random()*(max - min + 1));
    }

}
