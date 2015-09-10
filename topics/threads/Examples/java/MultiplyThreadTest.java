import java.lang.System;
import java.lang.InterruptedException;  

public class MultiplyThreadTest
{
    public static void main(String[] args)
    {
        MultiplyThread mt = new MultiplyThread(2, 3);

        mt.start();

        try {
		    mt.join();
        } catch (InterruptedException e) {
            e.printStackTrace();
            System.exit(1);
        }

        System.out.println("The thread returned " + mt.getResult());        
   }
}