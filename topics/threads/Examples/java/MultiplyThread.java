public class MultiplyThread extends Thread
{
    private int a, b, result;

    public MultiplyThread(int a, int b)
    {
        this.a = a;
        this.b = b;
    }

    public void run()
    {
        this.result = this.a * this.b;

    }

    public int getResult()
    {
        return this.result;
    }
}